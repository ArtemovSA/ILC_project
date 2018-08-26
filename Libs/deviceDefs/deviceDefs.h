
#ifndef DEV_DEF_H
#define DEV_DEF_H

//**********************************Macros**********************************************************
#define HI(x) (x & 0xFF00)>>8
#define LO(x) (x & 0x00FF)
#define HI16(x) (x & 0xFFFF0000)>>16
#define LO16(x) (x & 0x0000FFFF)
#define ADD(x,y) 0x0UL|(x<<8)|(y)
#define ADD16(x,y) 0x0ULL|(x<<16)|(y)
#define max(a,b) ((a) > (b) ? (a) : (b))
#define HIGH_LEVEL 1
#define LOW_LEVEL  0

//Device status type
typedef enum 
{
  DEV_OK       = 0,             //�������� ���������
  DEV_ERROR,                    //����������� ������
  DEV_CRC_ERROR,                //������ ����������� �����
  DEV_BUSY,                     //������ �����
  DEV_TIMEOUT,                  //�������
  DEV_NEXIST,                   //������ �� ����������
  DEV_CON_ERROR,                //������ ����������
  DEV_VAL_ERROR                 //������ ��������
} DEV_Status_t;

//Device info struct
typedef struct{
  uint16_t SW_version;          //Software version
  uint16_t HW_version;          //Hardware version
  uint32_t lastUpdate;          //Timestamp last update
}DEV_info_t;

#endif