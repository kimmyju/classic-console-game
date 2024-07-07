#include <stdio.h>
#include <Windows.h>											//음악 재상을 위한 헤더파일
#include <mmsystem.h>											// 음악 재생을 위한 헤더파일
#pragma comment(lib,"winmm.lib") 

#define TRUE 1
#define FALSE 0
#define ROW 15
#define COL 15

typedef enum													//ONone-빈칸, B-흑돌,W-백돌
{
	ONone = 0,
	B = 1,
	W = 2
}Bstate;

int OmokB[ROW][COL], Oturn = 0;									// OmokB 배열-오목 판, Oturn-백돌과 흑돌의 차례를 구분하기 위해 선언(흑돌-짝수 Oturn일때,백돌-홀수Oturn일때)
void OPrintGameStart();											//GameStart문구를 출력해 주는 함수.
void InitBoard();												//오목판을 초기화 해주는 함수
void PrintBoard();												//오목판의 현황을 프린트해 주는 함수
inline int MoveStone(int* LocX, int* LocY);						//돌을 놓아 주는 함수
inline void ColorSet(unsigned short back, unsigned int text);   //색을 입히기 위한 함수
void Ogotoxy(int x, int y);										//커서 위치를 이동시키는 함수
void Input(int* x, int* y);										//입력을 받는 함수
int CheckGame();												//게임을 끝낼지 검사해 주는 함수. (돌 5개가 모이면 게임 종료)

int Omok()
{
	char player1[5], player2[5], YN;
	int GameS, x, y, CheckTF;
	SetConsoleTitle(TEXT("오목 게임"));

replay:
	printf("\n<게임 룰>\n");									//게임 룰 설명
	printf("\n\n1. 플레이어는 각각 흑과 백을 선택하여 입력한다.");
	printf("\n\n2. 흑과 백은 차례대로 한번씩 자리를 선택하여 돌을 놓는다.");
	printf("\n\n3. 가로 세로 혹은 대각선으로 먼저 5개의 돌을 배치 시키면 승리하는 게임");

	printf("\n\n각각의 PLAYER은 흑과 백을 선택해주세요(입력: b/w)\n");//Player1과 PLAYER2 말 선택
	printf("PLAYER1: ");
	scanf_s("%s", player1, sizeof(player1));					//player이 원하는 색의 말을 입력 받음
	printf("PLAYER2: ");
	scanf_s("%s", player2, sizeof(player2));

	if (strcmp(player1, "b\0") == 0)							//player1이 만약 b를 입력 했다면 player1이 선공
	{
		
		printf("흑: PLAYER1, 백: PLAYER2\n");
		printf("player1이 선공합니다!\n");

	}
	else														//player2이 만약 b를 입력 했다면 player1이 선공
	{
		printf("흑: PLAYER2, 백: PLAYER1\n");
		printf("player2이 선공합니다!\n");

	}
	ColorSet(0, 4);												//텍스트 색을 빨간색으로
	printf("\n\nStart Game>  Enter");
	GameS = getch();											//키보드의 Enter키를 입력 받기 위해 getch()사용   
	if (GameS == 13)											//사용자의 입력이 Enter(아스키코드- 13)이라면 본격적으로 게임 시작
	{
		system("cls");
		PlaySound(TEXT("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\OmokBgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

		OPrintGameStart();										//GameStart!문구 출력
		Sleep(1000);											//1초간 화면을 정지
		InitBoard();											//오목판 초기화
		do
		{
			system("cls");


			PrintBoard();										//오목판을 print

			printf("\n이동할 자리를 입력하세요(ex → 2 3): ");
			Input(&x, &y);										//오목을 둘 자리를 Input()함수를 통해 입력 받음

			CheckTF = MoveStone(x, y);							//위에서 입력 받은 x,y를 MoveStone()함수로 보내 둘을 둘 수 있는 자리인지 확인(반환값-TRUE,FALSE)

			if (CheckTF == FALSE)								//77-78줄 -->만약 사용자가 선택한 자리에 다른 돌이 이미 존재한다면 MoveStone()함수에서 FALSE를 반환 받고 반복문의 처음으로 이동
				continue;										//TRUE값을 반환 받았다는것은 MoveStone함수 안에서 x,y 자리에 돌을 정상적으로  놓아주었다는 의미.  

			if (CheckGame() == FALSE)							//80-81줄 checkGame()을 통해 돌이 가로,세로,혹은 대각선으로 5개가 모였는지를 확인(FALSE-같은 색 돌이 5개가 다 모였다는 의미,TRUE-5개가 다 모이지 않았다는 의미)
				break;

			Oturn++;											//CheckGame()함수에서 같은 색 돌이 5개 모이지 않았을 시 OOturn++을 통해 다음 사용자의 차례로 넘겨줌
		} while (1);


	}


	system("cls");
	PrintBoard();												//최종 오목판 출력
	if (Oturn % 2 == 0)											//do while문에서 OOturn이 짝수인 상태로 나왔다면 흑돌의 승리
	{
		ColorSet(0, 14);
		printf("\n★ ");
		ColorSet(0, 4);
		printf("WIN: 흑    ");
		ColorSet(0, 1);
		printf("LOSE: 백\n");

	}
	else														//do while문에서 OOturn이 홀수인 상태로 나왔다면 백돌의 승리
	{
		ColorSet(0, 14);
		printf("\n★ ");
		ColorSet(0, 4);
		printf("WIN: 백    ");
		ColorSet(0, 1);
		printf("LOSE: 흑\n");

	}
	ColorSet(0, 4);
	printf("\nPLAY AGAIN?(y/n): ");
	ColorSet(0, 15);
	YN = getche();
	if (YN == 'y')											//만약 사용자가 y를 입력했다면 main의 맨 처음 부분으로 이동
	{
		Oturn = 0;
		system("cls");
		goto replay;
	}
	else													//사용자가 n를 입력했다면 게임 종료
	{
		printf("\n\n<게임을 종료합니다>");
		Sleep(1500);
		return 0;
	}
}

void InitBoard()											//체스판 초기화를 위한 함수
{
	int i, j;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			OmokB[i][j] = ONone;							//오목판을 모두 빈칸으로 초기화(enum의 ONone == 0)
		}
	}
}

