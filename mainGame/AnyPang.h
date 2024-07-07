#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>                                    // time rand 난수 받기 위한 라이브러리
#include <stdlib.h>                                  // 난수 생성을 위한 라이브러리
#include <windows.h>                                 // 콘솔창의 제목과 미로의 색깔을 위한 라이브러리
#include <stdbool.h>                                 // bool 타입 변수 사용을 위한 라이브러리
#include <conio.h>                                   // 방향키로 입력을 받아 플레이어를 움직이기 위한 라이브러리
#include <Windows.h>                                 //음악 재상을 위한 헤더파일
#include <mmsystem.h>                                // 음악 재생을 위한 헤더파일
#pragma comment(lib,"winmm.lib")                     //음악 재생
#define Size 10                                      // 맵 사이즈

#pragma region APlayerDirection      // 플레이어의 방향키 조절
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
    Spade = 1,                                  // ♠   -> 파란색
    Diamond = 2,                                // ◆   -> 노란색
    Heart = 3,                                  // ♥   -> 분홍색
    Clover = 4,                                 // ♣   -> 초록색
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
struct AT ATile[Size][Size] = { 0 };             // 타일 담을 배열
int selectX = -1;                                // 플레이어가 첫 번째로 선택한 타일의 x 좌표 담을 변수
int selectY = -1;                                // 플레이어가 첫 번째로 선택한 타일의 y 좌표 담을 변수
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
    SetConsoleTitle(TEXT("애니팡"));

    printf("\n< 게임 룰 >\n\n\n");
    printf("1. 방향키로 플레이어가 두 개의 칸을 선택한다.\n\n");
    printf("2. 플레이어가 선택한 칸이 서로 배치를 바꾼다.\n\n");
    printf("3. 연속된 모양이 가로 혹은 세로로 3개 이상있으면 해당 칸이 부셔진다.\n\n");
    printf("4. 25초간 부서진 칸에 따라 점수를 획득하는 게임");

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

void APrintGameStart()                            // 맵 만들기 전 GAME START 글씨 표시
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
                printf("■");
            }
            else
            {
                ATile[y][x].Atype = ABlock;
                ATile[y][x].shape = (rand() % 4 + 1);                   // 스페이드, 다이아, 하트, 클로버 중 랜덤으로 모양 생성
                randShape(ATile[y][x].shape, 1);                        // 생선한 모양 출력
            }
        }
        printf("\n");
    }
}

void randShape(int num, bool color)                                     // bool 자료형의 color 변수의 값 0일 때는 색, 아닐때(1)는 흑백 (APlayer의 이동 경로는 흑백)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);       // 기본으로 흑백 출력
    switch (num)
    {
    case Spade:
        if (color)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
        printf("♠");
        break;

    case Diamond:
        if (color)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        printf("◆");
        break;

    case Heart:
        if (color)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        printf("♥");
        break;

    case Clover:
        if (color)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        printf("♣");
        break;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                         // 색 흰색으로 초기화
}

