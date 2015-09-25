﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Windows.Markup;

namespace ModelViewer
{
    public partial class ModelViewerWindow : Form
    {
        private List<FileInfo> myEffectFiles = null;
        private FileInfo myCurrentModelFile = null;

        private string myCurrentEffectFilePath = Properties.Settings.Default.DefaultEffectFilePath;
        private string myCurrentModelFilePath = Properties.Settings.Default.DefaultModelFilePath;

        private string myEffectFolderPath = Properties.Settings.Default.DefaultEffectFolderDirectory;
        private string myModelFolderPath = Properties.Settings.Default.DefaultModelFolderDirectory;

        private Point myPreviousMousePosition;

        public ModelViewerWindow()
        {
            InitializeComponent();

            if (myModelFolderPath == "")
            {
                Properties.Settings.Default.DefaultModelFolderDirectory = Directory.GetCurrentDirectory();
                myModelFolderPath = Properties.Settings.Default.DefaultModelFolderDirectory;
                Properties.Settings.Default.Save();
            }
            if (myEffectFolderPath == "")
            {
                Properties.Settings.Default.DefaultEffectFolderDirectory = Directory.GetCurrentDirectory();
                myEffectFolderPath = Properties.Settings.Default.DefaultEffectFolderDirectory;
                Properties.Settings.Default.Save();
            }

            modelFileBrowser.InitialDirectory = myModelFolderPath;
            effectFolderBrowser.SelectedPath = myEffectFolderPath;

            myPreviousMousePosition = MousePosition;

            LoadEngine();
            FillEffectList();

            UpdateTimer.Start();
        }

        private void LoadEngine()
        {
            IntPtr windowHandle = ModelViewer.Handle;

            ModelViewer.Invalidate();

            Int32 width = ModelViewer.Width;
            Int32 height = ModelViewer.Height;

            NativeMethods.SetupWindow(width, height);
            NativeMethods.StartEngine(windowHandle);
            NativeMethods.Render();
        }

        private void Btn_OpenModel_Click(object sender, EventArgs e)
        {
            modelFileBrowser.ShowDialog();
            myCurrentModelFilePath = modelFileBrowser.FileName;

            myModelFolderPath = myCurrentModelFilePath.Replace(modelFileBrowser.SafeFileName, "");
            Properties.Settings.Default.DefaultModelFolderDirectory = myModelFolderPath;
            Properties.Settings.Default.Save();

            modelFileBrowser.InitialDirectory = myModelFolderPath;
        }

        private void Btn_OpenEffectFolder_Click(object sender, EventArgs e)
        {
            effectFolderBrowser.SelectedPath = myEffectFolderPath;
            effectFolderBrowser.ShowDialog();

            myEffectFolderPath = effectFolderBrowser.SelectedPath;

            if (myEffectFolderPath != "")
            {
                EffectFilter.Items.Clear();
                FillEffectList();
            }

            Properties.Settings.Default.DefaultEffectFolderDirectory = myEffectFolderPath;
            Properties.Settings.Default.Save();
        }

        private void FillEffectList()
        {
            DirectoryInfo currentDirectory = new DirectoryInfo(myEffectFolderPath);

            FileInfo[] filesInDirectory = currentDirectory.GetFiles();

            myEffectFiles = SortOutEffectFiles(filesInDirectory);

            for (int i = 0; i < myEffectFiles.Count; ++i)
            {
                EffectFilter.Items.Add(myEffectFiles[i].Name);
            }
        }

        private List<FileInfo> SortOutEffectFiles(FileInfo[] someFiles)
        {
            List<FileInfo> effectFile = new List<FileInfo>();

            for (int i = 0; i < someFiles.Length; ++i)
            {
                if (someFiles[i].Extension == ".fx")
                {
                    effectFile.Add(someFiles[i]);
                }
            }
            return effectFile;
        }

        private void ModelViewer_Paint(object sender, PaintEventArgs e)
        {
            NativeMethods.Update();
            NativeMethods.Render();
        }

        private void EffectFilter_SelectedIndexChanged(object sender, EventArgs e)
        {
            for (int i = 0; i < myEffectFiles.Count; ++i)
            {
                if (myEffectFiles[i].Name == EffectFilter.SelectedItem)
                {
                    myCurrentEffectFilePath = myEffectFiles[i].FullName;
                    //NativeMethods.SetEffect(myCurrentEffectFilePath);
                }
            }
        }

        private void UpdateTimer_Tick(object sender, EventArgs e)
        {
            ModelViewer.Invalidate();
        }

        private void Btn_LoadModel_Click(object sender, EventArgs e)
        {
            if (myCurrentModelFilePath == "")
            {
                MessageBox.Show("Error: No model file is selected");
                return;
            }
            if (myCurrentEffectFilePath == "")
            {
                MessageBox.Show("Error: No effect file is selected");
                return;
            }
            if (myCurrentModelFilePath != "" && myCurrentEffectFilePath != "")
            {
                if (Path.GetExtension(myCurrentModelFilePath) == ".fbx")
                {
                    NativeMethods.LoadModel(myCurrentModelFilePath, myCurrentEffectFilePath);
                }
                else
                {
                    MessageBox.Show("Error: " + Path.GetExtension(myCurrentModelFilePath) + " is not compatible. \nTry using a .fbx file instead.");
                }
            }
        }

        private void Btn_BackgroundColor_Click(object sender, EventArgs e)
        {
            BackgroundColorDialog.ShowDialog();
            Btn_BackgroundColor.BackColor = BackgroundColorDialog.Color;

            float redChannel =   (BackgroundColorDialog.Color.R)/ 255.0f;
            float greenChannel = (BackgroundColorDialog.Color.G)/ 255.0f;
            float blueChannel =  (BackgroundColorDialog.Color.B)/ 255.0f;
            float alphaChannel = (BackgroundColorDialog.Color.A)/ 255.0f;

            NativeMethods.SetClearColor(redChannel, greenChannel, blueChannel, alphaChannel);
        }

        private void hScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            float xValue = DirectionLightX.Value / (360.0f / 2) - 1;
            float xTruncatedValue = (float)(Math.Truncate((double)xValue * 100.0) / 100.0);
            DirectionLightXValue.Text = "X: " + xTruncatedValue.ToString();

            NativeMethods.DirectionaLightRotateX(xTruncatedValue);
        }

        private void DirectionalLightY_Scroll(object sender, ScrollEventArgs e)
        {
            float yValue = DirectionalLightY.Value / (360.0f /2) - 1;
            float yTruncatedValue = (float)(Math.Truncate((double)yValue * 100.0) / 100.0);
            DirectionalLightYValue.Text = "Y: " + yTruncatedValue.ToString();

            NativeMethods.DirectionaLightRotateY(yTruncatedValue);
        }

        private void DirectionalLightZ_Scroll(object sender, ScrollEventArgs e)
        {
            float zValue = DirectionalLightZ.Value / (360.0f / 2) - 1;
            float zTruncatedValue = (float)(Math.Truncate((double)zValue * 100.0) / 100.0);
            DirectionalLightZValue.Text = "Z: " + zTruncatedValue.ToString();

            NativeMethods.DirectionaLightRotateZ(zTruncatedValue);
        }



    }
}