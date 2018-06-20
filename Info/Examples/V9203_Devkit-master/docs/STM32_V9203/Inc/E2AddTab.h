#ifndef _E2ADDRTAB_H_
#define _E2ADDRTAB_H_
//      数据内容  

//    HEX    

#define  EEP_COMADD  0x0000  //通讯地址（12位）  

#define  EEP_METERNO  0x0006  //表号（12位）  

#define  EEP_ASSETNO  0x000C  //资产管理编码（ASCII码）  

#define  EEP_STANDUN  0x002C  //额定电压（ASCII码）  

#define  EEP_STANDIN  0x0032  //额定电流/基本电流（ASCII码）  

#define  EEP_MAXIN  0x0038  //最大电流（ASCII码）  

//#define    0x003E    

//#define    0x0040    

        

        

//      数据内容  

//    HEX    

#define  EEP_YGACCURACY  0x0040  //有功准确度等级（ASCII码）  

#define  EEP_DBWZXX  0x0044  //电表位置信息  

#define  EEP_METERMODEL  0x004F  //电表型号（ASCII码）  

#define  EEP_PDATE  0x0059  //生产日期（ASCII码）  

#define  EEP_PROTOCOLNO  0x0063  //协议版本号（ASCII码）  

#define  EEP_KHBH  0x0073  //客户编号  

#define  EEP_YGCALC  0x0079  //有功组合方式特征字  

#define  EEP_IARBDM  0x007A  //红外波特率状态字  

#define  EEP_UARTBDM  0x007B  //485波特率状态字  

#define  EEP_PROTIME  0x007C  //编程有效时间(自扩)  

//#define    0x007E  CRC  

//#define    0x0080    

        

//      数据内容  

//    HEX    

#define  EEP_645PASSWD1  0x0080  //第一级密码  

#define  EEP_645PASSWD2  0x0083  //第二级密码  

#define  EEP_645PASSWD3  0x0086  //第三级密码  

#define  EEP_645PASSWD4  0x0089  //第四级密码  

#define  EEP_645PASSWD5  0x008C  //第五级密码  

#define  EEP_645PASSWD6  0x008F  //第六级密码  

#define  EEP_645PASSWD7  0x0092  //第七级密码  

#define  EEP_645PASSWD8  0x0095  //第八级密码  

#define  EEP_645PASSWD9  0x0098  //第九级密码  

#define  EEP_CBR1   0x009B  //每月第一结算日  

#define  EEP_CBR2   0x009D  //每月第二结算日  

#define  EEP_CBR3   0x009F  //每月第三结算日  

#define  EEP_MAX_LX_NUM  0x00A1  //自动循环显示屏数  

#define  EEP_LXTM  0x00A2  //轮显显示时间  

#define  EEP_EDEC  0x00A3  //显示电能小数位数  

#define  EEP_PDEC  0x00A4  //显示功率(最大需量显示位数)  

#define  EEP_MAX_JX_NUM  0x00A5  //按键循环显示屏数  

#define  EEP_SHOWERRPOS  0x00A6  //ERR屏显示位置（自扩）  

#define  EEP_POWUPJXTM  0x00A7  //有电下键显显示时间(自扩)  

#define  EEP_POWDWJXTM  0x00A8  //停电下键显显示时间(自扩)  

#define  EEP_POWOFFXSTM  0x00A9  //停电显示时间(自扩)  

#define  EEP_FULLSHOWTM  0x00AB  //上电全屏时间（自扩）  

#define  EEP_POWUPBGTM  0x00AC  //上电开背光时间(自扩)  

#define  EEP_IRBGTM  0x00AD  //红外触发背光时间(自扩)  

#define  EEP_KEYBGTM  0x00AE  //按键触发背光时间(自扩)  

#define  EEP_ESAMERRBGTM  0x00AF  //ESAM错误字显示时间（背光亮时间）(自扩)  

#define  EEP_LEDSFCKTM  0x00B0  //跳闸灯自检时间(自扩)  

