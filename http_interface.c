#include "http_def.h"

uint16_t Daily_C_Curve[VALID_CURVE_POINT_NUM + 3] = {0}; 
uint16_t Daily_D_Curve[VALID_CURVE_POINT_NUM + 3] = {0}; 
uint32_t Daily_Column[VALID_HOUR_NUM*2 + 3]       = {0}; 
uint32_t Monthly_Column[VALID_DAY_NUM*2 + 3]      = {0}; 
uint32_t Yearly_Column[VALID_MONTH_NUM*2 + 3]     = {0};
uint32_t Total_Column[VALID_YEAR_NUM*2 + 3]       = {0};
struct h_resource h_Rsc[]={//html资源数组
	{h_000,sizeof(h_000)},
	{h_001,sizeof(h_001)},
	{h_002,sizeof(h_002)},
	{h_003,sizeof(h_003)},
	{h_004,sizeof(h_004)},
	{h_005,sizeof(h_005)},
	{h_006,sizeof(h_006)},
	{h_007,sizeof(h_007)},
	{h_008,sizeof(h_008)},
	{h_009,sizeof(h_009)},
	{h_010,sizeof(h_010)},
	{h_011,sizeof(h_011)},
	{h_012,sizeof(h_012)},
	{h_013,sizeof(h_013)},
	{h_014,sizeof(h_014)}
};
struct j_resource j_Rsc[]={//javascript资源数组
	{j_000,sizeof(j_000)},
	{j_001,sizeof(j_001)},
	{j_002,sizeof(j_002)}
};
struct c_resource c_Rsc[]={//css资源数组
	{c_000,sizeof(c_000)}
};
struct i_resource i_Rsc[]={//图片资源数组
	{i_000,sizeof(i_000)},
	{i_001,sizeof(i_001)},
	{i_002,sizeof(i_002)},
	{i_003,sizeof(i_003)},
	{i_004,sizeof(i_004)}
};
struct s_resource s_Rsc[]={//初始化数据缓冲区
	{s_000,sizeof(s_000)}
};
struct a_resource a_Rsc[]={//ajax返回确认
	{a_000,sizeof(a_000)}
};
struct p_resource p_Rsc[]={//用于导出配置文件、历史记录、曲线图、柱状图、历史发电量
	{p_000,sizeof(p_000)}
};

