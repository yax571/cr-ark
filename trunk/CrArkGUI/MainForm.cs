using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CrArkGUI
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            processForm = new ProcessForm();
            processForm.Dock = DockStyle.Fill;
            processForm.TopLevel = false;
            splitMain.Panel1.Controls.Add(processForm);
            processForm.Show();
        }

        //文件 -> 自动刷新
        private void menuItemAntoRefresh_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem item;

            item = sender as ToolStripMenuItem;
            autoRefresh = item.Checked;
        }

        //文件 -> 退出
        private void menuItemExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        //帮助 -> 关于
        private void menuItemAbout_Click(object sender, EventArgs e)
        {

        }

        private ProcessForm processForm;
        private Boolean autoRefresh;
    }
}
