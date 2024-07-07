#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>                                    // time rand ���� �ޱ� ���� ���̺귯��
#include <stdlib.h>                                  // ���� ������ ���� ���̺귯��
#include <windows.h>                                 // �ܼ�â�� ����� �̷��� ������ ���� ���̺귯��
#include <stdbool.h>                                 // bool Ÿ�� ���� ����� ���� ���̺귯��
#include <conio.h>                                   // ����Ű�� �Է��� �޾� �÷��̾ �����̱� ���� ���̺귯��
#include <Windows.h>                                 //���� ����� ���� �������
#include <mmsystem.h>                                // ���� ����� ���� �������
#pragma comment(lib,"winmm.lib")                     //���� ���
#define Size 10                                      // �� ������

#pragma region APlayerDirection      // �÷��̾��� ����Ű ����
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27
#define SPACE 32
#define ENTER 13
#pragma endregion

typedef enum _aTileAtype
{
    ANone = 0,
    AWall = 1,
    ABlock = 2,
};

typedef enum _aShape
{
    _ANone = 0,
    Spade = 1,                                  // ��   -> �Ķ���
    Diamond = 2,                                // ��   -> �����
    Heart = 3,                                  // ��   -> ��ȫ��
    Clover = 4,                                 // ��   -> �ʷϻ�
    Square = 5,
};

struct AT
{
    int x, y;
    enum _aTileAtype Atype;
    enum _aShape shape;
};

struct _APlayer
{
    int AposY, APosX;
};

#pragma region Tiles
struct AT ATile[Size][Size] = { 0 };             // Ÿ�� ���� �迭
int selectX = -1;                                // �÷��̾ ù ��°�� ������ Ÿ���� x ��ǥ ���� ����
int selectY = -1;                                // �÷��̾ ù ��°�� ������ Ÿ���� y ��ǥ ���� ����
struct _APlayer Aplayer = { 1, 1 };
int AnyBestScore = 0;
int AnyScore = 0;
time_t start, end;
#pragma endregion

void CreateMap();
void randShape(int num, bool color);
void Anygotoxy(int x, int y);
int APlay();
void SwapTile();
void Crash();
void LetDown();
void FillShape();
void PrintScore();
void AnyReadData();
void AnyWriteData();
void APrintGameStart();


