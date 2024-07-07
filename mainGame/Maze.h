#pragma warning(disable: 4996)
#include <stdio.h>
#include <stdlib.h>                 // 랜덤 함수 사용하기 위한 라이브러리
#include <windows.h>                // 콘솔창의 제목과 미로의 색깔을 위한 라이브러리
#include <time.h>                   // 난수 추출하기 위한 라이브러리
#include <conio.h>                  // 방향키로 입력을 받아 플레이어를 움직이기 위한 라이브러리
#include <Windows.h>                //음악 재상을 위한 헤더파일
#include <mmsystem.h>               // 음악 재생을 위한 헤더파일
#pragma comment(lib,"winmm.lib")    //음악 재생

#pragma region PlayerDirection      // 플레이어의 방향키 조절
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27
#define ENTER 13
#pragma endregion

typedef enum _tileType // 미로 타일 타입 구분
{
    None = 0,
    MWall = 1,
    MEmpty = 2,
};

struct T                             // T(ile) 구조체 (각 타일의 속성)
{
    int x, y;                        // x, y 값의 좌표
    enum _tileType Type;             // 각 배열의 상태 ( Empty/Wall )
    char SQAURE;
};

struct PLAYER                       // PLAYER 구조체 (움직이는 말)
{
    int PosY, PosX;                 // player  객체의 좌표값
};


#pragma region GSwinder              // 미로 찾기에 사용할 전역 변수
struct PLAYER _player = { 1, 1 };               // Player 타입의 _player 객체화
struct T Tile[100][100] = { 0 };             // 초기 타일 크기
int Size;                            // 미로의 크기
int DestX, DestY;                    // 미로의 도착지 (Destination)의 좌표
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
    SetConsoleTitle(TEXT("미로 게임"));         // 콘솔창 제목 변경

    printf("\n< 게임 룰 >\n\n\n");
    printf("1. 방향키로 플레이어가 미로의 길을 찾는다.\n\n");
    printf("2. 하단의 빨간색 목적지 칸에 도착하면 맵 클리어\n\n");
    printf("3. 다섯 개의 맵 모두 클리어하면 게임 종료\n\n");
    printf("4. 최대한 빨리 모든 맵을 클리어할수록 점수를 획득하는 게임");

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

        clock_t start = clock();                        // 시작 시작 측정
        while (1)
        {
            system("cls");
            GenerateSideWinder(Size);
            Play();
            if (round >= 5)
                break;
            Size += 2;
        }
        clock_t end = clock();                          // 끝난 시간 측정

        MazeScore = 100000 - ((end - start)/10);
    }
    ScorePrint();
}


void GenerateSideWinder(int Size)                       // 미로 구조 생성 함수
{
    srand(time(NULL));
    int y, x;
    for (y = 0; y < Size; y++)                          // 일단 길을 다 막아버리는 작업
    {
                                                        //   0 1 2 3 4 5 6 7 8
        for (x = 0; x < Size; x++)                      // 0 ■ ■ ■ ■ ■ ■ ■ ■ ■ 
        {                                               // 1 ■   ■   ■   ■   ■ 
            if (x % 2 == 0 || y % 2 == 0)               // 2 ■ ■ ■ ■ ■ ■ ■ ■ ■
            {                                           // 3 ■   ■   ■   ■   ■ 
                Tile[y][x].Type = MWall;                // 4 ■ ■ ■ ■ ■ ■ ■ ■ ■ 
                                                        // 5 ■   ■   ■   ■   ■  
            }                                           // 6 ■ ■ ■ ■ ■ ■ ■ ■ ■ 
            else                                        // 7 ■   ■   ■   ■   ■  
            {                                           // 8 ■ ■ ■ ■ ■ ■ ■ ■ ■ 
                Tile[y][x].Type = MEmpty;
            }
            Tile[y][x].SQAURE = '■';
        }
    }

    for (y = 0; y < Size; y++)                          //길을 뚫는 작업
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

            if (rand() % 2 == 0)            // rand에서 0이 나오면 오른쪽으로 한 칸씩 뚫음
            {
                Tile[y][x + 1].Type = MEmpty;
                count++;
            }
            else                            // rand에서 1이 나오면 뚫린 칸(count)중 한 칸 랜덤으로 아래로 뚫음
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
            // 플레이어 좌표를 갖고 와서, 그 좌표랑 현재 y, x가 일치하면 플레이저 전용 색상으로 표시
            if ((y == DestY && x == DestX) || (y == 1 && x == 1))
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);        // 출발지와 목적지의 좌표는 빨간색
                printf("■");
            }
            else if (_player.PosY == y && _player.PosX == x)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);        // 플레이어의 좌표는 파랑색
                printf("■");
            }
            else
            {
                GetTileColor(Tile[y][x]);
            }
        }
        printf("\n");
    }
}

