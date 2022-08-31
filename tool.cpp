#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <list>
#include <stdlib.h>
#include <assert.h>
#include <dirent.h>

#include "tool.h"
#include <string.h>
#include <stdio.h>

using namespace std;

Tool::Tool()
{

}

bool Tool::endWith(const char str[],const char end[])
{
     int lenend = strlen(end);
     int lenstr =  strlen(str);
     if(lenend>lenstr)
      return false;

      int start = lenstr - lenend;
      int i,j=0;
      for(i = start; i<lenstr;i++,j++)
      {
            if(str[i]!= end[j])
             return false;
      }
      return true;
}

bool Tool::validateDir(const char *dir)
{
    DIR* mydir = opendir(dir);
    if(mydir == NULL)
    {
        cout<<"Directory '"<<dir<<"' does not exist!\n";
        return false;
    }
    else
    {
        closedir(mydir);
        return true;
    }
}

void Tool::toUpper(char str[])
{
    int len = strlen(str);
    int i;
    for(i = 0; i <len; i++)
    {
        str[i] = toupper(str[i]);
    }
}

void Tool::toLower(char str[])
{
    int len = strlen(str);
    int i;
    for(i = 0; i <len; i++)
    {
        str[i] = tolower(str[i]);
    }
}

void Tool::parseFName(char fname[],const char path[])
{
    assert(fname);

    strcpy(fname,"");
    //cout<<path<<endl;
     int s_loc = Tool::lastindexof(path,'\\')+1;
     int  e_loc = Tool::lastindexof(path,'.')-1;
     Tool::subinbtw(fname,path,s_loc,e_loc);

        if(!strcmp(fname,""))
       {
            s_loc = Tool::lastindexof(path,'/')+1;
            e_loc = Tool::lastindexof(path,'.')-1;
            Tool::subinbtw(fname,path,s_loc,e_loc);
       }
       //cout<<fname<<endl;
       return ;
}

bool Tool::validatePath(const char *path)
{
    int idx1 = Tool::lastindexof(path,'/');
    //cout<<idx1<<endl;
    int idx2 = Tool::lastindexof(path,'\\');
    //cout<<idx2<<endl;

    if(idx1== -1&& idx2 == -1)
    {
        //means current directory
        return true;
    }

    int idx = idx1>idx2?idx1:idx2;
    idx = idx +1;

    char *tmpdir = new char[idx];
    strncpy(tmpdir,path,idx);
    tmpdir[idx] = '\0';
    //cout<<tmpdir<<endl;
    DIR* mydir = opendir(tmpdir);
    if(mydir == NULL)
    {
        cout<<"Directory '"<<tmpdir<<"' does not exist!\n";
        return false;
    }
    else
    {
        closedir(mydir);
        return true;
    }
    delete [] tmpdir;
}
bool Tool::validateFile(const char *fn)
{
    FILE* myfp = fopen(fn,"r");
    if(myfp == NULL)
    {
        cout<<"File '"<<fn<<"' does not exist!\n";
        return false;
    }
    else
    {
        fclose(myfp);
        return true;
    }
}
void Tool::replaceinbtw(char *dest,const char *rp,int start,int end)
{
      int i,j=0;

      int len= strlen(dest)-1;
      int len2= strlen(rp)-1;

      for(i=start; (i<=end)&&(i<=len);i++)
      {
            if(j<=len2)
            {
                  dest[i] =  rp[j];
                  j++;
            }else
            {
                  break;
            }
      }
      return ;
}
void Tool::subinbtw(char dest[],const char src[],int start,int end)
{
       int flen;
       int i,r_end,k;
       bool BEGIN = false;
       flen = strlen(src);
       if(start==flen)
       {
             return ;
       }
      r_end= end>=flen?(flen-1):end;
      k=0;
       for(i=start;i<=r_end;i++)
       {
            dest[k] = src[i];
            k++;
       }

        dest[k] = '\0';

}


