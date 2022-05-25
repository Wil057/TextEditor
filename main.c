#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include "imgui.h"
#include <windows.h>
#include <wingdi.h>
#include <windef.h>



static double winwidth,winheight; 
static double fontsize;
static int position=0;
static int page=1;
static char input[20][80];
static double mx,my;
static double lx,ly;
static double start_x,start_y;
static double cur_x,cur_y;
static double pretext;
static double fH;
static double h;	//�����ı��ĸ߶� 
static double setwhite = 0.4;
static char cutboard[1600];//���а� 
static char tmp[80]; 
static int pasteMode=0,focusMode=0,ColorMode=0,lockMode=0;

typedef enum {
	MouseTimer,
} MyTimer;
struct colormode{
	char light[20], deep[20];
	char font[20];
	char frame[20],label[20],hotFrame[20],hotLabel[20]; 
}color[3]={
	{
		"Madang", "LightGreen","Shark",
		"Salem","Salem","Shark","Shark"
	},
	{
		"AliceBlue", "JordyBlue","Shark",
		"JacksonPurple","JacksonPurple","Shark","Shark"
	},
	{
		"CREAM", "Dolly","Shark",
		"BurntOrange","BurntOrange","Shark","Shark"
	}
};
static char * menuListFile[] = {"File",  
	"New File   | Ctrl-N", 
	"Open File  | Ctrl-O",
	"Save As    | Ctrl-S",
	"Close File ",
	"Exit | Ctrl-Q"};
static char * menuListAbout[] = {"Preference",
	"FontSize",
	"FontStyle",
	"BackColor"};
static char * menuListPaste[] = {"Edit",  
	"Cut   | Ctrl-X", 
	"Copy  | Ctrl-C",
	"Paste | Ctrl-V",
	"Delet | Ctrl-D"};
static char * menuListSearch[] = {"Help", 
	"Search	  | Ctrl-F",
	"Replace  | Ctrl-R"};


void init_color();
void display();
void drawEditText();
void  Paste(int x,int y);
void Copy(int x1,int y1,int x2,int y2);
void Cut(int x1,int y1,int x2,int y2);
void SelectedZoneDelet(int x1,int y1,int x2,int y2) ;
void Search(char *Text,int *Pxy);
void Replace(int x,int y,char *search,char *replacement); 

void init_color()
{
	DefineColor("CREAM", 1, 1, 0.8);
	DefineColor("AliceBlue", 0.89,0.95,0.99);
	DefineColor("HummingBird",0.77, 0.937, 0.969);
	DefineColor("JordyBlue",0.54,0.77,0.96);	
	DefineColor("Spray",0.5059,0.8117,0.8784);
	DefineColor("Shark",0.004, 0.196, 0.263);
	DefineColor("JacksonPurple",0.122, 0.227, 0.576);
	DefineColor("Madang",0.784,0.968,0.773);
	DefineColor("LightGreen",0.482,0.937,0.698);
	DefineColor("Salem",0.118,0.5098,0.298);
	DefineColor("WitchHaze",1,0.9647,0.5608);
	DefineColor("Dolly",1,1,0.494);
	DefineColor("BurntOrange",0.827, 0.3294 ,0 );
}

int inText(double mx,double my)
{
	return (mx>=setwhite && my<=winheight-fH*3)?1:0;
}

double nearestCharX(double x,double y) //�����ڵ�ǰ�����������¾������x����ַ���xλ�� 
{
	int cursorLine = AnalyzePosition(y);
	int len = strlen(input[cursorLine]);
	int i,j;
	for(i=0;i<len;i++)
	{
		for(j=0;j<=i;j++)
		{
			tmp[j]=input[cursorLine][j];
		}
		tmp[j]='\0';
		if(x > TextStringWidth(tmp)+pretext )
		{
			tmp[j]=input[cursorLine][j];
			tmp[j+1]='\0';
			if(x < TextStringWidth(tmp)+pretext )
			{
				return TextStringWidth(tmp)+0.4;
			}
		}
	}
	if(x > TextStringWidth(input[cursorLine])+pretext)
	{
		return TextStringWidth(input[cursorLine])+pretext;
	}
	if(x < pretext)
	{
		return pretext;
	}
}

