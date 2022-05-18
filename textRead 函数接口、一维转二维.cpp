#include <stdio.h>
#include <string.h>

int TransTwo(char * Line, char **Lines)  //将一维char转变为二维，返回行数 
{
	int n=0; //储存行数   
	int i=0,f=0;
	while(Line[i]!=0)  
		{
			if(f==79)  //每行最多79个字符，第80个需储存\0 
			{
				char[n][f]='\0';
				n++;
				f=0; 
				continue; 
			}
			if(Line[i]=='\n')
			{
				Lines[n][f]='\0';
				n++;
				i++;
				f=0; 
				continue;
			}	
			Lines[n][f]=Line[i];
			i++;
			f++;
		}
		return n;
} 

void ReadLines(FILE *fp)
{   
	static int n=0; //储存总行数   
	int i=0,f=0;
	char Test[80] = {0};
	char TextString[1600]={0}; 
	else
	{
		while(!feof(fp))  
		{          
			memset(Test,0, sizeof(Test));                 
			fgets(Test,sizeof(Test) - 1, fp); // 包含了换行符              
			strcat(TextString,Test);	//将所有字符储存到TextString一维数组中 
		}
	}
	TransTwo(TextString,input);
}




