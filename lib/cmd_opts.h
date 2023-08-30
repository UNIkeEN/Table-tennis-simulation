/*关于控制台的相关操作*/
#pragma once
#include <string>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;
/*******************************************************
  函数名称：copts_setconsoletitle
  功    能：设置cmd窗口标题
*******************************************************/
void copts_setconsoletitle(const char *title);

/*******************************************************
  函数名称：copts_fixedconsolesize
  功    能：隐藏标题栏最大化按钮，禁止边框拖动改变窗口大小
*******************************************************/
void copts_fixconsolesize(void);

/*******************************************************
  函数名称：copts_cls
  功    能：清屏
*******************************************************/
void copts_cls(void);

/*******************************************************
  函数名称：copts_gotoxy
  功    能：将光标移动到指定位置
  输入参数：const int X       ：X轴坐标（列）
			const int Y       ：Y轴坐标（行）
  说    明：屏幕左上角坐标为(0,0)，在cmd窗口的大小未被调整的情况下，Win10为：
			横向x轴，对应列(0-119)
			纵向y轴，对应行(0-29)
*******************************************************/
void copts_gotoxy(const int X, const int Y);

/*******************************************************
  函数名称：copts_printxy
  功    能：在指定位置输出指定颜色字符串
  输入参数：const int &X，const int &Y 行列
         const string &str 输出字符串
*******************************************************/
void copts_printxy(const int &X,const int &Y,std::string str);

/*******************************************************
  函数名称：copts_printspace
  功    能：在指定位置输出空格
  输入参数：const int &X，const int &Y 行列
         const int &rpt 输出次数（长度）
*******************************************************/
void copts_printspace(const int &X,const int &Y,const int &rpt);

/*******************************************************
  函数名称：copts_printline
  功    能：在指定位置输出一条线
  输入参数：const int &X，const int &Y 行列
         const int &rpt 输出次数（长度）
*******************************************************/
void copts_printline(const int &X,const int &Y,const int &rpt);

/*******************************************************
  函数名称：copts_setconsoleborder
  功    能：改变cmd窗口的大小及缓冲区的大小
  输入参数：const int cols     ：新的列数
		   const int lines        ：新的行数
		   const int buffer_cols  ：新的缓冲区列数
		   const int buffer_lines ：新的缓冲区行数
  说    明：必须先设置缓冲区，再设置窗口大小，
*******************************************************/
void copts_setconsoleborder(int set_cols, int set_lines, int set_buffer_cols, int set_buffer_lines);

/*******************************************************
  函数名称：copts_hidecursor
  功    能：隐藏光标
*******************************************************/
void copts_hidecursor();

/*******************************************************
  函数名称：copts_hidecursor
  功    能：显示光标（横线）
*******************************************************/
void copts_showcursor();
