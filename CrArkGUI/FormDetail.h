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
    private: System::Windows::Forms::ListView^  listViewThreadDetail;
    private: System::Windows::Forms::ColumnHeader^  chThreadTid;
    private: System::Windows::Forms::ColumnHeader^  chThreadEThread;
    private: System::Windows::Forms::ColumnHeader^  chThreadSwitch;
    private: System::Windows::Forms::ColumnHeader^  chThreadPriority;
    private: System::Windows::Forms::ColumnHeader^  chThreadStart;
    private: System::Windows::Forms::ColumnHeader^  chThreadModName;
    private: System::Windows::Forms::ColumnHeader^  chThreadState;
    private: System::Windows::Forms::ListView^  listViewModDetail;
    private: System::Windows::Forms::ColumnHeader^  chModBase;
    private: System::Windows::Forms::ColumnHeader^  chModEntry;
    private: System::Windows::Forms::ColumnHeader^  chModSize;
    private: System::Windows::Forms::ColumnHeader^  chModName;
    private: System::Windows::Forms::ContextMenuStrip^  contextMenuThread;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemEndThread;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemForcEndThread;
    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemRefresh;
    private: System::Windows::Forms::ContextMenuStrip^  contextMenuMod;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemUnloadMod;
    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemRefresh2;
    private: System::ComponentModel::IContainer^  components;


    protected: 




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
            this->tabControl = (gcnew System::Windows::Forms::TabControl());
            this->tabPageProcessDetail = (gcnew System::Windows::Forms::TabPage());
            this->listViewProcDetail = (gcnew System::Windows::Forms::ListView());
            this->chProcDetailIndex = (gcnew System::Windows::Forms::ColumnHeader());
            this->chProcDetailName = (gcnew System::Windows::Forms::ColumnHeader());
            this->chProcDetailData = (gcnew System::Windows::Forms::ColumnHeader());
            this->tabPageThreadDetail = (gcnew System::Windows::Forms::TabPage());
            this->listViewThreadDetail = (gcnew System::Windows::Forms::ListView());
            this->chThreadTid = (gcnew System::Windows::Forms::ColumnHeader());
            this->chThreadEThread = (gcnew System::Windows::Forms::ColumnHeader());
            this->chThreadSwitch = (gcnew System::Windows::Forms::ColumnHeader());
            this->chThreadPriority = (gcnew System::Windows::Forms::ColumnHeader());
            this->chThreadStart = (gcnew System::Windows::Forms::ColumnHeader());
            this->chThreadModName = (gcnew System::Windows::Forms::ColumnHeader());
            this->chThreadState = (gcnew System::Windows::Forms::ColumnHeader());
            this->contextMenuThread = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->menuItemEndThread = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuItemForcEndThread = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->menuItemRefresh = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tabPageModuleDetail = (gcnew System::Windows::Forms::TabPage());
            this->listViewModDetail = (gcnew System::Windows::Forms::ListView());
            this->chModBase = (gcnew System::Windows::Forms::ColumnHeader());
            this->chModEntry = (gcnew System::Windows::Forms::ColumnHeader());
            this->chModSize = (gcnew System::Windows::Forms::ColumnHeader());
            this->chModName = (gcnew System::Windows::Forms::ColumnHeader());
            this->contextMenuMod = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->menuItemUnloadMod = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->menuItemRefresh2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tabControl->SuspendLayout();
            this->tabPageProcessDetail->SuspendLayout();
            this->tabPageThreadDetail->SuspendLayout();
            this->contextMenuThread->SuspendLayout();
            this->tabPageModuleDetail->SuspendLayout();
            this->contextMenuMod->SuspendLayout();
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
            this->tabControl->Size = System::Drawing::Size(603, 273);
            this->tabControl->TabIndex = 0;
            this->tabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &FormDetail::tabControl_SelectedIndexChanged);
            // 
            // tabPageProcessDetail
            // 
            this->tabPageProcessDetail->Controls->Add(this->listViewProcDetail);
            this->tabPageProcessDetail->Location = System::Drawing::Point(4, 21);
            this->tabPageProcessDetail->Name = L"tabPageProcessDetail";
            this->tabPageProcessDetail->Padding = System::Windows::Forms::Padding(3);
            this->tabPageProcessDetail->Size = System::Drawing::Size(595, 248);
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
            this->listViewProcDetail->Size = System::Drawing::Size(589, 242);
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
            this->tabPageThreadDetail->Controls->Add(this->listViewThreadDetail);
            this->tabPageThreadDetail->Location = System::Drawing::Point(4, 21);
            this->tabPageThreadDetail->Name = L"tabPageThreadDetail";
            this->tabPageThreadDetail->Padding = System::Windows::Forms::Padding(3);
            this->tabPageThreadDetail->Size = System::Drawing::Size(595, 248);
            this->tabPageThreadDetail->TabIndex = 1;
            this->tabPageThreadDetail->Text = L"线程信息";
            this->tabPageThreadDetail->UseVisualStyleBackColor = true;
            // 
            // listViewThreadDetail
            // 
            this->listViewThreadDetail->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(7) {this->chThreadTid, 
                this->chThreadEThread, this->chThreadSwitch, this->chThreadPriority, this->chThreadStart, this->chThreadModName, this->chThreadState});
            this->listViewThreadDetail->ContextMenuStrip = this->contextMenuThread;
            this->listViewThreadDetail->Dock = System::Windows::Forms::DockStyle::Fill;
            this->listViewThreadDetail->FullRowSelect = true;
            this->listViewThreadDetail->Location = System::Drawing::Point(3, 3);
            this->listViewThreadDetail->MultiSelect = false;
            this->listViewThreadDetail->Name = L"listViewThreadDetail";
            this->listViewThreadDetail->Size = System::Drawing::Size(589, 242);
            this->listViewThreadDetail->TabIndex = 0;
            this->listViewThreadDetail->UseCompatibleStateImageBehavior = false;
            this->listViewThreadDetail->View = System::Windows::Forms::View::Details;
            // 
            // chThreadTid
            // 
            this->chThreadTid->Text = L"TID";
            // 
            // chThreadEThread
            // 
            this->chThreadEThread->Text = L"ETHREAD";
            this->chThreadEThread->Width = 79;
            // 
            // chThreadSwitch
            // 
            this->chThreadSwitch->Text = L"切换数";
            // 
            // chThreadPriority
            // 
            this->chThreadPriority->Text = L"优先级";
            // 
            // chThreadStart
            // 
            this->chThreadStart->Text = L"起始地址";
            this->chThreadStart->Width = 80;
            // 
            // chThreadModName
            // 
            this->chThreadModName->Text = L"模块名称";
            this->chThreadModName->Width = 185;
            // 
            // chThreadState
            // 
            this->chThreadState->Text = L"状态";
            // 
            // contextMenuThread
            // 
            this->contextMenuThread->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->menuItemEndThread, 
                this->menuItemForcEndThread, this->toolStripSeparator1, this->menuItemRefresh2});
            this->contextMenuThread->Name = L"contextMenuThread";
            this->contextMenuThread->Size = System::Drawing::Size(161, 98);
            // 
            // menuItemEndThread
            // 
            this->menuItemEndThread->Name = L"menuItemEndThread";
            this->menuItemEndThread->Size = System::Drawing::Size(160, 22);
            this->menuItemEndThread->Text = L"结束线程(&T)";
            this->menuItemEndThread->Click += gcnew System::EventHandler(this, &FormDetail::menuItemEndThread_Click);
            // 
            // menuItemForcEndThread
            // 
            this->menuItemForcEndThread->Name = L"menuItemForcEndThread";
            this->menuItemForcEndThread->Size = System::Drawing::Size(160, 22);
            this->menuItemForcEndThread->Text = L"强制结束线程(&F)";
            this->menuItemForcEndThread->Click += gcnew System::EventHandler(this, &FormDetail::menuItemForcEndThread_Click);
            // 
            // toolStripSeparator1
            // 
            this->toolStripSeparator1->Name = L"toolStripSeparator1";
            this->toolStripSeparator1->Size = System::Drawing::Size(157, 6);
            // 
            // menuItemRefresh
            // 
            this->menuItemRefresh->Name = L"menuItemRefresh";
            this->menuItemRefresh->Size = System::Drawing::Size(160, 22);
            this->menuItemRefresh->Text = L"刷新(&R)";
            this->menuItemRefresh->Click += gcnew System::EventHandler(this, &FormDetail::menuItemRefresh_Click);
            // 
            // tabPageModuleDetail
            // 
            this->tabPageModuleDetail->Controls->Add(this->listViewModDetail);
            this->tabPageModuleDetail->Location = System::Drawing::Point(4, 21);
            this->tabPageModuleDetail->Name = L"tabPageModuleDetail";
            this->tabPageModuleDetail->Padding = System::Windows::Forms::Padding(3);
            this->tabPageModuleDetail->Size = System::Drawing::Size(595, 248);
            this->tabPageModuleDetail->TabIndex = 2;
            this->tabPageModuleDetail->Text = L"模块信息";
            this->tabPageModuleDetail->UseVisualStyleBackColor = true;
            // 
            // listViewModDetail
            // 
            this->listViewModDetail->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(4) {this->chModBase, 
                this->chModEntry, this->chModSize, this->chModName});
            this->listViewModDetail->ContextMenuStrip = this->contextMenuMod;
            this->listViewModDetail->Dock = System::Windows::Forms::DockStyle::Fill;
            this->listViewModDetail->FullRowSelect = true;
            this->listViewModDetail->Location = System::Drawing::Point(3, 3);
            this->listViewModDetail->MultiSelect = false;
            this->listViewModDetail->Name = L"listViewModDetail";
            this->listViewModDetail->Size = System::Drawing::Size(589, 242);
            this->listViewModDetail->TabIndex = 0;
            this->listViewModDetail->UseCompatibleStateImageBehavior = false;
            this->listViewModDetail->View = System::Windows::Forms::View::Details;
            // 
            // chModBase
            // 
            this->chModBase->Text = L"映像基址";
            this->chModBase->Width = 100;
            // 
            // chModEntry
            // 
            this->chModEntry->Text = L"入口点";
            this->chModEntry->Width = 100;
            // 
            // chModSize
            // 
            this->chModSize->Text = L"映像大小";
            this->chModSize->Width = 100;
            // 
            // chModName
            // 
            this->chModName->Text = L"模块名称";
            this->chModName->Width = 260;
            // 
            // contextMenuMod
            // 
            this->contextMenuMod->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->menuItemUnloadMod, 
                this->toolStripSeparator2, this->menuItemRefresh});
            this->contextMenuMod->Name = L"contextMenuMod";
            this->contextMenuMod->Size = System::Drawing::Size(161, 54);
            // 
            // menuItemUnloadMod
            // 
            this->menuItemUnloadMod->Name = L"menuItemUnloadMod";
            this->menuItemUnloadMod->Size = System::Drawing::Size(160, 22);
            this->menuItemUnloadMod->Text = L"卸载映像模块(&U)";
            this->menuItemUnloadMod->Click += gcnew System::EventHandler(this, &FormDetail::menuItemUnloadMod_Click);
            // 
            // toolStripSeparator2
            // 
            this->toolStripSeparator2->Name = L"toolStripSeparator2";
            this->toolStripSeparator2->Size = System::Drawing::Size(157, 6);
            // 
            // menuItemRefresh2
            // 
            this->menuItemRefresh2->Name = L"menuItemRefresh2";
            this->menuItemRefresh2->Size = System::Drawing::Size(160, 22);
            this->menuItemRefresh2->Text = L"刷新(&R)";
            this->menuItemRefresh2->Click += gcnew System::EventHandler(this, &FormDetail::menuItemRefresh2_Click);
            // 
            // FormDetail
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(603, 273);
            this->Controls->Add(this->tabControl);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            this->Name = L"FormDetail";
            this->Text = L"FormDetail";
            this->Load += gcnew System::EventHandler(this, &FormDetail::FormDetail_Load);
            this->tabControl->ResumeLayout(false);
            this->tabPageProcessDetail->ResumeLayout(false);
            this->tabPageThreadDetail->ResumeLayout(false);
            this->contextMenuThread->ResumeLayout(false);
            this->tabPageModuleDetail->ResumeLayout(false);
            this->contextMenuMod->ResumeLayout(false);
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

        //右键菜单 -> 刷新
    Void menuItemRefresh_Click(Object^  sender, EventArgs^  e) {
        RefreshContent();
    }

    Void menuItemRefresh2_Click(Object^  sender, EventArgs^  e) {
        RefreshContent();
    }

        //右键菜单 -> 结束线程
    Void menuItemEndThread_Click(Object^  sender, EventArgs^  e) {
        terminateThread(FALSE);
    }
        //右键菜单 -> 强制结束进程
    Void menuItemForcEndThread_Click(Object^  sender, EventArgs^  e) {
        terminateThread(TRUE);
    }

        //右键菜单 -> 卸载进程模块
    Void menuItemUnloadMod_Click(Object^  sender, EventArgs^  e) {
        IList ^itemList;
        ListViewItem ^item;
        PVOID baseAddress;

        itemList = listViewModDetail->SelectedItems;
        if(itemList == nullptr ||
           itemList->Count == 0)
           return;

        item = (ListViewItem^)itemList[0];
        baseAddress = ((ObjectBuffer^)item->Tag)->Object;
        CrUnmapProcessModule(CurrentProcess, baseAddress);

        RefreshContent();
    }
        //结束线程
    Void terminateThread(BOOLEAN ForceTerminate) {
        ListViewItem ^item;
        IList ^itemList;
        PVOID ethread;

        itemList = listViewThreadDetail->SelectedItems;
        if(itemList == nullptr ||
           itemList->Count == 0)
           return;

        item = (ListViewItem^)itemList[0];
        ethread = ((ObjectBuffer^)item->Tag)->Object;
        CrTerminateThread(ethread, 0, ForceTerminate);
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
        PThreadInfo threadInfo;
        PObjectIdTable threadTable;
        PModuleInfo modInfo;
        PProcessModuleList modList;
        char createTime[64];
        FILETIME fileTime;
        SYSTEMTIME sysTime;

        if(CurrentProcess == NULL)
            return;

        page = tabControl->SelectedTab;
        if(page == nullptr)
            return;

        if(page == tabPageProcessDetail) {
            listViewProcDetail->Items->Clear();
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
            listViewThreadDetail->Items->Clear();

            threadTable = CrThreadEnum(CurrentProcess);
            if(threadTable == NULL)
                return;
            
            for(DWORD i = 0; i < threadTable->Count; i++) {
                threadInfo = CrQueryThreadInfo(threadTable->Entry[i].Object);
                if(threadInfo == NULL)
                    continue;

                item = gcnew ListViewItem();
                item->Text = (gcnew UInt32(threadInfo->Tid))->ToString();
                item->SubItems->Add("0x" + (gcnew UInt32((DWORD)threadInfo->EThread))->ToString("X8"));
                item->SubItems->Add((gcnew UInt32(threadInfo->ContextSwitches))->ToString());
                item->SubItems->Add((gcnew UInt32(threadInfo->BasePriority))->ToString());
                item->SubItems->Add("0x" + (gcnew UInt32((DWORD)threadInfo->StartAddress))->ToString("X8"));
                item->SubItems->Add(gcnew String(threadInfo->ImagePath));
                item->Tag = gcnew ObjectBuffer(threadInfo->EThread);
                if(threadInfo->State == 0)
                    str = "Initialized";
                else if(threadInfo->State == 1)
                    str = "Ready";
                else if(threadInfo->State == 2)
                    str = "Running";
                else if(threadInfo->State == 3)
                    str = "Standby";
                else if(threadInfo->State == 4)
                    str = "Terminated";
                else if(threadInfo->State == 5)
                    str = "Waiting";
                else if(threadInfo->State == 6)
                    str = "Transition";
                else if(threadInfo->State == 7)
                    str = "DeferredReady";
                else if(threadInfo->State == 8)
                    str = "GateWait";
                item->SubItems->Add(str);
                listViewThreadDetail->Items->Add(item);

                CrFreeMem(threadInfo);
            }
            CrFreeMem(threadTable);
        }
        else if(page == tabPageModuleDetail) {
            listViewModDetail->Items->Clear();
            
            modList = CrQueryProcessModuleList(CurrentProcess);
            if(modList == NULL)
                return;

            for(DWORD i = 0; i < modList->Count; i++) {
                modInfo = CrQueryModuleInfo(modList->Process,
                                            modList->LdrDataTable[i]);
                if(modInfo == NULL)
                    continue;

                item = gcnew ListViewItem;
                item->Text = "0x" + (gcnew UInt32(modInfo->BaseAddress))->ToString("X8");
                item->SubItems->Add("0x" + (gcnew UInt32(modInfo->EntryPoint))->ToString("X8"));
                item->SubItems->Add("0x" + (gcnew UInt32(modInfo->SizeOfImage))->ToString("X8"));
                item->SubItems->Add(gcnew String(modInfo->FullPath));
                item->Tag = gcnew ObjectBuffer((PVOID)modInfo->BaseAddress);
                listViewModDetail->Items->Add(item);
                
                CrFreeMem(modInfo);
            }
            CrFreeMem(modList);            
        }
    }

    ref struct ObjectBuffer {
        ObjectBuffer(PVOID object) {
            Object = object;
        }
        PVOID Object;
    };
};
}
