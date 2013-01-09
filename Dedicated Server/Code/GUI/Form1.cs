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

namespace GUI
{
    public partial class Form1 : Form
    {
        const int MAX_PLAYERS = 32;
        CppCLI m_ServerEngine = null;

        public Form1()
        {
            Thread.CurrentThread.CurrentCulture = new CultureInfo("en-US");
            InitializeComponent();

            m_ServerEngine = new CppCLI();


        }

        public void ServerLoop()
        {
            while(this.Created)
            {
                Run();
                Application.DoEvents();
            }
        }

        private void Run()
        {
            if (this.m_ServerEngine.IsRunning())
            {
                //TODO, ret function. returns messages.
            }
            else
            {
                this.m_ServerEngine.ShutdownHost();
                this.button_Start.Enabled = true;
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
                    this.button_Start.Enabled = false;
            }

        }

        private void button_Start_EnabledChanged(object sender, EventArgs e)
        {
            if (button_Start.Enabled)
                button_Shutdown.Enabled = false;

            else if(!button_Start.Enabled)
                button_Shutdown.Enabled = true;
        }

        private void button_Shutdown_EnabledChanged(object sender, EventArgs e)
        {
            if (button_Shutdown.Enabled)
                button_Start.Enabled = false;

            else if (!button_Shutdown.Enabled)
                button_Start.Enabled = true;
        }

        private void button_Shutdown_Click(object sender, EventArgs e)
        {
            this.m_ServerEngine.ShutdownHost();
            this.button_Shutdown.Enabled = true;
        }

    }
}
