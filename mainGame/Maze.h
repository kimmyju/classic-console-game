#pragma warning(disable: 4996)
#include <stdio.h>
#include <stdlib.h>                 // ���� �Լ� ����ϱ� ���� ���̺귯��
#include <windows.h>                // �ܼ�â�� ����� �̷��� ������ ���� ���̺귯��
#include <time.h>                   // ���� �����ϱ� ���� ���̺귯��
#include <conio.h>                  // ����Ű�� �Է��� �޾� �÷��̾ �����̱� ���� ���̺귯��
#include <Windows.h>                //���� ����� ���� �������
#include <mmsystem.h>               // ���� ����� ���� �������
#pragma comment(lib,"winmm.lib")    //���� ���

#pragma region PlayerDirection      // �÷��̾��� ����Ű ����
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27
#define ENTER 13
#pragma endregion

typedef enum _tileType // �̷� Ÿ�� Ÿ�� ����
{
    None = 0,
    MWall = 1,
    MEmpty = 2,
};

struct T                             // T(ile) ����ü (�� Ÿ���� �Ӽ�)
{
    int x, y;                        // x, y ���� ��ǥ
    enum _tileType Type;             // �� �迭�� ���� ( Empty/Wall )
    char SQAURE;
};

struct PLAYER                       // PLAYER ����ü (�����̴� ��)
{
    int PosY, PosX;                 // player  ��ü�� ��ǥ��
};


#pragma region GSwinder              // �̷� ã�⿡ ����� ���� ����
struct PLAYER _player = { 1, 1 };               // Player Ÿ���� _player ��üȭ
struct T Tile[100][100] = { 0 };             // �ʱ� Ÿ�� ũ��
int Size;                            // �̷��� ũ��
int DestX, DestY;                    // �̷��� ������ (Destination)�� ��ǥ
int bestScore;
int MazeScore;
int round = 1;
#pragma endregion

void GenerateSideWinder(int Size);
void Render();
void GetTileColor(struct T Tile);
int Play();
void gotoxy(int x, int y);
void ScorePrint();
void WriteData();
void ReadData();
void MPrintGameStart();

