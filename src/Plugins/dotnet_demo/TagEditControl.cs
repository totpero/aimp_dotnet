﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using AIMP.SDK.TagEditor;

namespace DemoPlugin
{
    public partial class TagEditControl : UserControl
    {
        public TagEditControl(IAimpFileTag tag)
        {
            InitializeComponent();
        }
    }
}