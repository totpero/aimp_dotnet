﻿using System;
using System.Windows.Forms;

namespace DemoPlugin
{
    public partial class LoggerForm : Form
    {
        public LoggerForm()
        {
            InitializeComponent();
            Logger.Instance.OnLogChanged += (sender, s) =>
            {
                textBox1.Text += $"{s}{Environment.NewLine}";
            };
        }

        public void WriteLog(string message)
        {
            textBox1.Text += $"{message}{Environment.NewLine}";
        }
    }
}