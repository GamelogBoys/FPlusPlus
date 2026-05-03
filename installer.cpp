#include <windows.h>
#include <commctrl.h>
#include <wininet.h>
#include <shlobj.h>
#include <thread>
#include <string>
#include <fstream>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")

HWND hProgressBar, hStatus, hPathEdit;
std::string selectedPath = "C:\\FPlusPlus";

// --- THE ACTUAL INSTALL LOGIC ---
void InstallRoutine(HWND hwnd) {
    // 1. Ensure the directory exists
    CreateDirectoryA(selectedPath.c_str(), NULL);
    
    SetWindowTextA(hStatus, "Status: Connecting to server...");
    SendMessage(hProgressBar, PBM_SETPOS, 5, 0);

    HINTERNET hInternet = InternetOpenA("FPlusPlusInstaller", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    // CRITICAL: Replace this URL with your actual direct link to fpp.exe
    HINTERNET hUrl = InternetOpenUrlA(hInternet, "https://github.com/your-username/your-repo/releases/latest/download/fpp.exe", NULL, 0, INTERNET_FLAG_RELOAD, 0);

    if (!hUrl) {
        SetWindowTextA(hStatus, "Status: Error! Could not connect to server.");
        InternetCloseHandle(hInternet);
        return;
    }

    // 2. Open local file for writing
    std::string fullFilePath = selectedPath + "\\fpp.exe";
    HANDLE hFile = CreateFileA(fullFilePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        SetWindowTextA(hStatus, "Status: Error! Permissions denied. Run as Admin.");
        InternetCloseHandle(hUrl);
        InternetCloseHandle(hInternet);
        return;
    }

    // 3. The Actual Download/Save Loop
    char buffer[8192];
    DWORD bytesRead;
    int totalBytesRead = 0;
    SetWindowTextA(hStatus, "Status: Downloading frun.exe...");

    while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        DWORD bytesWritten;
        WriteFile(hFile, buffer, bytesRead, &bytesWritten, NULL);
        
        // Simple progress update (moves bar until ~90%)
        totalBytesRead += bytesRead;
        int currentPos = (totalBytesRead / 1024) % 90; 
        SendMessage(hProgressBar, PBM_SETPOS, 10 + currentPos, 0);
    }

    CloseHandle(hFile);
    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    // 4. Finish
    SendMessage(hProgressBar, PBM_SETPOS, 100, 0);
    SetWindowTextA(hStatus, "Status: Installation Successful!");
    MessageBoxA(hwnd, ("F++ has been installed to:\n" + selectedPath).c_str(), "Success", MB_OK | MB_ICONINFORMATION);
}

// Folder Picker Logic
void SelectFolder(HWND hwnd) {
    char path[MAX_PATH];
    BROWSEINFOA bi = { 0 };
    bi.lpszTitle = "Select Install Folder";
    LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);
    if (pidl != 0) {
        SHGetPathFromIDListA(pidl, path);
        selectedPath = std::string(path);
        // Ensure path ends correctly
        if (selectedPath.back() != '\\') selectedPath += "\\";
        selectedPath += "FPlusPlus";
        SetWindowTextA(hPathEdit, selectedPath.c_str());
    }
}

// Window Logic
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            CreateWindowA("Static", "F++ Professional Installer", WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 20, 1280, 40, hwnd, NULL, NULL, NULL);
            
            // Terms
            CreateWindowA("Static", "Terms of Service:", WS_VISIBLE | WS_CHILD, 140, 80, 200, 20, hwnd, NULL, NULL, NULL);
            CreateWindowExA(WS_EX_CLIENTEDGE, "Edit", "1. Do not decompile.\r\n2. Run using command './fpp run file_location'.\r\n3. Enjoy.", 
                            WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_READONLY, 140, 110, 1000, 300, hwnd, NULL, NULL, NULL);

            // Path
            CreateWindowA("Static", "Install Folder:", WS_VISIBLE | WS_CHILD, 140, 450, 200, 20, hwnd, NULL, NULL, NULL);
            hPathEdit = CreateWindowExA(WS_EX_CLIENTEDGE, "Edit", selectedPath.c_str(), WS_VISIBLE | WS_CHILD | ES_READONLY, 140, 480, 850, 30, hwnd, NULL, NULL, NULL);
            CreateWindowA("Button", "Browse", WS_VISIBLE | WS_CHILD, 1000, 480, 140, 30, hwnd, (HMENU)2, NULL, NULL);

            // Progress
            hStatus = CreateWindowA("Static", "Status: Ready", WS_VISIBLE | WS_CHILD, 140, 550, 1000, 20, hwnd, NULL, NULL, NULL);
            hProgressBar = CreateWindowExA(0, PROGRESS_CLASSA, NULL, WS_VISIBLE | WS_CHILD | PBS_SMOOTH, 140, 580, 1000, 35, hwnd, NULL, NULL, NULL);

            // Action
            CreateWindowA("Button", "AGREE & INSTALL", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 540, 640, 200, 50, hwnd, (HMENU)1, NULL, NULL);
            break;
        }
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) std::thread(InstallRoutine, hwnd).detach();
            if (LOWORD(wParam) == 2) SelectFolder(hwnd);
            break;
        case WM_DESTROY: PostQuitMessage(0); break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hI, HINSTANCE, LPSTR, int nS) {
    InitCommonControls();
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hI;
    wc.lpszClassName = "FPP_FINAL";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    RegisterClassA(&wc);
    HWND h = CreateWindowA("FPP_FINAL", "F++ Setup", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, hI, NULL);
    ShowWindow(h, nS);
    MSG m;
    while (GetMessage(&m, NULL, 0, 0)) { TranslateMessage(&m); DispatchMessage(&m); }
    return 0;
}