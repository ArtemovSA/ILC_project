#ifndef _E2ADDRTAB_H_
#define _E2ADDRTAB_H_
//      ��������  

//    HEX    

#define  EEP_COMADD  0x0000  //ͨѶ��ַ��12λ��  

#define  EEP_METERNO  0x0006  //��ţ�12λ��  

#define  EEP_ASSETNO  0x000C  //�ʲ�������루ASCII�룩  

#define  EEP_STANDUN  0x002C  //���ѹ��ASCII�룩  

#define  EEP_STANDIN  0x0032  //�����/����������ASCII�룩  

#define  EEP_MAXIN  0x0038  //��������ASCII�룩  

//#define    0x003E    

//#define    0x0040    

        

        

//      ��������  

//    HEX    

#define  EEP_YGACCURACY  0x0040  //�й�׼ȷ�ȵȼ���ASCII�룩  

#define  EEP_DBWZXX  0x0044  //���λ����Ϣ  

#define  EEP_METERMODEL  0x004F  //����ͺţ�ASCII�룩  

#define  EEP_PDATE  0x0059  //�������ڣ�ASCII�룩  

#define  EEP_PROTOCOLNO  0x0063  //Э��汾�ţ�ASCII�룩  

#define  EEP_KHBH  0x0073  //�ͻ����  

#define  EEP_YGCALC  0x0079  //�й���Ϸ�ʽ������  

#define  EEP_IARBDM  0x007A  //���Ⲩ����״̬��  

#define  EEP_UARTBDM  0x007B  //485������״̬��  

#define  EEP_PROTIME  0x007C  //�����Чʱ��(����)  

//#define    0x007E  CRC  

//#define    0x0080    

        

//      ��������  

//    HEX    

#define  EEP_645PASSWD1  0x0080  //��һ������  

#define  EEP_645PASSWD2  0x0083  //�ڶ�������  

#define  EEP_645PASSWD3  0x0086  //����������  

#define  EEP_645PASSWD4  0x0089  //���ļ�����  

#define  EEP_645PASSWD5  0x008C  //���弶����  

#define  EEP_645PASSWD6  0x008F  //����������  

#define  EEP_645PASSWD7  0x0092  //���߼�����  

#define  EEP_645PASSWD8  0x0095  //�ڰ˼�����  

#define  EEP_645PASSWD9  0x0098  //�ھż�����  

#define  EEP_CBR1   0x009B  //ÿ�µ�һ������  

#define  EEP_CBR2   0x009D  //ÿ�µڶ�������  

#define  EEP_CBR3   0x009F  //ÿ�µ���������  

#define  EEP_MAX_LX_NUM  0x00A1  //�Զ�ѭ����ʾ����  

#define  EEP_LXTM  0x00A2  //������ʾʱ��  

#define  EEP_EDEC  0x00A3  //��ʾ����С��λ��  

#define  EEP_PDEC  0x00A4  //��ʾ����(���������ʾλ��)  

#define  EEP_MAX_JX_NUM  0x00A5  //����ѭ����ʾ����  

#define  EEP_SHOWERRPOS  0x00A6  //ERR����ʾλ�ã�������  

#define  EEP_POWUPJXTM  0x00A7  //�е��¼�����ʾʱ��(����)  

#define  EEP_POWDWJXTM  0x00A8  //ͣ���¼�����ʾʱ��(����)  

#define  EEP_POWOFFXSTM  0x00A9  //ͣ����ʾʱ��(����)  

#define  EEP_FULLSHOWTM  0x00AB  //�ϵ�ȫ��ʱ�䣨������  

#define  EEP_POWUPBGTM  0x00AC  //�ϵ翪����ʱ��(����)  

#define  EEP_IRBGTM  0x00AD  //���ⴥ������ʱ��(����)  

#define  EEP_KEYBGTM  0x00AE  //������������ʱ��(����)  

#define  EEP_ESAMERRBGTM  0x00AF  //ESAM��������ʾʱ�䣨������ʱ�䣩(����)  

#define  EEP_LEDSFCKTM  0x00B0  //��բ���Լ�ʱ��(����)  

#define  EEP_ZDFREEZTM  0x00B1  //���㶳����ʼʱ��  

