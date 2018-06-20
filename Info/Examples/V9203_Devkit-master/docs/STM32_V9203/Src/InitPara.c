#define INIT_EXT
#include"Includes.h"


const uint8   InitPara0[]=
{
    0x11,0x11,0x11,0x11,0x11,0x11,  //ͨѶ��ַ
    0x01,0x00,0x00,0x00,0x00,0x00,  //���

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,    //�ʲ��������

    0,0,'V','0','2','2',    //���ѹ
    0,0,0,0,'A','5',        //�����
    0,0,0,'A','0','6',      //������
};

const uint8   InitPara1[]=
{
    0x00,'0','.','2',               //�й�׼ȷ�ȵȼ�
    0x00,0x00,0x00,0x00,0x00,       //���λ����Ϣ
    0x00,0x00,0x00,0x00,0x00,0x00,           

    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,       //����ͺ�

    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,       //��������


    0x32,0x31,0x2D,0x37,0x30,
    0x30,0x32,0x2D,0x35,0x34,
    0x36,0x20,0x54,0x2F,0x4C,0x44,  //Э��汾��

    0x00,0x00,0x00,0x00,0x00,0x00,  //�ͻ����

    0x05,                           //�й����������
    0x04,                           //���ƺ��� 
    0x08,                           //485-1
    0xf0,0x00,                      //�����Чʱ��(����)
};


const uint8   InitPara3[]=
{
    0x00,0x00,0x00,     //��һ������
    0x00,0x00,0x00,     //�ڶ�������
    0x00,0x00,0x00,     //����������
    0x00,0x00,0x00,     //���ļ�����
    0x11,0x11,0x11,     //���弶����
    0x00,0x00,0x00,     //����������
    0x00,0x00,0x00,     //���߼�����
    0x00,0x00,0x00,     //�ڰ˼�����
    0x00,0x00,0x00,     //�ھż�����
    0x00,0x01,          //��1������
    0x99,0x99,          //��2������
    0x99,0x99,          //��3������
    0x0B,               //�Զ�ѭ����ʾ����
    0x05,               //������ʾʱ��
    0x02,               //��ʾ����С��λ��
    0x04,               //��ʾ����С��λ��
    0x0B,               //����ѭ����ʾ����
    0x1a,               //������ʾ��λ��
    0x1E,               //�е��¼�����ʾʱ��(����)
    0x1E,               //ͣ���¼�����ʾʱ��(����)
    0x00,0x00,          //ͣ����ʾʱ��(����)
    0x05,               //�ϵ�ȫ��ʱ�䣨������
    0x05,               //�ϵ翪����ʱ��(����)
    0x14,               //���ⴥ������ʱ��(����)
    0x3C,               //������������ʱ��(����)
    0x05,               //ESAM��������ʾʱ�䣨������ʱ�䣩(����)
    0x02,               //��բ���Լ�ʱ��(����)
    0x00,0x00,0x01,0x01,0x10, //���㶳����ʼʱ��
    0x3c,                   //���㶳��ʱ����
    0x00,0x00,0x15,0x99,    //��ʱ����      
    0x00,0x00,              //�ն���ʱ��
    0x00,                   //������������֣������ú�����־��  
};


