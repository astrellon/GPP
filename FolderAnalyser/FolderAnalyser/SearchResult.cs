using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FolderAnalyser
{
    public class SearchResult : IComparable
    {
        public string result;
        public int matches;
        public Database db;

        public SearchResult()
        {
            result = null;
            matches = -1;
            db = null;
        }

        public SearchResult(string result, int matches, Database db)
        {
            this.result = result;
            this.matches = matches;
            this.db = db;
        }

        int IComparable.CompareTo(object obj)
        {
            SearchResult other = (SearchResult)obj;
            if (other == null)
            {
                return 0;
            }

            if (other.matches != matches)
            {
                return other.matches - matches;
            }
            return other.result.CompareTo(other.result);
        }
    }
}
