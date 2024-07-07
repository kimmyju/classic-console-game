#include<stdio.h>
#include <Windows.h>
#include "Maze.h"
#include "AnyPang.h"
#include "omok.h"
#include "Chess.h"
#define Success 1

void printMainMenu();
void GameMenu();
void MColorSet(unsigned short BackG, unsigned short Text);
void mainInput(int* s);
void LoadScore();

int main()
{
    int select;
    PlaySound(TEXT("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\BackGroundBgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
    ReadData();
    do
    {
        system("cls");
        printMainMenu();

        MColorSet(0, 4);
        printf("\nchoice �� ");
        mainInput(&select);
        ColorSet(0, 15);

        switch (select)
        {
        case 1:
            GameMenu();
            break;
        case 2: 
            LoadScore();
            break;
        case 3:
            return 0;
        default:
            printf("\n�߸��� �Է��Դϴ�. �ٽ� �Է��ϼ���");
            break;
        }
    } while (1);
}

void printMainMenu()
{
    MColorSet(0, 6);
    printf("\n���������� MAIN MENU ����������\n");
    MColorSet(0, 15);
    printf("\n\n1 PLAY GAME");
    printf("\n\n2 GAME RECORDS");
    printf("\n\n3 EXIT\n\n");

}

void GameMenu()
{
    int select;
    do
    {
        system("cls");
        MColorSet(0, 6);
        printf("\n���������� SELECT YOUR MINI GAME ����������\n");
        MColorSet(0, 15);
        printf("\n\n1 MAZE ");
        printf("\n\n2 ANYPANG ");
        printf("\n\n3 OMOK ");
        printf("\n\n4 CHESS ");
        printf("\n\n5 BACK TO MAIN \n\n");

        MColorSet(0, 4);
        printf("\nchoice �� ");
        MColorSet(0, 15);
        mainInput(&select);
        switch (select)
        {
        case 1:
            system("cls");
            Maze();
            PlaySound(TEXT("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\BackGroundBgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
            break;
        case 2:
            system("cls");
            AnyPang();
            PlaySound(TEXT("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\BackGroundBgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
            break;
        case 3:
            system("cls");
            Omok();
            PlaySound(TEXT("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\BackGroundBgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
            break;
        case 4:
            system("cls");
            Chess();
            PlaySound(TEXT("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\BackGroundBgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
            break;
        case 5:
            return;
        default:
            printf("\n�߸��� �Է��Դϴ�. �ٽ� �Է��ϼ���");
            continue;
        }
    } while (1);
}

void MColorSet(unsigned short back, unsigned short text) //���� ����ϱ� ���� �Լ�
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text | back << 4);
}

void mainInput(int* s)
{
    scanf_s("%d", s);
}

void LoadScore()
{
    AnyReadData();
    ReadData();

    system("cls");
    printf("\n�̷� ���� BestScore < %d >", bestScore);
    printf("\n\n�ִ��� ���� BestScore < %d >", AnyBestScore);
    Sleep(5000);
}