#define  EEP_ZDTMJG  0x00B6  //���㶳��ʱ����  

#define  EEP_DSFRZ  0x00B7  //��ʱ����ʱ��  

#define  EEP_RFREEZTM  0x00BB  //�ն���ʱ��  

#define  EEP_DBYXTZZ  0x00BD  //�������������1  

//#define    0x00BE  CRC  

        

        

//      ��������  

//    HEX    

#define  EEP_JBTOTAL  0x00C0  //����  

#define  EEP_JBBCDY  0x00C2  //��Ƶ�ѹ  

#define  EEP_JBBCDL  0x00C4  //��Ƶ���  

//#define    0x00C6  ����  

//#define    0x00C8  ��/Ǳ���ж�����ֵ�Ĵ���H  

//#define    0x00CC  ��/Ǳ���ж�����ֵ�Ĵ���L  

//#define    0x00D0  �����������ֵ  

//#define    0x00D4  �ǲ�0  

//#define    0x00D8  �������ƼĴ���0  

//#define    0x00DC  �������ƼĴ���1  

//#define    0x00E0  �������ƼĴ���2  

//#define    0x00E4  ģ����ƼĴ���0  

//#define    0x00E8  ģ����ƼĴ���1  

//#define    0x00EC  ģ����ƼĴ���2  

//#define    0x00F0  ģ����ƼĴ���3  

//#define    0x00F4  Aȫ����ѹ��Чֵ�Ȳ�Ĵ���  

//#define    0x00F8  Aȫ��������Чֵ�Ȳ�Ĵ���  

//#define    0x00FC  Aȫ���й����ʱȲ�Ĵ���  

//#define    0x0100  Aȫ���й����ʶ��β����Ĵ���     

//#define    0x0104  Aȫ���޹������ʶ��β����Ĵ���     

//#define    0x0108  Bȫ����ѹ��Чֵ�Ȳ�Ĵ���  

//#define    0x010C  Bȫ��������Чֵ�Ȳ�Ĵ���  

//#define    0x0110  Bȫ���й����ʱȲ�Ĵ���  

//#define    0x0114  Bȫ���й����ʶ��β����Ĵ���     

//#define    0x0118  Bȫ���޹������ʶ��β����Ĵ���     

//#define    0x011C  Cȫ����ѹ��Чֵ�Ȳ�Ĵ���  

//#define    0x0120  Cȫ��������Чֵ�Ȳ�Ĵ���  

//#define    0x0124  Cȫ���й����ʱȲ�Ĵ���  

//#define    0x0128  Cȫ���й����ʶ��β����Ĵ���     

//#define    0x012C  Cȫ���޹������ʶ��β����Ĵ���     

//#define    0x0130  ���ʱ���ϵ��  

//#define    0x0134  ��ѹͨ������ϵ��  

//#define    0x0138  ����ͨ��1����ϵ��  

//#define    0x013C  ����  

      //CRC  

        

//      ��������  

//    HEX    

#define  EEP_SQCNT  0x0180  //��ʱ����  

#define  EEP_RSDCNT  0x0181  //��ʱ�α���  

#define  EEP_FSDCNT  0x0182  //ʱ������ÿ���л�����  

#define  EEP_FEECNT  0x0183  //������  

#define  EEP_SZX  0x0184  //������״̬��  

#define  EEP_ZXSD  0x0185  //������ʱ�α�  

#define  EEP_SFRZTIME  0x0186  //�����֤ʱЧ  

#define  EEP_JRCNT  0x0188  //����������(������/����)  

#define  EEP_SQQHTM  0x018A  //����ʱ�����л�ʱ��  

#define  EEP_RSDQHTM  0x018F  //������ʱ�α��л�ʱ��  

#define  EEP_PDIRTIME  0x0194  //���ʷ����¼��ж���ʱʱ��  

#define  EEP_PDIRGATE  0x0195  //���ʷ����¼��й����ʴ�������  

//#define    0x0198  //����  

#define  EEP_CFSAVECFG  0x0199  //CF���������������  

#define  EEP_CBRCFG  0x019A  //�����ղ��������������  

#define  EEP_DJCFG  0x019B  //���Ჹ�������������  

#define  EEP_PWLCFG  0x019C  //����ȼ���Ч�����������  

