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

    private: System::ComponentModel::IContainer^  components;
    protected: 

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
            this->listViewSSDT = (gcnew System::Windows::Forms::ListView());
            this->headerIndex = (gcnew System::Windows::Forms::ColumnHeader());
            this->headerParam = (gcnew System::Windows::Forms::ColumnHeader());
            this->headerOriginAddress = (gcnew System::Windows::Forms::ColumnHeader());
            this->headerCurrentAddress = (gcnew System::Windows::Forms::ColumnHeader());
            this->headerModule = (gcnew System::Windows::Forms::ColumnHeader());
            this->headerHooked = (gcnew System::Windows::Forms::ColumnHeader());
            this->menuContext = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->menuItemRestore = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuContext->SuspendLayout();
            this->SuspendLayout();
            // 
            // listViewSSDT
            // 
            this->listViewSSDT->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(6) {this->headerIndex, this->headerParam, 
                this->headerOriginAddress, this->headerCurrentAddress, this->headerModule, this->headerHooked});
            this->listViewSSDT->ContextMenuStrip = this->menuContext;
            this->listViewSSDT->Dock = System::Windows::Forms::DockStyle::Fill;
            this->listViewSSDT->FullRowSelect = true;
            this->listViewSSDT->Location = System::Drawing::Point(0, 0);
            this->listViewSSDT->Name = L"listViewSSDT";
            this->listViewSSDT->ShowItemToolTips = true;
            this->listViewSSDT->Size = System::Drawing::Size(724, 442);
            this->listViewSSDT->TabIndex = 0;
            this->listViewSSDT->UseCompatibleStateImageBehavior = false;
            this->listViewSSDT->View = System::Windows::Forms::View::Details;
            // 
            // headerIndex
            // 
            this->headerIndex->Text = L"序号";
            // 
            // headerParam
            // 
            this->headerParam->Text = L"参数";
            // 
            // headerOriginAddress
            // 
            this->headerOriginAddress->Text = L"原始地址";
            this->headerOriginAddress->Width = 120;
            // 
            // headerCurrentAddress
            // 
            this->headerCurrentAddress->Text = L"当前地址";
            this->headerCurrentAddress->Width = 120;
            // 
            // headerModule
            // 
            this->headerModule->Text = L"模块";
            this->headerModule->Width = 300;
            // 
            // headerHooked
            // 
            this->headerHooked->Text = L"挂钩";
            this->headerHooked->Width = 40;
            // 
            // menuContext
            // 
            this->menuContext->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->menuItemRestore});
            this->menuContext->Name = L"menuContext";
            this->menuContext->Size = System::Drawing::Size(153, 48);
            // 
            // menuItemRestore
            // 
            this->menuItemRestore->Name = L"menuItemRestore";
            this->menuItemRestore->Size = System::Drawing::Size(152, 22);
            this->menuItemRestore->Text = L"还原选中项";
            this->menuItemRestore->Click += gcnew System::EventHandler(this, &FormSSDT::menuItemRestore_Click);
            // 
            // FormSSDT
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(724, 442);
            this->Controls->Add(this->listViewSSDT);
            this->Name = L"FormSSDT";
            this->Text = L"FormSSDT";
            this->Shown += gcnew System::EventHandler(this, &FormSSDT::FormSSDT_Shown);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FormSSDT::FormSSDT_FormClosed);
            this->menuContext->ResumeLayout(false);
            this->ResumeLayout(false);

        }

        Void FormSSDT_Shown(System::Object^  sender, System::EventArgs^  e) {
            RefreshContent();
        }
#pragma endregion
        public:
            void RefreshContent();
            Void FormSSDT_FormClosed(Object^  sender, FormClosedEventArgs^  e);
            Void menuItemRestore_Click(Object^  sender, EventArgs^  e);
};
}
