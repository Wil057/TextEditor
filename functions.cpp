//��ȡtext�ļ����浽input�� 
void ReadLines(FILE *fp)
{   
	char Test[80] = {0};
	char TextString[1600]={0}; 
	while(!feof(fp))  
	{          
		memset(Test,0, sizeof(Test));                 
		fgets(Test,sizeof(Test) - 1, fp); // �����˻��з�              
		strcat(TextString,Test);	//�������ַ����浽TextStringһά������ 
	} 
	TransTwo(TextString);
}


 //ͳ�������ݵ��������������м�ֻ��\0���У����У� 
int CountLines(char StringArray[][80])
{
	int n=0;   
	do
	{
		n++;
	}while(strlen(StringArray[n-1]!=0));
	return n;
} 



//��StringLineת��Ϊ��ά����浽input�� 
int TransTwo(char *StringLine )
{
	int n=0;
	int i=0,f=0;
	while(StringLine[i]!=0)
	{
		if(f==79)  //ÿ�����5���ַ� 
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
//��ReadLines����Ҫfclose 


 //����ά����input��Ϊһά����StringLine 
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


//ɾ��ѡ������ 
void SelectedZoneDelet(int x1,int y1,int x2,int y2) //xΪ���±꣬yΪ���±� ,1Ϊ���Ͻǣ�2Ϊ���½� 
{

	char StringLine[1600]=0;
	int i=0;
	int f=0;		//�ж��Ƿ���Ҫ���з� 
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
			
		input[y1][x1]='\0';	//�ı�y1 
		input[y2]=input[y2]+x2+1; //�ı�y2 
		
		
		for(i=0;i<y1;i++)  //����0~y1-1�� 
		{
			strcat(StringLine,input[i]);
			if(strlen(input[i])<79)
				StringLine[strlen(StringLine)]='\n';
		}
		strcat(StringLine,input[i]); //����y1��
		 
		for(i=y2;i<n;i++) //����y2~n�� 
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
	//�ж��Ƿ��л��з� 
	for(int i=0;i<n;i++)
	{
		if(strlen(input[i])<79)
			a[i]=1;
	}
	
	//ɾȥ�ַ� 
	for(int i=y1;i<y2-1;i++)
	{
		strcpy(input[i]+x1,input[i]+x2+1);
	} 
	
	//ĩβ���任�з� ��תΪһάchar 
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


void Copy(int x1,int y1,int x2,int y2) //xΪ���±꣬yΪ���±� ,1Ϊ���Ͻǣ�2Ϊ���½� 
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


int Cut(int x1,int y1,int x2,int y2) //xΪ���±꣬yΪ���±� ,1Ϊ���Ͻǣ�2Ϊ���½� 
{
	char TextString[1600]=0;
	Copy(x1,y1,x2,y2); 
	SelectedZoneDelet(x1,y1,x2,y2); 
}



