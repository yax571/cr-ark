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
            this.映像名称 = new System.Windows.Forms.ColumnHeader();
            this.PID = new System.Windows.Forms.ColumnHeader();
            this.路径 = new System.Windows.Forms.ColumnHeader();
            this.EPROCESS = new System.Windows.Forms.ColumnHeader();
            this.SuspendLayout();
            // 
            // listViewProcess
            // 
            this.listViewProcess.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.映像名称,
            this.PID,
            this.路径,
            this.EPROCESS});
            this.listViewProcess.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewProcess.Location = new System.Drawing.Point(0, 0);
            this.listViewProcess.Name = "listViewProcess";
            this.listViewProcess.Size = new System.Drawing.Size(292, 266);
            this.listViewProcess.TabIndex = 0;
            this.listViewProcess.UseCompatibleStateImageBehavior = false;
            // 
            // ProcessForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 266);
            this.Controls.Add(this.listViewProcess);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "ProcessForm";
            this.Text = "ProcessForm";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listViewProcess;
        private System.Windows.Forms.ColumnHeader 映像名称;
        private System.Windows.Forms.ColumnHeader PID;
        private System.Windows.Forms.ColumnHeader 路径;
        private System.Windows.Forms.ColumnHeader EPROCESS;

    }
}