#include <iostream>
#include <random>
#include <time.h>

const int COUNT_CYCLE = 10000000;    //Кол-во итераций цикла
const int SIZE = 4;                  //Размер используемыхь матриц
const int RAND = 10000;              //Диапазон рандомайзера

using namespace std;

//Функция ввывода содержимого матрицы на экран
void output(int ms[SIZE][SIZE])
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			cout << ms[i][j] << ' ';
		}
		cout << endl;
	}
}

//Функция обнуления содержимого матрицы
void null(int ms[SIZE][SIZE])
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			ms[i][j] = 0;
		}
	}
}

//Заполнение содержимого матрицы случайными числами в диапазоне <RAND>
void rand(int ms[SIZE][SIZE])
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			ms[i][j] = rand() % RAND;
		}
	}
}

int main()
{
	srand(time(NULL));
	clock_t start;                              //Время начала отсчета
	clock_t end;                                //Время конца отсчета
	int ms1[SIZE][SIZE];
	int ms2[SIZE][SIZE];
	int result_ms[SIZE][SIZE] = { 0 };          //Результирующая матрица

	rand(ms1);
	rand(ms2);

	cout << "Number of iterations: " << COUNT_CYCLE << endl;
	cout << "Random range: " << RAND << endl << endl;
	// C
	start = clock();
	for (int i = 0; i < COUNT_CYCLE; i++)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result_ms[i][j] = ms1[i][j] - ms2[i][j];
			}
		}
	}
	end = clock();
	//output(result_ms);
	cout << "C worktime is: " << (double)(end - start) / CLOCKS_PER_SEC << endl;
	null(result_ms);

	// ASM
	start = clock();
	for (int i = 0; i < COUNT_CYCLE; i++)
	{
		_asm {
			xor esi, esi          //Обнуляем индекс
			xor eax, eax	      //Значение разности
			mov ebx, 16           //Масив 4х4

			cycleAsm:
			mov eax, ms1[esi]
				sub eax, ms2[esi]
				mov result_ms[esi], eax
				add esi, 4

				dec ebx           //Уменьшаем колиество проходов
				cmp ebx, 0        //Сравниваем с нулем
				jne cycleAsm
		}
	}
	end = clock();
	//output(result_ms);
	cout << "ASM worktime is: " << (double)(end - start) / CLOCKS_PER_SEC << endl;

	// MMX
	null(result_ms);
	start = clock();
	for (int i = 0; i < COUNT_CYCLE; i++)
	{
		_asm {
			pusha
			xor esi, esi                   //Обнуляем индекс
			mov ebx, 8                     //Количество проходов по циклу

			cycleMMX:
			movq MM0, ms1[esi]             //Заносим значение в регистр MM0
				psubq MM0, ms2[esi]        //Вычитаем значение другого массива
				movq result_ms[esi], MM0   //Записываем значение в массив
				add esi, 8                 //Передвигаем указатель

				dec ebx                    //Уменьшаем количество проходов по циклу
				cmp ebx, 0                 //Сравниваем значение с 0
				jne cycleMMX
				emms
				popa
		}
	}
	//output(result_ms);
	end = clock();
	cout << "MMX worktime is: " << (double)(end - start) / CLOCKS_PER_SEC << endl << endl;
	system("pause");
	return 0;
}