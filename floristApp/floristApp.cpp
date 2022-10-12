// floristApp.cpp : Defines the entry point for the application.
//

#include<thread>

#include <windows.h>
#include "framework.h"
#include "floristApp.h"
#include "nnOperation.h"
#include <gdiplus.h>
#include <objidl.h>
#include<iostream>

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

using namespace std;

#define MAX_LOADSTRING 100

#define MENU_TRAINING  001
#define MENU_SHOW_RESULTS  002
#define RADIO_INIT 010
#define RADIO_LOAD 011
#define STATIC_1 101

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


nnOperation nn1;

HWND hLbl,hBtnSave,hBtnStartStop,hRBtn1,hRBtn2,hBtnInit,hBtnTest,hBtnSample;
HMENU hMenuBar, hMenu;


bool train_res = true;



// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



void thread1(HDC hdc, HWND hlbl) {
    nn1.trainOp(hdc,hlbl);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FLORISTAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);


    // Perform application initialization:

    setlocale(LC_ALL, "Russian");
    
    HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        100,100, 540,800, nullptr, nullptr, hInstance, nullptr);
   // CreateWindow()

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FLORISTAPP));
    MSG msg;
    HDC hdc = GetDC(hWnd);

   
    thread tr1(thread1, hdc, hLbl);
    tr1.detach();
   
    

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

        }
       
    }

    return (int) msg.wParam;
}












ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FLORISTAPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FLORISTAPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

  

   return TRUE;
}

