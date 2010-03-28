#pragma once

#include "..\CrArkKrnl\CrArkKrnl.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace CrArkGUI {

	/// <summary>
	/// Summary for FormDetail
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormDetail : public System::Windows::Forms::Form
	{
	public:
		FormDetail(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormDetail()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::TabControl^  tabControl;
    protected: 

    protected: 

    private: System::Windows::Forms::TabPage^  tabPageProcessDetail;
    private: System::Windows::Forms::TabPage^  tabPageThreadDetail;
    private: System::Windows::Forms::TabPage^  tabPageModuleDetail;
    private: System::Windows::Forms::ListView^  listViewProcDetail;
    private: System::Windows::Forms::ColumnHeader^  chProcDetailIndex;
    private: System::Windows::Forms::ColumnHeader^  chProcDetailName;
    private: System::Windows::Forms::ColumnHeader^  chProcDetailData;
    protected: 




	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->tabControl = (gcnew System::Windows::Forms::TabControl());
            this->tabPageProcessDetail = (gcnew System::Windows::Forms::TabPage());
            this->listViewProcDetail = (gcnew System::Windows::Forms::ListView());
            this->chProcDetailIndex = (gcnew System::Windows::Forms::ColumnHeader());
            this->chProcDetailName = (gcnew System::Windows::Forms::ColumnHeader());
            this->chProcDetailData = (gcnew System::Windows::Forms::ColumnHeader());
            this->tabPageThreadDetail = (gcnew System::Windows::Forms::TabPage());
            this->tabPageModuleDetail = (gcnew System::Windows::Forms::TabPage());
            this->tabControl->SuspendLayout();
            this->tabPageProcessDetail->SuspendLayout();
            this->SuspendLayout();
            // 
            // tabControl
            // 
            this->tabControl->Controls->Add(this->tabPageProcessDetail);
            this->tabControl->Controls->Add(this->tabPageThreadDetail);
            this->tabControl->Controls->Add(this->tabPageModuleDetail);
            this->tabControl->Dock = System::Windows::Forms::DockStyle::Fill;
            this->tabControl->Location = System::Drawing::Point(0, 0);
            this->tabControl->Name = L"tabControl";
            this->tabControl->SelectedIndex = 0;
            this->tabControl->Size = System::Drawing::Size(505, 273);
            this->tabControl->TabIndex = 0;
            this->tabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &FormDetail::tabControl_SelectedIndexChanged);
            // 
            // tabPageProcessDetail
            // 
            this->tabPageProcessDetail->Controls->Add(this->listViewProcDetail);
            this->tabPageProcessDetail->Location = System::Drawing::Point(4, 21);
            this->tabPageProcessDetail->Name = L"tabPageProcessDetail";
            this->tabPageProcessDetail->Padding = System::Windows::Forms::Padding(3);
            this->tabPageProcessDetail->Size = System::Drawing::Size(497, 248);
            this->tabPageProcessDetail->TabIndex = 0;
            this->tabPageProcessDetail->Text = L"详细信息";
            this->tabPageProcessDetail->UseVisualStyleBackColor = true;
            // 
            // listViewProcDetail
            // 
            this->listViewProcDetail->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {this->chProcDetailIndex, 
                this->chProcDetailName, this->chProcDetailData});
            this->listViewProcDetail->Dock = System::Windows::Forms::DockStyle::Fill;
            this->listViewProcDetail->FullRowSelect = true;
            this->listViewProcDetail->Location = System::Drawing::Point(3, 3);
            this->listViewProcDetail->MultiSelect = false;
            this->listViewProcDetail->Name = L"listViewProcDetail";
            this->listViewProcDetail->Size = System::Drawing::Size(491, 242);
            this->listViewProcDetail->TabIndex = 0;
            this->listViewProcDetail->UseCompatibleStateImageBehavior = false;
            this->listViewProcDetail->View = System::Windows::Forms::View::Details;
            // 
            // chProcDetailIndex
            // 
            this->chProcDetailIndex->Text = L"序号";
            // 
            // chProcDetailName
            // 
            this->chProcDetailName->Text = L"项目";
            this->chProcDetailName->Width = 100;
            // 
            // chProcDetailData
            // 
            this->chProcDetailData->Text = L"数据";
            this->chProcDetailData->Width = 300;
            // 
            // tabPageThreadDetail
            // 
            this->tabPageThreadDetail->Location = System::Drawing::Point(4, 21);
            this->tabPageThreadDetail->Name = L"tabPageThreadDetail";
            this->tabPageThreadDetail->Padding = System::Windows::Forms::Padding(3);
            this->tabPageThreadDetail->Size = System::Drawing::Size(497, 248);
            this->tabPageThreadDetail->TabIndex = 1;
            this->tabPageThreadDetail->Text = L"线程信息";
            this->tabPageThreadDetail->UseVisualStyleBackColor = true;
            // 
            // tabPageModuleDetail
            // 
            this->tabPageModuleDetail->Location = System::Drawing::Point(4, 21);
            this->tabPageModuleDetail->Name = L"tabPageModuleDetail";
            this->tabPageModuleDetail->Padding = System::Windows::Forms::Padding(3);
            this->tabPageModuleDetail->Size = System::Drawing::Size(497, 248);
            this->tabPageModuleDetail->TabIndex = 2;
            this->tabPageModuleDetail->Text = L"模块信息";
            this->tabPageModuleDetail->UseVisualStyleBackColor = true;
            // 
            // FormDetail
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(505, 273);
            this->Controls->Add(this->tabControl);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            this->Name = L"FormDetail";
            this->Text = L"FormDetail";
            this->Load += gcnew System::EventHandler(this, &FormDetail::FormDetail_Load);
            this->tabControl->ResumeLayout(false);
            this->tabPageProcessDetail->ResumeLayout(false);
            this->ResumeLayout(false);

        }