const uint8   InitPara5[sizeof(S_JBPM)-2]=
{
    0xB0,0x04,     //����
    0xf0,0x55,     //��Ƶ�ѹ
    0x88,0x13,     //��Ƶ���
    0x00,0x00,     //����

    0x05,0xFF,0xD3,0x2F,//�й�����ֵ�Ĵ���
    0xe5,0x21,0x02,0x00,//��/Ǳ���ж�����ֵ�Ĵ�����
    0x4e,0xeb,0x01,0x00,//��/Ǳ���ж�����ֵ�Ĵ�����
    0x00,0x00,0xf0,0xff,//�����������ֵ�Ĵ���
    0x82,0x82,0x82,0x00,//�ǲ�Ĵ���

    0xff,0x00,0x00,0x00,//�������ƼĴ���0
    0x00,0x00,0x00,0x00,//�������ƼĴ���1
    0xff,0x80,0x00,0x07,//�������ƼĴ���2

    0x33,0x03,0x00,0x00,//ģ����ƼĴ���0
    0x00,0x00,0x00,0x00,//ģ����ƼĴ���1
    0x00,0x54,0x00,0x77,//ģ����ƼĴ���2
    0x06,0x04,0x0d,0x00,//ģ����ƼĴ���3
   

    0x00,0x00,0x00,0x00,//A��ȫ����ѹ��Чֵ�Ȳ�Ĵ���
    0x00,0x00,0x00,0x00,//A��ȫ��������Чֵ�Ȳ�Ĵ���
    0x00,0x00,0x00,0x00,//A��ȫ���й����ʱȲ�Ĵ���
    0x00,0x00,0x00,0x00,//A��ȫ���й����ʶ��β����Ĵ���
    0x00,0x00,0x00,0x00,//A��ȫ���޹����ʶ��β����Ĵ���

    0x00,0x00,0x00,0x00,//B��ȫ����ѹ��Чֵ�Ȳ�Ĵ���
    0x00,0x00,0x00,0x00,//B��ȫ��������Чֵ�Ȳ�Ĵ���
    0x00,0x00,0x00,0x00,//B��ȫ���й����ʱȲ�Ĵ���
    0x00,0x00,0x00,0x00,//B��ȫ���й����ʶ��β����Ĵ���
    0x00,0x00,0x00,0x00,//B��ȫ���޹����ʶ��β����Ĵ���

    0x00,0x00,0x00,0x00,//C��ȫ����ѹ��Чֵ�Ȳ�Ĵ���
    0x00,0x00,0x00,0x00,//C��ȫ��������Чֵ�Ȳ�Ĵ���
    0x00,0x00,0x00,0x00,//C��ȫ���й����ʱȲ�Ĵ���
    0x00,0x00,0x00,0x00,//C��ȫ���й����ʶ��β����Ĵ���
    0x00,0x00,0x00,0x00,//C��ȫ���޹����ʶ��β����Ĵ���

    0x01,0x00,0x00,0x00,//��ѹ��ʾ����ϵ��k
    0x01,0x00,0x00,0x00,//����I1��ʾ����ϵ��k
    0x01,0x00,0x00,0x00,//������ʾ����ϵ��k
    0x00,0x00,          //����
    //CRC
};

const uint8   InitPara7[]=
{
    0x02,                       //��ʱ����
    0x02,                       //��ʱ�α���
    0x08,                       //��ʱ�Σ�ÿ����ܵ������л���
    0x04,                       //������
    0x7F,                       //������������
    0x01,                       //�����ղ���ʱ�α��
    0x05,0x00,                  //�����֤ʱЧ
    0x00,0x00,                  //����������
    0x00,0x00,0x00,0x00,0x00,   //ʱ�����л�ʱ��
    0x00,0x00,0x00,0x00,0x00,   //ʱ�α��л�ʱ��
    0x03,                       //���ʷ����¼��ж���ʱʱ��
    0x33,0x00,0x00,             //���ʷ����¼��й����ʴ������� 5A 220V 1100W 0.3%=3.3W
    0x00,                       //����    
    0x55,                       //CF���������������
    0x00,                       //�����ղ��������������
    0x00,                       //���Ჹ�������������
    0x00,0x00,                  //����ȼ���Ч�����������
    0x00,                       //ʱ����ʱ���л�ʱ�������жϷ�ʽ��������
    0x02,                       //������ʾ��ʽ���������������
    0x00,                       //ʱ����ʾ��ʽ���������������
    0x00,                       //ͣ����ʾģʽ���������������
    0x00,//�̵�����ⷽʽ���������������
    0x00,//������������������
    0x00,0x00,0x00,0x00,//������������ֵ��������
    0x55,               //����ģʽ״̬(����)
    0xA0,0x05,          //����ģʽ�˳�ʣ��ʱ��(����)
    0x00,0x00,0x18,0x23,0x05,0x12,0x03,//��ǰʱ�ӱ���(����)
    0x00,0x00,0x00,0x00,  //�й�����
    0x00,0x00,0x00,0x00,  //�޹�����
    0x00,0x00,   //�����ϱ�ģʽ��
};

const uint8   InitPara29[]=
{
    0x00,0x00,0x00,0x00,0x00,       //ѭ��1  ��ǰ�ܵ���
    0x00,0x01,0x01,0x02,0x00,       //ѭ��5 A��ѹ
    0x00,0x02,0x01,0x02,0x00,       //ѭ��6 B��ѹ
    0x00,0x03,0x01,0x02,0x00,       //ѭ��7 C��ѹ
    0x00,0x01,0x02,0x02,0x00,       //ѭ��8 A����
    0x00,0x02,0x02,0x02,0x00,       //ѭ��9 B����
    0x00,0x03,0x02,0x02,0x00,       //ѭ��10 C����

    0x00,0x00,0x03,0x02,0x00,       //ѭ��11 ����
    0x00,0x01,0x03,0x02,0x00,       //ѭ��12 A����
    0x00,0x02,0x03,0x02,0x00,       //ѭ��13 B����
    0x00,0x03,0x03,0x02,0x00,       //ѭ��14 c����
};

