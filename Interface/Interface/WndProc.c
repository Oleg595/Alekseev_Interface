#include"Inteface.h"
#include"Menu.h"
#include"OpenFile.h"
#define EDITID 1

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam){
    HDC hdc;
    static HMENU hMenu;
    static char szFileName[_MAX_PATH];
    static char szTitleName[_MAX_FNAME + _MAX_EXT];
    static HINSTANCE hInst;
    static HWND hwndEdit;
    static UINT iMsgFindReplace;
    RECT* lprect;
    int i, j, q, num;
    int size = 0;
    char szAppName[] = "PopMenu";//название меню
    static struct data_t data;
    int iPaintBeg, iPaintEnd, y, x, iPaintLeft, iPaintRight;
    CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
    PAINTSTRUCT ps;
    TEXTMETRIC tm;
    FILE* file;
    RECT rect;
    switch(iMsg)
    {
        case WM_CREATE:
            hInst =((LPCREATESTRUCT) lParam) -> hInstance;
            //создаём дочернее окно
                hwndEdit = CreateWindow("edit", NULL,
                WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
                WS_BORDER | ES_LEFT | ES_MULTILINE |
                ES_NOHIDESEL | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
                0, 0, 0, 0,
                hwnd,(HMENU) EDITID, hInst, NULL);
            SendMessage(hwndEdit, EM_LIMITTEXT, 32000, 0L);
            PopFileInitialize(hwnd);
            iMsgFindReplace = RegisterWindowMessage(FINDMSGSTRING);

            data.p_m = ORDINARY;
            hMenu = LoadMenu(hInst, szAppName);
            hMenu = GetSubMenu(hMenu, 0);
            hdc = GetDC(hwnd);
            SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
            GetTextMetrics(hdc, &tm);
            data.cxChar = tm.tmAveCharWidth;
            data.cyChar = tm.tmHeight + tm.tmExternalLeading;
            data.cxClient = LOWORD(lParam);//ширина окна
            data.cyClient = HIWORD(lParam);//высота окна
            ReleaseDC(hwnd, hdc);
            Read((char*)cs->lpCreateParams, &data);
            return 0;
        case WM_SIZE :
            j = data.cxClient / data.cxChar;
            data.cxClient = LOWORD(lParam);//ширина окна
            data.cyClient = HIWORD(lParam);//высота окна
            if(data.p_m == ORDINARY){
                data.Sc_pos = (data.num_str > pow(2, 16)) ? data.num_str / pow(2, 16) + 1 : 1;
                data.iVscrollMax = min(pow(2, 16), (max(0, data.num_str + 2 - data.cyClient / data.cyChar) / data.Sc_pos));
                data.iVscrollPos = (int)(min(data.iVscrollPos, data.iVscrollMax));
                SetScrollRange(hwnd, SB_VERT, 0, data.iVscrollMax, FALSE);
                SetScrollPos(hwnd, SB_VERT, data.iVscrollPos, TRUE);
                data.iHscrollMax = max(0, data.iMaxWidth - data.cxClient / data.cxChar);
                data.iHscrollPos = min(data.iHscrollPos, data.iHscrollMax);
                SetScrollRange(hwnd, SB_HORZ, 0, data.iHscrollMax, FALSE);
                SetScrollPos(hwnd, SB_HORZ, data.iHscrollPos, TRUE);
            }
            if(data.p_m == LAYOUT){
                data.iHscrollMax = 0;
                SetScrollRange(hwnd, SB_HORZ, 0, data.iHscrollMax, FALSE);
                data.num_layout_str = Layout_str(data.start_str, data.num_str, data.cxClient / data.cxChar);
                data.Sc_pos = (data.num_layout_str > pow(2, 16)) ? data.num_layout_str / pow(2, 16) + 1 : 1;
                data.iVscrollMax = min(pow(2, 16), (max(0, data.num_layout_str + 2 - data.cyClient / data.cyChar) / data.Sc_pos));
                data.iLayoutVscrollPos = Scroll_Pos(data.start_str, data.iVscrollPos, data.cxClient / data.cxChar);
                SetScrollRange(hwnd, SB_VERT, 0, data.iVscrollMax, TRUE);
                ScrollWindow(hwnd, 0, -data.cyChar * data.iVscrollInc * data.Sc_pos, NULL, NULL);
                q = data.start_layout / j;
                if(data.start_layout != 0){
                    if(j < data.cxClient / data.cxChar){
                        data.start_layout += q;
                        if(data.start_layout >= data.start_str[data.iVscrollPos].end_str - data.start_str[data.iVscrollPos].start_str){
                            data.start_layout = 0;
                            data.iVscrollPos++;
                            data.iLayoutVscrollPos++;
                        }
                    }
                    if(j > data.cxClient / data.cxChar){
                        data.start_layout -= q;
                    }
                }
                data.iLayoutVscrollPos = min(data.iLayoutVscrollPos, data.iVscrollMax);
                if(data.iVscrollMax == 0){
                    data.iVscrollPos = 0;
                    data.start_layout = 0;
                }
                SetScrollPos(hwnd, SB_VERT, data.iLayoutVscrollPos, TRUE);
                UpdateWindow(hwnd);
            }
            return 0;
        case WM_VSCROLL :
            switch(LOWORD(wParam))
            {
                case SB_TOP :
                    if(data.p_m == ORDINARY){
                        data.iVscrollInc = -data.iVscrollPos;
                    }
                    if(data.p_m == LAYOUT){
                        data.iVscrollInc = -data.iLayoutVscrollPos;
                    }
                    break;
                case SB_BOTTOM :
                    if(data.p_m == ORDINARY){
                        data.iVscrollInc = data.iVscrollMax - data.iVscrollPos;
                    }
                    if(data.p_m == LAYOUT){
                        data.iVscrollInc = data.iVscrollMax - data.iLayoutVscrollPos;
                    }
                    break;
                case SB_LINEUP :
                    data.iVscrollInc = -1;
                    break;
                case SB_LINEDOWN :
                    data.iVscrollInc = 1;
                    break;
                case SB_PAGEUP :
                    data.iVscrollInc = min(-1, -data.cyClient / data.cyChar);
                    break;
                case SB_PAGEDOWN :
                    data.iVscrollInc = max(1, data.cyClient / data.cyChar);
                    break;
                case SB_THUMBTRACK :
                    if(data.p_m == ORDINARY){
                        data.iVscrollInc = HIWORD(wParam) - data.iVscrollPos;
                    }
                    if(data.p_m == LAYOUT){
                        data.iVscrollInc = HIWORD(wParam) - data.iLayoutVscrollPos;
                    }
                    break;
                default :
                    data.iVscrollInc = 0;
            }
            if(data.p_m == ORDINARY){
                data.iVscrollInc = max(
                -data.iVscrollPos,
                min(data.iVscrollInc, data.iVscrollMax - data.iVscrollPos)
                );
            }
            if(data.p_m == LAYOUT){
                data.iVscrollInc = max(
                -data.iLayoutVscrollPos,
                min(data.iVscrollInc, data.iVscrollMax - data.iLayoutVscrollPos)
                );
            }
            if (data.iVscrollInc != 0)
            {
                if(data.p_m == ORDINARY){
                    data.iVscrollPos += data.iVscrollInc;
                    ScrollWindow(hwnd, 0, -data.cyChar * data.iVscrollInc * data.Sc_pos, NULL, NULL);
                    SetScrollPos(hwnd, SB_VERT, data.iVscrollPos, TRUE);
                }
                if(data.p_m == LAYOUT){
                    j = data.iVscrollInc * data.Sc_pos;
                    if(j > 0){
                        while(j != 0){
                            if(data.start_str[data.iVscrollPos].end_str - data.start_str[data.iVscrollPos].start_str < data.start_layout + data.cxClient / data.cxChar){
                                data.iVscrollPos++;
                                data.start_layout = 0;
                            }
                            else{
                                data.start_layout += data.cxClient / data.cxChar;
                            }
                            j--;
                        }
                        data.iLayoutVscrollPos += data.iVscrollInc;
                    }
                    else{
                        while(j != 0){
                            if(data.start_layout - data.cxClient / data.cxChar < 0){
                                data.iVscrollPos--;
                                if(data.start_str[data.iVscrollPos].end_str - data.start_str[data.iVscrollPos].start_str > data.cxClient / data.cxChar){
                                    if((data.start_str[data.iVscrollPos].end_str - data.start_str[data.iVscrollPos].start_str) % (data.cxClient / data.cxChar) != 0){
                                    data.start_layout = data.start_str[data.iVscrollPos].end_str - data.start_str[data.iVscrollPos].start_str - ((data.start_str[data.iVscrollPos].end_str - data.start_str[data.iVscrollPos].start_str) % (data.cxClient / data.cxChar));
                                    }
                                    else{
                                        data.start_layout = data.start_str[data.iVscrollPos].end_str - data.start_str[data.iVscrollPos].start_str - data.cxClient / data.cxChar;
                                    }
                                }
                                else{
                                    data.start_layout = 0;
                                }
                            }
                            else{
                                data.start_layout -= data.cxClient / data.cxChar;
                            }
                            j++;
                        }
                        data.iLayoutVscrollPos -= data.iVscrollInc;
                    }
                    ScrollWindow(hwnd, 0, -data.cyChar * data.iVscrollInc * data.Sc_pos, NULL, NULL);
                    SetScrollPos(hwnd, SB_VERT, data.iLayoutVscrollPos, TRUE);
                }
                hdc = BeginPaint(hwnd, &ps);
                lprect = &ps.rcPaint;
                lprect->top -= (lprect->top % data.cyChar + 1);
                EndPaint(hwnd, &ps);
                InvalidateRect(hwnd, lprect, TRUE);
            }
            return 0;
        case WM_HSCROLL :
        switch(LOWORD(wParam))
        {
        case SB_LINEUP :
            data.iHscrollInc = -1;
            break;
        case SB_LINEDOWN :
            data.iHscrollInc = 1;
            break;
        case SB_PAGEUP :
            data.iHscrollInc = -8;
            break;
        case SB_PAGEDOWN :
            data.iHscrollInc = 8;
            break;
        case SB_THUMBTRACK :
            data.iHscrollInc = HIWORD(wParam) - data.iHscrollPos;
            break;
        default :
            data.iHscrollInc = 0;
        }
        data.iHscrollInc = max(
        -data.iHscrollPos,
        min(data.iHscrollInc, data.iHscrollMax - data.iHscrollPos)
        );
        if (data.iHscrollInc != 0)
        {
        data.iHscrollPos += data.iHscrollInc;
        ScrollWindow(hwnd, -data.cxChar * data.iHscrollInc, 0, NULL, NULL);
        SetScrollPos(hwnd, SB_HORZ, data.iHscrollPos, TRUE);
        hdc = BeginPaint(hwnd, &ps);
        lprect = &ps.rcPaint;
        lprect->left -= (lprect->left % data.cxChar + data.cxChar);
        EndPaint(hwnd, &ps);
        InvalidateRect(hwnd, lprect, TRUE);
        }
        return 0;
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            GetClientRect(hwnd, &rect);
            SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
            GetTextMetrics(hdc, &tm);
            data.cxChar = tm.tmAveCharWidth;
            data.cyChar = tm.tmHeight + tm.tmExternalLeading;
            switch(data.p_m){
                case ORDINARY:
                    iPaintBeg = max(0, data.iVscrollPos * data.Sc_pos + ps.rcPaint.top / data.cyChar - 1);
                    iPaintEnd = min(data.num_str, data.iVscrollPos * data.Sc_pos + ps.rcPaint.bottom / data.cyChar);
                    iPaintLeft = ps.rcPaint.left / data.cxChar + data.iHscrollPos;
                    iPaintRight = ps.rcPaint.right / data.cxChar + data.iHscrollPos + 1;
                    for(i = iPaintBeg; i < iPaintEnd; i++){
                        y = data.cyChar * (i - data.iVscrollPos * data.Sc_pos);
                        TextOut(hdc, ps.rcPaint.left, y, data.str_data + data.start_str[i].start_str + iPaintLeft,
                                 min(data.start_str[i].end_str - data.start_str[i].start_str - iPaintLeft, iPaintRight - iPaintLeft + 1));
                    }
                    EndPaint(hwnd, &ps);
                    break;
                case LAYOUT:
                    iPaintBeg = max(0, ps.rcPaint.top / data.cyChar - 1);
                    iPaintEnd = ps.rcPaint.bottom / data.cyChar;
                    num = data.start_layout;
                    j = 0;
                    q = data.iVscrollPos;
                    while(j < iPaintBeg){
                        if(data.start_str[q].end_str - data.start_str[q].start_str - num > data.cxClient / data.cxChar){
                            num += data.cxClient / data.cxChar;
                        }
                        else{
                            q++;
                            num = 0;
                        }
                        j++;
                    }
                    for(i = iPaintBeg; (i <= iPaintEnd) && (q < data.num_str); i++){
                        y = data.cyChar * i;
                        TextOut(hdc, 0, y, data.str_data + data.start_str[q].start_str + num,
                                min(data.start_str[q].end_str - data.start_str[q].start_str - num, data.cxClient / data.cxChar));
                        if(data.start_str[q].end_str - data.start_str[q].start_str - num > data.cxClient / data.cxChar){
                            num += data.cxClient / data.cxChar;
                        }
                        else{
                            q++;
                            num = 0;
                        }
                    }
                    EndPaint(hwnd, &ps);
                    break;
                case NODATA:
                    EndPaint(hwnd, &ps);
                    break;
            }
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_KEYDOWN:
            switch(wParam){
            case VK_UP:
                SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0L);
                break;
            case VK_DOWN:
                SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
                break;
            case VK_LEFT:
                SendMessage(hwnd, WM_HSCROLL, SB_LINEUP, 0L);
                break;
            case VK_RIGHT:
                SendMessage(hwnd, WM_HSCROLL, SB_LINEDOWN, 0L);
                break;
            case VK_PRIOR:
                SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0L);
                break;
            case VK_NEXT:
                SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0L);
                break;
            }
            return 0;

    case WM_COMMAND :
        hMenu = GetMenu(hwnd);
        switch(LOWORD(wParam)){
        case IDM_OPEN:
            if(PopFileOpenDlg(hwnd, szFileName, szTitleName)){
                if(data.start_str != NULL){
                    free(data.start_str);
                    data.start_str = NULL;
                }
                if(data.str_data != NULL){
                    free(data.str_data);
                    data.str_data = NULL;
                }
                data.num_str = 0;
                data.num_layout_str = 0;
                data.iHscrollPos = 0;
                data.iVscrollPos = 0;
                Read(szFileName, &data);
            }
            SetScrollRange(hwnd, SB_VERT, 0, data.iVscrollMax, TRUE);
            SetScrollPos(hwnd, SB_VERT, 0, TRUE);
            SetScrollRange(hwnd, SB_HORZ, 0, data.iHscrollMax, TRUE);
            SetScrollPos(hwnd, SB_HORZ, data.iHscrollPos, TRUE);
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        case IDM_CLOSE:
            Clear(&data);
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        case IDM_EXIT:
            Clear(&data);
            SendMessage(hwnd, WM_DESTROY, 0, 0L);
            return 0;
        case IDM_ORDINARY_MODE:
            EnableMenuItem(hMenu, IDM_ORDINARY_MODE, MF_GRAYED);
            EnableMenuItem(hMenu, IDM_LAYOUT_MODE, MF_ENABLED);
            data.p_m = ORDINARY;
            data.Sc_pos = (data.num_str > pow(2, 16)) ? data.num_str / pow(2, 16) + 1 : 1;
            data.iVscrollPos = data.iVscrollPos / data.Sc_pos;
            data.iVscrollMax = (int)(max(0, data.num_str + 2 - data.cyClient / data.cyChar) / data.Sc_pos);
            SetScrollRange(hwnd, SB_VERT, 0, data.iVscrollMax, TRUE);
            SetScrollPos(hwnd, SB_VERT, data.iVscrollPos, TRUE);
            data.iHscrollMax = max(0, data.iMaxWidth - data.cxClient / data.cxChar);
            data.iHscrollPos = min(data.iHscrollPos, data.iHscrollMax);
            SetScrollRange(hwnd, SB_HORZ, 0, data.iHscrollMax, TRUE);
            SetScrollPos(hwnd, SB_HORZ, data.iHscrollPos, TRUE);
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        case IDM_LAYOUT_MODE:
            EnableMenuItem(hMenu, IDM_ORDINARY_MODE, MF_ENABLED);
            EnableMenuItem(hMenu, IDM_LAYOUT_MODE, MF_GRAYED);
            data.p_m = LAYOUT;
            data.iVscrollPos = data.iVscrollPos * data.Sc_pos;
            data.iLayoutVscrollPos = Scroll_Pos(data.start_str, data.iVscrollPos, data.cxClient / data.cxChar);
            data.start_layout = 0;
            data.num_layout_str = Layout_str(data.start_str, data.num_str, data.cxClient / data.cxChar);
            data.Sc_pos = (data.num_layout_str > pow(2, 16)) ? data.num_layout_str / pow(2, 16) + 1 : 1;
            data.iVscrollMax = (int)(max(0, data.num_layout_str + 2 - data.cyClient / data.cyChar) / data.Sc_pos);
            SetScrollRange(hwnd, SB_VERT, 0, data.iVscrollMax, TRUE);
            SetScrollPos(hwnd, SB_VERT, data.iLayoutVscrollPos, TRUE);
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        default:
            break;
        }
        break;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void Clear(struct data_t* data){//функция для очистки данных структуры data_t. Используется при закрытии файла и программы. На вход принимает указатель на структуру data_t
    if(data->start_str != NULL){
        free(data->start_str);
        data->start_str = NULL;
    }
    if(data->str_data != NULL){
        free(data->str_data);
        data->str_data = NULL;
    }
    data->num_str = 0;
    data->num_layout_str = 0;
    data->iHscrollPos = 0;
    data->iVscrollPos = 0;
}

