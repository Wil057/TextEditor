#include <stdio.h>
#include <string.h>

int TransTwo(char * Line, char **Lines)  //��һάcharת��Ϊ��ά���������� 
{
	int n=0; //��������   
	int i=0,f=0;
	while(Line[i]!=0)  
		{
			if(f==79)  //ÿ�����79���ַ�����80���财��\0 
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
	static int n=0; //����������   
	int i=0,f=0;
	char Test[80] = {0};
	char TextString[1600]={0}; 
	else
	{
		while(!feof(fp))  
		{          
			memset(Test,0, sizeof(Test));                 
			fgets(Test,sizeof(Test) - 1, fp); // �����˻��з�              
			strcat(TextString,Test);	//�������ַ����浽TextStringһά������ 
		}
	}
	TransTwo(TextString,input);
}




