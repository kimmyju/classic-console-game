#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define CROW 8
#define CCOL 8
#define CTRUE 1
#define CFALSE 0

typedef enum BLACK //흑말을 1~9까지 설정
{
	rook1 = 1, knight1 = 2, bishop1 = 3, queen = 4, king = 5, bishop2 = 6, knight2 = 7, rook2 = 8, pawn2 = 9
}P1;

typedef enum WHITE //백말을 11~19까지 설정
{
	rook3 = 11, knight3 = 12, bishop3 = 13, queen3 = 14, king3 = 15, bishop4 = 16, knight4 = 17, rook4 = 18, pawn4 = 19
}P2;

int turn = 0; // 짝수 TURN: 흑 ,홀수 TURN: 백
int LocX, LocY, x, y; //LocX,LocY 이동할 자리의 인덱스 /x,y-> 이동하고 싶은 말의 인덱스
int ChassBoard[CROW][CCOL]; //체스보드

void swap(int*, int*); //자리 이동
int King();
int Queen();
int Rook();
int Bishop();
int Knight();
int Pawn();
void CPrintGameStart();//game start 문구 출력 함수
void InitChassB();  //chass 초기화
void PrintChassB(); //게임판 프린트
void CColorSet(unsigned short back, unsigned int text); //색을 입히는 함수
void Cgotoxy(int CursorX, int CursorY);
int checkGame(); //king이 잡혔는지 확인하는 함수