#define  EEP_SQSDCFG  0x019E  //ʱ����ʱ���л�ʱ�������жϷ�ʽ��������  

#define  EEP_ENSHOWCFG  0x019F  //������ʾ��ʽ���������������  

#define  EEP_TMSHOWCFG  0x01A0  //ʱ����ʾ��ʽ���������������  

#define  EEP_PWOFFSHOWMDCFG  0x01A1  //ͣ����ʾģʽ���������������  

#define  EEP_JDQCKCFG  0x01A2  //�̵�����ⷽʽ���������������  

#define  EEP_METERCLRCNT  0x01A3  //������������������  

#define  EEP_METERCLFENGVAL  0x01A4  //������������ֵ��������  

#define  EEP_PRODUCE  0x01A8  //����ģʽ״̬(����)  

#define  EEP_PRODUCETM  0x01A9  //����ģʽ�˳�ʱ��(����)  

#define  EEP_DATETIME  0x01AB  //ʱ�ӱ���  

#define  EEP_EPS  0x01B2  //��ǰ�й�����  

#define  EEP_NEPS  0x01B6  //��ǰ�޹�����  

#define  EEP_ZDSBMSZ  0x01BA  //�����ϱ�ģʽ��  

//#define    0x01BC  CRC  

        

        

//      ��������  

//    HEX    

#define  EEP_LXTABLE  0x01C0  //������1  

//#define    0x01C5  ������2  

//#define    0x01CA  ������3  

//#define    0x01CF  ������4  

//#define    0x01D4  ������5  

//#define    0x01D9  ������6  

//#define    0x01DE  ������7  

//#define    0x01E3  ������8  

//#define    0x01E8  ������9  

//#define    0x01ED  ������10  

        

        

        

        

        

//      ��������  

//    HEX    

#define  EEP_JXTABLE  0x0200  //������1  

//#define    0x0205  ������2  

//#define    0x020A  ������3  

//#define    0x020F  ������4  

//#define    0x0214  ������5  

//#define    0x0219  ������6  

//#define    0x021E  ������7  

//#define    0x0223  ������8  

//#define    0x0228  ������9  

//#define    0x022D  ������10  

        

        

        

//      ��������  

//    HEX    

#define  EEP_EPFH_PT  0x0240  //����ָ��  

//#define    0x0242  ��ǰ��������й�����(��)  

//#define    0x0246  ��ǰ��������޹�����(��)  

//#define    0x024A  A���ѹ��Чֵ  

//#define    0x024C  B���ѹ��Чֵ  

//#define    0x024E  C���ѹ��Чֵ  

//#define    0x0250  A�������Чֵ  

//#define    0x0252  B�������Чֵ  

//#define    0x0254  C�������Чֵ  

//#define    0x0256  A���й�����  

//#define    0x0258  B���й�����  

//#define    0x025A  C���й�����  

//#define    0x025C  A���޹�����  

//#define    0x025E  B���޹�����  

//#define    0x0260  C���޹�����  

//#define    0x0262  ʱ��  

//#define    0x0266  36��4��24��6��7�����ݣ�  

//    0x5369    

        

        

//      ��������  

//    HEX    

#define  EEP_ZBCNT_PT 0x53C0  //�ܲ�ָ��  

#define  EEP_ZBUA_PT  0x53C2  //A��ѹԭʼֵ  

#define  EEP_ZBIA_PT  0x5DC2  //A����ԭʼֵ  

#define  EEP_ZBUB_PT  0x67C2  //B��ѹԭʼֵ  

#define  EEP_ZBIB_PT  0x71C2  //B����ԭʼֵ  

#define  EEP_ZBUC_PT  0x7BC2  //C��ѹԭʼֵ  

#define  EEP_ZBIC_PT  0x85C2  //C����ԭʼֵ 

#define  EEP_FFTUA_PT 0x9000  //256����洢��ַ,512�ֽ�
#define  EEP_FFTIA_PT 0x92BC
#define  EEP_FFTUB_PT 0x9578
#define  EEP_FFTIB_PT 0x9834
#define  EEP_FFTUC_PT 0x9AF0
#define  EEP_FFTIC_PT 0x9DAC


#endif
