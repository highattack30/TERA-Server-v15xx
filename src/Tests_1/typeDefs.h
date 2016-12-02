#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#define  WIN32_LEAN_AND_MEAN

#define uint8 unsigned char

#define uint16 unsigned short

#define uint32 unsigned int

#define uint64 unsigned long long

#define int8 char

#define int16 short

#define int32 int

#define int64 long long

#define STRING const char*

#define ulong unsigned long

#define uint unsigned int

#define byte unsigned char

#define entityId uint64

#define wchar uint16

#define NAME_MAX 128

#define slot_id uint16
#define item_eid uint64
#define item_id uint32

#ifndef WINAPI
#define WINAPI __stdcall
#endif


#define STACK_WHOLE 1
#define STACK_FRAGMENT 0

#endif // !TYPEDEFS_H

