#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <GL/glut.h>
#include <unistd.h>
#include <string.h>

using namespace std;

int focus=-1;
int focus_prev=-1;
int exit_condn=0;
int key_cond=0;
long npop=0;
float nlif=0.0;
int ninc=0;
int p=0;
int space_count=0;
float color[20][3]={{102,0,0},{255,0,0},{255,204,204},{255,153,51},{255,229,204},{255,255,51},{76,153,0},{0,102,0},{0,255,255},
					{0,153,153},{51,51,255}, {178,102,255},{255,102,255},{102,0,102},{255,0,127},{153,0,76},{128,128,128},{204,255,229},
					{255,204,153},{204,229,255}};

char nat[20][20]={"AL","AU","BD","BR","CA","CN","EG","FR","DE","GR","HU","IN","IR","IT","JP","MX","NZ","RU","UK","US"};
char code[20][3]={"AL","AU","BD","BR","CA","CN","EG","FR","DE","GR","HU","IN","IR","IT","JP","MX","NZ","RU","UK","US"};
int income[20][81];
float life[20][81];
long population[20][81];
int year=1800;

class country
{
	private:
		long pop;
		float lif;
		int inc;
		float col[3];
		int x,y;
		float r;
	
	public:
		void init_color(int i);
		void assign(int i,int j);
		void circle_plot();
		void map_xy();
		void scale_size();
		int coverage(int xp,int yp,int i);
		void get_data();
};

country nation[20];

void country::init_color(int i)
{
	col[0]=(color[i][0])/255;
	col[1]=(color[i][1])/255;
	col[2]=(color[i][2])/255;
}

void country::assign(int i,int j)
{
	pop=population[j][i];
	lif=life[j][i];
	inc=income[j][i];
}

void country::get_data()
{
	npop=pop;
	nlif=lif;
	ninc=inc;
}

void country::circle_plot()
{	
	glColor3fv(col);
	glBegin(GL_POLYGON); 
	for(float i = 0; i< 360; i++) 
	{ 
		float cx=r*cosf(i*3.14/180);
		float cy=r*sinf(i*3.14/180)*1.25;
		glVertex2f(cx+x,cy+y);
	}
	glEnd();

	glColor3f(0,0,0);
	glLineWidth(1);
	glBegin(GL_LINE_LOOP); 
	for(int i = 0; i< 360; i++) 
	{ 
		float cx=r*cosf(i*3.14/180.0);
		float cy=r*sinf(i*3.14/180.0)*1.25;
		glVertex2f(cx+x,cy+y);
	}
	glEnd();
}

void country::map_xy()
{
	y=75+(lif-20.0)*12.0;
	x=75+((log(inc)-4)*9*log(inc));
}

void country::scale_size()
{
	r=(sqrt(pop)/1533.0)+1;
}

