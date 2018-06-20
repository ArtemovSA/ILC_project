#define INIT_EXT
#include"Includes.h"


const uint8   InitPara0[]=
{
    0x11,0x11,0x11,0x11,0x11,0x11,  //通讯地址
    0x01,0x00,0x00,0x00,0x00,0x00,  //表号

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,    //资产管理编码

    0,0,'V','0','2','2',    //额定电压
    0,0,0,0,'A','5',        //额定电流
    0,0,0,'A','0','6',      //最大电流
};

const uint8   InitPara1[]=
{
    0x00,'0','.','2',               //有功准确度等级
    0x00,0x00,0x00,0x00,0x00,       //电表位置信息
    0x00,0x00,0x00,0x00,0x00,0x00,           

    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,       //电表型号

    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,       //生产日期


    0x32,0x31,0x2D,0x37,0x30,
    0x30,0x32,0x2D,0x35,0x34,
    0x36,0x20,0x54,0x2F,0x4C,0x44,  //协议版本号

    0x00,0x00,0x00,0x00,0x00,0x00,  //客户编号

    0x05,                           //有功组合特征字
    0x04,                           //调制红外 
    0x08,                           //485-1
    0xf0,0x00,                      //编程有效时间(自扩)
};


const uint8   InitPara3[]=
{
    0x00,0x00,0x00,     //第一级密码
    0x00,0x00,0x00,     //第二级密码
    0x00,0x00,0x00,     //第三级密码
    0x00,0x00,0x00,     //第四级密码
    0x11,0x11,0x11,     //第五级密码
    0x00,0x00,0x00,     //第六级密码
    0x00,0x00,0x00,     //第七级密码
    0x00,0x00,0x00,     //第八级密码
    0x00,0x00,0x00,     //第九级密码
    0x00,0x01,          //第1结算日
    0x99,0x99,          //第2结算日
    0x99,0x99,          //第3结算日
    0x0B,               //自动循环显示屏数
    0x05,               //轮显显示时间
    0x02,               //显示电能小数位数
    0x04,               //显示功率小数位数
    0x0B,               //按键循环显示屏数
    0x1a,               //错误显示屏位置
    0x1E,               //有电下键显显示时间(自扩)
    0x1E,               //停电下键显显示时间(自扩)
    0x00,0x00,          //停电显示时间(自扩)
    0x05,               //上电全屏时间（自扩）
    0x05,               //上电开背光时间(自扩)
    0x14,               //红外触发背光时间(自扩)
    0x3C,               //按键触发背光时间(自扩)
    0x05,               //ESAM错误字显示时间（背光亮时间）(自扩)
    0x02,               //跳闸灯自检时间(自扩)
    0x00,0x00,0x01,0x01,0x10, //整点冻结起始时间
    0x3c,                   //整点冻结时间间隔
    0x00,0x00,0x15,0x99,    //定时冻结      
    0x00,0x00,              //日冻结时间
    0x00,                   //电表运行特征字（不启用后续标志）  
};


