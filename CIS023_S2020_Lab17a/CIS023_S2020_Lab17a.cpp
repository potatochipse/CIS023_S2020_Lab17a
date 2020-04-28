// Module:		CIS023_S2020_Lab17a.cpp
// Author:		Miguel Antonio Logarta
// Date:		April 27, 2020
// Purpose:		Learn how to create a stack.
//				Initialize, push, pop, and delete stacks.
//				Use the standard file openand standard file save dialog boxes.
//

#include "framework.h"
#include "CIS023_S2020_Lab17a.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND hWnd_InFile;								// handle to file input edit box
HWND hWnd_OutFile;								// handle to file output edit box
HWND hWnd_btnOpen;								// butten, standard file open dialog
HWND hWnd_btnSave;								// button, standard file save as dialog

char fInputPath[MAX_PATH];						// input file name, selected by user
char fOutputPath[MAX_PATH];						// output file name, selected by user

vector<Radix> vNode;							// Create a stack

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool GetOpenFileName();							// prompt for read filespec using common open dialog box
bool GetSaveFileName();							// prompt for wrute filespec using common open dialog box
bool LoadFile();								// load file contents into vector
void SaveFile();								// save list into output file

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
    LoadStringW(hInstance, IDC_CIS023S2020LAB17A, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CIS023S2020LAB17A));

    MSG msg;

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



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CIS023S2020LAB17A));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+0);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CIS023S2020LAB17A);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 410, 200, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case BN_CLICKED:	// capture button click
			if (LOWORD(lParam) == (WORD)hWnd_btnOpen)	// launch standard file open dialog
			{
				if (GetOpenFileName())					// input filespec
				{
					if (LoadFile())						// load file into vector
						// enable the save button
						EnableWindow(hWnd_btnSave, true);
				}

			}

			else if (LOWORD(lParam) == (WORD)hWnd_btnSave) // launch standard file save dialog
			{
				if (GetSaveFileName())					// get output filespec
				{
					SaveFile();							// empty vector into output file

					// disable save button
					EnableWindow(hWnd_btnSave, false);
				}
			}
			break;
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
	case WM_CREATE:
	{
		CreateWindow(TEXT("STATIC"),
			TEXT("     Input file name:   "),
			WS_VISIBLE | WS_CHILD, 0, 10, 400, 20, hWnd, NULL, NULL, NULL);

		CreateWindow(TEXT("STATIC"),
			TEXT("     Output file name:   "),
			WS_VISIBLE | WS_CHILD, 0, 40, 400, 20, hWnd, NULL, NULL, NULL);

		hWnd_InFile = CreateWindow(TEXT("STATIC"),
			TEXT(""),
			WS_VISIBLE | WS_CHILD, 150, 10, 250, 20, hWnd, NULL, NULL, NULL);

		hWnd_OutFile = CreateWindow(TEXT("STATIC"),
			TEXT(""),
			WS_VISIBLE | WS_CHILD, 150, 40, 250, 20, hWnd, NULL, NULL, NULL);

		hWnd_btnOpen = CreateWindow(TEXT("BUTTON"),
			TEXT("Open"),
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			160, 100, 70, 30, hWnd, NULL, NULL, NULL);

		hWnd_btnSave = CreateWindow(TEXT("BUTTON"),
			TEXT("Save"),
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			250, 100, 70, 30, hWnd, NULL, NULL, NULL);

		EnableWindow(hWnd_btnSave, false); // Save button disabled until file successfully loaded

		SetWindowText(hWnd, _T("CIS 023, Lab 17a, Miguel Antonio Logarta"));
	}
	break;

	case WM_DESTROY:
		vNode.empty();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
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


