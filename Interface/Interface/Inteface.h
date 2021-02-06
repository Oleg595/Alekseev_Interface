#ifndef INTEFACE_H_INCLUDED
#define INTEFACE_H_INCLUDED
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<math.h>
#include<commdlg.h>

HINSTANCE hInst;

enum PAINT_MODE{
    ORDINARY,
    LAYOUT,
    NODATA
};

struct Str_data_t{//структура, которая хранит номер первого и последнего элемента строки
    int start_str;
    int end_str;
};

struct data_t{
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
};

int Layout_str(struct Str_data_t* start_str, int num_str, int size);/*функция, возвращая количество строк в режиме вёрстки. На вход принимает массив структур
Str_data_t, в котором содержится информация о начале и конце строки в обычном режиме, количество строк в обычном режиме и размер окна по оси x*/
int Scroll_Pos(struct Str_data_t* start_str, int scroll_pos, int size); /*функция, возвращающая положение бегунка для режима вёрстки. На вход принимает
массив структур Str_data_t, в которых содержится информация о конце и начале строки, положение бегунка относительно обычного режима и размер окна по оси x*/

#endif
