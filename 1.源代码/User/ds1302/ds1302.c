#include "ds1302.h"
#include "sys.h"
#include "delay.h"
#include "bsp_oled.h"
	
struct TIMEData TimeData;
u8 read_time[7];
volatile u8 time_buf[8] = {0};//初始时间2010年6月1号23点59分55秒 星期二
 
void ds1302_gpio_init(void)//CE,SCLK端口初始化
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PC.11  CE
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOC.11
GPIO_ResetBits(GPIOA,GPIO_Pin_10); 
 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC.12  SCLK
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOC.12
GPIO_ResetBits(GPIOA,GPIO_Pin_12); 
}
 
void ds1032_DATAOUT_init(void)//配置双向I/O端口为输出态
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //PC.10  DATA
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOC.10
GPIO_ResetBits(GPIOA,GPIO_Pin_11);
}
 
void ds1032_DATAINPUT_init(void)//配置双向I/O端口为输入态
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //PC.10 DATA
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOC.10
}
 
 
void ds1302_write_onebyte(u8 data)//向DS1302发送一字节数据
{
u8 count=0;
ds1032_DATAOUT_init();

SCLK_L;
for(count=0;count<8;count++)
	{	SCLK_L;
		if(data&0x01)
		{DATA_H;}
		else{DATA_L;}//先准备好数据再发送
		SCLK_H;//拉高时钟线，发送数据
		data>>=1;
	}
}
 
void ds1302_wirte_rig(u8 address,u8 data)//向指定寄存器地址发送数据
{
u8 temp1=address;
u8 temp2=data;
CE_L;SCLK_L;delay_us(1);
CE_H;delay_us(2);
ds1302_write_onebyte(temp1);
ds1302_write_onebyte(temp2);
CE_L;SCLK_L;delay_us(2);
}
 
u8 ds1302_read_rig(u8 address)//从指定地址读取一字节数据
{
u8 temp3=address;
u8 count=0;
u8 return_data=0x00;
CE_L;SCLK_L;delay_us(3);
CE_H;delay_us(3);
ds1302_write_onebyte(temp3);
ds1032_DATAINPUT_init();//配置I/O口为输入
delay_us(2);
for(count=0;count<8;count++)
	{
	delay_us(2);//使电平持续一段时间
	return_data>>=1;
	SCLK_H;delay_us(4);//使高电平持续一段时间
	SCLK_L;delay_us(14);//延时14us后再去读取电压，更加准确
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11))//此处有埋伏
	{return_data=return_data|0x80;}
	
	}
delay_us(2);
CE_L;DATA_L;
return return_data;
}

 
//void ds1032_init(void)
//{
//ds1302_wirte_rig(0x8e,0x00);//关闭写保护
//ds1302_wirte_rig(0x80,0x37);//seconds37秒
//ds1302_wirte_rig(0x82,0x20);//minutes58分
//ds1302_wirte_rig(0x84,0x23);//hours23时
//ds1302_wirte_rig(0x86,0x19);//date30日
//ds1302_wirte_rig(0x88,0x08);//months9月
//ds1302_wirte_rig(0x8a,0x05);//days星期日
//ds1302_wirte_rig(0x8c,0x22);//year2020年
//ds1302_wirte_rig(0x8e,0x80);//关闭写保护
//}

//向DS302写入时钟数据
void ds1302_write_time(void) 
{
ds1302_wirte_rig(0x8e,0x00);//关闭写保护
ds1302_wirte_rig(0x82,time_buf[5]);//minutes58分
ds1302_wirte_rig(0x84,time_buf[4]);//hours23时
ds1302_wirte_rig(0x86,time_buf[3]);//date30日
ds1302_wirte_rig(0x88,time_buf[2]);//months9月
ds1302_wirte_rig(0x8a,time_buf[6]);//days星期日
ds1302_wirte_rig(0x8c,time_buf[1]);//year2020年
ds1302_wirte_rig(0x8e,0x80);//关闭写保护
}



void ds1032_read_time(void)
{
read_time[0]=ds1302_read_rig(0x81);//读秒
read_time[1]=ds1302_read_rig(0x83);//读分
read_time[2]=ds1302_read_rig(0x85);//读时
read_time[3]=ds1302_read_rig(0x87);//读日
read_time[4]=ds1302_read_rig(0x89);//读月
read_time[5]=ds1302_read_rig(0x8B);//读星期
read_time[6]=ds1302_read_rig(0x8D);//读年
}
 
