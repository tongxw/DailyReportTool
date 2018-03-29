// outputExcel.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "WriteExcel.h"

int _tmain(int argc, _TCHAR* argv[])
{
	FILE *f = fopen ("d://test.xls", "wb");
	miniexcel::CMiniExcel miniexcel;
	miniexcel(1,1) = "����";
	miniexcel(1,1).setBorder(BORDER_LEFT | BORDER_TOP | BORDER_BOTTOM);
	miniexcel(1,1).setAlignament(ALIGN_CENTER);
	miniexcel.SetColumnWidth(1,20);

	miniexcel(1,2) = "�Ա�";
	miniexcel(1,2).setBorder(BORDER_LEFT | BORDER_TOP | BORDER_BOTTOM);
	miniexcel(1,2).setAlignament(ALIGN_CENTER);
	miniexcel.SetColumnWidth(2,10);

	miniexcel(1,3) = "��������";
	miniexcel(1,3).setBorder(BORDER_LEFT | BORDER_TOP | BORDER_BOTTOM  | BORDER_RIGHT);
	miniexcel(1,3).setAlignament(ALIGN_CENTER);
	miniexcel.SetColumnWidth(3,30);

	miniexcel(2,1) = "����";
	miniexcel(2,1).setBorder(BORDER_LEFT | BORDER_BOTTOM);
	miniexcel(2,1).setAlignament(ALIGN_CENTER);

	miniexcel(2,2) = "��";
	miniexcel(2,2).setBorder(BORDER_LEFT | BORDER_BOTTOM);
	miniexcel(2,2).setAlignament(ALIGN_CENTER);

	miniexcel(2,3) = "1986-11-11";
	miniexcel(2,3).setBorder(BORDER_LEFT | BORDER_BOTTOM  | BORDER_RIGHT);
	miniexcel(2,3).setAlignament(ALIGN_CENTER);

	miniexcel.Write(f);
	return 0;
}