int Chess()
{
	char player1[5], player2[5], YN;
	int checkTF, GameS;

	SetConsoleTitle(TEXT("체스 게임"));
replay:
	printf("\n<게임 룰>\n\n");
	printf("\n1. 플레이어는 각각 흑과 백을 선택하여 입력한다.\n");
	printf("\n2. 흑과 백은 차례대로 한번씩 자리를 선택하여 말을 놓는다.\n");
	printf("\n3. 상대팀의 KING을 잡으면 승리하는 게임");


	printf("\n\n각각의 PLAYER은 흑과 백을 선택해주세요(입력: b/w)\n"); //Player1과 PLAYER2 말 선택
	printf("PLAYER1: ");
	scanf_s("%s", player1, sizeof(player1));
	printf("PLAYER2: ");
	scanf_s("%s", player2, sizeof(player2));

	if (strcmp(player1, "b\0") == 0)
	{
		printf("흑: PLAYER1, 백: PLAYER2\n");
		printf("player1이 선공합니다!\n");

	}
	else
	{
		printf("흑: PLAYER2, 백: PLAYER1\n");
		printf("player2이 선공합니다!\n");

	}
	CColorSet(0, 4);
	printf("\n\nStart Game>  Enter");
	GameS = getch(); //키보드의 Enter키를 입력 받음
	if (GameS == 13) //Enter의 아스키코드 --> 13 
	{
		system("cls");
		PlaySound(TEXT("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\chessBgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

		CPrintGameStart(); //게임 스타트 문구 출력

		
		InitChassB(); //체스판 초기화


		do
		{
			CColorSet(0, 15);
			Sleep(1000);
			system("cls");
			PrintChassB(); //체스판 출력

			printf("이동할 말의 인덱스(ex -> 2 3): ");
			scanf_s("%d %d", &x, &y); //움직일 말의 인덱스

			printf("이동할 자리(ex -> 5 7): ");
			scanf_s("%d %d", &LocX, &LocY);////LocX,LocY-> 가고싶은 자리의 인덱스

			switch (ChassBoard[x][y]) //사용자가 움직일 말의 인덱스를 입력 받아 그에 해당하는 함수로 이동
			{
			case rook1://rook일때
			case rook2:
			case rook3:
			case rook4:
				checkTF = Rook();
				if (checkTF == CFALSE) // 각 말의 규칙에 알맞게 자리를 입력 했다면 해당 함수에서 swap , 규칙에 맞지 않은 이동을 선택 했을 시, CFALSE를 반환 받아 반복문의 처음으로 이동
					continue;

				break;
			case knight1://knight일때
			case knight2:
			case knight3:
			case knight4:
				checkTF = Knight();
				if (checkTF == CFALSE)
					continue;
				break;
			case bishop1: //bishop
			case bishop2:
			case bishop3:
			case bishop4:

				checkTF = Bishop();
				if (checkTF == CFALSE)
					continue;

				break;
			case queen://queen
			case queen3:
				checkTF = Queen();
				if (checkTF == CFALSE)
					continue;

				break;
			case king://king
			case king3:
				checkTF = King();
				if (checkTF == CFALSE)
					continue;

				break;
			case pawn2://pawn
			case pawn4:
				checkTF = Pawn();
				if (checkTF == CFALSE)
					continue;
				break;
			default:
				printf("잘못된 입력입니다. 다시 입력해주세요\n"); //ChassBoard[x][y]  == 0 즉 말이 없는 자리를 선택 했을 때
				continue;

			}
			CColorSet(0, 15);
			turn++;
		} while (checkGame() != 0);
	}
	turn--;
	system("cls");
	PrintChassB(); //최종 체스판 출력
	if (turn % 2 == 0) //155~176줄 승,패 출력
	{

		CColorSet(0, 14);
		printf("\n★");
		CColorSet(0, 4);
		printf("WIN: 흑");
		CColorSet(0, 1);
		printf("     LOSE: 백\n");

	}
	else
	{

		CColorSet(0, 14);
		printf("\n★");
		CColorSet(0, 4);
		printf("WIN: 백");
		CColorSet(0, 1);
		printf("     LOSE: 흑\n");

	}




	CColorSet(0, 4);
	printf("\nPLAY AGAIN?(y/n): "); //다시 플레이할건지를 입력 받음 
	YN = getche();
	CColorSet(0, 15);
	if (YN == 'y')
	{
		system("cls");
		turn = 0; //turn을 다시 0으로 초기화
		goto replay; //main함수의 처음으로 다시 이동
	}
	else  //n를 입력 했다면
	{
		printf("\n\n<게임을 종료합니다>");
		Sleep(1500);
		return 0;
	}
}

void InitChassB()  //체스판 초기화
{
	P1 pieceB = rook1; // 흑말의 이넘 변수 선언
	P2 pieceW = pawn4; //백말의 이넘 변수 선언
	int i, j, k;


	//ChassBoard[i][j]자리에  공백이면 0, 말이 들어가야 하는 자리라면 해당되는 말의 번호를(piece)삽입;
	for (i = 0; i < CROW; i++)
	{
		for (j = 0, k = CCOL - 1; j < CCOL; j++, k--)
		{
			if (i == 0)
				ChassBoard[i][j] = pieceB++;// 흑말의 enum에 있는 말들을 차례대로 넣어줌(rook1~rook2)

			else if (i == 1)
				ChassBoard[i][j] = pieceB; // 행이 1일때는 전부 흑말의 폰을 넣어줌(pawn2)

			else if (i == 6) //행이 6일때는 전부 백말의 폰을 넣어줌(pawn4)
			{

				if (j <= 6) //행:6,열:0~6까지는 전부 폰만 넣어주는 작업
				{
					ChassBoard[i][j] = pieceW;

				}
				else//행:6,열:7에서는 폰을 한번만 넣고 piece를 하나 감소해줌
				{
					ChassBoard[i][j] = pieceW;
					pieceW--;
				}
			}
			else if (i == 7)
				ChassBoard[i][k] = pieceW--;//백말의 enum에 있는 말들을 차례대로 넣어줌(rook4~rook3)

			else //나머지는 빈칸(0)으로 채워줌
				ChassBoard[i][j] = 0;
		}

	}


}

void PrintChassB()
{
	int i, j;
	if (turn % 2 == 0)
		printf("TURN: 흑\n\n");
	else
		printf("TURN: 백\n\n");
	for (i = 0; i < CROW; i++)
	{
		CColorSet(0, 15);
		printf("    %d", i);
	}
	printf("   \n");
	for (i = 0; i < CROW; i++)
	{
		CColorSet(0, 15);
		printf(" %d ", i);

		for (j = 0; j < CCOL; j++)
		{

			if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) //행:짝, 열:짝 || 행:홀,열:홀일때(칸이 흰색일때)
			{


				if (ChassBoard[i][j] >= 1 && ChassBoard[i][j] <= 9)//흑말 출력
				{
					CColorSet(6, 0);
					if ((ChassBoard[i][j] == rook1) || (ChassBoard[i][j] == rook2))
						printf("  R  ");
					else if ((ChassBoard[i][j] == knight1) || (ChassBoard[i][j] == knight2))
						printf(" Kn  ");
					else if ((ChassBoard[i][j] == bishop1) || (ChassBoard[i][j] == bishop2))
						printf("  B  ");
					else if (ChassBoard[i][j] == queen)
						printf("  Q  ");
					else if (ChassBoard[i][j] == king)
						printf("  K  ");
					else if (ChassBoard[i][j] == pawn2)
						printf("  P  ");


				}

				else if (ChassBoard[i][j] >= 11 && ChassBoard[i][j] <= 19) //백말 출력

				{
					CColorSet(6, 15);

					if ((ChassBoard[i][j] == rook3) || (ChassBoard[i][j] == rook4))
						printf("  R  ");
					else if ((ChassBoard[i][j] == knight3) || (ChassBoard[i][j] == knight4))
						printf(" Kn  ");
					else if ((ChassBoard[i][j] == bishop3) || (ChassBoard[i][j] == bishop4))
						printf("  B  ");
					else if (ChassBoard[i][j] == queen3)
						printf("  Q  ");
					else if (ChassBoard[i][j] == king3)
						printf("  K  ");
					else if (ChassBoard[i][j] == pawn4)
						printf("  P  ");

				}

				else
				{
					CColorSet(6, 6);
					printf("     ");
				}

			}
			else
			{
				if (ChassBoard[i][j] >= 1 && ChassBoard[i][j] <= 9)//흑말 출력
				{
					CColorSet(8, 0);
					if ((ChassBoard[i][j] == rook1) || (ChassBoard[i][j] == rook2))
						printf("  R  ");
					else if ((ChassBoard[i][j] == knight1) || (ChassBoard[i][j] == knight2))
						printf(" Kn  ");
					else if ((ChassBoard[i][j] == bishop1) || (ChassBoard[i][j] == bishop2))
						printf("  B  ");
					else if (ChassBoard[i][j] == queen)
						printf("  Q  ");
					else if (ChassBoard[i][j] == king)
						printf("  K  ");
					else if (ChassBoard[i][j] == pawn2)
						printf("  P  ");



				}

				else if (ChassBoard[i][j] >= 11 && ChassBoard[i][j] <= 19) //백말 출력

				{
					CColorSet(8, 15);

					if ((ChassBoard[i][j] == rook3) || (ChassBoard[i][j] == rook4))
						printf("  R  ");
					else if ((ChassBoard[i][j] == knight3) || (ChassBoard[i][j] == knight4))
						printf(" Kn  ");
					else if ((ChassBoard[i][j] == bishop3) || (ChassBoard[i][j] == bishop4))
						printf("  B  ");
					else if (ChassBoard[i][j] == queen3)
						printf("  Q  ");
					else if (ChassBoard[i][j] == king3)
						printf("  K  ");
					else if (ChassBoard[i][j] == pawn4)
						printf("  P  ");

				}
				else
				{
					CColorSet(8, 8);
					printf("     ");
				}
			}
		}

		printf("\n");

	}

	CColorSet(0, 15);

}

void CPrintGameStart()
{

	Cgotoxy(6, 7);
	CColorSet(0, 4);
	printf(" ##  #  # # ###      ## ###  #  ##  ###  #");

	Cgotoxy(6, 8);
	printf("#   # # ### #       #    #  # # # #  #   #");

	Cgotoxy(6, 9);
	printf("# # ### ### ##       #   #  ### ##   #   #");

	Cgotoxy(6, 10);
	printf("# # # # # # #         #  #  # # # #  #    ");

	Cgotoxy(6, 11);
	printf(" ## # # # # ###     ##   #  # # # #  #   #");
}

void Cgotoxy(int CursorX, int CursorY)
{
	COORD pos = { CursorX, CursorY };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void CColorSet(unsigned short back, unsigned short Ctext)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Ctext | back << 4);

}