void webserver_2s_time_callback(void)							//2s回调
{
	if(loginFlag!=0)//心跳包计数
	{
		loginFlag--;//计数减一
	}
	if(loginFlag2!=0)
	{
		loginFlag2--;//计数减一
	}
}
uint32_t return_max_u32(uint32_t *arr,uint8_t k,uint8_t num)	//返回32位无符号数中，最大的数
{
    uint32_t max=arr[k];
    for(uint16_t i=k;i<num;i++)
    {
        if(max<arr[i])
        {
            max=arr[i];
        }
    }
    if(max==0)return 100;
    else return (((max-1)/100)+1)*100;
}
void set_param(uint16_t num, uint16_t id, uint16_t val) 		//设置参数数组的保存值
{
    Param_flag[num]=id;
    Param_value[num]=val;
    save_flag_int=1;
}
uint16_t do_save_data_int(uint16_t id,uint16_t U16_Val)			//设置参数
{
	static uint16_t temp=0;
    switch(id)
    {   
        case 0x0101://系统时钟（年）
			if(2000<=U16_Val&&U16_Val<=2099)
			{
				Touch_Write_Time_Flag=true;
				Touch_Write_Time[0]=BinToBcd((uint8_t)(U16_Val-2000));
			}
		break;
        case 0x0102://系统时钟（月）
			if(U16_Val<=12)
			{
				Touch_Write_Time_Flag=true;
				Touch_Write_Time[1]=BinToBcd((uint8_t)U16_Val);
			}
		break;
        case 0x0103://系统时钟（日）
			if(U16_Val<=31)
			{
				Touch_Write_Time_Flag=true;
				Touch_Write_Time[2]=BinToBcd((uint8_t)U16_Val);
			}
		break;
        case 0x0104://系统时钟（时）
			if(U16_Val<=23)
			{
				Touch_Write_Time_Flag=true;
				Touch_Write_Time[3]=BinToBcd((uint8_t)U16_Val);
			}
		break;
        case 0x0105://系统时钟（分）
			if(U16_Val<=59)
			{
				Touch_Write_Time_Flag=true;
				Touch_Write_Time[4]=BinToBcd((uint8_t)U16_Val);
			}
		break;
        case 0x0106://系统时钟（秒）
			if(U16_Val<=59)
			{
				Touch_Write_Time_Flag=true;
				Touch_Write_Time[5]=BinToBcd((uint8_t)U16_Val);
			}
		break;
		case 0x0107://恒流模式
			/*if(PC_CAN_SET_DATA && SYS_NO_FAULT && (Inverter_Info.Com_Info.Grid_or_Offgrid==GRID))
			{
				if(abs((int16_t)U16_Val) <= P_Range[Inverter_Info.Com_Info.Device_Type].Const_I_Charge_I.Max)
				{
					Inverter_Info.Set_Info.Const_I_Charge_I = U16_Val;
					(void)write_eeprom(CONST_I_CHARGE_I_ADDRESS, &Inverter_Info.Set_Info.Const_I_Charge_I, 2);
					if(temp==0)
					{
						if(CURRENT_STATUS_NOT_IND_INVERTER(Inverter_Info.Run_Info.Run_Status))
						{
							if((Inverter_Info.Com_Info.IO_To_BMS_Mode == COM_BMS_MODE_NONE)||(D_BMS_WORK_OK || D_BMS_DISCHARGE_EMPTY))
							{
								Inverter_Info.Set_Info.Run_Mode=((int16_t)U16_Val)<0?1<<3:1<<8;
								SYS_Run_Mode_Last = Inverter_Info.Set_Info.Run_Mode;
								Send_Data_To_DSP_Flag = true;
								Inverter_Info.Com_Info.Open_or_Close=OPEN;
							}
						}
					}
				}
			}*/
			if(PC_CAN_SET_DATA && SYS_NO_FAULT && (Inverter_Info.Com_Info.Grid_or_Offgrid==GRID))
			{
				if(abs((int16_t)U16_Val) <= P_Range[Inverter_Info.Com_Info.Device_Type].Const_I_Charge_I.Max)
				{
					Inverter_Info.Set_Info.Const_I_Charge_I = U16_Val;
					(void)write_eeprom(CONST_I_CHARGE_I_ADDRESS, &Inverter_Info.Set_Info.Const_I_Charge_I, 2);
					if(temp==0)
					{
						if(CURRENT_STATUS_NOT_IND_INVERTER(Inverter_Info.Run_Info.Run_Status))
						{
						//edit by sundl 2016-1-28
							if(((int16_t)U16_Val)<0)
							{
								if((Inverter_Info.Com_Info.IO_To_BMS_Mode == COM_BMS_MODE_NONE)||(D_BMS_WORK_OK || D_BMS_DISCHARGE_EMPTY))
								{
									Inverter_Info.Set_Info.Run_Mode=1<<3;
									SYS_Run_Mode_Last = Inverter_Info.Set_Info.Run_Mode;
									Send_Data_To_DSP_Flag = true;
									Inverter_Info.Com_Info.Open_or_Close=OPEN;
								}
							}
							else
							{
								if((Inverter_Info.Com_Info.IO_To_BMS_Mode == COM_BMS_MODE_NONE)||(D_BMS_WORK_OK || D_BMS_CHARGE_FULL))
								{
									Inverter_Info.Set_Info.Run_Mode=1<<8;
									SYS_Run_Mode_Last = Inverter_Info.Set_Info.Run_Mode;
									Send_Data_To_DSP_Flag = true;
									Inverter_Info.Com_Info.Open_or_Close=OPEN;
								}
							}
							
						}
					}
				}
			}

		break;
		case 0x0108://恒压模式
			/*if(PC_CAN_SET_DATA && SYS_NO_FAULT && (Inverter_Info.Com_Info.Grid_or_Offgrid==GRID))
			{
				if((U16_Val >= P_Range[Inverter_Info.Com_Info.Device_Type].Const_V_Charge_V.Min) &&
				   (U16_Val <= P_Range[Inverter_Info.Com_Info.Device_Type].Const_V_Charge_V.Max))
				{
					Inverter_Info.Set_Info.Const_V_Charge_V = U16_Val;
					(void)write_eeprom(CONST_V_CHARGE_V_ADDRESS, &Inverter_Info.Set_Info.Const_V_Charge_V, 2);
					if(temp==1)
					{
						if(CURRENT_STATUS_NOT_IND_INVERTER(Inverter_Info.Run_Info.Run_Status))
						{
							if(Inverter_Info.Com_Info.IO_To_BMS_Mode == COM_BMS_MODE_NONE||(D_BMS_WORK_OK || D_BMS_DISCHARGE_EMPTY))
							{
								Inverter_Info.Set_Info.Run_Mode=(U16_Val>Inverter_Info.Run_Info.DC_Voltage)?1<<4:
															   ((U16_Val<Inverter_Info.Run_Info.DC_Voltage)?1<<9:
																Inverter_Info.Set_Info.Run_Mode);
								SYS_Run_Mode_Last = Inverter_Info.Set_Info.Run_Mode;
								Send_Data_To_DSP_Flag = true;
								Inverter_Info.Com_Info.Open_or_Close=OPEN;
							}
						}
					}
				}
			} */   
			if(PC_CAN_SET_DATA && SYS_NO_FAULT && (Inverter_Info.Com_Info.Grid_or_Offgrid==GRID))
			{
				if((U16_Val >= P_Range[Inverter_Info.Com_Info.Device_Type].Const_V_Charge_V.Min) &&
				   (U16_Val <= P_Range[Inverter_Info.Com_Info.Device_Type].Const_V_Charge_V.Max))
				{
					Inverter_Info.Set_Info.Const_V_Charge_V = U16_Val;
					(void)write_eeprom(CONST_V_CHARGE_V_ADDRESS, &Inverter_Info.Set_Info.Const_V_Charge_V, 2);
					if(temp==1)
					{
						if(CURRENT_STATUS_NOT_IND_INVERTER(Inverter_Info.Run_Info.Run_Status))
						{
						//edit by sundl 2016-1-28
							if(U16_Val>=Inverter_Info.Run_Info.DC_Voltage)
							{
								if((Inverter_Info.Com_Info.IO_To_BMS_Mode == COM_BMS_MODE_NONE)||(D_BMS_WORK_OK || D_BMS_DISCHARGE_EMPTY))
								{
									Inverter_Info.Set_Info.Run_Mode=1<<4;
									SYS_Run_Mode_Last = Inverter_Info.Set_Info.Run_Mode;
									Send_Data_To_DSP_Flag = true;
									Inverter_Info.Com_Info.Open_or_Close=OPEN;
								}
							}
							else
							{
								if((Inverter_Info.Com_Info.IO_To_BMS_Mode == COM_BMS_MODE_NONE)||(D_BMS_WORK_OK || D_BMS_CHARGE_FULL))
								{
									Inverter_Info.Set_Info.Run_Mode=1<<9;
									SYS_Run_Mode_Last = Inverter_Info.Set_Info.Run_Mode;
									Send_Data_To_DSP_Flag = true;
									Inverter_Info.Com_Info.Open_or_Close=OPEN;
								}
							}
							
						}
					}
				}
			}            
			
		break;
		case 0x0109://恒压限流值
			if(PC_CAN_SET_DATA && SYS_NO_FAULT && (Inverter_Info.Com_Info.Grid_or_Offgrid==GRID))
			{
				if((U16_Val >= P_Range[Inverter_Info.Com_Info.Device_Type].Const_V_Charge_I_Limit_Val.Min) &&
				   (U16_Val <= P_Range[Inverter_Info.Com_Info.Device_Type].Const_V_Charge_I_Limit_Val.Max))
				{
					Inverter_Info.Set_Info.Const_V_Charge_I_Limit_Val = U16_Val;
					(void)write_eeprom(CONST_V_CHARGE_I_LIMIT_VAL_ADDRESS, &Inverter_Info.Set_Info.Const_V_Charge_I_Limit_Val, 2);
				}
			}
		break;
		case 0x010A://恒功率模式（直流功率）
			/*if(PC_CAN_SET_DATA && SYS_NO_FAULT && (Inverter_Info.Com_Info.Grid_or_Offgrid==GRID))
			{
				if(abs((int16_t)U16_Val) <= P_Range[Inverter_Info.Com_Info.Device_Type].Const_P_Charge_P_DC.Max)
				{
					Inverter_Info.Set_Info.Const_P_Charge_P_DC = (int16_t)U16_Val;
					(void)write_eeprom(CONST_P_CHARGE_P_DC_ADDRESS, &Inverter_Info.Set_Info.Const_P_Charge_P_DC, 2);
					if(temp==2)
					{
						if(CURRENT_STATUS_NOT_IND_INVERTER(Inverter_Info.Run_Info.Run_Status))
						{
							if((Inverter_Info.Com_Info.IO_To_BMS_Mode == COM_BMS_MODE_NONE)||(D_BMS_WORK_OK || D_BMS_DISCHARGE_EMPTY))
							{
								Inverter_Info.Set_Info.Run_Mode=((int16_t)U16_Val)<0?1<<5:1<<10;
								SYS_Run_Mode_Last = Inverter_Info.Set_Info.Run_Mode;
								Send_Data_To_DSP_Flag = true;
								Inverter_Info.Com_Info.Open_or_Close=OPEN;
							}
						}
					}
				}
			}*/
			
			if(PC_CAN_SET_DATA && SYS_NO_FAULT && (Inverter_Info.Com_Info.Grid_or_Offgrid==GRID))
			{
				if(abs((int16_t)U16_Val) <= P_Range[Inverter_Info.Com_Info.Device_Type].Const_P_Charge_P_DC.Max)
				{
					Inverter_Info.Set_Info.Const_P_Charge_P_DC = (int16_t)U16_Val;
					(void)write_eeprom(CONST_P_CHARGE_P_DC_ADDRESS, &Inverter_Info.Set_Info.Const_P_Charge_P_DC, 2);
					if(temp==2)
					{
						if(CURRENT_STATUS_NOT_IND_INVERTER(Inverter_Info.Run_Info.Run_Status))
						{
						//edit by sundl 2016-1-28
							if(((int16_t)U16_Val)<0)
							{
								if((Inverter_Info.Com_Info.IO_To_BMS_Mode == COM_BMS_MODE_NONE)||(D_BMS_WORK_OK || D_BMS_DISCHARGE_EMPTY))
								{
									Inverter_Info.Set_Info.Run_Mode=1<<5;
									SYS_Run_Mode_Last = Inverter_Info.Set_Info.Run_Mode;
									Send_Data_To_DSP_Flag = true;
									Inverter_Info.Com_Info.Open_or_Close=OPEN;
								}
							}
							else
							{
								if((Inverter_Info.Com_Info.IO_To_BMS_Mode == COM_BMS_MODE_NONE)||(D_BMS_WORK_OK || D_BMS_CHARGE_FULL))
								{
									Inverter_Info.Set_Info.Run_Mode=1<<10;
									SYS_Run_Mode_Last = Inverter_Info.Set_Info.Run_Mode;
									Send_Data_To_DSP_Flag = true;
									Inverter_Info.Com_Info.Open_or_Close=OPEN;
								}
							}
							
						}
					}
				}
			}
			
		break;
		case 0x010E://恒功率模式（交流功率）
			/*if(PC_CAN_SET_DATA && SYS_NO_FAULT && (Inverter_Info.Com_Info.Grid_or_Offgrid==GRID))
			{
				if(abs((int16_t)U16_Val) <= P_Range[Inverter_Info.Com_Info.Device_Type].Const_P_Charge_P_DC.Max)
				{
					Inverter_Info.Set_Info.Const_P_Charge_P_AC = (int16_t)U16_Val;
					(void)write_eeprom(CONST_P_CHARGE_P_AC_ADDRESS, &Inverter_Info.Set_Info.Const_P_Charge_P_AC, 2);
					if(temp==3)
					{
						if(CURRENT_STATUS_NOT_IND_INVERTER(Inverter_Info.Run_Info.Run_Status))
						{
							if((Inverter_Info.Com_Info.IO_To_BMS_Mode == COM_BMS_MODE_NONE)||(D_BMS_WORK_OK || D_BMS_DISCHARGE_EMPTY))
							{
								Inverter_Info.Set_Info.Run_Mode=((int16_t)U16_Val)<0?1<<6:1<<11;
								SYS_Run_Mode_Last = Inverter_Info.Set_Info.Run_Mode;
								Send_Data_To_DSP_Flag = true;
								Inverter_Info.Com_Info.Open_or_Close=OPEN;
							}
						}
					}
				}
			}*/
			
			if(PC_CAN_SET_DATA && SYS_NO_FAULT && (Inverter_Info.Com_Info.Grid_or_Offgrid==GRID))
			{
				if(abs((int16_t)U16_Val) <= P_Range[Inverter_Info.Com_Info.Device_Type].Const_P_Charge_P_DC.Max)
				{
					Inverter_Info.Set_Info.Const_P_Charge_P_AC = (int16_t)U16_Val;
					(void)write_eeprom(CONST_P_CHARGE_P_AC_ADDRESS, &Inverter_Info.Set_Info.Const_P_Charge_P_AC, 2);
					if(temp==3)
					{
						if(CURRENT_STATUS_NOT_IND_INVERTER(Inverter_Info.Run_Info.Run_Status))
						{
						//edit by sundl 2016-1-28
							if(((int16_t)U16_Val)<0)
							{
								if((Inverter_Info.Com_Info.IO_To_BMS_Mode == COM_BMS_MODE_NONE)||(D_BMS_WORK_OK || D_BMS_DISCHARGE_EMPTY))
								{
									Inverter_Info.Set_Info.Run_Mode=1<<6;
									SYS_Run_Mode_Last = Inverter_Info.Set_Info.Run_Mode;
									Send_Data_To_DSP_Flag = true;
									Inverter_Info.Com_Info.Open_or_Close=OPEN;
								}
							}
							else
							{
								if((Inverter_Info.Com_Info.IO_To_BMS_Mode == COM_BMS_MODE_NONE)||(D_BMS_WORK_OK || D_BMS_CHARGE_FULL))
								{
									Inverter_Info.Set_Info.Run_Mode=1<<11;
									SYS_Run_Mode_Last = Inverter_Info.Set_Info.Run_Mode;
									Send_Data_To_DSP_Flag = true;
									Inverter_Info.Com_Info.Open_or_Close=OPEN;
								}
							}
						
						}
					}
				}
			}
		break;
        case 0x010F://独立逆变电压
            if(PC_CAN_SET_DATA)
            {
                if((U16_Val >= P_Range[Inverter_Info.Com_Info.Device_Type].Ind_Inverter_V.Min) &&
                   (U16_Val <= P_Range[Inverter_Info.Com_Info.Device_Type].Ind_Inverter_V.Max))
                {
					Inverter_Info.Set_Info.Ind_Inverter_V = U16_Val;
					(void)write_eeprom(IND_INVERTER_V_ADDRESS, &Inverter_Info.Set_Info.Ind_Inverter_V, 2);
					Send_Data_To_DSP_Flag = true;
				}
			}
		break;
        case 0x0110://独立逆变频率
            if(PC_CAN_SET_DATA)
            {
			    //liy2015.03.12 start 
			    //delet
                /*if((U16_Val >= P_Range[Inverter_Info.Com_Info.Device_Type].Ind_Inverter_F[Inverter_Info.Com_Info.Grid_Feq].Min) &&
                   (U16_Val <= P_Range[Inverter_Info.Com_Info.Device_Type].Ind_Inverter_F[Inverter_Info.Com_Info.Grid_Feq].Max))
                {
					Inverter_Info.Set_Info.Ind_Inverter_F = U16_Val;
					(void)write_eeprom(IND_INVERTER_F_ADDRESS, &Inverter_Info.Set_Info.Ind_Inverter_F, 2);
					Send_Data_To_DSP_Flag = true;
				}*/
					//add
					if(CURRENT_STATUS_VSG_INVERTER(Inverter_Info.Run_Info.Run_Status))
					{
						if((U16_Val >= P_Range[Inverter_Info.Com_Info.Device_Type].Ind_Inverter_F[Inverter_Info.Com_Info.Grid_Feq].Min) &&
						 (U16_Val <= P_Range[Inverter_Info.Com_Info.Device_Type].Ind_Inverter_F[Inverter_Info.Com_Info.Grid_Feq].Max))
						{
							Inverter_Info.Set_Info.Ind_Inverter_F = U16_Val;
							(void)write_eeprom(IND_INVERTER_F_ADDRESS, &Inverter_Info.Set_Info.Ind_Inverter_F, 2);
							Send_Data_To_DSP_Flag = true;								
						}
					}
					
					else
					{
					#if _FREQ_BOTH_
						if(((U16_Val >= P_Range[Inverter_Info.Com_Info.Device_Type].Ind_Inverter_F[F_50Hz].Min) &&
							 (U16_Val <= P_Range[Inverter_Info.Com_Info.Device_Type].Ind_Inverter_F[F_50Hz].Max)) ||
							((U16_Val >= P_Range[Inverter_Info.Com_Info.Device_Type].Ind_Inverter_F[F_60Hz].Min) &&
							 (U16_Val <= P_Range[Inverter_Info.Com_Info.Device_Type].Ind_Inverter_F[F_60Hz].Max)))
						{
							Inverter_Info.Set_Info.Ind_Inverter_F = U16_Val;
							(void)write_eeprom(IND_INVERTER_F_ADDRESS, &Inverter_Info.Set_Info.Ind_Inverter_F, 2);
							Send_Data_To_DSP_Flag = true;
						}
					#else
						if((U16_Val >= P_Range[Inverter_Info.Com_Info.Device_Type].Ind_Inverter_F[Inverter_Info.Com_Info.Grid_Feq].Min) &&
							 (U16_Val <= P_Range[Inverter_Info.Com_Info.Device_Type].Ind_Inverter_F[Inverter_Info.Com_Info.Grid_Feq].Max))
							
						{
							Inverter_Info.Set_Info.Ind_Inverter_F = U16_Val;
							(void)write_eeprom(IND_INVERTER_F_ADDRESS, &Inverter_Info.Set_Info.Ind_Inverter_F, 2);
							Send_Data_To_DSP_Flag = true;
						}
					#endif
					}
				  //liy2015.03.12 stop
			}
		break;
		case 0x0111://并网、离网、混合
			if(PC_CAN_SET_DATA&&(!LED_STATUS_RUN))
			{
				if(U16_Val==GRID||U16_Val==OFFGRID||U16_Val==MIXED)
				{
					if((Inverter_Info.Com_Info.Grid_or_Offgrid == GRID) && (U16_Val==OFFGRID))//并网转离网
					{
						if(Inverter_Info.Run_Info.Grid_Status==1)//有电网
						{
							//不能转
						}
						else//无电网
						{
							Inverter_Info.Com_Info.Grid_or_Offgrid = U16_Val;
							(void)write_eeprom(GRID_OR_OFFGRID_ADDRESS, &Inverter_Info.Com_Info.Grid_or_Offgrid, 1);
							Send_Data_To_DSP_Flag = true;
						}
					}
					else if((Inverter_Info.Com_Info.Grid_or_Offgrid == OFFGRID) && (U16_Val==GRID))//离网转并网
					{
						Inverter_Info.Com_Info.Grid_or_Offgrid = U16_Val;
						(void)write_eeprom(GRID_OR_OFFGRID_ADDRESS, &Inverter_Info.Com_Info.Grid_or_Offgrid, 1);
						Send_Data_To_DSP_Flag = true;
					}
				}
			}
		break;
        case 0x0112://无功调节方式
            if(PC_CAN_SET_DATA)
            {
                if((U16_Val == SHUT_OFF) || (U16_Val == PF) || (U16_Val == QVAR))
                {
					Inverter_Info.Set_Info.Qvar_Adj_Mode = U16_Val;
					(void)write_eeprom(QVAR_ADJ_MODE_ADDRESS, &Inverter_Info.Set_Info.Qvar_Adj_Mode, 2);
					Send_Data_To_DSP_Flag = true;
				}
				if(Inverter_Info.Set_Info.Qvar_Adj_Mode == SHUT_OFF)//关闭
				{
					Inverter_Info.Set_Info.Qvar_Power_Ratio = P_Range[Inverter_Info.Com_Info.Device_Type].Qvar_Power_Ratio.Dft;				
					(void)write_eeprom(QVAR_POWER_RATIO_ADDRESS, &Inverter_Info.Set_Info.Qvar_Power_Ratio, 2);//关闭无功比例
					
					Inverter_Info.Set_Info.Power_Factor = P_Range[Inverter_Info.Com_Info.Device_Type].Power_Factor.Dft;				
					(void)write_eeprom(POWER_FACTOR_ADDRESS, &Inverter_Info.Set_Info.Power_Factor, 2);//关闭功率因数
				}
				else if(Inverter_Info.Set_Info.Qvar_Adj_Mode == PF)
				{
					Inverter_Info.Set_Info.Qvar_Power_Ratio = P_Range[Inverter_Info.Com_Info.Device_Type].Qvar_Power_Ratio.Dft;				
					(void)write_eeprom(QVAR_POWER_RATIO_ADDRESS, &Inverter_Info.Set_Info.Qvar_Power_Ratio, 2);//关闭无功比例
				}
				else if(Inverter_Info.Set_Info.Qvar_Adj_Mode == QVAR)
				{
					Inverter_Info.Set_Info.Power_Factor = P_Range[Inverter_Info.Com_Info.Device_Type].Power_Factor.Dft;				
					(void)write_eeprom(POWER_FACTOR_ADDRESS, &Inverter_Info.Set_Info.Power_Factor, 2);//关闭功率因数
				}
			}
		break;
        case 0x0113://电网电压跌落调节方式
            if(PC_CAN_SET_DATA)
            {
                if((U16_Val == ISLAND) || (U16_Val == LVRT) || (U16_Val == SEAMLESS))
                {
					Inverter_Info.Set_Info.Grid_V_Drop_Adj_Mode = U16_Val;
					(void)write_eeprom(GRID_V_DROP_ADJ_MODE_ADDRESS, &Inverter_Info.Set_Info.Grid_V_Drop_Adj_Mode, 2);
					Send_Data_To_DSP_Flag = true;
				}
			}
		break;
        case 0x0114://功率因数
            if(PC_CAN_SET_DATA)
            {
				if(Inverter_Info.Set_Info.Qvar_Adj_Mode == PF)//ccg 2013.5.2
				{
					if((U16_Val & (uint16_t)(1<<15)) != 0)//负数
					{
						if(((65536 - U16_Val) >= P_Range[Inverter_Info.Com_Info.Device_Type].Power_Factor.Min) &&
						   ((65536 - U16_Val) <= P_Range[Inverter_Info.Com_Info.Device_Type].Power_Factor.Max))
						{
							Inverter_Info.Set_Info.Power_Factor = (int16_t)U16_Val;
							(void)write_eeprom(POWER_FACTOR_ADDRESS, &Inverter_Info.Set_Info.Power_Factor, 2);
							Send_Data_To_DSP_Flag = true;
						}
					}
					else
					{
						if((U16_Val >= P_Range[Inverter_Info.Com_Info.Device_Type].Power_Factor.Min) &&
						   (U16_Val <= P_Range[Inverter_Info.Com_Info.Device_Type].Power_Factor.Max))
						{
							Inverter_Info.Set_Info.Power_Factor = (int16_t)U16_Val;
							(void)write_eeprom(POWER_FACTOR_ADDRESS, &Inverter_Info.Set_Info.Power_Factor, 2);
							Send_Data_To_DSP_Flag = true;
						}
					}
				}
			}
		break;
        case 0x0115://无功比例
            if(PC_CAN_SET_DATA)//ccg 2013.5.2
            {
				if(Inverter_Info.Set_Info.Qvar_Adj_Mode == QVAR)
				{
					if(((int16_t)U16_Val >= P_Range[Inverter_Info.Com_Info.Device_Type].Qvar_Power_Ratio.Min) &&
					   ((int16_t)U16_Val <= P_Range[Inverter_Info.Com_Info.Device_Type].Qvar_Power_Ratio.Max))
					{
						Inverter_Info.Set_Info.Qvar_Power_Ratio = (int16_t)U16_Val;
						(void)write_eeprom(QVAR_POWER_RATIO_ADDRESS, &Inverter_Info.Set_Info.Qvar_Power_Ratio, 2);
						Send_Data_To_DSP_Flag = true;
					}
				}
			}
		break;
		case 0x0116://并网模式设置
			if(PC_CAN_SET_DATA&&(Inverter_Info.Com_Info.Grid_or_Offgrid==GRID))
			{
				if(U16_Val<=5)
				{
					temp=U16_Val;
					switch(U16_Val)
					{
						case 4:
							if(SYS_NO_FAULT)//待机
							{
								Inverter_Info.Set_Info.Run_Mode=1<<2;
								SYS_Run_Mode_Last = Inverter_Info.Set_Info.Run_Mode;
								Send_Data_To_DSP_Flag = true;
								Inverter_Info.Com_Info.Open_or_Close=STANDBY;
							}
						break;
						case 5:
							if(SYS_NO_FAULT)//停机
							{
								Inverter_Info.Set_Info.Run_Mode=1<<1;
								SYS_Run_Mode_Last = Inverter_Info.Set_Info.Run_Mode;
								Send_Data_To_DSP_Flag = true;
								Inverter_Info.Com_Info.Open_or_Close=CLOSE;
							}							
						break;
					}
				}
			}
		break;
		case 0x0117://离网主动、被动
			if(PC_CAN_SET_DATA&&(Inverter_Info.Com_Info.Grid_or_Offgrid==OFFGRID))
			{
				if(U16_Val==ACTIVE||U16_Val==PASSIVE)
				{
					Inverter_Info.Com_Info.Active_or_Passive = U16_Val;
					(void)write_eeprom(ACTIVE_OR_PASSIVE_ADDRESS, &Inverter_Info.Com_Info.Active_or_Passive, 1);
				}
			}
		break;
		case 0x0118://离网停机、启动
			if(PC_CAN_SET_DATA&&(Inverter_Info.Com_Info.Grid_or_Offgrid==OFFGRID))
			{
				if(U16_Val==CLOSE||U16_Val==OPEN||U16_Val==STANDBY)
				{
					Inverter_Info.Com_Info.Open_or_Close = U16_Val;
				}
			}
		break;
		case 0x0135://充电量补偿值
			if(U16_Val==0)
			{
				Inverter_Info.Com_Info.Charge_Compensate=0;
			}
			else if((U16_Val & (uint16_t)(1<<15)) != 0)//说明是负数
			{
				if(Inverter_Info.Com_Info.Total_Input_Energy_Last>=((UINT16_MAX + 1 - U16_Val)*10))
				{
					Inverter_Info.Com_Info.Total_Input_Energy_Last -= (UINT16_MAX + 1 - U16_Val)*10;
					Inverter_Info.Com_Info.Charge_Compensate=(int16_t)U16_Val;
				}
				else
				{
					Inverter_Info.Com_Info.Charge_Compensate=0;
				}
			}
			else
			{
				if((U16_Val*10)<=(UINT32_MAX - Inverter_Info.Com_Info.Total_Input_Energy_Last))
				{
					Inverter_Info.Com_Info.Total_Input_Energy_Last += U16_Val*10;
					Inverter_Info.Com_Info.Charge_Compensate=(int16_t)U16_Val;
				}
				else
				{
					Inverter_Info.Com_Info.Charge_Compensate=0;
				}
			}
			Inverter_Info.Com_Info.Total_Input_Energy =
			Inverter_Info.Com_Info.Total_Input_Energy_Last + 
			Inverter_Info.Com_Info.Charge_Accumulation/360000.0;//总充电量
		break;
        case 0x0136://放电量补偿值
			if(U16_Val==0)
			{
				Inverter_Info.Com_Info.Discharge_Compensate=0;
			}
			else if((U16_Val & (uint16_t)(1<<15)) != 0)//说明是负数
			{
				if(Inverter_Info.Com_Info.Total_Output_Energy_Last>=((UINT16_MAX + 1 - U16_Val)*10))
				{
					Inverter_Info.Com_Info.Total_Output_Energy_Last -= (UINT16_MAX + 1 - U16_Val)*10;
					Inverter_Info.Com_Info.Discharge_Compensate=(int16_t)U16_Val;
				}
				else
				{
					Inverter_Info.Com_Info.Discharge_Compensate=0;
				}
			}
			else
			{
				if((U16_Val*10)<=(UINT32_MAX - Inverter_Info.Com_Info.Total_Output_Energy_Last))
				{
					Inverter_Info.Com_Info.Total_Output_Energy_Last += U16_Val*10;
					Inverter_Info.Com_Info.Discharge_Compensate=(int16_t)U16_Val;
				}
				else
				{
					Inverter_Info.Com_Info.Discharge_Compensate=0;
				}
			}
			Inverter_Info.Com_Info.Total_Output_Energy =
			Inverter_Info.Com_Info.Total_Output_Energy_Last + 
			Inverter_Info.Com_Info.Discharge_Accumulation/360000.0;//总放电量 
		break;
		case 0x0137://IP地址[0]
			if(U16_Val <= 255)
			{
				Inverter_Info.Com_Info.Net.ip[0] = (uint8_t)U16_Val;
				(void)write_eeprom(NET_IP_0_ADDRESS, &Inverter_Info.Com_Info.Net.ip[0], 1);
// 				Com_Data.Touch_Set_Net_Flag = true;
			}
		break;
		case 0x0138://IP地址[1]
			if(U16_Val <= 255)
			{
				Inverter_Info.Com_Info.Net.ip[1] = (uint8_t)U16_Val;
				(void)write_eeprom(NET_IP_1_ADDRESS, &Inverter_Info.Com_Info.Net.ip[1], 1);
// 				Com_Data.Touch_Set_Net_Flag = true;
			}
		break;
		case 0x0139://IP地址[2]
			if(U16_Val <= 255)
			{
				Inverter_Info.Com_Info.Net.ip[2] = (uint8_t)U16_Val;
				(void)write_eeprom(NET_IP_2_ADDRESS, &Inverter_Info.Com_Info.Net.ip[2], 1);
// 				Com_Data.Touch_Set_Net_Flag = true;
			}
		break;
		case 0x013A://IP地址[3]
			if(U16_Val <= 255)
			{
				Inverter_Info.Com_Info.Net.ip[3] = (uint8_t)U16_Val;
				(void)write_eeprom(NET_IP_3_ADDRESS, &Inverter_Info.Com_Info.Net.ip[3], 1);
// 				Com_Data.Touch_Set_Net_Flag = true;
			}
		break;
		case 0x013B://子网掩码[0]
			if(U16_Val <= 255)
			{
				Inverter_Info.Com_Info.Net.submask[0] = (uint8_t)U16_Val;
				(void)write_eeprom(NET_SUBMASK_0_ADDRESS, &Inverter_Info.Com_Info.Net.submask[0], 1);
// 				Com_Data.Touch_Set_Net_Flag = true;
			}
		break;
		case 0x013C://子网掩码[1]
			if(U16_Val <= 255)
			{
				Inverter_Info.Com_Info.Net.submask[1] = (uint8_t)U16_Val;
				(void)write_eeprom(NET_SUBMASK_1_ADDRESS, &Inverter_Info.Com_Info.Net.submask[1], 1);
// 				Com_Data.Touch_Set_Net_Flag = true;
			}
		break;
		case 0x013D://子网掩码[2]
			if(U16_Val <= 255)
			{
				Inverter_Info.Com_Info.Net.submask[2] = (uint8_t)U16_Val;
				(void)write_eeprom(NET_SUBMASK_2_ADDRESS, &Inverter_Info.Com_Info.Net.submask[2], 1);
// 				Com_Data.Touch_Set_Net_Flag = true;
			}
		break;
		case 0x013E://子网掩码[3]
			if(U16_Val <= 255)
			{
				Inverter_Info.Com_Info.Net.submask[3] = (uint8_t)U16_Val;
				(void)write_eeprom(NET_SUBMASK_3_ADDRESS, &Inverter_Info.Com_Info.Net.submask[3], 1);
// 				Com_Data.Touch_Set_Net_Flag = true;
			}
		break;
		case 0x013F://网关[0]
			if(U16_Val <= 255)
			{
				Inverter_Info.Com_Info.Net.gateway[0] = (uint8_t)U16_Val;
				(void)write_eeprom(NET_GATEWAY_0_ADDRESS, &Inverter_Info.Com_Info.Net.gateway[0], 1);
// 				Com_Data.Touch_Set_Net_Flag = true;
			}
		break;
		case 0x0140://网关[1]
			if(U16_Val <= 255)
			{
				Inverter_Info.Com_Info.Net.gateway[1] = (uint8_t)U16_Val;
				(void)write_eeprom(NET_GATEWAY_1_ADDRESS, &Inverter_Info.Com_Info.Net.gateway[1], 1);
// 				Com_Data.Touch_Set_Net_Flag = true;
			}
		break;
		case 0x0141://网关[2]
			if(U16_Val <= 255)
			{
				Inverter_Info.Com_Info.Net.gateway[2] = (uint8_t)U16_Val;
				(void)write_eeprom(NET_GATEWAY_2_ADDRESS, &Inverter_Info.Com_Info.Net.gateway[2], 1);
// 				Com_Data.Touch_Set_Net_Flag = true;
			}
		break;
		case 0x0142://网关[3]
			if(U16_Val <= 255)
			{
				Inverter_Info.Com_Info.Net.gateway[3] = (uint8_t)U16_Val;
				(void)write_eeprom(NET_GATEWAY_3_ADDRESS, &Inverter_Info.Com_Info.Net.gateway[3], 1);
// 				Com_Data.Touch_Set_Net_Flag = true;
			}
		break;
		case 0x0143://波特率
			if(1 <= U16_Val && U16_Val <= 4)
			{
				Inverter_Info.Com_Info.Baud_Rate=(uint8_t)U16_Val;
				(void)write_eeprom(BAUD_RATE_ADDRESS, &Inverter_Info.Com_Info.Baud_Rate, 1);
				uint16_t baudrate=0;
				switch(Inverter_Info.Com_Info.Baud_Rate)
				{
					case 1:baudrate=4800;break;
					case 2:baudrate=9600;break;
					case 3:baudrate=19200;break;
					case 4:baudrate=38400;break;
				}
				(void)drv_ioctl(gt_glp.fserial1,USART_SET_BAUD,&baudrate);
			}
		break;
		case 0x0144://上位机通讯地址
			if((1 <= U16_Val) && (U16_Val <= 247))
			{
				Inverter_Info.Com_Info.Modbus_Address=(uint8_t)U16_Val;
				(void)write_eeprom(MODBUS_ADDRESS_ADDRESS, &Inverter_Info.Com_Info.Modbus_Address, 1);
			}
		break;
		case 0x0145://端口号
			if((1 <= U16_Val) && (U16_Val <= UINT16_MAX) && (U16_Val != WEBSERVER_PORT) && (U16_Val != IEC104_PORT))
			{
				Inverter_Info.Com_Info.Modbus_Port = U16_Val;
				(void)write_eeprom(NET_PORT_ADDRESS, &Inverter_Info.Com_Info.Modbus_Port, 2);
				Com_Data.Touch_Set_Net_Port_Flag = true;
			}
		break;
        case 0x0146://BMS通讯方式
			if((U16_Val==COM_BMS_MODE_CAN) || (U16_Val==COM_BMS_MODE_UART485) || (U16_Val==COM_BMS_MODE_NONE))
			{
				Inverter_Info.Com_Info.IO_To_BMS_Mode=(uint8_t)U16_Val;
				(void)write_eeprom(IO_BMS_COM_MODE_ADDRESS, &Inverter_Info.Com_Info.IO_To_BMS_Mode, 1);
				if((uint8_t)U16_Val==COM_BMS_MODE_NONE)
				{
					Inverter_Info.Com_Info.BMS_Fault_Contact_Enabled = SWITCH_OFF;
					(void)write_eeprom(BMS_FAULT_CONTACT_ENABLED_ADDRESS, &Inverter_Info.Com_Info.BMS_Fault_Contact_Enabled, 1);
					Inverter_Info.Com_Info.BMS_Warning_Contact_Enabled = SWITCH_OFF;
					(void)write_eeprom(BMS_WARNING_CONTACT_ENABLED_ADDRESS, &Inverter_Info.Com_Info.BMS_Warning_Contact_Enabled, 1);
				}
			}
		break;
        case 0x0147://BMS故障干接点使能
			if(Inverter_Info.Com_Info.IO_To_BMS_Mode!=COM_BMS_MODE_NONE)
			{
				if((U16_Val == SWITCH_OFF) || (U16_Val == SWITCH_ON_OPEN) || (U16_Val == SWITCH_ON_CLOSE))
				{
					Inverter_Info.Com_Info.BMS_Fault_Contact_Enabled = (uint8_t)U16_Val;
					(void)write_eeprom(BMS_FAULT_CONTACT_ENABLED_ADDRESS, &Inverter_Info.Com_Info.BMS_Fault_Contact_Enabled, 1);
				}
			}
		break;
        case 0x0148://BMS告警干接点使能
			if(Inverter_Info.Com_Info.IO_To_BMS_Mode!=COM_BMS_MODE_NONE)
			{
				if((U16_Val == SWITCH_OFF) || (U16_Val == SWITCH_ON_OPEN) || (U16_Val == SWITCH_ON_CLOSE))
				{
					Inverter_Info.Com_Info.BMS_Warning_Contact_Enabled = (uint8_t)U16_Val;
					(void)write_eeprom(BMS_WARNING_CONTACT_ENABLED_ADDRESS, &Inverter_Info.Com_Info.BMS_Warning_Contact_Enabled, 1);
				}
			}
		break;
		case 0x0149://BMS通讯地址
			if((1 <= U16_Val) && (U16_Val <= 247))
			{
				Inverter_Info.Com_Info.BMS_Address=(uint8_t)U16_Val;
				(void)write_eeprom(BMS_ADDRESS_ADDRESS, &Inverter_Info.Com_Info.BMS_Address, 1);
			}
		break;
		case 0x014A://IEC104通讯地址
			if((1 <= U16_Val) && (U16_Val <= 65535))
			{
				Inverter_Info.Com_Info.IEC104_Address=U16_Val;
				(void)write_eeprom(IEC104_ADDRESS_ADDRESS, &Inverter_Info.Com_Info.IEC104_Address, 2);
			}
		break;
		case 0x014B://IEC104超时时间T1
 			if((1 <= U16_Val) && (U16_Val < Inverter_Info.Com_Info.IEC104_T3))
			{
				Inverter_Info.Com_Info.IEC104_T1=U16_Val;
				(void)write_eeprom(IEC104_T1_ADDRESS, &Inverter_Info.Com_Info.IEC104_T1, 2);
			}
		break;
		case 0x014C://IEC104超时时间T3
 			if((Inverter_Info.Com_Info.IEC104_T1 < U16_Val) && (U16_Val <= 255))
			{
				Inverter_Info.Com_Info.IEC104_T3=U16_Val;
				(void)write_eeprom(IEC104_T3_ADDRESS, &Inverter_Info.Com_Info.IEC104_T3, 2);
			}
		break;
		case 0x014D://IEC104参数k
 			if((Inverter_Info.Com_Info.IEC104_W<=((uint16_t)U16_Val*2/3))&&(U16_Val<=32767))
			{
				Inverter_Info.Com_Info.IEC104_K = U16_Val;
				(void)write_eeprom(IEC104_K_ADDRESS,&Inverter_Info.Com_Info.IEC104_K,2);
			}
		break;
		case 0x014E://IEC104参数w
 			if((1<=U16_Val)&&(U16_Val<=((uint16_t)Inverter_Info.Com_Info.IEC104_K*2/3)))
			{
				Inverter_Info.Com_Info.IEC104_W = U16_Val;
				(void)write_eeprom(IEC104_W_ADDRESS,&Inverter_Info.Com_Info.IEC104_W,2);
			}
		break;
        case 0x0301://页面语言
			if(U16_Val == CHINESE || U16_Val == ENGLISH)
			{
				Webserver_Language = (uint8_t)U16_Val;
				(void)write_eeprom(WEBSERVER_LANGUAGE_ADDRESS, &Webserver_Language, 1);
			}
		break;
        default:
        break;
    }
    return ~0;
}
uint32_t get_param(uint32_t addr)								//获取参数
{
    uint32_t value = 0;
    switch(addr)
    {   
        case 0x0001:value = Device_Type_Code[Inverter_Info.Com_Info.Device_Type];break;							//设备类型代码
        case 0x0002:value = Inverter_Info.Com_Info.Interver_EFF;break;											//工作效率
        case 0x0003:value = Inverter_Info.Run_Info.DC_Voltage;break;											//直流电压
        case 0x0004:value = Inverter_Info.Run_Info.DC_Current;break;											//直流电流
        case 0x0005:value = Inverter_Info.Com_Info.DC_Power;break;												//直流功率
        case 0x0006:value = gATTPara.dis_St;break;																//视在功率
        case 0x0007:value = gATTPara.dis_Pt;break;																//有功功率
        case 0x0008:value = gATTPara.dis_Qt;break;																//无功功率
        case 0x0009:value = gATTPara.dis_Pft;break;																//功率因数
        case 0x000A:value = gATTPara.dis_Va;break;																//A相电压
        case 0x000B:value = gATTPara.dis_Ia;break;																//A相电流
        case 0x000C:value = gATTPara.dis_Vc;break;																//B相电压
        case 0x000D:value = gATTPara.dis_Ib;break;																//B相电流
        case 0x000E:value = gATTPara.dis_Vc;break;																//C相电压
        case 0x000F:value = gATTPara.dis_Ic;break;																//C相电流
        case 0x0010:value = Inverter_Info.Run_Info.AC_Frequency;break;											//电网频率
        case 0x0011:value = Inverter_Info.Run_Info.Inside_Temp;break;											//机内温度
        case 0x0012:value = Inverter_Info.Run_Info.A1_M_Temp;break;												//模块A1温度
        case 0x0013:value = Inverter_Info.Run_Info.B1_M_Temp;break;												//模块B1温度
        case 0x0014:value = Inverter_Info.Run_Info.C1_M_Temp;break;												//模块C1温度
        case 0x0015:value = Inverter_Info.Run_Info.A2_M_Temp;break;												//模块A2温度
        case 0x0016:value = Inverter_Info.Run_Info.B2_M_Temp;break;												//模块B2温度
        case 0x0017:value = Inverter_Info.Run_Info.C2_M_Temp;break;												//模块C2温度
        case 0x0018:value = Inverter_Info.Com_Info.Day_Input_Energy;break;										//日充电量
        case 0x0019:value = Inverter_Info.Com_Info.Day_Output_Energy;break;										//日放电量
        case 0x001A:value = Inverter_Info.Com_Info.Month_Input_Energy/10;break;									//月充电量
        case 0x001B:value = Inverter_Info.Com_Info.Month_Output_Energy/10;break;								//月放电量
        case 0x001C:value = Inverter_Info.Com_Info.Total_Input_Energy/10;break;									//总充电量
        case 0x001D:value = Inverter_Info.Com_Info.Total_Output_Energy/10;break;								//总放电量
        case 0x001E:value = Inverter_Info.Com_Info.Day_Input_Time;break;										//日充电时间
        case 0x001F:value = Inverter_Info.Com_Info.Day_Output_Time;break;										//日放电时间
        case 0x0020:value = Inverter_Info.Com_Info.Month_Input_Time/60;break;									//月充电时间
        case 0x0021:value = Inverter_Info.Com_Info.Month_Output_Time/60;break;									//月放电时间
        case 0x0022:value = Inverter_Info.Com_Info.Total_Input_Time/60;break;									//总充电时间
        case 0x0023:value = Inverter_Info.Com_Info.Total_Output_Time/60;break;									//总放电时间
        case 0x0024:value = Inverter_Info.Com_Info.Total_Run_Hour;break;										//总运行时间
        case 0x0025:value = (SYS_NO_FAULT)?(Inverter_Info.Run_Info.Run_Status):(1<<16);break;					//设备状态
        case 0x0026:value = Inverter_Info.Run_Info.Fault_Status1;break;											//故障状态1
        case 0x0027:value = Inverter_Info.Run_Info.Fault_Status2;break;											//故障状态2
		case 0x0028:value = P_Range[Inverter_Info.Com_Info.Device_Type].Const_I_Charge_I.Min;break;				//恒流充电电流下限
		case 0x0029:value = P_Range[Inverter_Info.Com_Info.Device_Type].Const_I_Charge_I.Max;break;				//恒流充电电流上限
		case 0x002A:value = P_Range[Inverter_Info.Com_Info.Device_Type].Const_V_Charge_V.Min;break;				//恒压充电电压下限
		case 0x002B:value = P_Range[Inverter_Info.Com_Info.Device_Type].Const_V_Charge_V.Max;break;				//恒压充电电压上限
		case 0x002C:value = P_Range[Inverter_Info.Com_Info.Device_Type].Const_V_Charge_I_Limit_Val.Min;break;	//恒压充电限流值下限
		case 0x002D:value = P_Range[Inverter_Info.Com_Info.Device_Type].Const_V_Charge_I_Limit_Val.Max;break;	//恒压充电限流值上限
		case 0x002E:value = P_Range[Inverter_Info.Com_Info.Device_Type].Const_P_Charge_P_DC.Min;break;				//恒功率充电功率下限
		case 0x002F:value = P_Range[Inverter_Info.Com_Info.Device_Type].Const_P_Charge_P_DC.Max;break;				//恒功率充电功率上限
		case 0x0030:value = P_Range[Inverter_Info.Com_Info.Device_Type].Const_I_Discharge_I.Min;break;			//恒流放电电流下限
		case 0x0031:value = P_Range[Inverter_Info.Com_Info.Device_Type].Const_I_Discharge_I.Max;break;			//恒流放电电流上限
		case 0x0032:value = P_Range[Inverter_Info.Com_Info.Device_Type].Const_V_Discharge_V.Min;break;			//恒压放电电压下限
		case 0x0033:value = P_Range[Inverter_Info.Com_Info.Device_Type].Const_V_Discharge_V.Max;break;			//恒压放电电压上限
		case 0x0034:value = P_Range[Inverter_Info.Com_Info.Device_Type].Const_V_Discharge_I_Limit_Val.Min;break;//恒压放电限流值下限
		case 0x0035:value = P_Range[Inverter_Info.Com_Info.Device_Type].Const_V_Discharge_I_Limit_Val.Max;break;//恒压放电限流值上限
		case 0x0036:value = P_Range[Inverter_Info.Com_Info.Device_Type].Const_P_Discharge_P_DC.Min;break;			//恒功率放电功率下限
		case 0x0037:value = P_Range[Inverter_Info.Com_Info.Device_Type].Const_P_Discharge_P_DC.Max;break;			//恒功率放电功率上限
		case 0x0038:value = P_Range[Inverter_Info.Com_Info.Device_Type].Ind_Inverter_V.Min;break;				//独立逆变电压下限
		case 0x0039:value = P_Range[Inverter_Info.Com_Info.Device_Type].Ind_Inverter_V.Max;break;				//独立逆变电压上限
		case 0x003A:value = P_Range[Inverter_Info.Com_Info.Device_Type].Ind_Inverter_F[Inverter_Info.Com_Info.Grid_Feq].Min;break;//独立逆变频率下限
		case 0x003B:value = P_Range[Inverter_Info.Com_Info.Device_Type].Ind_Inverter_F[Inverter_Info.Com_Info.Grid_Feq].Max;break;//独立逆变频率上限
		case 0x003C:value = P_Range[Inverter_Info.Com_Info.Device_Type].Power_Factor.Min;break;					//功率因数下限
		case 0x003D:value = P_Range[Inverter_Info.Com_Info.Device_Type].Power_Factor.Max;break;					//功率因数上限
		case 0x003E:value = P_Range[Inverter_Info.Com_Info.Device_Type].Qvar_Power_Ratio.Min;break;				//无功比例下限
		case 0x003F:value = P_Range[Inverter_Info.Com_Info.Device_Type].Qvar_Power_Ratio.Max;break;				//无功比例上限
		case 0x0040:value=!Inverter_Info.Com_Info.IO_To_BMS_Mode? 0:                         					//电池组状态 ccg 2013.12.26
						   IO_Check_Fault.BMS_Com_Fault?          6:
						   D_BMS_CHARGE_FULL?                     1:
						   D_BMS_DISCHARGE_EMPTY?                 2:
						   D_BMS_STANDBY?                         3:
						   D_BMS_STOP?                            4:
						   D_BMS_WORK_OK?                         5:0;break;
        case 0x0041:value = Inverter_Info.Com_Info.Year_Input_Energy/10;break;									//年充电量
        case 0x0042:value = Inverter_Info.Com_Info.Year_Output_Energy/10;break;									//年放电量
        case 0x0043:value = Inverter_Info.Com_Info.Year_Input_Time/60;break;									//年充电时间
        case 0x0044:value = Inverter_Info.Com_Info.Year_Output_Time/60;break;									//年放电时间

		case 0x0101:value = BcdToBin(showTime.tm_year)+2000;break;												//系统时间（年）
		case 0x0102:value = BcdToBin(showTime.tm_mon);break;													//系统时间（月）
		case 0x0103:value = BcdToBin(showTime.tm_day);break;													//系统时间（日）
		case 0x0104:value = BcdToBin(showTime.tm_hour);break;													//系统时间（时）
		case 0x0105:value = BcdToBin(showTime.tm_min);break;													//系统时间（分）
		case 0x0106:value = BcdToBin(showTime.tm_sec);break;													//系统时间（秒）
		case 0x0107:value = Inverter_Info.Set_Info.Const_I_Charge_I;break;										//恒流充电电流
		case 0x0108:value = Inverter_Info.Set_Info.Const_V_Charge_V;break;										//恒压充电电压
		case 0x0109:value = Inverter_Info.Set_Info.Const_V_Charge_I_Limit_Val;break;							//恒压充电限制电流
		case 0x010A:value = Inverter_Info.Set_Info.Const_P_Charge_P_DC;break;									//恒功率充电功率
// 		case 0x010B:value = Inverter_Info.Set_Info.Const_I_Discharge_I;break;									//恒流放电电流
// 		case 0x010C:value = Inverter_Info.Set_Info.Const_V_Discharge_V;break;									//限压放电电压
// 		case 0x010D:value = Inverter_Info.Set_Info.Const_V_Discharge_I_Limit_Val;break;							//限压放电限制电流
		case 0x010E:value = Inverter_Info.Set_Info.Const_P_Charge_P_AC;break;									//恒功率放电功率
		case 0x010F:value = Inverter_Info.Set_Info.Ind_Inverter_V;break;										//独立逆变电压
		case 0x0110:value = Inverter_Info.Set_Info.Ind_Inverter_F;break;										//独立逆变频率
		case 0x0111:value = Inverter_Info.Com_Info.Grid_or_Offgrid;break;										//工作模式
		case 0x0112:value = Inverter_Info.Set_Info.Qvar_Adj_Mode;break;											//无功调节方式
		case 0x0113:value = Inverter_Info.Set_Info.Grid_V_Drop_Adj_Mode;break;									//电网电压跌落调节方式
		case 0x0114:value = Inverter_Info.Set_Info.Power_Factor;break;											//功率因数
		case 0x0115:value = Inverter_Info.Set_Info.Qvar_Power_Ratio;break;										//无功比例
		case 0x0116:value = CURRENT_STATUS_CC_DC_MODE(Inverter_Info.Run_Info.Run_Status)? 0://并网模式
							CURRENT_STATUS_CV_DC_MODE(Inverter_Info.Run_Info.Run_Status)? 1:
							CURRENT_STATUS_CP_DC_MODE(Inverter_Info.Run_Info.Run_Status)? 2:
							CURRENT_STATUS_CP_AC_MODE(Inverter_Info.Run_Info.Run_Status)? 3:
					 CURRENT_STATUS_GRID_STANDBY_MODE(Inverter_Info.Run_Info.Run_Status)? 4:
						CURRENT_STATUS_GRID_STOP_MODE(Inverter_Info.Run_Info.Run_Status)? 5:5;break;										//
		case 0x0117:value = Inverter_Info.Com_Info.Active_or_Passive;break;										//主动、被动模式
		case 0x0118:value = CURRENT_STATUS_OFFGRID_START_MODE(Inverter_Info.Run_Info.Run_Status)? 1:			//离网启动、停机
						    (CURRENT_STATUS_OFFGRID_STOP_MODE(Inverter_Info.Run_Info.Run_Status) ? 0:1);break;										//

		case 0x0137:value = Inverter_Info.Com_Info.Net.ip[0];break;												//IP地址[0]
		case 0x0138:value = Inverter_Info.Com_Info.Net.ip[1];break;												//IP地址[1]
		case 0x0139:value = Inverter_Info.Com_Info.Net.ip[2];break;												//IP地址[2]
		case 0x013A:value = Inverter_Info.Com_Info.Net.ip[3];break;												//IP地址[3]
		case 0x013B:value = Inverter_Info.Com_Info.Net.submask[0];break;										//子网掩码[0]
		case 0x013C:value = Inverter_Info.Com_Info.Net.submask[1];break;										//子网掩码[1]
		case 0x013D:value = Inverter_Info.Com_Info.Net.submask[2];break;										//子网掩码[2]
		case 0x013E:value = Inverter_Info.Com_Info.Net.submask[3];break;										//子网掩码[3]
		case 0x013F:value = Inverter_Info.Com_Info.Net.gateway[0];break;										//网关[0]
		case 0x0140:value = Inverter_Info.Com_Info.Net.gateway[1];break;										//网关[1]
		case 0x0141:value = Inverter_Info.Com_Info.Net.gateway[2];break;										//网关[2]
		case 0x0142:value = Inverter_Info.Com_Info.Net.gateway[3];break;										//网关[3]
		case 0x0143:value = Inverter_Info.Com_Info.Baud_Rate;break;												//波特率
		case 0x0144:value = Inverter_Info.Com_Info.Modbus_Address;break;										//上位机通讯地址
		case 0x0145:value = Inverter_Info.Com_Info.Modbus_Port;break;											//modbus端口号
		case 0x0146:value = Inverter_Info.Com_Info.IO_To_BMS_Mode;break;										//BMS通讯方式
		case 0x0147:value = Inverter_Info.Com_Info.BMS_Fault_Contact_Enabled;break;								//BMS故障干接点
		case 0x0148:value = Inverter_Info.Com_Info.BMS_Warning_Contact_Enabled;break;							//BMS告警干接点
		case 0x0149:value = Inverter_Info.Com_Info.BMS_Address;break;											//BMS通讯地址
		case 0x014A:value = Inverter_Info.Com_Info.IEC104_Address;break;										//IEC104通讯地址
		case 0x014B:value = Inverter_Info.Com_Info.IEC104_T1;break;												//IEC104超时时间T1
		case 0x014C:value = Inverter_Info.Com_Info.IEC104_T3;break;												//IEC104超时时间T3
		case 0x014D:value = Inverter_Info.Com_Info.IEC104_K;break;												//IEC104参数k
		case 0x014E:value = Inverter_Info.Com_Info.IEC104_W;break;												//IEC104参数w

		case 0x0206:value = SuperPwd;break;																		//是否是超级密码登录

		case 0x0282:value = userFlag;break;																		//当前用户标识

		case 0x0301:value = Webserver_Language;break;															//页面语言
	}
    return value;
}


