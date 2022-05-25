//一、功能性函数


//统计有内容的行数，包含在中间只有\0的行（空行） 
int CountLines(char StringArray[][80])  //
{
	int n=0;   
	while(strlen(StringArray[19-n]==0))
	{
		n++;
	}
	return 20-n;
} 
//将StringLine转变为二维数组存到input中 
int TransTwo(char *StringLine ) //
{
	int n=0;
	int i=0,f=0;
	while(StringLine[i]!=0)
	{
		if(f==79)  //每行最多5个字符 
		{
			input[n][f]='\0';
			n++;
			f=0; 
			i++;
			continue;
		}
		if(StringLine[i]=='\n')
		{
			
			input[n][f]='\0';
			n++;
			i++;
			f=0; 
			continue; 
		}	
		input[n][f]=StringLine[i];
		i++;
		f++;
	}
	return n;
}
//将二维数组input变为一维数组储存到StringLine 
void TransOne(char *StringLine)  //
{
	int n=0;
	int i=0;
	n=CountLines(input); 
	
	for(i=0;i<n-1;i++)
	{
		strcat(StringLine,input[i]);
		if(strlen(input[i])<79)
			StringLine[strlen(StringLine)]='\n';
	}
	strcat(StringLine,input[i]);
}
//二、文件处理函数 

//读取text文件储存到input中 
void ReadLines(FILE *fp)
{   
	char Test[80] = {0};
	char TextString[1600]={0}; 
	while(!feof(fp))  
	{          
		memset(Test,0, sizeof(Test));                 
		fgets(Test,sizeof(Test) - 1, fp); // 包含了换行符              
		strcat(TextString,Test);	//将所有字符储存到TextString一维数组中 
	} 
	TransTwo(TextString);
}
//将input保存到fp文件中 
void TextSave(FILE *fp)
{
	char TextString[1600]=0;
	TransOne(TextString);
	fprintf(fp,"%s",TextString);
} 


//三、文本编辑函数 

//复制选中区域 
void Copy(int x1,int y1,int x2,int y2) //x为列下标，y为行下标 ,1为左上角，2为右下角   //
{

	if(y1==y2)
	{
		for(int j=x1;j<=x2;j++)
		{
			copyboard[strlen(copyboard)]=input[y1][j];
		} 
	}
	else
	{
		strcat(copyboard,input[y1]+x1);
		for(int i=y1+1;i<=y2-1;i++)
		{	
			strcat(copyboard,input[i]);
		} 
		for(int j=0;j<=x2;j++)
		{
			copyboard[strlen(copyboard)]=input[y2][j];
		} 		
	}	
}
//删除选中区域 
void SelectedZoneDelet(int x1,int y1,int x2,int y2) //x为列下标，y为行下标 ,1为左上角，2为右下角  //
{
	char StringLine[1600]={};
	int i=0;
	int f=0;		//判断是否需要换行符 
	int n=CountLines(input);
	
	if(y1==y2)
	{
		if(strlen(input[y1])<79)
			f=1; 
		strcpy(input[y1]+x1,input[y1]+x2+1);//改变y1 
		
		for(int i=0;i<y1;i++)//处理0~y1-1行 
		{
			strcat(StringLine,input[i]);
			if(strlen(input[i])<79)
				StringLine[strlen(StringLine)]='\n';
		}
		
		strcat(StringLine,input[i]);//处理y1行
		if(f)
			StringLine[strlen(StringLine)]='\n';
			
		for(int i=y1+1;i<n-1;i++)	//处理y1~n行 
		{
			strcat(StringLine,input[i]);
			if(strlen(input[i])<79)
				StringLine[strlen(StringLine)]='\n';
		} 
		strcat(StringLine,input[i]);
	}
	else
	{
		if(strlen(input[y2])<79)
			f=1;
			
		input[y1][x1]='\0';	//改变y1 
		strcpy(input[y2],input[y2]+x2+1); //改变y2 
		
		for(i=0;i<y1;i++)  //处理0~y1-1行 
		{
			strcat(StringLine,input[i]);
			if(strlen(input[i])<79)
				StringLine[strlen(StringLine)]='\n';
		}
		strcat(StringLine,input[i]); //处理y1行
		i=y2;
		strcat(StringLine,input[i]);//处理y2行
		if(f)
			StringLine[strlen(StringLine)]='\n';
			
		for(i=y2+1;i<n-1;i++) //处理y2~n行 
		{
			strcat(StringLine,input[i]);
			if(strlen(input[i])<79)
				StringLine[strlen(StringLine)]='\n';
		}
		strcat(StringLine,input[i]);
	}

}  
//剪切——先复制选中区域，后删除选中区域 
int Cut(int x1,int y1,int x2,int y2) //x为列下标，y为行下标 ,1为左上角，2为右下角 
{
	char TextString[1600]=0;
	Copy(x1,y1,x2,y2); 
	SelectedZoneDelet(x1,y1,x2,y2); 
}
//粘贴
void  Paste(int x,int y)
{
	char StringLine[1600]={};
	char StringTmp[1600]={}; 
	int count=0;
	//数在x，y是第几个字符，包括\n 
	for(int i=0;i<y;i++)
	{
		for(int j=0;j<79;j++)
		{
			count++;
			if(input[i][j]=='\0')
				break;
		}
	}
	count=count+x;//x,y转为一维后的下标
	//用replaement替代search 
	TransOne(StringLine);
	strcpy(StringTmp,StringLine+count);
	strcpy(StringLine+count,copyboard);
	strcat(StringLine,StringTmp);
	
	TransTwo(StringLine);
} 

 
//四、查找函数 

//查找指定内容 
//查找 
void Search(char *Text,int *Pxy)   //数组奇数项为x坐标，即列数，偶数项为y坐标，即偶数 
{
	int i=0,j=0;
	int m;
	int c=0;
	int n=CountLines(input);
	int p,q=0;
	while(i<n)
	{
		if(input[i][j]==*Text)
		{
			p=i;
			q=j;
			for(m=1;m<strlen(Text);m++)
			{
				q++;
				if(q==79)
				{
					p++;
					q=0;
				} 
				else if(input[p][q]=='\0')
					break;
					
				if(input[p][q]!=*(Text+m))
					break;
			}
			if(m==strlen(Text))
			{
				Pxy[2*c]=i;
				Pxy[2*c+1]=j;	
				c++;
			}
				
		}
		j++;
		if(input[i][j]=='\0')
		{
			i++;
			j=0;
		}
		
	}
}  


//替换指定内容
void Replace(int x,int y,char *search,char *replacement) 
{
	int f=1;
	char StringLine[1600]={};
	char StringTmp[1600]={}; 
	int count=0;
	
	TransOne(StringLine);
	//数在x，y是第几个字符，包括\n 
	for(int i=0;i<x;i++)
	{
		for(int j=0;j<79;j++)
		{
			count++;
			if(input[i][j]=='\0')
				break;
		}
	}
	count=count+y;//x,y转为一维后的下标
	for(int i=0;i<strlen(search);i++)
	{
		if(search[i]!=*(StringLine+count+i))
			f=0;
	}
	if(f)
	{
		//用replaement替代search 
		strcpy(StringTmp,StringLine+count+strlen(search));
		strcpy(StringLine+count,replacement);
		strcat(StringLine,StringTmp);
		TransTwo(StringLine); 
	}
		
}



