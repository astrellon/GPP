namespace FolderAnalyser
{
    partial class Form1
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
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lstResults = new System.Windows.Forms.ListBox();
            this.txtInput = new System.Windows.Forms.TextBox();
            this.btnSearch = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.txtNumResults = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.trcPages = new System.Windows.Forms.TrackBar();
            this.btnUpdateSearch = new System.Windows.Forms.Button();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trcPages)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(773, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(45, 20);
            this.loadToolStripMenuItem.Text = "Load";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.loadToolStripMenuItem_Click);
            // 
            // lstResults
            // 
            this.lstResults.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lstResults.FormattingEnabled = true;
            this.lstResults.Location = new System.Drawing.Point(12, 79);
            this.lstResults.Name = "lstResults";
            this.lstResults.Size = new System.Drawing.Size(749, 316);
            this.lstResults.TabIndex = 1;
            // 
            // txtInput
            // 
            this.txtInput.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtInput.Location = new System.Drawing.Point(12, 27);
            this.txtInput.Name = "txtInput";
            this.txtInput.Size = new System.Drawing.Size(668, 20);
            this.txtInput.TabIndex = 2;
            this.txtInput.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtInput_KeyPress);
            // 
            // btnSearch
            // 
            this.btnSearch.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnSearch.Location = new System.Drawing.Point(686, 27);
            this.btnSearch.Name = "btnSearch";
            this.btnSearch.Size = new System.Drawing.Size(75, 23);
            this.btnSearch.TabIndex = 3;
            this.btnSearch.Text = "Search";
            this.btnSearch.UseVisualStyleBackColor = true;
            this.btnSearch.Click += new System.EventHandler(this.btnSearch_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 54);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(97, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Number of Results:";
            // 
            // txtNumResults
            // 
            this.txtNumResults.Location = new System.Drawing.Point(116, 51);
            this.txtNumResults.Name = "txtNumResults";
            this.txtNumResults.Size = new System.Drawing.Size(100, 20);
            this.txtNumResults.TabIndex = 5;
            this.txtNumResults.Text = "100";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(223, 53);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(75, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "Page Number:";
            // 
            // trcPages
            // 
            this.trcPages.Location = new System.Drawing.Point(304, 51);
            this.trcPages.Name = "trcPages";
            this.trcPages.Size = new System.Drawing.Size(170, 45);
            this.trcPages.TabIndex = 7;
            // 
            // btnUpdateSearch
            // 
            this.btnUpdateSearch.Location = new System.Drawing.Point(480, 51);
            this.btnUpdateSearch.Name = "btnUpdateSearch";
            this.btnUpdateSearch.Size = new System.Drawing.Size(75, 23);
            this.btnUpdateSearch.TabIndex = 8;
            this.btnUpdateSearch.Text = "Update";
            this.btnUpdateSearch.UseVisualStyleBackColor = true;
            this.btnUpdateSearch.Click += new System.EventHandler(this.btnUpdateSearch_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(773, 409);
            this.Controls.Add(this.btnUpdateSearch);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.txtNumResults);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnSearch);
            this.Controls.Add(this.txtInput);
            this.Controls.Add(this.lstResults);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.trcPages);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Folder Analyser";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trcPages)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.ListBox lstResults;
        private System.Windows.Forms.TextBox txtInput;
        private System.Windows.Forms.Button btnSearch;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtNumResults;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TrackBar trcPages;
        private System.Windows.Forms.Button btnUpdateSearch;
    }
}