#pragma endregion
    private:
        PVOID CurrentProcess;
    public:
    property  PVOID EProcess {
        void set(PVOID Process) {
            CurrentProcess = Process;
            RefreshContent();
        }
        PVOID get() {
            return CurrentProcess;
        }
    }
    private:
        //窗口初始化
    Void FormDetail_Load(Object^  sender, EventArgs^  e) {
        ListViewItem^ item;
        item = gcnew ListViewItem();
        item->Text = "1";
        item->SubItems->Add("CrArk 版本");
        item->SubItems->Add("1.0");
        listViewProcDetail->Items->Add(item);
    }

        //tab选项改变时的响应
    Void tabControl_SelectedIndexChanged(Object^  sender, EventArgs^  e) {
         RefreshContent();
    }

        //刷新当前页面
    Void RefreshContent() {
        TabPage^ page;
        ListViewItem^ item;
        UInt32^ num;
        PProcessInfo procInfo;
        String^ str;
        PProcessNameInfo procNameInfo;
        char createTime[64];
        FILETIME fileTime;
        SYSTEMTIME sysTime;

        listViewProcDetail->Items->Clear();

        if(CurrentProcess == NULL)
            return;

        page = tabControl->SelectedTab;
        if(page == nullptr)
            return;

        if(page == tabPageProcessDetail) {
            procInfo = CrQueryProcessInfo(CurrentProcess);
            if(procInfo == NULL)
                return;
            //EPROCESS
            item = gcnew ListViewItem;
            item->Text = "1";
            item->SubItems->Add("EPROCESS");
            num = gcnew UInt32((DWORD)CurrentProcess);
            item->SubItems->Add("0x" + num->ToString("X8"));
            listViewProcDetail->Items->Add(item);

            procNameInfo = CrQueryProcessName(CurrentProcess);
            if(procNameInfo) {
                //映像名称
                item = gcnew ListViewItem;
                item->Text = "2";
                item->SubItems->Add("映像名称");
                item->SubItems->Add(gcnew String(procNameInfo->ImageName));
                listViewProcDetail->Items->Add(item);

                //路径
                item = gcnew ListViewItem;
                item->Text = "3";
                item->SubItems->Add("路径");
                item->SubItems->Add(gcnew String(procNameInfo->FullPath));
                listViewProcDetail->Items->Add(item);

                CrFreeMem(procNameInfo);
            }

            //启动时间
            fileTime.dwHighDateTime = procInfo->CreateTime.HighPart;
            fileTime.dwLowDateTime = procInfo->CreateTime.LowPart;
            FileTimeToSystemTime(&fileTime, &sysTime);
            wsprintfA(createTime, "%lu/%lu/%lu %lu:%lu:%lu.%lu", 
                    sysTime.wYear, sysTime.wMonth, sysTime.wDay,
                    sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
            item = gcnew ListViewItem;
            item->Text = "4";
            item->SubItems->Add("启动时间");
            item->SubItems->Add(gcnew String(createTime));
            listViewProcDetail->Items->Add(item);

            //PID
            item = gcnew ListViewItem;
            item->Text = "5";
            item->SubItems->Add("PID");
            item->SubItems->Add((gcnew UInt32(procInfo->UniqueProcessId))->ToString());
            listViewProcDetail->Items->Add(item);

            //父进程PID
            item = gcnew ListViewItem;
            item->Text = "6";
            item->SubItems->Add("父进程PID");
            item->SubItems->Add((gcnew UInt32(procInfo->InheritedFromUniqueProcessId))->ToString());
            listViewProcDetail->Items->Add(item);

            //基本优先级
            item = gcnew ListViewItem;
            item->Text = "7";
            item->SubItems->Add("基本优先级");
            item->SubItems->Add((gcnew UInt32(procInfo->BasePriority))->ToString());
            listViewProcDetail->Items->Add(item);

            //状态
            item = gcnew ListViewItem;
            item->Text = "8";
            item->SubItems->Add("状态");
            str = (gcnew UInt32(procInfo->State))->ToString();
            if(procInfo->State == 0)
                str = str + "(就绪)";
            else if(procInfo->State == 1)
                str = str + "(空闲)";
            item->SubItems->Add(str);
            listViewProcDetail->Items->Add(item);

            //PEB
            item = gcnew ListViewItem;
            item->Text = "9";
            item->SubItems->Add("PEB");
            item->SubItems->Add("0x" + (gcnew UInt32((DWORD)procInfo->Peb))->ToString("X8"));
            listViewProcDetail->Items->Add(item);

            //句柄计数
            item = gcnew ListViewItem;
            item->Text = "10";
            item->SubItems->Add("句柄计数");
            item->SubItems->Add((gcnew UInt32(procInfo->HandleCount))->ToString());
            listViewProcDetail->Items->Add(item);

            //调试端口
            item = gcnew ListViewItem;
            item->Text = "11";
            item->SubItems->Add("调试端口");
            item->SubItems->Add(procInfo->DebugPort ? (gcnew UInt32(procInfo->DebugPort))->ToString() : "NULL");
            listViewProcDetail->Items->Add(item);

            CrFreeMem(procInfo);
        }
        else if(page == tabPageThreadDetail) {

        }
        else if(page == tabPageModuleDetail) {

        }
    }
};
}
