#include <reg52.h>	         //���õ�Ƭ��ͷ�ļ�
#define uchar unsigned char  //�޷����ַ��� �궨��	������Χ0~255
#define uint  unsigned int	 //�޷������� �궨��	������Χ0~65535
#include <intrins.h>

//����ܶ�ѡ����      0     1    2    3    4    5	6	 7	  8	   9	
uchar code smg_du[]={0x28,0xEE,0x32,0xA2,0xE4,0xA1,0x21,0xEA,0x20,0xA0,
					0x60,0x25,0x39,0x26,0x31,0x71,0xff};	 //����

//�����λѡ����
//sbit P0_0=P0^0;//������P0_0ΪP0�ڵĵ�1λ���Ա����λ������bit��int char֮��Ĳ�ֻ࣬����char=8λ, bit=1λ���ѡ����Ǳ������������ڱ�������з����ַ��
sbit smg_we1 = P3^4;	    //�����λѡ����
sbit smg_we2 = P3^5;
sbit smg_we3 = P3^6;
sbit smg_we4 = P3^7;

uchar dis_smg[8] =  {0x90,0xf5,0x1c,0x34,0x71,0x32,0x12,0xf4};	
bit flag_500ms;
sbit beep = P2^3;
uchar a_a;
uchar menu_1;	   	//���ò�����

uchar fen,miao,weimiao;     		// ����ʱ��
uchar num_value;    // �����м�ı���
bit flag_num_en;   // ����ʱ����ʼ��ʱʹ�ܱ�־λ
uchar set_fen = 30; // ���õ���ʱ������ʼֵ 
uchar set_miao = 0; // ���õ���ʱ������ʼֵ 

/***********************1ms��ʱ����*****************************/
void delay_1ms(uint q)
{
	uint i,j;
	for(i=0;i<q;i++)
		for(j=0;j<120;j++);
}


/***********************����λѡ����*****************************/
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


/*************��ʱ��0��ʼ������***************/
void time_init()	  
{
	EA   = 1;	 	  //�����ж�
	TMOD = 0X01;	  //��ʱ��0��������ʽ1
	ET0  = 1;		  //����ʱ��0�ж� 
	TR0  = 1;		  //����ʱ��0��ʱ
}

/***********************������ʾ����*****************************/

void display()
{
	uchar i;
	for(i=0;i<4;i++)
	{					          
		P1 = 0xff;			 //���� 
		smg_we_switch(i);	 //λѡ
		P1 = dis_smg[i];	 //��ѡ	     
		delay_1ms(1);			 
	}	
}

/********************������������*****************/
uchar key_can;	 //����ֵ

void key()	 //������������
{
	static uchar key_new;
	key_can = 20;                   //����ֵ��ԭ
	if((P2 & 0x07) != 0x07)		//��������
	{
		delay_1ms(1);	     	//����������
		if(((P2 & 0x07) != 0x07) && (key_new == 1))
		{						//ȷ���ǰ�������
			key_new = 0;
			switch(P2 & 0x07)
			{
				case 0x06: key_can = 1; break;	   //�õ�k2��ֵ
				case 0x05: key_can = 2; break;	   //�õ�k3��ֵ
				case 0x03: key_can = 3; break;	   //�õ�k4��ֵ
			}
		}			
	}
	else 
		key_new = 1;	
}

