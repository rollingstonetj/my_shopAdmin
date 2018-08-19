#include <reg52.h>	         //调用单片机头文件
#define uchar unsigned char  //无符号字符型 宏定义	变量范围0~255
#define uint  unsigned int	 //无符号整型 宏定义	变量范围0~65535
#include <intrins.h>

//数码管段选定义      0     1    2    3    4    5	6	 7	  8	   9	
uchar code smg_du[]={0x28,0xEE,0x32,0xA2,0xE4,0xA1,0x21,0xEA,0x20,0xA0,
					0x60,0x25,0x39,0x26,0x31,0x71,0xff};	 //断码

//数码管位选定义
//sbit P0_0=P0^0;//即定义P0_0为P0口的第1位，以便进行位操作。bit和int char之类的差不多，只不过char=8位, bit=1位而已。都是变量，编译器在编译过程中分配地址。
sbit smg_we1 = P3^4;	    //数码管位选定义
sbit smg_we2 = P3^5;
sbit smg_we3 = P3^6;
sbit smg_we4 = P3^7;

uchar dis_smg[8] =  {0x90,0xf5,0x1c,0x34,0x71,0x32,0x12,0xf4};	
bit flag_500ms;
sbit beep = P2^3;
uchar a_a;
uchar menu_1;	   	//设置参数用

uchar fen,miao,weimiao;     		// 倒计时数
uchar num_value;    // 用做中间的变量
bit flag_num_en;   // 倒计时器开始计时使能标志位
uchar set_fen = 30; // 设置倒计时数的启始值 
uchar set_miao = 0; // 设置倒计时数的启始值 

/***********************1ms延时函数*****************************/
void delay_1ms(uint q)
{
	uint i,j;
	for(i=0;i<q;i++)
		for(j=0;j<120;j++);
}


/***********************数码位选函数*****************************/
void smg_we_switch(uchar i)
{
	switch(i)
	{
		case 0: smg_we1 = 0;  smg_we2 = 1; smg_we3 = 1;  smg_we4 = 1; break;
		case 1: smg_we1 = 1;  smg_we2 = 0; smg_we3 = 1;  smg_we4 = 1; break;
		case 2: smg_we1 = 1;  smg_we2 = 1; smg_we3 = 0;  smg_we4 = 1; break;
		case 3: smg_we1 = 1;  smg_we2 = 1; smg_we3 = 1;  smg_we4 = 0; break;
	}	
}


/*************定时器0初始化程序***************/
void time_init()	  
{
	EA   = 1;	 	  //开总中断
	TMOD = 0X01;	  //定时器0、工作方式1
	ET0  = 1;		  //开定时器0中断 
	TR0  = 1;		  //允许定时器0定时
}

/***********************数码显示函数*****************************/

void display()
{
	uchar i;
	for(i=0;i<4;i++)
	{					          
		P1 = 0xff;			 //消隐 
		smg_we_switch(i);	 //位选
		P1 = dis_smg[i];	 //段选	     
		delay_1ms(1);			 
	}	
}

/********************独立按键程序*****************/
uchar key_can;	 //按键值

void key()	 //独立按键程序
{
	static uchar key_new;
	key_can = 20;                   //按键值还原
	if((P2 & 0x07) != 0x07)		//按键按下
	{
		delay_1ms(1);	     	//按键消抖动
		if(((P2 & 0x07) != 0x07) && (key_new == 1))
		{						//确认是按键按下
			key_new = 0;
			switch(P2 & 0x07)
			{
				case 0x06: key_can = 1; break;	   //得到k2键值
				case 0x05: key_can = 2; break;	   //得到k3键值
				case 0x03: key_can = 3; break;	   //得到k4键值
			}
		}			
	}
	else 
		key_new = 1;	
}

