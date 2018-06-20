/*
*版本控制
*每添加一个版本请注明当前版本与上一版本不同之处,在哪几个源文件中有变动,具体指明
*
*/
#ifndef __VERSION_H__
#define __VERSION_H__


//表计类型
#define CONST_A_METER   0               //三相液晶表
#define CONST_B_METER   1               //单相表
#define CONST_C_METER   2


#define __T26_S01__                     //单相多费率表,浙江需求

/**表计选择和版本控制
版本定义
*/

#if defined(__T26_S01__)
    #define __METER_TYPE__ CONST_B_METER
#endif

#if (__METER_TYPE__ != CONST_B_METER)
    #error "VersionExt.h: system not define B meter type!!!"
#endif

#define __VER_SOFT_B__  "S309VG01.01"      //当前软件版本
#define __VER_HARD_B__  0x01            //当前硬件版本

extern const __root uint8  gCuc_VerSoft[];
extern const __root uint8  gCuc_VerHard[];


#endif