void PrintBoard()
{
	int i, j;
	ColorSet(0, 15);
	if (Oturn % 2 == 0)										//145-148줄 -> Oturn이 짝수인지 홀수인지를 판별하여 누구의 차례인지를 출력해줌
		printf("\n  turn: 흑");
	else
		printf("\n  turn: 백");
	printf("\n   ");
	for (i = 0; i < ROW; i++)
	{

		printf("  %2d", i);
	}
	printf("    \n");

	for (i = 0; i < ROW; i++)
	{
		ColorSet(0, 15);
		printf(" %2d ", i);

		for (j = 0; j < COL; j++)
		{
			// if(행:짝 열:짝 || 행:홀 열:홀) else(행:홀 열 : 짝 || 행 : 짝 열 : 홀)로 나눠준 이유는 오목판의 색을 번갈아가면서 찍어주기 위함
			if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1))//행:짝 열:짝 || 행:홀 열:홀
			{
				if (OmokB[i][j] == B)						//흑돌이 있을 때
				{

					ColorSet(6, 0);
					printf(" ● ");
				}
				else if (OmokB[i][j] == W)					//백돌이 있을 때
				{
					ColorSet(6, 15);
					printf(" ● ");
				}

				else										//ONone일 때(빈칸일 때)
				{
					ColorSet(6, 6);
					printf("    ");
				}
			}
			else//행:홀 열 : 짝 || 행 : 짝 열 : 홀
			{
				if (OmokB[i][j] == B)
				{
					ColorSet(14, 0);
					printf(" ● ");
				}
				else if (OmokB[i][j] == W)
				{
					ColorSet(14, 15);
					printf(" ● ");
				}

				else
				{
					ColorSet(14, 14);
					printf("    ");
				}
			}

		}

		printf("\n");
	}

	ColorSet(0, 15);
}

