/*
*�汾����
*ÿ���һ���汾��ע����ǰ�汾����һ�汾��֮ͬ��,���ļ���Դ�ļ����б䶯,����ָ��
*
*/
#ifndef __VERSION_H__
#define __VERSION_H__


//�������
#define CONST_A_METER   0               //����Һ����
#define CONST_B_METER   1               //�����
#define CONST_C_METER   2


#define __T26_S01__                     //�������ʱ�,�㽭����

/**���ѡ��Ͱ汾����
�汾����
*/

#if defined(__T26_S01__)
    #define __METER_TYPE__ CONST_B_METER
#endif

#if (__METER_TYPE__ != CONST_B_METER)
    #error "VersionExt.h: system not define B meter type!!!"
#endif

#define __VER_SOFT_B__  "S309VG01.01"      //��ǰ����汾
#define __VER_HARD_B__  0x01            //��ǰӲ���汾

extern const __root uint8  gCuc_VerSoft[];
extern const __root uint8  gCuc_VerHard[];


#endif

