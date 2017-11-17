#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#pragma warning(disable:4996) //Tắt các cảnh báo của IDE (như _getch ,...)

//Struct lưu thông tin của 1 ô trong lưới sudoku.
typedef struct
{
	int stt;  //trạng thái của 1 ô (stt =1: giá trị có thể thay đổi được, stt = 0: giá trị không thể thay đổi được)
	int value;
}CELL;

//Các biến toàn cục sử dụng trong chương trình
CELL SUDOKU[9][9];
int DE_BAI[9][9] = { 0 };
int DAP_AN[9][9] = { 0 };
#define MAX 200

//Các hàm sử dụng trong chương trình
//Các hàm khởi tạo, giải Sudoku: 
void Init();
bool Find_Empty_Cell(int Grid[][9], int &row, int &col);
bool Used_In_Row(int Grid[][9], int row, int num);
bool Used_In_Box(int Grid[][9], int boxStartRow, int boxStartCol, int num);
bool Is_Correct_Cell(int Grid[][9], int row, int col, int num);
bool Solve_Sudoku(int Grid[][9]);
void Generate_Sudoku();
//
void Ghi_File(char* filename, int a[9][9]);
void Doc_File(char* filename);

//Các hàm tạo giao diện, đồ họa cho game:
void resizeConsole(int width, int height);
void gotoxy(int x, int y);
void textcolor(int x);
void clrscr();
void Giao_Dien(int &chon);
void In_Bang_Sudoku();
void In_Duong_Bien();
void In_Huong_Dan();

//Các hàm xử lí trong game:
void Start_Game();
bool Is_Win();
void Create_New_Sudoku();
void Create_Slant_Line_File();
void Create_Slant_Line_Sudoku();
void Show_Hint(int x, int y);
void Show_Wrong_Position();
void Complete_Sudoku();
void Save_Game();
bool Is_Win();
void Tro_Choi_Moi();
void Tiep_Tuc_Choi();
void Huong_Dan();


//Hàm khởi tạo lại các giá trị
void Init()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			DE_BAI[i][j] = 0;
			DAP_AN[i][j] = 0;
			SUDOKU[i][j].value = 0;
			SUDOKU[i][j].stt = 0;
		}
	}
}

bool Find_Empty_Cell(int Grid[][9], int &row, int &col)
{
	for (row = 0; row < 9; row++)
	{
		for (col = 0; col < 9; col++)
		{
			if (Grid[row][col] == 0)
				return true;
		}
	}
	return false;
}

bool Used_In_Row(int Grid[][9], int row, int num)
{
	for (int col = 0; col < 9; col++)
	{
		if (Grid[row][col] == num)
			return true;
	}
	return false;
}
bool Used_In_Column(int Grid[][9], int col, int num)
{
	for (int row = 0; row < 9; row++)
	{
		if (Grid[row][col] == num)
			return true;
	}
	return false;
}
bool Used_In_Box(int Grid[][9], int boxStartRow, int boxStartCol, int num)
{
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			if (Grid[row + boxStartRow][col + boxStartCol] == num)
				return true;
		}
	}
	return false;
}

bool Is_Correct_Cell(int Grid[][9], int row, int col, int num)
{

	return(!Used_In_Row(Grid, row, num) && !Used_In_Column(Grid, col, num) && !Used_In_Box(Grid, row - row % 3, col - col % 3, num));
}