inline void ColorSet(unsigned short back, unsigned int text)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text | back << 4);
}

void Ogotoxy(int x, int y)									//커서 위치를 이동 시켜는 함수
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Input(int* x, int* y)									//입력을 받아주는 함수(자리 입력은 main에서만 받기 때문에 main의x,y의 주솟값을 인수로 넘김
{
	scanf_s("%d %d", x, y);
}

inline int MoveStone(int LocX, int LocY)					//돌을 놓아주는 함수
{
	if (Oturn % 2 == 0)										//흑돌의 차례인 경우
	{
		if (OmokB[LocX][LocY] == W || OmokB[LocX][LocY] == B) //사용자가 입력한 자리에 다른 돌이 존재한다면 return FALSE
			return FALSE;
		else if (OmokB[LocX][LocY] == ONone)				 //빈 자리라면 Omok[][]배열에 흑돌의 enum상수인 B를 넣어줌(B=1)
			OmokB[LocX][LocY] = B;
		else												//오목판을 벗어나는 경우
		{
			printf("\n잘못된 입력입니다. 자리를 다시 입력하세요");
			return FALSE;
		}

	}
	else													//백돌의 차례인 경우(위와 동일)
	{
		if (OmokB[LocX][LocY] == W || OmokB[LocX][LocY] == B)
			return FALSE;
		else if (OmokB[LocX][LocY] == ONone)				//빈 자리라면 Omok[][]배열에 백돌의 enum상수인 W를 넣어줌(W=2) 
			OmokB[LocX][LocY] = W;
		else
		{
			printf("\n잘못된 입력입니다. 자리를 다시 입력하세요");
			return FALSE;
		}
	}
	return TRUE;
}

