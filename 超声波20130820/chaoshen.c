/***********************************************************************************************************/
//hc-sr04 超声波测距模块 DEMO 程序
//晶振：11。0592
//程序	  QQ： 517535000
//taobao店铺：http://shop58246706.taobao.com
//接线：模块TRIG/TX接 P2.7     ECH0/RX 接P2.6
//数码管：共阳数码管P1接数据口,P2.5 P2.4 P2.3接选通数码管
/***********************************************************************************************************/	    
#include <reg52.H>		//器件配置文件
#include <intrins.h>
			 
unsigned int  i=0,J=0,K=0,time=0;
unsigned int  timer=0;
unsigned char posit=0;
unsigned long S=0;
bit      flag =0;
sbit  RX  =P2^6;
sbit  TX  =P2^7;
sbit  ena  =P3^0;
sbit  enb  =P3^2;
sbit p2_0 = P2^0;
sbit p2_1 = P2^1;
sbit p2_2 = P2^2;
sbit p2_3 = P2^3;



unsigned char code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,
                        0xf8,0x80,0x90};





void Delay(unsigned int s)
{
	unsigned int i;
	for(i=0; i<s; i++);

}

/********************************************************/
   			 //扫描数码管

 void Display(unsigned long x)
{
  
p2_0=0;p2_1=1;p2_2=1;p2_3=1;
P0 =table[x/100%10];
Delay(5);


p2_0=1;p2_1=0;p2_2=1;p2_3=1;
P0 =table[x/10%10];
Delay(5);

p2_0=1;p2_1=1;p2_2=0;p2_3=1;
P0 =table[x%10];
Delay(5);

p2_0=1;p2_1=1;p2_2=1;p2_3=0;
P0 =table[x/1000%10];
Delay(5);

p2_3=1;
Delay(2);
}
		//关闭个位数码管
	

/********************************************************/
    void Conut(void)
	{
	 time=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	
	 S=(time*1.7)/100;     //算出来是CM
	 if((S>=400)||flag==1) //超出测量范围显示“-”
	 {	 
	  flag=0;
	  S=500;
	 }
	 }
/********************************************************/
     void zd0() interrupt 1 		 //T0中断用来计数器溢出,超过测距范围
  {
   // TH0=0;
   // TL0=0;
    flag=1;							 //中断溢出标志
  }
/********************************************************/
   void  zd3()  interrupt 3 		 //T1中断用来扫描数码管和计800MS启动模块
  {
	 TH1=0x75;
	 TL1=0x30;
	 Display(S);
	 timer++;
	 if(timer>=10)
	 {
	  timer=0;
	  TX=1;			                //800MS  启动一次模块
 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();

	  TX=0;
	 } 
  }
/*********************************************************/

	void  main(  void  )

  {  
    TMOD=0x11;		   //设T0为方式1，GATE=1；
	TH0=0;
	TL0=0;          
	TH1=0x75;		   //2MS定时
	TL1=0x30;
	ET0=1;             //允许T0中断
	ET1=1;			   //允许T1中断
	TR1=1;			   //开启定时器
	EA=1;			   //开启总中断
    ena=1;
    enb=1;
    P1=0X00;
    
    while(S<70){

	
	 while(!RX);		//当RX为零时等待
	 TR0=1;			    //开启计数
	 while(RX);			//当RX为1计数并等待
	 TR0=0;				//关闭计数
     Conut();			//计算
     P1=0X00;
     Delay(10000);
     P1=0X09;
     
     
     }



 while(S>=70)
	{

	
	 while(!RX);		//当RX为零时等待
	 TR0=1;			    //开启计数
	 while(RX);			//当RX为1计数并等待
	 TR0=0;				//关闭计数
     Conut();			//计算
     P1=0X00;
     Delay(10000);
     P1=0X0B;
 
     }
 
 }               