void ds1032_read_realTime(void)
{
ds1032_read_time();  //BCD码转换为10进制
TimeData.second=(read_time[0]>>4)*10+(read_time[0]&0x0f);
TimeData.minute=((read_time[1]>>4)&(0x07))*10+(read_time[1]&0x0f);
TimeData.hour=(read_time[2]>>4)*10+(read_time[2]&0x0f);
TimeData.day=(read_time[3]>>4)*10+(read_time[3]&0x0f);
TimeData.month=(read_time[4]>>4)*10+(read_time[4]&0x0f);
TimeData.week=read_time[5];
TimeData.year=(read_time[6]>>4)*10+(read_time[6]&0x0f)+2000;
}


void RealTime_Display_Init(void)         //上电初始化显示实时时间
{
	  OLED_ShowChinese(84,0,12);//星
		OLED_ShowChinese(98,0,13);//期
		OLED_ShowChinese(112,0,TimeData.week+13);                    //用oled实时显示秒钟数

	 OLED_ShowNum(0,0,TimeData.year,4,16);                    //用oled实时显示分钟数
		 OLED_ShowString(32,0,"-",16);                      //用oled实时显示倒计时分号
	   OLED_ShowNum(42,0,TimeData.month,2,16);                    //用oled实时显示分钟数
		 OLED_ShowString(58,0,"-",16);                      //用oled实时显示倒计时分号
	   OLED_ShowNum(68,0,TimeData.day,2,16);                    //用oled实时显示秒钟数
			if(TimeData.month<10)                                     //防止显示1位数，不整齐
			{  
			  	OLED_ShowString(42,0,"0",16);                 //0位补偿
			}
			if(TimeData.day<10)                                     //防止显示1位数，不整齐
			{
			  	OLED_ShowString(68,0,"0",16);                 //0位补偿
			}			
			
		
			
		 OLED_ShowNum(34,3,TimeData.hour,2,16);                    //用oled实时显示分钟数
		 OLED_ShowString(54,3,":",16);                      //用oled实时显示倒计时分号
	   OLED_ShowNum(64,3,TimeData.minute,2,16);                    //用oled实时显示分钟数
		 OLED_ShowString(84,3,":",16);                      //用oled实时显示倒计时分号
	   OLED_ShowNum(94,3,TimeData.second,2,16);                    //用oled实时显示秒钟数
			if(TimeData.minute<10)                                     //防止显示1位数，不整齐
			{  
			  	OLED_ShowString(64,3,"0",16);                 //0位补偿
			}
			if(TimeData.second<10)                                     //防止显示1位数，不整齐
			{
			  	OLED_ShowString(94,3,"0",16);                 //0位补偿
			}		
			if(TimeData.hour<10)                                     //防止显示1位数，不整齐
			{  
			  	OLED_ShowString(34,3,"0",16);                 //0位补偿
			}
			 OLED_ShowChinese(0,6,35);//温
		   OLED_ShowChinese(16,6,36);//度
			 OLED_ShowString(30,6,":",16);                      //用oled实时显示倒计时分号
			 OLED_ShowNum(36,6,temperature,2,16);                   
		   OLED_ShowChinese(54,6,39);//℃	
		

			
			 OLED_ShowChinese(68,6,37);//湿
		   OLED_ShowChinese(84,6,38);//度
			 OLED_ShowString(98,6,":",16);                      //用oled实时显示倒计时分号
			 OLED_ShowNum(104,6,humidity,2,16);  
				OLED_ShowString(120,6,"%",16); 
		}

		
		
		
