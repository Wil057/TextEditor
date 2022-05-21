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


 //统计有内容的行数，包含在中间只有\0的行（空行） 
int CountLines(char StringArray[][80])
{
	int n=0;   
	do
	{
		n++;
	}while(strlen(StringArray[n-1]!=0));
	return n;
} 



//将StringLine转变为二维数组存到input中 
int TransTwo(char *StringLine )
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
//在ReadLines后需要fclose 


 //将二维数组input变为一维数组StringLine 
void TransOne(char *StringLine) 
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


//删除选中区域 
void SelectedZoneDelet(int x1,int y1,int x2,int y2) //x为列下标，y为行下标 ,1为左上角，2为右下角 
{

	char StringLine[1600]=0;
	int i=0;
	int f=0;		//判断是否需要换行符 
	int n=CountLines(input) ;
	
	if(y1==y2)
	{
		if(strlen(input[y1])<79)
			f=1; 
		strcpy(input[y1]+x1,input[y1]+x2+1)
		
		
		for(int i=0;i<y1;i++)
		{
			strcat(StringLine,input[i]);
			if(strlen(input[i])<79)
				StringLine[strlen(StringLine)]='\n';
		}
		strcat(StringLine,input[i]);
		if(f)
			StringLine[strlen(StringLine)]='\n';
	}
	else
	{
		if(strlen(input[y2])<79)
			f=1;
			
		input[y1][x1]='\0';	//改变y1 
		input[y2]=input[y2]+x2+1; //改变y2 
		
		
		for(i=0;i<y1;i++)  //处理0~y1-1行 
		{
			strcat(StringLine,input[i]);
			if(strlen(input[i])<79)
				StringLine[strlen(StringLine)]='\n';
		}
		strcat(StringLine,input[i]); //处理y1行
		 
		for(i=y2;i<n;i++) //处理y2~n行 
		{
			strcat(StringLine,input[i]);
			if(strlen(input[i])<79)
				StringLine[strlen(StringLine)]='\n';
		}
		strcat(StringLine,input[i]);
	}
	 	/*int n=CountLines(input);
	int a[n]=0;
	char StringLine[1600]=0;
	//判断是否有换行符 
	for(int i=0;i<n;i++)
	{
		if(strlen(input[i])<79)
			a[i]=1;
	}
	
	//删去字符 
	for(int i=y1;i<y2-1;i++)
	{
		strcpy(input[i]+x1,input[i]+x2+1);
	} 
	
	//末尾补充换行符 并转为一维char 
	for(int i=0;i<n-1;i++)
	{
		strcat(StringLine,input[i]);
		if(a[i]=1)
			StringLine[strlen(StringLine)]='\n';
	}
	strcat(StringLine,input[i]);
	TransTwo(StringLine)
	*/
} 


void Copy(int x1,int y1,int x2,int y2) //x为列下标，y为行下标 ,1为左上角，2为右下角 
{

	if(y1==y2)
	{
		for(int j=x1,j<=x2,j++)
		{
			tmp[strlen(tmp)]=input[y1][j];
		} 
	}
	else
	{
		strcat(tmp,input[y1]+x1);
		for(int i=y1+1,i<=y2-1,i++)
		{	
			strcat(tmp,input[i]);
		} 
		for(int j=0,j<=x2,j++)
		{
			tmp[strlen(tmp)]=input[y2][j];
		} 
		
	}
	
}


int Cut(int x1,int y1,int x2,int y2) //x为列下标，y为行下标 ,1为左上角，2为右下角 
{
	char TextString[1600]=0;
	Copy(x1,y1,x2,y2); 
	SelectedZoneDelet(x1,y1,x2,y2); 
}



