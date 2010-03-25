using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CrArkGUI
{
    public partial class ProcessForm : Form
    {
        public ProcessForm()
        {
            ListViewItem item;
            InitializeComponent();

            item = new ListViewItem("csrss.exe");
            item.SubItems.Add("123");
            item.SubItems.Add(@"c:\windows\csrss.exe");
            item.SubItems.Add("0x123456");
            listViewProcess.Items.Add(item);
        }
    }
}
