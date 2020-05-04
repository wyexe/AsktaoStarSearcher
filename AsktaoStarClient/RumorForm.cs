using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AsktaoStarClient
{
    public partial class RumorForm : Form
    {
        public RumorForm()
        {
            InitializeComponent();
        }

        [DllImport("AsktaoStarClientDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetRumor(StringBuilder Text);

        [DllImport("AsktaoStarClientDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Initialize();

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool TerminateProcess(IntPtr hProcess, uint uExitCode);

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

        private void RumorForm_Load(object sender, EventArgs e)
        {
            this.Text = "连接服务器中...";
            Task.Factory.StartNew(new Action(() =>
            {
                if (!ConnectServer())
                    return;

                StringBuilder Text = new StringBuilder(1024);
                while (true)
                {
                    Thread.Sleep(1000);

                    int RetCode = GetRumor(Text);
                    if (RetCode == -1)
                    {
                        MessageBox.Show("看星控制台与服务器失去连接!");
                        break;
                    }
                    else if (RetCode == 0)
                    {
                        continue;
                    }


                    richTextBox1.Invoke(new Action(() => { richTextBox1.AppendText(Text.ToString() + "\r\n"); }));
                }
            }));
        }
    }
}
