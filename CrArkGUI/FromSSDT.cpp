#include "StdAfx.h"
#include "FormSSDT.h"

PServiceTableInfo g_serviceTableInfo = NULL;

void CrArkGUI::FormSSDT::RefreshContent() {
    DWORD i;
    ListViewItem^ item;
    String^ str;

    if(g_serviceTableInfo != NULL)
        CrFreeMem(g_serviceTableInfo);

    g_serviceTableInfo = CrGetSSDTInfo();
    if(g_serviceTableInfo == NULL)
        return;

    for(i = 0; i < g_serviceTableInfo->Count; i++) {
        UInt32^ num = gcnew UInt32(i);
        item = gcnew ListViewItem("0x" + num->ToString("X"));

        item->Tag = gcnew UInt32((DWORD)g_serviceTableInfo->OriginAddress[i]);
        item->SubItems->Add("0");

        num = (DWORD)g_serviceTableInfo->OriginAddress[i];
        str = "0x" + num->ToString("X8");
        item->SubItems->Add(str);

        num = (DWORD)g_serviceTableInfo->CurrentAddress[i];
        str = "0x" + num->ToString("X8");
        item->SubItems->Add(str);

        item->SubItems->Add("xxxxxxxxx");
        
        if(g_serviceTableInfo->OriginAddress[i] == 
            g_serviceTableInfo->CurrentAddress[i]) {
            item->SubItems->Add("-");
        }
        else {
            item->SubItems->Add("Yes");
            item->ForeColor = System::Drawing::Color::Red;
        }
        listViewSSDT->Items->Add(item);
    }
}

Void CrArkGUI::FormSSDT::FormSSDT_FormClosed(Object^  sender, FormClosedEventArgs^  e) {
    if(g_serviceTableInfo) {
        CrFreeMem(g_serviceTableInfo);
        g_serviceTableInfo = NULL;
    }
}

Void CrArkGUI::FormSSDT::menuItemRestore_Click(Object^  sender, EventArgs^  e) {
    ListViewItem^ item;

    try {
        item = listViewSSDT->SelectedItems[0];
    }catch(Exception^) {

    }
}