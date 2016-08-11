#include "includes.h"

void net_start_server(void)
{
    pcb_modbus_tcp = tcp_new();
    pcb_modbus_udp = udp_new();
    pcb_iec104 	   = tcp_new();
    pcb_webserver  = tcp_new();

    if (pcb_modbus_tcp != NULL)//Modbus协议
    {
		for(uint8_t i=0;i<TCP_MODBUS_LINK_NUM;i++)
		{
			g_modbus_ss_tcp[i]  = NULL;
		}
        err_t err = tcp_bind(pcb_modbus_tcp, IP_ADDR_ANY, Inverter_Info.Com_Info.Modbus_Port);//绑定端口号
        if (err == ERR_USE)
        {
            (void)tcp_close(pcb_modbus_tcp);
            pcb_modbus_tcp = NULL;
        }
        else
        {
            pcb_modbus_tcp=tcp_listen(pcb_modbus_tcp);
            tcp_accept(pcb_modbus_tcp, tcp_modbus_server_accept);
        }
    }
    if (pcb_modbus_udp != NULL)//Modbus协议
    {
        err_t err = udp_bind(pcb_modbus_udp, IP_ADDR_ANY, Inverter_Info.Com_Info.Modbus_Port);//绑定端口号
        if (err == ERR_USE)
        {
            (void)udp_remove(pcb_modbus_udp);
            pcb_modbus_udp = NULL;
        }
        else
        {
            udp_recv(pcb_modbus_udp, udp_modbus_server_recv, NULL);
        }
    }
    if (pcb_iec104     != NULL)//IEC104协议
    {
		for(uint8_t i=0;i<TCP_IEC104_LINK_NUM;i++)
		{
			g_iec104_ss[i]  = NULL;
		}
        err_t err = tcp_bind(pcb_iec104, IP_ADDR_ANY, Inverter_Info.Com_Info.IEC104_Port);//绑定端口号
        if (err == ERR_USE)
        {
            (void)tcp_close(pcb_iec104);
            pcb_iec104 = NULL;
        }
        else
        {
            pcb_iec104=tcp_listen(pcb_iec104);
            tcp_accept(pcb_iec104, IEC104_linkAccept);
        }
    }
    if (pcb_webserver  != NULL)//Webserver协议
    {
        g_webserver_hs = NULL;
        err_t err = tcp_bind(pcb_webserver, IP_ADDR_ANY, Inverter_Info.Com_Info.Webserver_Port);//绑定端口号
        if (err == ERR_USE)
        {
            (void)tcp_close(pcb_webserver);
            pcb_webserver = NULL;
        }
        else
        {
            pcb_webserver=tcp_listen(pcb_webserver);
            tcp_accept(pcb_webserver, tcp_webserver_server_accept);
        }
    }
    embf_delay(2000);
}
void task_net(void *p_arg)
{
	static const uint8_t temp = (REMOTE_MESURE_END_ADDR - REMOTE_MESURE_BEGIN_ADDR) / MAX_MESURE_PER_PKG + 1;//遥测上传的次数
//-------------------------------------------
    uint32_t l_currti;
    uint32_t l_currti1s;
    uint32_t l_currti2s;
    uint32_t l_currti_temp;
    (void)drv_ioctl(gt_glp.fnet, NET_SET_INIT_ALL, &Inverter_Info.Com_Info.Net);
    
    Net_Already_Init=true;//ccg 2013.5.17
    
    net_start_server();
    
    Com_Data.Start_DHCP = true;
    
    l_currti=embf_get_tick();
    l_currti1s=embf_get_tick();
    l_currti2s=embf_get_tick();
    while(true)
    {
		l_currti_temp = embf_get_tick();
        while(ETH_CheckFrameReceived() != 0u)
        {
            LwIP_Pkt_Handle(0u);
        }
        if ((uint32_t)fabs(l_currti_temp-l_currti)>190u)//200ms的误差是10ms
        {
			l_currti=l_currti_temp;
            LwIP_Periodic_Handle(l_currti_temp);
        }
        if ((uint32_t)fabs(l_currti_temp-l_currti1s)>951u)//1000ms的误差是49ms
        {
			l_currti1s=l_currti_temp;
			IEC104_APDU_TimeOutProc();
        }
        if ((uint32_t)fabs(l_currti_temp-l_currti2s)>2000u)
        {
			l_currti2s=l_currti_temp;
            webserver_2s_time_callback();
		//--------------------------------------------
			for(uint8_t i=0;i<TCP_IEC104_LINK_NUM;i++)
			{
				if(g_iec104_ss[i]!=NULL&&Start_Call_All[i] == true)//104总招之后，才主动上传
				{
					for(uint8_t j =0;j<temp;j++)
					{
						IEC104_UploadRemoteMesure(g_iec104_ss[i]);
					}
				}
			}
        }
//-----------------------------------------------------
        if((netline_connected() == true) && (Net_Already_Init == true))//修改网络参数 ccg 2013.6.3
        {
            if(Com_Data.Touch_Set_Net_Flag == true)
            {
                Com_Data.Touch_Set_Net_Flag = false;
                if(Inverter_Info.Com_Info.Net.dhcp_flag == D_DHCP_ON)
                {
                    if(Com_Data.DHCP_State_Last == D_DHCP_OFF)
                    {
                        net_ioctl(NET_SET_DHCP_ON, NULL);
                        memset(Inverter_Info.Com_Info.Net.ip, 0, 4);
                    }
						  Com_Data.Start_DHCP = true;
                }    
                else if(Inverter_Info.Com_Info.Net.dhcp_flag == D_DHCP_OFF)
                {
                    if(Com_Data.DHCP_State_Last == D_DHCP_ON)
                    {
                        net_ioctl(NET_SET_DHCP_OFF, NULL);
                    }
// 					if(Inverter_Info.Com_Info.Net.ip[0]     !=Inverter_Info.Com_Info.Net_temp.ip[0]     ||
// 					   Inverter_Info.Com_Info.Net.ip[1]     !=Inverter_Info.Com_Info.Net_temp.ip[1]     ||
// 					   Inverter_Info.Com_Info.Net.ip[2]     !=Inverter_Info.Com_Info.Net_temp.ip[2]     ||
// 					   Inverter_Info.Com_Info.Net.ip[3]     !=Inverter_Info.Com_Info.Net_temp.ip[3]     ||
// 					   Inverter_Info.Com_Info.Net.submask[0]!=Inverter_Info.Com_Info.Net_temp.submask[0]||
// 					   Inverter_Info.Com_Info.Net.submask[1]!=Inverter_Info.Com_Info.Net_temp.submask[1]||
// 					   Inverter_Info.Com_Info.Net.submask[2]!=Inverter_Info.Com_Info.Net_temp.submask[2]||
// 					   Inverter_Info.Com_Info.Net.submask[3]!=Inverter_Info.Com_Info.Net_temp.submask[3]||
// 					   Inverter_Info.Com_Info.Net.gateway[0]!=Inverter_Info.Com_Info.Net_temp.gateway[0]||
// 					   Inverter_Info.Com_Info.Net.gateway[1]!=Inverter_Info.Com_Info.Net_temp.gateway[1]||
// 					   Inverter_Info.Com_Info.Net.gateway[2]!=Inverter_Info.Com_Info.Net_temp.gateway[2]||
// 					   Inverter_Info.Com_Info.Net.gateway[3]!=Inverter_Info.Com_Info.Net_temp.gateway[3])
// 					{
// 						Inverter_Info.Com_Info.Net.ip[0]=Inverter_Info.Com_Info.Net_temp.ip[0];
// 						Inverter_Info.Com_Info.Net.ip[1]=Inverter_Info.Com_Info.Net_temp.ip[1];
// 						Inverter_Info.Com_Info.Net.ip[2]=Inverter_Info.Com_Info.Net_temp.ip[2];
// 						Inverter_Info.Com_Info.Net.ip[3]=Inverter_Info.Com_Info.Net_temp.ip[3];
// 						Inverter_Info.Com_Info.Net.submask[0]=Inverter_Info.Com_Info.Net_temp.submask[0];
// 						Inverter_Info.Com_Info.Net.submask[1]=Inverter_Info.Com_Info.Net_temp.submask[1];
// 						Inverter_Info.Com_Info.Net.submask[2]=Inverter_Info.Com_Info.Net_temp.submask[2];
// 						Inverter_Info.Com_Info.Net.submask[3]=Inverter_Info.Com_Info.Net_temp.submask[3];
// 						Inverter_Info.Com_Info.Net.gateway[0]=Inverter_Info.Com_Info.Net_temp.gateway[0];
// 						Inverter_Info.Com_Info.Net.gateway[1]=Inverter_Info.Com_Info.Net_temp.gateway[1];
// 						Inverter_Info.Com_Info.Net.gateway[2]=Inverter_Info.Com_Info.Net_temp.gateway[2];
// 						Inverter_Info.Com_Info.Net.gateway[3]=Inverter_Info.Com_Info.Net_temp.gateway[3];
// 						net_ioctl(NET_SET_IP,Inverter_Info.Com_Info.Net.ip);//ccg 2013.5.17
// 						net_ioctl(NET_SET_SUBMASK,Inverter_Info.Com_Info.Net.submask);
// 						net_ioctl(NET_SET_GATEWAY,Inverter_Info.Com_Info.Net.gateway);
// 						(void)write_eeprom(NET_IP_0_ADDRESS,Inverter_Info.Com_Info.Net.ip,4);
// 						(void)write_eeprom(NET_SUBMASK_0_ADDRESS,Inverter_Info.Com_Info.Net.submask,4);
// 						(void)write_eeprom(NET_GATEWAY_0_ADDRESS,Inverter_Info.Com_Info.Net.gateway,4);
// 					}
                }
            }
			if(Com_Data.Touch_Set_Net_Port_Flag==true)
			{
				Com_Data.Touch_Set_Net_Port_Flag=false;
				tcp_modbus_server_change_port(Inverter_Info.Com_Info.Modbus_Port);//监听新的端口号
			}
//-----------------------------------------------------
            if(Com_Data.Start_DHCP == true)
            {
                if(Inverter_Info.Com_Info.Net.dhcp_flag == D_DHCP_ON)
                {
                    memset(Inverter_Info.Com_Info.Net.ip, 0, 4);
                    drv_ioctl(gt_glp.fnet, NET_GET_DHCP_STATUS, &Com_Data.DHCP_Bound);
                    if(Com_Data.DHCP_Bound == DHCP_BOUND)//DHCP分配成功
                    {
                        Com_Data.Start_DHCP = false;
                        drv_ioctl(gt_glp.fnet, NET_GET_IP,      Inverter_Info.Com_Info.Net.ip);
                        drv_ioctl(gt_glp.fnet, NET_GET_SUBMASK, Inverter_Info.Com_Info.Net.submask);
                        drv_ioctl(gt_glp.fnet, NET_GET_GATEWAY, Inverter_Info.Com_Info.Net.gateway);
                        drv_ioctl(gt_glp.fnet, NET_GET_DNS1,    Inverter_Info.Com_Info.Net.dns1);
                        drv_ioctl(gt_glp.fnet, NET_GET_DNS2,    Inverter_Info.Com_Info.Net.dns2);
                        GetTcpAddr(Inverter_Info.Com_Info.Net.dns1, Inverter_Info.Com_Info.Net.dns2, SOLARBANK_DOMAIN_NAME, 1);
                        swap(Inverter_Info.Com_Info.Net.ip);
                        swap(Inverter_Info.Com_Info.Net.submask);
                        swap(Inverter_Info.Com_Info.Net.gateway);
                        swap(Inverter_Info.Com_Info.Net.dns1);
                        swap(Inverter_Info.Com_Info.Net.dns2);
                    }
                    else
                    {
                        Com_Data.Start_DHCP = true;//不断的DHCP
                    }
                }
                else if(Inverter_Info.Com_Info.Net.dhcp_flag == D_DHCP_OFF)
                {
                    Com_Data.Start_DHCP = false;
                }
            }
        }
        if(netline_connected() == false)//网线断开时，使下次插上后，重新DHCP;
        {
            Com_Data.Start_DHCP = true;
        }
//-----------------------------------------------------
		doChangeSignalUpload();//遥信变位上传  ccg 2013.11.30
//-----------------------------------------------------
		if(save_flag_int)//webserver
		{
			save_flag_int=0;
			webserver_do_save_int();
		}
//-----------------------------------------------------
		for(uint8_t i=0;i<TCP_MODBUS_LINK_NUM;i++)
		{
			//上位机通过网络，建立连接后，40S内无访问，计量板主动断开连接。
			if((Inverter_Info.Com_Info.IO_PC_Modbus_TCP_Com_Count[i] == 0) && (NULL != g_modbus_ss_tcp[i]))
			{
				tcp_modbus_server_conn_close(g_modbus_ss_tcp[i]->pcb, (TCPSvrState *)g_modbus_ss_tcp[i]);
			}
		}
		embf_delay(2u);
    }
}
