﻿namespace EntityEditor
{
    partial class EntityEditorForm
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
            this.EntityPanel = new System.Windows.Forms.Panel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.Btn_Add = new System.Windows.Forms.Button();
            this.Btn_Edit = new System.Windows.Forms.Button();
            this.Btn_Remove = new System.Windows.Forms.Button();
            this.EntityContentList = new System.Windows.Forms.ListBox();
            this.EntityPanelMenu = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newEntityToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openEntityToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.saveEntityToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openEntityToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addComponentToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editComponentToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeComponentToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.renameEntityToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openEntityFile = new System.Windows.Forms.OpenFileDialog();
            this.saveEntityFile = new System.Windows.Forms.SaveFileDialog();
            this.EntityPanel.SuspendLayout();
            this.panel1.SuspendLayout();
            this.EntityPanelMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // EntityPanel
            // 
            this.EntityPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.EntityPanel.BackColor = System.Drawing.Color.White;
            this.EntityPanel.Controls.Add(this.panel1);
            this.EntityPanel.Controls.Add(this.EntityContentList);
            this.EntityPanel.Controls.Add(this.EntityPanelMenu);
            this.EntityPanel.Location = new System.Drawing.Point(0, 0);
            this.EntityPanel.Name = "EntityPanel";
            this.EntityPanel.Size = new System.Drawing.Size(354, 511);
            this.EntityPanel.TabIndex = 0;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.Btn_Add);
            this.panel1.Controls.Add(this.Btn_Edit);
            this.panel1.Controls.Add(this.Btn_Remove);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(0, 481);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(354, 30);
            this.panel1.TabIndex = 5;
            // 
            // Btn_Add
            // 
            this.Btn_Add.Dock = System.Windows.Forms.DockStyle.Left;
            this.Btn_Add.Location = new System.Drawing.Point(234, 0);
            this.Btn_Add.Name = "Btn_Add";
            this.Btn_Add.Size = new System.Drawing.Size(117, 30);
            this.Btn_Add.TabIndex = 2;
            this.Btn_Add.Text = "Add";
            this.Btn_Add.UseVisualStyleBackColor = true;
            this.Btn_Add.Click += new System.EventHandler(this.Btn_Add_Click);
            // 
            // Btn_Edit
            // 
            this.Btn_Edit.Dock = System.Windows.Forms.DockStyle.Left;
            this.Btn_Edit.Location = new System.Drawing.Point(117, 0);
            this.Btn_Edit.Name = "Btn_Edit";
            this.Btn_Edit.Size = new System.Drawing.Size(117, 30);
            this.Btn_Edit.TabIndex = 3;
            this.Btn_Edit.Text = "Edit";
            this.Btn_Edit.UseVisualStyleBackColor = true;
            this.Btn_Edit.Click += new System.EventHandler(this.Btn_Edit_Click);
            // 
            // Btn_Remove
            // 
            this.Btn_Remove.Dock = System.Windows.Forms.DockStyle.Left;
            this.Btn_Remove.Location = new System.Drawing.Point(0, 0);
            this.Btn_Remove.Name = "Btn_Remove";
            this.Btn_Remove.Size = new System.Drawing.Size(117, 30);
            this.Btn_Remove.TabIndex = 4;
            this.Btn_Remove.Text = "Remove";
            this.Btn_Remove.UseVisualStyleBackColor = true;
            this.Btn_Remove.Click += new System.EventHandler(this.Btn_Remove_Click);
            // 
            // EntityContentList
            // 
            this.EntityContentList.Dock = System.Windows.Forms.DockStyle.Top;
            this.EntityContentList.FormattingEnabled = true;
            this.EntityContentList.Location = new System.Drawing.Point(0, 24);
            this.EntityContentList.Name = "EntityContentList";
            this.EntityContentList.Size = new System.Drawing.Size(354, 459);
            this.EntityContentList.TabIndex = 1;
            this.EntityContentList.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.EntityContentList_MouseDoubleClick);
            // 
            // EntityPanelMenu
            // 
            this.EntityPanelMenu.BackColor = System.Drawing.Color.LightGray;
            this.EntityPanelMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.openEntityToolStripMenuItem,
            this.renameEntityToolStripMenuItem});
            this.EntityPanelMenu.Location = new System.Drawing.Point(0, 0);
            this.EntityPanelMenu.Name = "EntityPanelMenu";
            this.EntityPanelMenu.Size = new System.Drawing.Size(354, 24);
            this.EntityPanelMenu.TabIndex = 0;
            this.EntityPanelMenu.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newEntityToolStripMenuItem,
            this.openEntityToolStripMenuItem1,
            this.saveEntityToolStripMenuItem1,
            this.saveAsToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // newEntityToolStripMenuItem
            // 
            this.newEntityToolStripMenuItem.Name = "newEntityToolStripMenuItem";
            this.newEntityToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.newEntityToolStripMenuItem.Text = "New Entity";
            this.newEntityToolStripMenuItem.Click += new System.EventHandler(this.newEntityToolStripMenuItem_Click);
            // 
            // openEntityToolStripMenuItem1
            // 
            this.openEntityToolStripMenuItem1.Name = "openEntityToolStripMenuItem1";
            this.openEntityToolStripMenuItem1.Size = new System.Drawing.Size(152, 22);
            this.openEntityToolStripMenuItem1.Text = "Open Entity";
            this.openEntityToolStripMenuItem1.Click += new System.EventHandler(this.openEntityToolStripMenuItem1_Click);
            // 
            // saveEntityToolStripMenuItem1
            // 
            this.saveEntityToolStripMenuItem1.Name = "saveEntityToolStripMenuItem1";
            this.saveEntityToolStripMenuItem1.Size = new System.Drawing.Size(152, 22);
            this.saveEntityToolStripMenuItem1.Text = "Save Entity";
            this.saveEntityToolStripMenuItem1.Click += new System.EventHandler(this.saveEntityToolStripMenuItem1_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.saveAsToolStripMenuItem.Text = "Save As..";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // openEntityToolStripMenuItem
            // 
            this.openEntityToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addComponentToolStripMenuItem,
            this.editComponentToolStripMenuItem,
            this.removeComponentToolStripMenuItem});
            this.openEntityToolStripMenuItem.Name = "openEntityToolStripMenuItem";
            this.openEntityToolStripMenuItem.Size = new System.Drawing.Size(88, 20);
            this.openEntityToolStripMenuItem.Text = "Components";
            // 
            // addComponentToolStripMenuItem
            // 
            this.addComponentToolStripMenuItem.Name = "addComponentToolStripMenuItem";
            this.addComponentToolStripMenuItem.Size = new System.Drawing.Size(184, 22);
            this.addComponentToolStripMenuItem.Text = "Add Component";
            this.addComponentToolStripMenuItem.Click += new System.EventHandler(this.addComponentToolStripMenuItem_Click);
            // 
            // editComponentToolStripMenuItem
            // 
            this.editComponentToolStripMenuItem.Name = "editComponentToolStripMenuItem";
            this.editComponentToolStripMenuItem.Size = new System.Drawing.Size(184, 22);
            this.editComponentToolStripMenuItem.Text = "Edit Component";
            this.editComponentToolStripMenuItem.Click += new System.EventHandler(this.editComponentToolStripMenuItem_Click);
            // 
            // removeComponentToolStripMenuItem
            // 
            this.removeComponentToolStripMenuItem.Name = "removeComponentToolStripMenuItem";
            this.removeComponentToolStripMenuItem.Size = new System.Drawing.Size(184, 22);
            this.removeComponentToolStripMenuItem.Text = "Remove Component";
            this.removeComponentToolStripMenuItem.Click += new System.EventHandler(this.removeComponentToolStripMenuItem_Click);
            // 
            // renameEntityToolStripMenuItem
            // 
            this.renameEntityToolStripMenuItem.Name = "renameEntityToolStripMenuItem";
            this.renameEntityToolStripMenuItem.Size = new System.Drawing.Size(95, 20);
            this.renameEntityToolStripMenuItem.Text = "Rename Entity";
            this.renameEntityToolStripMenuItem.Click += new System.EventHandler(this.renameEntityToolStripMenuItem_Click);
            // 
            // EntityEditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(351, 511);
            this.Controls.Add(this.EntityPanel);
            this.MainMenuStrip = this.EntityPanelMenu;
            this.Name = "EntityEditorForm";
            this.Text = "Form1";
            this.EntityPanel.ResumeLayout(false);
            this.EntityPanel.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.EntityPanelMenu.ResumeLayout(false);
            this.EntityPanelMenu.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel EntityPanel;
        private System.Windows.Forms.MenuStrip EntityPanelMenu;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openEntityToolStripMenuItem;
        private System.Windows.Forms.OpenFileDialog openEntityFile;
        private System.Windows.Forms.ListBox EntityContentList;
        private System.Windows.Forms.ToolStripMenuItem newEntityToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openEntityToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem saveEntityToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem addComponentToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editComponentToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem renameEntityToolStripMenuItem;
        private System.Windows.Forms.Button Btn_Edit;
        private System.Windows.Forms.Button Btn_Add;
        private System.Windows.Forms.Button Btn_Remove;
        private System.Windows.Forms.ToolStripMenuItem removeComponentToolStripMenuItem;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.SaveFileDialog saveEntityFile;
    }
}
