#include <dos.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define COLOR_COUNT 7 //���������� ������

struct VIDEO
{
	unsigned char symbol;
	unsigned char attribute;
};

unsigned char colors[COLOR_COUNT] = { 0x71,0x62,0x43,0x54,0x35,0x26,0x17 }; //������ ������ ��� �����������
char color = 0x89;	//����������� ����;

void changeColor() //��������� ����� �� ���������
{
	color = colors[rand() % COLOR_COUNT];
	return;
}

void print()
{
	char temp;  //������� ���
	int i, val;
	VIDEO far* screen = (VIDEO far*)MK_FP(0xB800, 0); //�������� ����������

	val = inp(0x21);  //�������� ������� ����� �������� �����������
	for (i = 0; i < 8; i++) //������ �� ��������
	{
		temp = val % 2; //�������� ��������� ���
		val = val >> 1; //�������� �� ���� ��� ������
		screen->symbol = temp + '0'; //��������� ��� ��� ������ ('0' ��� '1')
		screen->attribute = color; //���������� ����
		screen++; //� ���������� �������
	}
	screen++; //������

	val = inp(0xA1); //�������� ������� ����� �������� �����������

	for (i = 0; i < 8; i++) //������ �� ��������
	{
		temp = val % 2; //�������� ��������� ���
		val = val >> 1; //�������� �� ���� ��� ������
		screen->symbol = temp + '0'; //��������� ��� ��� ������ ('0' ��� '1')		screen->attribute = color;	 //���������� ����
		screen++; //� ���������� �������
	}
	screen += 63;	 //������� �� ��������� ������
	outp(0x20, 0x0A); //������������ �� ������� �������� �������� �����������

	val = inp(0x20); //�������� ������� �������� �������� �����������         
	for (i = 0; i < 8; i++) //������ �� ��������	
	{
		temp = val % 2; //�������� ��������� ���
		val = val >> 1; //�������� �� ���� ��� ������
		screen->symbol = temp + '0'; //��������� ��� ��� ������ ('0' ��� '1')
		screen->attribute = color; //���������� ����
		screen++; //� ���������� �������
	}
	screen++; //������

	outp(0xA0, 0x0A); //������������ �� ������� �������� �������� �����������
	val = inp(0xA0); //�������� ������� �������� �������� �����������         	for (i = 0; i < 8; i++) //������ �� ��������
	{
		temp = val % 2; //�������� ��������� ���
		val = val >> 1; //�������� �� ���� ��� ������
		screen->symbol = temp + '0'; //��������� ��� ��� ������ ('0' ��� '1')		screen->attribute = color;	 //���������� ����
		screen++; //� ���������� �������
	}
	screen += 63;	 //������� �� ��������� ������

	outp(0x20, 0x0B); //������������ �� ������� ������������ �������� �����������	val = inp(0x20); //�������� ������� ������������ �������� �����������         

	for (i = 0; i < 8; i++) //������ �� ��������	
	{
		temp = val % 2; //�������� ��������� ���
		val = val >> 1; //�������� �� ���� ��� ������
		screen->symbol = temp + '0'; //��������� ��� ��� ������ ('0' ��� '1')
		screen->attribute = color; //���������� ����
		screen++; //� ���������� �������
	}
	screen++; //������

	outp(0xA0, 0x0B); //������������ �� ������� ������������ �������� �����������
	val = inp(0xA0); //�������� ������� ������������ �������� �����������  
	for (i = 0; i < 8; i++) //������ �� ��������
	{
		temp = val % 2; //�������� ��������� ���
		val = val >> 1; //�������� �� ���� ��� ������
		screen->symbol = temp + '0'; //��������� ��� ��� ������ ('0' ��� '1')		screen->attribute = color;	 //���������� ����
		screen++; //� ���������� �������
	}
}

//IRQ 0-7
void interrupt(*oldint8) (...);  // IRQ 0 - ������
void interrupt(*oldint9) (...);  // IRQ 1 - ����������
void interrupt(*oldint10) (...); // IRQ 2 � ��������� ����������� ������� �����������
void interrupt(*oldint11) (...); // IRQ 3 � ���������������� ���� COM 2
void interrupt(*oldint12) (...); // IRQ 4 � ���������������� ���� COM 1
void interrupt(*oldint13) (...); // IRQ 5 � ������������ ���� LPT2
void interrupt(*oldint14) (...); // IRQ 6 � ���������� ������-����������
void interrupt(*oldint15) (...); // IRQ 7 � ������������ ���� LPT1

//IRQ 8-15
void interrupt(*oldint70) (...); // IRQ 8 � ���� ��������� �������
void interrupt(*oldint71) (...); // IRQ 9 � ���������
void interrupt(*oldint72) (...); // IRQ 10 � ���������� �������������
void interrupt(*oldint73) (...); // IRQ 11 - ���������
void interrupt(*oldint74) (...); // IRQ 12 � ���� PS/2
void interrupt(*oldint75) (...); // IRQ 13 � �������������� �����������
void interrupt(*oldint76) (...); // IRQ 14 � ������ ���������� ������� �����
void interrupt(*oldint77) (...); // IRQ 15 � ������ ���������� ������� �����

/*����� ����������*/
void interrupt  newint08(...) { changeColor(); print(); oldint8(); } // int8
void interrupt  newint09(...) { changeColor(); print(); oldint9(); } //int9
void interrupt  newint0A(...) { changeColor(); print(); oldint10(); } //int10
void interrupt  newint0B(...) { changeColor(); print(); oldint11(); } //int11
void interrupt  newint0C(...) { changeColor(); print(); oldint12(); } //int12
void interrupt  newint0D(...) { changeColor(); print(); oldint13(); } //int 13
void interrupt  newint0E(...) { changeColor(); print(); oldint14(); } //int 14
void interrupt  newint0F(...) { changeColor(); print(); oldint15(); } //int 15																							
void interrupt  newintC0(...) { changeColor(); print(); oldint70(); } //int70
void interrupt  newintC1(...) { changeColor(); print(); oldint71(); } //int71
void interrupt  newintC2(...) { changeColor(); print(); oldint72(); } //int72
void interrupt  newintC3(...) { changeColor(); print(); oldint73(); } //int73
void interrupt  newintC4(...) { changeColor(); print(); oldint74(); } //int74
void interrupt  newintC5(...) { changeColor(); print(); oldint75(); } //int75
void interrupt  newintC6(...) { changeColor(); print(); oldint76(); } //int76
void interrupt  newintC7(...) { changeColor(); print(); oldint77(); } //int77

void initialize()
{
	//IRQ 0-7
	oldint8 = getvect(0x08);  // IRQ  0
	oldint9 = getvect(0x09);  // IRQ  1
	oldint10 = getvect(0x0A); // IRQ  2
	oldint11 = getvect(0x0B); // IRQ  3
	oldint12 = getvect(0x0C); // IRQ  4
	oldint13 = getvect(0x0D); // IRQ  5
	oldint14 = getvect(0x0E); // IRQ  6
	oldint15 = getvect(0x0F); // IRQ  7

	  //IRQ 8-15
	oldint70 = getvect(0x70); // IRQ  8
	oldint71 = getvect(0x71); // IRQ  9
	oldint72 = getvect(0x72); // IRQ 10 
	oldint73 = getvect(0x73); // IRQ 11 
	oldint74 = getvect(0x74); // IRQ 12
	oldint75 = getvect(0x75); // IRQ 13
	oldint76 = getvect(0x76); // IRQ 14
	oldint77 = getvect(0x77); // IRQ 15

	/*setvect �������� ����� ��������� ��������� ����������, �� ������� ��������� (newint08), � ������� �������� ����������, ��������� ������� (0xD8)*/

	//��������������� IRQ 0-7
	setvect(0xD8, newint08);
	setvect(0xD9, newint09);
	setvect(0xDA, newint0A);
	setvect(0xDB, newint0B);
	setvect(0xDC, newint0C);
	setvect(0xDE, newint0D);
	setvect(0xDF, newint0E);
	setvect(0xE1, newint0F);

	//��������������� IRQ8-15
	setvect(0x08, newintC0);
	setvect(0x09, newintC1);
	setvect(0x0A, newintC2);
	setvect(0x0B, newintC3);
	setvect(0x0C, newintC4);
	setvect(0x0D, newintC5);
	setvect(0x0E, newintC6);
	setvect(0x0F, newintC7);

	_disable(); // ������ ���������� (Clear Interrupt Flag)

/*������ outp ������� ����, ����������� (0x11) � ����, ������� ������������ ���������� (0x20)*/
	//������������� ���������� ��� �������� �����������
	outp(0x20, 0x11);	//ICW1 � ������������� �������� �����������
	outp(0x21, 0xD8);	//ICW2 � ������� ������ ��� ��������
	outp(0x21, 0x04);	//ICW3 � ���� ��� �������� ����������� (� �������� �������)
	outp(0x21, 0x01);	//ICW4 � �� ���������

		//������������� ���������� ��� �������� �����������
	outp(0xA0, 0x11);  	//ICW1 � ������������� �������� �����������
	outp(0xA1, 0x08);	//ICW2 � ������� ������ ��� ��������
	outp(0xA1, 0x02);	//ICW3 � ���������� ������������ ������ � ��������
	outp(0xA1, 0x01);	//ICW4 � �� ���������

	_enable(); // ������������� ����������� ����������
}

int main()
{
	unsigned far* fp; //��������� ���������
	initialize();
	system("cls");
	printf("                   - mask\n"); //������� ����� �������� � �������� ������������
	printf("                   - prepare\n"); //������� �������� �������� � ��������
	printf("                   - service\n"); //������� ������������ �������� � ��������
	printf("Master   Slave\n");

	FP_SEG(fp) = _psp; //�������� �������
	FP_OFF(fp) = 0x2c; //�������� �������� �������� ������ � ����������� �����
	_dos_freemem(*fp); //������������ ������ ��� ���

	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1); //��������� �����������, ������ �������� � ��� ����������, ������ � ����� ������, ������� ������ ���� �������������� ��� ��������� ����� � ����������
	return 0;
}