const uint8   InitPara5[sizeof(S_JBPM)-2]=
{
    0xB0,0x04,     //表常数
    0xf0,0x55,     //标称电压
    0x88,0x13,     //标称电流
    0x00,0x00,     //保留

    0x05,0xFF,0xD3,0x2F,//有功门限值寄存器
    0xe5,0x21,0x02,0x00,//起动/潜动判断门限值寄存器高
    0x4e,0xeb,0x01,0x00,//起动/潜动判断门限值寄存器低
    0x00,0x00,0xf0,0xff,//电流检测门限值寄存器
    0x82,0x82,0x82,0x00,//角差寄存器

    0xff,0x00,0x00,0x00,//计量控制寄存器0
    0x00,0x00,0x00,0x00,//计量控制寄存器1
    0xff,0x80,0x00,0x07,//计量控制寄存器2

    0x33,0x03,0x00,0x00,//模拟控制寄存器0
    0x00,0x00,0x00,0x00,//模拟控制寄存器1
    0x00,0x54,0x00,0x77,//模拟控制寄存器2
    0x06,0x04,0x0d,0x00,//模拟控制寄存器3
   

    0x00,0x00,0x00,0x00,//A相全波电压有效值比差寄存器
    0x00,0x00,0x00,0x00,//A相全波电流有效值比差寄存器
    0x00,0x00,0x00,0x00,//A相全波有功功率比差寄存器
    0x00,0x00,0x00,0x00,//A相全波有功功率二次补偿寄存器
    0x00,0x00,0x00,0x00,//A相全波无功功率二次补偿寄存器

    0x00,0x00,0x00,0x00,//B相全波电压有效值比差寄存器
    0x00,0x00,0x00,0x00,//B相全波电流有效值比差寄存器
    0x00,0x00,0x00,0x00,//B相全波有功功率比差寄存器
    0x00,0x00,0x00,0x00,//B相全波有功功率二次补偿寄存器
    0x00,0x00,0x00,0x00,//B相全波无功功率二次补偿寄存器

    0x00,0x00,0x00,0x00,//C相全波电压有效值比差寄存器
    0x00,0x00,0x00,0x00,//C相全波电流有效值比差寄存器
    0x00,0x00,0x00,0x00,//C相全波有功功率比差寄存器
    0x00,0x00,0x00,0x00,//C相全波有功功率二次补偿寄存器
    0x00,0x00,0x00,0x00,//C相全波无功功率二次补偿寄存器

    0x01,0x00,0x00,0x00,//电压显示比例系数k
    0x01,0x00,0x00,0x00,//电流I1显示比例系数k
    0x01,0x00,0x00,0x00,//功率显示比例系数k
    0x00,0x00,          //保留
    //CRC
};

const uint8   InitPara7[]=
{
    0x02,                       //年时区数
    0x02,                       //日时段表数
    0x08,                       //日时段（每组合总电量日切换）
    0x04,                       //费率数
    0x7F,                       //周休日特征字
    0x01,                       //周休日采用时段表号
    0x05,0x00,                  //身份认证时效
    0x00,0x00,                  //公共假日数
    0x00,0x00,0x00,0x00,0x00,   //时区表切换时间
    0x00,0x00,0x00,0x00,0x00,   //时段表切换时间
    0x03,                       //功率反向事件判定延时时间
    0x33,0x00,0x00,             //功率反向事件有功功率触发下限 5A 220V 1100W 0.3%=3.3W
    0x00,                       //保留    
    0x55,                       //CF保存配置项（自扩）
    0x00,                       //抄表日补冻配置项（自扩）
    0x00,                       //冻结补冻配置项（自扩）
    0x00,0x00,                  //密码等级有效配置项（自扩）
    0x00,                       //时区和时段切换时间设置判断方式（自扩）
    0x02,                       //电量显示格式参数配置项（自扩）
    0x00,                       //时间显示格式参数配置项（自扩）
    0x00,                       //停电显示模式参数配置项（自扩）
    0x00,//继电器检测方式参数配置项（自扩）
    0x00,//电表清零次数（自扩）
    0x00,0x00,0x00,0x00,//电表清零电量阀值（自扩）
    0x55,               //生产模式状态(自扩)
    0xA0,0x05,          //生产模式退出剩余时间(自扩)
    0x00,0x00,0x18,0x23,0x05,0x12,0x03,//当前时钟备份(自扩)
    0x00,0x00,0x00,0x00,  //有功电量
    0x00,0x00,0x00,0x00,  //无功电量
    0x00,0x00,   //主动上报模式字
};

const uint8   InitPara29[]=
{
    0x00,0x00,0x00,0x00,0x00,       //循显1  当前总电量
    0x00,0x01,0x01,0x02,0x00,       //循显5 A电压
    0x00,0x02,0x01,0x02,0x00,       //循显6 B电压
    0x00,0x03,0x01,0x02,0x00,       //循显7 C电压
    0x00,0x01,0x02,0x02,0x00,       //循显8 A电流
    0x00,0x02,0x02,0x02,0x00,       //循显9 B电流
    0x00,0x03,0x02,0x02,0x00,       //循显10 C电流

    0x00,0x00,0x03,0x02,0x00,       //循显11 功率
    0x00,0x01,0x03,0x02,0x00,       //循显12 A功率
    0x00,0x02,0x03,0x02,0x00,       //循显13 B功率
    0x00,0x03,0x03,0x02,0x00,       //循显14 c功率
};

