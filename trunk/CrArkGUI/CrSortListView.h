#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

/*
namespace CrArkGUI{
    //可以按列排序的ListView
    ref class CrSortListView : public ListView {
    public:
        CrSortListView() : ListView(){
            this->Sorting = 
        }

    private:
        ref class CrSortListViewSorter : IComparer {
        private:
            CrSortListView^ Parent;
        public:
            CrSortListViewSorter(CrSortListViewSorter^ parent) {
                Parent = parent;
            }

            virtual int Compare(Object^ Object1, Object^ Object2) {
                ListViewItem ^item1, ^item2;

                item1 = (ListViewItem^)Object1;
                item2 = (ListViewItem^)Object2;

            }
        };
    };
}*/