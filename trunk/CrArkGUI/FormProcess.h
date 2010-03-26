#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Windows;


namespace CrArkGUI {
	/// <summary>
	/// Summary for FormProcess
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormProcess : public System::Windows::Forms::Form
	{
	public:
		FormProcess(void)
		{
			InitializeComponent();
            ShowDeleting = true;
            ListSortAscend = true;
            ListSortColumn = 0;

            //列表排序
            listViewProcess->ListViewItemSorter = gcnew ListProcessSorter(this);
            listViewProcess->Sorting = SortOrder::Ascending;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormProcess()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::ListView^  listViewProcess;
    private: System::Windows::Forms::ColumnHeader^  headerImageName;
    private: System::Windows::Forms::ColumnHeader^  headerPid;
    private: System::Windows::Forms::ColumnHeader^  headerPath;
    private: System::Windows::Forms::ColumnHeader^  headerEProcess;
    private: System::Windows::Forms::ContextMenuStrip^  contextMenu;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemCopy;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemRefresh;
    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemTerminateProcess;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemForceTerminateProcess;
    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemShowDeleting;

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
            this->listViewProcess = (gcnew System::Windows::Forms::ListView());
            this->headerImageName = (gcnew System::Windows::Forms::ColumnHeader());
            this->headerPid = (gcnew System::Windows::Forms::ColumnHeader());
            this->headerPath = (gcnew System::Windows::Forms::ColumnHeader());
            this->headerEProcess = (gcnew System::Windows::Forms::ColumnHeader());
            this->contextMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->menuItemCopy = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuItemRefresh = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->menuItemTerminateProcess = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuItemForceTerminateProcess = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->menuItemShowDeleting = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->contextMenu->SuspendLayout();
            this->SuspendLayout();
            // 
            // listViewProcess
            // 
            this->listViewProcess->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(4) {this->headerImageName, 
                this->headerPid, this->headerPath, this->headerEProcess});
            this->listViewProcess->ContextMenuStrip = this->contextMenu;
            this->listViewProcess->Dock = System::Windows::Forms::DockStyle::Fill;
            this->listViewProcess->FullRowSelect = true;
            this->listViewProcess->Location = System::Drawing::Point(0, 0);
            this->listViewProcess->Name = L"listViewProcess";
            this->listViewProcess->Size = System::Drawing::Size(508, 266);
            this->listViewProcess->TabIndex = 0;
            this->listViewProcess->UseCompatibleStateImageBehavior = false;
            this->listViewProcess->View = System::Windows::Forms::View::Details;
            this->listViewProcess->ColumnClick += gcnew System::Windows::Forms::ColumnClickEventHandler(this, &FormProcess::listViewProcess_ColumnClick);
            // 
            // headerImageName
            // 
            this->headerImageName->Text = L"映像名称";
            this->headerImageName->Width = 110;
            // 
            // headerPid
            // 
            this->headerPid->Text = L"PID";
            this->headerPid->Width = 75;
            // 
            // headerPath
            // 
            this->headerPath->Text = L"路径";
            this->headerPath->Width = 250;
            // 
            // headerEProcess
            // 
            this->headerEProcess->Text = L"EProcess";
            this->headerEProcess->Width = 75;
            // 
            // contextMenu
            // 
            this->contextMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->menuItemCopy, this->menuItemRefresh, 
                this->toolStripSeparator1, this->menuItemTerminateProcess, this->menuItemForceTerminateProcess, this->toolStripSeparator2, this->menuItemShowDeleting});
            this->contextMenu->Name = L"contextMenu";
            this->contextMenu->Size = System::Drawing::Size(227, 148);
            // 
            // menuItemCopy
            // 
            this->menuItemCopy->Name = L"menuItemCopy";
            this->menuItemCopy->Size = System::Drawing::Size(226, 22);
            this->menuItemCopy->Text = L"复制到剪贴板(&C)";
            this->menuItemCopy->Click += gcnew System::EventHandler(this, &FormProcess::menuItemCopy_Click);
            // 
            // menuItemRefresh
            // 
            this->menuItemRefresh->Name = L"menuItemRefresh";
            this->menuItemRefresh->Size = System::Drawing::Size(226, 22);
            this->menuItemRefresh->Text = L"刷新(&R)";
            this->menuItemRefresh->Click += gcnew System::EventHandler(this, &FormProcess::menuItemRefresh_Click);
            // 
            // toolStripSeparator1
            // 
            this->toolStripSeparator1->Name = L"toolStripSeparator1";
            this->toolStripSeparator1->Size = System::Drawing::Size(223, 6);
            // 
            // menuItemTerminateProcess
            // 
            this->menuItemTerminateProcess->Name = L"menuItemTerminateProcess";
            this->menuItemTerminateProcess->Size = System::Drawing::Size(226, 22);
            this->menuItemTerminateProcess->Text = L"结束进程(&T)";
            this->menuItemTerminateProcess->Click += gcnew System::EventHandler(this, &FormProcess::menuItemTerminateProcess_Click);
            // 
            // menuItemForceTerminateProcess
            // 
            this->menuItemForceTerminateProcess->Name = L"menuItemForceTerminateProcess";
            this->menuItemForceTerminateProcess->Size = System::Drawing::Size(226, 22);
            this->menuItemForceTerminateProcess->Text = L"强制结束进程(&F)";
            this->menuItemForceTerminateProcess->Click += gcnew System::EventHandler(this, &FormProcess::menuItemForceTerminateProcess_Click);
            // 
            // toolStripSeparator2
            // 
            this->toolStripSeparator2->Name = L"toolStripSeparator2";
            this->toolStripSeparator2->Size = System::Drawing::Size(223, 6);
            // 
            // menuItemShowDeleting
            // 
            this->menuItemShowDeleting->Checked = true;
            this->menuItemShowDeleting->CheckOnClick = true;
            this->menuItemShowDeleting->CheckState = System::Windows::Forms::CheckState::Checked;
            this->menuItemShowDeleting->Name = L"menuItemShowDeleting";
            this->menuItemShowDeleting->Size = System::Drawing::Size(226, 22);
            this->menuItemShowDeleting->Text = L"显示状态为 &Deleting 的进程";
            this->menuItemShowDeleting->Click += gcnew System::EventHandler(this, &FormProcess::menuItemShowDeleting_Click);
            // 
            // FormProcess
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(508, 266);
            this->Controls->Add(this->listViewProcess);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            this->Name = L"FormProcess";
            this->Text = L"FormProcess";
            this->Load += gcnew System::EventHandler(this, &FormProcess::FormProcess_Load);
            this->contextMenu->ResumeLayout(false);
            this->ResumeLayout(false);

        }