const uint8   InitPara37[]=
{
    0x00,0x00,0x00,0x00,0x00,       //按显1  当前总电量
    0x00,0x01,0x01,0x02,0x00,       //按显13 A电压
    0x00,0x02,0x01,0x02,0x00,       //按显14 B电压
    0x00,0x03,0x01,0x02,0x00,       //按显15 C电压

    0x00,0x01,0x02,0x02,0x00,       //按显16 A电流
    0x00,0x02,0x02,0x02,0x00,       //按显17 B电流
    0x00,0x03,0x02,0x02,0x00,       //按显18 C电流

    0x00,0x00,0x03,0x02,0x00,       //按显19 功率
    0x00,0x01,0x03,0x02,0x00,       //按显20 A功率
    0x00,0x02,0x03,0x02,0x00,       //按显21 B功率  
    0x00,0x03,0x03,0x02,0x00,       //循显22 c功率
};

typedef struct 
{
    const uint8  * E2ParaTabAdrr;   //E2参数表格地址
    uint16 E2Adrr;          //E2地址
    uint8  uclen;           //数据长度
}GS_E2PARA;

const  GS_E2PARA   PageCRCAddr[]=
{
    {InitPara0,     EEP_COMADD,     sizeof(InitPara0),  },
    {InitPara1,     EEP_YGACCURACY, sizeof(InitPara1),  },
    {InitPara3,     EEP_645PASSWD1, sizeof(InitPara3),  },
    {InitPara7,     EEP_SQCNT,      sizeof(InitPara7),  },
};

