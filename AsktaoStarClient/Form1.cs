using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Media;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AsktaoStarClient
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        [DllImport("AsktaoStarClientDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetStarInfo(StringBuilder Text);

        [DllImport("AsktaoStarClientDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Initialize();

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool TerminateProcess(IntPtr hProcess, uint uExitCode);

        [DllImport("AsktaoStarClientDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetRumor(StringBuilder Text);


        private void Form1_Load(object sender, EventArgs e)
        {
            this.Text = "连接服务器中...";
            if (!ConnectServer())
                return;


            Task.Factory.StartNew(new Action(() => { StarWorker(); }));
            Task.Factory.StartNew(new Action(() => { RumorWorker(); }));
        }


        private bool ConnectServer()
        {
            if (Initialize() == 0)
            {
                this.Invoke(new Action(() => { this.Text = "连接失败!"; }));
                return false;
            }
            this.Invoke(new Action(() => { this.Text = "连接成功!"; }));
            return true;
        }


        private int nRumorAfterSec = 30;
        private void RumorWorker()
        {
            StringBuilder Text = new StringBuilder(1024);
            while (true)
            {
                Thread.Sleep(1000);

                var RetCode = GetRumor(Text);
                if (RetCode == -1)
                {
                    MessageBox.Show("看星控制台与服务器失去连接!");
                    break;
                }
                else if (RetCode == 0)
                {
                    continue;
                }


                var Level = GetRumorTextLevel(Text.ToString());
                if (Level <= 0)
                {
                    MessageBox.Show("无效的谣言文本:[" + Text.ToString() + "]");
                    continue;
                }


                Task.Factory.StartNew(new Action(() =>
                {
                    var nTempLevel = Level;
                    var RumorText = Text.ToString();
                    //Thread.Sleep(nRumorAfterSec * 1000);
                    try
                    {
                        new SoundPlayer() { SoundLocation = @"qq.wav" }.Play();
                    }
                    catch (Exception)
                    {
                        MessageBox.Show("qq.wav文件呢?");
                        return;
                    }
                    
                    AppendTextToConsole(RumorText, Color.Green, rbtRomor);
                   /* if (nRumorAfterSec < 180)
                        Thread.Sleep((180 - 1 - nRumorAfterSec - 1) * 1000);


                    try
                    {
                        new SoundPlayer() { SoundLocation = @"qq1.wav" }.Play();
                    }
                    catch (Exception)
                    {
                        MessageBox.Show("qq1.wav文件呢?");
                        return;
                    }


                    Thread.Sleep(1000);
                    AppendTextToConsole(RumorText, Color.Green, rbtRefreshRomor);*/
                }));
            }
        }

        private static int GetRumorTextLevel(string Text)
        {
            var Index = Text.IndexOf("(", StringComparison.Ordinal);
            return Index == -1 ? 0 : int.Parse(Text.Substring(Index + 1, Text.IndexOf("级", StringComparison.Ordinal) - Index - 1));
        }

        private void StarWorker()
        {
            var StarText = new StringBuilder(1024);
            while (true)
            {
                Thread.Sleep(1000);
                var Level = GetStarInfo(StarText);
                if (Level == -1)
                {
                    MessageBox.Show("看星控制台与服务器失去连接!");
                    break;
                }
                else if (Level == 0)
                {
                    continue;
                }

  
                AppendTextToConsole(StarText.ToString(), Color.Green, rtbStar);
            }
        }

        private void AppendTextToConsole(String Text, Color Clr, RichTextBox rbt)
        {
            rbt.Invoke(new Action(() => { rbt.SelectionColor = Clr; rbt.AppendText(Text + "\r\n"); }));
        }

        private void ClearConsoleText(int Level)
        {
            if (Level <= 136)
            {
                rbtRomor.Invoke(new Action(() =>
                {
                    var Text = rbtRomor.Text.ToString();
                    rbtRomor.Text = Text.Substring(Text.IndexOf("\r\n", StringComparison.Ordinal) + 2);
                }));
            }
            else
            {
                rbtRefreshRomor.Invoke(new Action(() =>
                {
                    var Text = rbtRefreshRomor.Text.ToString();
                    rbtRefreshRomor.Text = Text.Substring(Text.IndexOf("\r\n", StringComparison.Ordinal) + 2);
                }));
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            TerminateProcess(new IntPtr(-1), 0);
        }

        private void 清空ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            (contextMenuStrip1.SourceControl as RichTextBox).Text = "";
        }

        private void btnModifyTick_Click(object sender, EventArgs e)
        {
           
        }

        private void btnModifyTick_Click_1(object sender, EventArgs e)
        {
            try
            {
                if (btnModifyTick.Text == "确定")
                {
                    nRumorAfterSec = int.Parse(txtRumorSec.Text);
                    txtRumorSec.Enabled = false;
                    btnModifyTick.Text = "修改";
                }
                else
                {
                    txtRumorSec.Enabled = true;
                    btnModifyTick.Text = "确定";
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("btnModifyTick_Click异常:" + ex.Message);
            }
        }
    }
}