void AnyPang()
{
    int GameS;
    SetConsoleTitle(TEXT("�ִ���"));

    printf("\n< ���� �� >\n\n\n");
    printf("1. ����Ű�� �÷��̾ �� ���� ĭ�� �����Ѵ�.\n\n");
    printf("2. �÷��̾ ������ ĭ�� ���� ��ġ�� �ٲ۴�.\n\n");
    printf("3. ���ӵ� ����� ���� Ȥ�� ���η� 3�� �̻������� �ش� ĭ�� �μ�����.\n\n");
    printf("4. 25�ʰ� �μ��� ĭ�� ���� ������ ȹ���ϴ� ����");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    printf("\n\n\nStart Game>  Enter");

    GameS = _getch();
    if (GameS == ENTER)
    {
        system("cls");
        PlaySound(TEXT("C:C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\AnyPangBgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

        APrintGameStart();
        Sleep(1000);
        system("cls");

        CreateMap();
        PrintScore();
        time(&start);

        while ((int)(time(&end) - start) <= 25)
        {
            PrintScore();
            APlay();
        }
    }
}

void APrintGameStart()                            // �� ����� �� GAME START �۾� ǥ��
{
    Anygotoxy(6, 7);
    printf(" ##  #  # # ###      ## ###  #  ##  ###  #");

    Anygotoxy(6, 8);
    printf("#   # # ### #       #    #  # # # #  #   #");

    Anygotoxy(6, 9);
    printf("# # ### ### ##       #   #  ### ##   #   #");

    Anygotoxy(6, 10);
    printf("# # # # # # #         #  #  # # # #  #    ");

    Anygotoxy(6, 11);
    printf(" ## # # # # ###     ##   #  # # # #  #   #");
    Anygotoxy(10, 21);
}

void CreateMap()
{
    int y, x;
    srand(time(NULL));


    for (y = 0; y < Size; y++)
    {
        for (x = 0; x < Size; x++)
        {
            if (y == 0 || x == 0 || x == Size - 1 || y == Size - 1)
            {

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                ATile[y][x].Atype = AWall;
                ATile[y][x].shape = Square;
                printf("��");
            }
            else
            {
                ATile[y][x].Atype = ABlock;
                ATile[y][x].shape = (rand() % 4 + 1);                   // �����̵�, ���̾�, ��Ʈ, Ŭ�ι� �� �������� ��� ����
                randShape(ATile[y][x].shape, 1);                        // ������ ��� ���
            }
        }
        printf("\n");
    }
}

void randShape(int num, bool color)                                     // bool �ڷ����� color ������ �� 0�� ���� ��, �ƴҶ�(1)�� ��� (APlayer�� �̵� ��δ� ���)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);       // �⺻���� ��� ���
    switch (num)
    {
    case Spade:
        if (color)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
        printf("��");
        break;

    case Diamond:
        if (color)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        printf("��");
        break;

    case Heart:
        if (color)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        printf("��");
        break;

    case Clover:
        if (color)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        printf("��");
        break;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                         // �� ������� �ʱ�ȭ
}

int APlay()
{
    Anygotoxy(1, 1);                                                                     // APlayer �ʱ� ��ġ ����

    while ((int)(time(&end) - start) <= 25)                                              // 15�� ���� �ݺ�
    {
        // Ŀ�� ��ġ�� �÷��̾� ��ġ�� �ٲ�
        if (_kbhit())                                                                    // Ű���尡 ������ if �� ������
        {
            int key = _getch();

            if (key == 224)                                                              // getch() �Լ��� 1�� ��ȯ�� ����   (��, space�� ��� 1������ �ٷ� �����ϰ� ����Ű�� ��쿡�� 2�� ��ȯ������ ����)
            {
                key = _getch();
                switch (key)                                                             // 1. getch() �Լ��� 2�� ��ȯ�� �����ؼ� ������ ����
                {
                case LEFT:
                    if (!(Aplayer.APosX == selectX && Aplayer.AposY == selectY))         // ** �÷��̾ �ٲٷ��� ������ ĭ�� �ƴϸ�
                        randShape(ATile[Aplayer.AposY][Aplayer.APosX].shape, 1);         // 2. ������ �ִ� ĭ �ٽ� �÷��� �������
                    Aplayer.APosX = (Aplayer.APosX + 6) % (Size - 2) + 1;                // 3. �̵��� ��ǥ ���
                    Anygotoxy(Aplayer.APosX, Aplayer.AposY);                             // 4. Ŀ�� �̵�   
                    randShape(ATile[Aplayer.AposY][Aplayer.APosX].shape, 0);             // 5. �÷��̾� ��ġ ǥ��
                    break;
                case RIGHT:
                    if (!(Aplayer.APosX == selectX && Aplayer.AposY == selectY))
                        randShape(ATile[Aplayer.AposY][Aplayer.APosX].shape, 1);
                    Aplayer.APosX = (Aplayer.APosX + 8) % (Size - 2) + 1;
                    Anygotoxy(Aplayer.APosX, Aplayer.AposY);
                    randShape(ATile[Aplayer.AposY][Aplayer.APosX].shape, 0);
                    break;
                case UP:
                    if (!(Aplayer.APosX == selectX && Aplayer.AposY == selectY))
                        randShape(ATile[Aplayer.AposY][Aplayer.APosX].shape, 1);
                    Aplayer.AposY = (Aplayer.AposY + 6) % (Size - 2) + 1;
                    Anygotoxy(Aplayer.APosX, Aplayer.AposY);
                    randShape(ATile[Aplayer.AposY][Aplayer.APosX].shape, 0);
                    break;
                case DOWN:
                    if (!(Aplayer.APosX == selectX && Aplayer.AposY == selectY))
                        randShape(ATile[Aplayer.AposY][Aplayer.APosX].shape, 1);
                    Aplayer.AposY = (Aplayer.AposY + 8) % (Size - 2) + 1;
                    Anygotoxy(Aplayer.APosX, Aplayer.AposY);
                    randShape(ATile[Aplayer.AposY][Aplayer.APosX].shape, 0);
                    break;

                default:
                    break;
                }
            }
            else if (key == SPACE)
            {
                if (selectX != -1)                                 // ������ �ٲٷ��� ������ �迭�� ������ (x���� -1�� �ƴ�)�迭�� ������ ����
                {
                    /*
                    * �� ����� ����ġ ���־��� �� �迭�� Ȯ���Ͽ� �νð�,
                    * ������ �Ʒ������� ������ ����
                    * ���ʺ��ͺ���ִ� ĭ���� Ȯ���Ͽ� ������ ������� �ٽ� ä���
                    */
                    SwapTile();
                    Crash();
                    PrintScore();
                    Anygotoxy(Aplayer.APosX, Aplayer.AposY);
                    continue;
                }
                selectX = Aplayer.APosX;                          // ù ��°�� ������ �迭�̸� ������ �迭 ��ǥ ����
                selectY = Aplayer.AposY;
                randShape(ATile[selectY][selectX].shape, 0);      // ���õ� Ÿ���� �� ������� �ٲ���
            }
            Crash();
            PrintScore();
            LetDown();
            FillShape();
            Crash();
            gotoxy(Aplayer.APosX, Aplayer.AposY);
            end = clock();

        }
    }
    system("cls");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);

    printf("      # #   #      # # # #      #      # # #    #\n");
    printf("    #	    #      #           # #     #    #   #\n");
    printf("   #	    #      # # # #    # # #    # # #    #\n");
    printf("    #	    #	   #	     #	   #   #    #   \n");
    printf("      # #   # # #  # # # #  #	    #  #     #  #\n\n\n");

    Sleep(1000);

    system("cls");

    printf("\nScore > %d", AnyScore);
    if (AnyScore > AnyBestScore)
        AnyWriteData();
    printf("\n\nBest Score [%d]", AnyBestScore);

    Sleep(2000);
    return;
}

void Anygotoxy(int x, int y)                                              // player�� �����Ӵ�� Ŀ���� �����̴� �Լ�
{
    COORD pos = { x * 2, y };                                             // x �� �׸� ũ�⿡ �°� ��ǥ�� �ٲٰ�, y �� ��ǥ�� �°�

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void SwapTile()
{
    int shape_;                                                                              // ù ��°�� ������ �迭�� ��ǥ�� coord�� �����ͷ� ����

    shape_ = ATile[Aplayer.AposY][Aplayer.APosX].shape;                                      // 1. �� struct �迭 �Ӽ� �� shape(���) �������� ���� �ٲ��ش�.
    ATile[Aplayer.AposY][Aplayer.APosX].shape = ATile[selectY][selectX].shape;
    ATile[selectY][selectX].shape = shape_;


    Anygotoxy(selectX, selectY);                                                             // 2. Ŀ�� ��ġ�� ù ��°�� �����ߴ� ĭ���� �̵���Ų ����
    randShape(ATile[selectY][selectX].shape, 1);                                             // 3. �� ��°�� ������ ���� ĭ�� ����� ����ش�.
    Anygotoxy(Aplayer.APosX, Aplayer.AposY);                                                 // 4. Ŀ�� ��ġ�� �����ִ� ��ǥ�� �̵���Ű��
    randShape(ATile[Aplayer.AposY][Aplayer.APosX].shape, 1);                                 // 5. ù ��°�� �����ߴ� ĭ�� ����� ���� ���
    Anygotoxy(Aplayer.APosX, Aplayer.AposY);                                                 // 6. ����ϴ��� �зȴ� Ŀ�� �ٽ� ���� ��ġ�� ����
    selectX = -1;                                                                            // 7. ���������� ������ �迭�� x��ǥ���� �ʱ�ȭ
}

void Crash()                                                                                            // 3 �� �̻����� ���ӵ� �迭 Ȯ���ϱ� ���� �Լ�
{
    int y, x, xcnt = 0, ycnt = 0, i;
    for (y = 1; y < Size - 1; y++)
    {
        for (x = 1; x < Size - 1; x++)
        {
            if (ATile[y][x].shape == ANone)                                                // *����ִ� Ÿ���̸� skip
                continue;
            if (ATile[y][x].shape == ATile[y][x + 1].shape)                                // 1. x ��ǥ �������� �������� 2�� �̻� ������
                xcnt++;                                                                    // 2. cnt ������ ������Ű��

            else                                                                           // �������� ����� ���� ������ �� ���� ����� ���� üũ
            {
                if (xcnt >= 2)                                                             // *3. ���ӵ� ����� 3�� �̻��̸�,
                {
                    Anygotoxy(x - xcnt, y);                                                // 3-1. Ŀ�� ��ġ�� ���ӵ� ����� ù��° �ڸ��� �̵���Ű��

                    i = xcnt;
                    while (i-- >= 0)
                        randShape(ATile[y][x - xcnt].shape, 0);                            // 3-2. �ν� ĭ�� ����� 0.5�ʰ� �� ������ ǥ��
                    Sleep(500);
                    Anygotoxy(x - xcnt, y);                                                // 3-3. �ٽ� Ŀ�� ��ġ�� ���ӵ� ����� ù��° �ڸ��� �̵���Ű��
                    AnyScore += xcnt * 100;
                    while (xcnt >= 0)
                    {
                        printf("  ");
                        ATile[y][x - xcnt].shape = ANone;                                  // 3-4. ��� �����ϰ� ATile�� ��� �Ӽ��� ANone ������ ��ȯ
                        ATile[y][x - xcnt].Atype = ANone;
                        xcnt--;
                    }
                }
                xcnt = 0;                                                                  // *3. ���ӵ� ����� 2�� ���ϸ� cnt ���� 0���� �ʱ�ȭ
                PrintScore();
            }
        }
        LetDown();
    }

    for (x = 1; x < Size - 1; x++)
    {
        for (y = 1; y < Size - 1; y++)
        {
            if (ATile[y][x].shape == ANone)
                continue;
            if (ATile[y][x].shape == ATile[y + 1][x].shape)
                ycnt++;
            else
            {
                if (ycnt >= 2)
                {
                    i = ycnt;
                    while (i >= 0)
                    {
                        Anygotoxy(x, y - i);
                        randShape(ATile[y - i--][x].shape, 0);
                    }
                    Sleep(500);
                    AnyScore += ycnt * 100;
                    while (ycnt >= 0)
                    {
                        Anygotoxy(x, y - ycnt);
                        printf("  ");
                        ATile[y - ycnt][x].shape = ANone;
                        ATile[y - ycnt][x].Atype = ANone;
                        ycnt--;
                    }
                }
                ycnt = 0;
            }

        }
    }
    PrintScore();
    LetDown();
}

void LetDown()
{
    int y, x, _y, i, j;
    struct AT swap[Size][Size];

    for (x = 1; x < Size - 1; x++)                                             // 1. 1������ �� ������ �� �پ�
    {
        for (y = Size - 2; y > 0; y--)                                         // 1-2. ������ ĭ���� �ö󰡸� üũ
        {
            _y = y;
            if (ATile[y][x].Atype == ANone)
            {
                while (ATile[_y][x].Atype == ANone)                            // 2.�� ĭ�� �ƴ� ĭ���� �� ĭ�� �ø�
                    _y--;

                if (ATile[_y][x].Atype == AWall)                               // * 2-2. ���� ������ ��ĭ �ƴ� ��ĭ�� ���̶�� ������ �� �����Ƿ� for ���� ���� continue;
                    continue;

                else if (ATile[y][x].Atype == ANone)                           // 2-3. �� ĭ�� ���� �ƴϸ� �ٸ� ��� ĭ�� ��� �ۿ� ����
                {
                    while (ATile[_y][x].Atype == ABlock)
                    {
                        Anygotoxy(x, _y);
                        printf("  ");                                          // 3. �о��� ĭ ���� ��� �����ϰ�,
                                                                               // 3-1. ĭ �ٲ㼭 ������ ��ü
                        swap[y][x] = ATile[_y][x];
                        ATile[_y][x] = ATile[y][x];
                        ATile[y][x] = swap[y][x];

                        Anygotoxy(x, y);                                      // 4. ���������� �и� ĭ ��� ����Ʈ
                        randShape(ATile[y][x].shape, 1);

                    }
                }
            }
        }
    }
}

void FillShape()
{
    int y = 0, x = 0;

    for (x = 1; x < Size - 1; x++)
        for (y = 1; y < Size - 1; y++)
            if (ATile[y][x].Atype == ANone)
            {
                Anygotoxy(x, y);
                ATile[y][x].Atype = ABlock;
                ATile[y][x].shape = (rand() % 4 + 1);
                randShape(ATile[y][x].shape, 1);
            }
}

void PrintScore()
{
    AnyReadData();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    Anygotoxy(0, Size + 4);
    printf("Score > %d", AnyScore);
    if (AnyScore > AnyBestScore)
        AnyWriteData();
    Anygotoxy(0, Size + 2);
    printf("Best Score [%d]", AnyBestScore);
}

void AnyReadData()
{
    FILE* fp;
    fp = fopen("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\AnyBestScore.txt", "rt");        // "rt" �б� ���

    fscanf(fp, "%d", &AnyBestScore);                                                            // ���� ���� AnyBestScore �ҷ���
    fclose(fp);                                                                                 // ���� �ݱ�
}

void AnyWriteData()
{
    FILE* fp;

    fp = fopen("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\AnyBestScore.txt", "wt");        // "wt" ������
    fprintf(fp, "%d", AnyScore);                                                                // bestscore ����
    fclose(fp);                                                                                 // ���� �ݱ�
}