/*=========================================================================================\n
* @function_name: InitE2Data
* @function_file: InitPara.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-04)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void InitE2Data(uint8 uc_RTCSave)
{
    Word16 CRC1;
    uint8 i;
    uint16 ShowLen;
    uint16 Wrlen;

    ClRCRCWord(guc_InitWd);

    BE_ReadP(EEP_COMADD,XDATA_RAMZONE,6);                   //读取通讯地址

    //通讯地址和表号，参数初始化时不更新
    if(JudgeIsNum(XDATA_RAMZONE,6)==Const_Data_Err)         //通讯地址不是BCD吗格式的话，初始化通讯地址
    {
        memcpy(XDATA_RAMZONE,InitPara0,12);                //拷贝Flash数据到RAM中
        SysE2ParaSetManage(EEP_COMADD, XDATA_RAMZONE,12);    //写通讯地址
    }

   // memcpy(XDATA_RAMZONE,PageCRCAddr[0].E2ParaTabAdrr+12,PageCRCAddr[0].uclen-12);            //拷贝Flash数据到RAM中
   // SysE2ParaSetManage(PageCRCAddr[0].E2Adrr+12,XDATA_RAMZONE,PageCRCAddr[0].uclen-12);         //写入顶点温度和B值到E2中

    for(i=1;i<sizeof(PageCRCAddr)/sizeof(GS_E2PARA);i++)
    {
        CLRWDT();               //喂狗
        memcpy(XDATA_RAMZONE,PageCRCAddr[i].E2ParaTabAdrr,PageCRCAddr[i].uclen);//拷贝Flash数据到RAM中
        CRC1.word=do_CRC(XDATA_RAMZONE,PageCRCAddr[i].uclen);
        XDATA_RAMZONE[PageCRCAddr[i].uclen]=CRC1.byte[0];
        XDATA_RAMZONE[PageCRCAddr[i].uclen+1]=CRC1.byte[1];   //拷贝CRC到RAM中
        //BE_WriteP(PageCRCAddr[i].E2Adrr,XDATA_RAMZONE,PageCRCAddr[i].uclen+2);   //写入到E2中
        SysE2ParaSetManage(PageCRCAddr[i].E2Adrr,XDATA_RAMZONE,PageCRCAddr[i].uclen+2);
     }

    //校表参数单独判断,首先判读E2中的CRC是否正确，如果正确，则不需要初始化
    BE_ReadP(EEP_JBTOTAL,XDATA_RAMZONE,sizeof(S_JBPM));
    if(Uint8_To_Uint16(XDATA_RAMZONE+sizeof(S_JBPM)-2)!=do_CRC(XDATA_RAMZONE,sizeof(S_JBPM)-2))
    {

        CLRWDT();               //喂狗
        memcpy(XDATA_RAMZONE,InitPara5,sizeof(S_JBPM)-2);                     //拷贝Flash数据到RAM中
        CRC1.word=do_CRC(XDATA_RAMZONE,sizeof(S_JBPM)-2);
        XDATA_RAMZONE[sizeof(S_JBPM)-2]=CRC1.byte[0];
        XDATA_RAMZONE[sizeof(S_JBPM)-1]=CRC1.byte[1];                            //拷贝CRC到RAM中
        //BE_WriteP(EEP_JBTOTAL,XDATA_RAMZONE,sizeof(S_JBPM));                    //写入到E2中
        SysE2ParaSetManage(EEP_JBTOTAL,XDATA_RAMZONE,sizeof(S_JBPM));
    }

    //写入轮显参数

    ShowLen=sizeof(InitPara29);
    for(i=0;i<((sizeof(InitPara29)-1)/64+1);i++)
    {

        CLRWDT();               //喂狗
        if(ShowLen>=64)
        {
            memcpy(XDATA_RAMZONE,&InitPara29[i*64],64);    //拷贝Flash数据到RAM中
            Wrlen=64;
            ShowLen-=64;
        }else
        {
            memcpy(XDATA_RAMZONE,&InitPara29[i*64],ShowLen);    //拷贝Flash数据到RAM中
            Wrlen=ShowLen;
        }

        //BE_WriteP(EEP_LXTABLE+i*64,XDATA_RAMZONE,Wrlen);          //写入E2中
        SysE2ParaSetManage(EEP_LXTABLE+i*64,XDATA_RAMZONE,Wrlen);   
    }

    //写入键显参数
    ShowLen=sizeof(InitPara37); 
    for(i=0;i<((sizeof(InitPara37)-1)/64+1);i++)            
    {
        CLRWDT();                                                   //喂狗
        if(ShowLen>64)
        {
            memcpy(XDATA_RAMZONE,&InitPara37[i*64],64);           //拷贝Flash数据到RAM中
            Wrlen=64;
            ShowLen-=64;
        }else
        {
            memcpy(XDATA_RAMZONE,&InitPara37[i*64],ShowLen);      //拷贝Flash数据到RAM中
            Wrlen=ShowLen;
        }
        //BE_WriteP(EEP_JXTABLE+i*64,XDATA_RAMZONE,Wrlen);            //写入E2中
        SysE2ParaSetManage(EEP_JXTABLE+i*64,XDATA_RAMZONE,Wrlen);  
    }

    CLRWDT();                                                       //喂狗

   // SetCRCWord(guc_ClearWd);
    Data_ClearMeter();                                              //电表清零
    uint8 addr;
    addr = EEP_EPFH_PT + 2;
    SysE2ParaSetManage(EEP_EPFH_PT,(uint8*)&addr,2);
    memset(XDATA_RAMZONE,0x00,64);                                              //先清零外部RAM 
    /*for(j=0;j<gCs_MeterClearAddrTab[0].ucPageCnt;j++)                           //参数初始化的时候清零电表清零记录
    {
        CLRWDT();               //喂狗
        BE_WriteP(gCs_MeterClearAddrTab[0].uiE2Addr+64*j,XDATA_RAMZONE,64);    //电表清零记录
    }*/
//
    //gs_FeeData.uc_FeeRSDN=0x01;
    //gs_FeeData.uc_FeeSQN=0x01;
    gs_MeterState.ui_MeterState3.RunCode3Word.RateTable=false;                  //修改电表状态字，使用第一套时段
    gs_MeterState.ui_MeterState3.RunCode3Word.TimeZoneTable=false;              //修改电表状态字，使用第一套时段
    
   // memset((uint8*)&gs_ReportState,0x00,sizeof(REPORTSTATE));                   //主动上报状态清零
   // memset((uint8*)&gs_ReportStateFlg,0x00,sizeof(REPORTSTATE));                //主动上报状态标志清零
  
    guc_FactoryType=FactorMd;
    gui_DyProDmOut=0x2760;      //7天
    //BE_WriteP(EEP_PRODUCELEFTTM,(uint8*)&gui_DyProDmOut,0x02);                  //厂内剩余时间
    SysE2ParaSetManage(EEP_PRODUCE,(uint8*)&guc_FactoryType,1);                 //写入厂内状态

    SleepTimeProc();                                            //从RTC读取时钟到RAM中
    gui_RefreshEvent|=(flgEtPara_Config+flgEtPara_Fee+flgEtPara_Show+flgEtPara_LoadCurve+flgEtPara_PowDir);
}