const uint8   InitPara37[]=
{
    0x00,0x00,0x00,0x00,0x00,       //����1  ��ǰ�ܵ���
    0x00,0x01,0x01,0x02,0x00,       //����13 A��ѹ
    0x00,0x02,0x01,0x02,0x00,       //����14 B��ѹ
    0x00,0x03,0x01,0x02,0x00,       //����15 C��ѹ

    0x00,0x01,0x02,0x02,0x00,       //����16 A����
    0x00,0x02,0x02,0x02,0x00,       //����17 B����
    0x00,0x03,0x02,0x02,0x00,       //����18 C����

    0x00,0x00,0x03,0x02,0x00,       //����19 ����
    0x00,0x01,0x03,0x02,0x00,       //����20 A����
    0x00,0x02,0x03,0x02,0x00,       //����21 B����  
    0x00,0x03,0x03,0x02,0x00,       //ѭ��22 c����
};

typedef struct 
{
    const uint8  * E2ParaTabAdrr;   //E2��������ַ
    uint16 E2Adrr;          //E2��ַ
    uint8  uclen;           //���ݳ���
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
* @����: 
* 
* @����: 
* @����: 
* @����:   lwb (2012-05-04)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void InitE2Data(uint8 uc_RTCSave)
{
    Word16 CRC1;
    uint8 i;
    uint16 ShowLen;
    uint16 Wrlen;

    ClRCRCWord(guc_InitWd);

    BE_ReadP(EEP_COMADD,XDATA_RAMZONE,6);                   //��ȡͨѶ��ַ

    //ͨѶ��ַ�ͱ�ţ�������ʼ��ʱ������
    if(JudgeIsNum(XDATA_RAMZONE,6)==Const_Data_Err)         //ͨѶ��ַ����BCD���ʽ�Ļ�����ʼ��ͨѶ��ַ
    {
        memcpy(XDATA_RAMZONE,InitPara0,12);                //����Flash���ݵ�RAM��
        SysE2ParaSetManage(EEP_COMADD, XDATA_RAMZONE,12);    //дͨѶ��ַ
    }

   // memcpy(XDATA_RAMZONE,PageCRCAddr[0].E2ParaTabAdrr+12,PageCRCAddr[0].uclen-12);            //����Flash���ݵ�RAM��
   // SysE2ParaSetManage(PageCRCAddr[0].E2Adrr+12,XDATA_RAMZONE,PageCRCAddr[0].uclen-12);         //д�붥���¶Ⱥ�Bֵ��E2��

    for(i=1;i<sizeof(PageCRCAddr)/sizeof(GS_E2PARA);i++)
    {
        CLRWDT();               //ι��
        memcpy(XDATA_RAMZONE,PageCRCAddr[i].E2ParaTabAdrr,PageCRCAddr[i].uclen);//����Flash���ݵ�RAM��
        CRC1.word=do_CRC(XDATA_RAMZONE,PageCRCAddr[i].uclen);
        XDATA_RAMZONE[PageCRCAddr[i].uclen]=CRC1.byte[0];
        XDATA_RAMZONE[PageCRCAddr[i].uclen+1]=CRC1.byte[1];   //����CRC��RAM��
        //BE_WriteP(PageCRCAddr[i].E2Adrr,XDATA_RAMZONE,PageCRCAddr[i].uclen+2);   //д�뵽E2��
        SysE2ParaSetManage(PageCRCAddr[i].E2Adrr,XDATA_RAMZONE,PageCRCAddr[i].uclen+2);
     }

    //У����������ж�,�����ж�E2�е�CRC�Ƿ���ȷ�������ȷ������Ҫ��ʼ��
    BE_ReadP(EEP_JBTOTAL,XDATA_RAMZONE,sizeof(S_JBPM));
    if(Uint8_To_Uint16(XDATA_RAMZONE+sizeof(S_JBPM)-2)!=do_CRC(XDATA_RAMZONE,sizeof(S_JBPM)-2))
    {

        CLRWDT();               //ι��
        memcpy(XDATA_RAMZONE,InitPara5,sizeof(S_JBPM)-2);                     //����Flash���ݵ�RAM��
        CRC1.word=do_CRC(XDATA_RAMZONE,sizeof(S_JBPM)-2);
        XDATA_RAMZONE[sizeof(S_JBPM)-2]=CRC1.byte[0];
        XDATA_RAMZONE[sizeof(S_JBPM)-1]=CRC1.byte[1];                            //����CRC��RAM��
        //BE_WriteP(EEP_JBTOTAL,XDATA_RAMZONE,sizeof(S_JBPM));                    //д�뵽E2��
        SysE2ParaSetManage(EEP_JBTOTAL,XDATA_RAMZONE,sizeof(S_JBPM));
    }

    //д�����Բ���

    ShowLen=sizeof(InitPara29);
    for(i=0;i<((sizeof(InitPara29)-1)/64+1);i++)
    {

        CLRWDT();               //ι��
        if(ShowLen>=64)
        {
            memcpy(XDATA_RAMZONE,&InitPara29[i*64],64);    //����Flash���ݵ�RAM��
            Wrlen=64;
            ShowLen-=64;
        }else
        {
            memcpy(XDATA_RAMZONE,&InitPara29[i*64],ShowLen);    //����Flash���ݵ�RAM��
            Wrlen=ShowLen;
        }

        //BE_WriteP(EEP_LXTABLE+i*64,XDATA_RAMZONE,Wrlen);          //д��E2��
        SysE2ParaSetManage(EEP_LXTABLE+i*64,XDATA_RAMZONE,Wrlen);   
    }

    //д����Բ���
    ShowLen=sizeof(InitPara37); 
    for(i=0;i<((sizeof(InitPara37)-1)/64+1);i++)            
    {
        CLRWDT();                                                   //ι��
        if(ShowLen>64)
        {
            memcpy(XDATA_RAMZONE,&InitPara37[i*64],64);           //����Flash���ݵ�RAM��
            Wrlen=64;
            ShowLen-=64;
        }else
        {
            memcpy(XDATA_RAMZONE,&InitPara37[i*64],ShowLen);      //����Flash���ݵ�RAM��
            Wrlen=ShowLen;
        }
        //BE_WriteP(EEP_JXTABLE+i*64,XDATA_RAMZONE,Wrlen);            //д��E2��
        SysE2ParaSetManage(EEP_JXTABLE+i*64,XDATA_RAMZONE,Wrlen);  
    }

    CLRWDT();                                                       //ι��

   // SetCRCWord(guc_ClearWd);
    Data_ClearMeter();                                              //�������
    uint8 addr;
    addr = EEP_EPFH_PT + 2;
    SysE2ParaSetManage(EEP_EPFH_PT,(uint8*)&addr,2);
    memset(XDATA_RAMZONE,0x00,64);                                              //�������ⲿRAM 
    /*for(j=0;j<gCs_MeterClearAddrTab[0].ucPageCnt;j++)                           //������ʼ����ʱ�������������¼
    {
        CLRWDT();               //ι��
        BE_WriteP(gCs_MeterClearAddrTab[0].uiE2Addr+64*j,XDATA_RAMZONE,64);    //��������¼
    }*/
//
    //gs_FeeData.uc_FeeRSDN=0x01;
    //gs_FeeData.uc_FeeSQN=0x01;
    gs_MeterState.ui_MeterState3.RunCode3Word.RateTable=false;                  //�޸ĵ��״̬�֣�ʹ�õ�һ��ʱ��
    gs_MeterState.ui_MeterState3.RunCode3Word.TimeZoneTable=false;              //�޸ĵ��״̬�֣�ʹ�õ�һ��ʱ��
    
   // memset((uint8*)&gs_ReportState,0x00,sizeof(REPORTSTATE));                   //�����ϱ�״̬����
   // memset((uint8*)&gs_ReportStateFlg,0x00,sizeof(REPORTSTATE));                //�����ϱ�״̬��־����
  
    guc_FactoryType=FactorMd;
    gui_DyProDmOut=0x2760;      //7��
    //BE_WriteP(EEP_PRODUCELEFTTM,(uint8*)&gui_DyProDmOut,0x02);                  //����ʣ��ʱ��
    SysE2ParaSetManage(EEP_PRODUCE,(uint8*)&guc_FactoryType,1);                 //д�볧��״̬

    SleepTimeProc();                                            //��RTC��ȡʱ�ӵ�RAM��
    gui_RefreshEvent|=(flgEtPara_Config+flgEtPara_Fee+flgEtPara_Show+flgEtPara_LoadCurve+flgEtPara_PowDir);
}
