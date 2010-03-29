#pragma once

#include "FormProcess.h"
#include "FormDetail.h"

namespace CrArkGUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
            AutoRefresh = false;
		}

	protected:
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::MenuStrip^  menuStripFile;
    protected: 
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemFile;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemAutoRefresh;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemExit;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemTools;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemHelp;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemAbout;
    private: System::Windows::Forms::SplitContainer^  splitContainerMain;
    private: System::Windows::Forms::Timer^  timerRefresh;
    private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->components = (gcnew System::ComponentModel::Container());
            this->menuStripFile = (gcnew System::Windows::Forms::MenuStrip());
            this->menuItemFile = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuItemAutoRefresh = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuItemExit = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuItemTools = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuItemHelp = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuItemAbout = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->splitContainerMain = (gcnew System::Windows::Forms::SplitContainer());
            this->timerRefresh = (gcnew System::Windows::Forms::Timer(this->components));
            this->menuStripFile->SuspendLayout();
            this->splitContainerMain->SuspendLayout();
            this->SuspendLayout();
            // 
            // menuStripFile
            // 
            this->menuStripFile->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->menuItemFile, 
                this->menuItemTools, this->menuItemHelp});
            this->menuStripFile->Location = System::Drawing::Point(0, 0);
            this->menuStripFile->Name = L"menuStripFile";
            this->menuStripFile->Size = System::Drawing::Size(641, 24);
            this->menuStripFile->TabIndex = 0;
            this->menuStripFile->Text = L"menuStrip1";
            // 
            // menuItemFile
            // 
            this->menuItemFile->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->menuItemAutoRefresh, 
                this->menuItemExit});
            this->menuItemFile->Name = L"menuItemFile";
            this->menuItemFile->Size = System::Drawing::Size(59, 20);
            this->menuItemFile->Text = L"文件(&F)";
            // 
            // menuItemAutoRefresh
            // 
            this->menuItemAutoRefresh->CheckOnClick = true;
            this->menuItemAutoRefresh->Name = L"menuItemAutoRefresh";
            this->menuItemAutoRefresh->Size = System::Drawing::Size(152, 22);
            this->menuItemAutoRefresh->Text = L"自动刷新(&A)";
            this->menuItemAutoRefresh->Click += gcnew System::EventHandler(this, &Form1::menuItemAutoRefresh_Click);
            // 
            // menuItemExit
            // 
            this->menuItemExit->Name = L"menuItemExit";
            this->menuItemExit->Size = System::Drawing::Size(152, 22);
            this->menuItemExit->Text = L"退出(&X)";
            this->menuItemExit->Click += gcnew System::EventHandler(this, &Form1::menuItemExit_Click);
            // 
            // menuItemTools
            // 
            this->menuItemTools->Name = L"menuItemTools";
            this->menuItemTools->Size = System::Drawing::Size(59, 20);
            this->menuItemTools->Text = L"工具(&T)";
            // 
            // menuItemHelp
            // 
            this->menuItemHelp->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->menuItemAbout});
            this->menuItemHelp->Name = L"menuItemHelp";
            this->menuItemHelp->Size = System::Drawing::Size(59, 20);
            this->menuItemHelp->Text = L"帮助(&H)";
            // 
            // menuItemAbout
            // 
            this->menuItemAbout->Name = L"menuItemAbout";
            this->menuItemAbout->Size = System::Drawing::Size(112, 22);
            this->menuItemAbout->Text = L"关于(&A)";
            // 
            // splitContainerMain
            // 
            this->splitContainerMain->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainerMain->Location = System::Drawing::Point(0, 24);
            this->splitContainerMain->Name = L"splitContainerMain";
            this->splitContainerMain->Orientation = System::Windows::Forms::Orientation::Horizontal;
            this->splitContainerMain->Size = System::Drawing::Size(641, 543);
            this->splitContainerMain->SplitterDistance = 362;
            this->splitContainerMain->TabIndex = 1;
            // 
            // timerRefresh
            // 
            this->timerRefresh->Enabled = true;
            this->timerRefresh->Interval = 2000;
            this->timerRefresh->Tick += gcnew System::EventHandler(this, &Form1::timerRefresh_Tick);
            // 
            // Form1
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(641, 567);
            this->Controls->Add(this->splitContainerMain);
            this->Controls->Add(this->menuStripFile);
            this->MainMenuStrip = this->menuStripFile;
            this->Name = L"Form1";
            this->Text = L"Cr-AntiRootkit";
            this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
            this->menuStripFile->ResumeLayout(false);
            this->menuStripFile->PerformLayout();
            this->splitContainerMain->ResumeLayout(false);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

    private:
        Boolean AutoRefresh;
        FormProcess^ ProcessWnd;
        FormDetail^ DetailWnd;

    private: 
        //窗口加载时
        Void Form1_Load(Object^  sender, EventArgs^  e) {
            //详细信息窗口
            DetailWnd = gcnew FormDetail();
            DetailWnd->TopLevel = false;
            DetailWnd->Dock = DockStyle::Fill;
            splitContainerMain->Panel2->Controls->Add(DetailWnd);
            DetailWnd->Show();

            //进程窗口
            ProcessWnd = gcnew FormProcess(DetailWnd);
            ProcessWnd->TopLevel = false;
            ProcessWnd->Dock = DockStyle::Fill;
            splitContainerMain->Panel1->Controls->Add(ProcessWnd);
            ProcessWnd->Show();
        }

        //文件 -> 退出
        Void menuItemExit_Click(Object^  sender, EventArgs^  e) {
            this->Close();
        }

        //文件 -> 自动刷新
        Void menuItemAutoRefresh_Click(Object^  sender, EventArgs^  e) {
            ToolStripMenuItem^ item;

            item = (ToolStripMenuItem^)sender;
            AutoRefresh = item->Checked;
        }

        //刷新进程列表的定时器
        Void timerRefresh_Tick(Object^  sender, EventArgs^  e) {
            if(AutoRefresh)
                ProcessWnd->ProcessRefresh();
        }
};
}