int nearestCharX_pos(double x,double y) //�����ڵ�ǰ�����������¾������x����ַ���xλ�� 
{
	int cursorLine = AnalyzePosition(y);
	int len = strlen(input[cursorLine]);
	int i,j;
	for(i=0;i<len;i++)
	{
		for(j=0;j<=i;j++)
		{
			tmp[j]=input[cursorLine][j];
		}
		tmp[j]='\0';
		if(x > TextStringWidth(tmp)+pretext )
		{
			tmp[j]=input[cursorLine][j];
			tmp[j+1]='\0';
			if(x < TextStringWidth(tmp)+pretext )
			{
				return j;
			}
		}
	}
	if(x > TextStringWidth(input[cursorLine])+pretext)
	{
		return strlen(input[cursorLine]);
	}
	if(x < pretext)
	{
		return 0;
	}
}

double nearestCharY(double ly)	//����ly�����ֵ׶˵ĵ�yֵ 
{
	double set_y=winheight-fH*3; //set_y�ǵ�һ��ֱ�ߵ�λ�� 
	int i; 
	
	for(i=0;i<20;i++)
	{
		if(ly <= set_y-i*h && ly >= set_y-(i+1)*h)
		{
			return set_y-(i+1)*h+fH/2; 
		}
	}
	return 0;
}

int AnalyzePosition(double y)	//according to the position of y,we could analyze which line this y belongs to.
{
	double set_y=winheight-fH*3; //set_y�ǵ�һ��ֱ�ߵ�λ�� 
	int i;
	
	for(i=0;i<20;i++)
	{
		if(y <= set_y-i*h && y >= set_y-(i+1)*h)
		{
			return i;
		}
	}
	return 0;
}

unsigned char openfile(char* filename)
{
	char file[MAX_PATH], * fileselect = NULL;
	OPENFILENAME open;
	
	ZeroMemory(&open, sizeof(OPENFILENAME));
	open.lStructSize = sizeof(OPENFILENAME);
	open.lpstrFile = file;
	open.lpstrFile[0] = '\0';
	open.nMaxFile = MAX_PATH;
	open.nFilterIndex = 1;
	open.lpstrFileTitle = NULL;
	open.nMaxFileTitle = 0;
	open.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetOpenFileName(&open)) 
	{
		fileselect = open.lpstrFile;
		strcpy(filename, fileselect);
		return 1;
	}
	else 
	{
		return 0;
	}
}
//�ļ���ȡ���� 
void ReadLines(FILE *fp)
{
	char Test[80] = {0};
	char TextString[1600]={0}; 
	while(!feof(fp))  
	{          
		memset(Test,0, sizeof(Test));                 
		fgets(Test,sizeof(Test) - 1, fp);            
		strcat(TextString,Test);	
	} 
	TransTwo(TextString);
}
//ͳ�������ݵ��������������м�ֻ��\0���У����У� 
int CountLines(char StringArray[][80])	
{
	int n=0;   
	while(strlen(StringArray[19-n])==0)
	{
		n++;
	}
	return 20-n;
} 
//����ά����input��Ϊһά���鴢�浽StringLine 
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
//��StringLineת��Ϊ��ά����浽input�� 
int TransTwo(char *StringLine)
{
	int n=0;
	int i=0,f=0;
	while(StringLine[i]!=0)
	{
		if(f==79)  
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

void CharEventProcess(char ch)
{
	uiGetChar(ch); 
	display();
}

void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key,event);
	switch(event)
	{
		case BUTTON_DOWN:
		{
			if(key==VK_UP)
			{
				position > 0 ? --position : position;
				break;
			}
			else if(key==VK_DOWN)
			{
				position<20 ? ++position : position;
				break;
			}
			else if(key==VK_RETURN)
			{
				position<20? ++position : position;
				break;
			}
			else if(key==VK_BACK)
			{
				if(strlen(input[position])==0)
				{
					position>0 ? --position : position;
					break;
				}
			}
		}
	}
	display();
}