#pragma endregion
    public:
        Void ProcessRefresh()
        {
            PObjectIdTable objIdTable;
            PProcessNameInfo procNameInfo;
            ListViewItem ^item;
            
            objIdTable = CrProcessEnum(ShowDeleting ? TRUE : FALSE);
            if(objIdTable == NULL)
                return;
            listViewProcess->Items->Clear();

            for(int i = 0; i < objIdTable->Count; i++) {
                String ^procName;
                String ^procPath;
                String ^uniqueId;

                item = gcnew ListViewItem();
                item->Tag = gcnew ObjectBuffer(objIdTable->Entry[i].Object);

                procNameInfo = CrQueryProcessName(objIdTable->Entry[i].Object);
                if(procNameInfo) {
                    procName = gcnew String(procNameInfo->ImageName);
                    procPath = gcnew String(procNameInfo->FullPath);
                    item->Text = procName;
                    item->SubItems->Add((gcnew UInt32(objIdTable->Entry[i].UniqueId))->ToString());
                    item->SubItems->Add(procPath);
                    uniqueId = gcnew String("0x");
                    uniqueId += (gcnew UInt32((ULONG32)objIdTable->Entry[i].Object))->ToString("X8");
                    item->SubItems->Add(uniqueId);
                    CrFreeMem(procNameInfo);
                    listViewProcess->Items->Add(item);
                }
            }

            CrFreeMem(objIdTable);
        }
    private: 
        Void menuItemShowDeleting_Click(Object^  sender, EventArgs^  e) {
             ToolStripMenuItem^ item;

             item = (ToolStripMenuItem^)sender;
             ShowDeleting = item->Checked;
             ProcessRefresh();
        }

        //加载窗口时刷新列表
        Void FormProcess_Load(Object^  sender, EventArgs^  e) {
            ProcessRefresh();
        }

        //排序
        Void listViewProcess_ColumnClick(Object^  sender, ColumnClickEventArgs^  e) {
            if(ListSortColumn == e->Column) {
                ListSortAscend = !ListSortAscend;
            } else {
                ListSortAscend = true;
                ListSortColumn = e->Column;
            }
            listViewProcess->Sort();
        }

        //右键菜单 -> 刷新
        Void menuItemRefresh_Click(Object^  sender, EventArgs^  e) {
            ProcessRefresh();
        }

        //右键菜单 -> 结束进程
        Void menuItemTerminateProcess_Click(Object^  sender, EventArgs^  e) {
            terminateProcess(FALSE);
            ProcessRefresh();
        }

        //右键菜单 -> 强制结束进程
        Void menuItemForceTerminateProcess_Click(Object^  sender, EventArgs^  e) {
            if(Forms::MessageBox::Show("强制结束进程有危险, 要继续吗?",
                                        "提示",
                                        MessageBoxButtons::YesNo) == Forms::DialogResult::Yes)
            {
                terminateProcess(TRUE);
                ProcessRefresh();
            }
        }

        //右键菜单 -> 复制到剪切板
        Void menuItemCopy_Click(Object^  sender, EventArgs^  e) {
            String ^data;
            IList ^itemsList;
            ListViewItem ^item;

            itemsList = listViewProcess->SelectedItems;
            if(itemsList == nullptr)
                return;

            data = "";
            for(int i = 0; i < itemsList->Count; i++) {
                item = (ListViewItem^)itemsList[i];
                data += "进程映像: " + item->SubItems[0]->Text + "\r\n" +
                        "PID: " + item->SubItems[1]->Text + "\r\n" +
                        "路径: " + item->SubItems[2]->Text + "\r\n" +
                        "EPROCESS: " + item->SubItems[3]->Text + "\r\n\r\n";
            }

            Clipboard::SetText(data);
            Forms::MessageBox::Show("信息已经被拷贝到剪切板.", "提示", MessageBoxButtons::OK);
        }

        Void terminateProcess(BOOLEAN ForceTerminate) {
            IList^ itemsList;
            ListViewItem^ item;
            ObjectBuffer^ objBuffer;

            itemsList = listViewProcess->SelectedItems;
            if(itemsList == nullptr)
                return;

            for(int i = 0; i < itemsList->Count; i++) {
                item = (ListViewItem^)itemsList[i];
                objBuffer = (ObjectBuffer^)item->Tag;
                CrTerminateProcess(objBuffer->Object,
                    0,
                    ForceTerminate);
            }
        }

    private:
        Boolean ShowDeleting;
        Boolean ListSortAscend;
        Int32 ListSortColumn;

    private:
        ref struct ObjectBuffer{
            ObjectBuffer(PVOID obj) {
               Object = obj;
            }
            PVOID Object;
        };

        ref class ListProcessSorter : public IComparer {
        private:
            FormProcess ^parent;

        public:
            ListProcessSorter(FormProcess^ Parent) {
                parent = Parent;
            }

            virtual int Compare(Object^ Object1, Object^ Object2) {
                ListViewItem ^item1, ^item2;
                Int32 num1, num2;

                item1 = (ListViewItem^)Object1;
                item2 = (ListViewItem^)Object2;

                switch(parent->ListSortColumn) {
                    case 0:         //映像名称排序
                        if(parent->ListSortAscend)
                            return String::Compare(item1->Text, item2->Text, true);
                        else
                            return String::Compare(item2->Text, item1->Text, true);
                        break;

                    case 1:         //PID
                        num1 = Int32::Parse(item1->SubItems[1]->Text);
                        num2 = Int32::Parse(item2->SubItems[1]->Text);
                        if(parent->ListSortAscend)
                            return num1 - num2;
                        else
                            return num2 - num1;
                        break;

                    case 2:         //路径
                        if(parent->ListSortAscend)
                            return String::Compare(item1->SubItems[2]->Text, item2->SubItems[2]->Text, true);
                        else
                            return String::Compare(item2->SubItems[2]->Text, item1->SubItems[2]->Text, true);
                        break;

                    case 3:         //EPROCESS
                        if(parent->ListSortAscend)
                            return String::Compare(item1->SubItems[3]->Text, item2->SubItems[3]->Text, true);
                        else
                            return String::Compare(item2->SubItems[3]->Text, item1->SubItems[3]->Text, true);
                        break;
                }
                return 1;
            }
        };
};
}