int Tool::firstindexof(const char *src,const char ch)
{
      int i;
      int len= strlen(src);
      for(i=0; i< len;i++)
      {
            if(src[i] == ch)
            return i;
      }
      return -1;
}

void Tool::subfirstof(char *dest,const char *src,const char stopw)
{
     int flen;
     int i;
     flen= strlen(src)-1;
     for(i=0;i<flen;i++)
     {
        if(src[i] == stopw)
        {
           break;
        }
     }

     int j;
     for(j=0;j<i;j++)
     {
       dest[j] = src[j];
     }
     dest[j] = '\0';
}

void Tool::split_twin(char *first, char *second,const char *src,const char ch)
{
    int i,j, len = strlen(src)-1;
    int brk = 1;
    for(i=0; i< len; i++)
    {
        if(src[i] != ch)
        {
            first[i] = src[i];
        }else
        {
            break;
        }
    }
       first[i] = '\0';
       i = i+1;

    for(j = i; j <= len; j++)
    {
        second[j-i] =  src[j];
        //printf("%c",src[j]);
    }
    //printf("\n");
    second[j-i] = '\0';
    //cout<<first<<endl;
    return ;
}

void Tool::sublastof(string &dest,const char *src,const char ch)
{
     int len,flen;
     int i;
     flen= strlen(src)-1;
     for(i=flen;i>=0;i--)
     {
        if(src[i] == ch)
        {
           break;
        }
     }

     i++;
     for(;i<=flen;i++)
     {
       dest = dest + src[i];
     }

}

int Tool::lastindexof(const char *src,char ch)
{
      int i,end;
      end =  strlen(src)-1;
      for(i=end; i>=0;i--)
      {
            if(src[i] == ch)
            return i;
      }
      return -1;
}

void Tool::parsePair(const char src_str[], string &fst_str,string &secd_str,const char splitter)
{
      int len,i,j;
      len =  strlen(src_str);

      for(i=0; i< len; i++)
      {
            if(src_str[i] == splitter)
            {
                  break;
            }
      }
      for(j=0;j<i;j++)
      {
            fst_str += src_str[j];
      }

      for(j=i+1;j<len;j++)
      {
            secd_str += src_str[j];
      }
      return ;
}

void Tool::substr(char str[],char dest[],char start)
{
	int len,i;
	len =  strlen(str);
	for(i=0;i<len;i++)
	{
		if(str[i]==start)
			break;
	}
	i=i+1;
	int j=0;
	for(;i<len;i++)
	{
        if(str[i] != '\n')
		{
                 dest[j]=str[i];
                 j++;
         }else
         {
               break;
          }
	}
	dest[j]='\0';
}

void Tool::splitby(const char *line,char splitter,vector<string> &words)
{
	assert(line!=NULL);

	int len=strlen(line);
	int i,size = 0;
	char ch;

	string word="";

	for(i=0;i<len;i++)
	{
		ch=line[i];
		if(ch!=splitter)
		{
			word=word+ch;
		}
		else
		{
			words.push_back(word);
			word="";
			size = 0;
		}
	}
	words.push_back(word);
	return ;
}

void Tool::parseLine(char *line, const char spliter, set<int> &regTab)
{
    assert(line != NULL);

    int len=strlen(line);
	int i,size = 0;
	char word[30];
	char ch;
	int val,num;
	bool PARSED = true;
	num = 0;

	for(i=0;i<len;i++)
	{
		ch=line[i];

		if((ch >='0' && ch <= '9'))
		{
		    PARSED = false;
			word[size]=ch;
			size++;
		}else if(ch == '+'||ch=='-')
		{
		    word[size]=ch;
			size++;
		}else if(ch == '.')
		{
		    if(!PARSED)
		    {
		        word[size]=ch;
		        size++;
		    }
		}
		else
		{
		    if(!PARSED)
			{
			    word[size]='\0';
			    val=atoi(word);
			    regTab.insert(val);

			    PARSED = true;
			    strcpy(word,"");
			    size = 0;
			}
		}
	}
	word[size]='\0';
	if(!PARSED&& strcmp(word,""))
	{
		val=atoi(word);
		regTab.insert(val);
	}
}

