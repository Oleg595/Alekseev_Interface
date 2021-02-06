#ifndef OPENFILE_H_INCLUDED
#define OPENFILE_H_INCLUDED
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>

static OPENFILENAME ofn;

void PopFileInitialize(HWND hwnd);/*На вход принимается описатель окна. Функция для инициализации самого окна*/
BOOL PopFileOpenDlg(HWND hwnd, PSTR pstrFileName, PSTR pstrTitleName);/*функция, отвечающая за открытия диалогового окна. На вход принимается описатель окна
 и 2 макроса PSTR: указатели на буфер полного имени файла и на буфер только имени файла*/

#endif // OPENFILE_H_INCLUDED
