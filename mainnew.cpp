#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "imgui.h"
#include <windows.h>
#include <wingdi.h>
#include <windef.h>
#include <winuser.h>
#include <math.h>
#define MAXLINE 20

static double winwidth,winheight; 
static double set_y;
static double fontsize;
static char alignment;
static int position=0;
static int moveCursor;
static int curLine=20;
static int page=0;
static char filename[40];
static char input[MAXLINE][80];
static double mx,my;				//��ǰ�����λ�� 
static double lx,ly;				//�����һ�ΰ��������λ�� 
static double rx,ry;				//�����һ�ΰ����Ҽ���λ�� 
static double start_x,start_y;		// ��ʼ���Ƹ�������ʼ�� 
static double cur_x,cur_y;			//(����)���Ƹ��� ����ϻ��Ƹ��� 
static double pretext;				//�����������������ʾ�ľ��� 
static double fH;					//���ֵĸ߶� 
static double h;					//���еĸ߶� 
static double setwhite = 0.4;		//�����������߿�ľ��� 
static char cutboard[200];			//���а� 
static char searchboard[80]; 
static char replacement[80];
static FILE *fp;
struct SearchPosition *head=NULL;
static int SearchCount;
static char tmp[80]; 
static int saveFlag=0,				//�����ʶ 
           fileFlag=0,
           gridFlag=0,
           colorFlag=0,
		   searchFlag=0,
		   replaceFlag=0;
static int  pasteMode=0,
			focusMode=0,
			colorMode=0,
			lockMode=0;
			 

typedef enum {
	MouseTimer,
} MyTimer;

struct SearchPosition{
	int x;
	int y;
	struct SearchPosition *next; 
};
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
	"New File    | Ctrl-N", 
	"Open File   | Ctrl-O",
	"Save File	 | Ctrl-S",
	"Save New ",
	"Close File ",
	"Exit | Ctrl-Q"};
static char * menuListPreference[] = {"Preference",
	"FontSize",
	"FontStyle",
	"GridFormat"};
static char * menuListPaste[] = {"Edit",  
	"Cut   | Ctrl-X", 
	"Copy  | Ctrl-C",
	"Paste | Ctrl-V",
	"Delete | Ctrl-D"};
static char * menuListSearch[] = {"Help", 
	"Search	  | Ctrl-F",
	"Replace  | Ctrl-R"};
static char * menuListColor[] = {"BackgroundColor",
"Green",
"Blue",
"Yellow",
"white"};

void init_color();
void display();
void drawEditText();
int getMoveCursor();

void TransOne(char *StringLine);
void Paste(int x,int y);
void Copy(int x1,int y1,int x2,int y2);
void Cut(int x1,int y1,int x2,int y2);
void SelectedZoneDelet(int x1,int y1,int x2,int y2) ;
int Search(char *Text,int *Pxy);
void Replace(char *replacement); 
void SelectedZoneDelete(int x1,int y1,int x2,int y2);
void TextSave(FILE *fp); 
void FreeStruct(struct SearchPosition *p);


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
	DefineColor("lightPurple",0.9,0.821,0.9);
}

int inText(double mx,double my)		  //�жϵ�������Ƿ����ı����� 
{
	return (mx >= setwhite && my <= winheight-fH*3)?1:0;
}

double nearestCharX(double x,double y) //�����ڵ�ǰ�����������¾������x����ַ���xλ�� 
{
	int cursorLine = AnalyzeY(y);
	int len = strlen(input[cursorLine]);
	int i,j;
	for(i=0;i<len;i++)
	{
		for(j=0;j<=i;j++)
		{
			tmp[j]=input[cursorLine][j];
		}
		tmp[j]='\0';
		if(x >= TextStringWidth(tmp)+pretext)
		{
			tmp[j]=input[cursorLine][j];
			tmp[j+1]='\0';
			if(x <= TextStringWidth(tmp)+pretext )
			{
				return TextStringWidth(tmp)+0.4;
			}
		}
	}
	if(x >= TextStringWidth(input[cursorLine])+pretext)
	{
		return TextStringWidth(input[cursorLine])+pretext;
	}
	if(x <= pretext)
	{
		return pretext;
	}
}

double nearestCharY(double ly)	//����ly�����ֵ׶˵ĵ�yֵ 
{
	int i; 
	for(i=0;i<20;i++)
	{
		if(ly <= set_y-i*h && ly >= set_y-(i+1)*h)
		{
			return set_y-(i+1)*h+fH/2.0; 
		}
	}
	return 0;
}