bool Solve_Sudoku(int Grid[][9])
{
	int row, col;
	if (!Find_Empty_Cell(Grid, row, col))
		return true;
	for (int k = 9; k >= 0; k--)
	{
		if (Is_Correct_Cell(Grid, row, col, k))
		{
			Grid[row][col] = k;
			if (Solve_Sudoku(Grid))
				return true;
			else
				Grid[row][col] = 0;
		}
	}
	return false;
}
//Hàm tạo bảng Sudoku.
void Generate_Sudoku()
{
	srand(time(NULL));
	do
	{
		int m, n, k;
		DE_BAI[0][0] = 1 + rand() % 9;
		for (int i = 0; i < 10; i++)
		{
			m = rand() % 9;   // 0->8
			n = rand() % 9;	  // 0->8
			k = 1 + rand() % 9;
			while (DE_BAI[m][n] != 0)
			{
				m = rand() % 9;
				n = rand() % 9;
			}
			if (Is_Correct_Cell(DE_BAI, m, n, k))
				DE_BAI[m][n] = k;
			else
			{
				k = 9;
				DE_BAI[m][n] = k;
				while (!Is_Correct_Cell(DE_BAI, m, n, k))
				{
					k--;
					DE_BAI[m][n] = k;
					if (k == 0)
					{
						DE_BAI[m][n] = 0;
						break;
					}
				}
			}
		}
	} while (!Solve_Sudoku(DE_BAI));
	//Sao chép bảng Sudoku đề bài vào bảng đáp án
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			DAP_AN[i][j] = DE_BAI[i][j];
		}
	}
	
	//Xóa các số đi chỉ để lại 30 số gợi ý (đề bài)
	for (int j = 0; j < 51; j++)
	{
		int m, n;
		m = rand() % 9;   // 0->8
		n = rand() % 9;	  // 0->8
		if (DE_BAI[m][n] != 0)
		{
			DE_BAI[m][n] = 0;
		}
		else
		{
			while (DE_BAI[m][n] == 0)
			{
				m = rand() % 9;
				n = rand() % 9;
			}
			DE_BAI[m][n] = 0;
		}
	}
	return;
}

void Ghi_File(char* filename, int a[9][9])
{
	FILE* f = fopen(filename, "wb");
	if (f == NULL)
	{
		printf("Khong mo duoc file!\n");
		return;
	}
	else
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
				fwrite(a, sizeof(int), 81, f);
		}
	}
}
//Đọc file Sudoku.in và lưu vào mảng SUDOKU[9][9] (SUDOKU[i][j].value)
void Doc_File(char* filename)
{
	int temp_Sudoku[9][9];
	FILE* fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		printf("Khong mo duoc file!\n");
		exit(1); //Thoát chương trình.
	}
	else
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				fread(&SUDOKU[i][j].value, sizeof(int), 1, fp);
			}
		}
	}
	//Thay đổi giá trị biến trạng thái:
	//Giá trị 1: đây là ô đề bài, không thể thay đổi giá trị
	//Giá trị 0: đây là ô trống, có thể thay đổi giá trị
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (SUDOKU[i][j].value != 0)
				SUDOKU[i][j].stt = 1;
			else
				SUDOKU[i][j].stt = 0;
		}
	}
	/*for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			printf("%4d", SUDOKU[i][j].value);
		}
		printf("\n");
	}
	printf("\n");
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			printf("%4d", SUDOKU[i][j].stt);
		}
		printf("\n");
	}*/
	return;
}

//CÁC HÀM TẠO GIAO DIỆN, ĐỒ HỌA:
void resizeConsole(int width, int height)		// Hàm thay đổi cửa sổ cmd
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

