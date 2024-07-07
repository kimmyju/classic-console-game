#include <stdio.h>
#include <Windows.h>											//���� ����� ���� �������
#include <mmsystem.h>											// ���� ����� ���� �������
#pragma comment(lib,"winmm.lib") 

#define TRUE 1
#define FALSE 0
#define ROW 15
#define COL 15

typedef enum													//ONone-��ĭ, B-�浹,W-�鵹
{
	ONone = 0,
	B = 1,
	W = 2
}Bstate;

int OmokB[ROW][COL], Oturn = 0;									// OmokB �迭-���� ��, Oturn-�鵹�� �浹�� ���ʸ� �����ϱ� ���� ����(�浹-¦�� Oturn�϶�,�鵹-Ȧ��Oturn�϶�)
void OPrintGameStart();											//GameStart������ ����� �ִ� �Լ�.
void InitBoard();												//�������� �ʱ�ȭ ���ִ� �Լ�
void PrintBoard();												//�������� ��Ȳ�� ����Ʈ�� �ִ� �Լ�
inline int MoveStone(int* LocX, int* LocY);						//���� ���� �ִ� �Լ�
inline void ColorSet(unsigned short back, unsigned int text);   //���� ������ ���� �Լ�
void Ogotoxy(int x, int y);										//Ŀ�� ��ġ�� �̵���Ű�� �Լ�
void Input(int* x, int* y);										//�Է��� �޴� �Լ�
int CheckGame();												//������ ������ �˻��� �ִ� �Լ�. (�� 5���� ���̸� ���� ����)