void MouseEventProcess(int x, int y, int button, int event)
{
	uiGetMouse(x,y,button,event);
	mx = ScaleXInches(x);	//x is pixel,we should convert it into inches.
	my = ScaleYInches(y);
	switch(event)
	{
		case BUTTON_DOWN:
		{
			if(button==LEFT_BUTTON)
			{
				if(inText(mx,my))
				{
					focusMode = 1;
					lockMode = 0;
				}
				position=AnalyzePosition(my);
				lx = nearestCharX(mx,my);	//needed for focusMode.
				ly = nearestCharY(my);
			}
			else if(button==RIGHT_BUTTON)
			{
				pasteMode = 1;
			}
			break;
		}
		case BUTTON_UP:
		{
			if(button == LEFT_BUTTON)
			{
				if(focusMode==1)
				{
					lockMode = 1;
					focusMode = 0;
				}
			}
			break;
		}
		case BUTTON_DOUBLECLICK:
		{
			focusMode = 0;
			lockMode = 0;
			pasteMode = 0;
			break;
		}
		case MOUSEMOVE:
		{
			if(focusMode)
			{
				mx=nearestCharX(mx,my);
				my=nearestCharY(my);
			}
			break;
		}
	}
	display();
}

void drawBackground()
{
	if(ColorMode != 0 )
	{
		SetPenColor(color[ColorMode].deep);
		StartFilledRegion(1);
		MovePen(0, 0); 
		DrawLine(winwidth, 0);
		DrawLine(0, winheight);
		DrawLine(-winwidth, 0);
		DrawLine(0, -winheight);
		EndFilledRegion();
	}
}

void drawMenu()
{
	static char * selectedLabel = NULL;
	double x = 0;
	double y = winheight;
	double h = fH*1.5; // �ؼ��߶�
	double w = TextStringWidth(menuListAbout[0])*1.8; // �ؼ����
	double wlist = TextStringWidth(menuListAbout[0])*1.8;
	int    selection;
	
	drawMenuBar(0,y-h,winwidth,h);

	selection = menuList(1, x, y-h, w, wlist, h, menuListFile, sizeof(menuListFile)/sizeof(menuListFile[0]));
	if(selection>0) 
	{
		switch(selection)
		{
			case 1:
				{
					Copy(nearestCharX_pos(lx,ly),AnalyzePosition(ly),nearestCharX_pos(cur_x,cur_y),AnalyzePosition(cur_y));
					break;
				}
			case 2:
				{
					FILE *fp;
					char filename[80];
					unsigned char check=0;
					check=openfile(filename);
					if((fp = fopen(filename,"r+"))!=0)
					{
						ReadLines(fp);
					}
					fclose(fp);
					break;
				}
			case 3:
				{
					
					break;
				}
			case 4:
				{
		
				}
		};		
	}
	else if( selection==5 ) 
	{
		
	}

	selection = menuList(2,x+w,  y-h, w, wlist,h, menuListAbout,sizeof(menuListAbout)/sizeof(menuListAbout[0]));
	if( selection>0 ) 
	{
		switch(selection)
		{
			case 1:
			{
				 ColorMode=1;
				 break;
			}
		}
	};
	selection = menuList(3,x+2*w,  y-h, w, wlist,h, menuListPaste,sizeof(menuListPaste)/sizeof(menuListPaste[0]));
	if( selection>0 ) 
	{
		switch(selection)
		{
			case 1:
			{
				Cut(nearestCharX_pos(lx,ly),AnalyzePosition(ly),nearestCharX_pos(cur_x,cur_y),AnalyzePosition(cur_y));
				break;
			}
			case 2:
			{
				Copy(nearestCharX_pos(lx,ly),AnalyzePosition(ly),nearestCharX_pos(cur_x,cur_y),AnalyzePosition(cur_y));
				break;
			}
			case 3:
			{
				Paste(nearestCharX_pos(mx,my),position);
				break;
			}
			case 4:
			{
				SelectedZoneDelet(nearestCharX_pos(lx,ly),AnalyzePosition(ly),nearestCharX_pos(cur_x,cur_y),AnalyzePosition(cur_y));
				break;
			}
		}
	};
	
	selection = menuList(4,x+3*w,  y-h, w, wlist,h, menuListSearch,sizeof(menuListSearch)/sizeof(menuListSearch[0]));
	if( selection>0 ) 
	{
		switch(selection)
		{
			case 1:
			{
				//Search();
				break;
			}
			case 2:
			{
				//Replace();
				break;
			}
		}
	};
}