#define  EEP_ZDFREEZTM  0x00B1  //整点冻结起始时间  

#define  EEP_ZDTMJG  0x00B6  //整点冻结时间间隔  

#define  EEP_DSFRZ  0x00B7  //定时冻结时间  

#define  EEP_RFREEZTM  0x00BB  //日冻结时间  

#define  EEP_DBYXTZZ  0x00BD  //电表运行特征字1  

//#define    0x00BE  CRC  

        

        

//      数据内容  

//    HEX    

#define  EEP_JBTOTAL  0x00C0  //表常数  

#define  EEP_JBBCDY  0x00C2  //标称电压  

#define  EEP_JBBCDL  0x00C4  //标称电流  

//#define    0x00C6  保留  

//#define    0x00C8  起动/潜动判断门限值寄存器H  

//#define    0x00CC  起动/潜动判断门限值寄存器L  

//#define    0x00D0  电流检测门限值  

//#define    0x00D4  角差0  

//#define    0x00D8  计量控制寄存器0  

//#define    0x00DC  计量控制寄存器1  

//#define    0x00E0  计量控制寄存器2  

//#define    0x00E4  模拟控制寄存器0  

//#define    0x00E8  模拟控制寄存器1  

//#define    0x00EC  模拟控制寄存器2  

//#define    0x00F0  模拟控制寄存器3  

//#define    0x00F4  A全波电压有效值比差寄存器  

//#define    0x00F8  A全波电流有效值比差寄存器  

//#define    0x00FC  A全波有功功率比差寄存器  

//#define    0x0100  A全波有功功率二次补偿寄存器     

//#define    0x0104  A全波无功功功率二次补偿寄存器     

//#define    0x0108  B全波电压有效值比差寄存器  

//#define    0x010C  B全波电流有效值比差寄存器  

//#define    0x0110  B全波有功功率比差寄存器  

//#define    0x0114  B全波有功功率二次补偿寄存器     

//#define    0x0118  B全波无功功功率二次补偿寄存器     

//#define    0x011C  C全波电压有效值比差寄存器  

//#define    0x0120  C全波电流有效值比差寄存器  

//#define    0x0124  C全波有功功率比差寄存器  

//#define    0x0128  C全波有功功率二次补偿寄存器     

//#define    0x012C  C全波无功功功率二次补偿寄存器     

//#define    0x0130  功率比例系数  

//#define    0x0134  电压通道比例系数  

//#define    0x0138  电流通道1比例系数  

//#define    0x013C  保留  

      //CRC  

        

//      数据内容  

//    HEX    

#define  EEP_SQCNT  0x0180  //年时区数  

#define  EEP_RSDCNT  0x0181  //日时段表数  

#define  EEP_FSDCNT  0x0182  //时段数（每日切换数）  

#define  EEP_FEECNT  0x0183  //费率数  

#define  EEP_SZX  0x0184  //周休日状态字  

#define  EEP_ZXSD  0x0185  //周休日时段表  

#define  EEP_SFRZTIME  0x0186  //身份认证时效  

#define  EEP_JRCNT  0x0188  //公共假日数(不带年/带年)  

#define  EEP_SQQHTM  0x018A  //两套时区表切换时间  

#define  EEP_RSDQHTM  0x018F  //两套日时段表切换时间  

#define  EEP_PDIRTIME  0x0194  //功率反向事件判定延时时间  

#define  EEP_PDIRGATE  0x0195  //功率反向事件有功功率触发下限  

//#define    0x0198  //保留  

#define  EEP_CFSAVECFG  0x0199  //CF保存配置项（自扩）  

#define  EEP_CBRCFG  0x019A  //抄表日补冻配置项（自扩）  

#define  EEP_DJCFG  0x019B  //冻结补冻配置项（自扩）  

#define  EEP_PWLCFG  0x019C  //密码等级有效配置项（自扩）  

#define  EEP_SQSDCFG  0x019E  //时区和时段切换时间设置判断方式（自扩）  

