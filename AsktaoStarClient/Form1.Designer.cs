namespace AsktaoStarClient
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
            this.components = new System.ComponentModel.Container();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.清空ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.rbtRomor = new System.Windows.Forms.RichTextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.rbtRefreshRomor = new System.Windows.Forms.RichTextBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.rtbStar = new System.Windows.Forms.RichTextBox();
            this.btnModifyTick = new System.Windows.Forms.Button();
            this.txtRumorSec = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.contextMenuStrip1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.清空ToolStripMenuItem});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(101, 26);
            // 
            // 清空ToolStripMenuItem
            // 
            this.清空ToolStripMenuItem.Name = "清空ToolStripMenuItem";
            this.清空ToolStripMenuItem.Size = new System.Drawing.Size(100, 22);
            this.清空ToolStripMenuItem.Text = "清空";
            this.清空ToolStripMenuItem.Click += new System.EventHandler(this.清空ToolStripMenuItem_Click);
            // 
            // rbtRomor
            // 
            this.rbtRomor.BackColor = System.Drawing.Color.Black;
            this.rbtRomor.ContextMenuStrip = this.contextMenuStrip1;
            this.rbtRomor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rbtRomor.ForeColor = System.Drawing.Color.Green;
            this.rbtRomor.Location = new System.Drawing.Point(3, 17);
            this.rbtRomor.Name = "rbtRomor";
            this.rbtRomor.Size = new System.Drawing.Size(554, 780);
            this.rbtRomor.TabIndex = 1;
            this.rbtRomor.Text = "";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.rbtRomor);
            this.groupBox1.Location = new System.Drawing.Point(4, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(560, 800);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "谣言控制台";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.rbtRefreshRomor);
            this.groupBox2.Location = new System.Drawing.Point(570, 0);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(560, 800);
            this.groupBox2.TabIndex = 3;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "谣言刷新控制台";
            // 
            // rbtRefreshRomor
            // 
            this.rbtRefreshRomor.BackColor = System.Drawing.Color.Black;
            this.rbtRefreshRomor.ContextMenuStrip = this.contextMenuStrip1;
            this.rbtRefreshRomor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rbtRefreshRomor.ForeColor = System.Drawing.Color.Green;
            this.rbtRefreshRomor.Location = new System.Drawing.Point(3, 17);
            this.rbtRefreshRomor.Name = "rbtRefreshRomor";
            this.rbtRefreshRomor.Size = new System.Drawing.Size(554, 780);
            this.rbtRefreshRomor.TabIndex = 1;
            this.rbtRefreshRomor.Text = "";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.rtbStar);
            this.groupBox3.Location = new System.Drawing.Point(1136, 0);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(560, 800);
            this.groupBox3.TabIndex = 3;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "看星控制台";
            // 
            // rtbStar
            // 
            this.rtbStar.BackColor = System.Drawing.Color.Black;
            this.rtbStar.ContextMenuStrip = this.contextMenuStrip1;
            this.rtbStar.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rtbStar.ForeColor = System.Drawing.Color.Green;
            this.rtbStar.Location = new System.Drawing.Point(3, 17);
            this.rtbStar.Name = "rtbStar";
            this.rtbStar.Size = new System.Drawing.Size(554, 780);
            this.rtbStar.TabIndex = 1;
            this.rtbStar.Text = "";
            // 
            // btnModifyTick
            // 
            this.btnModifyTick.Enabled = false;
            this.btnModifyTick.Location = new System.Drawing.Point(158, 798);
            this.btnModifyTick.Name = "btnModifyTick";
            this.btnModifyTick.Size = new System.Drawing.Size(51, 23);
            this.btnModifyTick.TabIndex = 9;
            this.btnModifyTick.Text = "确定";
            this.btnModifyTick.UseVisualStyleBackColor = true;
            this.btnModifyTick.Click += new System.EventHandler(this.btnModifyTick_Click_1);
            // 
            // txtRumorSec
            // 
            this.txtRumorSec.Enabled = false;
            this.txtRumorSec.Location = new System.Drawing.Point(86, 800);
            this.txtRumorSec.Name = "txtRumorSec";
            this.txtRumorSec.Size = new System.Drawing.Size(56, 21);
            this.txtRumorSec.TabIndex = 8;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 803);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 12);
            this.label1.TabIndex = 7;
            this.label1.Text = "谣言时间/秒:";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1708, 835);
            this.Controls.Add(this.btnModifyTick);
            this.Controls.Add(this.txtRumorSec);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "Form";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.contextMenuStrip1.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem 清空ToolStripMenuItem;
        private System.Windows.Forms.RichTextBox rbtRomor;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.RichTextBox rbtRefreshRomor;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.RichTextBox rtbStar;
        private System.Windows.Forms.Button btnModifyTick;
        private System.Windows.Forms.TextBox txtRumorSec;
        private System.Windows.Forms.Label label1;
    }
}

