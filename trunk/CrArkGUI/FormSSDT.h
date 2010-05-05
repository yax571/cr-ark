#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CrArkGUI {

	/// <summary>
	/// Summary for FromSSDT
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormSSDT : public System::Windows::Forms::Form
	{
	public:
		FormSSDT(void)
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
		~FormSSDT()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::ListView^  listViewSSDT;
    private: System::Windows::Forms::ColumnHeader^  headerIndex;
    private: System::Windows::Forms::ColumnHeader^  headerParam;
    private: System::Windows::Forms::ColumnHeader^  headerOriginAddress;
    private: System::Windows::Forms::ColumnHeader^  headerCurrentAddress;
    private: System::Windows::Forms::ColumnHeader^  headerModule;
    private: System::Windows::Forms::ColumnHeader^  headerHooked;
    private: System::Windows::Forms::ContextMenuStrip^  menuContext;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemRestore;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemRefresh;
    private: System::Windows::Forms::ColumnHeader^  headerFuncName;
    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
    private: System::Windows::Forms::ToolStripMenuItem^  menuItemHookOnly;

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
            this->listViewSSDT = (gcnew System::Windows::Forms::ListView());
            this->headerIndex = (gcnew System::Windows::Forms::ColumnHeader());
            this->headerParam = (gcnew System::Windows::Forms::ColumnHeader());
            this->headerOriginAddress = (gcnew System::Windows::Forms::ColumnHeader());
            this->headerCurrentAddress = (gcnew System::Windows::Forms::ColumnHeader());
            this->headerModule = (gcnew System::Windows::Forms::ColumnHeader());
            this->headerHooked = (gcnew System::Windows::Forms::ColumnHeader());
            this->menuContext = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->menuItemRestore = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuItemRefresh = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->headerFuncName = (gcnew System::Windows::Forms::ColumnHeader());
            this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->menuItemHookOnly = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuContext->SuspendLayout();
            this->SuspendLayout();
            // 
            // listViewSSDT
            // 
            this->listViewSSDT->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(7) {this->headerIndex, this->headerFuncName, 
                this->headerParam, this->headerOriginAddress, this->headerCurrentAddress, this->headerModule, this->headerHooked});
            this->listViewSSDT->ContextMenuStrip = this->menuContext;
            this->listViewSSDT->Dock = System::Windows::Forms::DockStyle::Fill;
            this->listViewSSDT->FullRowSelect = true;
            this->listViewSSDT->Location = System::Drawing::Point(0, 0);
            this->listViewSSDT->Name = L"listViewSSDT";
            this->listViewSSDT->ShowItemToolTips = true;
            this->listViewSSDT->Size = System::Drawing::Size(786, 442);
            this->listViewSSDT->TabIndex = 0;
            this->listViewSSDT->UseCompatibleStateImageBehavior = false;
            this->listViewSSDT->View = System::Windows::Forms::View::Details;
            // 
            // headerIndex
            // 
            this->headerIndex->Text = L"���";
            // 
            // headerParam
            // 
            this->headerParam->Text = L"����";
            // 
            // headerOriginAddress
            // 
            this->headerOriginAddress->Text = L"ԭʼ��ַ";
            this->headerOriginAddress->Width = 120;
            // 
            // headerCurrentAddress
            // 
            this->headerCurrentAddress->Text = L"��ǰ��ַ";
            this->headerCurrentAddress->Width = 120;
            // 
            // headerModule
            // 
            this->headerModule->Text = L"ģ��";
            this->headerModule->Width = 250;
            // 
            // headerHooked
            // 
            this->headerHooked->Text = L"�ҹ�";
            this->headerHooked->Width = 40;
            // 
            // menuContext
            // 
            this->menuContext->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->menuItemRestore, 
                this->menuItemRefresh, this->toolStripSeparator1, this->menuItemHookOnly});
            this->menuContext->Name = L"menuContext";
            this->menuContext->Size = System::Drawing::Size(179, 98);
            // 
            // menuItemRestore
            // 
            this->menuItemRestore->Name = L"menuItemRestore";
            this->menuItemRestore->Size = System::Drawing::Size(178, 22);
            this->menuItemRestore->Text = L"��ԭѡ����";
            this->menuItemRestore->Click += gcnew System::EventHandler(this, &FormSSDT::menuItemRestore_Click);
            // 
            // menuItemRefresh
            // 
            this->menuItemRefresh->Name = L"menuItemRefresh";
            this->menuItemRefresh->Size = System::Drawing::Size(178, 22);
            this->menuItemRefresh->Text = L"ˢ��(&R)";
            this->menuItemRefresh->Click += gcnew System::EventHandler(this, &FormSSDT::menuItemRefresh_Click);
            // 
            // headerFuncName
            // 
            this->headerFuncName->Text = L"��������";
            this->headerFuncName->Width = 120;
            // 
            // toolStripSeparator1
            // 
            this->toolStripSeparator1->Name = L"toolStripSeparator1";
            this->toolStripSeparator1->Size = System::Drawing::Size(175, 6);
            // 
            // menuItemHookOnly
            // 
            this->menuItemHookOnly->CheckOnClick = true;
            this->menuItemHookOnly->Name = L"menuItemHookOnly";
            this->menuItemHookOnly->Size = System::Drawing::Size(178, 22);
            this->menuItemHookOnly->Text = L"ֻ��ʾ���ҹ��ĺ���";
            this->menuItemHookOnly->Click += gcnew System::EventHandler(this, &FormSSDT::menuItemHookOnly_Click);
            // 
            // FormSSDT
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(786, 442);
            this->Controls->Add(this->listViewSSDT);
            this->Name = L"FormSSDT";
            this->Text = L"SSDT�鿴";
            this->Shown += gcnew System::EventHandler(this, &FormSSDT::FormSSDT_Shown);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FormSSDT::FormSSDT_FormClosed);
            this->menuContext->ResumeLayout(false);
            this->ResumeLayout(false);

        }
#pragma endregion
        public:
            void RefreshContent(BOOL HookOnly);
            Void FormSSDT_FormClosed(Object^  sender, FormClosedEventArgs^  e);
            Void menuItemRestore_Click(Object^  sender, EventArgs^  e);
            Void menuItemRefresh_Click(System::Object^  sender, System::EventArgs^  e);
            Void menuItemHookOnly_Click(System::Object^  sender, System::EventArgs^  e);
            Void FormSSDT_Shown(System::Object^  sender, System::EventArgs^  e);

            ref struct InfoBuffer {
                DWORD ServiceNumber;
                PVOID OriginAddress;
                PVOID CurrentAddress;
            };
};
}