// use common file open dialog box to get input filespec
bool GetOpenFileName()
{
	char szFileNameIN[MAX_PATH];

	// get the input file name
	OPENFILENAME ofn;
	ZeroMemory(&fInputPath, sizeof(fInputPath));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = LPWSTR("Comma delimeted\0*.csv\0");
	ofn.lpstrFile = LPWSTR(fInputPath);
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = LPWSTR(szFileNameIN);
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrTitle = LPWSTR("Select an input File");
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(LPOPENFILENAMEA(&ofn))) // user selected an input file
	{
		// display input file name on window
		size_t pReturnValue;
		wchar_t wcstr[MAX_LOADSTRING];
		mbstowcs_s(&pReturnValue, wcstr, MAX_LOADSTRING, szFileNameIN, MAX_LOADSTRING);
		SendMessage(hWnd_InFile, WM_SETTEXT, NULL, LPARAM(wcstr));

		return true;
	}
	else
		return false;
}

// use common save file dialog box to get output filespece
bool GetSaveFileName()
{
	char szFileNameOUT[MAX_PATH];

	// get the output file name
	OPENFILENAME ofn;
	ZeroMemory(&fOutputPath, sizeof(fOutputPath));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = LPWSTR("All files\0*.*\0");
	ofn.lpstrFile = LPWSTR(fOutputPath);
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = LPWSTR(szFileNameOUT);
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrTitle = LPWSTR("Select an output File");
	ofn.Flags = OFN_DONTADDTORECENT | OFN_OVERWRITEPROMPT;

	if (GetSaveFileNameA(LPOPENFILENAMEA(&ofn))) // user selected an output file
	{
		// copy the string into a char*
		size_t pReturnValue;
		wchar_t wcstr[MAX_LOADSTRING];
		mbstowcs_s(&pReturnValue, wcstr, MAX_LOADSTRING, szFileNameOUT, MAX_LOADSTRING);
		SendMessage(hWnd_OutFile, WM_SETTEXT, NULL, LPARAM(wcstr));

		return true;
	}
	else
		return false;

}

// load file contents into vector
bool LoadFile()
{
	string strName = "";								// temps to hold file data
	string strDec = "";
	string strHex = "";
	string strOct = "";
	string strBin = "";

	ifstream fInput;									// file pointer

	try
	{
		// make sure the file will open before creating the stack
		fInput.open(fInputPath);						// open file
		if (!fInput)
			throw 1;

		// Push the file contents onto the stack
		while (fInput.good())
		{
			// Read the strings from the input file
			getline(fInput, strName, ',');
			getline(fInput, strDec, ',');
			getline(fInput, strHex, ',');
			getline(fInput, strOct, ',');
			getline(fInput, strBin, '\n');

			Radix newNode(strName, strDec, strHex, strOct, strBin);			// Create new object to store those values
			vNode.push_back(newNode);										// Push object to stack
		}


		fInput.close();					// done with the input file

		return true;
	}
	catch (int)							// file open error
	{
		MessageBox(NULL,
			TEXT("Unable to open input file."),
			TEXT("File Open Error"),
			MB_ICONEXCLAMATION);

		return false;
	}

	return true;

}

// empty vector into output file
void SaveFile()
{
	ofstream outFile;					// output file pointer
	string strData;						// node data converted to string

	try
	{
		// make sure the file will open before creating the stack
		outFile.open(fOutputPath);
		if (outFile.bad())
			throw 1;

		// Write stack to the output file
		while (vNode.size() > 0)				// Until the stack is empty
		{
			// Temporary variables to hold file data
			string strName = "";								
			string strDec = "";
			string strHex = "";
			string strOct = "";
			string strBin = "";

			Radix *newNode = &vNode.back();					// Create pointer to object at the top of the stack
			
			// Convert TCHARs to strings 
			TCHARtoString(newNode->GetName(), &strName);	
			TCHARtoString(newNode->GetDec(), &strDec);
			TCHARtoString(newNode->GetHex(), &strHex);
			TCHARtoString(newNode->GetOct(), &strOct);
			TCHARtoString(newNode->GetBin(), &strBin);

			// Output converted strings to the output file
			outFile << strName<< ','
				<< strDec << ',' 
				<< strHex << ','
				<< strOct << ','
				<< strBin << '\n';

			// Pop the stack
			vNode.pop_back();
		}

		outFile.close();				// done with the output file
	}
	catch (int)
	{
		MessageBox(NULL,
			TEXT("Unable to open ouput file."),
			TEXT("File Write Error"),
			MB_ICONEXCLAMATION);

	}

}