void webserver_do_save_int(void)								//异步保存参数值(数值型)
{
	Touch_Write_Time[0]=showTime.tm_year;
	Touch_Write_Time[1]=showTime.tm_mon;
	Touch_Write_Time[2]=showTime.tm_day;
	Touch_Write_Time[3]=showTime.tm_hour;
	Touch_Write_Time[4]=showTime.tm_min;
	Touch_Write_Time[5]=showTime.tm_sec;
	
    for(int i=0;i<PARAMETER_NUM;i++)
    {
        if(Param_flag[i]!=0)
        {
            do_save_data_int(Param_flag[i],Param_value[i]);
            Param_flag[i]=0;
        }
    }
	if(Touch_Write_Time_Flag==true)
	{
		Touch_Write_Time_Flag=false;
		(void)drv_write(gt_glp.frtc,Touch_Write_Time,6);//设置时间函数
		delay10s=true;
		Send_Time_To_DSP_Flag=true;
		Clear_Data = true;								//判断是否是新的一天，是否要清除前一天的数据
	}
}
void s_handler(int id,char* data)								//处理初始化参数值
{
    switch(id)
    {
        case 0: //登录
            sprintf(s_000,
                "HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n"
                "{"
					"ID_0x0202:'%s',"  //设备名称
                    "ID_0x0301:%d"     //页面语言
			    "}",
                    Device_Info[DEVICE_NAME],
                    get_param(0x0301)
			);
            s_Rsc[0].size = strlen(s_000);
        break;
        case 1: //框架
            sprintf(s_000,
                "HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n"
                "{"
					"ID_0x0202:'%s',"  //设备名称
                    "ID_0x0301:%d"     //页面语言
			    "}",
                    Device_Info[DEVICE_NAME],
                    get_param(0x0301)
			);
            s_Rsc[0].size = strlen(s_000);
        break;
		case 2://概要信息
            sprintf(s_000,
                "HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n"
                "{"
                    "ID_0x0301:%d,"   //页面语言
                    "ID_0x0025:%d,"   //当前状态
                    "ID_0x0005:%d,"   //实时功率
                    "ID_0x0024:%d,"   //总运行时间
                    "ID_0x0040:%d,"   //电池状态
                    "ID_0x001C:%d,"   //总充电量
                    "ID_0x001D:%d"    //总放电量
			    "}",
                    get_param(0x0301),
                    get_param(0x0025),
                    get_param(0x0005),
                    get_param(0x0024),
                    get_param(0x0040),
                    get_param(0x001C),
                    get_param(0x001D)
			);
            s_Rsc[0].size = strlen(s_000);
		break;
        case 3: //详细信息
            sprintf(s_000,
                "HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n"
                "{"
                    "ID_0x0001:%d,"   //设备类型
                    "ID_0x0301:%d,"   //页面语言
                    "ID_0x0003:%d,"   //直流电压
                    "ID_0x0004:%d,"   //直流电流
                    "ID_0x0005:%d,"   //直流功率
                    "ID_0x0006:%d,"   //视在功率
                    "ID_0x0007:%d,"   //有功功率
                    "ID_0x0008:%d,"   //无功功率
                    "ID_0x0009:%d,"   //功率因数
                    "ID_0x000A:%d,"   //A相电压
                    "ID_0x000B:%d,"   //A相电流
                    "ID_0x000C:%d,"   //B相电压
                    "ID_0x000D:%d,"   //B相电流
                    "ID_0x000E:%d,"   //C相电压
                    "ID_0x000F:%d,"   //C相电流
                    "ID_0x0010:%d,"   //电网频率
                    "ID_0x0011:%d,"   //机内温度
                    "ID_0x0012:%d,"   //模块A1温度
                    "ID_0x0013:%d,"   //模块B1温度
                    "ID_0x0014:%d,"   //模块C1温度
                    "ID_0x0015:%d,"   //模块A2温度
                    "ID_0x0016:%d,"   //模块B2温度
                    "ID_0x0017:%d,"   //模块C2温度
                    "ID_0x0018:%d,"   //日充电量
                    "ID_0x0019:%d,"   //日放电量
                    "ID_0x001A:%d,"   //月充电量
                    "ID_0x001B:%d,"   //月放电量
                    "ID_0x001C:%d,"   //总充电量
                    "ID_0x001D:%d,"   //总放电量
                    "ID_0x001E:%d,"   //日充电时间
                    "ID_0x001F:%d,"   //日放电时间
                    "ID_0x0020:%d,"   //月充电时间
                    "ID_0x0021:%d,"   //月放电时间
                    "ID_0x0022:%d,"   //总充电时间
                    "ID_0x0023:%d,"   //总放电时间
                    "ID_0x0024:%d,"   //总运行电时间
                    "ID_0x0026:%d,"   //故障状态1
                    "ID_0x0027:%d,"   //故障状态2
                    "ID_0x0041:%d,"   //年充电量
                    "ID_0x0042:%d,"   //年放电量
                    "ID_0x0043:%d,"   //年充电时间
                    "ID_0x0044:%d"    //年放电时间
			    "}",
                    get_param(0x0001),
                    get_param(0x0301),
                    get_param(0x0003),
                    get_param(0x0004),
                    get_param(0x0005),
                    get_param(0x0006),
                    get_param(0x0007),
                    get_param(0x0008),
                    get_param(0x0009),
                    get_param(0x000A),
                    get_param(0x000B),
                    get_param(0x000C),
                    get_param(0x000D),
                    get_param(0x000E),
                    get_param(0x000F),
                    get_param(0x0010),
                    get_param(0x0011),
                    get_param(0x0012),
                    get_param(0x0013),
                    get_param(0x0014),
                    get_param(0x0015),
                    get_param(0x0016),
                    get_param(0x0017),
                    get_param(0x0018),
                    get_param(0x0019),
                    get_param(0x001A),
                    get_param(0x001B),
                    get_param(0x001C),
                    get_param(0x001D),
                    get_param(0x001E),
                    get_param(0x001F),
                    get_param(0x0020),
                    get_param(0x0021),
                    get_param(0x0022),
                    get_param(0x0023),
                    get_param(0x0024),
                    get_param(0x0026),
                    get_param(0x0027),
                    get_param(0x0041),
                    get_param(0x0042),
                    get_param(0x0043),
                    get_param(0x0044)
			);
            s_Rsc[0].size = strlen(s_000);
        break;
        case 4: //系统参数
            sprintf(s_000,
                "HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n"
                "{"
                    "ID_0x0301:%d,"   //页面语言
                    "ID_0x0101:%d,"   //系统时钟(年)
                    "ID_0x0102:%d,"   //系统时钟(月)
                    "ID_0x0103:%d,"   //系统时钟(日)
                    "ID_0x0104:%d,"   //系统时钟(时)
                    "ID_0x0105:%d,"   //系统时钟(分)
                    "ID_0x0282:%d"    //用户名称
			    "}",
                    get_param(0x0301),
                    get_param(0x0101),
                    get_param(0x0102),
                    get_param(0x0103),
                    get_param(0x0104),
                    get_param(0x0105),
                    get_param(0x0282)
			);
            s_Rsc[0].size = strlen(s_000);
        break;
        case 5: //运行参数
            sprintf(s_000,
                "HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n"
                "{"
                    "ID_0x0301:%d,"   //页面语言
                    "ID_0x0107:%d,"   //恒流充电电流
                    "ID_0x0108:%d,"   //恒压充电电压
                    "ID_0x0109:%d,"   //恒压充电限流值
                    "ID_0x010A:%d,"   //恒功率充电功率

                    "ID_0x010E:%d,"   //恒功率放电功率
                    "ID_0x010F:%d,"   //独立逆变电压
                    "ID_0x0110:%d,"   //独立逆变频率
                    "ID_0x0111:%d,"   //工作模式
                    "ID_0x0112:%d,"   //无功调节选择开关
                    "ID_0x0113:%d,"   //电网电压跌落调节方式
                    "ID_0x0114:%d,"   //功率因数
                    "ID_0x0115:%d,"   //无功比例
                    "ID_0x0116:%d,"   //
                    "ID_0x0117:%d,"   //
                    "ID_0x0118:%d,"   //
                    "ID_0x0028:%d,"   //恒流充电电流下限
                    "ID_0x0029:%d,"   //恒流充电电流上限
                    "ID_0x002A:%d,"   //恒压充电电压下限
                    "ID_0x002B:%d,"   //恒压充电电压上限
                    "ID_0x002C:%d,"   //恒压充电限流值下限
                    "ID_0x002D:%d,"   //恒压充电限流值上限
                    "ID_0x002E:%d,"   //恒功率充电功率下限
                    "ID_0x002F:%d,"   //恒功率充电功率上限

                    "ID_0x0036:%d,"   //恒功率放电功率下限
                    "ID_0x0037:%d,"   //恒功率放电功率上限
                    "ID_0x0038:%d,"   //独立逆变电压下限
                    "ID_0x0039:%d,"   //独立逆变电压上限
                    "ID_0x003A:%d,"   //独立逆变频率下限
                    "ID_0x003B:%d,"   //独立逆变频率上限
                    "ID_0x003C:%d,"   //功率因数下限
                    "ID_0x003D:%d,"   //功率因数上限
                    "ID_0x003E:%d,"   //无功比例下限
                    "ID_0x003F:%d,"   //无功比例上限
                    "ID_0x0282:%d"    //用户名称
		
			    "}",
                    get_param(0x0301),
                    get_param(0x0107),
                    get_param(0x0108),
                    get_param(0x0109),
                    get_param(0x010A),

                    get_param(0x010E),
                    get_param(0x010F),
                    get_param(0x0110),
                    get_param(0x0111),
                    get_param(0x0112),
                    get_param(0x0113),
                    get_param(0x0114),
                    get_param(0x0115),
                    get_param(0x0116),
                    get_param(0x0117),
                    get_param(0x0118),
                    get_param(0x0028),
                    get_param(0x0029),
                    get_param(0x002A),
                    get_param(0x002B),
                    get_param(0x002C),
                    get_param(0x002D),
                    get_param(0x002E),
                    get_param(0x002F),

                    get_param(0x0036),
                    get_param(0x0037),
                    get_param(0x0038),
                    get_param(0x0039),
                    get_param(0x003A),
                    get_param(0x003B),
                    get_param(0x003C),
                    get_param(0x003D),
                    get_param(0x003E),
                    get_param(0x003F),
                    get_param(0x0282)
			);
            s_Rsc[0].size = strlen(s_000);
			
        break;
        case 6: //通讯参数
            sprintf(s_000,
                "HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n"
                "{"
                    "ID_0x0301:%d,"   //页面语言
                    "ID_0x0137:%d,"   //IP地址[0]
                    "ID_0x0138:%d,"   //IP地址[1]
                    "ID_0x0139:%d,"   //IP地址[2]
                    "ID_0x013A:%d,"   //IP地址[3]
                    "ID_0x013B:%d,"   //子网掩码[0]
                    "ID_0x013C:%d,"   //子网掩码[1]
                    "ID_0x013D:%d,"   //子网掩码[2]
                    "ID_0x013E:%d,"   //子网掩码[3]
                    "ID_0x013F:%d,"   //网关[0]
                    "ID_0x0140:%d,"   //网关[1]
                    "ID_0x0141:%d,"   //网关[2]
                    "ID_0x0142:%d,"   //网关[3]
                    "ID_0x0143:%d,"   //波特率
                    "ID_0x0144:%d,"   //上位机通讯地址
                    "ID_0x0145:%d,"   //端口号
                    "ID_0x0146:%d,"   //BMS通讯方式
                    "ID_0x0147:%d,"   //BMS故障干接点
                    "ID_0x0148:%d,"   //BMS告警干接点
                    "ID_0x0149:%d,"   //BMS通讯地址
                    "ID_0x014A:%d,"   //IEC104通讯地址
                    "ID_0x014B:%d,"   //IEC104超时时间T1
                    "ID_0x014C:%d,"   //IEC104超时时间T3
                    "ID_0x014D:%d,"   //IEC104参数K
                    "ID_0x014E:%d,"   //IEC104参数W
                    "ID_0x0282:%d"    //用户名称
			    "}",
                    get_param(0x0301),
                    get_param(0x0137),
                    get_param(0x0138),
                    get_param(0x0139),
                    get_param(0x013A),
                    get_param(0x013B),
                    get_param(0x013C),
                    get_param(0x013D),
                    get_param(0x013E),
                    get_param(0x013F),
                    get_param(0x0140),
                    get_param(0x0141),
                    get_param(0x0142),
                    get_param(0x0143),
                    get_param(0x0144),
                    get_param(0x0145),
                    get_param(0x0146),
                    get_param(0x0147),
                    get_param(0x0148),
                    get_param(0x0149),
                    get_param(0x014A),
                    get_param(0x014B),
                    get_param(0x014C),
                    get_param(0x014D),
                    get_param(0x014E),
                    get_param(0x0282)
			);
            s_Rsc[0].size = strlen(s_000);
        break;
        case 10://系统信息
            sprintf(s_000,
				"HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n"
				"{"
                    "ID_0x0301:%d,"    //页面语言 
                    "ID_0x0203:'%s',"  //序列号
					"ID_0x0204:'%s',"  //设备名称
                    "ID_0x0205:'%s',"  //页面语言 
                    "ID_0x0206:%d"     //超级密码
				"}",
                    get_param(0x0301),
					Device_Info[FIRMWARE_VEB1],
					Device_Info[FIRMWARE_VEB2],
                    Device_Info[FIRMWARE_VEB3],
                    get_param(0x0206)
			);
            s_Rsc[0].size = strlen(s_000);
        break;
        case 12://logo
            sprintf(s_000,
				"HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n"
				"{"
                    "ID_0x0201:'%s',"  //序列号
					"ID_0x0202:'%s',"  //设备名称
                    "ID_0x0301:%d,"    //页面语言 
                    "ID_0x0282:%d"     //用户名称
				"}",
					IAT_Info.Set_Info.SN_Number,
                    Device_Info[DEVICE_NAME],
                    get_param(0x0301),
                    get_param(0x0282)
			);
            s_Rsc[0].size = strlen(s_000);
        break;
        case 7://运行记录
        case 8://事件记录
        case 9://故障记录
        case 13://菜单
            sprintf(s_000,
                "HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n"
                "{"
					"ID_0x0301:%d" //页面语言 
			    "}",
                    get_param(0x0301)
			);
            s_Rsc[0].size = strlen(s_000);
        break;
        default:
        break;
    }
}
void a_handler(int id,char* data)								//处理返回的参数，并回应
{
    char *ptemp;//切割出的内容
    char delim[] = "?=&+";//切割字符串
    uint32_t id_No=~0;//参数序号
    int32_t value=~0;//参数值
	uint8_t set_num=0;

    char login=~0;//修改密码时区分是超级密码还是普通密码进入来修改密码的。
    char login_c1=~0;//判断用户名和密码的标识
    char login_c2=~0;//判断用户名和密码的标识
    char login_s1=~0;//判断是否为超级密码登陆
    char login_s2=~0;//判断是否为超级密码登陆
    char login1=~0;//判断是否是用户1登录
    char login2=~0;//判断是否是用户2登录
    static char flag=0;//密码确认还是修改密码
    int i=0;//用于复制密码的循环变量

	
    ptemp = strtok(data,delim);
    ptemp = strtok(NULL,delim);
    while(strcmp(ptemp,"time"))
    {
        if(!strcmp(ptemp,"0x0282"))//用户名确认
        {
            ptemp = strtok(NULL,delim);
            login1=strcmp(ptemp,"0");
            login2=strcmp(ptemp,"1");
        }
        else if(!strcmp(ptemp,"0x0283"))//密码确认
        {
            if(id==10)
            {
                if(SuperPwd==0x55)
                {
                    ptemp = strtok(NULL,delim);
                    if(userFlag==0x11)
                    {
                        while(*(ptemp+i)!='\0')
                        {
                            password[i]=*(ptemp+i);
                            i++;
                        }
                        password[i]='\0';
						(void)write_eeprom(WEBSERVER_PASSWORD_ADDRESS, password, 8);
                    }
                    else if(userFlag==0x22)
                    {
                        while(*(ptemp+i)!='\0')
                        {
                            password2[i]=*(ptemp+i);
                            i++;
                        }
                        password2[i]='\0';
						(void)write_eeprom(WEBSERVER_PASSWORD2_ADDRESS, password2, 8);
                    }
                    login=0;
                }
                else
                {
                    if(flag==0)
                    {
                        ptemp = strtok(NULL,delim);
                        if(userFlag==0x11)
                        {
                            if(SuperPwd==0x55)
                            {
                                login=0;
                            }
                            else
                            {
                                login=strcmp(ptemp,(char*)password);//判断密码
                            }
                        }
                        else if(userFlag==0x22)
                        {
                            if(SuperPwd==0x55)
                            {
                                login=0;
                            }
                            else
                            {
                                login=strcmp(ptemp,(char*)password2);//判断密码
                            }
                        }
                        if(login==0)flag=1;
                    }
                    else if(flag==1)//密码修改
                    {
                        flag=0;
                        ptemp = strtok(NULL,delim);
                        if(userFlag==0x11)
                        {
                            while(*(ptemp+i)!='\0')
                            {
                                password[i]=*(ptemp+i);
                                i++;
                            }
                            password[i]='\0';
							(void)write_eeprom(WEBSERVER_PASSWORD_ADDRESS, password, 8);
                        }
                        else if(userFlag==0x22)
                        {
                            while(*(ptemp+i)!='\0')
                            {
                                password2[i]=*(ptemp+i);
                                i++;
                            }
                            password2[i]='\0';
							(void)write_eeprom(WEBSERVER_PASSWORD2_ADDRESS, password2, 8);
                        }
                        login=0;
                    }
                }
            }
            else
            {
                ptemp = strtok(NULL,delim);
                if(login1==0)//用户1登陆
                {
                    login_c1=strcmp(ptemp,(char*)password);//根据用户名判断密码是否正确
                    login_s1=strcmp(ptemp,(char*)super_password);//密码都不正确，判断是否为超级密码
                    if(login_c1==0)
                    {
                        SuperPwd=0;
                    }
                    if(login_s1==0)//是使用超级密码登录的
                    {
                        SuperPwd=0x55;
                    }
                }
                else if(login2==0)
                {    
                    login_c2=strcmp(ptemp,(char*)password2);//同上
                    login_s2=strcmp(ptemp,(char*)super_password);//密码都不正确，判断是否为超级密码
                    if(login_c2==0)
                    {
                        SuperPwd=0;
                    }
                    if(login_s2==0)//是使用超级密码登录的
                    {
                        SuperPwd=0x55;
                    }
                }
            }
        }
        else if(!strcmp(ptemp,"feed"))//喂狗登录，将登录标识置4
        {
            ptemp = strtok(NULL,delim);
            loginFlag=(uint32_t)atoi(ptemp);
        }
		else
		{
			id_No=(uint16_t)strtoul(ptemp,0,16);
			ptemp = strtok(NULL,delim);
			value=(uint16_t)atoi(ptemp);
			set_param(set_num++,id_No,value);
		}
		ptemp = strtok(NULL,delim);
    }

    switch(id)
    {
        case 0:
            if(login1==0)//用户1的用户名正确
            {
                if(login_c1==0 || login_s1==0)//密码也正确
                {
                    if(loginFlag==0)//且无人登录
                    {   
                        userFlag=0x11;//表示当前用户为"用户1"
						(void)write_eeprom(WEBSERVER_USERFLAG_ADDRESS, &userFlag, 1);
                        loginFlag=2;//标识为已登录
                        loginFlag2=2;//标识为已登录
                        sprintf(a_000,"HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n%d",1);
                    }
                    else//但已经有人登录
                    {
						if(loginFlag2!=0)
						{
							sprintf(a_000,"HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n%d",1);
						}
						else
						{
							sprintf(a_000,"HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n%d",2);
						}
                    }
                }
                else//用户名正确，密码错误
                {
                    sprintf(a_000,"HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n%d",0);
                }
            }
            else if(login2==0)//用户2的用户名正确
            {
                if(login_c2==0 || login_s2==0)//密码也正确
                {
                    if(loginFlag==0)//且无人登录
                    {
                        userFlag=0x22;//表示当前用户为"用户2"
						(void)write_eeprom(WEBSERVER_USERFLAG_ADDRESS, &userFlag, 1);
                        loginFlag=2;//标识为已登录
                        loginFlag2=2;//标识为已登录
                        sprintf(a_000,"HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n%d",1);
                    }
                    else//但已经有人登录
                    {
						if(loginFlag2!=0)
						{
							sprintf(a_000,"HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n%d",1);
						}
						else
						{
							sprintf(a_000,"HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n%d",2);
						}
                    }
                }
                else//用户名正确，密码错误
                {
                    sprintf(a_000,"HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n%d",0);
                }
            }
		break;
        case 1:
            sprintf(a_000,"HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n%d",0);
		break;
        case 4:
        case 5:
        case 6:
            sprintf(a_000,"HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n%d",1);
        break;
        case 10:
            sprintf(a_000,"HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n%d",login?0:1);
        break;
        case 12:
            sprintf(a_000,"HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n%d",1);
        break;
        default:
            sprintf(a_000,"HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n%d",0);
        break;
    }
    a_Rsc[0].size = strlen(a_000);
}
bool Leap_Year(int year)										//是否为闰年
{ 
	bool rtn = false;
    if ((year % 4 == 0 && year % 100 != 0 )|| year % 400 == 0) 
	{
        rtn = true;
	}
	else
	{
		rtn = false; 
	}
	return rtn;
} 
uint8_t How_Many_Day(int year,int month) 						//计算月发电量柱状图应该显示的天数
{
    uint8_t day=28;
    if(month==2)
    {
        if(Leap_Year(year)) day=29;
        else day=28;
    }
    else if(month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12)
        day=31;
    else if(month==4 || month==6 || month==9 || month==11)
        day=30;
    return day;
}
void Load_Power_Curve(uint16_t *data1,uint16_t *data2, uint8_t id)//装载充放电功率曲线
{
    uint8_t rcd_num = get_record_num(E_DAY_C_CURVE_LOG);
    uint8_t buf[EVERY_DAY_C_CURVE_BUF_LEN]= {0};
    if(data1 != NULL && data2 != NULL)
	{
		memset(data1, 0, VALID_CURVE_POINT_NUM + 3);
		memset(data2, 0, VALID_CURVE_POINT_NUM + 3);
		 
		if(id == 0)//今天的功率曲线存放在数组中
		{
			data1[0] = BcdToBin(showTime.tm_year)+2000;
			data1[1] = BcdToBin(showTime.tm_mon);
			data1[2] = BcdToBin(showTime.tm_day);
			memcpy(data1 + 3, Touch_Data.Touch_Charge_Power_Curve_Temp+2, VALID_CURVE_POINT_NUM * sizeof(uint16_t));
			memcpy(data2 + 3, Touch_Data.Touch_Discharge_Power_Curve_Temp+2, VALID_CURVE_POINT_NUM * sizeof(uint16_t));
		}
		else if(rcd_num != 0 && id <= rcd_num)
		{
			if(get_record_by_index(E_DAY_C_CURVE_LOG, rcd_num - id + 1, buf, EVERY_DAY_C_CURVE_BUF_LEN) == 1)
			{
				data1[0] = BcdToBin(buf[2])+2000;
				data1[1] = BcdToBin(buf[3]);
				data1[2] = BcdToBin(buf[4]);
				memcpy(data1 + 3, buf + 8, VALID_CURVE_POINT_NUM * sizeof(uint16_t));       
			}
			if(get_record_by_index(E_DAY_D_CURVE_LOG, rcd_num - id + 1, buf, EVERY_DAY_D_CURVE_BUF_LEN) == 1)
			{
				data2[0] = BcdToBin(buf[2])+2000;
				data2[1] = BcdToBin(buf[3]);
				data2[2] = BcdToBin(buf[4]);
				memcpy(data2 + 3, buf + 8, VALID_CURVE_POINT_NUM * sizeof(uint16_t));       
			}
		}       
    }
}
void Load_Day_Column(uint32_t *data, uint8_t id)				//装载日充放电量
{
    uint8_t rcd_num = get_record_num(E_DAY_C_ENERGY_LOG);
    uint8_t buf1[EVERY_DAY_C_ENERGY_BUF_LEN]= {0};
    uint8_t buf2[EVERY_DAY_D_ENERGY_BUF_LEN]= {0};
    if(data != NULL)
    {
		if(id == 0)//本月的日发电量存放在数组中    
		{
			data[0] = BcdToBin(showTime.tm_year)+2000;
			data[1] = BcdToBin(showTime.tm_mon);
			data[2] = BcdToBin(showTime.tm_day);
			for(uint8_t i = 0; i < VALID_HOUR_NUM; i++)
			{
				data[i*2 + 3] = Touch_Data.Touch_Day_Charge_Elec_Column[i];
				data[i*2 + 4] = Touch_Data.Touch_Day_Discharge_Elec_Column[i];
			}
		}
		else
		{
			memset(data, 0, (VALID_HOUR_NUM * 2 + 3) * sizeof(uint32_t));
			if(rcd_num != 0 && id <= rcd_num)
			{
				if((get_record_by_index(E_DAY_C_ENERGY_LOG, rcd_num - id + 1, buf1, EVERY_DAY_C_ENERGY_BUF_LEN) == 1) &&
				   (get_record_by_index(E_DAY_D_ENERGY_LOG, rcd_num - id + 1, buf2, EVERY_DAY_D_ENERGY_BUF_LEN) == 1))
				{
					data[0] = BcdToBin(buf1[2])+2000;
					data[1] = BcdToBin(buf1[3]);
					data[2] = BcdToBin(buf1[4]);
					for(uint8_t i = 0; i < VALID_HOUR_NUM; i++)
					{
						memcpy(data+ i*2    + 3, buf1 + 8 + i * sizeof(uint32_t),sizeof(uint32_t));
						memcpy(data+(i*2+1) + 3, buf2 + 8 + i * sizeof(uint32_t),sizeof(uint32_t));
					} 
				}
			}
		}
	}
}
void Load_Month_Column(uint32_t *data, uint8_t id)				//装载月充放电量
{
    uint8_t rcd_num = get_record_num(E_MON_C_ENERGY_LOG);
    uint8_t buf1[EVERY_MON_C_ENERGY_BUF_LEN]= {0};
    uint8_t buf2[EVERY_MON_D_ENERGY_BUF_LEN]= {0};
    if(data != NULL)
    {
		if(id == 0)//本月的日发电量存放在数组中    
		{
			data[0] = BcdToBin(showTime.tm_year)+2000;
			data[1] = BcdToBin(showTime.tm_mon);
			data[2] = BcdToBin(showTime.tm_day);
			for(uint8_t i = 0; i < VALID_DAY_NUM; i++)
			{
				data[i*2 + 3] = Touch_Data.Touch_Month_Charge_Elec_Column[i];
				data[i*2 + 4] = Touch_Data.Touch_Month_Discharge_Elec_Column[i];
			}
		}
		else
		{
			memset(data, 0, (VALID_DAY_NUM * 2 + 3) * sizeof(uint32_t));
			if(rcd_num != 0 && id <= rcd_num)
			{
				if((get_record_by_index(E_MON_C_ENERGY_LOG, rcd_num - id + 1, buf1, EVERY_MON_C_ENERGY_BUF_LEN) == 1) &&
				   (get_record_by_index(E_MON_D_ENERGY_LOG, rcd_num - id + 1, buf2, EVERY_MON_D_ENERGY_BUF_LEN) == 1))
				{
					data[0] = BcdToBin(buf1[2])+2000;
					data[1] = BcdToBin(buf1[3]);
					data[2] = BcdToBin(buf1[4]);
					for(uint8_t i = 0; i < VALID_DAY_NUM; i++)
					{
						memcpy(data+ i*2    + 3, buf1 + 8 + i * sizeof(uint32_t),sizeof(uint32_t));
						memcpy(data+(i*2+1) + 3, buf2 + 8 + i * sizeof(uint32_t),sizeof(uint32_t));
					} 
				}
			}
		}
	}
}
void Load_Year_Column(uint32_t *data, uint8_t id)				//装载年充放电量
{
    uint8_t rcd_num = get_record_num(E_YEAR_C_ENERGY_LOG);
    uint8_t buf1[EVERY_YEAR_C_ENERGY_BUF_LEN]= {0};
    uint8_t buf2[EVERY_YEAR_D_ENERGY_BUF_LEN]= {0};
    if(data != NULL)
    {
		if(id == 0)//本月的日发电量存放在数组中    
		{
			data[0] = BcdToBin(showTime.tm_year)+2000;
			data[1] = BcdToBin(showTime.tm_mon);
			data[2] = BcdToBin(showTime.tm_day);
			for(uint8_t i = 0; i < VALID_MONTH_NUM; i++)
			{
				data[i*2 + 3] = Touch_Data.Touch_Year_Charge_Elec_Column[i];
				data[i*2 + 4] = Touch_Data.Touch_Year_Discharge_Elec_Column[i];
			}
		}
		else
		{
			memset(data, 0, (VALID_MONTH_NUM * 2 + 3) * sizeof(uint32_t));
			if(rcd_num != 0 && id <= rcd_num)
			{
				if((get_record_by_index(E_YEAR_C_ENERGY_LOG, rcd_num - id + 1, buf1, EVERY_YEAR_C_ENERGY_BUF_LEN) == 1) &&
				   (get_record_by_index(E_YEAR_D_ENERGY_LOG, rcd_num - id + 1, buf2, EVERY_YEAR_D_ENERGY_BUF_LEN) == 1))
				{
					data[0] = BcdToBin(buf1[2])+2000;
					data[1] = BcdToBin(buf1[3]);
					data[2] = BcdToBin(buf1[4]);
					for(uint8_t i = 0; i < VALID_MONTH_NUM; i++)
					{
						memcpy(data+ i*2    + 3, buf1 + 8 + i * sizeof(uint32_t),sizeof(uint32_t));
						memcpy(data+(i*2+1) + 3, buf2 + 8 + i * sizeof(uint32_t),sizeof(uint32_t));
					} 
				}
			}
		}
	}
}
void Load_Total_Column(uint32_t *data, uint8_t id)				//装载总充放电量
{
    uint8_t rcd_num = get_record_num(E_TOTAL_C_ENERGY_LOG);
    uint8_t buf1[EVERY_TOTAL_C_ENERGY_BUF_LEN]= {0};
    uint8_t buf2[EVERY_TOTAL_D_ENERGY_BUF_LEN]= {0};
    if(data != NULL)
    {
		if(id == 0)//本月的日发电量存放在数组中    
		{
			data[0] = BcdToBin(showTime.tm_year)+2000;
			data[1] = BcdToBin(showTime.tm_mon);
			data[2] = BcdToBin(showTime.tm_day);
			for(uint8_t i = 0; i < VALID_YEAR_NUM; i++)
			{
				data[i*2 + 3] = Touch_Data.Touch_Total_Charge_Elec_Column[i];
				data[i*2 + 4] = Touch_Data.Touch_Total_Discharge_Elec_Column[i];
			}
		}
		else
		{
			memset(data, 0, VALID_YEAR_NUM * sizeof(uint32_t) + 3);
			if(rcd_num != 0 && id <= rcd_num)
			{
				if((get_record_by_index(E_TOTAL_C_ENERGY_LOG, rcd_num - id + 1, buf1, EVERY_TOTAL_C_ENERGY_BUF_LEN) == 1) &&
				   (get_record_by_index(E_TOTAL_D_ENERGY_LOG, rcd_num - id + 1, buf2, EVERY_TOTAL_D_ENERGY_BUF_LEN) == 1))
				{
					data[0] = BcdToBin(buf1[2])+2000;
					data[1] = BcdToBin(buf1[3]);
					data[2] = BcdToBin(buf1[4]);
					for(uint8_t i = 0; i < VALID_YEAR_NUM; i++)
					{
						memcpy(data+ i*2    + 3, buf1 + 8 + i * sizeof(uint32_t),sizeof(uint32_t));
						memcpy(data+(i*2+1) + 3, buf2 + 8 + i * sizeof(uint32_t),sizeof(uint32_t));
					} 
				}
			}
		}
	}
}
void jsCharts_data_to_show(uint8_t id, uint8_t Which_Record)	//查询不同的曲线和柱状图数据。
{ 
	uint8_t month_day = 0;
    memset(p_000,0,sizeof(p_000));
    sprintf(jsCharts_temp,"HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nAccept-Ranges:bytes\r\n\r\n");
    strcat(p_000,jsCharts_temp);
    switch(id)
    {
        case 1://充放电功率曲线图
			Load_Power_Curve(Daily_C_Curve,Daily_D_Curve,Which_Record);
			//充电
            sprintf(jsCharts_temp,"var data1_c=new period([");
            strcat(p_000,jsCharts_temp);
			sprintf(jsCharts_temp,"%d", Daily_C_Curve[3]/10);
			strcat(p_000,jsCharts_temp);
			for(uint8_t i=1;i <VALID_CURVE_POINT_NUM;i++)
			{
				sprintf(jsCharts_temp,",%d", Daily_C_Curve[i+3]/10);
				strcat(p_000,jsCharts_temp);
			}
            sprintf(jsCharts_temp,"],[]);");
            strcat(p_000,jsCharts_temp);
			//放电
            sprintf(jsCharts_temp,"var data1_d=new period([");
            strcat(p_000,jsCharts_temp);
			sprintf(jsCharts_temp,"%d", (1200-Daily_D_Curve[3])/10);
			strcat(p_000,jsCharts_temp);
			for(uint8_t i=1;i <VALID_CURVE_POINT_NUM;i++)
			{
				sprintf(jsCharts_temp,",%d", (1200-Daily_D_Curve[i+3])/10);
				strcat(p_000,jsCharts_temp);
			}
            sprintf(jsCharts_temp,"],[]);query_year=%d;query_month=%d;query_day=%d;curve_num=%d;",
				Daily_C_Curve[0],Daily_C_Curve[1],Daily_C_Curve[2],get_record_num(E_DAY_C_CURVE_LOG) + 1);
            strcat(p_000,jsCharts_temp);
            p_Rsc[0].size = strlen(p_000);
        break;
        case 2://日充放电量图
			Load_Day_Column(Daily_Column,Which_Record);
            sprintf(jsCharts_temp,"var data2=new Array(['00',%d]",Daily_Column[3]/360000);
            strcat(p_000,jsCharts_temp);       
			sprintf(jsCharts_temp,",['',%d]",Daily_Column[4]/360000);
			strcat(p_000,jsCharts_temp);
            for(uint8_t i=1;i<24;i++)
            {
				sprintf(jsCharts_temp,",['%02d',%d]",i,Daily_Column[i*2+3]/360000);
				strcat(p_000,jsCharts_temp);
				sprintf(jsCharts_temp,",['',%d]",Daily_Column[i*2+4]/360000);
				strcat(p_000,jsCharts_temp);
            }
            sprintf(jsCharts_temp,",['%02d',%d]);query_year=%d;query_month=%d;query_day=%d;day_num=%d;",
			VALID_HOUR_NUM*2,
			return_max_u32(Daily_Column,3,VALID_HOUR_NUM*2+3)>360000?((((return_max_u32(Daily_Column,3,VALID_HOUR_NUM*2+3)/360000)-1)/100)+1)*100:100,
			Daily_Column[0],Daily_Column[1],Daily_Column[2],get_record_num(E_DAY_C_ENERGY_LOG) + 1);
            strcat(p_000,jsCharts_temp);
            p_Rsc[0].size = strlen(p_000);
        break;
        case 3://月充放电量图
			Load_Month_Column(Monthly_Column,Which_Record);
			month_day=How_Many_Day(Monthly_Column[0],Monthly_Column[1]);
            sprintf(jsCharts_temp,"var data3=new Array(['01',%d]",Monthly_Column[3]);
            strcat(p_000,jsCharts_temp);       
			sprintf(jsCharts_temp,",['',%d]",Monthly_Column[4]);
			strcat(p_000,jsCharts_temp);
            for(uint8_t i=1;i<month_day;i++)
            {
				sprintf(jsCharts_temp,",['%02d',%d]",i+1,Monthly_Column[i*2+3]);
				strcat(p_000,jsCharts_temp);
				sprintf(jsCharts_temp,",['',%d]",Monthly_Column[i*2+4]);
				strcat(p_000,jsCharts_temp);
            }
            sprintf(jsCharts_temp,",['%02d',%d]);query_year=%d;query_month=%d;query_day=%d;month_num=%d;",
			month_day*2,return_max_u32(Monthly_Column,3,month_day*2),
			Monthly_Column[0],Monthly_Column[1],Monthly_Column[2],get_record_num(E_MON_C_ENERGY_LOG) + 1);
            strcat(p_000,jsCharts_temp);
            p_Rsc[0].size = strlen(p_000);
        break;
        case 4://年充放电量图
			Load_Year_Column(Yearly_Column,Which_Record);
            sprintf(jsCharts_temp,"var data4=new Array(['01',%d]",Yearly_Column[3]);
            strcat(p_000,jsCharts_temp);       
			sprintf(jsCharts_temp,",['',%d]",Yearly_Column[4]);
			strcat(p_000,jsCharts_temp);
            for(uint8_t i=1;i<12;i++)
            {
				sprintf(jsCharts_temp,",['%02d',%d]",i+1,Yearly_Column[i*2+3]);
				strcat(p_000,jsCharts_temp);
				sprintf(jsCharts_temp,",['',%d]",Yearly_Column[i*2+4]);
				strcat(p_000,jsCharts_temp);
            }
            sprintf(jsCharts_temp,",['%02d',%d]);query_year=%d;query_month=%d;query_day=%d;year_num=%d;",
			VALID_MONTH_NUM*2,return_max_u32(Yearly_Column,3,VALID_MONTH_NUM*2+3),
			Yearly_Column[0],Yearly_Column[1],Yearly_Column[2],get_record_num(E_YEAR_C_ENERGY_LOG) + 1);
            strcat(p_000,jsCharts_temp);
            p_Rsc[0].size = strlen(p_000);
        break;
        case 5://总充放电量图
 			Load_Total_Column(Total_Column,Which_Record);
            sprintf(jsCharts_temp,"var data5=new Array(['%d',%d]",2000+(Total_Column[0]-2000)/15*15,Total_Column[3]);
            strcat(p_000,jsCharts_temp);       
			sprintf(jsCharts_temp,",['',%d]",Total_Column[4]);
			strcat(p_000,jsCharts_temp);
            for(uint8_t i=1;i<VALID_YEAR_NUM;i++)
            {
				sprintf(jsCharts_temp,",['%02d',%d]",i+2000+(Total_Column[0]-2000)/15*15,Total_Column[i*2+3]);
				strcat(p_000,jsCharts_temp);
				sprintf(jsCharts_temp,",['',%d]",Total_Column[i*2+4]);
				strcat(p_000,jsCharts_temp);
            }
            sprintf(jsCharts_temp,",['%02d',%d]);query_year=%d;query_month=%d;query_day=%d;total_num=%d;",
			VALID_YEAR_NUM*2,return_max_u32(Total_Column,3,VALID_YEAR_NUM*2+3),
			Total_Column[0],Total_Column[1],Total_Column[2],get_record_num(E_TOTAL_C_ENERGY_LOG) + 1);
            strcat(p_000,jsCharts_temp);
            p_Rsc[0].size = strlen(p_000);
        break;
        default:
            p_000[0]=0;
            p_Rsc[0].size = 1;
        break;
    }
}

