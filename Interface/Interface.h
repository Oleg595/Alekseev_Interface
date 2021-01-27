#pragma once
#pragma warning (disable:4996)

#ifdef INTERFACE_H_EXPORTS
#define INTERFACE_H __declspec(dllexport)
#else
#define INTERFACE_H __declspec(dllimport)
#endif

#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<commdlg.h>

INTERFACE_H enum PAINT_MODE {
    ORDINARY,
    LAYOUT,
    NODATA
};

INTERFACE_H struct Str_data_t {//структура, которая хранит номер первого и последнего элемента строки
    int start_str;
    int end_str;
};

INTERFACE_H struct data_t {
    char* str_data;//массив всех считанных символов
    int cxChar, cyChar;//размеры символов
    int num_str;//количество считанных строк
    int num_layout_str;//количество строк в режиме вёрстки
    struct Str_data_t* start_str;// структура, хранящая индексы начала и конца каждой считанной строки
    int start_layout;//переменная, хранящая позицию для начала печати в режиме вёрстки относительно текущей строки
    int iVscrollPos, iLayoutVscrollPos;//позиции бегунка в обычном режиме и режиме вёрстки по вертикали
    int iHscrollPos;//позиция бегунка по горизонтали
    int cxClient, cyClient;//размеры окна
    int iVscrollMax, iVscrollInc;//максимальное значение для бегунка и значение смещения бегунка при соответствующих действиях по вертикали
    int iHscrollMax, iHscrollInc;//максимальное значение для бегунка и значение смещения бегунка при соответствующих действиях по горизонтали
    int iMaxWidth;//наибольшая ширина строки
    int Sc_pos;//переменная, определяющая, на сколько строк нужно опуститься при прокрутке на 1 строку
    enum PAINT_MODE p_m;//переменная, в которой определяется режим отрисовки: обычны или вёрстки
    OPENFILENAME* ofn;
};

INTERFACE_H void Read(char szFileName[], struct data_t* data);/*Функция, считывающая данные с файла и записывающая их структуру data_t,
которая хранит данные окна. На вход принимает имя файла и указатель на структуру.*/

INTERFACE_H void Clear(struct data_t* data);/*функция для очистки данных структуры data_t. Используется при закрытии файла и программы.
На вход принимает указатель на структуру data_t*/

INTERFACE_H int NewStartPos(int start_pos, int size);/*функция, возвращающая место начала видимой части файла, относительно первой видимой строки,
на вход принимается прошлая такая позиция и новый размер окна по оси x*/

INTERFACE_H int Layout_str(struct Str_data_t* start_str, int num_str, int size);/*функция, возвращая количество строк в режиме вёрстки. На вход принимает массив структур
Str_data_t, в котором содержится информация о начале и конце строки в обычном режиме, количество строк в обычном режиме и размер окна по оси x*/

INTERFACE_H int Scroll_Pos(struct Str_data_t* start_str, int scroll_pos, int size); /*функция, возвращающая положение бегунка для режима вёрстки. На вход принимает
массив структур Str_data_t, в которых содержится информация о конце и начале строки, положение бегунка относительно обычного режима и размер окна по оси x*/

INTERFACE_H void PopFileInitialize(HWND hwnd, OPENFILENAME* ofn);/*На вход принимается описатель окна. Функция для инициализации самого окна*/

INTERFACE_H BOOL PopFileOpenDlg(HWND hwnd, OPENFILENAME* ofn, PSTR pstrFileName, PSTR pstrTitleName);/*функция, отвечающая за открытия диалогового окна. На вход принимается описатель окна
 //и 2 макроса PSTR: указатели на буфер полного имени файла и на буфер только имени файла*/

 //#endif // INTERFACE_DLL_H_INCLUDED