int Omok()
{
	char player1[5], player2[5], YN;
	int GameS, x, y, CheckTF;
	SetConsoleTitle(TEXT("���� ����"));

replay:
	printf("\n<���� ��>\n");									//���� �� ����
	printf("\n\n1. �÷��̾�� ���� ��� ���� �����Ͽ� �Է��Ѵ�.");
	printf("\n\n2. ��� ���� ���ʴ�� �ѹ��� �ڸ��� �����Ͽ� ���� ���´�.");
	printf("\n\n3. ���� ���� Ȥ�� �밢������ ���� 5���� ���� ��ġ ��Ű�� �¸��ϴ� ����");

	printf("\n\n������ PLAYER�� ��� ���� �������ּ���(�Է�: b/w)\n");//Player1�� PLAYER2 �� ����
	printf("PLAYER1: ");
	scanf_s("%s", player1, sizeof(player1));					//player�� ���ϴ� ���� ���� �Է� ����
	printf("PLAYER2: ");
	scanf_s("%s", player2, sizeof(player2));

	if (strcmp(player1, "b\0") == 0)							//player1�� ���� b�� �Է� �ߴٸ� player1�� ����
	{
		
		printf("��: PLAYER1, ��: PLAYER2\n");
		printf("player1�� �����մϴ�!\n");

	}
	else														//player2�� ���� b�� �Է� �ߴٸ� player1�� ����
	{
		printf("��: PLAYER2, ��: PLAYER1\n");
		printf("player2�� �����մϴ�!\n");

	}
	ColorSet(0, 4);												//�ؽ�Ʈ ���� ����������
	printf("\n\nStart Game>  Enter");
	GameS = getch();											//Ű������ EnterŰ�� �Է� �ޱ� ���� getch()���   
	if (GameS == 13)											//������� �Է��� Enter(�ƽ�Ű�ڵ�- 13)�̶�� ���������� ���� ����
	{
		system("cls");
		PlaySound(TEXT("C:\\Users\\yuido\\Downloads\\MiniGameConsole-master\\MiniGameConsole-master\\mainGame\\OmokBgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

		OPrintGameStart();										//GameStart!���� ���
		Sleep(1000);											//1�ʰ� ȭ���� ����
		InitBoard();											//������ �ʱ�ȭ
		do
		{
			system("cls");


			PrintBoard();										//�������� print

			printf("\n�̵��� �ڸ��� �Է��ϼ���(ex �� 2 3): ");
			Input(&x, &y);										//������ �� �ڸ��� Input()�Լ��� ���� �Է� ����

			CheckTF = MoveStone(x, y);							//������ �Է� ���� x,y�� MoveStone()�Լ��� ���� ���� �� �� �ִ� �ڸ����� Ȯ��(��ȯ��-TRUE,FALSE)

			if (CheckTF == FALSE)								//77-78�� -->���� ����ڰ� ������ �ڸ��� �ٸ� ���� �̹� �����Ѵٸ� MoveStone()�Լ����� FALSE�� ��ȯ �ް� �ݺ����� ó������ �̵�
				continue;										//TRUE���� ��ȯ �޾Ҵٴ°��� MoveStone�Լ� �ȿ��� x,y �ڸ��� ���� ����������  �����־��ٴ� �ǹ�.  

			if (CheckGame() == FALSE)							//80-81�� checkGame()�� ���� ���� ����,����,Ȥ�� �밢������ 5���� �𿴴����� Ȯ��(FALSE-���� �� ���� 5���� �� �𿴴ٴ� �ǹ�,TRUE-5���� �� ������ �ʾҴٴ� �ǹ�)
				break;

			Oturn++;											//CheckGame()�Լ����� ���� �� ���� 5�� ������ �ʾ��� �� OOturn++�� ���� ���� ������� ���ʷ� �Ѱ���
		} while (1);


	}


	system("cls");
	PrintBoard();												//���� ������ ���
	if (Oturn % 2 == 0)											//do while������ OOturn�� ¦���� ���·� ���Դٸ� �浹�� �¸�
	{
		ColorSet(0, 14);
		printf("\n�� ");
		ColorSet(0, 4);
		printf("WIN: ��    ");
		ColorSet(0, 1);
		printf("LOSE: ��\n");

	}
	else														//do while������ OOturn�� Ȧ���� ���·� ���Դٸ� �鵹�� �¸�
	{
		ColorSet(0, 14);
		printf("\n�� ");
		ColorSet(0, 4);
		printf("WIN: ��    ");
		ColorSet(0, 1);
		printf("LOSE: ��\n");

	}
	ColorSet(0, 4);
	printf("\nPLAY AGAIN?(y/n): ");
	ColorSet(0, 15);
	YN = getche();
	if (YN == 'y')											//���� ����ڰ� y�� �Է��ߴٸ� main�� �� ó�� �κ����� �̵�
	{
		Oturn = 0;
		system("cls");
		goto replay;
	}
	else													//����ڰ� n�� �Է��ߴٸ� ���� ����
	{
		printf("\n\n<������ �����մϴ�>");
		Sleep(1500);
		return 0;
	}
}

void InitBoard()											//ü���� �ʱ�ȭ�� ���� �Լ�
{
	int i, j;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			OmokB[i][j] = ONone;							//�������� ��� ��ĭ���� �ʱ�ȭ(enum�� ONone == 0)
		}
	}
}