/****************按键处理数码管显示函数***************/
void key_with()
{
	if(key_can == 1)
	{
		menu_1 ++;
		if(menu_1 >= 3)
		{
			menu_1 = 0;
			fen = set_fen;
			miao = set_miao;
		}
	}
	if(menu_1 == 0)   			//倒计时器按键操作开始 暂停
	{ 		
		if(key_can == 2)   		//开始键
		{
			flag_num_en = 1;
			beep = 0;	  //打开蜂鸣器
			delay_1ms(30);
			beep = 1;	  //关闭蜂鸣器
		}	
		if(key_can == 3)		//暂停键
		{
			flag_num_en = 0;
			beep = 0;	  //打开蜂鸣器
			delay_1ms(30);
			beep = 1;	  //关闭蜂鸣器
		}
	}
	if(menu_1 == 1)				//设置倒计时器开始数
	{
		if(key_can == 2)
		{
			set_miao ++ ;		// 设置数加
			if(set_miao > 60)
				set_miao = 0;	//最大加到99	
			dis_smg[0] = smg_du[set_miao % 10];	      //取个数显示
			dis_smg[1] = smg_du[set_miao / 10 % 10] ;  //取十位显示
		}
		if(key_can == 3)
		{
			if(set_miao == 0)
				set_miao = 60;	
				set_miao -- ;		// 设置数减
			dis_smg[0] = smg_du[set_miao % 10];	      //取个数显示
			dis_smg[1] = smg_du[set_miao / 10 % 10] ;  //取十位显示
		}
	}	

	if(menu_1 == 2)				//设置倒计时器开始数
	{
		if(key_can == 2)
		{
			set_fen ++ ;		// 设置数加
			if(set_fen > 99)
				set_fen = 0;	//最大加到99	
			dis_smg[2] = smg_du[set_fen % 10];	      //取个数显示
			dis_smg[3] = smg_du[set_fen / 10 % 10] ;  //取十位显示
		}
		if(key_can == 3)
		{
			if(set_fen == 0)
				set_fen = 99;
				set_fen -- ;		// 设置数减
			dis_smg[2] = smg_du[set_fen % 10];	      //取个数显示
			dis_smg[3] = smg_du[set_fen / 10 % 10] ;  //取十位显示
		}
	}	
}  


/******************主程序**********************/	   
void main()
{
	beep = 0;				    //开机蜂鸣器叫一声
	delay_1ms(100);
	P0 = P1 = P2 = P3 = 0xff;   //初始化IO口
	time_init();
	miao = set_miao;	   //开机显示设置数
	fen  = set_fen;	   //开机显示设置数
	while(1)
	{
		key();			   //按键扫描函数
		if(key_can < 10)
		{
			key_with();    //按键执行函数
		}	
			
		if(menu_1 == 0)	 //扫描显示倒计时数
		{	 
			dis_smg[0] = smg_du[miao % 10];
			dis_smg[1] = smg_du[miao / 10 % 10];
			dis_smg[2] = smg_du[fen % 10] & 0xdf;
			dis_smg[3] = smg_du[fen / 10 % 10];
			if(flag_500ms == 1)
			{
				flag_500ms = 0;
				if((miao <= 5) && (fen == 0))
					beep = ~beep;
			}

			if((fen == 0) && (miao == 0))
			{
				beep = 1;        //关闭蜂鸣器
				miao = set_miao;	 //让显示最大数
				fen  = set_fen;	     //让显示最大数
				flag_num_en = 0;
				weimiao = 1;
			}

		}

		if(menu_1 == 1)			 //在设置的时候十位数闪烁
		{
			if(flag_500ms == 1)
			{
				dis_smg[0] = smg_du[set_miao % 10];	      //取个位显示
				dis_smg[1] = smg_du[set_miao / 10 % 10];   //取十位显示
				dis_smg[2] = smg_du[set_fen % 10] & 0xdf;	      //取个数显示
				dis_smg[3] = smg_du[set_fen / 10 % 10] ;  //取十位显示
			}
			else 
			{
				dis_smg[0] = 0xff;	      //取个位显示
				dis_smg[1] = 0xff;   //取十位显示
				dis_smg[2] = smg_du[set_fen % 10] & 0xdf;	      //取个数显示
				dis_smg[3] = smg_du[set_fen / 10 % 10] ;  //取十位显示
			}
		}

		if(menu_1 == 2)			 //在设置的时候十位数闪烁
		{
			if(flag_500ms == 1)
			{
				dis_smg[0] = smg_du[set_miao % 10];	      //取个位显示
				dis_smg[1] = smg_du[set_miao / 10 % 10];   //取十位显示
				dis_smg[2] = smg_du[set_fen % 10] & 0xdf;	      //取个数显示
				dis_smg[3] = smg_du[set_fen / 10 % 10] ;  //取十位显示
			}
			else 
			{
				dis_smg[0] = smg_du[set_miao % 10];	      //取个位显示
				dis_smg[1] = smg_du[set_miao / 10 % 10];   //取十位显示
				dis_smg[2] = 0xff;	      //取个数显示
				dis_smg[3] = 0xff ;  //取十位显示
			}
		}

		display();		  //数码管显示函数


	}
}

/*************定时器0中断服务程序***************/
void time0_int() interrupt 1
{	
	static uint value;
	TH0 = 0xd8;
	TL0 = 0xf0;     // 10ms
	value ++;
	if(value >= 50)
	{
		value = 0;
		flag_500ms = ~flag_500ms;
	}
	if(flag_num_en == 1)
	{
		num_value = 0;	
		if(weimiao == 0)
		{
			weimiao = 100;
			if(miao == 0)
			{		
				miao = 60;		
				if(fen != 0)
					fen --;
			}
			miao --;				   //倒计数减一次
		}
		weimiao --;

	}
}