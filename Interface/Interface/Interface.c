#include"Inteface.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static HWND hDlgModeless;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    static char szAppName[] = "HelloWin";
    HWND hwnd;
    MSG msg;
    HACCEL hAccel;
    WNDCLASSEX wndclass;
    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground =(HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = "Menu";
    wndclass.lpszClassName = szAppName;
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&wndclass);
    hwnd = CreateWindow(szAppName, "System Metrics",
        WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, szCmdLine);
    hInst = hInstance;
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);
    hAccel = LoadAccelerators(hInstance, szAppName);
    while(GetMessage(&msg, NULL, 0, 0)){
        if(hDlgModeless == NULL || !IsDialogMessage(hDlgModeless, &msg)){
            if(!TranslateAccelerator(hwnd, hAccel, &msg)){
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
    return msg.wParam;
}

int Layout_str(struct Str_data_t* start_str, int num_str, int size){
    int num, i = 0;
    int count = 0;
    for(i = 0; i < num_str; i++){
        num = start_str[i].end_str - start_str[i].start_str;//без учёта знака перевода строки
        count += (num / size) + ((num % size > 0) ? 1 : 0);
    }
    return count;
}

int Scroll_Pos(struct Str_data_t* start_str, int scroll_pos, int size){
    int pos = 0;
    int i = 0;
    for(; i < scroll_pos; i++){
        pos += (start_str[i].end_str - start_str[i].start_str) / size + (((start_str[i].end_str - start_str[i].start_str) % size != 0) ? 1 : 0);
    }
    return pos;
}