void Tool::parseLine(char *line,int len_of_line,char spliter,float* parsedVals,int &num)
{
	assert(line != NULL);
	assert(parsedVals != NULL);

	int len=len_of_line;
	int i,size = 0;
	char word[30];
	char ch;
	bool PARSED = true;

	num = 0;
	for(i=0;i<len;i++)
	{
		ch=line[i];

		if(ch >='0' && ch <= '9')
		{
		    PARSED = false;
			word[size]=ch;
			size++;
		}else if(ch == '+'||ch=='-')
		{
		    word[size]=ch;
			size++;
		}else if(ch == '.')
		{
		    if(!PARSED)
		    {
		        word[size]=ch;
		        size++;
		    }
		}
		else
		{
		    if(!PARSED)
			{
			    word[size]='\0';
			    parsedVals[num]=atof(word);

			    PARSED = true;
			    strcpy(word,"");
			    size = 0;
			    num++;
			}
		}
	}
	word[size]='\0';
	if(!PARSED && strcmp(word,""))
	{
		parsedVals[num]=atof(word);
        num = num+1;
	}
}


set<unsigned int> Tool::parseLine(char *line, const char spliter)
{
    assert(line != NULL);
    set<unsigned int> mylist;

    int len=strlen(line);

	int i,size = 0;
	char word[30];
	char ch;
	unsigned int val;
	int num;
	bool PARSED = true;

	num = 0;
	for(i=0;i<len;i++)
	{
		ch=line[i];

		if(ch >='0' && ch <= '9')
		{
		    PARSED = false;
			word[size]=ch;
			size++;
		}else if(ch == '+'||ch=='-')
		{
		    word[size]=ch;
			size++;
		}else if(ch == '.')
		{
		    if(!PARSED)
		    {
		        word[size]=ch;
		        size++;
		    }
		}
		else
		{
		    if(!PARSED)
			{
			    word[size]='\0';
			    val=atoi(word);
			    mylist.insert(val);

			    PARSED = true;
			    strcpy(word,"");
			    size = 0;
			}
		}
	}
	word[size]='\0';
	if(!PARSED&& strcmp(word,""))
	{
		val=atoi(word);
		mylist.insert(val);
	}
	return mylist;
}

void Tool::trimEnd(char *line,const char ch)
{
	int len=strlen(line);
	int i,j;

	if(!len)
	return ;

	for(i=len-1;i>0;i--)
	{
		//cout<<line.at(i)<<endl;
		if(line[i]!=ch)
		{
			break;
		}
	}
	for(j=i+1; j <len;j++)
	{
		line[j] = '\0';
	}
	//cout<<"In trimEnd, end"<<line<<endl;
	return ;
}
void Tool::trimHead(char *line,const int len,const char ch)
	{
	int i,j;
	if(!len)
	return ;
	for(i=0;i<len;i++)
	{
		if(line[i]!=ch)
		{
			break;
		}
	}
	for(j=0;line[i]!='\0';j++,i++)
	{
		line[j] = line[i];
	}
	return ;
	}

void Tool::wash(char *line,int len_of_line)
{
	//cout<<"In Tool::Wash, begin: "<<line<<endl;
	Tool::trimHead(line,len_of_line,' ');
	Tool::trimEnd(line,' ');
	Tool::trimEnd(line,'\n');
	Tool::trimEnd(line,'\r');
	//cout<<"In Tool::Wash, end: "<<line<<endl;
	return ;
}