#define  EEP_ENSHOWCFG  0x019F  //电量显示格式参数配置项（自扩）  

#define  EEP_TMSHOWCFG  0x01A0  //时间显示格式参数配置项（自扩）  

#define  EEP_PWOFFSHOWMDCFG  0x01A1  //停电显示模式参数配置项（自扩）  

#define  EEP_JDQCKCFG  0x01A2  //继电器检测方式参数配置项（自扩）  

#define  EEP_METERCLRCNT  0x01A3  //电表清零次数（自扩）  

#define  EEP_METERCLFENGVAL  0x01A4  //电表清零电量阀值（自扩）  

#define  EEP_PRODUCE  0x01A8  //生产模式状态(自扩)  

#define  EEP_PRODUCETM  0x01A9  //生产模式退出时间(自扩)  

#define  EEP_DATETIME  0x01AB  //时钟备份  

#define  EEP_EPS  0x01B2  //当前有功电量  

#define  EEP_NEPS  0x01B6  //当前无功电量  

#define  EEP_ZDSBMSZ  0x01BA  //主动上报模式字  

//#define    0x01BC  CRC  

        

        

//      数据内容  

//    HEX    

#define  EEP_LXTABLE  0x01C0  //轮显项1  

//#define    0x01C5  轮显项2  

//#define    0x01CA  轮显项3  

//#define    0x01CF  轮显项4  

//#define    0x01D4  轮显项5  

//#define    0x01D9  轮显项6  

//#define    0x01DE  轮显项7  

//#define    0x01E3  轮显项8  

//#define    0x01E8  轮显项9  

//#define    0x01ED  轮显项10  

        

        

        

        

        

//      数据内容  

//    HEX    

#define  EEP_JXTABLE  0x0200  //键显项1  

//#define    0x0205  键显项2  

//#define    0x020A  键显项3  

//#define    0x020F  键显项4  

//#define    0x0214  键显项5  

//#define    0x0219  键显项6  

//#define    0x021E  键显项7  

//#define    0x0223  键显项8  

//#define    0x0228  键显项9  

//#define    0x022D  键显项10  

        

        

        

//      数据内容  

//    HEX    

#define  EEP_EPFH_PT  0x0240  //负荷指针  

//#define    0x0242  当前组合正向有功电量(总)  

//#define    0x0246  当前组合正向无功电量(总)  

//#define    0x024A  A相电压有效值  

//#define    0x024C  B相电压有效值  

//#define    0x024E  C相电压有效值  

//#define    0x0250  A相电流有效值  

//#define    0x0252  B相电流有效值  

//#define    0x0254  C相电流有效值  

//#define    0x0256  A相有功功率  

//#define    0x0258  B相有功功率  

//#define    0x025A  C相有功功率  

//#define    0x025C  A相无功功率  

//#define    0x025E  B相无功功率  

//#define    0x0260  C相无功功率  

//#define    0x0262  时间  

//#define    0x0266  36×4×24×6（7天数据）  

//    0x5369    

        

        

//      数据内容  

//    HEX    

#define  EEP_ZBCNT_PT 0x53C0  //周波指针  

#define  EEP_ZBUA_PT  0x53C2  //A电压原始值  

#define  EEP_ZBIA_PT  0x5DC2  //A电流原始值  

#define  EEP_ZBUB_PT  0x67C2  //B电压原始值  

#define  EEP_ZBIB_PT  0x71C2  //B电流原始值  

#define  EEP_ZBUC_PT  0x7BC2  //C电压原始值  

#define  EEP_ZBIC_PT  0x85C2  //C电流原始值 

#define  EEP_FFTUA_PT 0x9000  //256个点存储地址,512字节
#define  EEP_FFTIA_PT 0x92BC
#define  EEP_FFTUB_PT 0x9578
#define  EEP_FFTIB_PT 0x9834
#define  EEP_FFTUC_PT 0x9AF0
#define  EEP_FFTIC_PT 0x9DAC


#endif
