#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define CROW 8
#define CCOL 8
#define CTRUE 1
#define CFALSE 0

typedef enum BLACK //�渻�� 1~9���� ����
{
	rook1 = 1, knight1 = 2, bishop1 = 3, queen = 4, king = 5, bishop2 = 6, knight2 = 7, rook2 = 8, pawn2 = 9
}P1;

typedef enum WHITE //�鸻�� 11~19���� ����
{
	rook3 = 11, knight3 = 12, bishop3 = 13, queen3 = 14, king3 = 15, bishop4 = 16, knight4 = 17, rook4 = 18, pawn4 = 19
}P2;

int turn = 0; // ¦�� TURN: �� ,Ȧ�� TURN: ��
int LocX, LocY, x, y; //LocX,LocY �̵��� �ڸ��� �ε��� /x,y-> �̵��ϰ� ���� ���� �ε���
int ChassBoard[CROW][CCOL]; //ü������

void swap(int*, int*); //�ڸ� �̵�
int King();
int Queen();
int Rook();
int Bishop();
int Knight();
int Pawn();
void CPrintGameStart();//game start ���� ��� �Լ�
void InitChassB();  //chass �ʱ�ȭ
void PrintChassB(); //������ ����Ʈ
void CColorSet(unsigned short back, unsigned int text); //���� ������ �Լ�
void Cgotoxy(int CursorX, int CursorY);
int checkGame(); //king�� �������� Ȯ���ϴ� �Լ�

int Chess()
{
	char player1[5], player2[5], YN;
	int checkTF, GameS;

	SetConsoleTitle(TEXT("ü�� ����"));
replay:
	printf("\n<���� ��>\n\n");
	printf("\n1. �÷��̾�� ���� ��� ���� �����Ͽ� �Է��Ѵ�.\n");
	printf("\n2. ��� ���� ���ʴ�� �ѹ��� �ڸ��� �����Ͽ� ���� ���´�.\n");
	printf("\n3. ������� KING�� ������ �¸��ϴ� ����");


	printf("\n\n������ PLAYER�� ��� ���� �������ּ���(�Է�: b/w)\n"); //Player1�� PLAYER2 �� ����
	printf("PLAYER1: ");
	scanf_s("%s", player1, sizeof(player1));
	printf("PLAYER2: ");
	scanf_s("%s", player2, sizeof(player2));

	if (strcmp(player1, "b\0") == 0)
	{
		printf("��: PLAYER1, ��: PLAYER2\n");
		printf("player1�� �����մϴ�!\n");

	}
	else
	{
		printf("��: PLAYER2, ��: PLAYER1\n");
		printf("player2�� �����մϴ�!\n");

	}
	CColorSet(0, 4);
	printf("\n\nStart Game>  Enter");
	GameS = getch(); //Ű������ EnterŰ�� �Է� ����
	if (GameS == 13) //Enter�� �ƽ�Ű�ڵ� --> 13 
	{
		system("cls");
		PlaySound(TEXT("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\chessBgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

		CPrintGameStart(); //���� ��ŸƮ ���� ���

		
		InitChassB(); //ü���� �ʱ�ȭ


		do
		{
			CColorSet(0, 15);
			Sleep(1000);
			system("cls");
			PrintChassB(); //ü���� ���

			printf("�̵��� ���� �ε���(ex -> 2 3): ");
			scanf_s("%d %d", &x, &y); //������ ���� �ε���

			printf("�̵��� �ڸ�(ex -> 5 7): ");
			scanf_s("%d %d", &LocX, &LocY);////LocX,LocY-> ������� �ڸ��� �ε���

			switch (ChassBoard[x][y]) //����ڰ� ������ ���� �ε����� �Է� �޾� �׿� �ش��ϴ� �Լ��� �̵�
			{
			case rook1://rook�϶�
			case rook2:
			case rook3:
			case rook4:
				checkTF = Rook();
				if (checkTF == CFALSE) // �� ���� ��Ģ�� �˸°� �ڸ��� �Է� �ߴٸ� �ش� �Լ����� swap , ��Ģ�� ���� ���� �̵��� ���� ���� ��, CFALSE�� ��ȯ �޾� �ݺ����� ó������ �̵�
					continue;

				break;
			case knight1://knight�϶�
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
				printf("�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���\n"); //ChassBoard[x][y]  == 0 �� ���� ���� �ڸ��� ���� ���� ��
				continue;

			}
			CColorSet(0, 15);
			turn++;
		} while (checkGame() != 0);
	}
	turn--;
	system("cls");
	PrintChassB(); //���� ü���� ���
	if (turn % 2 == 0) //155~176�� ��,�� ���
	{

		CColorSet(0, 14);
		printf("\n��");
		CColorSet(0, 4);
		printf("WIN: ��");
		CColorSet(0, 1);
		printf("     LOSE: ��\n");

	}
	else
	{

		CColorSet(0, 14);
		printf("\n��");
		CColorSet(0, 4);
		printf("WIN: ��");
		CColorSet(0, 1);
		printf("     LOSE: ��\n");

	}




	CColorSet(0, 4);
	printf("\nPLAY AGAIN?(y/n): "); //�ٽ� �÷����Ұ����� �Է� ���� 
	YN = getche();
	CColorSet(0, 15);
	if (YN == 'y')
	{
		system("cls");
		turn = 0; //turn�� �ٽ� 0���� �ʱ�ȭ
		goto replay; //main�Լ��� ó������ �ٽ� �̵�
	}
	else  //n�� �Է� �ߴٸ�
	{
		printf("\n\n<������ �����մϴ�>");
		Sleep(1500);
		return 0;
	}
}

void InitChassB()  //ü���� �ʱ�ȭ
{
	P1 pieceB = rook1; // �渻�� �̳� ���� ����
	P2 pieceW = pawn4; //�鸻�� �̳� ���� ����
	int i, j, k;


	//ChassBoard[i][j]�ڸ���  �����̸� 0, ���� ���� �ϴ� �ڸ���� �ش�Ǵ� ���� ��ȣ��(piece)����;
	for (i = 0; i < CROW; i++)
	{
		for (j = 0, k = CCOL - 1; j < CCOL; j++, k--)
		{
			if (i == 0)
				ChassBoard[i][j] = pieceB++;// �渻�� enum�� �ִ� ������ ���ʴ�� �־���(rook1~rook2)

			else if (i == 1)
				ChassBoard[i][j] = pieceB; // ���� 1�϶��� ���� �渻�� ���� �־���(pawn2)

			else if (i == 6) //���� 6�϶��� ���� �鸻�� ���� �־���(pawn4)
			{

				if (j <= 6) //��:6,��:0~6������ ���� ���� �־��ִ� �۾�
				{
					ChassBoard[i][j] = pieceW;

				}
				else//��:6,��:7������ ���� �ѹ��� �ְ� piece�� �ϳ� ��������
				{
					ChassBoard[i][j] = pieceW;
					pieceW--;
				}
			}
			else if (i == 7)
				ChassBoard[i][k] = pieceW--;//�鸻�� enum�� �ִ� ������ ���ʴ�� �־���(rook4~rook3)

			else //�������� ��ĭ(0)���� ä����
				ChassBoard[i][j] = 0;
		}

	}


}

void PrintChassB()
{
	int i, j;
	if (turn % 2 == 0)
		printf("TURN: ��\n\n");
	else
		printf("TURN: ��\n\n");
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

			if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) //��:¦, ��:¦ || ��:Ȧ,��:Ȧ�϶�(ĭ�� ����϶�)
			{


				if (ChassBoard[i][j] >= 1 && ChassBoard[i][j] <= 9)//�渻 ���
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

				else if (ChassBoard[i][j] >= 11 && ChassBoard[i][j] <= 19) //�鸻 ���

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
				if (ChassBoard[i][j] >= 1 && ChassBoard[i][j] <= 9)//�渻 ���
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

				else if (ChassBoard[i][j] >= 11 && ChassBoard[i][j] <= 19) //�鸻 ���

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

int King()//��
{

	if (turn % 2 == 0) //��
	{
		if (LocX == x + 1 && LocY == y) //������ ��ĭ
		{
			if (ChassBoard[LocX][LocY] == 0) //�ڸ��� ��� ������ �׳� �̵�
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19) //������ ����(���⼭ �鸻) ������ ����
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("���� ����!\n");
			}
			else//�츮 �� ���� ������ main���� �ٽ� �ڸ��� �Է� �޵���!
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}

		}
		else if (LocX == x - 1 && LocY == y)// �ڷ� ��ĭ
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				CColorSet(0, 4);
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}

		}
		else if (LocX == x && LocY == y + 1)//���������� ��ĭ
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}

		}

		else if (LocX == x && LocY == y - 1)//�������� ��ĭ
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}

		}
		else if (LocX == x + 1 && LocY == y - 1)//�� �밢 ��ĭ
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}

		}

		else if (LocX == x + 1 && LocY == y + 1)//�� �밢 ��ĭ
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}

		else if (LocX == x - 1 && LocY == y - 1)//�� �밢 �Ʒ� ��ĭ
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}

		}

		else if (LocX == x - 1 && LocY == y + 1)//�� �밢 �Ʒ� ��ĭ
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}

		}

		else
		{
			CColorSet(0, 1);
			printf("��KING�� �̵� ������ ������ϴ�!\n");
			printf("KING�� �� �� �� �� �밢������ ��ĭ�� �̵� �����մϴ�\n");
			printf("�ٽ� �Է����ּ���\n");
			return CFALSE;

		}

	}

	else//��
	{


		if (LocX == x - 1 && LocY == y) //������ ��ĭ
		{
			if (ChassBoard[LocX][LocY] == 0) //�̵� �ϰ� ���� �ڸ��� �����̶��
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9) //�̵��ϰ� ���� �ڸ��� �渻�� �ִٸ�
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else if (LocX == x + 1 && LocY == y)// �ڷ� ��ĭ
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}

		}
		else if (LocX == x && LocY == y + 1)//�� 
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}

		else if (LocX == x && LocY == y - 1)//��
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}

		}
		else if (LocX == x - 1 && LocY == y - 1)//�� �밢
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}

		else if (LocX == x - 1 && LocY == y + 1)//�� �밢
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}

		else if (LocX == x + 1 && LocY == y - 1)//�� �밢 ��
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}

		else if (LocX == x + 1 && LocY == y + 1)//�� �밢 ��
		{
			if (ChassBoard[LocX][LocY] == 0)
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}

		}

		else // �� �� �� �� �밢������ ��ĭ �̻��� �����ߴٸ�
		{
			CColorSet(0, 1);
			printf("��KING�� �̵� ������ ������ϴ�!\n");
			printf("KING�� �� �� �� �� �밢������ ��ĭ�� �̵� �����մϴ�\n");
			printf("�ٽ� �Է����ּ���\n");
			return CFALSE;

		}
	}
	return CTRUE;
}