void Tool::fetchfiles(vector<char *> &fnames,const char *fname)
{
	FILE *fp=fopen(fname,"r");
	printf("file Name %s\n hi",fname);
	if(!fp)
	{
		printf("%s can not open!\n",fname);
		return ;
	}

	char onefile[100];
	char *crntname;
	while(!feof(fp))
	{
		fscanf(fp,"%s\n",onefile);
		//printf("%s\n",onefile);
		crntname=new char[100];
		strcpy(crntname,onefile);
		fnames.push_back(crntname);
	}
	fclose(fp);
}


void Tool::time2Str(char *tmStr,const int seconds)
{
    int realtm = seconds;
       int tm_hour = realtm/3600;
       realtm = seconds%3600;
       int tm_min = realtm/60;
       int tm_sec = realtm%60;
       //char *tmstr = new char[12];
       char hour[3];
       char min[3];
       char sec[3];

 if(tm_hour>9)
       {
           sprintf(hour,"%d",tm_hour);
       }else if(tm_hour>0)
       {
           sprintf(hour,"0%d",tm_hour);
       }else
       {
           sprintf(hour,"0");
       }
       if(tm_min>9)
       {
           sprintf(min,"%d",tm_min);
       }else if(tm_min >0)
       {
           sprintf(min,"0%d",tm_min);
       }else
       {
           sprintf(min,"00",tm_min);
       }

       if(tm_sec>9)
       {
           sprintf(sec,"%d",tm_sec);
       }else if(tm_sec > 0)
       {
           sprintf(sec,"0%d",tm_sec);
       }else
       {
           sprintf(sec,"00");
       }

       sprintf(tmStr,"%s:%s:%s",hour,min,sec);
}

void Tool::cpySubstr(const char srcstr[],char dest[],const char bstart,const char bend)
{
     int i,be,bs,end;
     be = strlen(srcstr)-1;
     bs = be;
     end =  bs;
     //cout<<"start: "<<bstart<<endl;
     //cout<<"end: "<<bend<<endl;
     bool PASSED = false;
     for(i = end; i>=0;i--)
     {
           if((srcstr[i] != bstart)&&(!PASSED))
           {
               be--;
               bs--;
           }else
           {
                PASSED = true;
            }
           if(srcstr[i] != bend)
           {
              be--;
           }else
           {
                //cout<<srcstr[i]<<endl;
                break;
           }
     }

     be = be+4;
     for(i=be;i<bs;i++)
     {

         dest[i-be] = srcstr[i];
         //cout<<dest[i-be];
     }
     //cout<<endl;
     dest[i-be] = '\0';
}

const char *Tool::time2Str(const int seconds)
{
       int realtm = seconds;
       int tm_hour = realtm/3600;
       realtm = seconds%3600;
       int tm_min = realtm/60;
       int tm_sec = realtm%60;
       char *tmstr = new char[12];
       char hour[3];
       char min[3];
       char sec[3];


       if(tm_hour>9)
       {
           sprintf(hour,"%d",tm_hour);
       }else if(tm_hour>0)
       {
           sprintf(hour,"0%d",tm_hour);
       }else
       {
           sprintf(hour,"0");
       }
       if(tm_min>9)
       {
           sprintf(min,"%d",tm_min);
       }else if(tm_min > 0)
       {
           sprintf(min,"0%d",tm_min);
       }else
       {
           sprintf(min,"00");
       }

       if(tm_sec>9)
       {
           sprintf(sec,"%d",tm_sec);
       }else if(tm_sec > 0)
       {
           sprintf(sec,"0%d",tm_sec);
       }else
       {
           sprintf(sec,"00");
       }

       sprintf(tmstr,"%s:%s:%s",hour,min,sec);
       return tmstr;
}

void Tool::testTool()
{
	char dir[]="e:\\wanlei\\lab\\dest\\pairs";
	if(Tool::endWith(dir,"\\"))
	 {
        printf("you are right.\n");
     }else
     {
          printf("you are wrong.\n");
      }

}
Tool::~Tool()
{

}