void Maze()
{
    int GameS;
    Size = 15;
    SetConsoleTitle(TEXT("�̷� ����"));         // �ܼ�â ���� ����

    printf("\n< ���� �� >\n\n\n");
    printf("1. ����Ű�� �÷��̾ �̷��� ���� ã�´�.\n\n");
    printf("2. �ϴ��� ������ ������ ĭ�� �����ϸ� �� Ŭ����\n\n");
    printf("3. �ټ� ���� �� ��� Ŭ�����ϸ� ���� ����\n\n");
    printf("4. �ִ��� ���� ��� ���� Ŭ�����Ҽ��� ������ ȹ���ϴ� ����");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    printf("\n\n\nStart Game>  Enter");
    GameS = _getch();

    if (GameS == ENTER)
    {
        system("cls");
        PlaySound(TEXT("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\MazeBgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

        MPrintGameStart();
        Sleep(1000);
        system("cls");

        clock_t start = clock();                        // ���� ���� ����
        while (1)
        {
            system("cls");
            GenerateSideWinder(Size);
            Play();
            if (round >= 5)
                break;
            Size += 2;
        }
        clock_t end = clock();                          // ���� �ð� ����

        MazeScore = 100000 - ((end - start)/10);
    }
    ScorePrint();
}


void GenerateSideWinder(int Size)                       // �̷� ���� ���� �Լ�
{
    srand(time(NULL));
    int y, x;
    for (y = 0; y < Size; y++)                          // �ϴ� ���� �� ���ƹ����� �۾�
    {
                                                        //   0 1 2 3 4 5 6 7 8
        for (x = 0; x < Size; x++)                      // 0 �� �� �� �� �� �� �� �� �� 
        {                                               // 1 ��   ��   ��   ��   �� 
            if (x % 2 == 0 || y % 2 == 0)               // 2 �� �� �� �� �� �� �� �� ��
            {                                           // 3 ��   ��   ��   ��   �� 
                Tile[y][x].Type = MWall;                // 4 �� �� �� �� �� �� �� �� �� 
                                                        // 5 ��   ��   ��   ��   ��  
            }                                           // 6 �� �� �� �� �� �� �� �� �� 
            else                                        // 7 ��   ��   ��   ��   ��  
            {                                           // 8 �� �� �� �� �� �� �� �� �� 
                Tile[y][x].Type = MEmpty;
            }
            Tile[y][x].SQAURE = '��';
        }
    }

    for (y = 0; y < Size; y++)                          //���� �մ� �۾�
    {
        int count = 1;
        for (x = 0; x < Size; x++)
        {
            if (x % 2 == 0 || y % 2 == 0)
            {
                continue;
            }

            if (y == Size - 2 && x == Size - 2)
            {
                continue;
            }

            if (y == Size - 2)
            {
                Tile[y][x + 1].Type = MEmpty;
                continue;
            }

            if (x == Size - 2)
            {
                Tile[y + 1][x].Type = MEmpty;
                continue;
            }

            if (rand() % 2 == 0)            // rand���� 0�� ������ ���������� �� ĭ�� ����
            {
                Tile[y][x + 1].Type = MEmpty;
                count++;
            }
            else                            // rand���� 1�� ������ �ո� ĭ(count)�� �� ĭ �������� �Ʒ��� ����
            {
                int randomIndex = rand() % count;
                Tile[y + 1][x - randomIndex * 2].Type = MEmpty;
                count = 1;
            }
        }
    }
    Render();
    round++;
}

void Render()
{
    DestY = Size - 2;
    DestX = Size - 2;
    _player.PosX = 1;
    _player.PosY = 1;

    for (int y = 0; y < Size; y++)
    {
        for (int x = 0; x < Size; x++)
        {
            // �÷��̾� ��ǥ�� ���� �ͼ�, �� ��ǥ�� ���� y, x�� ��ġ�ϸ� �÷����� ���� �������� ǥ��
            if ((y == DestY && x == DestX) || (y == 1 && x == 1))
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);        // ������� �������� ��ǥ�� ������
                printf("��");
            }
            else if (_player.PosY == y && _player.PosX == x)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);        // �÷��̾��� ��ǥ�� �Ķ���
                printf("��");
            }
            else
            {
                GetTileColor(Tile[y][x]);
            }
        }
        printf("\n");
    }
}

void GetTileColor(struct T Tile)                                                    // Tile ��ü�� TileType�� �˻��Ͽ�,
{
    switch (Tile.Type)
    {
    case MWall:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);                // ���̸� ���,
        printf("��");
        break;
    case MEmpty:
        printf("  ");
        break;
    default:
        break;
    }
}


