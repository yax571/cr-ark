// CrArkGUI.cpp : main project file.

#include "stdafx.h"
#include "FormMain.h"

using namespace CrArkGUI;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{

    //首先加载初始化DLL文件
    if(!CrInitialize()) {
        MessageBoxW(NULL, L"核心初始化失败", L"错误", MB_OK);
        return 0;
    }

    // Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());

    CrUninitialize();
	return 0;
}
