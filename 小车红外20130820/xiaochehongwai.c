#include<reg51.h>

#include<stdlib.h>
#define uchar unsigned char
#define uint unsigned int
uint a,b,c,d,t;
sbit in1=P1^1;
sbit in2=P1^2;
sbit in3=P1^3;
sbit in4=P1^4;
sbit light=P1^5;
sbit right=P0^0;
sbit lift=P0^1;
 
void delay(uint a)
{ 
uint i,j,k;

for(i=a;i>0;i--)
    for(j=100;j>0;j--)
	for(k=100;k>0;k--);
}
void init()      //初始化函数 

{ 

    t = 0; 

     

    

    TMOD = 0x02;    //设置定时器0为工作方式2 

    TH0 = 100;        //装初值                          

    TL0 = 7;                                 

    EA = 1;         //开总中断 

    ET0 = 1;        //开定时器0中断 

    TR0 = 1;        //启动定时0 

    
}
void main(void)
{
init();
	
while(1)
{	
    srand(t);
    a=((char)rand())%10;
    b=(a+1)%2; 
    c=a%2;   
	right=1;
	lift=1;
	in1=1;
	in2=0;
	in3=0;
	in4=1;
	if(light==0)
		{
		 right=0;
		 lift=0;
		 delay(5);
		 right=1;
		 lift=1;
		 in1=b;
		 in2=c;
		 in3=b;
	     in4=c;
		 delay(4);
		 right=0;
		 lift=0;
		 delay(10);
		
		 }
	  




}	 
}
void time0() interrupt 1     //定时器0中断 

{         

    t++;        //时间加1 

    if(t == 100) 

    { 

        t = 0;  

    } 
}