int CheckGame()												//돌이 5개가 다 모였는지 체크 해주는 함수
{
	int i, j, k, t;                                         // 1. 오목은 결국 5개가 일렬로 모였을때 시작하는 부분과 끝나는 부분이 생긴다. 
	for (i = 0; i < ROW; i++)                               // 2. 오목 배열의 모든 칸을 검사해준다.               
	{                                                       // 3. 이때, 만약 어떠한 칸에 돌이 존재 한다면 돌이 존재하는 칸의 다음칸부터 돌이5개가 모일 수 있는 방향으로만 같은 돌 5개가 존재하는지 검사를 해준다  
		for (j = 0; j < COL; j++)
		{
			if (i <= 3 && j <= 3)                           //인덱스가 i <=3, j <=3인 경우, 오른쪽 방향,아래 방향, 오른쪽 아래 대각선으로만 돌이 5개가 모일 수 있다.(나머지 방향은 돌이 5개가 모일 수 없기 때문에 검사 하는것이 무의미함)               
			{
				if (OmokB[i][j] == W || OmokB[i][j] == B)   //돌이 존재할 때               
				{
					for (k = j + 1; k <= j + 4; k++)        //오른쪽 방향 검사         
					{
						if (OmokB[i][j] != OmokB[i][k])     //만약 Omok[i][j]에 있는 돌과 오른쪽 방향의 4개의 돌중  같지 않은것이 존재한다면 break를 통해 다음 인덱스로 이동
							break;
					}
					if (k == j + 4 + 1)						//만약 위에 for문 안에 if문에 걸리지 않는다면 돌이 5개 모여 있다는 의미이므로 return FALSE을 반환해 게임 종료
						return FALSE;
					for (k = i + 1; k <= i + 4; k++)		//아래 방향으로 검사
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
					for (k = i + 1, t = j + 1; k <= i + 4; k++, t++)//오른쪽 아래 대각선으로 검사
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;

				}

			}
			else if (i <= 3 && j >= 4 && j <= 10)			//→ ← ↓ ↘ ↙방향으로만 돌이 5개가 모일 수 있음(나머지 방향은 돌이 5개가 모일 수 없으므로 검사가 무의미)
			{
				if (OmokB[i][j] == W || OmokB[i][j] == B)
				{
					for (k = j + 1; k <= j + 4; k++)		//오른쪽 방향 검사
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j + 4 + 1)
						return FALSE;
					for (k = j - 1; k >= j - 4; k--)		//왼쪽 방향 검사
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j - 4 - 1)
						return FALSE;
					for (k = i + 1; k <= i + 4; k++)		//아래 방향으로 검사
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
					for (k = i + 1, t = j + 1; k <= i + 4; k++, t++)//오른쪽 아래 대각선으로 검사
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
					for (k = i + 1, t = j - 1; k <= i + 4; k++, t--)//왼쪽 아래 대각선으로 검사
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
				}
			}
			else if (i <= 3 && j >= 11 && j <= COL - 1)			//→ ← ↓ ↘ ↙방향으로만 돌이 5개가 모일 수 있음(나머지 방향은 돌이 5개가 모일 수 없으므로 검사가 무의미)
			{
				if (OmokB[i][j] == W || OmokB[i][j] == B)
				{

					for (k = j - 1; k >= j - 4; k--)		//왼쪽 방향 검사
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j - 4 - 1)
						return FALSE;
					for (k = i + 1; k <= i + 4; k++)		//아래 방향으로 검사
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;

					for (k = i + 1, t = j - 1; k <= i + 4; k++, t--)//왼쪽 아래 대각선으로 검사
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
				}
			}
			else if (i >= 4 && i <= 10 && j <= 3)			//→  ↑↓ ↘ ↗방향으로만 돌이 5개가 모일 수 있음(나머지 방향은 돌이 5개가 모일 수 없으므로 검사가 무의미)
			{
				if (OmokB[i][j] == W || OmokB[i][j] == B)
				{
					for (k = j + 1; k <= j + 4; k++)		//오른쪽 방향 검사
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j + 4 + 1)
						return FALSE;

					for (k = i - 1; k >= i - 4; k--)		//위
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;

					for (k = i + 1; k <= i + 4; k++)		//아래 방향으로 검사
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
					for (k = i - 1, t = j + 1; k >= i - 4; k--, t++)//위 오른쪽 대각선 검사
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}

					if (k == i - 4 - 1)
						return FALSE;
					for (k = i + 1, t = j + 1; k <= i + 4; k++, t++)//오른쪽 아래 대각선으로 검사
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;

				}
			}
			else if (i >= 4 && i <= 10 && j >= 4 && j <= 10)			//→ ← ↑↓ ↗ ↖ ↘ ↙ 모든 방향으로 돌이 5개가 모일 수 있음(모든 방향으로 검사 해주어야 함)
			{
				if (OmokB[i][j] == W || OmokB[i][j] == B)
				{
					for (k = j + 1; k <= j + 4; k++)					//오른쪽 방향 검사
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j + 4 + 1)
						return FALSE;

					for (k = j - 1; k >= j - 4; k--)					//왼쪽 방향 검사
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j - 4 - 1)
						return FALSE;

					for (k = i - 1; k >= i - 4; k--)					//위
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;

					for (k = i + 1; k <= i + 4; k++)				//아래 방향으로 검사
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
					for (k = i - 1, t = j + 1; k >= i - 4; k--, t++)//위 오른쪽 대각선 검사
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}

					if (k == i - 4 - 1)
						return FALSE;
					for (k = i - 1, t = j - 1; k >= i - 4; k--, t--)//위 왼쪽 대각선 검사
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;

					for (k = i + 1, t = j + 1; k <= i + 4; k++, t++)//오른쪽 아래 대각선으로 검사
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
					for (k = i + 1, t = j - 1; k <= i + 4; k++, t--)//왼쪽 아래 대각선으로 검사
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;

				}

			}
			else if (i >= 4 && i <= 10 && j >= 11 && j <= COL - 1)		//↑↓← ↖↙방향으로만 돌이 5개가 모일 수 있음(나머지 방향은 돌이 5개가 모일 수 없으므로 검사가 무의미)
			{

				if (OmokB[i][j] == W || OmokB[i][j] == B)
				{
					for (k = i - 1; k >= i - 4; k--)					//위
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;
					for (k = i + 1; k <= i + 4; k++)					//아래 방향으로 검사
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;

					for (k = j - 1; k >= j - 4; k--)					//왼쪽 방향 검사
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j - 4 - 1)
						return FALSE;


					for (k = i - 1, t = j - 1; k >= i - 4; k--, t--)	//위 왼쪽 대각선 검사
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;

					for (k = i + 1, t = j - 1; k <= i + 4; k++, t--)	//왼쪽 아래 대각선으로 검사
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
				}
			}
			else if (i >= 11 && j <= 3)									//→ ↗ ↑ 방향으로만 돌이 5개가 모일 수 있음(나머지 방향은 돌이 5개가 모일 수 없으므로 검사가 무의미)
			{
				if (OmokB[i][j] == W || OmokB[i][j] == B)
				{
					for (k = i - 1; k >= i - 4; k--)					//위
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;

					for (k = i - 1, t = j + 1; k >= i - 4; k--, t++)	//위 오른쪽 대각선 검사
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}

					if (k == i - 4 - 1)
						return FALSE;

					for (k = j + 1; k <= j + 4; k++)					//오른쪽 방향 검사
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j + 4 + 1)
						return FALSE;
				}
			}
			else if (i >= 11 && j >= 4 && j <= 10)						//→ ← ↑ ↗↖방향으로만 돌이 5개가 모일 수 있음(나머지 방향은 돌이 5개가 모일 수 없으므로 검사가 무의미)
			{
				if (OmokB[i][j] == W || OmokB[i][j] == B)
				{
					for (k = j - 1; k >= j - 4; k--)					//왼쪽 방향 검사
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j - 4 - 1)
						return FALSE;

					for (k = i - 1, t = j - 1; k >= i - 4; k--, t--)	//위 왼쪽 대각선 검사
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;
					for (k = i - 1; k >= i - 4; k--)					//위
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;

					for (k = i - 1, t = j + 1; k >= i - 4; k--, t++)	//위 오른쪽 대각선 검사
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}

					if (k == i - 4 - 1)
						return FALSE;

					for (k = j + 1; k <= j + 4; k++)					//오른쪽 방향 검사
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j + 4 + 1)
						return FALSE;
				}
			}
			else if (i >= 11 && j >= 11 && j <= COL - 1)				 // ↑ ← ↖방향으로만 돌이 5개가 모일 수 있음(나머지 방향은 돌이 5개가 모일 수 없으므로 검사가 무의미)
			{
				if (OmokB[i][j] == W || OmokB[i][j] == B)
				{
					for (k = i - 1; k >= i - 4; k--)					//위
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;


					for (k = i - 1, t = j - 1; k >= i - 4; k--, t--)	//위 왼쪽 대각선 검사
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;

					for (k = j - 1; k >= j - 4; k--)					//왼쪽 방향 검사
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j - 4 - 1)
						return FALSE;

				}
			}
		}
	}
	return TRUE;														//만약 5개가 모인 돌이 없다면(위에 if문에 걸리지 않아 이곳에 도달한다면 return  TRUE를 통해 게임 계속 진행
}

void OPrintGameStart()
{
	Ogotoxy(6, 7);
	ColorSet(0, 4);
	printf(" ##  #  # # ###      ## ###  #  ##  ###  #");

	Ogotoxy(6, 8);
	printf("#   # # ### #       #    #  # # # #  #   #");

	Ogotoxy(6, 9);
	printf("# # ### ### ##       #   #  ### ##   #   #");

	Ogotoxy(6, 10);
	printf("# # # # # # #         #  #  # # # #  #    ");

	Ogotoxy(6, 11);
	printf(" ## # # # # ###     ##   #  # # # #  #   #");
}