int Play()
{
    COORD TILE = { 0, Size };
    while ((!(_player.PosX == DestX && _player.PosY == DestY)))
    {
        // Ŀ�� ��ġ�� �÷��̾� ��ġ�� �ٲ�
        if (_kbhit())                                                               // Ű���尡 ������ if �� ������
        {
            int key = _getch();
            if (key == 224)                                                         // getch() �Լ��� 1�� ��ȯ�� ����
            {
                key = _getch();
                switch (key)                                                        // getch() �Լ��� 2�� ��ȯ�� �����ؼ� ������ ����
                {
                case LEFT:
                    if (Tile[_player.PosY][_player.PosX - 1].Type == MWall)         // ������ ĭ�� ������ üũ
                    {
                        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), TILE);
                        printf("������ �̵��Ͻ� �� �����ϴ�.");
                        break;
                    }
                    Tile[_player.PosY][--_player.PosX];                             // 1. �̵��� �������� ��ǥ���� �̸� ��ȭ
                    printf("  ");                                                   // 2. ���� Ŀ�� ��ġ�� �׸� ����
                    gotoxy(_player.PosX, _player.PosY);                             // 3. Ŀ���� �̵��� ĭ���� �̵�
                    printf("��");                                                    // 4. �÷��̾� ��ü ����Ʈ
                    break;
                case RIGHT:
                    if (Tile[_player.PosY][_player.PosX + 1].Type == MWall)        // ������ ĭ�� ������ üũ
                    {
                        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), TILE);
                        printf("������ �̵��Ͻ� �� �����ϴ�.");
                        break;
                    }
                    Tile[_player.PosY][++_player.PosX];                            // player ���� ����������
                    printf("  ");
                    gotoxy(_player.PosX, _player.PosY);
                    printf("��");
                    break;
                case UP:
                    if (Tile[_player.PosY - 1][_player.PosX].Type == MWall)        // ������ ĭ�� ������ üũ
                    {
                        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), TILE);
                        printf("������ �̵��Ͻ� �� �����ϴ�.");
                        break;
                    }
                    Tile[--_player.PosY][_player.PosX];                            // player ���� ��������
                    printf("  ");
                    gotoxy(_player.PosX, _player.PosY);
                    printf("��");
                    break;
                case DOWN:
                    if (Tile[_player.PosY + 1][_player.PosX].Type == MWall)        // ������ ĭ�� ������ üũ
                    {
                        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), TILE);
                        printf("������ �̵��Ͻ� �� �����ϴ�.");
                        break;
                    }
                    Tile[++_player.PosY][_player.PosX];                            // player ���� ��������
                    printf("  ");
                    gotoxy(_player.PosX, _player.PosY);
                    printf("��");
                    break;
                case ESC:
                    printf("���� ����\n");
                    return 0;
                }
                gotoxy(_player.PosX, _player.PosY);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
            }
        }
    }
    return 0;
}


void gotoxy(int x, int y)                                                       // player�� �����Ӵ�� Ŀ���� �����̴� �Լ�
{
    COORD pos = { x * 2, y };                                                   // x �� �׸� ũ�⿡ �°� ��ǥ�� �ٲٰ�, y �� ��ǥ�� �°�

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
}

void ScorePrint()
{

    system("cls");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);

    printf("       # #  #      # # # #      #      # # #    #\n");
    printf("     #	    #      #           # #     #    #   #\n");
    printf("    #	    #      # # # #    # # #    # # #    #\n");
    printf("     #	    #	   #	     #	   #   #    #   \n");
    printf("      # #   # # #  # # # #  #	    #  #     #  #\n\n\n");

    Sleep(1000);
    
    system("cls");

    ReadData();
    printf("\nScore > %d\n", MazeScore);

    if (MazeScore > bestScore)
        WriteData();
    printf("Best Score [%d]", bestScore);
    Sleep(2000);
    return;
}

void ReadData()
{
    FILE* fp;
    fp = fopen("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\MazeBestScore.txt", "rt");               // �б� ���

    fscanf(fp, "%d", &bestScore);                                                                       // ���� ���� bestscore �ҷ���
    fclose(fp);                                                                                         // ���� �ݱ�
}

void WriteData()
{
    FILE* fp;

    fp = fopen("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\MazeBestScore.txt", "wt");               // ������� ���� ����
    fprintf(fp, "%d", MazeScore);                                                                       // bestscore ����
    fclose(fp);                                                                                         // ���� �ݱ�
}

void MPrintGameStart()                                                                                  // �� ����� �� GAME START �۾� ǥ��
{
    gotoxy(6, 7);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    printf(" ##  #  # # ###      ## ###  #  ##  ###  #");

    gotoxy(6, 8);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    printf("#   # # ### #       #    #  # # # #  #   #");

    gotoxy(6, 9);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    printf("# # ### ### ##       #   #  ### ##   #   #");

    gotoxy(6, 10);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    printf("# # # # # # #         #  #  # # # #  #    ");

    gotoxy(6, 11);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    printf(" ## # # # # ###     ##   #  # # # #  #   #");
}