void plot_axis()
{
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(1);
	glColor3f(0.87,0.87,0.87);

	glBegin(GL_LINES);
	for(int i=0;i<=7;i++)
	{
		int x1=75+(i*9*(i+4));
		int x2=75+((i+1)*9*(i+5));
		int gap=(x2-x1)/6;
		for(int j=1;j<=5;j++)
		{
			glVertex2f(x1,75);
			glVertex2f(x1,890);
			x1+=gap;
		}
	}
	for(int i=0;i<=13;i++)
	{
		int y1=75+(i*60);
		int y2=75+((i+1)*60);
		int gap=(y2-y1)/5;
		for(int j=1;j<=5;j++)
		{
			if(i==13 && j==5)
				continue;
			glVertex2f(75,y1);
			glVertex2f(880,y1);
			y1+=gap;
		}
	}
	glEnd();

	glColor3f(0,0,0);
	glLineWidth(2);

	glBegin(GL_LINE_STRIP);
		glVertex2f(75,900);
		glVertex2f(75,75);
		glVertex2f(900,75);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex2f(70,895);
		glVertex2f(75,900);
		glVertex2f(80,895);
	glEnd();
		
	glBegin(GL_LINE_STRIP);
		glVertex2f(895,80);
		glVertex2f(900,75);
		glVertex2f(895,70);
	glEnd();
	
	glPointSize(5);
	int startx=25;
	glColor3f(0,0,0);
	for(int i=1;i<=13;i++)
	{
		glBegin(GL_POINTS);
			glVertex2f(75,75+((startx-20)*12));
		glEnd();	

		char a=(char)((startx/10)+(int)'0');
		char b=(char)((startx%10)+(int)'0');
		glRasterPos2d(45,70+i*60);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,a);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,b);
		startx+=5;
	}
		
	double e=exp(1);
	double starty=pow(e,5);
	int x[7]={150,400,1000,3000,8000,22000,60000};
	for(int i=1;i<=7;i++)
	{
		glBegin(GL_POINTS);
			glVertex2f(75+((log(starty)-4)*9*log(starty)),75);
		glEnd();

		int val=i+4;
		char a=(char)(val/10+(int)'0');
		char b=(char)(val%10+(int)'0');

		glRasterPos2d(75+((log(starty)-4)*9*log(starty))-15,50);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'e');
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'^');
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,a);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,b);

		val=x[i-1];
		glRasterPos2d(75+((log(starty)-4)*9*log(starty))-22,27);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'(');
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'~');		

		char dis[10];
		int count=-1;
		while(val!=0)
		{
			count+=1;
			dis[count]=(char)(val%10+(int)'0');
			val/=10;
		}
		
		while(count!=-1)
		{
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13,dis[count]);
			count-=1;
		}
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,')');

		starty*=e;
	}

	glRasterPos2d(325,5);
	char a1[50]="Income per Person (GDP/Capita) (log)";
	for(int i=0;i<strlen(a1);i++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,a1[i]);
	char a2[50]="Life Expectancy";
	for(int i=0;i<strlen(a2);i++)
	{
		glRasterPos2d(20,650-i*18);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,a2[i]);			
	}
	glRasterPos2d(0,350);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'(');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'y');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'e');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'a');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'r');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'s');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,')');

	char y[4];
	if(year>2015)
		year=2015;
	y[0]=((year/1000)%10)+(int)'0';
	y[1]=((year/100)%10)+(int)'0';
	y[2]=((year/10)%10)+(int)'0';
	y[3]=(year%10)+(int)'0';

	
	glColor3f(0.75,0.75,0.75);
	glPushMatrix();
	glLineWidth(6);
	glPointSize(1);

	glTranslatef(300,450,0);
	for(int i=0;i<4;i++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,y[i]);
	glPopMatrix();

	glColor3f(0,0,0);
	glRasterPos2d(300,920);
	char title[50]="WEALTH & HEALTH OF NATIONS";
	for(int i=0;i<strlen(title);i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,title[i]);

	glLineWidth(1);
	for(int i=0;i<20*30;i+=30)
	{
		for(int j=0;j<2;j++)
		{
			if(j==0)
			{
				glColor3f(color[i/30][0]/255,color[i/30][1]/255,color[i/30][2]/255);
				glBegin(GL_POLYGON);
				glVertex2f(j+890,i+200);
				glVertex2f(j+890+50,i+200);
				glVertex2f(j+890+50,i+30+200);
				glVertex2f(j+890,i+30+200);
				glEnd();
			}
			else
			{
				glColor3f(0,0,0);
				glBegin(GL_LINE_LOOP);
				glVertex2f(j+890,i+200);
				glVertex2f(j+890+50,i+200);
				glVertex2f(j+890+50,i+30+200);
				glVertex2f(j+890,i+30+200);
				glEnd();
				glBegin(GL_LINE_LOOP);
				glVertex2f(j+890+50,i+200);
				glVertex2f(j+890+100,i+200);
				glVertex2f(j+890+100,i+30+200);
				glVertex2f(j+890+50,i+30+200);
				glEnd();
				glRasterPos2d(j+900+50,i+200+10);
				glutBitmapCharacter(GLUT_BITMAP_8_BY_13,code[i/30][0]);
				glutBitmapCharacter(GLUT_BITMAP_8_BY_13,code[i/30][1]);
			}
		}
	}
	glutSwapBuffers();
}

int country::coverage(int xp,int yp,int i)
{
	if(abs(xp-x)<10 && abs(yp-y)<10)
		return 1;
	else
		return 0;

}

void hover(int x,int y)
{
	for(int i=0;i<20;i++)
	{
		int x1=(x/900.0)*1000;
		int y1=((720-y)/720.0)*1000;
		if(nation[i].coverage(x1,y1,i))
			focus=i;
	}
}

void init()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,1000,0,1000);
	glMatrixMode(GL_MODELVIEW);
}

void key(unsigned char ch,int x,int y)
{
	if(ch=='s'||ch=='S')
		key_cond=1;
	else if(ch=='q'||ch=='Q')
		key_cond=2;
	else if(ch==' ')
	{
		space_count+=1;
		if(space_count%2==1)
			key_cond=3;
		else if(space_count%2==0)
			key_cond=1;
	}
		
}