int APlay()
{
    Anygotoxy(1, 1);                                                                     // APlayer 초기 위치 설정

    while ((int)(time(&end) - start) <= 25)                                              // 15초 동안 반복
    {
        // 커서 위치를 플레이어 위치로 바꿈
        if (_kbhit())                                                                    // 키보드가 눌리면 if 문 안으로
        {
            int key = _getch();

            if (key == 224)                                                              // getch() 함수의 1차 반환값 추출   (단, space의 경우 1차에서 바로 추출하고 방향키인 경우에는 2차 반환값으로 추출)
            {
                key = _getch();
                switch (key)                                                             // 1. getch() 함수의 2차 반환값 추출해서 움직임 조절
                {
                case LEFT:
                    if (!(Aplayer.APosX == selectX && Aplayer.AposY == selectY))         // ** 플레이어가 바꾸려고 선택한 칸이 아니면
                        randShape(ATile[Aplayer.AposY][Aplayer.APosX].shape, 1);         // 2. 기존에 있던 칸 다시 컬러로 만들어줌
                    Aplayer.APosX = (Aplayer.APosX + 6) % (Size - 2) + 1;                // 3. 이동할 좌표 계산
                    Anygotoxy(Aplayer.APosX, Aplayer.AposY);                             // 4. 커서 이동   
                    randShape(ATile[Aplayer.AposY][Aplayer.APosX].shape, 0);             // 5. 플레이어 위치 표시
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
                if (selectX != -1)                                 // 기존에 바꾸려고 선택한 배열이 선택한 (x값이 -1이 아닌)배열이 있으면 스왑
                {
                    /*
                    * 각 모양을 스위치 해주었을 때 배열을 확인하여 부시고,
                    * 모양들을 아랫쪽으로 내려준 다음
                    * 위쪽부터비어있는 칸들을 확인하여 랜덤의 모양으로 다시 채운다
                    */
                    SwapTile();
                    Crash();
                    PrintScore();
                    Anygotoxy(Aplayer.APosX, Aplayer.AposY);
                    continue;
                }
                selectX = Aplayer.APosX;                          // 첫 번째로 선택한 배열이면 선택한 배열 좌표 저장
                selectY = Aplayer.AposY;
                randShape(ATile[selectY][selectX].shape, 0);      // 선택된 타일의 색 흰색으로 바꿔줌
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

void Anygotoxy(int x, int y)                                              // player의 움직임대로 커서를 움직이는 함수
{
    COORD pos = { x * 2, y };                                             // x 는 네모 크기에 맞게 좌표를 바꾸고, y 는 좌표에 맞게

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void SwapTile()
{
    int shape_;                                                                              // 첫 번째로 선택한 배열의 좌표값 coord형 데이터로 저장

    shape_ = ATile[Aplayer.AposY][Aplayer.APosX].shape;                                      // 1. 두 struct 배열 속성 중 shape(모양) 데이터의 값을 바꿔준다.
    ATile[Aplayer.AposY][Aplayer.APosX].shape = ATile[selectY][selectX].shape;
    ATile[selectY][selectX].shape = shape_;


    Anygotoxy(selectX, selectY);                                                             // 2. 커서 위치를 첫 번째로 선택했던 칸으로 이동시킨 다음
    randShape(ATile[selectY][selectX].shape, 1);                                             // 3. 두 번째로 선택한 현재 칸의 모양을 찍어준다.
    Anygotoxy(Aplayer.APosX, Aplayer.AposY);                                                 // 4. 커서 위치를 원래있던 좌표로 이동시키고
    randShape(ATile[Aplayer.AposY][Aplayer.APosX].shape, 1);                                 // 5. 첫 번째로 선택했던 칸의 모양을 먼저 찍고
    Anygotoxy(Aplayer.APosX, Aplayer.AposY);                                                 // 6. 출력하느라 밀렸던 커서 다시 현재 위치로 복귀
    selectX = -1;                                                                            // 7. 마지막으로 선택한 배열의 x좌표값을 초기화
}

void Crash()                                                                                            // 3 개 이상으로 연속된 배열 확인하기 위한 함수
{
    int y, x, xcnt = 0, ycnt = 0, i;
    for (y = 1; y < Size - 1; y++)
    {
        for (x = 1; x < Size - 1; x++)
        {
            if (ATile[y][x].shape == ANone)                                                // *비어있는 타일이면 skip
                continue;
            if (ATile[y][x].shape == ATile[y][x + 1].shape)                                // 1. x 좌표 기준으로 연속으로 2개 이상 나오면
                xcnt++;                                                                    // 2. cnt 개수를 증가시키고

            else                                                                           // 연속으로 모양이 같지 않으면 두 가지 경우의 수를 체크
            {
                if (xcnt >= 2)                                                             // *3. 연속된 모양이 3개 이상이면,
                {
                    Anygotoxy(x - xcnt, y);                                                // 3-1. 커서 위치를 연속된 모양의 첫번째 자리로 이동시키고

                    i = xcnt;
                    while (i-- >= 0)
                        randShape(ATile[y][x - xcnt].shape, 0);                            // 3-2. 부실 칸의 모양을 0.5초간 흰 색으로 표시
                    Sleep(500);
                    Anygotoxy(x - xcnt, y);                                                // 3-3. 다시 커서 위치를 연속된 모양의 첫번째 자리로 이동시키고
                    AnyScore += xcnt * 100;
                    while (xcnt >= 0)
                    {
                        printf("  ");
                        ATile[y][x - xcnt].shape = ANone;                                  // 3-4. 모양 삭제하고 ATile의 모양 속성도 ANone 값으로 변환
                        ATile[y][x - xcnt].Atype = ANone;
                        xcnt--;
                    }
                }
                xcnt = 0;                                                                  // *3. 연속된 모양이 2개 이하면 cnt 개수 0으로 초기화
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

    for (x = 1; x < Size - 1; x++)                                             // 1. 1열부터 열 단위로 한 줄씩
    {
        for (y = Size - 2; y > 0; y--)                                         // 1-2. 마지막 칸부터 올라가며 체크
        {
            _y = y;
            if (ATile[y][x].Atype == ANone)
            {
                while (ATile[_y][x].Atype == ANone)                            // 2.빈 칸이 아닌 칸까지 한 칸씩 올림
                    _y--;

                if (ATile[_y][x].Atype == AWall)                               // * 2-2. 만약 최초의 빈칸 아닌 윗칸이 벽이라면 내려줄 게 없으므로 for 문에 대한 continue;
                    continue;

                else if (ATile[y][x].Atype == ANone)                           // 2-3. 빈 칸도 벽도 아니면 다른 모양 칸인 경우 밖에 없음
                {
                    while (ATile[_y][x].Atype == ABlock)
                    {
                        Anygotoxy(x, _y);
                        printf("  ");                                          // 3. 밀어줄 칸 먼저 모양 정리하고,
                                                                               // 3-1. 칸 바꿔서 데이터 교체
                        swap[y][x] = ATile[_y][x];
                        ATile[_y][x] = ATile[y][x];
                        ATile[y][x] = swap[y][x];

                        Anygotoxy(x, y);                                      // 4. 마지막으로 밀린 칸 모양 프린트
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
    fp = fopen("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\AnyBestScore.txt", "rt");        // "rt" 읽기 모드

    fscanf(fp, "%d", &AnyBestScore);                                                            // 파일 내에 AnyBestScore 불러옴
    fclose(fp);                                                                                 // 파일 닫기
}

void AnyWriteData()
{
    FILE* fp;

    fp = fopen("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\AnyBestScore.txt", "wt");        // "wt" 쓰기모드
    fprintf(fp, "%d", AnyScore);                                                                // bestscore 삽입
    fclose(fp);                                                                                 // 파일 닫기
}