void GetTileColor(struct T Tile)                                                    // Tile 객체의 TileType을 검사하여,
{
    switch (Tile.Type)
    {
    case MWall:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);                // 벽이면 흰색,
        printf("■");
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
        // 커서 위치를 플레이어 위치로 바꿈
        if (_kbhit())                                                               // 키보드가 눌리면 if 문 안으로
        {
            int key = _getch();
            if (key == 224)                                                         // getch() 함수의 1차 반환값 추출
            {
                key = _getch();
                switch (key)                                                        // getch() 함수의 2차 반환값 추출해서 움직임 조절
                {
                case LEFT:
                    if (Tile[_player.PosY][_player.PosX - 1].Type == MWall)         // 움직일 칸이 벽인지 체크
                    {
                        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), TILE);
                        printf("벽으로 이동하실 수 없습니다.");
                        break;
                    }
                    Tile[_player.PosY][--_player.PosX];                             // 1. 이동할 방향으로 좌표값만 미리 변화
                    printf("  ");                                                   // 2. 현재 커서 위치의 네모를 지움
                    gotoxy(_player.PosX, _player.PosY);                             // 3. 커서를 이동할 칸으로 이동
                    printf("■");                                                    // 4. 플레이어 객체 프린트
                    break;
                case RIGHT:
                    if (Tile[_player.PosY][_player.PosX + 1].Type == MWall)        // 움직일 칸이 벽인지 체크
                    {
                        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), TILE);
                        printf("벽으로 이동하실 수 없습니다.");
                        break;
                    }
                    Tile[_player.PosY][++_player.PosX];                            // player 방향 오른쪽으로
                    printf("  ");
                    gotoxy(_player.PosX, _player.PosY);
                    printf("■");
                    break;
                case UP:
                    if (Tile[_player.PosY - 1][_player.PosX].Type == MWall)        // 움직일 칸이 벽인지 체크
                    {
                        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), TILE);
                        printf("벽으로 이동하실 수 없습니다.");
                        break;
                    }
                    Tile[--_player.PosY][_player.PosX];                            // player 방향 왼쪽으로
                    printf("  ");
                    gotoxy(_player.PosX, _player.PosY);
                    printf("■");
                    break;
                case DOWN:
                    if (Tile[_player.PosY + 1][_player.PosX].Type == MWall)        // 움직일 칸이 벽인지 체크
                    {
                        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), TILE);
                        printf("벽으로 이동하실 수 없습니다.");
                        break;
                    }
                    Tile[++_player.PosY][_player.PosX];                            // player 방향 왼쪽으로
                    printf("  ");
                    gotoxy(_player.PosX, _player.PosY);
                    printf("■");
                    break;
                case ESC:
                    printf("게임 종료\n");
                    return 0;
                }
                gotoxy(_player.PosX, _player.PosY);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
            }
        }
    }
    return 0;
}


void gotoxy(int x, int y)                                                       // player의 움직임대로 커서를 움직이는 함수
{
    COORD pos = { x * 2, y };                                                   // x 는 네모 크기에 맞게 좌표를 바꾸고, y 는 좌표에 맞게

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
    fp = fopen("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\MazeBestScore.txt", "rt");               // 읽기 모드

    fscanf(fp, "%d", &bestScore);                                                                       // 파일 내에 bestscore 불러옴
    fclose(fp);                                                                                         // 파일 닫기
}

void WriteData()
{
    FILE* fp;

    fp = fopen("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\MazeBestScore.txt", "wt");               // 쓰기모드로 파일 열기
    fprintf(fp, "%d", MazeScore);                                                                       // bestscore 삽입
    fclose(fp);                                                                                         // 파일 닫기
}

void MPrintGameStart()                                                                                  // 맵 만들기 전 GAME START 글씨 표시
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