void display_init()
{
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0,0,0);
	glRasterPos2d(375,450);
	char begin[50]="Press S to get Started";
	for(int i=0;i<strlen(begin);i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,begin[i]);
	glutSwapBuffers();
}

void display_start()
{
	if(p==0)
	{
		char timer[4]={'3','2','1','0'};
		glLineWidth(6);
		for(int i=0;i<4;i++)
		{
			glClearColor(1,1,1,1);
			glClear(GL_COLOR_BUFFER_BIT);
			glColor3f(0.5,0.5,0.5);		
			glPushMatrix();
			glTranslatef(450,450,0);
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,timer[i]);
			glPopMatrix();
			glutSwapBuffers();
			sleep(1);
		}	
	}
	if(p<81)
	{
		plot_axis();
		for(int q=0;q<20;q++)
		{
			nation[q].assign(p,q);
			nation[q].scale_size();
			nation[q].map_xy();
			nation[q].circle_plot();
			glutSwapBuffers();
		}	

		sleep(1);
	
		if(year>=1950)
			year+=1;
		else
			year+=10;
		p+=1;
	}
}

void display_hover()
{
	plot_axis();
	for(int i=0;i<20;i++)
		nation[i].circle_plot();
	
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
		glVertex2f(890,930);
		glVertex2f(990,930);
		glVertex2f(990,810);
		glVertex2f(890,810);
	glEnd();

	glRasterPos2d(895,905);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15 ,'N');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15 ,'a');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15 ,'t');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15 ,':');
	for(int i=0;i<strlen(nat[focus]);i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15 ,nat[focus][i]);

	nation[focus].get_data();
	int p1=npop/10000000;
	int l1=(int)nlif;
	int i1=ninc;
	npop=nlif=ninc=0;

	char dis[10];
	
	int count=-1;
	glRasterPos2d(895,880);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'P');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'o');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'p');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,':');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'0');
	while(p1!=0)
	{
		count+=1;
		dis[count]=(char)(p1%10+(int)'0');
		p1/=10;
	}
	while(count!=-1)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,dis[count]);
		count-=1;
	}
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'C');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'r');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'.');

	count=-1;
	glRasterPos2d(895,855);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'L');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'i');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'f');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,':');
	while(l1!=0)
	{
		count+=1;
		dis[count]=(char)(l1%10+(int)'0');
		l1/=10;
	}
	while(count!=-1)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,dis[count]);
		count-=1;
	}

	count=-1;
	glRasterPos2d(895,830);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'I');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'n');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'c');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,':');
	while(i1!=0)
	{
		count+=1;
		dis[count]=(char)(i1%10+(int)'0');
		i1/=10;
	}
	while(count!=-1)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,dis[count]);
		count-=1;
	}

	focus_prev=focus;
	focus=-1;
	glutSwapBuffers();
}

void display_end()
{
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT);

	glRasterPos2d(400,500);
	glColor3f(0,0,0);
	char begin[50]="Thank You !";
	for(int i=0;i<strlen(begin);i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,begin[i]);
	glutSwapBuffers();	
	sleep(3);
	exit(0);
}

void idle()
{	
	glutPostRedisplay();
}

void display()
{

	if(key_cond==0)
		display_init();

	else if(key_cond==1 && exit_condn==0)
	{
		display_start();
		if(p>80)
			exit_condn=1;
	}

	else if(key_cond==2 && exit_condn==1)
		display_end();

	else if(key_cond==3)
		if(focus!=-1)
			if(focus!=focus_prev)
				display_hover();

	if(focus!=-1 && exit_condn==1)
		if(focus!=focus_prev)
			display_hover();

	glutPassiveMotionFunc(hover);
	glutSwapBuffers();
}
		
template <class GT>
void get_arr_from_file(ifstream &fh, GT arr[20][81])
{
	string line;
	for(int i=0;i<20;i++)
	{
		getline(fh,line);

		int j = 0;
		GT fval;
		string sval;
	    stringstream ssin(line);
	    while (ssin.good() && j < 81)
	    {
	    	stringstream ss;
	        ssin>>sval;
	        ss<<sval;
	        ss>>fval;
	        arr[i][j]=fval;
	        ++j;
	    }
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(900,720);
	glutInitWindowPosition(10,10);
	
	for(int i=0;i<20;i++)
		nation[i].init_color(i);

	ifstream inc("income");
	ifstream lif("life");
	ifstream pop("population");
	
	get_arr_from_file(inc,income);
	get_arr_from_file(lif,life);
	get_arr_from_file(pop,population);

	glutCreateWindow("Graph");
	init();
	glutKeyboardFunc(key);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMainLoop();
 	return 0;
}