// CrArkGUI.cpp : main project file.

#include "stdafx.h"
#include "FormMain.h"

using namespace CrArkGUI;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{

    //���ȼ��س�ʼ��DLL�ļ�
    if(!CrInitialize()) {
        MessageBoxW(NULL, L"���ĳ�ʼ��ʧ��", L"����", MB_OK);
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