int AnalyzeX(double x,double y) //����������������һ���汾�����غ��ı�������ص��±� 
{
	int cursorLine = AnalyzeY(y);
	int len = strlen(input[cursorLine]);
	int i,j;
	for(i=0;i<len;i++)
	{
		for(j=0;j<=i;j++)
		{
			tmp[j]=input[cursorLine][j];
		}
		tmp[j]='\0';
		if(x >= TextStringWidth(tmp)+pretext )
		{
			tmp[j]=input[cursorLine][j];
			tmp[j+1]='\0';
			if(x <= TextStringWidth(tmp)+pretext )
			{
				return j;
			}
		}
	}
	if(x >= TextStringWidth(input[cursorLine])+pretext)
	{
		return strlen(input[cursorLine]);
	}
}

int AnalyzeY(double y)	
{
	int i;
	for(i=0;i<20;i++)
	{
		if(y <= set_y-i*h && y >= set_y-(i+1)*h)
		{
			return i;
		}
	}
	if(position)
	{
		return position;
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

void TextSave(FILE *fp)		//��input���浽fp�ļ��� 
{
	char TextString[800]={};
	TransOne(TextString);
	fprintf(fp,"%s",TextString);
} 

void ReadLines(FILE *fp)	//�ļ���ȡ���� 
{
	char Test[80] = {0};
	char TextString[200]={0}; 
	while(!feof(fp))  
	{          
		memset(Test,0, sizeof(Test));                 
		fgets(Test,sizeof(Test) - 1, fp);            
		strcat(TextString,Test);	
	} 
	TransTwo(TextString);
}

int CountLines(char StringArray[][80])	//ͳ�������ݵ��������������м�ֻ��\0���У����У� 
{
	int n=0;   
	while(strlen(StringArray[19-n])==0)
	{
		n++;
	}
	return 20-n;
} 

void TransOne(char *StringLine)	//����ά����input��Ϊһά���鴢�浽StringLine 
{
	int n=0;
	int i=0;
	
	n=max(CountLines(input),position+1); 
	
	for(i=0;i<n-1;i++)
	{
		strcat(StringLine,input[i]);
		if(strlen(input[i])<79)
			StringLine[strlen(StringLine)]='\n';
	}
	strcat(StringLine,input[i]);
}

int TransTwo(char *StringLine)	//��StringLineת��Ϊ��ά����浽input�� 
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

void moveLines(int start,int flag)	//flag��ʾѡ����ϵ��»��Ǵ��µ��� 
{
	int i;	
	if(flag)
	{
		for(i=curLine;i>start+1;i--)
		{
			strcpy(input[i],input[i-1]);
		}
		memset(input[start+1],0,sizeof(input[start+1]));
		position++;		
	}
	else
	{
		for(i=start;i<curLine-1;i++)
		{
			strcpy(input[i],input[i+1]);
		}
		position--;
	}
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
				if(position)
				{
					moveLines(position,1);
				}
				break;
			}
			else if(key==VK_BACK)
			{
				if( (strlen(input[position])) == 0)
				{
					moveLines(position,0);
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
				pasteMode=0;
				if(inText(mx,my))
				{
					focusMode = 1;
					lockMode = 0;
				}
				position=AnalyzeY(my);
				lx = nearestCharX(mx,my);	//needed for focusMode.
				ly = nearestCharY(my);
			}
			else if(button==RIGHT_BUTTON)
			{
				pasteMode = 1;
				rx=mx,ry=my;
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
	if(colorFlag != 0 )
	{
		SetPenColor(color[colorMode].light);
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
	double w = TextStringWidth(menuListPreference[0])*1.8; // �ؼ�����
	double wlist = TextStringWidth(menuListPreference[0])*1.8;
	int    selection;
	
	drawMenuBar(0,y-h,winwidth,h);

	selection = menuList(GenUIID(0), x, y-h, w, wlist, h, menuListFile, sizeof(menuListFile)/sizeof(menuListFile[0]));
	if(selection>0) 
	{
		switch(selection)
		{
			case 1:
			{
				page=1,fileFlag=1;
				break;
			}
			case 2:
			{
				page=1;
				if(saveFlag==0&&fileFlag==1)
				{
					int choose;
					choose=MessageBox ( NULL, TEXT ("�����ı���"), TEXT ("Confirm"), MB_OKCANCEL|MB_ICONWARNING);
					switch(choose)
					{
						case 1://set "save" module here
						{
							if((fp = fopen(filename,"w+"))!=0)
							{
								TextSave(fp);
							}
							fclose(fp);
							page=0;
							saveFlag==1;
							break;
						}
						case 2:page=0;break;
					};
				}
				fileFlag=1;
				openfile(filename);
				if((fp = fopen(filename,"r+"))!=0)
				{
					ReadLines(fp);
				}
				fclose(fp);
				break;
			}
			case 3:
			{
				saveFlag=1;
				if((fp = fopen(filename,"w+"))!=0)
				{
					TextSave(fp);
				}
				fclose(fp);
				break;
			}
			case 4:
			{
				if(fileFlag==1)
				{
					if((fp = fopen("NewText.txt","w+"))!=0)
					{
						TextSave(fp);
						strcpy(filename,"NewText.txt");
					}
					fclose(fp);
				}
			
				break;
			}
			case 5:
			{
				int choose;
				if(fileFlag==1&&saveFlag==0)
				{
					choose=MessageBox ( NULL, TEXT ("�����ı���"), TEXT ("Confirm"), MB_OKCANCEL|MB_ICONWARNING);
					switch(choose)
					{
						case 1://set "save" module here
						{
							if((fp = fopen(filename,"w+"))!=0)
							{
								TextSave(fp);
							}
							fclose(fp);
							page=0;
							saveFlag==1;
							break;
						}
						case 2:page=0;;break;
					};
				}
				break;
			}
			case 6:
			{
				if(saveFlag==1)
				{
					exit(-1);	
				}
				else
				{
					int choose;
					choose=MessageBox ( NULL, TEXT ("�����ı���"), TEXT ("Confirm"), MB_OKCANCEL|MB_ICONWARNING);
					switch(choose)
					{
						case 1:
						{
							if((fp = fopen(filename,"w+"))!=0)
							{
								TextSave(fp);
							}
							fclose(fp);
							page=0;
							break; 
						}
						case 2:exit(-1);break;
					};
					exit(-1);
				}
			}
		};		
	}
	
	selection = menuList(GenUIID(0),x+w, y-h, w, wlist,h,menuListPreference,sizeof(menuListPreference)/sizeof(menuListPreference[0]));
	if( selection>0 ) 
	{
		switch(selection)
		{
			case 1:
			{
				SetPenSize(2);
				break;
			}
			case 2:
			{
				SetStyle(2);
				break;
				
			}
			case 3:
			{
				if(gridFlag==1) gridFlag=0;
				else gridFlag=1;
				break;
			}
		}
	};
	
	selection = menuList(GenUIID(0),x+2*w,  y-h, w, wlist,h, menuListPaste,sizeof(menuListPaste)/sizeof(menuListPaste[0]));
	if( selection>0 ) 
	{
		switch(selection)
		{
			case 1:
			{
				lockMode=0;
				memset(cutboard,0,sizeof(cutboard));
				Cut(AnalyzeX(lx,ly),AnalyzeY(ly),AnalyzeX(cur_x,cur_y)-1,AnalyzeY(cur_y));
				break;
			}
			case 2:
			{
				lockMode=0;
				memset(cutboard,0,sizeof(cutboard));
				Copy(AnalyzeX(lx,ly),AnalyzeY(ly),AnalyzeX(cur_x,cur_y)-1,AnalyzeY(cur_y));
				break;
			}
			case 3:
			{
				Paste(AnalyzeX(lx,ly),position);
				break;
			}
			case 4:
			{
				SelectedZoneDelete(AnalyzeX(lx,ly),AnalyzeY(ly),AnalyzeX(cur_x,cur_y)-1,AnalyzeY(cur_y));
				break;
			}
		}
	};
	
	selection = menuList(GenUIID(0),x+3*w,  y-h, w, wlist,h, menuListSearch,sizeof(menuListSearch)/sizeof(menuListSearch[0]));
	if( selection>0 ) 
	{
		switch(selection)
		{
			case 1:
			{
				if(searchFlag==1)
				{
					int i; 
					int searchpolines[80];
					struct SearchPosition *tail=NULL,*p;
					
					FreeStruct(head); 
					SearchCount=Search(searchboard,searchpolines);
					//��λ����Ϣ��¼�������� 
					for (i = 0; i < SearchCount; i++)
					{
						p=(struct SearchPosition *)malloc(sizeof(struct SearchPosition));
						p->x=searchpolines[i*2+1];
						p->y=searchpolines[i*2];
						p->next=NULL;
						if(head==NULL)
						{
							head=p;
						} 
						else
						{
							tail->next=p;
						}
						tail=p;	
					}
					searchFlag=0;
				}
				else
				{
					searchFlag=1;
				}
				break;
			}
			case 2:
			{
				if(replaceFlag==1)
				{
					Replace(replacement);
					replaceFlag=0;
				}
				else
					replaceFlag=1;
				break;
			}
		}
	};
	
	selection = menuList(GenUIID(0),x+4*w,  y-h, w, wlist,h, menuListColor,sizeof(menuListColor)/sizeof(menuListColor[0]));
	{
		switch(selection)
		{
			case 1:colorFlag=1,colorMode=0;break;
			case 2:colorFlag=1,colorMode=1;break;
			case 3:colorFlag=1,colorMode=2;break;
			case 4:colorFlag=0;break;
		}

	}
}
//�ͷ����� 
void FreeStruct(struct SearchPosition *p)
{
	if(p!=NULL)
	{
		if(p->next==NULL)
		{
			 free(p);
		}		
		else
		{ 
			FreeStruct(p->next);
			free(p);
		}
	}

}

void drawEditor()
{	
	int i=0;
	
	if(gridFlag==0) 
	{
		if(colorFlag==0)
		{
			SetPenColor("white");
		}
		else
		{
			SetPenColor(color[colorMode].light);
		}
	}
	else 
	{
		SetPenColor("blue");
	}
	
	MovePen(setwhite,set_y);
	DrawLine(0,-winheight);
	MovePen(setwhite,set_y);
	DrawLine(winwidth-2*setwhite,0);
	MovePen(winwidth-setwhite,set_y); //'winwidth-setwhite"�������Ҳ��xֵ 
	DrawLine(0,-winwidth);
	
	for(i=0;i<20;i++)
	{
		MovePen(setwhite,set_y-i*h);
		DrawLine(winwidth-2*setwhite,0);
	}
}

void DrawEditedText()
{
	double y=winheight-fH*3-fH*2; //��Ϊ������һ��ֱ���Ƿָ��ߣ����������õڶ���ֱ�߿�ʼ���ı��� 
	alignment='L';
	
	int i=0;
	for(i=0;i<20;i++)
	{
		SetPenColor("white");
		if(i!=position)
		{
			drawBox(setwhite,y-fH*2*i,winwidth-setwhite*2,h,0,input[i],alignment,"blue");
		}
	}
}

void drawEditText()
{
	double x=0;
	double y=winheight-fH*3-fH*2; //��Ϊ������һ��ֱ���Ƿָ��ߣ����������õڶ���ֱ�߿�ʼ���ı��� 
	double w = winwidth; // �ؼ�����
	
	switch(position)
	{
		case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:
		case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:
		textbox(GenUIID(0),x+setwhite,y-fH*2*position,w-2*setwhite,h,input[position],sizeof(input[position]),&moveCursor);
		break;
	};
}

void drawboard()
{
	double x = 0;
	double y = winheight;
	double h = fH*1.5; // �ؼ��߶�
	double w = TextStringWidth(menuListPreference[0])*1.8; // �ؼ�����
	if(searchFlag==1)
	{
		drawLabel(x+6*w,  y-0.8*h,"search:");
		textbox(GenUIID(0),x+6.6*w,  y-h, w,h,searchboard,sizeof(searchboard),&moveCursor);
	}
	if(replaceFlag==1)
	{
		drawLabel(x+5.71*w,  y-1.8*h,"replacement:");
		textbox(GenUIID(0),x+6.6*w,  y-2*h, w,h,replacement,sizeof(replacement),&moveCursor);
	}
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
	
	SetPenColor("lightPurple");
	for(;fabs(start_y - cur_y) < 0.001 ||start_y >= cur_y; start_y-=h ) //���ǵ��������ıȽ��ǲ�ȷ���ģ���������һ������ 
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
	double offset=0.05;
	if(button(GenUIID(0),rx+offset, ry-h-offset, winwidth/8, h ,"Cut"))
	{	
		pasteMode=0;
		Cut(AnalyzeX(lx,ly),AnalyzeY(ly),AnalyzeX(cur_x,cur_y)-1,AnalyzeY(cur_y));
	}
	if(button(GenUIID(0),rx+offset,ry-2*h-offset,winwidth/8,h,"Copy"))
	{
		pasteMode=0,lockMode=0;
		Copy(AnalyzeX(lx,ly),AnalyzeY(ly),AnalyzeX(cur_x,cur_y)-1,AnalyzeY(cur_y));
	}
	if(button(GenUIID(0),rx+offset,ry-3*h-offset,winwidth/8,h,"Paste"))
	{
		pasteMode=0;
		Paste(AnalyzeX(lx,ly),position);
	}
	if(button(GenUIID(0),rx+offset,ry-4*h-offset,winwidth/8,h,"Search"))
	{
		pasteMode=0;
		if(searchFlag==1)
		{
			int i; 
			int searchpolines[80];
			struct SearchPosition *tail=NULL,*p;
		
			FreeStruct(head); 
			SearchCount=Search(searchboard,searchpolines);
			//��λ����Ϣ��¼�������� 
			for (i = 0; i < SearchCount; i++)
			{
				p=(struct SearchPosition *)malloc(sizeof(struct SearchPosition));
				p->x=searchpolines[i*2+1];
				p->y=searchpolines[i*2];
				p->next=NULL;
				if(head==NULL)
				{
					head=p;
				} 
				else
				{
					tail->next=p;
				}
				tail=p;	
			}
			searchFlag=0;
		}
		else
		{
			searchFlag=1;
		}
	}
	if(button(GenUIID(0),rx+offset,ry-5*h-offset,winwidth/8,h,"Replace"))
	{
		pasteMode=0;
		if(replaceFlag==1)
		{
			Replace(replacement);
			replaceFlag=0;
		}
		else
			replaceFlag=1;
	}
}

void Copy(int x1,int y1,int x2,int y2) //����ѡ������ 
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

void SelectedZoneDelete(int x1,int y1,int x2,int y2) //ɾ��ѡ������   
{
	char StringLine[200]={};
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
		for(i=y1+1;i<=y2-1;i++)
			input[i][0]='\0';
			
		for(i=0;i<y1;i++)  //����0~y1-1�� 
		{
			strcat(StringLine,input[i]);
			if(strlen(input[i])<79)
				StringLine[strlen(StringLine)]='\n';
		}
		i=y1;
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

void Paste(int x,int y)	//ճ��
{
	int i,j;
	char StringLine[200]={};
	char StringTmp[200]={}; 
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

void Cut(int x1,int y1,int x2,int y2) //����--�ȸ���ѡ�����򣬺�ɾ��ѡ������  
{
	char TextString[200]={};
	Copy(x1,y1,x2,y2); 
	SelectedZoneDelete(x1,y1,x2,y2); 
}

int Search(char *Text,int *Pxy)   //����ָ�����ݣ�����������Ϊx���꣬��������ż����0,2,4Ϊy���꣬������ 
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
	return c;
}  

void Replace(char *replacement) 	//�滻ָ�����ݣ���ʼ�滻λ�ã�xyͬ�ϣ������滻�����ݣ��滻�������ݣ���λ�ô����滻���ݲ������򲻻��滻 
{
	int i=0,j=0;
	char StringLine[800]={};
	char StringTmp[400]={}; 
	
	TransOne(StringLine);
	while(i<strlen(StringLine))
	{
		if(StringLine[i]==*searchboard)
		{
			for(j=1;j<strlen(searchboard);j++)
			{
				if(searchboard[j]!=StringLine[i+j])
				{
					break;
				}
			}
			if(j==strlen(searchboard))
			{
				strcpy(StringTmp,StringLine+i+strlen(searchboard));
				strcpy(StringLine+i,replacement);
				strcat(StringLine,StringTmp);
				i=i+strlen(replacement)-1;
			}
		}
		i++;
	}
	TransTwo(StringLine); 	
}

void synchronize()			//needed for paste with keyboard.
{
	
}

void display()
{
	if(page==0)
	{
		DisplayClear();
		drawBackground();
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
		drawEditText();
		DrawEditedText();
		drawEditor();
		if(pasteMode)
		{
			drawPasteHelper();
		}
		drawMenu();		
	}
	drawboard();
	
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
	set_y=winheight-fH*3;						//set_y�ǵ�һ�еĸ߶� 
	
	registerCharEvent(CharEventProcess);        // �ַ�
	registerKeyboardEvent(KeyboardEventProcess);// ����
	registerMouseEvent(MouseEventProcess);      // ���
	registerTimerEvent(setconfig);				//�����˸ 
	
	startTimer(MouseTimer,200);
}