int Queen()//����
{
	int i, j;
	if (turn % 2 == 0)//��
	{
		if (LocY == y && (LocX - x <= 7 || x - LocX <= 7))//�� �� ����-->������ ���� �ε����κ��� ������������ �� �Ǵ� �� �������� ���� Ȯ��
		{


			if (LocX > x) //����ڰ� �������� �̵��Ϸ� �Ҷ�, �������� ������ �ٸ� ���� ���� �ϴ��� �˻�
				for (i = x + 1; i < LocX; i++)
				{
					if (ChassBoard[i][y] != 0) //����ڰ� ������ �������� ���� �ڸ��鿡 �ٸ� ������ ���� �Ѵٸ� return CFALSE 
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}
			else//����ڰ� �Ʒ������� �̵��Ϸ� �Ҷ�, �������� ������ �ٸ� ���� ���� �ϴ��� �˻�
				for (i = x - 1; i > LocX; i--)
				{
					if (ChassBoard[i][y] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}

			if (ChassBoard[LocX][LocY] == 0) //�̵��� �������� �����̶��
			{
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);

			}
			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19) //�������� ����� ��(���⼭�� �鸻)�� �ִٸ� ����
			{
				ChassBoard[LocX][LocY] = 0;
				swap(&ChassBoard[x][y], &ChassBoard[LocX][LocY]);
				CColorSet(0, 4);
				printf("���� ����!\n");
			}
			else //ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9 --> �츮 �� ���� ������ return CFALSE
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}

		else if (LocX == x && (LocY - y <= 7 || y - LocY <= 7))//��,�� ����-->������ ���� �ε����κ��� ������������ �� �Ǵ� �� �������� ���� Ȯ��
		{
			if (LocY > y) //���������� ���� �˻�
				for (i = y + 1; i < LocY; i++)
				{
					if (ChassBoard[x][i] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}
			else//�������� ���� �˻�
				for (i = y - 1; i > LocY; i--)
				{
					if (ChassBoard[x][i] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else if ((LocX - x <= 7 && LocY - y <= 7) || (LocX - x >= -7 && LocY - y >= -7) || (LocX - x <= 7 && LocY - y >= -7) || (LocX - x >= -7 && LocY - y <= 7))//�밢������-->������ ���� �ε����κ��� ������������ �밢�� �������� ���� Ȯ��
		{
			if (LocX > x && LocY > y) //�� ������ �밢������ �� �� �� ���� �ڸ����� �˻�
				for (i = x + 1, j = y + 1; i < LocX; i++, j++)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}
			else if (LocX > x && LocY < y)//�� ���� �밢������ ���� �� ���� �ڸ����� �˻�
				for (i = x + 1, j = y - 1; i < LocX; i++, j--)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}

			else if (LocX < x&& LocY < y)//�Ʒ� ���� �밢������ �� �� �� ���� �ڸ����� �˻�
				for (i = x - 1, j = y - 1; i > LocX; i--, j--)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}
			else//�Ʒ� ������ �밢������ �� �� �� ���� �ڸ����� �˻�
				for (i = x - 1, j = y + 1; i > LocX; i--, j++)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}

		else //���� �̵��� �� �ִ� ��Ģ�� ��߳��� ��(Queen�� ��� �� �� �� �� �밢�� ���� �̵��� �ƴ� ��)
		{
			CColorSet(0, 1);
			printf("��QUEEN�� �̵� ������ ������ϴ�!\n");
			printf("QUEEN�� �� �� �� �� �밢�����θ� �̵� �����մϴ�\n");
			printf("�ٽ� �Է����ּ���\n");
			return CFALSE;
		}
	}

	else//��
	{
		if (LocY == y && (LocX - x <= 7 || x - LocX <= 7))//���� �渻�� ���� ����
		{

			if (LocX > x) //���� �̵� �� �˻�
				for (i = x + 1; i < LocX; i++)
				{
					if (ChassBoard[i][y] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}
			else// �Ʒ��� �̵� �� �˻�
				for (i = x - 1; i > LocX; i--)
				{
					if (ChassBoard[i][y] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}

		else if (LocX == x && (LocY - y <= 7 || y - LocY <= 7))
		{
			if (LocY > y) //���������� ���� �˻�
				for (i = y + 1; i < LocY; i++)
				{
					if (ChassBoard[x][i] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}
			else//�������� ���� �˻�
				for (i = y - 1; i > LocY; i--)
				{
					if (ChassBoard[x][i] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else if ((LocX - x <= 7 && LocY - y <= 7) || (LocX - x >= -7 && LocY - y >= -7) || (LocX - x <= 7 && LocY - y >= -7) || (LocX - x >= -7 && LocY - y <= 7))//�밢��
		{
			if (LocX > x && LocY > y) //�� ������ �밢������ �� �� �˻�
				for (i = x + 1, j = y + 1; i < LocX; i++, j++)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}
			else if (LocX > x && LocY < y)//�� ���� �밢������ ���� �˻�
				for (i = x + 1, j = y - 1; i <= LocX; i++, j--)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}

			else if (LocX < x&& LocY < y)//�Ʒ� ���� �밢������ �� �� �˻�
				for (i = x - 1, j = y - 1; i > LocX; i--, j--)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}
			else//�Ʒ� ������ �밢������ �ö󰥶� �˻�
				for (i = x - 1, j = y + 1; i > LocX; i--, j++)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}

		else
		{
			CColorSet(0, 1);
			printf("��QUEEN�� �̵� ������ ������ϴ�!\n");
			printf("QUEEN�� �� �� �� �� �밢�����θ� �̵� �����մϴ�\n");
			printf("�ٽ� �Է����ּ���\n");
			return CFALSE;
		}
	}
	return CTRUE;
}

int Rook()//��
{
	int i, j;
	if (turn % 2 == 0)//��
	{
		if (LocY == y && (LocX - x <= 7 || x - LocX <= 7))//�� ��
		{

			if (LocX > x) //�˻�
				for (i = x + 1; i < LocX; i++)
				{
					if (ChassBoard[i][y] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return FALSE;

					}
				}
			else//�˻�
				for (i = x - 1; i > LocX; i--)
				{
					if (ChassBoard[i][y] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}

		else if (LocX == x && (LocY - y <= 7 || y - LocY <= 7))//��,��
		{
			if (LocY > y) //���������� ���� �˻�
				for (i = y + 1; i < LocY; i++)
				{
					if (ChassBoard[x][i] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}
			else//�������� ���� �˻�
				for (i = y - 1; i > LocY; i--)
				{
					if (ChassBoard[x][i] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else
		{
			CColorSet(0, 1);
			printf("��ROOK�� �̵� ������ ������ϴ�!\n");
			printf("ROOK�� �� �� �� ��θ� �̵� �����մϴ�\n");
			printf("�ٽ� �Է����ּ���\n");
			return CFALSE;
		}
	}
	else//��
	{
		if (LocY == y && (LocX - x <= 7 || x - LocX <= 7))//�� ��
		{

			if (LocX > x) //�˻�
				for (i = x + 1; i < LocX; i++)
				{
					if (ChassBoard[i][y] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}
			else//�˻�
				for (i = x - 1; i > LocX; i--)
				{
					if (ChassBoard[i][y] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}

		else if (LocX == x && (LocY - y <= 7 || y - LocY <= 7))//��,��
		{
			if (LocY > y) //���������� ���� �˻�
				for (i = y + 1; i < LocY; i++)
				{
					if (ChassBoard[x][i] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}
			else//�������� ���� �˻�
				for (i = y - 1; i > LocY; i--)
				{
					if (ChassBoard[x][i] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else
		{
			CColorSet(0, 1);
			printf("��ROOK�� �̵� ������ ������ϴ�!\n");
			printf("ROOK�� �� �� �� ��θ� �̵� �����մϴ�\n");
			printf("�ٽ� �Է����ּ���\n");
			return CFALSE;
		}
	}
	return CTRUE;
}

int Bishop()//���
{
	int i, j;
	if (turn % 2 == 0)
	{

		if ((LocX - x <= 7 && LocY - y <= 7) || (LocX - x >= -7 && LocY - y >= -7) || (LocX - x <= 7 && LocY - y >= -7) || (LocX - x >= -7 && LocY - y <= 7))
		{
			if (LocX > x && LocY > y) //������ �밢������ �� �� �˻�
				for (i = x + 1, j = y + 1; i < LocX; i++, j++)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}
			else if (LocX > x && LocY < y)//���� �밢������ ���� �˻�
				for (i = x + 1, j = y - 1; i < LocX; i++, j--)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}

			else if (LocX < x&& LocY < y)//�Ʒ� ���� �밢������ �� �� �˻�
				for (i = x - 1, j = y - 1; i > LocX; i--, j--)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}
			else//�Ʒ� ������ �밢������ �ö󰥶� �˻�
				for (i = x - 1, j = y + 1; i > LocX; i--, j++)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}

		else
		{
			CColorSet(0, 1);
			printf("��BISHOP�� �̵� ������ ������ϴ�!\n");
			printf("BISHOP�� �밢�����θ� �̵� �����մϴ�\n");
			printf("�ٽ� �Է����ּ���\n");
			return CFALSE;
		}
	}

	else
	{

		if ((LocX - x <= 7 && LocY - y <= 7) || (LocX - x >= -7 && LocY - y >= -7) || (LocX - x <= 7 && LocY - y >= -7) || (LocX - x >= -7 && LocY - y <= 7))
		{
			if (LocX > x && LocY > y) //������ �밢������ �� �� �˻�
				for (i = x + 1, j = y + 1; i < LocX; i++, j++)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}
			else if (LocX > x && LocY < y)//���� �밢������ ���� �˻�
				for (i = x + 1, j = y - 1; i < LocX; i++, j--)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}

			else if (LocX < x&& LocY < y)//�Ʒ� ���� �밢������ �� �� �˻�
				for (i = x - 1, j = y - 1; i > LocX; i--, j--)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
						return CFALSE;

					}
				}
			else//�Ʒ� ������ �밢������ �ö󰥶� �˻�
				for (i = x - 1, j = y + 1; i > LocX; i--, j++)
				{
					if (ChassBoard[i][j] != 0)
					{

						printf("�̵��� �� ���� ���! �ڸ��� �ٽ� �����ϼ���\n");
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}

		else
		{
			CColorSet(0, 1);
			printf("��BISHOP�� �̵� ������ ������ϴ�!\n");
			printf("BISHOP�� �밢�����θ� �̵� �����մϴ�\n");
			printf("�ٽ� �Է����ּ���\n");
			return CFALSE;
		}
	}
	return CTRUE;
}

int Knight()
{
	if (turn % 2 == 0)//�渻�� ���
	{
		if ((LocX == x + 1 && LocY == y + 2) || (LocX == x + 1 && LocY == y - 2)) //�� ��ĭ ��2,�� ��ĭ �� 2���� �̵��� ��
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}

		}
		else if ((LocX == x + 2 && LocY == y + 1) || (LocX == x + 2 && LocY == y - 1))//�� 2 ��1, �� 2 ��1
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}

		else if ((LocY == y - 1 && LocX == x + 2) || (LocY == y + 1 && LocX == x + 2)) //�� 1 ��2 , ��1 ��2
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else if ((LocY == y - 1 && LocX == x - 2) || (LocY == y + 1 && LocX == x - 2)) //�� 1 ��2 , ��1 ��2
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else if ((LocY == y - 2 && LocX == x + 1) || (LocY == y + 2 && LocX == x + 1)) //�� 2 ��1 , ��2 ��1
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else if ((LocY == y - 2 && LocX == x - 1) || (LocY == y + 2 && LocX == x - 1)) //�� 2 ��1 , ��2 ��1
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else if ((LocX == x - 1 && LocY == y - 2) || (LocX == x - 1 && LocY == y + 2)) //��1 ��2 , ��1 ��2
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else if ((LocX == x - 2 && LocY == y - 1) || (LocX == x - 2 && LocY == y + 1)) //��2 ��1 , ��2 ��1
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else
		{
			CColorSet(0, 1);
			printf("��KNIGHT�� �̵� ������ ������ϴ�!\n");

			printf("�ٽ� �Է����ּ���\n");
			return CFALSE;
		}
	}
	else //��
	{
		if ((LocX == x + 1 && LocY == y + 2) || (LocX == x + 1 && LocY == y - 2)) //�� ��ĭ ��2,�� ��ĭ �� 2
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}

		}
		else if ((LocX == x + 2 && LocY == y + 1) || (LocX == x + 2 && LocY == y - 1))//�� 2 ��1, �� 2 ��1
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}

		else if ((LocY == y - 1 && LocX == x + 2) || (LocY == y + 1 && LocX == x + 2)) //�� 1 ��2 , ��1 ��2
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else if ((LocY == y - 1 && LocX == x - 2) || (LocY == y + 1 && LocX == x - 2)) //�� 1 ��2 , ��1 ��2
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else if ((LocY == y - 2 && LocX == x + 1) || (LocY == y + 2 && LocX == x + 1)) //�� 2 ��1 , ��2 ��1
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else if ((LocY == y - 2 && LocX == x - 1) || (LocY == y + 2 && LocX == x - 1)) //�� 2 ��1 , ��2 ��1
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else if ((LocX == x - 1 && LocY == y - 2) || (LocX == x - 1 && LocY == y + 2)) //��1 ��2 , ��1 ��2
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else if ((LocX == x - 2 && LocY == y - 1) || (LocX == x - 2 && LocY == y + 1)) //��2 ��1 , ��2 ��1
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
				printf("���� ����!\n");
			}
			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else
		{
			CColorSet(0, 1);
			printf("��KNIGHT�� �̵� ������ ������ϴ�!\n");

			printf("�ٽ� �Է����ּ���\n");
			return CFALSE;
		}
	}
	return CTRUE;
}

int Pawn()//�̺�--> ó�� �̵��� ��� �ִ� ��ĭ, �� �ܿ��� ��ĭ�� �̵����� ����� �־�� ��.
{

	if (turn % 2 == 0) //�渻
	{

		//�Ʒ� x == 1 --> ���� �ѹ��� �������� ���� ����(�渻�� ���� ChassBoard�� 1�࿡ ���� ��)�϶��� �ִ� ��ĭ�� �̵��� �� �ֵ��� ��
		if (x == 1 && ((y == LocY) && (LocX - x == 1 || LocX - x == 2))) //��:ó�� ������ �̵��Ҷ�,(y == LocY) && (((LocX - x) == 1 || (LocX - x) == 2)) == 1) -> (���� ����) && (�̵��ϰ� ���� �ڸ��� ��� ���� ��ġ�� ���� 1ĭ or 2ĭ ������ ��)
		{
			if (ChassBoard[LocX][LocY] == 0) //�̵��ϰ� ���� �ڸ��� ����ִٸ�(0�̶��)
			{
				swap(&ChassBoard[LocX][LocY], &ChassBoard[x][y]);


			}
			else //�̵��ϰ� ���� �ڸ��� �ٸ� ���� ���� �Ѵٸ�
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else if (y == LocY && LocX - x == 1) //��(ó�� �����̴°� �ƴҶ�): ������ ��ĭ �̵��� ����
		{
			if (ChassBoard[LocX][LocY] == 0)
			{

				swap(&ChassBoard[LocX][LocY], &ChassBoard[x][y]);

			}

			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}



		else if (LocX == x + 1 && LocY == y - 1)//��: ���� �밢������ ����
		{

			if (ChassBoard[LocX][LocY] == 0)
			{
				printf("������ ���� �����ϴ�!�ڸ��� �ٽ� �Է��� �ּ���\n");
				return CFALSE;
			}
			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0; //���� ���� ���� �������ϱ� �ڸ��� ��� -> 0
				swap(&ChassBoard[LocX][LocY], &ChassBoard[x][y]);// 
				CColorSet(0, 4);
				printf("���� ����!\n");

			}
			else
			{
				printf("�̵��� �� ���� ���! �ڸ��� �ٽ� ���� ���ּ���\n");
				return CFALSE;
			}

		}

		else if (LocX == x + 1 && LocY == y + 1)//��: ������ �밢������ ����
		{
			if (ChassBoard[LocX][LocY] == 0)
			{
				printf("������ ���� �����ϴ�!�ڸ��� �ٽ� �Է��� �ּ���\n");
				return CFALSE;
			}
			else if (ChassBoard[LocX][LocY] >= 11 && ChassBoard[LocX][LocY] <= 19)
			{
				ChassBoard[LocX][LocY] = 0; //���� ���� ���� �������ϱ� �ڸ��� ��� -> 0
				swap(&ChassBoard[LocX][LocY], &ChassBoard[x][y]);// 
				CColorSet(0, 4);
				printf("���� ����!\n");

			}
			else
			{
				printf("�̵��� �� ���� ���! �ڸ��� �ٽ� ���� ���ּ���\n");
				return CFALSE;
			}
		}

		else
		{
			CColorSet(0, 1);
			printf("��PAWN�� �̵� ������ ������ϴ�!\n");
			printf("�ٽ� �Է����ּ���\n");
			return CFALSE;
		}
	}

	else
	{

		if (x == 6 && (y == LocY) && (x - LocX == 1 || x - LocX == 2))  //���� ó�� ������ �̵��Ҷ�, (y == LocY) && ((x - LocX) == 1) -> (���� ����) && (�̵��ϰ� ���� �ڸ��� ��� ���� ��ġ�� ���� 1ĭ or2ĭ ������ ��)
		{
			if (ChassBoard[LocX][LocY] == 0)//�̵��ϰ� ���� �ڸ��� ����ִٸ�(0�̶��)
			{
				swap(&ChassBoard[LocX][LocY], &ChassBoard[x][y]);


			}
			else //�̵� �ϰ���� �ڸ��� �ٸ� ���� ���� ��
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}
		else if ((y == LocY) && (x - LocX == 1))//�鸻�� ó�� �̵��ϴ°��� �ƴ� ��, (y == LocY) && ((x - LocX) == 1) -> (���� ����) && (�̵��ϰ� ���� �ڸ��� ��� ���� ��ġ�� ���� 1ĭ ������ ��)
		{

			if (ChassBoard[LocX][LocY] == 0)
			{
				swap(&ChassBoard[LocX][LocY], &ChassBoard[x][y]);

			}

			else
			{
				printf("�̵��� �ڸ��� �̹� ���� ���� �մϴ�! �ڸ��� �ٽ� �����ϼ���\n");
				return CFALSE;
			}
		}

		else if (LocX == x - 1 && LocY == y - 1)//��: ���� �밢������ ����
		{

			if (ChassBoard[LocX][LocY] == 0)//�ڸ��� ��� �ִٸ�
			{
				printf("������ ���� �����ϴ�!�ڸ��� �ٽ� �Է��� �ּ���\n");
				return CFALSE;

			}
			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0; //���� ���� ���� �������ϱ� �ڸ��� ��� -> 0
				swap(&ChassBoard[LocX][LocY], &ChassBoard[x][y]);// 
				CColorSet(0, 4);
				printf("���� ����!\n");

			}
			else
			{
				printf("�̵��� �� ���� ���! �ڸ��� �ٽ� ���� ���ּ���\n");
				return CFALSE;
			}
		}

		else if (LocX == x - 1 && LocY == y + 1)//��: ������ �밢������ ����
		{
			if (ChassBoard[LocX][LocY] == 0) //�ڸ��� ��� �ִٸ�
			{
				printf("������ ���� �����ϴ�!�ڸ��� �ٽ� �Է��� �ּ���\n");
				return CFALSE;

			}
			else if (ChassBoard[LocX][LocY] >= 1 && ChassBoard[LocX][LocY] <= 9)
			{
				ChassBoard[LocX][LocY] = 0; //���� ���� ���� �������ϱ� �ڸ��� ��� -> 0
				swap(&ChassBoard[LocX][LocY], &ChassBoard[x][y]);// 
				CColorSet(0, 4);
				printf("���� ����!\n");

			}
			else
			{
				printf("�̵��� �� ���� ���! �ڸ��� �ٽ� ���� ���ּ���\n");
				return CFALSE;
			}
		}
		else
		{
			CColorSet(0, 1);
			printf("��PAWN�� �̵� ������ ������ϴ�!\n");
			printf("�ٽ� �Է����ּ���\n");
			return CFALSE;

		}

	}
	return CTRUE;
}

void swap(int* a, int* b)//�迭 ���� �ٲ��ֱ� ���� �Լ�
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

int checkGame() //king�� ���������� Ȯ���ϴ� �Լ�
{
	int i, j, CountK = 0;
	for (i = 0; i < CROW; i++)
		for (j = 0; j < CCOL; j++)
			if (ChassBoard[i][j] == 5 || ChassBoard[i][j] == 15) //5�� �渻�� king, 15�� �鸻�� king
				CountK++; //king�� ���ڸ� ������

	if (CountK == 2) //���� �θ��� king�� ��� ��� �ִٸ�
		return CTRUE;
	else //king�� �Ѹ��̶��
		return CFALSE;
}