void hideEveryThing() {


    ShowWindow(hBtnSave, SW_HIDE);
    ShowWindow(hBtnInit, SW_HIDE);
    SetWindowTextA(hLbl, "");
    ShowWindow(hLbl, SW_HIDE);
    ShowWindow(hBtnStartStop, SW_HIDE);
    ShowWindow(hRBtn1, SW_HIDE);
    ShowWindow(hRBtn2, SW_HIDE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Gdiplus::GdiplusStartupInput gdiplusStartInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartInput, nullptr);
    switch (message)
    {
    case WM_CREATE:                                                                         //______Window Components
        {
        hInst = ((LPCREATESTRUCT)lParam)->hInstance;

        hLbl = CreateWindow(
            L"static", L"label1",
            WS_CHILD | WS_VISIBLE,
            10, 520, 300, 95,
            hWnd, (HMENU)STATIC_1, NULL, 0
        );
       
        
        ShowWindow(hLbl, SW_HIDE);
        /*
        HWND hEdt1 = CreateWindow(L"edit", L"10",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 50, 50, 60, 20,
            hWnd, NULL, hInst, NULL);
        ShowWindow(hEdt1, SW_SHOWNORMAL);
        */

       

        hBtnStartStop = CreateWindow(L"button", L"Start training",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            360, 520, 150, 30, hWnd, NULL, hInst, NULL);
        ShowWindow(hBtnStartStop, SW_HIDE);

        hBtnInit = CreateWindow(L"button", L"Initialize new",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            360, 555, 150, 30, hWnd, NULL, hInst, NULL);
        ShowWindow(hBtnInit, SW_HIDE);

        hBtnSave = CreateWindow(L"button", L"Save NN",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            360, 590, 150, 30, hWnd, NULL, hInst, NULL);
        ShowWindow(hBtnSave, SW_HIDE);

        hRBtn1 = CreateWindow(L"button", L"Initialize new NN",
            WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
            10, 520, 150, 30, hWnd, (HMENU)RADIO_INIT, hInst, NULL);
        ShowWindow(hRBtn1, SW_HIDE);
        hBtnTest = CreateWindow(L"button", L"TEST",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            360, 660, 150, 30, hWnd,NULL, hInst, NULL);
        ShowWindow(hBtnTest, SW_SHOW);
        hRBtn2 = CreateWindow(L"button", L"Load NN from file",
            WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
            10, 555, 150, 30, hWnd, (HMENU)RADIO_LOAD, hInst, NULL);
        ShowWindow(hRBtn2, SW_HIDE);

        hMenuBar = CreateMenu();
        hMenu = CreateMenu();
        AppendMenu(hMenu, MF_STRING, MENU_TRAINING, L"&Training");
        AppendMenu(hMenu, MF_STRING, MENU_SHOW_RESULTS, L"&Results");
        AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hMenu, L"&FileTEST");
        SetMenu(hWnd, hMenuBar);
        break;
        }

    
    case WM_CTLCOLORSTATIC:
    {

        HDC hdcStatic = (HDC)wParam; // or obtain the static handle in some other way
        SetBkMode(hdcStatic, OPAQUE);
        SetTextColor(hdcStatic, RGB(0, 0, 0)); // text color
        SetBkColor(hdcStatic, RGB(255, 255, 255));
       

        if (GetDlgCtrlID((HWND)lParam) == STATIC_1) // Target the specific component
        {
            HDC hdcStatic = (HDC)wParam; // or obtain the static handle in some other way
            SetBkMode(hdcStatic, OPAQUE);
            SetTextColor(hdcStatic, RGB(0, 0, 0)); // text color
            
            SetBkColor(hdcStatic, RGB(255, 255,255));
            return (LRESULT)GetStockObject(WHITE_BRUSH);
        }
        return (LRESULT)GetStockObject(WHITE_BRUSH);
    }
    break;
    
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            
            // Parse the menu selections:

            if (lParam == (LPARAM)hBtnSave)    // Saving
            {
                nn1.nn.saveToFile();
                SetWindowTextA(hLbl, "Saved");
            }
            if (lParam == (LPARAM)hBtnInit)    // Saving
            {
                nn1.nn.initialize();
            }
            if (lParam == (LPARAM)hBtnStartStop)    // START STOP training
            {

                if (train_res) {
                    if (!nn1.training) SetWindowTextA(hBtnStartStop, "Stop training");
                    else SetWindowTextA(hBtnStartStop, "Start training");
                    nn1.training = !nn1.training;

                    ShowWindow(hBtnSave, SW_SHOW);
                    ShowWindow(hBtnInit, SW_SHOW);
                   
                }
                else {
                    if (!nn1.showResults) SetWindowTextA(hBtnStartStop, "Stop showing results");
                    else SetWindowTextA(hBtnStartStop, "Start showing results");
                    nn1.showResults = !nn1.showResults;

                    ShowWindow(hBtnSave, SW_HIDE);
                    ShowWindow(hBtnInit, SW_HIDE);
                }

                ShowWindow(hRBtn1, SW_HIDE);
                ShowWindow(hRBtn2, SW_HIDE);


            }
            if (lParam == (LPARAM)hBtnTest)    // Test
            {
                hideEveryThing();
                nn1.evaluateImage(GetDC(hWnd),hLbl);
                ShowWindow(hLbl, SW_SHOW);
               // nn1.makeTrainingSamples(hLbl);
            }

            switch (LOWORD(wParam)) {

            case MENU_TRAINING:
                SetWindowTextA(hBtnStartStop, "Start training");
                train_res = true;
                nn1.training = false;
                nn1.showResults = false;
                hideEveryThing();

                ShowWindow(hRBtn2, SW_SHOW);
                ShowWindow(hRBtn1, SW_SHOW);
                SendDlgItemMessage(hWnd, RADIO_LOAD, BM_SETCHECK, 0, 0);
                SendDlgItemMessage(hWnd, RADIO_INIT, BM_SETCHECK, 0, 0);
                
                break;

            case MENU_SHOW_RESULTS:
                SetWindowTextA(hBtnStartStop, "Start showing results");
                train_res = false;
                nn1.training = false;
                nn1.showResults = false;
                hideEveryThing();
                ShowWindow(hBtnStartStop, SW_SHOW);
                ShowWindow(hLbl, SW_SHOW);
                
    
                break;

            case RADIO_INIT:                                                        // NEW NN
                if (SendDlgItemMessage(hWnd, RADIO_INIT, BM_GETCHECK, 0, 0) == 0)
                {
                    ShowWindow(hBtnStartStop, SW_SHOW);
                    ShowWindow(hLbl, SW_SHOW);
                    nn1.loadFile = false;
                    nn1.setup = true;
                   
                    SendDlgItemMessage(hWnd, RADIO_INIT, BM_SETCHECK, 1, 0);
                    SendDlgItemMessage(hWnd, RADIO_LOAD, BM_SETCHECK, 0, 0);
                }
                break;
            case RADIO_LOAD:                                                        //LOAD NN
                if (SendDlgItemMessage(hWnd, RADIO_LOAD, BM_GETCHECK, 0, 0) == 0)
                {
                    ShowWindow(hBtnStartStop, SW_SHOW);
                    ShowWindow(hLbl, SW_SHOW);
                    nn1.loadFile = true;
                    nn1.setup = true;
                    SendDlgItemMessage(hWnd, RADIO_LOAD, BM_SETCHECK, 1, 0);
                    SendDlgItemMessage(hWnd, RADIO_INIT, BM_SETCHECK, 0, 0);
                }
                break;
            }

            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
                
            }

            

        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    


    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
