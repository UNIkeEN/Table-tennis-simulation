#include "cmd_opts.h"
using namespace std;

static const HANDLE __hout = GetStdHandle(STD_OUTPUT_HANDLE);		//取标准输出设备对应的句柄
static const HANDLE __hin  = GetStdHandle(STD_INPUT_HANDLE);		//取标准输入设备对应的句柄

void copts_setconsoletitle(const char *title)
{
	SetConsoleTitleA(title);
}
void copts_cls(void){
    system("cls");
}
void copts_fixconsolesize(void){
    SetWindowLongPtrA(
        GetConsoleWindow(),
        GWL_STYLE,
        GetWindowLongPtrA(GetConsoleWindow(),GWL_STYLE) 
        & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX );
}
void copts_gotoxy(const int X, const int Y)
{
	COORD coord;
	coord.X = X;
	coord.Y = Y;
	SetConsoleCursorPosition(__hout, coord);
}
void copts_printxy(const int &X,const int &Y,string str)
{
    copts_gotoxy(X,Y);
    cout<<str;
}
void copts_printspace(const int &X,const int &Y,const int &rpt)
{
    copts_gotoxy(X,Y);
    for(int i=1;i<=rpt;i++) cout<<' ';
}
void copts_printline(const int &X,const int &Y,const int &rpt)
{
    copts_gotoxy(X,Y);
    for(int i=1;i<=rpt;i++) cout<<'-';
}
void copts_setconsoleborder(int set_cols, int set_lines, int set_buffer_cols, int set_buffer_lines)
{
	/* 取当前系统允许的窗口的行列最大值 */
	COORD max_coord;
	max_coord = GetLargestConsoleWindowSize(__hout); /* .X 和 .Y 分别是窗口的列和行的最大值 */
	/* 设置窗口的行列大小（从0开始，0 ~ lines-1, 0 ~ cols-1）*/
	SMALL_RECT rect;
	rect.Top = 0;
	rect.Bottom = set_lines - 1;
	rect.Left = 0;
	rect.Right = set_cols - 1;
	/* 设置缓冲区的行列大小(缺省或小于窗口值则与窗口值一样) */
	COORD cr;
	cr.X = (set_buffer_cols == -1 || set_buffer_cols < set_cols) ? set_cols : set_buffer_cols;		//未给出或给出的值小于set_cols则用set_cols，未控制上限
	cr.Y = (set_buffer_lines == -1 || set_buffer_lines < set_lines) ? set_lines : set_buffer_lines;	//未给出或给出的值小于set_lines则用set_lines，未控制上限
	/* 取当前窗口及缓冲区的大小 */
	int cur_cols, cur_lines, cur_buffer_cols, cur_buffer_lines;
	CONSOLE_SCREEN_BUFFER_INFO binfo;
	GetConsoleScreenBufferInfo(__hout, &binfo);

	cur_cols = binfo.srWindow.Right - binfo.srWindow.Left + 1;	//可见窗口的列数
	cur_lines = binfo.srWindow.Bottom - binfo.srWindow.Top + 1;	//可见窗口的行数
	cur_buffer_cols = binfo.dwSize.X;							//缓冲区的列数
	cur_buffer_lines = binfo.dwSize.Y;							//缓冲区的行数

	copts_cls();
	/* 设置窗口大小时，现缓冲区的列值要确保窗口值 */
	if (cr.X <= cur_buffer_cols) {
		if (cr.Y <= cur_buffer_lines) {
			SetConsoleWindowInfo(__hout, true, &rect);//设置窗口
			SetConsoleScreenBufferSize(__hout, cr);//设置缓冲区
		}
		else {
			COORD tmpcr;
			tmpcr.X = cur_buffer_cols;
			tmpcr.Y = cr.Y;
			SetConsoleScreenBufferSize(__hout, tmpcr);//设置缓冲区

			SetConsoleWindowInfo(__hout, true, &rect);//设置窗口
			SetConsoleScreenBufferSize(__hout, cr);//设置缓冲区
		}
	}
	else {
		if (cr.Y >= cur_buffer_lines) {
			SetConsoleScreenBufferSize(__hout, cr);//设置缓冲区
			SetConsoleWindowInfo(__hout, true, &rect);//设置窗口
		}
		else {
			COORD tmpcr;
			tmpcr.X = cr.X;
			tmpcr.Y = cur_buffer_lines;
			SetConsoleScreenBufferSize(__hout, tmpcr);//设置缓冲区

			SetConsoleWindowInfo(__hout, true, &rect);//设置窗口
			SetConsoleScreenBufferSize(__hout, cr);//设置缓冲区
		}
	}
	return;
}

void copts_hidecursor(){
	CONSOLE_CURSOR_INFO tmp;
	tmp.bVisible = 0;
	tmp.dwSize = 1;
	SetConsoleCursorInfo(__hout, &tmp);
}

void copts_showcursor(){
	CONSOLE_CURSOR_INFO tmp;
	tmp.bVisible = 1;
	tmp.dwSize = 25;
	SetConsoleCursorInfo(__hout, &tmp);
}