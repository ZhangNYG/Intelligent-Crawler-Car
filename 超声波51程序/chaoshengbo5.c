#include <reg51.h> 
#define uchar unsigned char 
#define uint unsigned int 
#define ulong unsigned long 
int k=100,m,i,j;
sbit p2_0 = P2^0;
sbit p2_1 = P2^1;
sbit p2_2 = P2^2;
sbit p2_3 = P2^3;

sbit Trig =P1^0; 
sbit Echo =P1^1;
sbit test =P1^7; ///指示灯


uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,
                        0xf8,0x80,0x90}; 

uchar flag,outcomeH,outcomeL,temp; 
bit succeed_flag; 
void delay(uint count)//延时
{ 
   uint i; 
   while(count) 
        {
   	     i=200;
   		 while(i>0) 
         i--;
         count--; 
        }    
} 

void delay_20us() //延时
{ 
   uchar bt ; 
   for(bt=0;bt<100;bt++); 
} 


void delay2( m)
{
for (i=m;i>0;i--)
for(j=125;j>0;j--);
}

void display(uint distance_date) 
{ 
    uchar m,dm,cm,mm; 
	m=distance_date/1000;
    dm=distance_date%1000/100; 
    cm=distance_date%100/10;
    mm=distance_date%10; 
	while(k--)
	{
    	p2_0=0;p2_1=1;p2_2=1;p2_3=1;
		P0 =table[m];
		delay2(10);
		P0=0x00;
		
		p2_0=1;p2_1=0;p2_2=1;p2_3=1;
		P0 =table[dm];
		delay2(10);
		P0=0x00;
		
		p2_0=1;p2_1=1;p2_2=0;p2_3=1;
		P0 =table[cm];
		delay2(10);
		P0=0x00;
		
		p2_0=1;p2_1=1;p2_2=1;p2_3=0;
		P0 =table[mm];
		delay2(10);
		P0=0x00;
        
		p2_3=1;
        delay2(8);
	 }
}

void main(void)
{  
	uint distance_data=0; 
	uchar a;
 	float tem=0.17;
 	flag=0;//标志位，1表示收到回波，0则未收到
 	test=0;//控制灯闪烁 
	Trig=0;	
	TMOD=0x11; //对定时器1选择16位模式
	IT0=0; //设置触发方式为低电平触发
	IE0=0;
 	EA=1; //开总中断	
	ET0=0; 
	TR0=0; 
	while(1) 
		{ 
			EA=0; 
			TH1=0;
			TL1=0; 
			Trig=1;//让Trig引脚为高电平
 			delay_20us();//延时20毫秒
 			Trig=0; //产生方波
			while(Echo==0); //等待Echo变为低电平，即开始发送超声波
			TR1=1;//开始计时
			succeed_flag=0;
 			EX0=1; 
			TF1=0; 
			EA=1; 
 			while((TH1<25)&&(!IE0));//等待25毫秒或者中断产生（即收到回波）
 			TR1=0; //停止计时
			EX0=0; //关闭中断
			if(succeed_flag==1)//如果收到回波
 				{ 
					distance_data=outcomeH; //先保存高8位
					distance_data<<=8; //将高8位左移，低8位则全置0
					distance_data=distance_data|outcomeL;  	//将高8位组成新的16位数据				
 					distance_data*=tem; 
					distance_data=distance_data*126000/120000; //代入晶振频率算出距离
 					for(a=300;a>0;a--)
 						{
 							display(distance_data);//现实距离
 						} 
				} 
			if(succeed_flag==0) //如果没有收到回波
				{ 
					distance_data=0; 
					display(distance_data);
					test = !test; 
				} 
		} 
} 

INTO_() interrupt 0
{ 
	outcomeH =TH1; 
	outcomeL =TL1;
 	succeed_flag=1; 
	EX0=0; 
}