void p_handler(int id,char* data)								//处理曲线图和柱状图的参数请求
{
    char *ptemp;
    char delim[] = "?=&+";
    ptemp = strtok(data,delim);
    ptemp = strtok(NULL,delim);
    ptemp = strtok(NULL,delim);//查询的是哪一条记录
    jsCharts_data_to_show(id,(uint8_t)atoi(ptemp));
}
void r_handler(int id,char* data)								//导出历史记录（运行记录、故障记录）
{
    uint8_t run_buf[EVERY_RUNRECORD_BUF_LEN] = {0};
	int32_t records_num=0;//运行记录的条数
    LogTime time;
    char *ptemp;
    char delim[] = "?=&+";
    uint8_t page = 0;
	uint8_t array[16];
    ptemp = strtok(data,delim);
    ptemp = strtok(NULL,delim);
    ptemp = strtok(NULL,delim);
    time.Year = BinToBcd((uint8_t)(atoi(ptemp)-2000));
    ptemp = strtok(NULL,delim);
    ptemp = strtok(NULL,delim);
    time.Month = BinToBcd((uint8_t)atoi(ptemp));
    ptemp = strtok(NULL,delim);
    ptemp = strtok(NULL,delim);
    time.Date = BinToBcd((uint8_t)atoi(ptemp));
    ptemp = strtok(NULL,delim);
    ptemp = strtok(NULL,delim);
    page = atoi(ptemp);
    memset(jsCharts_temp,0,sizeof(jsCharts_temp));
    memset(p_000,0,sizeof(p_000));
    sprintf(p_000,"HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\n\r\n");
    
    switch(id)
    {
        case 7://运行记录
			records_num = start_query(E_RUNRECORD_LOG, &time, 1, run_buf, EVERY_RUNRECORD_BUF_LEN);
			if(records_num<=0)records_num=0;
            sprintf(jsCharts_temp,"{'R':[");
            strcat(p_000,jsCharts_temp);
            for(int i = (page-1)*10+1;page >= 1 && i<=(((page * 10) > records_num) ? records_num : (page * 10));i++)
            {
				query_rcd(i, run_buf, EVERY_RUNRECORD_BUF_LEN);
                sprintf(jsCharts_temp,"{");
                strcat(p_000,jsCharts_temp);
                //读失败就放前一条
                sprintf(jsCharts_temp,
                "'C1':%d,"
                "'C2':'%4d.%02d.%02d/%02d:%02d:00',"
                "'C3':%d,"					//运行状态
                "'C4':'%.1fV',"				//直流电压
                "'C5':'%.1fA',"				//直流电流
                "'C6':'%.1fkW',"			//直流功率
                "'C7':'%.1fV',"				//A相电压
                "'C8':'%.1fV',"				//B相电压
                "'C9':'%.1fV',"				//C相电压
                "'C10':'%.1fA',"			//A相电流
                "'C11':'%.1fA',"			//B相电流
                "'C12':'%.1fA',"			//C相电流
                "'C13':'%.1fkVA',"			//视在功率
                "'C14':'%.1fkW',"			//有功功率
                "'C15':'%.1fkVar',"			//无功功率
                "'C16':'%.2fHz',"			//电网频率
                "'C17':'%.3f',"				//功率因数
                "'C18':'%.1f&#176;C',"		//机内温度
                "'C19':'%.1f%%',"			//工作效率
                "'C20':'%.1fkWh',"			//日充电电量
                "'C21':'%.1fkWh',"			//日放电电量
                "'C22':'%dkWh',"			//总充电电量
                "'C23':'%dkWh',"			//总放电电量
                "'C24':'%dh'",				//总运行时间
                i,
                BcdToBin(run_buf[2])+2000,BcdToBin(run_buf[3]),
				BcdToBin(run_buf[4]),BcdToBin(run_buf[5]),BcdToBin(run_buf[6]),
				GetU16(run_buf+8),			//运行状态		2B
				GetU16(run_buf+10)/10.0,	//直流电压		2B
				GetU16(run_buf+12)/10.0,	//直流电流    	2B
				GetU32(run_buf+14)/1000.0,	//直流功率    	4B
				GetU32(run_buf+18)/10.0,	//A相电压    	4B
				GetU32(run_buf+22)/10.0,	//B相电压    	4B
				GetU32(run_buf+26)/10.0,	//C相电压    	4B
				GetU32(run_buf+30)/10.0,	//A相电流    	4B
				GetU32(run_buf+34)/10.0,	//B相电流    	4B
				GetU32(run_buf+38)/10.0,	//C相电流    	4B
				GetU32(run_buf+42)/1000.0,	//视在功率    	4B
				GetU32(run_buf+46)/1000.0,	//有功功率    	4B
				(int32_t)GetU32(run_buf+50)/1000.0,	//无功功率    	4B
				GetU16(run_buf+54)/100.0,	//电网频率    	2B
				(int32_t)GetU32(run_buf+56)/1000.0,	//功率因数    	4B
				(int16_t)GetU16(run_buf+60)/10.0,	//机内温度    	2B
				GetU16(run_buf+62)/10.0,	//工作效率    	2B
				GetU16(run_buf+64)/10.0,	//总充电电量    2B
				GetU16(run_buf+66)/10.0,	//总放电电量    2B
				GetU32(run_buf+68),			//总充电电量    4B
				GetU32(run_buf+72),			//总放电电量    4B
				GetU32(run_buf+76)			//总运行时间    4B
				);
                strcat(p_000,jsCharts_temp);
                sprintf(jsCharts_temp,"},");
                strcat(p_000,jsCharts_temp);
            }
            sprintf(jsCharts_temp,"{'end':0}]};Records=%d;",records_num);
            strcat(p_000,jsCharts_temp);
            p_Rsc[0].size = strlen(p_000);
            end_query();
            break;
        case 8://事件记录
            sprintf(jsCharts_temp,"{'r':[");
            strcat(p_000,jsCharts_temp);
     
            for(int i=(page-1)*10+1; page >= 1 && i<=(((page*10)>read_event_record.total_nub)?read_event_record.total_nub:(page*10));i++)
            {
                Read_OnConditionNB_Record(read_event_record.ptr, read_event_record.total_nub,i,HISEVENTRECORD_START_ADDRESS,array) ;
                sprintf(jsCharts_temp,"{");
                strcat(p_000,jsCharts_temp);
                sprintf(jsCharts_temp,
                "'C1':%d,"
                "'C2':'%4d/%02d/%02d&nbsp;&nbsp;%02d:%02d:%02d',"
                "'C3':'%d'",
                i,
				BcdToBin(array[0])+2000,
				BcdToBin(array[1]),
				BcdToBin(array[2]),
				BcdToBin(array[3]),
				BcdToBin(array[4]),
				BcdToBin(array[5]),
				GetU16(array+6));
                strcat(p_000,jsCharts_temp);
                sprintf(jsCharts_temp,"},");
                strcat(p_000,jsCharts_temp);
            }
            sprintf(jsCharts_temp,"{'end':0}]};Records=%d;",read_event_record.total_nub);
            strcat(p_000,jsCharts_temp);
            p_Rsc[0].size = strlen(p_000);
        break;
        case 9://故障记录
            sprintf(jsCharts_temp,"{'r':[");
            strcat(p_000,jsCharts_temp);
     
            for(int i=(page-1)*10+1; page >= 1 && i<=(((page*10)>read_fault_record.total_nub)?read_fault_record.total_nub:(page*10));i++)
            {
				Read_OnConditionNB_Record(read_fault_record.ptr, read_fault_record.total_nub,i,HISFAULTRECORD_START_ADDRESS,array) ;
                sprintf(jsCharts_temp,"{");
                strcat(p_000,jsCharts_temp);
                sprintf(jsCharts_temp,
                "'C1':%d,"
                "'C2':'%4d/%02d/%02d&nbsp;&nbsp;%02d:%02d:%02d',"
                "'C3':'%d'",
                i,
				BcdToBin(array[0])+2000,
				BcdToBin(array[1]),
				BcdToBin(array[2]),
				BcdToBin(array[3]),
				BcdToBin(array[4]),
				BcdToBin(array[5]),
				array[6]);
                strcat(p_000,jsCharts_temp);
                sprintf(jsCharts_temp,"},");
                strcat(p_000,jsCharts_temp);
            }
            sprintf(jsCharts_temp,"{'end':0}]};Records=%d;",read_fault_record.total_nub);
            strcat(p_000,jsCharts_temp);
            p_Rsc[0].size = strlen(p_000);
        break;
        default:                                                                           
            p_000[0]=0;
            p_Rsc[0].size = 1;
        break;
    }
}
void his_handler(uint8_t type,char* data)						//导出历史记录
{
    uint8_t run_buf[EVERY_RUNRECORD_BUF_LEN] = {0};
	int32_t records_num=0;//运行记录的条数
	const uint8_t minute[3]={0,20,40};
	uint8_t array[16]={0};
	uint8_t month_day = 0;
	static char stemp[5];
	memset(stemp,0,sizeof(stemp));
	memcpy(stemp,data+21,4);
    R_time.Year = BinToBcd((uint8_t)(atoi(stemp)-2000));
	memset(stemp,0,sizeof(stemp));
	memcpy(stemp,data+26,2);
    R_time.Month = BinToBcd((uint8_t)atoi(stemp));
	memset(stemp,0,sizeof(stemp));
	memcpy(stemp,data+29,2);
    R_time.Date = BinToBcd((uint8_t)atoi(stemp));

	uint8_t temp=0;
	if(Webserver_Language!=0x55)
	{
		temp=1;
	}
    memset(p_000,0,sizeof(p_000));
    sprintf(p_000,"HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\n\r\n");
    switch(type)
    {
        case 1://充放电功率
			sprintf(jsCharts_temp,"%s,%s,%s,%s\r\n",Export_Name[0][temp],Export_Name[1][temp],Export_Name[2][temp],Export_Name[3][temp]);
			strcat(p_000,jsCharts_temp);
            for(uint8_t i=0;i < VALID_CURVE_POINT_NUM;i++)
            {				
                sprintf(jsCharts_temp,"%02d,%02d:%02d,%dkW,%dkW\r\n",i+1,i/3,minute[i%3],Daily_C_Curve[i+3]/2,Daily_D_Curve[i+3]/2);
                strcat(p_000,jsCharts_temp);
            }
            p_Rsc[0].size =strlen(p_000);
		break;
        case 2://日充放电量
			sprintf(jsCharts_temp,"%s,%s,%s,%s\r\n",Export_Name[0][temp],Export_Name[29][temp],Export_Name[4][temp],Export_Name[5][temp]);
			strcat(p_000,jsCharts_temp);
            for(uint8_t i=0;i < VALID_HOUR_NUM;i++)
            {				
                sprintf(jsCharts_temp,"%02d,%02d:00~%02d:00,%.1fkWh,%.1fkWh\r\n",i+1,i,(i+1)%24,Daily_Column[i*2+3]/3600000.0,Daily_Column[i*2+4]/3600000.0);
                strcat(p_000,jsCharts_temp);
            }
            p_Rsc[0].size =strlen(p_000);
		break;
        case 3://月充放电量
			month_day=How_Many_Day(Monthly_Column[0],Monthly_Column[1]);
			sprintf(jsCharts_temp,"%s,%s,%s,%s\r\n",Export_Name[0][temp],Export_Name[1][temp],Export_Name[4][temp],Export_Name[5][temp]);
			strcat(p_000,jsCharts_temp);
            for(uint8_t i=0;i < month_day;i++)
            {				
                sprintf(jsCharts_temp,"%02d,%02d,%.1fkWh,%.1fkWh\r\n",i+1,i+1,Monthly_Column[i*2+3]/10.0,Monthly_Column[i*2+4]/10.0);
                strcat(p_000,jsCharts_temp);
            }
            p_Rsc[0].size =strlen(p_000);
		break;
        case 4://年充放电量
			sprintf(jsCharts_temp,"%s,%s,%s,%s\r\n",Export_Name[0][temp],Export_Name[1][temp],Export_Name[4][temp],Export_Name[5][temp]);
			strcat(p_000,jsCharts_temp);
            for(uint8_t i=0;i < VALID_MONTH_NUM;i++)
            {				
                sprintf(jsCharts_temp,"%02d,%02d,%.1fkWh,%.1fkWh\r\n",i+1,i+1,Yearly_Column[i*2+3]/10.0,Yearly_Column[i*2+4]/10.0);
                strcat(p_000,jsCharts_temp);
            }
            p_Rsc[0].size =strlen(p_000);
		break;
        case 5://总充放电量
			sprintf(jsCharts_temp,"%s,%s,%s,%s\r\n",Export_Name[0][temp],Export_Name[1][temp],Export_Name[4][temp],Export_Name[5][temp]);
			strcat(p_000,jsCharts_temp);
            for(uint8_t i=0;i < VALID_YEAR_NUM;i++)
            {				
                sprintf(jsCharts_temp,"%02d,%02d,%.1fkWh,%.1fkWh\r\n",i+1,i+2000+(Total_Column[0]-2000)/15*15,Total_Column[i*2+3]/10.0,Total_Column[i*2+4]/10.0);
                strcat(p_000,jsCharts_temp);
            }
            p_Rsc[0].size =strlen(p_000);
		break;
        case 6://运行信息
            sprintf(jsCharts_temp,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\r\n",
					Export_Name[0][temp],Export_Name[6][temp],Export_Name[7][temp],Export_Name[8][temp],Export_Name[9][temp],
					Export_Name[10][temp],Export_Name[11][temp],Export_Name[12][temp],Export_Name[13][temp],Export_Name[14][temp],
					Export_Name[15][temp],Export_Name[16][temp],Export_Name[17][temp],Export_Name[18][temp],Export_Name[19][temp],
					Export_Name[20][temp],Export_Name[21][temp],Export_Name[22][temp],Export_Name[23][temp],Export_Name[24][temp],
					Export_Name[25][temp],Export_Name[26][temp],Export_Name[27][temp],Export_Name[28][temp]);
            strcat(p_000,jsCharts_temp);
			
			records_num = start_query(E_RUNRECORD_LOG, &R_time, 1, run_buf, EVERY_RUNRECORD_BUF_LEN);
			if((records_num > 0) && (records_num < RUNRECORD_DAY_MAX_NUM))
			{
				for(uint8_t i=1;i<=records_num;i++)
				{
					query_rcd(i, run_buf, EVERY_RUNRECORD_BUF_LEN);
					sprintf(jsCharts_temp,
						"%d,"
						"%4d.%02d.%02d/%02d:%02d:00,"
						"%s,"						//运行状态
						"%.1fV,"					//直流电压
						"%.1fA,"					//直流电流
						"%.1fkW,"					//直流功率
						"%.1fV,"					//A相电压
						"%.1fV,"					//B相电压
						"%.1fV,"					//C相电压
						"%.1fA,"					//A相电流
						"%.1fA,"					//B相电流
						"%.1fA,"					//C相电流
						"%.1fkW,"					//视在功率
						"%.1fkW,"					//有功功率
						"%.1fkVar,"					//无功功率
						"%.2fHz,"					//电网频率
						"%.3f,"						//功率因数
						"%.1f°C,"				    //机内温度
						"%.1f%%,"					//工作效率
						"%.1fkWh,"					//日充电电量
						"%.1fkWh,"					//日放电电量
						"%dkWh,"					//总充电电量
						"%dkWh,"					//总放电电量
						"%dh\r\n",					//总运行时间
						i,
						BcdToBin(run_buf[2])+2000,BcdToBin(run_buf[3]),BcdToBin(run_buf[4]),BcdToBin(run_buf[5]),BcdToBin(run_buf[6]),
						Event_Name[GetU16(run_buf+8)][temp],//运行状态		2B
						GetU16(run_buf+10)/10.0,	//直流电压		2B
						GetU16(run_buf+12)/10.0,	//直流电流    	2B
						GetU32(run_buf+14)/1000.0,	//直流功率    	4B
						GetU32(run_buf+18)/10.0,	//A相电压    	4B
						GetU32(run_buf+22)/10.0,	//B相电压    	4B
						GetU32(run_buf+26)/10.0,	//C相电压    	4B
						GetU32(run_buf+30)/10.0,	//A相电流    	4B
						GetU32(run_buf+34)/10.0,	//B相电流    	4B
						GetU32(run_buf+38)/10.0,	//C相电流    	4B
						GetU32(run_buf+42)/1000.0,	//视在功率    	4B
						GetU32(run_buf+46)/1000.0,	//有功功率    	4B
						GetU32(run_buf+50)/1000.0,	//无功功率    	4B
						GetU16(run_buf+54)/100.0,	//电网频率    	2B
						GetU32(run_buf+56)/1000.0,	//功率因数    	4B
						GetU16(run_buf+60)/10.0,	//机内温度    	2B
						GetU16(run_buf+62)/10.0,	//工作效率    	2B
						GetU16(run_buf+64)/10.0,	//日充电电量    2B
						GetU16(run_buf+66)/10.0,	//日放电电量    2B
						GetU32(run_buf+68),			//总充电电量    4B
						GetU32(run_buf+72),			//总放电电量    4B
						GetU32(run_buf+76)			//总运行时间    4B
					);
					strcat(p_000,jsCharts_temp);
				}
			}
            end_query();
            p_Rsc[0].size =strlen(p_000);
		break;
        case 7://事件信息
			sprintf(jsCharts_temp,"%s,%s,%s\r\n",Event_Name[18][temp],Event_Name[19][temp],Event_Name[20][temp]);
			strcat(p_000,jsCharts_temp);
            for(int i=1;i <= read_event_record.total_nub;i++)
            {
				Read_OnConditionNB_Record(read_event_record.ptr, read_event_record.total_nub,i,HISEVENTRECORD_START_ADDRESS,array);
				
                sprintf(jsCharts_temp,"%02d,%04d.%02d.%02d/%02d:%02d:%02d,%s\r\n",
				i,
				BcdToBin(array[0])+2000,
				BcdToBin(array[1]),
				BcdToBin(array[2]),
				BcdToBin(array[3]),
				BcdToBin(array[4]),
				BcdToBin(array[5]),
				Event_Name[(uint8_t)GetU16(array+6)][temp]);
                strcat(p_000,jsCharts_temp);
            }
            p_Rsc[0].size =strlen(p_000);
        break;
        case 8://故障信息
			sprintf(jsCharts_temp,"%s,%s,%s\r\n",Fault_Name[65][temp],Fault_Name[66][temp],Fault_Name[67][temp]);
			strcat(p_000,jsCharts_temp);
            for(int i=1;i <= read_fault_record.total_nub;i++)
            {
				Read_OnConditionNB_Record(read_fault_record.ptr, read_fault_record.total_nub,i,HISFAULTRECORD_START_ADDRESS,array);
				
                sprintf(jsCharts_temp,"%02d,%04d.%02d.%02d/%02d:%02d:%02d,%s\r\n",
				i,
				BcdToBin(array[0])+2000,
				BcdToBin(array[1]),
				BcdToBin(array[2]),
				BcdToBin(array[3]),
				BcdToBin(array[4]),
				BcdToBin(array[5]),
				Fault_Name[(uint8_t)array[6]][temp]);
                strcat(p_000,jsCharts_temp);
            }
            p_Rsc[0].size =strlen(p_000);
        break;
        default:            
            p_000[0]=0;
            p_Rsc[0].size = 1;
        break;
    }
}
