#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

unsigned int notes[] = { 196, 261, 329, 196, 261, 329, 196, 261, 329 };
unsigned int note_delay = 400;

void PlaySound();
void StateWords();
void CharToBin(unsigned char state, char* str);
void TurnSpeaker(int isActive);
void SetCount(int iDivider);
void Menu();
void CE_inp_convert(unsigned char high, unsigned char low);

int main() {
	Menu();
	return 0;
}

void Menu() {
	int choice = 0;
	while (1) {
		system("cls");
		printf("1 - Play sound");
		printf("\n2 - Print channels state words");
		printf("\n0 - Exit");

		printf("\n\nEnter choice: ");
		scanf("%d", &choice);
		if (choice >= 0 && choice <= 2) {
			switch (choice) {
			case 0:
				return;

			case 1:
				PlaySound();
				break;

			case 2:
				StateWords();
				printf("\n\nPress any key to continue: ");
				scanf("%d", &choice);
				break;
			}
		}
	}
}

//функция считывающая слова состояния каналов
void StateWords()
{
	char* bin_state;
	int iChannel;
	unsigned char state;
	bin_state = (char*)calloc(9, sizeof(char));
	if (bin_state == NULL)
	{
		printf("Memory allocation error");
		exit(EXIT_FAILURE);
	}

	for (iChannel = 0; iChannel < 3; iChannel++)
	{
		switch (iChannel)
		{
		case 0:
		{
			outp(0x43, 0xE2); //заносим управляющее слово, 
			//соответствующее команде RBC (Чтение состояния канала) и номеру канала 0
			state = inp(0x40); //чтение слова состояния канала 0
			CharToBin(state, bin_state);
			printf("\nChannel 0x40 word: %s\n", bin_state);
			unsigned char low_bits = inp(0x40);
			unsigned char high_bits = inp(0x40);
			CE_inp_convert(high_bits, low_bits);
			break;
		}
		case 1:
		{
			bin_state[0] = '\0';
			outp(0x43, 0xE4); //заносим управляющее слово, 
			//соответствующее команде RBC (Чтение состояния канала) и номеру канала 1
			state = inp(0x41); //чтение слова состояния канала 1
			CharToBin(state, bin_state);
			printf("\nChannel 0x41 word: %s\n", bin_state);
			unsigned char low_bits = inp(0x41);
			unsigned char high_bits = inp(0x41);
			CE_inp_convert(high_bits, low_bits);
			break;
		}
		case 2:
		{
			bin_state[0] = '\0';
			outp(0x43, 0xE8); //заносим управляющее слово, 
			//соответствующее команде RBC (Чтение состояния канала) и номеру канала 2
			state = inp(0x42); //чтение слова состояния канала 2
			CharToBin(state, bin_state);
			printf("\nChannel 0x42 word: %s\n", bin_state);
			unsigned char low_bits = inp(0x42);
			unsigned char high_bits = inp(0x42);
			CE_inp_convert(high_bits, low_bits);
			break;
		}
		}
	}
	free(bin_state);
	return;
}

void CE_inp_convert(unsigned char high, unsigned char low)
{
	char* CE_response = (char*)malloc(5 * sizeof(char));
	int i = 3;
	while (i > 1)
	{
		char tmp1 = high & 15;
		if (tmp1 >= 10)
		{
			CE_response[i - 2] = 'A' + (tmp1 - 10);
		}
		else
		{
			CE_response[i - 2] = '0' + tmp1;
		}

		char tmp2 = low & 15;
		if (tmp2 >= 10)
		{
			CE_response[i] = 'A' + (tmp2 - 10);
		}
		else
		{
			CE_response[i] = '0' + tmp2;
		}

		i--;
		high >>= 4;
		low >>= 4;
	}

	CE_response[4] = 0;

	printf("CE register: 0x%s\n", CE_response);
}

//функция перевода в двоичный код
void CharToBin(unsigned char state, char* str)
{
	int i, j;
	char temp;
	for (i = 7; i >= 0; i--)
	{
		temp = state % 2;
		state /= 2;
		str[i] = temp + '0';
	}
	str[8] = '\0';
}

//функция установки значения счетчика
void SetCount(int iDivider) {
	long base = 1193180; //максимальная частота
	long kd;
	outp(0x43, 0xB6); //10110110 - канал 2, операция 4, режим 3, формат 0
	kd = base / iDivider;
	outp(0x42, kd % 256); // младший байт делителя
	kd /= 256;
	outp(0x42, kd); //старший байт делителя
	return;
}

//функция работы с громкоговорителем
void TurnSpeaker(int isActive) {
	if (isActive) {
		outp(0x61, inp(0x61) | 3); //устанавливаем 2 младших бита 11
		return;
	}
	else {
		outp(0x61, inp(0x61) & 0xFC); //устанавливаем 2 младших бита 00
		return;
	}
}

//функция воспроизведения песни
void PlaySound() {
	for (int i = 0; i < 9; i++) {
		SetCount(notes[i]);
		TurnSpeaker(1); //включаем громкоговоритель
		delay(note_delay); //устанавливаем длительность мс
		TurnSpeaker(0); //выключаем громкоговоритель
	}
}