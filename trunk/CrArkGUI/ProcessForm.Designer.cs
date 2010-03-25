namespace CrArkGUI
{
    partial class ProcessForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.listViewProcess = new System.Windows.Forms.ListView();
            this.columnHeaderImageName = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderPid = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderPath = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderEProcess = new System.Windows.Forms.ColumnHeader();
            this.SuspendLayout();
            // 
            // listViewProcess
            // 
            this.listViewProcess.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeaderImageName,
            this.columnHeaderPid,
            this.columnHeaderPath,
            this.columnHeaderEProcess});
            this.listViewProcess.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewProcess.Location = new System.Drawing.Point(0, 0);
            this.listViewProcess.Name = "listViewProcess";
            this.listViewProcess.Size = new System.Drawing.Size(475, 266);
            this.listViewProcess.TabIndex = 0;
            this.listViewProcess.UseCompatibleStateImageBehavior = false;
            this.listViewProcess.View = System.Windows.Forms.View.Details;
            // 
            // columnHeaderImageName
            // 
            this.columnHeaderImageName.Text = "映像名称";
            this.columnHeaderImageName.Width = 100;
            // 
            // columnHeaderPid
            // 
            this.columnHeaderPid.Text = "PID";
            this.columnHeaderPid.Width = 80;
            // 
            // columnHeaderPath
            // 
            this.columnHeaderPath.Text = "路径";
            this.columnHeaderPath.Width = 200;
            // 
            // columnHeaderEProcess
            // 
            this.columnHeaderEProcess.Text = "EPROCESS";
            this.columnHeaderEProcess.Width = 80;
            // 
            // ProcessForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(475, 266);
            this.Controls.Add(this.listViewProcess);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "ProcessForm";
            this.Text = "ProcessForm";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listViewProcess;
        private System.Windows.Forms.ColumnHeader columnHeaderImageName;
        private System.Windows.Forms.ColumnHeader columnHeaderPid;
        private System.Windows.Forms.ColumnHeader columnHeaderPath;
        private System.Windows.Forms.ColumnHeader columnHeaderEProcess;

    }
}