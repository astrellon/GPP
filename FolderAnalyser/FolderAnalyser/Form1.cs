using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace FolderAnalyser
{
    public partial class Form1 : Form
    {
        public const string DB_CACHE = "db.cache";

        private string mTitleBase;

        private int mPageNum = 0;
        private int mNumResults = 100;
        
        List<SearchResult> mLastSearch = null;

        private List<Database> mDB = new List<Database>();

        public Form1()
        {
            InitializeComponent();

            mTitleBase = this.Text;
        }

        private void displayResults(List<SearchResult> results, int start = 0, int end = 100)
        {
            lstResults.Items.Clear();

            end = Math.Min(end, results.Count);

            for (int i = start; i < end; i++)
            {
                SearchResult result = results[i];
                lstResults.Items.Add(result.matches + ": " + result.result);
            }

            if (results.Count == 0)
            {
                lstResults.Items.Add("No results");
            }
        }

        private void updateTitle(string title)
        {
            this.Text = mTitleBase + " " + title;
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            loadFile();
        }

        private void loadFile()
        {
            using (FolderBrowserDialog browser = new FolderBrowserDialog())
            {
                if (browser.ShowDialog() == DialogResult.OK)
                {
                    //MessageBox.Show("Folder: " + browser.SelectedPath);
                    Database db = new Database();
                    db.indexDirectory(browser.SelectedPath);
                }
            }
            /*using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "Directory List |*.txt| All files |*.*";
                //openFileDialog.FilterIndex = 1;
                openFileDialog.RestoreDirectory = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    updateTitle("Loading " + openFileDialog.FileName);

                    Database db = new Database(openFileDialog.FileName);
                    db.loadFile(openFileDialog.FileName);

                    mDB.Add(db);

                    updateTitle(openFileDialog.FileName);
                }
            }*/
        }

        private List<SearchResult> search(string terms)
        {
            List<SearchResult> results = new List<SearchResult>();
            foreach (Database db in mDB)
            {
                List<SearchResult> dbResults = db.search(terms);
                results.AddRange(dbResults);
            }

            mLastSearch = results;

            return results;
        }

        private void btnSearch_Click(object sender, EventArgs e)
        {
            List<SearchResult> results = search(txtInput.Text);
            displayResults(results);
        }

        private void txtInput_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                List<SearchResult> results = search(txtInput.Text);
                displayResults(results);
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            /*if (File.Exists(DB_CACHE) && false)
            {
                Stream cache = File.OpenRead(DB_CACHE);
                BinaryFormatter formatter = new BinaryFormatter();
                Dictionary<string, Database> loaded = (Dictionary<string, Database>)formatter.Deserialize(cache);

                if (loaded != null)
                {
                    mDB = loaded;
                }
                cache.Close();
            }*/
        }

        private void updateSearch()
        {
            if (mLastSearch == null)
            {
                return;
            }
            int numPages = mNumResults / mLastSearch.Count;
            trcPages.Maximum = numPages;
        }

        private void btnUpdateSearch_Click(object sender, EventArgs e)
        {
            if (mLastSearch == null)
            {
                return;
            }

            mNumResults = int.Parse(txtNumResults.Text);
            updateSearch();

            mPageNum = trcPages.Value;

            displayResults(mLastSearch, mPageNum * mNumResults, (mPageNum + 1) * mNumResults);
        }
    }
}