int King()//왕
{

	if (turn % 2 == 0) //흑
	{
		if (LocX == x + 1 && LocY == y) //앞으로 한칸
		{
			if (ChassBoard[LocX][LocY] == 0) //자리가 비어 있으면 그냥 이동
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19) //상대방의 말이(여기서 백말) 있으면 공격
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else//우리 편 말이 있으면 main에서 다시 자리를 입력 받도록!
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}

		}
		else if (LocX == x - 1 && LocY == y)// 뒤로 한칸
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				CColorSet(0, 4);
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}

		}
		else if (LocX == x && LocY == y + 1)//오른쪽으로 한칸
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}

		}

		else if (LocX == x && LocY == y - 1)//왼쪽으로 한칸
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}

		}
		else if (LocX == x + 1 && LocY == y - 1)//왼 대각 한칸
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}

		}

		else if (LocX == x + 1 && LocY == y + 1)//오 대각 한칸
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}

		else if (LocX == x - 1 && LocY == y - 1)//왼 대각 아래 한칸
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}

		}

		else if (LocX == x - 1 && LocY == y + 1)//오 대각 아래 한칸
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}

		}

		else
		{
			CColorSet(0, 1);
			printf("※KING의 이동 범위를 벗어났습니다!\n");
			printf("KING은 상 하 좌 우 대각선으로 한칸만 이동 가능합니다\n");
			printf("다시 입력해주세요\n");
			return CFALSE;

		}

	}

	else//백
	{


		if (LocX == x - 1 && LocY == y) //앞으로 한칸
		{
			if (ChassBoard[LocX][LocY] == 0) //이동 하고 싶은 자리가 공백이라면
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9) //이동하고 싶은 자리에 흑말이 있다면
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else if (LocX == x + 1 && LocY == y)// 뒤로 한칸
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}

		}
		else if (LocX == x && LocY == y + 1)//오 
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}

		else if (LocX == x && LocY == y - 1)//왼
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}

		}
		else if (LocX == x - 1 && LocY == y - 1)//왼 대각
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}

		else if (LocX == x - 1 && LocY == y + 1)//오 대각
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}

		else if (LocX == x + 1 && LocY == y - 1)//왼 대각 뒤
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}

		else if (LocX == x + 1 && LocY == y + 1)//오 대각 뒤
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}

		}

		else // 상 하 좌 우 대각선으로 한칸 이상을 선택했다면
		{
			CColorSet(0, 1);
			printf("※KING의 이동 범위를 벗어났습니다!\n");
			printf("KING은 상 하 좌 우 대각선으로 한칸만 이동 가능합니다\n");
			printf("다시 입력해주세요\n");
			return CFALSE;

		}
	}
	return CTRUE;
}