void PrintBoard()
{
	int i, j;
	ColorSet(0, 15);
	if (Oturn % 2 == 0)										//145-148�� -> Oturn�� ¦������ Ȧ�������� �Ǻ��Ͽ� ������ ���������� �������
		printf("\n  turn: ��");
	else
		printf("\n  turn: ��");
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
			// if(��:¦ ��:¦ || ��:Ȧ ��:Ȧ) else(��:Ȧ �� : ¦ || �� : ¦ �� : Ȧ)�� ������ ������ �������� ���� �����ư��鼭 ����ֱ� ����
			if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1))//��:¦ ��:¦ || ��:Ȧ ��:Ȧ
			{
				if (OmokB[i][j] == B)						//�浹�� ���� ��
				{

					ColorSet(6, 0);
					printf(" �� ");
				}
				else if (OmokB[i][j] == W)					//�鵹�� ���� ��
				{
					ColorSet(6, 15);
					printf(" �� ");
				}

				else										//ONone�� ��(��ĭ�� ��)
				{
					ColorSet(6, 6);
					printf("    ");
				}
			}
			else//��:Ȧ �� : ¦ || �� : ¦ �� : Ȧ
			{
				if (OmokB[i][j] == B)
				{
					ColorSet(14, 0);
					printf(" �� ");
				}
				else if (OmokB[i][j] == W)
				{
					ColorSet(14, 15);
					printf(" �� ");
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

void Ogotoxy(int x, int y)									//Ŀ�� ��ġ�� �̵� ���Ѵ� �Լ�
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Input(int* x, int* y)									//�Է��� �޾��ִ� �Լ�(�ڸ� �Է��� main������ �ޱ� ������ main��x,y�� �ּڰ��� �μ��� �ѱ�
{
	scanf_s("%d %d", x, y);
}

inline int MoveStone(int LocX, int LocY)					//���� �����ִ� �Լ�
{
	if (Oturn % 2 == 0)										//�浹�� ������ ���
	{
		if (OmokB[LocX][LocY] == W || OmokB[LocX][LocY] == B) //����ڰ� �Է��� �ڸ��� �ٸ� ���� �����Ѵٸ� return FALSE
			return FALSE;
		else if (OmokB[LocX][LocY] == ONone)				 //�� �ڸ���� Omok[][]�迭�� �浹�� enum����� B�� �־���(B=1)
			OmokB[LocX][LocY] = B;
		else												//�������� ����� ���
		{
			printf("\n�߸��� �Է��Դϴ�. �ڸ��� �ٽ� �Է��ϼ���");
			return FALSE;
		}

	}
	else													//�鵹�� ������ ���(���� ����)
	{
		if (OmokB[LocX][LocY] == W || OmokB[LocX][LocY] == B)
			return FALSE;
		else if (OmokB[LocX][LocY] == ONone)				//�� �ڸ���� Omok[][]�迭�� �鵹�� enum����� W�� �־���(W=2) 
			OmokB[LocX][LocY] = W;
		else
		{
			printf("\n�߸��� �Է��Դϴ�. �ڸ��� �ٽ� �Է��ϼ���");
			return FALSE;
		}
	}
	return TRUE;
}

int CheckGame()												//���� 5���� �� �𿴴��� üũ ���ִ� �Լ�
{
	int i, j, k, t;                                         // 1. ������ �ᱹ 5���� �Ϸķ� ������ �����ϴ� �κа� ������ �κ��� �����. 
	for (i = 0; i < ROW; i++)                               // 2. ���� �迭�� ��� ĭ�� �˻����ش�.               
	{                                                       // 3. �̶�, ���� ��� ĭ�� ���� ���� �Ѵٸ� ���� �����ϴ� ĭ�� ����ĭ���� ����5���� ���� �� �ִ� �������θ� ���� �� 5���� �����ϴ��� �˻縦 ���ش�  
		for (j = 0; j < COL; j++)
		{
			if (i <= 3 && j <= 3)                           //�ε����� i <=3, j <=3�� ���, ������ ����,�Ʒ� ����, ������ �Ʒ� �밢�����θ� ���� 5���� ���� �� �ִ�.(������ ������ ���� 5���� ���� �� ���� ������ �˻� �ϴ°��� ���ǹ���)               
			{
				if (OmokB[i][j] == W || OmokB[i][j] == B)   //���� ������ ��               
				{
					for (k = j + 1; k <= j + 4; k++)        //������ ���� �˻�         
					{
						if (OmokB[i][j] != OmokB[i][k])     //���� Omok[i][j]�� �ִ� ���� ������ ������ 4���� ����  ���� �������� �����Ѵٸ� break�� ���� ���� �ε����� �̵�
							break;
					}
					if (k == j + 4 + 1)						//���� ���� for�� �ȿ� if���� �ɸ��� �ʴ´ٸ� ���� 5�� �� �ִٴ� �ǹ��̹Ƿ� return FALSE�� ��ȯ�� ���� ����
						return FALSE;
					for (k = i + 1; k <= i + 4; k++)		//�Ʒ� �������� �˻�
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
					for (k = i + 1, t = j + 1; k <= i + 4; k++, t++)//������ �Ʒ� �밢������ �˻�
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;

				}

			}
			else if (i <= 3 && j >= 4 && j <= 10)			//�� �� �� �� �׹������θ� ���� 5���� ���� �� ����(������ ������ ���� 5���� ���� �� �����Ƿ� �˻簡 ���ǹ�)
			{
				if (OmokB[i][j] == W || OmokB[i][j] == B)
				{
					for (k = j + 1; k <= j + 4; k++)		//������ ���� �˻�
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j + 4 + 1)
						return FALSE;
					for (k = j - 1; k >= j - 4; k--)		//���� ���� �˻�
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j - 4 - 1)
						return FALSE;
					for (k = i + 1; k <= i + 4; k++)		//�Ʒ� �������� �˻�
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
					for (k = i + 1, t = j + 1; k <= i + 4; k++, t++)//������ �Ʒ� �밢������ �˻�
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
					for (k = i + 1, t = j - 1; k <= i + 4; k++, t--)//���� �Ʒ� �밢������ �˻�
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
				}
			}
			else if (i <= 3 && j >= 11 && j <= COL - 1)			//�� �� �� �� �׹������θ� ���� 5���� ���� �� ����(������ ������ ���� 5���� ���� �� �����Ƿ� �˻簡 ���ǹ�)
			{
				if (OmokB[i][j] == W || OmokB[i][j] == B)
				{

					for (k = j - 1; k >= j - 4; k--)		//���� ���� �˻�
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j - 4 - 1)
						return FALSE;
					for (k = i + 1; k <= i + 4; k++)		//�Ʒ� �������� �˻�
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;

					for (k = i + 1, t = j - 1; k <= i + 4; k++, t--)//���� �Ʒ� �밢������ �˻�
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
				}
			}
			else if (i >= 4 && i <= 10 && j <= 3)			//��  ��� �� �ֹ������θ� ���� 5���� ���� �� ����(������ ������ ���� 5���� ���� �� �����Ƿ� �˻簡 ���ǹ�)
			{
				if (OmokB[i][j] == W || OmokB[i][j] == B)
				{
					for (k = j + 1; k <= j + 4; k++)		//������ ���� �˻�
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j + 4 + 1)
						return FALSE;

					for (k = i - 1; k >= i - 4; k--)		//��
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;

					for (k = i + 1; k <= i + 4; k++)		//�Ʒ� �������� �˻�
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
					for (k = i - 1, t = j + 1; k >= i - 4; k--, t++)//�� ������ �밢�� �˻�
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}

					if (k == i - 4 - 1)
						return FALSE;
					for (k = i + 1, t = j + 1; k <= i + 4; k++, t++)//������ �Ʒ� �밢������ �˻�
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;

				}
			}
			else if (i >= 4 && i <= 10 && j >= 4 && j <= 10)			//�� �� ��� �� �� �� �� ��� �������� ���� 5���� ���� �� ����(��� �������� �˻� ���־�� ��)
			{
				if (OmokB[i][j] == W || OmokB[i][j] == B)
				{
					for (k = j + 1; k <= j + 4; k++)					//������ ���� �˻�
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j + 4 + 1)
						return FALSE;

					for (k = j - 1; k >= j - 4; k--)					//���� ���� �˻�
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j - 4 - 1)
						return FALSE;

					for (k = i - 1; k >= i - 4; k--)					//��
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;

					for (k = i + 1; k <= i + 4; k++)				//�Ʒ� �������� �˻�
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
					for (k = i - 1, t = j + 1; k >= i - 4; k--, t++)//�� ������ �밢�� �˻�
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}

					if (k == i - 4 - 1)
						return FALSE;
					for (k = i - 1, t = j - 1; k >= i - 4; k--, t--)//�� ���� �밢�� �˻�
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;

					for (k = i + 1, t = j + 1; k <= i + 4; k++, t++)//������ �Ʒ� �밢������ �˻�
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
					for (k = i + 1, t = j - 1; k <= i + 4; k++, t--)//���� �Ʒ� �밢������ �˻�
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;

				}

			}
			else if (i >= 4 && i <= 10 && j >= 11 && j <= COL - 1)		//���� �آ׹������θ� ���� 5���� ���� �� ����(������ ������ ���� 5���� ���� �� �����Ƿ� �˻簡 ���ǹ�)
			{

				if (OmokB[i][j] == W || OmokB[i][j] == B)
				{
					for (k = i - 1; k >= i - 4; k--)					//��
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;
					for (k = i + 1; k <= i + 4; k++)					//�Ʒ� �������� �˻�
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;

					for (k = j - 1; k >= j - 4; k--)					//���� ���� �˻�
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j - 4 - 1)
						return FALSE;


					for (k = i - 1, t = j - 1; k >= i - 4; k--, t--)	//�� ���� �밢�� �˻�
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;

					for (k = i + 1, t = j - 1; k <= i + 4; k++, t--)	//���� �Ʒ� �밢������ �˻�
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i + 4 + 1)
						return FALSE;
				}
			}
			else if (i >= 11 && j <= 3)									//�� �� �� �������θ� ���� 5���� ���� �� ����(������ ������ ���� 5���� ���� �� �����Ƿ� �˻簡 ���ǹ�)
			{
				if (OmokB[i][j] == W || OmokB[i][j] == B)
				{
					for (k = i - 1; k >= i - 4; k--)					//��
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;

					for (k = i - 1, t = j + 1; k >= i - 4; k--, t++)	//�� ������ �밢�� �˻�
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}

					if (k == i - 4 - 1)
						return FALSE;

					for (k = j + 1; k <= j + 4; k++)					//������ ���� �˻�
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j + 4 + 1)
						return FALSE;
				}
			}
			else if (i >= 11 && j >= 4 && j <= 10)						//�� �� �� �֢ع������θ� ���� 5���� ���� �� ����(������ ������ ���� 5���� ���� �� �����Ƿ� �˻簡 ���ǹ�)
			{
				if (OmokB[i][j] == W || OmokB[i][j] == B)
				{
					for (k = j - 1; k >= j - 4; k--)					//���� ���� �˻�
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j - 4 - 1)
						return FALSE;

					for (k = i - 1, t = j - 1; k >= i - 4; k--, t--)	//�� ���� �밢�� �˻�
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;
					for (k = i - 1; k >= i - 4; k--)					//��
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;

					for (k = i - 1, t = j + 1; k >= i - 4; k--, t++)	//�� ������ �밢�� �˻�
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}

					if (k == i - 4 - 1)
						return FALSE;

					for (k = j + 1; k <= j + 4; k++)					//������ ���� �˻�
					{
						if (OmokB[i][j] != OmokB[i][k])
							break;
					}
					if (k == j + 4 + 1)
						return FALSE;
				}
			}
			else if (i >= 11 && j >= 11 && j <= COL - 1)				 // �� �� �ع������θ� ���� 5���� ���� �� ����(������ ������ ���� 5���� ���� �� �����Ƿ� �˻簡 ���ǹ�)
			{
				if (OmokB[i][j] == W || OmokB[i][j] == B)
				{
					for (k = i - 1; k >= i - 4; k--)					//��
					{
						if (OmokB[i][j] != OmokB[k][j])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;


					for (k = i - 1, t = j - 1; k >= i - 4; k--, t--)	//�� ���� �밢�� �˻�
					{
						if (OmokB[i][j] != OmokB[k][t])
							break;
					}
					if (k == i - 4 - 1)
						return FALSE;

					for (k = j - 1; k >= j - 4; k--)					//���� ���� �˻�
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
	return TRUE;														//���� 5���� ���� ���� ���ٸ�(���� if���� �ɸ��� �ʾ� �̰��� �����Ѵٸ� return  TRUE�� ���� ���� ��� ����
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