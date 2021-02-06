#include"OpenFile.h"

void PopFileInitialize(HWND hwnd){
    static char szFilter[] = "Text Files(*.TXT)\0*.txt\0";
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.hInstance = NULL;
    ofn.lpstrFilter = szFilter;
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter = 0;
    ofn.nFilterIndex = 0;
    ofn.lpstrFile = NULL; // Set in Open and Close functions
    ofn.nMaxFile = _MAX_PATH;
    ofn.lpstrFileTitle = NULL; // Set in Open and Close functions
    ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = NULL;
    ofn.Flags = 0; // Set in Open and Close functions
    ofn.nFileOffset = 0;
    ofn.nFileExtension = 0;
    ofn.lpstrDefExt = "txt";
    ofn.lCustData = 0L;
    ofn.lpfnHook = NULL;
    ofn.lpTemplateName = NULL;
}

BOOL PopFileOpenDlg(HWND hwnd, PSTR pstrFileName, PSTR pstrTitleName){
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = pstrFileName;
    ofn.lpstrFileTitle = pstrTitleName;
    ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
    return GetOpenFileName(&ofn);
}
