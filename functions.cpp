//һ�������Ժ���


//ͳ�������ݵ��������������м�ֻ��\0���У����У� 
int CountLines(char StringArray[][80])  //
{
	int n=0;   
	while(strlen(StringArray[19-n]==0))
	{
		n++;
	}
	return 20-n;
} 
//��StringLineת��Ϊ��ά����浽input�� 
int TransTwo(char *StringLine ) //
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
//����ά����input��Ϊһά���鴢�浽StringLine 
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
//�����ļ������� 

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
//��input���浽fp�ļ��� 
void TextSave(FILE *fp)
{
	char TextString[1600]=0;
	TransOne(TextString);
	fprintf(fp,"%s",TextString);
} 


//�����ı��༭���� 

//����ѡ������ 
void Copy(int x1,int y1,int x2,int y2) //xΪ���±꣬yΪ���±� ,1Ϊ���Ͻǣ�2Ϊ���½�   //
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
//ɾ��ѡ������ 
void SelectedZoneDelet(int x1,int y1,int x2,int y2) //xΪ���±꣬yΪ���±� ,1Ϊ���Ͻǣ�2Ϊ���½�  //
{
	char StringLine[1600]={};
	int i=0;
	int f=0;		//�ж��Ƿ���Ҫ���з� 
	int n=CountLines(input);
	
	if(y1==y2)
	{
		if(strlen(input[y1])<79)
			f=1; 
		strcpy(input[y1]+x1,input[y1]+x2+1);//�ı�y1 
		
		for(int i=0;i<y1;i++)//����0~y1-1�� 
		{
			strcat(StringLine,input[i]);
			if(strlen(input[i])<79)
				StringLine[strlen(StringLine)]='\n';
		}
		
		strcat(StringLine,input[i]);//����y1��
		if(f)
			StringLine[strlen(StringLine)]='\n';
			
		for(int i=y1+1;i<n-1;i++)	//����y1~n�� 
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
			
		input[y1][x1]='\0';	//�ı�y1 
		strcpy(input[y2],input[y2]+x2+1); //�ı�y2 
		
		for(i=0;i<y1;i++)  //����0~y1-1�� 
		{
			strcat(StringLine,input[i]);
			if(strlen(input[i])<79)
				StringLine[strlen(StringLine)]='\n';
		}
		strcat(StringLine,input[i]); //����y1��
		i=y2;
		strcat(StringLine,input[i]);//����y2��
		if(f)
			StringLine[strlen(StringLine)]='\n';
			
		for(i=y2+1;i<n-1;i++) //����y2~n�� 
		{
			strcat(StringLine,input[i]);
			if(strlen(input[i])<79)
				StringLine[strlen(StringLine)]='\n';
		}
		strcat(StringLine,input[i]);
	}

}  
//���С����ȸ���ѡ�����򣬺�ɾ��ѡ������ 
int Cut(int x1,int y1,int x2,int y2) //xΪ���±꣬yΪ���±� ,1Ϊ���Ͻǣ�2Ϊ���½� 
{
	char TextString[1600]=0;
	Copy(x1,y1,x2,y2); 
	SelectedZoneDelet(x1,y1,x2,y2); 
}
//ճ��
void  Paste(int x,int y)
{
	char StringLine[1600]={};
	char StringTmp[1600]={}; 
	int count=0;
	//����x��y�ǵڼ����ַ�������\n 
	for(int i=0;i<y;i++)
	{
		for(int j=0;j<79;j++)
		{
			count++;
			if(input[i][j]=='\0')
				break;
		}
	}
	count=count+x;//x,yתΪһά����±�
	//��replaement���search 
	TransOne(StringLine);
	strcpy(StringTmp,StringLine+count);
	strcpy(StringLine+count,copyboard);
	strcat(StringLine,StringTmp);
	
	TransTwo(StringLine);
} 

 
//�ġ����Һ��� 

//����ָ������ 
//���� 
void Search(char *Text,int *Pxy)   //����������Ϊx���꣬��������ż����Ϊy���꣬��ż�� 
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


//�滻ָ������
void Replace(int x,int y,char *search,char *replacement) 
{
	int f=1;
	char StringLine[1600]={};
	char StringTmp[1600]={}; 
	int count=0;
	
	TransOne(StringLine);
	//����x��y�ǵڼ����ַ�������\n 
	for(int i=0;i<x;i++)
	{
		for(int j=0;j<79;j++)
		{
			count++;
			if(input[i][j]=='\0')
				break;
		}
	}
	count=count+y;//x,yתΪһά����±�
	for(int i=0;i<strlen(search);i++)
	{
		if(search[i]!=*(StringLine+count+i))
			f=0;
	}
	if(f)
	{
		//��replaement���search 
		strcpy(StringTmp,StringLine+count+strlen(search));
		strcpy(StringLine+count,replacement);
		strcat(StringLine,StringTmp);
		TransTwo(StringLine); 
	}
		
}