/****************���������������ʾ����***************/
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
	if(menu_1 == 0)   			//����ʱ������������ʼ ��ͣ
	{ 		
		if(key_can == 2)   		//��ʼ��
		{
			flag_num_en = 1;
			beep = 0;	  //�򿪷�����
			delay_1ms(30);
			beep = 1;	  //�رշ�����
		}	
		if(key_can == 3)		//��ͣ��
		{
			flag_num_en = 0;
			beep = 0;	  //�򿪷�����
			delay_1ms(30);
			beep = 1;	  //�رշ�����
		}
	}
	if(menu_1 == 1)				//���õ���ʱ����ʼ��
	{
		if(key_can == 2)
		{
			set_miao ++ ;		// ��������
			if(set_miao > 60)
				set_miao = 0;	//���ӵ�99	
			dis_smg[0] = smg_du[set_miao % 10];	      //ȡ������ʾ
			dis_smg[1] = smg_du[set_miao / 10 % 10] ;  //ȡʮλ��ʾ
		}
		if(key_can == 3)
		{
			if(set_miao == 0)
				set_miao = 60;	
				set_miao -- ;		// ��������
			dis_smg[0] = smg_du[set_miao % 10];	      //ȡ������ʾ
			dis_smg[1] = smg_du[set_miao / 10 % 10] ;  //ȡʮλ��ʾ
		}
	}	

	if(menu_1 == 2)				//���õ���ʱ����ʼ��
	{
		if(key_can == 2)
		{
			set_fen ++ ;		// ��������
			if(set_fen > 99)
				set_fen = 0;	//���ӵ�99	
			dis_smg[2] = smg_du[set_fen % 10];	      //ȡ������ʾ
			dis_smg[3] = smg_du[set_fen / 10 % 10] ;  //ȡʮλ��ʾ
		}
		if(key_can == 3)
		{
			if(set_fen == 0)
				set_fen = 99;
				set_fen -- ;		// ��������
			dis_smg[2] = smg_du[set_fen % 10];	      //ȡ������ʾ
			dis_smg[3] = smg_du[set_fen / 10 % 10] ;  //ȡʮλ��ʾ
		}
	}	
}  


/******************������**********************/	   
void main()
{
	beep = 0;				    //������������һ��
	delay_1ms(100);
	P0 = P1 = P2 = P3 = 0xff;   //��ʼ��IO��
	time_init();
	miao = set_miao;	   //������ʾ������
	fen  = set_fen;	   //������ʾ������
	while(1)
	{
		key();			   //����ɨ�躯��
		if(key_can < 10)
		{
			key_with();    //����ִ�к���
		}	
			
		if(menu_1 == 0)	 //ɨ����ʾ����ʱ��
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
				beep = 1;        //�رշ�����
				miao = set_miao;	 //����ʾ�����
				fen  = set_fen;	     //����ʾ�����
				flag_num_en = 0;
				weimiao = 1;
			}

		}

		if(menu_1 == 1)			 //�����õ�ʱ��ʮλ����˸
		{
			if(flag_500ms == 1)
			{
				dis_smg[0] = smg_du[set_miao % 10];	      //ȡ��λ��ʾ
				dis_smg[1] = smg_du[set_miao / 10 % 10];   //ȡʮλ��ʾ
				dis_smg[2] = smg_du[set_fen % 10] & 0xdf;	      //ȡ������ʾ
				dis_smg[3] = smg_du[set_fen / 10 % 10] ;  //ȡʮλ��ʾ
			}
			else 
			{
				dis_smg[0] = 0xff;	      //ȡ��λ��ʾ
				dis_smg[1] = 0xff;   //ȡʮλ��ʾ
				dis_smg[2] = smg_du[set_fen % 10] & 0xdf;	      //ȡ������ʾ
				dis_smg[3] = smg_du[set_fen / 10 % 10] ;  //ȡʮλ��ʾ
			}
		}

		if(menu_1 == 2)			 //�����õ�ʱ��ʮλ����˸
		{
			if(flag_500ms == 1)
			{
				dis_smg[0] = smg_du[set_miao % 10];	      //ȡ��λ��ʾ
				dis_smg[1] = smg_du[set_miao / 10 % 10];   //ȡʮλ��ʾ
				dis_smg[2] = smg_du[set_fen % 10] & 0xdf;	      //ȡ������ʾ
				dis_smg[3] = smg_du[set_fen / 10 % 10] ;  //ȡʮλ��ʾ
			}
			else 
			{
				dis_smg[0] = smg_du[set_miao % 10];	      //ȡ��λ��ʾ
				dis_smg[1] = smg_du[set_miao / 10 % 10];   //ȡʮλ��ʾ
				dis_smg[2] = 0xff;	      //ȡ������ʾ
				dis_smg[3] = 0xff ;  //ȡʮλ��ʾ
			}
		}

		display();		  //�������ʾ����


	}
}

/*************��ʱ��0�жϷ������***************/
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
			miao --;				   //��������һ��
		}
		weimiao --;

	}
}