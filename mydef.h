//--------------------------------------------------------------------------
// Copyright 2005-2010 SCUD Electronic Co.,Ltd
// 
// 文件名:  mydef.h
//
// 描述:    这个文件为MPLAB C30 的数据类型定义
// 
// 历史:    时间        修改人            内容
//        2010/06/09   Denny Yang       
//                   
//--------------------------------------------------------------------------

#ifndef MYDEF_H
#define MYDEF_H


//--------------------------------------
// 包含文件
//--------------------------------------

//--------------------------------------
// API 和类型定义
//--------------------------------------
#ifndef NULL
#define NULL                          0
#endif

#define EEDATA_INIT_FLAG              0
#define EEDATA_RAW_FLAG               1

#define _BV(bit)                      (1 << (bit))

typedef enum _BOOL { FALSE_MAY = 0, TRUE_MAY } BOOL;    /* Undefined size */
typedef enum _BIT { CLEAR = 0, SET_MAY }  BIT;

typedef unsigned char                UINT8;
typedef signed char                   INT8;
typedef char                          CHAR;

typedef unsigned int                 UINT16;
typedef int                           INT16;

typedef unsigned long                UINT32;
typedef long                          INT32;

//typedef unsigned long long           UINT64;
//typedef long long                     INT64;


typedef float                         FLOAT32;
typedef long double                   FLOAT64;         


#define MIN(x, y)                      (((x) < (y))? (x):(y))
#define MAX(x, y)                      (((x) > (y))? (x):(y))
#define ABSSUB(x, y)  	                (MAX(x, y) - MIN(x, y))


#define SLEEP()         Sleep()
#define IDLE()          Idle()
#define CLRWDT()        ClrWdt()
#define NOP()           Nop()
#define ENABLEWDT();    {WDTCONbits.SWDTEN = 1;}
#define DISABLEWDT();   {WDTCONbits.SWDTEN = 0;}
#define RESET();        Reset();

typedef union _UNION8
{
    UINT8 U8;
    
    struct
    {
        unsigned bit0 : 1;
        unsigned bit1 : 1;
        unsigned bit2 : 1;
        unsigned bit3 : 1;
        unsigned bit4 : 1;
        unsigned bit5 : 1;
        unsigned bit6 : 1;
        unsigned bit7 : 1;
    };
}UNION8;

// A union type for byte or word access for 16 bit values.
typedef union _UNION16
{
    UINT16 U16;
    UINT8  U8[2];
    struct
    {
        unsigned bit0  : 1;
        unsigned bit1  : 1;
        unsigned bit2  : 1;
        unsigned bit3  : 1;
        unsigned bit4  : 1;
        unsigned bit5  : 1;
        unsigned bit6  : 1;
        unsigned bit7  : 1;
        unsigned bit8  : 1;
        unsigned bit9  : 1;
        unsigned bit10 : 1;
        unsigned bit11 : 1;
        unsigned bit12 : 1;
        unsigned bit13 : 1;    
        unsigned bit14 : 1;
        unsigned bit15 : 1;
    };
} UNION16;

// A union type for byte, word, or dword access for 32 bit values.
typedef union _UNION32 
{
    UINT32 U32;

    struct 
    {
        UINT16 LS16;
        UINT16 MS16;
    };

    UINT8 U8[4];
}UNION32;

// A union type for byte, word, or dword access for 64 bit values.
#if 0
typedef union _UNION64 
{
    UINT64 U64;
    

    UINT32 U32[2];
    UINT16 U16[4];
    UINT8  U8[8];
    
}UNION64;
     
#endif // MYDEF_H
#endif