void Read(char szFileName[], struct data_t* data){//Функция, считывающая данные с файла и записывающая их структуру data_t, которая хранит данные окна. На вход принимает имя файла и указатель на структуру.
    FILE* file;
    int i, j, size;
    data->start_layout = 0;
    data->iHscrollPos = 0;
    file = fopen(szFileName, "rb");
    if(file == NULL){
        return;
    }
            fseek(file, 0, SEEK_END);
            size = (int)ftell(file);
            data->str_data = (char*)malloc(sizeof(char) * (size + 1));
            fseek(file, 0, SEEK_SET);
            fread(data->str_data, 1, size, file);
            j = 0;
            data->num_str = 1;
            data->iMaxWidth = 0;
            for(i = 0; i < size; i++){
                j++;
                if((data->str_data[i] == '\n')){
                    if(j > data->iMaxWidth){
                        data->iMaxWidth = j;
                    }
                    j = 0;
                    data->num_str++;
                }
            }
            data->str_data[i] = '\0';
            data->Sc_pos = (data->num_str > pow(2, 16)) ? data->num_str / pow(2, 16) + 1 : 1;
            data->start_str = (struct Str_data_t*)malloc(sizeof(struct Str_data_t)* (data->num_str));
            data->start_str[0].start_str = 0;
            j = 0;
            for(i = 1; i < size; i++){
                if(data->str_data[i] == '\n'){
                    data->start_str[j].end_str = i - 1;
                    data->start_str[j + 1].start_str = i + 1;
                    j++;
                }
            }
            data->iHscrollMax = max(0, data->iMaxWidth - data->cxClient / data->cxChar);
            data->iHscrollPos = min(data->iHscrollPos, data->iHscrollMax);
            data->start_str[j].end_str = size + 1;
            data->iVscrollMax = min(pow(2, 16), (max(0, data->num_str + 2 - data->cyClient / data->cyChar) / data->Sc_pos));
            if(data->p_m == LAYOUT){
                data->iLayoutVscrollPos = 0;
                data->num_layout_str = Layout_str(data->start_str, data->num_str, data->cxClient / data->cxChar);
                data->iHscrollMax = 0;
                data->iVscrollMax = (int)(max(0, data->num_layout_str + 2 - data->cyClient / data->cyChar) / data->Sc_pos);
            }
}
