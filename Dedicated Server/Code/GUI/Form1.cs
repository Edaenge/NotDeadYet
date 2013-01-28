using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Interop;
using System.Threading;
using System.Globalization;
using System.Net;
using System.IO;

namespace GUI
{
    public partial class Form1 : Form
    {
        const int MAX_PLAYERS = 32;
        const string RUNNING = "Running";
        const string NOT_RUNNING = "Not Running";
        String IPADD = "";

        int max_pl = 0;
        CppCLI m_ServerEngine = null;

        public Form1()
        {
            Thread.CurrentThread.CurrentCulture = new CultureInfo("en-US");
            InitializeComponent();

            m_ServerEngine = new CppCLI();


        }

        public void ServerLoop()
        {
            backgroundWorker1.RunWorkerAsync(null);

            while(this.Created)
            {
                Run();
                Application.DoEvents();

                Thread.Sleep(1);

            }
        }

        private void Run()
        {
            if (this.m_ServerEngine.IsRunning())
            {
                toolStripStatusLabel2.Text = Convert.ToString(m_ServerEngine.GetNrOfPlayers() + "/" + max_pl);
                toolStripStatusLabel4.Text = RUNNING;
            }
            else
            {
                this.m_ServerEngine.ShutdownHost();
                this.button_Start.Enabled = true;
                toolStripStatusLabel4.Text = NOT_RUNNING;

            }
        }

        private void button_Start_MouseClick(object sender, MouseEventArgs e)
        {
            int port;
            int players;
            bool isNr = false;

            String str_port = textBox_Port.Text.Trim();
            String str_players =textBox_Players.Text.Trim();

          
            isNr = int.TryParse(str_port, out port);

            if (!isNr)
            {
                MessageBox.Show("Invalid port: \"" + str_port+"\"");
                return;
            }

            isNr = int.TryParse(str_players, out players);

            if (!isNr)
            {
                MessageBox.Show("Invalid port: \"" + str_players+"\"");
                return;
            }

            if (players > MAX_PLAYERS || players <= 0)
            {
                MessageBox.Show("Invalid number of players: \"" + players + "\".\n"
                                +"Valid number of players are are: 1-"+MAX_PLAYERS+".");
                return;
            }

            String ret = m_ServerEngine.Init(port, players);
            if (ret != "")
            {
                MessageBox.Show("Error: "+ret);
            }
            else
            {
                if (!this.m_ServerEngine.StartHost())
                    MessageBox.Show("Error: Server was never initialized.");
                else
                {
                    this.button_Start.Enabled = false;
                    max_pl = players;
                    textBox_public_ip.Text = IPADD + ":" +textBox_Port.Text; 
                }
            }

        }

        private void button_Start_EnabledChanged(object sender, EventArgs e)
        {
            if (button_Start.Enabled)
            {
                button_Shutdown.Enabled = false;
                textBox_Players.Enabled = true;
                textBox_Port.Enabled = true;
            }
            else if (!button_Start.Enabled)
            {
                button_Shutdown.Enabled = true;
                textBox_Players.Enabled = false;
                textBox_Port.Enabled = false;
            }

            
        }

        private void button_Shutdown_EnabledChanged(object sender, EventArgs e)
        {
            if (button_Shutdown.Enabled)
            {
                button_Start.Enabled = false;
                textBox_Players.Enabled = true;
                textBox_Port.Enabled = true;
            }

            else if (!button_Shutdown.Enabled)
            {
                button_Start.Enabled = true;
                textBox_Players.Enabled = false;
                textBox_Port.Enabled = false;
            }
        }

        private void button_Shutdown_Click(object sender, EventArgs e)
        {
            this.m_ServerEngine.ShutdownHost();
            this.button_Shutdown.Enabled = true;
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {

            String direction = "";
            WebRequest request = WebRequest.Create("http://checkip.dyndns.org/");
            using (WebResponse response = request.GetResponse())
            using (StreamReader stream = new StreamReader(response.GetResponseStream()))
            {
                direction = stream.ReadToEnd();
            }

            //Search for the ip in the html
            int first = direction.IndexOf("Address: ") + 9;
            int last = direction.LastIndexOf("</body>");
            direction = direction.Substring(first, last - first);
            IPADD = direction;

        }

        private void button_copy_MouseClick(object sender, MouseEventArgs e)
        {
            Clipboard.SetText(textBox_public_ip.Text);
        }

    }
}