void drawEditor()
{
	double x=0;
	double y=winheight-fH*3; //������һ��ֱ�ߵ�λ�� 
	double w = winwidth; // �ؼ����

	SetPenColor("blue");
	MovePen(x+setwhite,y);
	DrawLine(0,-winheight);
	MovePen(winwidth-setwhite,y); //'winwidth-setwhite"�������Ҳ��xֵ 
	DrawLine(0,-winwidth);
	
	int i=0;
	for(i=0;i<20;i++)
	{
		MovePen(x+setwhite,y-h*i);
		DrawLine(winwidth-setwhite*2,0);
	}
}

void DrawEditedText()
{
	double x=0;
	double y=winheight-fH*4.45; //��Ϊ������һ��ֱ���Ƿָ��ߣ����������õڶ���ֱ�߿�ʼ���ı��� 
	double w = winwidth; // �ؼ����
	
	int i=0;
	for(i=0;i<20;i++)
	{
		SetPenColor("blue");
		if(i!=position)
		{
			drawLabel(x+pretext,y-fH*2*i,input[i]);
		}
	}
}

void drawEditText()
{
	double x=0;
	double y=winheight-fH*3-fH*2; //��Ϊ������һ��ֱ���Ƿָ��ߣ����������õڶ���ֱ�߿�ʼ���ı��� 
	double w = winwidth; // �ؼ����
	
	switch(position)
	{
		case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:
		case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:
		textbox(2,x+setwhite,y-fH*2*position,w-2*setwhite,h,input[position],sizeof(input[position]));
		break;
	};
	
}

static double repo_curx,repo_cury;
void drawFocus()
{
	if(!lockMode)	
	{
		start_x = lx,start_y = ly; 
		cur_y=nearestCharY(my);
		cur_x=nearestCharX(mx,my);
		repo_curx=cur_x;
		repo_cury=cur_y;
	}
	
	SetPenColor(color[0].light);
	for(;fabs((double)(start_y - cur_y)) < 0.001 ||start_y >= cur_y; start_y-=h ) //���ǵ��������ıȽ��ǲ�ȷ���ģ���������һ������ 
	{
		if(fabs(start_y - cur_y) > 0.001)
		{
			StartFilledRegion(1);
			MovePen(start_x,start_y);    //lx and ly means "last x"&"last y" 
			DrawLine((winwidth-start_x),0);
			DrawLine(0,fH);
			DrawLine(-(winwidth-start_x),0);
			DrawLine(0,-fH);
			EndFilledRegion();
			start_x=pretext;	
		}
		else
		{
			StartFilledRegion(1);
			MovePen(start_x,start_y);   
			DrawLine((cur_x-start_x),0);
			DrawLine(0,fH);
			DrawLine(-(cur_x-start_x),0);
			DrawLine(0,-fH);
			EndFilledRegion();
		}	
	}
	start_y=ly;
	start_x=lx;
	cur_x=repo_curx;
	cur_y=repo_cury;
}