void gotoxy(int x, int y)   //x xem như số dòng, y xem như số cột trong ma trận 2 chiều.
{
	HANDLE hConsoleOutput;
	COORD Cursor_an_Pos = { y, x };
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

void textcolor(int x)
{
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}

//Hàm xóa màn hình
void clrscr()
{
	DWORD n;      
	DWORD size;                     
	COORD coord = { 0 };              
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(h, &csbi);
	size = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(h, TEXT(' '), size, coord, &n);
	GetConsoleScreenBufferInfo(h, &csbi);
	FillConsoleOutputAttribute(h, csbi.wAttributes, size, coord, &n);
	SetConsoleCursorPosition(h, coord);
}

void Giao_Dien(int &chon)
{

	char s[MAX];
	resizeConsole(900, 650);
	FILE *f = fopen("GIAODIEN1.txt", "rt");
	if (f == NULL)
	{
		printf("Khong mo duoc file!");
		getch();
		exit(1);
	}
	textcolor(14);
	int i = 0;
	while (!feof(f))
	{
		fgets(s, MAX, f);
		gotoxy(i + 3, 25);
		printf("%s", s);
		Sleep(10);
		i++;
	}
	fclose(f);

	//Hiện đường viền quanh chữ SUDOKU
	textcolor(14);
	gotoxy(1, 20);
	for (int i = 0; i < 70; i++)
	{
		printf("_");
		Sleep(5);
	}

	gotoxy(2, 19);
	for (int i = 3; i < 15; i++)
	{
		printf("|\n");
		gotoxy(i, 19);
		Sleep(5);
	}
	gotoxy(13, 20);
	for (int i = 0; i < 70; i++)
	{
		printf("_");
		Sleep(5);
	}
	gotoxy(2, 90);
	for (int i = 3; i < 15; i++)
	{
		printf("|\n");
		Sleep(5);
		gotoxy(i, 90);
	}

	gotoxy(14, 20);
	textcolor(10);
	printf("Product by TEAM 1512535 - 1512563 - 1512571");
	textcolor(176);
	gotoxy(18, 40);
	printf(" 1. NEW GAME                        ");
	gotoxy(19, 40);
	printf(" 2. CONTINUE GAME                   ");
	gotoxy(20, 40); 
	printf(" 3. GUIDE                           ");
	gotoxy(21, 40);
	printf(" 4. COMPUTER PLAY IN SLANT LINE MODE");
	gotoxy(22, 40);
	printf(" 5. EXIT                            ");
	gotoxy(23, 40);
	printf("                                    ");
	gotoxy(24, 40);
	printf("  ENTER 1/2/3/4/5:                  ");
	scanf("%d", &chon);
	textcolor(15);
}

void In_Bang_Sudoku()
{
	//In bảng Sudoku:
	gotoxy(9, 10);
	textcolor(11);

	printf("%c", 218);
	for (int i = 0; i < 35; i++)
	{
		printf("%c", 196);
	}
	printf("%c", 191);
	printf("\n");
	//
	for (int i = 1; i < 19; i++)
	{
		gotoxy(9 + i, 10);
		printf("%c", 179);
	}
	gotoxy(27, 10);
	printf("%c", 192);
	//
	gotoxy(27, 11);
	for (int i = 0; i < 36; i++)
	{
		printf("%c", 196);
	}
	//
	for (int j = 7; j < 25; j++)
	{
		gotoxy(j + 3, 46);
		printf("%c", 179);
	}
	gotoxy(27, 46);
	printf("%c", 217);

	textcolor(11);
	int temp = 11;
	for (int i = 0; i < 8; i++)
	{
		if (temp != 15 && temp != 21)
		{
			for (int j = 11; j < 46; j++)
			{
				gotoxy(temp, j);
				printf("%c", 196);
			}
		}
		temp += 2;
	}
	temp = 14;
	for (int i = 0; i < 8; i++)
	{
		if (temp != 22 && temp != 34)
		{
			for (int j = 10; j < 27; j++)
			{
				gotoxy(j, temp);
				printf("%c", 179);
			}
		}
		temp += 4;
	}

	//Kẻ đường viền đậm:
	textcolor(9);
	for (int i = 10; i < 27; i++)
	{
		gotoxy(i, 22);
		if (i == 15 || i == 21)
			printf("%c", 206);
		else
			printf("%c", 186);

		gotoxy(i, 34);
		if (i == 15 || i == 21)
			printf("%c", 206);
		else
			printf("%c", 186);
	}

	for (int i = 11; i < 46; i++)
	{
		gotoxy(15, i);
		if (i == 22 || i == 34)
			printf("%c", 206);
		else
			printf("%c", 205);

		gotoxy(21, i);
		if (i == 22 || i == 34)
			printf("%c", 206);
		else
			printf("%c", 205);
	}
	//In các giá trị:
	int m, n;
	m = 10;
	for (int i = 0; i < 9; i++)
	{
		n = 12;
		for (int j = 0; j < 9; j++)
		{
			gotoxy(m, n);
			if (SUDOKU[i][j].value != 0 && SUDOKU[i][j].stt == 1)
			{
				textcolor(14);
				printf("%d", SUDOKU[i][j].value);
			}
			if (SUDOKU[i][j].value != 0 && SUDOKU[i][j].stt == 0)
			{
				textcolor(13);
				printf("%d", SUDOKU[i][j].value);
			}
			if (SUDOKU[i][j].value != 0 && SUDOKU[i][j].stt == 2)
			{
				textcolor(12);
				printf("%d", SUDOKU[i][j].value);
			}
			n += 4;
		}
		m += 2;
	}
}

void In_Duong_Bien()
{
	gotoxy(0, 60);
	for (int j = 0; j < 33; j++)
	{
		gotoxy(j, 60);
		printf("|\n");
	}
	//In chữ SUDOKU:
	FILE *f = fopen("GIAODIEN2.txt", "rt");
	if (f == NULL)
	{
		printf("Khong mo duoc file!");
		getch();
		exit(1);
	}
	char s[MAX];
	textcolor(14);
	int i = 0;
	while (!feof(f))
	{
		fgets(s, MAX, f);
		gotoxy(i, 3);
		printf("%s", s);
		i++;
	}
	fclose(f);
	return ;
}
void In_Huong_Dan()
{
	textcolor(14);
	gotoxy(1, 62);
	printf("* Press N to create new sudoku board");
	gotoxy(2, 62);
	printf("* Press W to show hint");
	gotoxy(3, 62);
	printf("* Press I to show wrong number");
	gotoxy(4, 62);
	printf("* Press C to Computer play");
	gotoxy(5, 62);
	printf("* Press F to complete Sudoku is solving");
	gotoxy(6, 62);
	printf("* Press S to save game");
	gotoxy(7, 62);
	printf("* Press E to exit game");
	gotoxy(8, 62);
	printf("* Press B to back to menu");
}
//Hàm xử lý chính các quá trình trong game
void Start_Game()
{
	int x = 10;
	int y = 12;
	gotoxy(x, y);
	// Bắt đầu và xử lý game
	while (true)
	{
		if (kbhit())
		{
			char c = getch();
			if (c == 'H' && x >= 12)  //Xuống
			{
				x = x - 2;
				gotoxy(x, y);
			}
			else if (c == 'P' && x <= 24) //Lên
			{
				x = x + 2;
				gotoxy(x, y);
			}
			else if (c == 'K' && y >= 16) //Qua trái
			{
				y = y - 4;
				gotoxy(x, y);
			}
			else if (c == 'M' && y <= 40) //Qua Phải
			{
				y = y + 4;
				gotoxy(x, y);
			}
			if (c >= '1' && c <= '9' && SUDOKU[(x - 10) / 2][(y - 12) / 4].stt == 0)  //Nhập số
			{
				SUDOKU[(x - 10) / 2][(y - 12) / 4].value = c - 48;
				textcolor(13);
				printf("%d", SUDOKU[(x - 10) / 2][(y - 12) / 4].value);
				gotoxy(x, y);
				textcolor(7);
			}
			else if (c == 'N' || c == 'n') //tạo lưới Sudoku mới
			{
				clrscr();
				Create_New_Sudoku();
			}
			else if (c == 'C' || c == 'c') //in lưới Sudoku hoàn chỉnh, nếu giá trị mà người dùng nhập khác với đáp án thì tô đỏ
			{
				Complete_Sudoku();
			}
			else if ((c == 'W' || c == 'w') && SUDOKU[(x - 10) / 2][(y - 12) / 4].stt == 0)   //gợi ý giá trị của 1 ô đang bí
			{
				Show_Hint(x, y);
			}
			else if (c == 'I' || c == 'i') //Gợi ý vị trí sai
			{
				Show_Wrong_Position();
			}
			else if (c == 'F' || c == 'f')
			{
				Complete_Sudoku();
			}
			else if (c == 'S' || c == 's') //Lưu lại game
			{
				Save_Game();
			}
			else if (c == 'E' || c == 'e') //Thoát game
			{
				exit(0);
			}
			else if (c == 'B' || c == 'b') //Trở về menu ban đầu
			{
				clrscr();

			MENU:;
			
				int chon;
				Giao_Dien(chon);
				while (chon != 1 && chon != 2 && chon != 3 && chon != 4 && chon != 5)
				{
					scanf("%d", &chon);
				}
				do
				{
					clrscr();
					switch (chon)
					{
					case 1:
					{
							  Tro_Choi_Moi();
							  break;
					}
					case 2:
					{
							  Tiep_Tuc_Choi();
							  break;
					}
					case 3:
					{
							  Huong_Dan();
							  char c;
							  while (true)
							  {
								  if (kbhit())
								  {
									  c = getch();
									  if (c == 'B' || c == 'b')
									  {
										  clrscr();
										  goto MENU;
									  }
									  if (c == 'E' || c == 'e')
										  exit(0);
								  }
							  }
					}
					case 4:
					{
							  Create_Slant_Line_Sudoku();
							  break;
					}
					case 5:
					{
							  exit(0);
					}
					}
				} while (chon != 5);
			}
		}
		if (Is_Win())
		{
			gotoxy(15, 70);
			textcolor(224);
			printf("                                 ");
			gotoxy(16, 70);
			printf("     ********* WIN *********     ");
			gotoxy(17, 70);
			printf("                                 ");
			textcolor(15);
		}
	}
}
//Hàm kiểm tra xem lưới Sudoku đã hoàn thành đúng chưa?
bool Is_Win()
{
	bool check = true;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (SUDOKU[i][j].stt == 0 && SUDOKU[i][j].value != DAP_AN[i][j])
			{
				check = false;
				return check;
			}
		}
	}
	return check;
}
//Hàm tạo lưới Sudoku mới
void Create_New_Sudoku()
{
	Init();
	Generate_Sudoku();
	Ghi_File("Sudoku.in", DE_BAI);
	Doc_File("Sudoku.in");
	Tro_Choi_Moi();
}
//Hàm tạo file sudoku.in chứa đường xuyên
void Create_Slant_Line_File()
{
	int a[9][9] = { 0 };
	int dem = 1;
	int i = rand() % 9;
	int j = rand() % 9;
	a[i][j] = -1;
	srand(time(NULL));
	while (dem < 9)
	{
		while (a[i][j] == -1 || (a[i + 1][j + 1] != -1 && a[i - 1][j + 1] != -1 && a[i - 1][j - 1] != -1 && a[i + 1][j - 1] != -1) || i == 0 || j == 0 || i == 8 || j == 8)
		{
			i = rand() % 9;
			j = rand() % 9;
		}
		a[i][j] = -1;
		dem++;
	}
	Ghi_File("Sudoku.in", a);
}
//Hàm tạo lưới Sudoku mới có đường xuyên
void Create_Slant_Line_Sudoku()
{
	Init();
	//Tạo tập tin Sudoku.in chứa đường xuyên
	Create_Slant_Line_File();

	FILE* f = fopen("Sudoku.in", "rb");
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			fread(&DE_BAI[i][j], sizeof(int), 1, f);
	}
	fclose(f);
	
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (DE_BAI[i][j] == -1)
				SUDOKU[i][j].stt = 2;  //Trạng thái của các ô đường xuyên là 2;
		}
	}
	srand(time(NULL));
	int temp = 1;  //1->9
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (DE_BAI[i][j] == -1)
			{
				DE_BAI[i][j] = temp;
				temp++;
			}
		}
	}
	if (!Solve_Sudoku(DE_BAI))
	{
		printf("ERROR!!!!!!!!!!!!");
		getch();
		exit(1);
	}
		
	// Sao chép giá trị của mảng đề bài vào mảng SUDOKU chính
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			SUDOKU[i][j].value = DE_BAI[i][j];
			if (SUDOKU[i][j].stt != 2)
				SUDOKU[i][j].stt = 1;
		}
	}
	clrscr();
	In_Bang_Sudoku();
	In_Duong_Bien();
	textcolor(14);
	gotoxy(8, 62);
	printf("* Press B to back to menu");
	gotoxy(9, 62);
	printf("* Press E to exit game   ");
	char c;
	while (true)
	{
		if (kbhit())
		{
			c = getch();
			if (c == 'B' || c == 'b')
			{
				clrscr();
			MENU:;

				int chon;
				Giao_Dien(chon);
				while (chon != 1 && chon != 2 && chon != 3 && chon != 4 && chon != 5)
				{
					scanf("%d", &chon);
				}
				do
				{
					clrscr();
					switch (chon)
					{
					case 1:
					{
							  Tro_Choi_Moi();
							  break;
					}
					case 2:
					{
							  Tiep_Tuc_Choi();
							  break;
					}
					case 3:
					{
							  Huong_Dan();
							  char c;
							  while (true)
							  {
								  if (kbhit())
								  {
									  c = getch();
									  if (c == 'B' || c == 'b')
									  {
										  clrscr();
										  goto MENU;
									  }
									  if (c == 'E' || c == 'e')
										  exit(0);
								  }
							  }
					}
					case 4:
					{
							  Create_Slant_Line_Sudoku();
							  break;
					}
					case 5:
					{
							  exit(0);
					}
					}
				} while (chon != 5);
			}
			if (c == 'E' || c == 'e')
			{
				exit(0);
			}
		}
	}

}
//Hàm gợi ý giá trị cần điền của 1 ô
void Show_Hint(int x, int y)
{
	gotoxy(x, y);
	SUDOKU[(x - 10) / 2][(y - 12) / 4].value = DAP_AN[(x - 10) / 2][(y - 12) / 4];
	textcolor(13);
	printf("%d", SUDOKU[(x - 10) / 2][(y - 12) / 4].value);
}
//Hàm hiển thị các vị trí sai
void Show_Wrong_Position()
{
	int m, n;
	int p, q;
	m = p = 10;
	q = 12;
	for (int i = 0; i < 9; i++)
	{
		n = 12;
		for (int j = 0; j < 9; j++)
		{
			gotoxy(m, n);
			if (SUDOKU[i][j].stt == 0 && SUDOKU[i][j].value != DAP_AN[i][j] && SUDOKU[i][j].value != 0)
			{
				textcolor(207);
				gotoxy(m, n);
				printf("%d", SUDOKU[i][j].value);
				textcolor(15);
				p = m;
				q = n;
			}
			n += 4;
		}
		m += 2;
	}
	gotoxy(p, q);
}
//Hàm để máy điền Sudoku
void Complete_Sudoku()
{
	int m, n;
	m = 10;
	for (int i = 0; i < 9; i++)
	{
		n = 12;
		for (int j = 0; j < 9; j++)
		{
			gotoxy(m, n);
			if (SUDOKU[i][j].stt == 0 && SUDOKU[i][j].value != DAP_AN[i][j])
			{
				SUDOKU[i][j].value = DAP_AN[i][j];
				textcolor(12);
				gotoxy(m, n);
				printf("%d", SUDOKU[i][j].value);
			}
			n += 4;
		}
		m += 2;
	}
}
//Hàm lưu game
void Save_Game()
{
	FILE* f = fopen("Sudoku.out", "wb");
	if (f == NULL)
	{
		printf("Khong mo duoc file!");
		getch();
		exit(1);
	}
	//Ghi giá trị của lưới
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			fwrite(&SUDOKU[i][j].value, sizeof(int), 1, f);
		}
	}
	//Ghi trạng thái của từng ô
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			fwrite(&SUDOKU[i][j].stt, sizeof(int), 1, f);
		}
	}
	textcolor(12);
	gotoxy(10, 65);
	printf("GAME SAVED!");
	gotoxy(11, 65);
	printf("Do you want to continue game?");
	gotoxy(12, 65);
	printf("YES : press Y");
	gotoxy(13, 65);
	printf("NO: press E to exit game");
	char chon;
	while (true)
	{
		if (kbhit())
		{
			chon = getch();
			if (chon == 'Y' || chon == 'y')
			{
				clrscr();
				Tro_Choi_Moi();
			}
			if (chon == 'E' || chon == 'e')
				exit(0);
		}
	}
}

