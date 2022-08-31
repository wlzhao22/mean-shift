#if !defined TOOL_H
#define TOOL_H

#include <string>
#include <vector>
#include <list>
#include <set>
#include <iostream>

using namespace std;

class Tool
{
public:
		Tool();
		static void splitby(const char *line,char spliter,vector<string> &words);

			//not a safe method
      static void parsePair(const char src_str[], string &fst_str,string &secd_str,const char splitter);
      static void cpySubstr(const char srcstr[],char dest[],const char bstart,const char bend);
      static void cpySubStr(char *dest,const char str[],const int index);
      static void substr(char str[],char dest[],char start);
      static void subinbtw(char dest[],const char src[],int start,int end);

      static bool endWith(const char str[],const char end[]);
      static void trimEnd(char *line,const char ch);
      static void trimHead(char *line,const int len,const char ch);
      static void sublastof(string &dest,const char *src,const char ch);
      static void subfirstof(char *dest,const char *src,const char ch);
      static int firstindexof(const char *src,const char ch);
      static int lastindexof(const char *src,const char ch);

      static void replaceinbtw(char *dest,const char *rp,int start,int end);
      static void split_twin(char *first, char *second,const char *src,const char ch);


      static void testTool();
      static void wash(char *line,int len_of_line);

      static void parseLine(char *line, const char spliter, set<int> &regTab);
      static set<unsigned int> parseLine(char *line, const char spliter);
      static void parseLine(char *line,int len_of_line,char spliter,float* parsedVals,int &num);
      static void fetchfiles(vector<char *> &fnames,const char *fname);

      static bool validateDir(const char *dir);
      static bool validateFile(const char *fn);
      static bool validatePath(const char *path);

      static void parseFName(char fname[],const char path[]);
      static void toUpper(char str[]);
      static void toLower(char str[]);

      static const char *time2Str(const int seconds);
      static void time2Str(char *tmStr,const int seconds);

	virtual ~Tool();

};

#endif