void RealTime_Display(void)            //实时时间日期实时刷新显示	
		{
			OLED_ShowNum(94,3,TimeData.second,2,16);                    //用oled实时显示秒钟数
			if(TimeData.second<10)                                     //防止显示1位数，不整齐
			{
			  	OLED_ShowString(94,3,"0",16);                 //0位补偿
			}		
			
			
			if(TimeData.second==0)
			{

		 OLED_ShowNum(0,0,TimeData.year,4,16);                    //用oled实时显示分钟数
		 OLED_ShowString(32,0,"-",16);                      //用oled实时显示倒计时分号
	   OLED_ShowNum(42,0,TimeData.month,2,16);                    //用oled实时显示分钟数
		 OLED_ShowString(58,0,"-",16);                      //用oled实时显示倒计时分号
	   OLED_ShowNum(68,0,TimeData.day,2,16);                    //用oled实时显示秒钟数
  
				if(TimeData.month<10)                                     //防止显示1位数，不整齐
			{  
			  	OLED_ShowString(42,0,"0",16);                 //0位补偿
			}
			if(TimeData.day<10)                                     //防止显示1位数，不整齐
			{
			  	OLED_ShowString(68,0,"0",16);                 //0位补偿
			}			
			
		
			
		 OLED_ShowNum(34,3,TimeData.hour,2,16);                    //用oled实时显示分钟数
		 OLED_ShowString(54,3,":",16);                      //用oled实时显示倒计时分号
	   OLED_ShowNum(64,3,TimeData.minute,2,16);                    //用oled实时显示分钟数
		 OLED_ShowString(84,3,":",16);                      //用oled实时显示倒计时分号
	 
			if(TimeData.minute<10)                                     //防止显示1位数，不整齐
			{  
			  	OLED_ShowString(64,3,"0",16);                 //0位补偿
			}
			
			if(TimeData.hour<10)                                     //防止显示1位数，不整齐
			{  
			  	OLED_ShowString(34,3,"0",16);                 //0位补偿
			}
			
			}
			
		   OLED_ShowChinese(0,6,35);//温
		   OLED_ShowChinese(16,6,36);//度
			 OLED_ShowString(30,6,":",16);                      //用oled实时显示倒计时分号
			 OLED_ShowNum(36,6,temperature,2,16);                   
		   OLED_ShowChinese(54,6,39);//℃	
		

			
			 OLED_ShowChinese(68,6,37);//湿
		   OLED_ShowChinese(84,6,38);//度
			 OLED_ShowString(98,6,":",16);                      //用oled实时显示倒计时分号
			 OLED_ShowNum(104,6,humidity,2,16);  
			 OLED_ShowString(120,6,"%",16); 
		}
		
		void Modify_RealTime_Display(void)            //修改后时间日期刷新一次显示
		{
			 OLED_ShowChinese(84,0,12);//星
		   OLED_ShowChinese(98,0,13);//期
		   OLED_ShowChinese(112,0,TimeData.week+13);                    //用oled实时显示秒钟数
			
			OLED_ShowNum(94,3,TimeData.second,2,16);                    //用oled实时显示秒钟数
			if(TimeData.second<10)                                     //防止显示1位数，不整齐
			{
			  	OLED_ShowString(94,3,"0",16);                 //0位补偿
			}		


		 OLED_ShowNum(0,0,TimeData.year,4,16);                    //用oled实时显示分钟数
		 OLED_ShowString(32,0,"-",16);                      //用oled实时显示倒计时分号
	   OLED_ShowNum(42,0,TimeData.month,2,16);                    //用oled实时显示分钟数
		 OLED_ShowString(58,0,"-",16);                      //用oled实时显示倒计时分号
	   OLED_ShowNum(68,0,TimeData.day,2,16);                    //用oled实时显示秒钟数
			if(TimeData.month<10)                                     //防止显示1位数，不整齐
			{  
			  	OLED_ShowString(42,0,"0",16);                 //0位补偿
			}
			if(TimeData.day<10)                                     //防止显示1位数，不整齐
			{
			  	OLED_ShowString(68,0,"0",16);                 //0位补偿
			}			
			
		
			
		 OLED_ShowNum(34,3,TimeData.hour,2,16);                    //用oled实时显示分钟数
		 OLED_ShowString(54,3,":",16);                      //用oled实时显示倒计时分号
	   OLED_ShowNum(64,3,TimeData.minute,2,16);                    //用oled实时显示分钟数
		 OLED_ShowString(84,3,":",16);                      //用oled实时显示倒计时分号
	 
			if(TimeData.minute<10)                                     //防止显示1位数，不整齐
			{  
			  	OLED_ShowString(64,3,"0",16);                 //0位补偿
			}
			if(TimeData.hour<10)                                     //防止显示1位数，不整齐
			{  
			  	OLED_ShowString(34,3,"0",16);                 //0位补偿
			}
			  OLED_ShowChinese(0,6,35);//温
		   OLED_ShowChinese(16,6,36);//度
			 OLED_ShowString(30,6,":",16);                      //用oled实时显示倒计时分号
			 OLED_ShowNum(36,6,temperature,2,16);                   
		   OLED_ShowChinese(54,6,39);//℃	
		

			
			 OLED_ShowChinese(68,6,37);//湿
		   OLED_ShowChinese(84,6,38);//度
			 OLED_ShowString(98,6,":",16);                      //用oled实时显示倒计时分号
			 OLED_ShowNum(104,6,humidity,2,16);  
			 OLED_ShowString(120,6,"%",16); 
		}
	