int Queen()//여왕
{
	int i, j;
	if (turn % 2 == 0)//흑
	{
		if (LocY == y && (LocX - x <= 7 || x - LocX <= 7))//상 하 방향-->선택한 말의 인덱스로부터 목적지까지가 상 또는 하 방향인지 먼저 확인
		{


			if (LocX > x) //사용자가 위쪽으로 이동하려 할때, 목적지의 전까지 다른 말이 존재 하는지 검사
				for (i = x + 1; i < LocX; i++)
				{
					if (ChassBoard[i][y] != 0) //사용자가 선택한 목적지의 이전 자리들에 다른 말들이 존재 한다면 return CFALSE 
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			else//사용자가 아래쪽으로 이동하려 할때, 목적지의 전까지 다른 말이 존재 하는지 검사
				for (i = x - 1; i > LocX; i--)
				{
					if (ChassBoard[i][y] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}

			if (ChassBoard[LocX][LocY] == 0) //이동할 목적지가 공백이라면
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}
			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19) //목적지에 상대팀 말(여기서는 백말)이 있다면 공격
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else //ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9 --> 우리 편 말이 있을때 return CFALSE
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}

		else if (LocX == x && (LocY - y <= 7 || y - LocY <= 7))//좌,우 방향-->선택한 말의 인덱스로부터 목적지까지가 좌 또는 우 방향인지 먼저 확인
		{
			if (LocY > y) //오른쪽으로 갈때 검사
				for (i = y + 1; i < LocY; i++)
				{
					if (ChassBoard[x][i] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			else//왼쪽으로 갈때 검사
				for (i = y - 1; i > LocY; i--)
				{
					if (ChassBoard[x][i] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}

			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}
			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else if ((LocX - x <= 7 && LocY - y <= 7) || (LocX - x >= -7 && LocY - y >= -7) || (LocX - x <= 7 && LocY - y >= -7) || (LocX - x >= -7 && LocY - y <= 7))//대각선방향-->선택한 말의 인덱스로부터 목적지까지가 대각선 방향인지 먼저 확인
		{
			if (LocX > x && LocY > y) //위 오른쪽 대각선으로 갈 때 그 이전 자리들을 검사
				for (i = x + 1, j = y + 1; i < LocX; i++, j++)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			else if (LocX > x && LocY < y)//위 왼쪽 대각선으로 갈때 그 이전 자리들을 검사
				for (i = x + 1, j = y - 1; i < LocX; i++, j--)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}

			else if (LocX < x&& LocY < y)//아래 왼쪽 대각선으로 갈 때 그 이전 자리들을 검사
				for (i = x - 1, j = y - 1; i > LocX; i--, j--)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			else//아래 오른쪽 대각선으로 갈 때 그 이전 자리들을 검사
				for (i = x - 1, j = y + 1; i > LocX; i--, j++)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}
			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}

		else //말이 이동할 수 있는 규칙에 어긋났을 때(Queen의 경우 상 하 좌 우 대각선 방향 이동이 아닐 때)
		{
			CColorSet(0, 1);
			printf("※QUEEN의 이동 범위를 벗어났습니다!\n");
			printf("QUEEN은 상 하 좌 우 대각선으로만 이동 가능합니다\n");
			printf("다시 입력해주세요\n");
			return CFALSE;
		}
	}

	else//백
	{
		if (LocY == y && (LocX - x <= 7 || x - LocX <= 7))//위에 흑말일 경우와 동일
		{

			if (LocX > x) //위로 이동 시 검사
				for (i = x + 1; i < LocX; i++)
				{
					if (ChassBoard[i][y] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			else// 아래로 이동 시 검사
				for (i = x - 1; i > LocX; i--)
				{
					if (ChassBoard[i][y] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}

			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}
			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}

		else if (LocX == x && (LocY - y <= 7 || y - LocY <= 7))
		{
			if (LocY > y) //오른쪽으로 갈때 검사
				for (i = y + 1; i < LocY; i++)
				{
					if (ChassBoard[x][i] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			else//왼쪽으로 갈때 검사
				for (i = y - 1; i > LocY; i--)
				{
					if (ChassBoard[x][i] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}

			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}
			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else if ((LocX - x <= 7 && LocY - y <= 7) || (LocX - x >= -7 && LocY - y >= -7) || (LocX - x <= 7 && LocY - y >= -7) || (LocX - x >= -7 && LocY - y <= 7))//대각선
		{
			if (LocX > x && LocY > y) //위 오른쪽 대각선으로 갈 때 검사
				for (i = x + 1, j = y + 1; i < LocX; i++, j++)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			else if (LocX > x && LocY < y)//위 왼쪽 대각선으로 갈때 검사
				for (i = x + 1, j = y - 1; i <= LocX; i++, j--)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}

			else if (LocX < x&& LocY < y)//아래 왼쪽 대각선으로 갈 떄 검사
				for (i = x - 1, j = y - 1; i > LocX; i--, j--)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			else//아래 오른쪽 대각선으로 올라갈때 검사
				for (i = x - 1, j = y + 1; i > LocX; i--, j++)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}
			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}

		else
		{
			CColorSet(0, 1);
			printf("※QUEEN의 이동 범위를 벗어났습니다!\n");
			printf("QUEEN은 상 하 좌 우 대각선으로만 이동 가능합니다\n");
			printf("다시 입력해주세요\n");
			return CFALSE;
		}
	}
	return CTRUE;
}

int Rook()//성
{
	int i, j;
	if (turn % 2 == 0)//흑
	{
		if (LocY == y && (LocX - x <= 7 || x - LocX <= 7))//상 하
		{

			if (LocX > x) //검사
				for (i = x + 1; i < LocX; i++)
				{
					if (ChassBoard[i][y] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return FALSE;

					}
				}
			else//검사
				for (i = x - 1; i > LocX; i--)
				{
					if (ChassBoard[i][y] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}

			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}
			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}

		else if (LocX == x && (LocY - y <= 7 || y - LocY <= 7))//좌,우
		{
			if (LocY > y) //오른쪽으로 갈때 검사
				for (i = y + 1; i < LocY; i++)
				{
					if (ChassBoard[x][i] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			else//왼쪽으로 갈때 검사
				for (i = y - 1; i > LocY; i--)
				{
					if (ChassBoard[x][i] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}

			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}
			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else
		{
			CColorSet(0, 1);
			printf("※ROOK의 이동 범위를 벗어났습니다!\n");
			printf("ROOK은 상 하 좌 우로만 이동 가능합니다\n");
			printf("다시 입력해주세요\n");
			return CFALSE;
		}
	}
	else//백
	{
		if (LocY == y && (LocX - x <= 7 || x - LocX <= 7))//상 하
		{

			if (LocX > x) //검사
				for (i = x + 1; i < LocX; i++)
				{
					if (ChassBoard[i][y] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			else//검사
				for (i = x - 1; i > LocX; i--)
				{
					if (ChassBoard[i][y] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}

			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}
			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}

		else if (LocX == x && (LocY - y <= 7 || y - LocY <= 7))//좌,우
		{
			if (LocY > y) //오른쪽으로 갈때 검사
				for (i = y + 1; i < LocY; i++)
				{
					if (ChassBoard[x][i] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			else//왼쪽으로 갈때 검사
				for (i = y - 1; i > LocY; i--)
				{
					if (ChassBoard[x][i] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}

			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}
			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else
		{
			CColorSet(0, 1);
			printf("※ROOK의 이동 범위를 벗어났습니다!\n");
			printf("ROOK은 상 하 좌 우로만 이동 가능합니다\n");
			printf("다시 입력해주세요\n");
			return CFALSE;
		}
	}
	return CTRUE;
}

int Bishop()//비숍
{
	int i, j;
	if (turn % 2 == 0)
	{

		if ((LocX - x <= 7 && LocY - y <= 7) || (LocX - x >= -7 && LocY - y >= -7) || (LocX - x <= 7 && LocY - y >= -7) || (LocX - x >= -7 && LocY - y <= 7))
		{
			if (LocX > x && LocY > y) //오른쪽 대각선으로 갈 때 검사
				for (i = x + 1, j = y + 1; i < LocX; i++, j++)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			else if (LocX > x && LocY < y)//왼쪽 대각선으로 갈때 검사
				for (i = x + 1, j = y - 1; i < LocX; i++, j--)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}

			else if (LocX < x&& LocY < y)//아래 왼쪽 대각선으로 갈 떄 검사
				for (i = x - 1, j = y - 1; i > LocX; i--, j--)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			else//아래 오른쪽 대각선으로 올라갈때 검사
				for (i = x - 1, j = y + 1; i > LocX; i--, j++)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}

		else
		{
			CColorSet(0, 1);
			printf("※BISHOP의 이동 범위를 벗어났습니다!\n");
			printf("BISHOP은 대각선으로만 이동 가능합니다\n");
			printf("다시 입력해주세요\n");
			return CFALSE;
		}
	}

	else
	{

		if ((LocX - x <= 7 && LocY - y <= 7) || (LocX - x >= -7 && LocY - y >= -7) || (LocX - x <= 7 && LocY - y >= -7) || (LocX - x >= -7 && LocY - y <= 7))
		{
			if (LocX > x && LocY > y) //오른쪽 대각선으로 갈 때 검사
				for (i = x + 1, j = y + 1; i < LocX; i++, j++)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			else if (LocX > x && LocY < y)//왼쪽 대각선으로 갈때 검사
				for (i = x + 1, j = y - 1; i < LocX; i++, j--)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}

			else if (LocX < x&& LocY < y)//아래 왼쪽 대각선으로 갈 떄 검사
				for (i = x - 1, j = y - 1; i > LocX; i--, j--)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			else//아래 오른쪽 대각선으로 올라갈때 검사
				for (i = x - 1, j = y + 1; i > LocX; i--, j++)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("이동할 수 없는 경로! 자리를 다시 선택하세요\n");
						return CFALSE;

					}
				}
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}

		else
		{
			CColorSet(0, 1);
			printf("※BISHOP의 이동 범위를 벗어났습니다!\n");
			printf("BISHOP은 대각선으로만 이동 가능합니다\n");
			printf("다시 입력해주세요\n");
			return CFALSE;
		}
	}
	return CTRUE;
}

int Knight()
{
	if (turn % 2 == 0)//흑말일 경우
	{
		if ((LocX == x + 1 && LocY == y + 2) || (LocX == x + 1 && LocY == y - 2)) //위 한칸 오2,위 한칸 왼 2으로 이동할 때
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}

		}
		else if ((LocX == x + 2 && LocY == y + 1) || (LocX == x + 2 && LocY == y - 1))//위 2 오1, 위 2 왼1
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}

		else if ((LocY == y - 1 && LocX == x + 2) || (LocY == y + 1 && LocX == x + 2)) //왼 1 위2 , 오1 위2
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else if ((LocY == y - 1 && LocX == x - 2) || (LocY == y + 1 && LocX == x - 2)) //왼 1 아2 , 오1 아2
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else if ((LocY == y - 2 && LocX == x + 1) || (LocY == y + 2 && LocX == x + 1)) //왼 2 위1 , 오2 위1
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else if ((LocY == y - 2 && LocX == x - 1) || (LocY == y + 2 && LocX == x - 1)) //왼 2 아1 , 오2 아1
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else if ((LocX == x - 1 && LocY == y - 2) || (LocX == x - 1 && LocY == y + 2)) //아1 왼2 , 아1 오2
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else if ((LocX == x - 2 && LocY == y - 1) || (LocX == x - 2 && LocY == y + 1)) //아2 왼1 , 아2 오1
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else
		{
			CColorSet(0, 1);
			printf("※KNIGHT의 이동 범위를 벗어났습니다!\n");

			printf("다시 입력해주세요\n");
			return CFALSE;
		}
	}
	else //백
	{
		if ((LocX == x + 1 && LocY == y + 2) || (LocX == x + 1 && LocY == y - 2)) //위 한칸 오2,위 한칸 왼 2
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}

		}
		else if ((LocX == x + 2 && LocY == y + 1) || (LocX == x + 2 && LocY == y - 1))//위 2 오1, 위 2 왼1
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}

		else if ((LocY == y - 1 && LocX == x + 2) || (LocY == y + 1 && LocX == x + 2)) //왼 1 위2 , 오1 위2
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else if ((LocY == y - 1 && LocX == x - 2) || (LocY == y + 1 && LocX == x - 2)) //왼 1 아2 , 오1 아2
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else if ((LocY == y - 2 && LocX == x + 1) || (LocY == y + 2 && LocX == x + 1)) //왼 2 위1 , 오2 위1
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else if ((LocY == y - 2 && LocX == x - 1) || (LocY == y + 2 && LocX == x - 1)) //왼 2 아1 , 오2 아1
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else if ((LocX == x - 1 && LocY == y - 2) || (LocX == x - 1 && LocY == y + 2)) //아1 왼2 , 아1 오2
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else if ((LocX == x - 2 && LocY == y - 1) || (LocX == x - 2 && LocY == y + 1)) //아2 왼1 , 아2 오1
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("공격 성공!\n");
			}
			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else
		{
			CColorSet(0, 1);
			printf("※KNIGHT의 이동 범위를 벗어났습니다!\n");

			printf("다시 입력해주세요\n");
			return CFALSE;
		}
	}
	return CTRUE;
}

int Pawn()//쫄병--> 처음 이동할 경우 최대 두칸, 그 외에는 한칸을 이동함을 고려해 주어야 함.
{

	if (turn % 2 == 0) //흑말
	{

		//아래 x == 1 --> 폰이 한번도 움직이지 않은 상태(흑말의 폰이 ChassBoard의 1행에 있을 때)일때는 최대 두칸을 이동할 수 있도록 함
		if (x == 1 && ((y == LocY) && (LocX - x == 1 || LocX - x == 2))) //흑:처음 앞으로 이동할때,(y == LocY) && (((LocX - x) == 1 || (LocX - x) == 2)) == 1) -> (열은 동일) && (이동하고 싶은 자리의 행과 현재 위치의 행이 1칸 or 2칸 차이일 때)
		{
			if (ChassBoard[LocX][LocY] == 0) //이동하고 싶은 자리가 비어있다면(0이라면)
			{
				swap(&ChassBoard[LocX][LocY], &ChassBoard[x][y]);


			}
			else //이동하고 싶은 자리에 다른 말이 존재 한다면
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else if (y == LocY && LocX - x == 1) //흑(처음 움직이는게 아닐때): 앞으로 한칸 이동만 가능
		{
			if (ChassBoard[LocX][LocY] == 0)
			{

				swap(&ChassBoard[LocX][LocY], &ChassBoard[x][y]);

			}

			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}



		else if (LocX == x + 1 && LocY == y - 1)//흑: 왼쪽 대각선으로 공격
		{

			if (ChassBoard[LocX][LocY] == 0)
			{
				printf("공격할 말이 없습니다!자리를 다시 입력해 주세요\n");
				return CFALSE;
			}
			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0; //공격 당한 말이 잡혔으니까 자리의 요소 -> 0
				swap(&ChassBoard[LocX][LocY], &ChassBoard[x][y]);// 
				CColorSet(0, 4);
				printf("공격 성공!\n");

			}
			else
			{
				printf("이동할 수 없는 경로! 자리를 다시 선택 해주세요\n");
				return CFALSE;
			}

		}

		else if (LocX == x + 1 && LocY == y + 1)//흑: 오른쪽 대각선으로 공격
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				printf("공격할 말이 없습니다!자리를 다시 입력해 주세요\n");
				return CFALSE;
			}
			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0; //공격 당한 말이 잡혔으니까 자리의 요소 -> 0
				swap(&ChassBoard[LocX][LocY], &ChassBoard[x][y]);// 
				CColorSet(0, 4);
				printf("공격 성공!\n");

			}
			else
			{
				printf("이동할 수 없는 경로! 자리를 다시 선택 해주세요\n");
				return CFALSE;
			}
		}

		else
		{
			CColorSet(0, 1);
			printf("※PAWN의 이동 범위를 벗어났습니다!\n");
			printf("다시 입력해주세요\n");
			return CFALSE;
		}
	}

	else
	{

		if (x == 6 && (y == LocY) && (x - LocX == 1 || x - LocX == 2))  //백이 처음 앞으로 이동할때, (y == LocY) && ((x - LocX) == 1) -> (열은 동일) && (이동하고 싶은 자리의 행과 현재 위치의 행이 1칸 or2칸 차이일 때)
		{
			if (ChassBoard[LocX][LocY] == 0)//이동하고 싶은 자리가 비어있다면(0이라면)
			{
				swap(&ChassBoard[LocX][LocY], &ChassBoard[x][y]);


			}
			else //이동 하고싶은 자리에 다른 말이 있을 때
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}
		else if ((y == LocY) && (x - LocX == 1))//백말이 처음 이동하는것이 아닐 때, (y == LocY) && ((x - LocX) == 1) -> (열은 동일) && (이동하고 싶은 자리의 행과 현재 위치의 행이 1칸 차이일 때)
		{

			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[LocX][LocY], &ChassBoard[x][y]);

			}

			else
			{
				printf("이동할 자리에 이미 말이 존재 합니다! 자리를 다시 선택하세요\n");
				return CFALSE;
			}
		}

		else if (LocX == x - 1 && LocY == y - 1)//백: 왼쪽 대각선으로 공격
		{

			if (ChassBoard[LocX][LocY] == 0)//자리가 비어 있다면
			{
				printf("공격할 말이 없습니다!자리를 다시 입력해 주세요\n");
				return CFALSE;

			}
			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0; //공격 당한 말이 잡혔으니까 자리의 요소 -> 0
				swap(&ChassBoard[LocX][LocY], &ChassBoard[x][y]);// 
				CColorSet(0, 4);
				printf("공격 성공!\n");

			}
			else
			{
				printf("이동할 수 없는 경로! 자리를 다시 선택 해주세요\n");
				return CFALSE;
			}
		}

		else if (LocX == x - 1 && LocY == y + 1)//백: 오른쪽 대각선으로 공격
		{
			if (ChassBoard[LocX][LocY] == 0) //자리가 비어 있다면
			{
				printf("공격할 말이 없습니다!자리를 다시 입력해 주세요\n");
				return CFALSE;

			}
			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0; //공격 당한 말이 잡혔으니까 자리의 요소 -> 0
				swap(&ChassBoard[LocX][LocY], &ChassBoard[x][y]);// 
				CColorSet(0, 4);
				printf("공격 성공!\n");

			}
			else
			{
				printf("이동할 수 없는 경로! 자리를 다시 선택 해주세요\n");
				return CFALSE;
			}
		}
		else
		{
			CColorSet(0, 1);
			printf("※PAWN의 이동 범위를 벗어났습니다!\n");
			printf("다시 입력해주세요\n");
			return CFALSE;

		}

	}
	return CTRUE;
}

void swap(int* a, int* b)//배열 값을 바꿔주기 위한 함수
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

int checkGame() //king이 잡혔는지를 확인하는 함수
{
	int i, j, CountK = 0;
	for (i = 0; i < CROW; i++)
		for (j = 0; j < CCOL; j++)
			if (ChassBoard[i][j] == 5 || ChassBoard[i][j] == 15) //5는 흑말의 king, 15는 백말의 king
				CountK++; //king의 숫자를 세어줌

	if (CountK == 2) //만약 두명의 king이 모두 살아 있다면
		return CTRUE;
	else //king이 한명이라면
		return CFALSE;
}
