using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace FolderAnalyser
{
    [Serializable]
    public class Database
    {
        private static Regex splitter = new Regex("([A-Za-z0-9]+:)|(\\\\)|(/)|(\\.)|([\\s\\-_\\?\\[\\]\\{\\}\\(\\)\\|\\+\\=]+)", RegexOptions.ECMAScript);

        public static List<string> parseEntry(string entry)
        {
            string[] results = splitter.Split(entry);
            List<string> list = new List<string>();
            for (int i = 0; i < results.Length; i++)
            {
                if (results[i].Length > 0)
                {
                    char c = results[i][0];
                    if (!splitter.IsMatch(results[i]))
                    {
                        list.Add(results[i].ToLower());
                    }
                }
            }
            return list;
        }

        private string mName = null;
        private Dictionary<string, List<int>> mKeywords = null;
        private string [] mEntries = null;
        private string mRootFolder = null;

        public Database(string name = null)
        {
            mName = name;
            mKeywords = new Dictionary<string, List<int>>();
        }

        public string name
        {
            get
            {
                return mName;
            }
            set
            {
                mName = value;
            }
        }

        public void indexDirectory(string directoryName)
        {
            mRootFolder = directoryName;

            List<string> directoryStack = new List<string>();

            indexDirectory(directoryName, ref directoryStack);
        }

        private void indexDirectory(string directoryName, ref List<string> directoryStack)
        {
            string[] directories = Directory.GetDirectories(directoryName);

            for (int i = 0; i < directories.Length; i++)
            {
                string directory = directories[i];
                //string nextFullPath = fullPath;
                /*char last = fullPath[fullPath.Length - 1];
                if (last == '\\')
                {
                    nextFullPath += directory;
                }
                else
                {
                    nextFullPath += '\\' + directory;
                }*/
                directoryStack.Add(directory);

                indexDirectory(directory, ref directoryStack);
            }

            string[] files = Directory.GetFiles(directoryName);
            for (int i = 0; i < files.Length; i++)
            {

            }
        }

        public List<SearchResult> search(string terms)
        {
            List<string> split = parseEntry(terms);

            List<SearchResult> result = new List<SearchResult>();

            Dictionary<string, int> matches = new Dictionary<string, int>();
            foreach (string term in split)
            {
                //foreach (KeyValuePair<string, List<int>> database in mKeywords)
                if (mKeywords.ContainsKey(term))
                {
                    List<int> entryNums = mKeywords[term];

                    foreach (int entryNum in entryNums)
                    {
                        string entry = mEntries[entryNum];
                        if (!matches.ContainsKey(entry))
                        {
                            matches[entry] = 1;
                        }
                        else
                        {
                            matches[entry]++;
                        }
                    }
                }
            }

            foreach (KeyValuePair<string, int> pair in matches)
            {
                result.Add(new SearchResult(pair.Key, pair.Value, this));
            }

            result.Sort();

            return result;
        }

        public void parseList(string[] entries)
        {
            mEntries = entries;

            for (int i = 0; i < entries.Length; i++)
            {
                List<string> results = parseEntry(entries[i]);

                parseResults(i, results);
            }
        }

        private void parseResults(int entryNum, List<string> results)
        {
            for (int i = 0; i < results.Count; i++)
            {
                if (!mKeywords.ContainsKey(results[i]))
                {
                    mKeywords[results[i]] = new List<int>();
                }
                List<int> keywordEntries = mKeywords[results[i]];
                keywordEntries.Add(entryNum);
            }
        }

        public void loadFile(string filename)
        {
            StreamReader streamReader = new StreamReader(filename);
            string fileText = streamReader.ReadToEnd();
            streamReader.Close();

            Database db = new Database(filename);

            string[] lines = fileText.Split(new char[] { '\n' });

            parseList(lines);
        }
    }
}
