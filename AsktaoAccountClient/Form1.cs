using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AsktaoAccountClient
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            dataGridView1.SortCompare += DataGridView1_SortCompare;
        }

        private Dictionary<String, KeepALiveContent> _VecPlayerKeepALive = new Dictionary<string, KeepALiveContent>();
        private bool _IsRomor = false;

        Color LoseConnectColor = Color.Red;
        Color AnswerQuestionColor = Color.Red;

        [DllImport("AsktaoStarClientDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetAccountKeepALive(bool IsRomor);

        [DllImport("AsktaoStarClientDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetAccountKeepALiveContent(StringBuilder Text);

        [DllImport("AsktaoStarClientDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Initialize();

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool TerminateProcess(IntPtr hProcess, uint uExitCode);

        private void Form1_Load(object sender, EventArgs e)
        {
            try
            {
                _IsRomor = MessageBox.Show("点[是]进入谣言账号控制台, 点[否]进入找星账号控制台", "提示", MessageBoxButtons.YesNo) == DialogResult.Yes;
                dataGridView1.Columns.Cast<DataGridViewColumn>().ToList().ForEach(itm => itm.SortMode = DataGridViewColumnSortMode.NotSortable);
                dataGridView1.AllowUserToAddRows = false;
                dataGridView1.GetType().GetProperty("DoubleBuffered", System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.NonPublic)?.SetValue(dataGridView1, true, null);
                Task.Factory.StartNew(() =>
                {
                    try
                    {
                        if (Initialize() == 0)
                        {
                            MessageBox.Show("连接服务器失败!");
                        }

                        RefreshWorker();
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("async Form1_Load:" + ex.Message);
                        TerminateProcess(new IntPtr(-1), 0);
                    }
                });

                Task.Factory.StartNew(() =>
                {
                    try
                    {
                        //CheckNoResponeWorker();
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("CheckNoResponeWorker:" + ex.Message);
                        TerminateProcess(new IntPtr(-1), 0);
                    }
                });
            }
            catch (Exception ex)
            {
                MessageBox.Show("Form1_Load:" + ex.Message);
            }
           
        }

        private void CheckNoResponeWorker()
        {
            while (true)
            {
                Thread.Sleep(5000);
                lock (_VecPlayerKeepALive)
                {
                    foreach (var itm in _VecPlayerKeepALive)
                    {
                        if ((DateTime.Now - itm.Value.Tick).TotalSeconds >= 20 )
                        {
                            dataGridView1.Invoke(new Action(()=> 
                            {
                                for (var i = 0; i < dataGridView1.Rows.Count; i++)
                                {
                                    if (dataGridView1.Rows[i].Cells[0].Value.ToString() != itm.Key)
                                        continue;


                                    dataGridView1.Rows[i].Cells[2].Value = "掉线";
                                    break;
                                }
                            }));
                        }
                    }
                }
            }
        }


        private void RefreshWorker()
        {
            try
            {
                var Text = new StringBuilder(1024);
                while (true)
                {
                    Thread.Sleep(1000);
                    if (GetAccountKeepALive(_IsRomor) == 0)
                    {
                        MessageBox.Show("账号控制台与服务器失去连接!");
                        break;
                    }


                    for (var i = 0; i < 1000 && GetAccountKeepALiveContent(Text) != 0; ++i)
                    {
                        var Content = KeepALiveContent.Convert(Text.ToString());
                        if (Content.Status == String.Empty)
                            continue;


                        var bExist = false;
                        lock (_VecPlayerKeepALive)
                        {
                            bExist = _VecPlayerKeepALive.ContainsKey(Content.PlayerName);
                            if (!bExist)
                                _VecPlayerKeepALive.Add(Content.PlayerName, Content);
                            else
                                _VecPlayerKeepALive[Content.PlayerName] = Content;

                        }


                        dataGridView1.Invoke(new Action(() =>
                        {
                            if (!bExist)
                            {
                                dataGridView1.Rows.Add();
                                dataGridView1.Rows[dataGridView1.Rows.Count - 1].Cells[0].Value = Content.PlayerName;
                                dataGridView1.Rows[dataGridView1.Rows.Count - 1].Cells[1].Value = Content.ComputerName;
                                dataGridView1.Rows[dataGridView1.Rows.Count - 1].Cells[2].Value = Content.Status;
                            }
                            else
                            {
                                for (var j = 0; j < dataGridView1.Rows.Count; j++)
                                {
                                    if (dataGridView1.Rows[j].Cells[0].Value.ToString() == Content.PlayerName)
                                    {
                                        dataGridView1.Rows[j].Cells[2].Value = Content.Status != "正常" ? Content.Status : Content.Location;
                                        break;
                                    }
                                }
                            }

                            dataGridView1.Sort(dataGridView1.Columns[2], ListSortDirection.Descending);
                        }));
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("RefreshWorker:" + ex.Message);
            }
        }

        private int ConvertStatusToNumber(String Text)
        {
            switch (Text)
            {
                case "老君":
                    return 3;
                case "掉线":
                    return 2;
                case "正常":
                    return 1;
                default:
                    break;
            }
            return 0;
        }

        private void DataGridView1_SortCompare(object sender, DataGridViewSortCompareEventArgs e)
        {
            if (e.Column.Index == 2)
            {
                int Number1 = ConvertStatusToNumber(e.CellValue1.ToString());
                int Number2 = ConvertStatusToNumber(e.CellValue2.ToString());
                if (Number1 == Number2)
                    e.SortResult = String.Compare(dataGridView1.Rows[e.RowIndex1].Cells[e.Column.Index].Value.ToString(), dataGridView1.Rows[e.RowIndex2].Cells[e.Column.Index].Value.ToString());
                else
                    e.SortResult = Number1 > Number2 ? 1 : -1;

                e.Handled = true;
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            TerminateProcess(new IntPtr(-1), 0);
        }

        private void contextMenuStrip1_Opening(object sender, CancelEventArgs e)
        {

        }

        private void 删除ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (dataGridView1.SelectedRows.Count == 0)
                return;


            lock (_VecPlayerKeepALive)
            {
                _VecPlayerKeepALive.Remove(dataGridView1.SelectedRows[0].Cells[0].Value.ToString());
            }
            dataGridView1.Rows.RemoveAt(dataGridView1.SelectedRows[0].Index);
        }

        private void dataGridView1_MouseClick(object sender, MouseEventArgs e)
        {
           
        }

        private void dataGridView1_CellMouseDown(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right && e.RowIndex != -1)
            {
                dataGridView1.ClearSelection();
                dataGridView1.Rows[e.RowIndex].Selected = true;
            }
        }
    }


    public struct KeepALiveContent
    {
        public String PlayerName { get; set; }
        public String ComputerName { get; set; }
        public String Location { get; set; }
        public String Status { get; set; }
        public DateTime Tick { get; set; }

        public static KeepALiveContent Convert(String Text)
        {
            try
            {
                var Vec = Text.Split(new String[] { "," }, StringSplitOptions.RemoveEmptyEntries);
                return new KeepALiveContent() {Status = Vec[0], PlayerName = Vec[1], ComputerName = Vec[2], Location = Vec[3], Tick = DateTime.Now };
            }
            catch (Exception)
            {
                MessageBox.Show("无效的日志文本:" + Text);
            }
            return new KeepALiveContent() { Status = String.Empty };
        }
    }
}