void drawPasteHelper()
{
	double offset=0.2;
	if( button(1,mx+offset, my+offset, winwidth/8, h , "Cut"))
	{	
		//Cut();
	}
}
//����ѡ������ 
void Copy(int x1,int y1,int x2,int y2) //xΪ���±꣬yΪ���±� ,1Ϊ���Ͻǣ�2Ϊ���½�   //
{
	int i,j;
	if(y1==y2)
	{
		for(j=x1;j<=x2;j++)
		{
			cutboard[strlen(cutboard)]=input[y1][j];
		} 
	}
	else
	{
		strcat(cutboard,input[y1]+x1);
		for(i=y1+1;i<=y2-1;i++)
		{	
			strcat(cutboard,input[i]);
		} 
		for(j=0;j<=x2;j++)
		{
			cutboard[strlen(cutboard)]=input[y2][j];
		} 		
	}	
}
//ɾ��ѡ������ 
void SelectedZoneDelet(int x1,int y1,int x2,int y2) //xΪ���±꣬yΪ���±� ,1Ϊ���Ͻǣ�2Ϊ���½�  
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
		
		for(i=0;i<y1;i++)//����0~y1-1�� 
		{
			strcat(StringLine,input[i]);
			if(strlen(input[i])<79)
				StringLine[strlen(StringLine)]='\n';
		}
		
		strcat(StringLine,input[i]);//����y1��
		if(f)
			StringLine[strlen(StringLine)]='\n';
			
		for(i=y1+1;i<n-1;i++)	//����y1~n�� 
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
//ճ��
void  Paste(int x,int y)
{
	int i,j;
	char StringLine[1600]={};
	char StringTmp[1600]={}; 
	int count=0;
	//����x��y�ǵڼ����ַ�������\n 
	for(i=0;i<y;i++)
	{
		for(j=0;j<79;j++)
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
	strcpy(StringLine+count,cutboard);
	strcat(StringLine,StringTmp);
	
	TransTwo(StringLine);
} 
//���С����ȸ���ѡ�����򣬺�ɾ��ѡ������ 
void Cut(int x1,int y1,int x2,int y2) //xΪ���±꣬yΪ���±� ,1Ϊ���Ͻǣ�2Ϊ���½� 
{
	char TextString[1600]={};
	Copy(x1,y1,x2,y2); 
	SelectedZoneDelet(x1,y1,x2,y2); 
}

//����ָ������ 
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
void Replace(int x,int y,char *search,char *replacement) 	//	��ʼ�滻λ�ã�xyͬ�ϣ������滻�����ݣ��滻�������ݣ���λ�ô����滻���ݲ������򲻻��滻 
{
	int i,j;
	int f=1;
	char StringLine[1600]={};
	char StringTmp[1600]={}; 
	int count=0;
	
	TransOne(StringLine);
	//����x��y�ǵڼ����ַ�������\n 
	for(i=0;i<x;i++)
	{
		for(j=0;j<79;j++)
		{
			count++;
			if(input[i][j]=='\0')
				break;
		}
	}
	count=count+y;//x,yתΪһά����±�
	for(i=0;i<strlen(search);i++)
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


void display()
{
	if(page==0)
	{
		DisplayClear();
		drawMenu();
	}
	if(page==1)
	{
		DisplayClear();
		drawBackground();
		if(focusMode||lockMode)
		{
			drawFocus();
		}
		drawMenu();
		drawEditor();
		drawEditText();
		DrawEditedText();
		if(pasteMode)
		{
			drawPasteHelper();
		}
	}
}

void setconfig(int timerID)
{
	switch(timerID)
	{
		case MouseTimer:
		{
			display();
			break;
		}
	};
}

void Main()
{
	InitGraphics();
	init_color();								//��ʼ����ɫ���� 
	
    winwidth = GetWindowWidth();
    winheight = GetWindowHeight();
    
	pretext = GetFontAscent()/2.0+0.4;			//�Ӵ�������˵���һ�����ִ�ӡ�ľ��� 
	fH = GetFontHeight();						//����߶� 
	h = fH*2;									// �����ı��߶� 
	
	registerCharEvent(CharEventProcess);        // �ַ�
	registerKeyboardEvent(KeyboardEventProcess);// ����
	registerMouseEvent(MouseEventProcess);      // ���
	registerTimerEvent(setconfig);				//�����˸ 
	
	startTimer(MouseTimer,200);
}