void Tro_Choi_Moi()
{
	In_Bang_Sudoku();
	In_Duong_Bien();
	In_Huong_Dan();
	Start_Game();
	Sleep(5000);
}
void Tiep_Tuc_Choi()
{
	FILE* f = fopen("Sudoku.out", "rb");
	if (f == NULL)
	{
		clrscr();
		textcolor(12);
		gotoxy(10, 10);
		printf("There is no game was saved!!!");
		gotoxy(11, 10);
		printf("Do you want to play new game?");
		gotoxy(12, 10);
		printf("YES: press Y to play new game");
		gotoxy(13, 10);
		printf("NO: press E to exit game");
		char chon;
		while (true)
		{
			if (kbhit())
			{
				chon = getch();
				if (chon == 'Y' || chon == 'y')
				{
					clrscr();
					Tro_Choi_Moi();
				}
				if (chon == 'E' || chon == 'e')
					exit(0);
			}
		}
	}
	else
	{
		Init();
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				fread(&SUDOKU[i][j].value, sizeof(int), 1, f);
			}
		}
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				fread(&SUDOKU[i][j].stt, sizeof(int), 1, f);
			}
		}
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (SUDOKU[i][j].stt == 1)
				{
					DAP_AN[i][j] = SUDOKU[i][j].value;
				}
			}
		}
		if (!Solve_Sudoku(DAP_AN))
		{
			printf("ERROR!!!!!");
			getch();
			exit(1);
		}
		Tro_Choi_Moi();
	}
	fclose(f);
}
void Huong_Dan()
{

	textcolor(10);
	printf("Dien so tu 1 toi 9 sao cho:\n ");
	printf("\t* Moi cot doc,\n");
	printf("\t* Moi hang ngang,\n");
	printf("\t* Moi ma tran con 3x3 (vung duoc vien dam)\n");
	printf("co du cac so tu 1 toi 9 ma cac so nay khong duoc\n");
	printf("trung nhau trong cung hang, cung cot va cung ma tran con 3x3!\n\n");
	printf("\t* Nhan B de quay lai\n");
	printf("\t* Nhan E de thoat game");
}

int main()
{
	Generate_Sudoku();
	Ghi_File("Sudoku.in", DE_BAI);
	Doc_File("Sudoku.in");
	int chon;
BACK:;
	Giao_Dien(chon);
	while (chon != 1 && chon != 2 && chon != 3 && chon != 4 && chon != 5)
	{
		scanf("%d", &chon);
	}
	do
	{
		clrscr();
		switch (chon)
		{
		case 1:
		{
				  Tro_Choi_Moi();
				  break;
		}
		case 2:
		{
				  Tiep_Tuc_Choi();
				  break;
		}
		case 3:
		{
				  Huong_Dan();
				  char c;
				  while (true)
				  {
					  if (kbhit())
					  {
						  c = getch();
						  if (c == 'B' || c == 'b')
						  {
							  clrscr();
							  goto BACK;
						  }
						  if (c == 'E' || c == 'e')
							  exit(0);
					  }
				  }
		}
		case 4:
		{
				  Create_Slant_Line_Sudoku();
				  break;
		}
		case 5:
		{
				  exit(0);
		}
		}
	} while (chon != 5);
	return 0;
}
