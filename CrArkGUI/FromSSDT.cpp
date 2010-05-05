#include "StdAfx.h"
#include "FormSSDT.h"
#include "..\CrArkKrnl\CrArkKrnl.h"

using namespace System::Windows::Forms;

PServiceTableInfo g_serviceTableInfo = NULL;
BOOL g_ShowHookOnly = FALSE;

void CrArkGUI::FormSSDT::RefreshContent(BOOL HookOnly) {
    DWORD i;
    ListViewItem^ item;
    String^ str;

    if(g_serviceTableInfo != NULL)
        CrFreeMem(g_serviceTableInfo);

    g_serviceTableInfo = CrGetSSDTInfo();
    if(g_serviceTableInfo == NULL)
        return;

    listViewSSDT->Items->Clear();

    for(i = 0; i < g_serviceTableInfo->Count; i++) {
        if(g_serviceTableInfo->OriginAddress[i] == 
            g_serviceTableInfo->CurrentAddress[i] &&
            HookOnly)
            continue;

        UInt32^ num = gcnew UInt32(i);
        InfoBuffer^ infoBuffer = gcnew(InfoBuffer);

        infoBuffer->OriginAddress = g_serviceTableInfo->OriginAddress[i];
        infoBuffer->CurrentAddress = g_serviceTableInfo->CurrentAddress[i];
        infoBuffer->ServiceNumber = i;

        item = gcnew ListViewItem("0x" + num->ToString("X"));
        item->Tag = infoBuffer;

        str = gcnew String(g_serviceTableInfo->FunctionName[i]);
        item->SubItems->Add(str);

        num = (DWORD)g_serviceTableInfo->ParamTable[i];
        str = "0x" + num->ToString("X");
        item->SubItems->Add(str);

        num = (DWORD)g_serviceTableInfo->OriginAddress[i];
        str = "0x" + num->ToString("X8");
        item->SubItems->Add(str);

        num = (DWORD)g_serviceTableInfo->CurrentAddress[i];
        str = "0x" + num->ToString("X8");
        item->SubItems->Add(str);

        str = gcnew String(g_serviceTableInfo->CurrentModuleName[i]);
        item->SubItems->Add(str);
        
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

Void CrArkGUI::FormSSDT::menuItemRefresh_Click(System::Object^  sender, System::EventArgs^  e) {
    RefreshContent(g_ShowHookOnly);
}

Void CrArkGUI::FormSSDT::menuItemRestore_Click(Object^  sender, EventArgs^  e) {
    ListViewItem^ item;
    InfoBuffer^ buf;

    try {
        item = listViewSSDT->SelectedItems[0];
        buf = (InfoBuffer^)item->Tag;
        if(buf->CurrentAddress == buf->OriginAddress)
            return;

        CrFixSsdt(buf->ServiceNumber, buf->OriginAddress);
        RefreshContent(g_ShowHookOnly);
    }catch(Exception^) {

    }
}

Void CrArkGUI::FormSSDT::menuItemHookOnly_Click(System::Object^  sender, System::EventArgs^  e) {
    ToolStripMenuItem^ item;

    item = (ToolStripMenuItem^)sender;
    g_ShowHookOnly = item->Checked;

    RefreshContent(g_ShowHookOnly);
}

Void CrArkGUI::FormSSDT::FormSSDT_Shown(System::Object^  sender, System::EventArgs^  e) {
    RefreshContent(g_ShowHookOnly);
}