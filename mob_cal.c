/*  MOBILE CALENDAR ENHANCED WITH A BIT OF MOUSE PROGRAMMING
		    SHATROOPA SAXENA
*/
#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<stdlib.h>
struct date d;
struct time t;
union REGS in,out;
int cpx,cpy;
int dm()//detects whether mouse programming is active or not
{
	in.x.ax=0;
	int86(0x33,&in,&out);
	if(out.x.ax==0)//mouse not initialised
	{
		printf("\n mouse not initialised");
		getch();
		exit(0);
		return 0;
	}
	else//mouse successfully initialised
		return 1;
}
void sm()//displays the mouse in text mode<cursor>
{
	in.x.ax=1;
	int86(0x33,&in,&out);
}
int m_c()//detects which mouse button is clicked
{
	while(!kbhit())
	{
		in.x.ax=3;
		int86(0x33,&in,&out);
		if(out.x.bx==1)//left click
			return 1;
		if(out.x.bx==2)//right click
			return 2;
		delay(100);
	}
}
int cp()//detects the cursor coordinates x & y on a left click
{
	while(!kbhit())
	{
		in.x.ax=3;
		int86(0x33,&in,&out);
		if(out.x.bx==1)//left click
		{
			cpx=out.x.cx;//x - coordinate
			cpy=out.x.dx;//y - coordinate
			return 1;
		}
		delay(100);
	}
}
void calendar(int,int);
int day_count(int,int);
int leap_year(int);
int getkey();
int i,loop_counter1,position,loop_counter2,days,display_counter,blink;
void cd()//displays current date
{
	getdate(&d);
	printf("Today's date : %d / %d / %d",d.da_day,d.da_mon,d.da_year);
}
void ct()//displays current time
{
	while(!kbhit())
	{
		gettime(&t);
		gotoxy(60,1);
		printf("Time : %2d:%2d:%2d.%2d",t.ti_hour,t.ti_min,t.ti_sec,t.ti_hund);
	}
}
void cal(int mon,int yr)//provides flexibility of arrow keys
{
	clrscr();
	calendar(mon,yr);
	printf("\n \n press Esc to view the options or arrow keys to move through the calendar");
	ct();
	do
	{
		switch(getkey())
		{
			case 72:clrscr();//up
				yr=yr+1;
				calendar(mon,yr);
				printf("\n\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
				printf("\n\tPress Escape to exit this sequence of arrow keys\n");
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
				ct();
				break;
			case 80:clrscr();//down
				yr=yr-1;
				calendar(mon,yr);
				printf("\n\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
				printf("\n\tPress Escape to exit this sequence of arrow keys\n");
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
				ct();
				break;
			case 75:clrscr();//left
				mon=mon-1;
				if(mon==0)
				{
					mon=12;
					yr=yr-1;
				}
				calendar(mon,yr);
				printf("\n\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
				printf("\n\tPress Escape to exit this sequence of arrow keys\n");
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
				ct();
				break;
			case 77:clrscr();//right
				mon=mon+1;
				if(mon==13)
				{
					mon=1;
					yr=yr+1;
				}
				calendar(mon,yr);
				printf("\n\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
				printf("\n\tPress Escape to exit this sequence of arrow keys\n");
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
				ct();
				break;
			case 27:return; //escape
		}
	}while(1);
}
int getkey()//obtains the scan code or ascii code for the key pressed
{
	int s;
	s=(int)getch();
	if(s==0)
		s=(int)getch();
	return s;
}
/* if we use the built-in union REGS then scan code for escape key is 1
whereas in this case it returns the ascii value of the escape key which is 27;
arrow keys - scan code
*/
void calendar(int m,int y)//displays the calendar for a particular month & year
{
	int var=1,ly_chk=0,loop_counter3=0;
	char week[7][10]={"sun","mon","tue","wed","thu","fri","sat"};
	cd();
	printf("\n \n date displayed : %d / %d / %d",blink,m,y);
	printf("\n\n\n\n YEAR=%d\t\tMONTH=",y);
	switch(m)
	{
		case 1:printf("January");
			break;
		case 2:printf("February");
			break;
		case 3:printf("March");
			break;
		case 4:printf("April");
			break;
		case 5:printf("May");
			break;
		case 6:printf("June");
			break;
		case 7:printf("July");
			break;
		case 8:printf("August");
			break;
		case 9:printf("September");
			break;
		case 10:printf("October");
			break;
		case 11:printf("November");
			break;
		case 12:printf("December");
			break;
		default:printf("month not defined");
			return;
	}
	printf("\n\n");
	for(i=0;i<7;i++)
		printf("%s\t",week[i]);
	printf("\n");
	for(loop_counter1=1901;loop_counter1<=y;loop_counter1++)
	{
		var=var+1;
		ly_chk=leap_year(loop_counter1);
		if(ly_chk==5&&loop_counter1!=y)
			var=var+1;
	}
	position=var;
	if(m!=1)
	{
		for(loop_counter2=1;loop_counter2<m;loop_counter2++)
		{
			days=day_count(loop_counter2,y);
			position=position+days;

		}
	}
	position=position%7;
	while(loop_counter3<position)
	{
		printf("\t");
		loop_counter3=loop_counter3+1;
	}
	days=day_count(m,y);
	for(display_counter=1;display_counter<=days;display_counter++)
	{
		if(display_counter==blink)
			textcolor(5+BLINK);//magenta
		cprintf("%d",display_counter);
		textcolor(15);
		printf("\t");
		if((display_counter+loop_counter3)%7==0)
			printf("\n");
	}
}
int day_count(int n,int u)//counts the number of days in a month
{
	int d;
	if(n==1||n==3||n==5||n==7||n==8||n==10||n==12)
		d=31;
	if(n==4||n==6||n==9||n==11)
		d=30;
	if((u%4)==0&&((u%100)!=0||(u%400)==0))
		if(n==2)
			d=29;
	if((u%4)!=0&&n==2)
		d=28;
	return d;
}
int leap_year(int k)//checks for leap year
{
	int ly;
	if((k%4)==0&&((k%100)!=0||(k%400)==0))
		ly=5;
	else
		ly=0;
	return ly;
}
void main()
{
	int m,month,year;
	clrscr();
	dm();
	sm();
	textcolor(15);//white
	printf("\n calendar...click!");
	m=m_c();
	if(m)
	{
		getdate(&d);
		blink=d.da_day;
		cal(d.da_mon,d.da_year);
	}
	do
	{
		gotoxy(58,10);
		textcolor(2);//green
		cprintf("OPTIONS....click!");
//		cp();
//		printf("\n %d,%d",cpx,cpy);
//		getch();
		fflush(stdin);
		cp();
		textcolor(15);//white
		if(cpx&&cpy)
		{
			clrscr();
			printf("\n OPTIONS");
			printf("\n\n\n\n\t 1. Go to date\n");
			printf("\t 2. Go to current date\n");
		}
		fflush(stdin);
		cprintf("\n press a key");
		getch();
		printf("\n left mouse click => choice 1");
		printf("\n right mouse click => choice 2 ");
		printf("\n press any key to exit");
		m=m_c();
		if(m==1)

	{
			clrscr();
			cprintf("\n Enter month in no.s");
			scanf("%d",&month);
			cprintf("\n Enter year >=1900 in 4 digits");
			scanf("%d",&year);
			printf("\n Enter day");
			scanf("%d",&blink);
			cal(month,year);
		}
		else if(m==2)
		{
			clrscr();
			getdate(&d);
			blink=d.da_day;
			cal(d.da_mon,d.da_year);
		}
	}while(!kbhit());
	clrscr();
	getch();
	cd();
	printf(" \n\n NOTICE THE CLOCK AND DATE--it's coded");
	textcolor(3);
	gotoxy(5,5);
	cprintf(" UNIQUE FEATURE : INPUT THROUGH MOUSE CLICKS");
	textcolor(15);//white
	printf("\n\n\n\n\n\n\n\n\n\n\n\n Press any key to stop the clock");
	sm();
	ct();
	getch();
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n Now click anywhere on the screen to exit");
	m_c();
	clrscr();
}
