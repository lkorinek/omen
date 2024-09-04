#include <omen/managers/dev/fb.h>

#define TABSIZE 8

uint32_t* vmem = 0;
size_t width = 0;
size_t height = 0;

/*
font array taken from here, take care of the copyright if any applicable.
https://sourceforge.net/p/dosbox/code-0/HEAD/tree/dosbox/trunk/src/ints/int10_memory.cpp
*/

uint8_t font[256 * 16] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7e, 0x81, 0xa5, 0x81, 0x81, 0xbd,
	0x99, 0x81, 0x81, 0x7e, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7e, 0xff, 0xdb, 0xff, 0xff, 0xc3,
	0xe7, 0xff, 0xff, 0x7e, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x6c, 0xfe, 0xfe, 0xfe,
	0xfe, 0x7c, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x7c, 0xfe,
	0x7c, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x18, 0x3c, 0x3c, 0xe7, 0xe7,
	0xe7, 0x18, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x18, 0x3c, 0x7e, 0xff, 0xff,
	0x7e, 0x18, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3c,
	0x3c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xc3,
	0xc3, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x66, 0x42,
	0x42, 0x66, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0x99, 0xbd,
	0xbd, 0x99, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x1e, 0x0e, 0x1a, 0x32, 0x78, 0xcc,
	0xcc, 0xcc, 0xcc, 0x78, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3c, 0x66, 0x66, 0x66, 0x66, 0x3c,
	0x18, 0x7e, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3f, 0x33, 0x3f, 0x30, 0x30, 0x30,
	0x30, 0x70, 0xf0, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7f, 0x63, 0x7f, 0x63, 0x63, 0x63,
	0x63, 0x67, 0xe7, 0xe6, 0xc0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x18, 0x18, 0xdb, 0x3c, 0xe7,
	0x3c, 0xdb, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfe, 0xf8,
	0xf0, 0xe0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x06, 0x0e, 0x1e, 0x3e, 0xfe, 0x3e,
	0x1e, 0x0e, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x3c, 0x7e, 0x18, 0x18, 0x18,
	0x7e, 0x3c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
	0x66, 0x00, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7f, 0xdb, 0xdb, 0xdb, 0x7b, 0x1b,
	0x1b, 0x1b, 0x1b, 0x1b, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x7c, 0xc6, 0x60, 0x38, 0x6c, 0xc6, 0xc6,
	0x6c, 0x38, 0x0c, 0xc6, 0x7c, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xfe, 0xfe, 0xfe, 0xfe, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x3c, 0x7e, 0x18, 0x18, 0x18,
	0x7e, 0x3c, 0x18, 0x7e, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x3c, 0x7e, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x7e, 0x3c, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x0c, 0xfe,
	0x0c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x60, 0xfe,
	0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0,
	0xc0, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x6c, 0xfe,
	0x6c, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x38, 0x7c,
	0x7c, 0xfe, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xfe, 0xfe, 0x7c, 0x7c,
	0x38, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x3c, 0x3c, 0x3c, 0x18, 0x18,
	0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x66, 0x66, 0x66, 0x24, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x6c, 0x6c, 0xfe, 0x6c, 0x6c,
	0x6c, 0xfe, 0x6c, 0x6c, 0x00, 0x00, 0x00, 0x00,
	0x18, 0x18, 0x7c, 0xc6, 0xc2, 0xc0, 0x7c, 0x06,
	0x06, 0x86, 0xc6, 0x7c, 0x18, 0x18, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xc2, 0xc6, 0x0c, 0x18,
	0x30, 0x60, 0xc6, 0x86, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x38, 0x6c, 0x6c, 0x38, 0x76, 0xdc,
	0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x30, 0x30, 0x30, 0x60, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x18, 0x0c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0c, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x3c, 0xff,
	0x3c, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7e,
	0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x18, 0x18, 0x18, 0x30, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x0c, 0x18,
	0x30, 0x60, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x38, 0x6c, 0xc6, 0xc6, 0xd6, 0xd6,
	0xc6, 0xc6, 0x6c, 0x38, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x38, 0x78, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x7e, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7c, 0xc6, 0x06, 0x0c, 0x18, 0x30,
	0x60, 0xc0, 0xc6, 0xfe, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7c, 0xc6, 0x06, 0x06, 0x3c, 0x06,
	0x06, 0x06, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x0c, 0x1c, 0x3c, 0x6c, 0xcc, 0xfe,
	0x0c, 0x0c, 0x0c, 0x1e, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xfe, 0xc0, 0xc0, 0xc0, 0xfc, 0x06,
	0x06, 0x06, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x38, 0x60, 0xc0, 0xc0, 0xfc, 0xc6,
	0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xfe, 0xc6, 0x06, 0x06, 0x0c, 0x18,
	0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0xc6,
	0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7e, 0x06,
	0x06, 0x06, 0x0c, 0x78, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
	0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
	0x00, 0x18, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x06, 0x0c, 0x18, 0x30, 0x60,
	0x30, 0x18, 0x0c, 0x06, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00,
	0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06,
	0x0c, 0x18, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7c, 0xc6, 0xc6, 0x0c, 0x18, 0x18,
	0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xde, 0xde,
	0xde, 0xdc, 0xc0, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x10, 0x38, 0x6c, 0xc6, 0xc6, 0xfe,
	0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xfc, 0x66, 0x66, 0x66, 0x7c, 0x66,
	0x66, 0x66, 0x66, 0xfc, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3c, 0x66, 0xc2, 0xc0, 0xc0, 0xc0,
	0xc0, 0xc2, 0x66, 0x3c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf8, 0x6c, 0x66, 0x66, 0x66, 0x66,
	0x66, 0x66, 0x6c, 0xf8, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xfe, 0x66, 0x62, 0x68, 0x78, 0x68,
	0x60, 0x62, 0x66, 0xfe, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xfe, 0x66, 0x62, 0x68, 0x78, 0x68,
	0x60, 0x60, 0x60, 0xf0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3c, 0x66, 0xc2, 0xc0, 0xc0, 0xde,
	0xc6, 0xc6, 0x66, 0x3a, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xfe, 0xc6,
	0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1e, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0xcc, 0xcc, 0xcc, 0x78, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xe6, 0x66, 0x66, 0x6c, 0x78, 0x78,
	0x6c, 0x66, 0x66, 0xe6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf0, 0x60, 0x60, 0x60, 0x60, 0x60,
	0x60, 0x62, 0x66, 0xfe, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xc6, 0xee, 0xfe, 0xfe, 0xd6, 0xc6,
	0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xc6, 0xe6, 0xf6, 0xfe, 0xde, 0xce,
	0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6,
	0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xfc, 0x66, 0x66, 0x66, 0x7c, 0x60,
	0x60, 0x60, 0x60, 0xf0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6,
	0xc6, 0xd6, 0xde, 0x7c, 0x0c, 0x0e, 0x00, 0x00,
	0x00, 0x00, 0xfc, 0x66, 0x66, 0x66, 0x7c, 0x6c,
	0x66, 0x66, 0x66, 0xe6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7c, 0xc6, 0xc6, 0x60, 0x38, 0x0c,
	0x06, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7e, 0x7e, 0x5a, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6,
	0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6,
	0xc6, 0x6c, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xd6, 0xd6,
	0xd6, 0xfe, 0xee, 0x6c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xc6, 0xc6, 0x6c, 0x7c, 0x38, 0x38,
	0x7c, 0x6c, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x18,
	0x18, 0x18, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xfe, 0xc6, 0x86, 0x0c, 0x18, 0x30,
	0x60, 0xc2, 0xc6, 0xfe, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x3c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0x70, 0x38,
	0x1c, 0x0e, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0c, 0x0c, 0x3c, 0x00, 0x00, 0x00, 0x00,
	0x10, 0x38, 0x6c, 0xc6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00,
	0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0c, 0x7c,
	0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xe0, 0x60, 0x60, 0x78, 0x6c, 0x66,
	0x66, 0x66, 0x66, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc0,
	0xc0, 0xc0, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1c, 0x0c, 0x0c, 0x3c, 0x6c, 0xcc,
	0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xfe,
	0xc0, 0xc0, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x38, 0x6c, 0x64, 0x60, 0xf0, 0x60,
	0x60, 0x60, 0x60, 0xf0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xcc, 0xcc,
	0xcc, 0xcc, 0xcc, 0x7c, 0x0c, 0xcc, 0x78, 0x00,
	0x00, 0x00, 0xe0, 0x60, 0x60, 0x6c, 0x76, 0x66,
	0x66, 0x66, 0x66, 0xe6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x18, 0x00, 0x38, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x06, 0x06, 0x00, 0x0e, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3c, 0x00,
	0x00, 0x00, 0xe0, 0x60, 0x60, 0x66, 0x6c, 0x78,
	0x78, 0x6c, 0x66, 0xe6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xec, 0xfe, 0xd6,
	0xd6, 0xd6, 0xd6, 0xc6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x66, 0x66,
	0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6,
	0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x66, 0x66,
	0x66, 0x66, 0x66, 0x7c, 0x60, 0x60, 0xf0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xcc, 0xcc,
	0xcc, 0xcc, 0xcc, 0x7c, 0x0c, 0x0c, 0x1e, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x76, 0x66,
	0x60, 0x60, 0x60, 0xf0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0x60,
	0x38, 0x0c, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x10, 0x30, 0x30, 0xfc, 0x30, 0x30,
	0x30, 0x30, 0x36, 0x1c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0xcc, 0xcc,
	0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66,
	0x66, 0x66, 0x3c, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xd6,
	0xd6, 0xd6, 0xfe, 0x6c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0x6c, 0x38,
	0x38, 0x38, 0x6c, 0xc6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6,
	0xc6, 0xc6, 0xc6, 0x7e, 0x06, 0x0c, 0xf8, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xcc, 0x18,
	0x30, 0x60, 0xc6, 0xfe, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x0e, 0x18, 0x18, 0x18, 0x70, 0x18,
	0x18, 0x18, 0x18, 0x0e, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x70, 0x18, 0x18, 0x18, 0x0e, 0x18,
	0x18, 0x18, 0x18, 0x70, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x76, 0xdc, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x6c, 0xc6,
	0xc6, 0xc6, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3c, 0x66, 0xc2, 0xc0, 0xc0, 0xc0,
	0xc2, 0x66, 0x3c, 0x0c, 0x06, 0x7c, 0x00, 0x00,
	0x00, 0x00, 0xcc, 0x00, 0x00, 0xcc, 0xcc, 0xcc,
	0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x0c, 0x18, 0x30, 0x00, 0x7c, 0xc6, 0xfe,
	0xc0, 0xc0, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x10, 0x38, 0x6c, 0x00, 0x78, 0x0c, 0x7c,
	0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xcc, 0x00, 0x00, 0x78, 0x0c, 0x7c,
	0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x60, 0x30, 0x18, 0x00, 0x78, 0x0c, 0x7c,
	0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x38, 0x6c, 0x38, 0x00, 0x78, 0x0c, 0x7c,
	0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x3c, 0x66, 0x60, 0x60,
	0x66, 0x3c, 0x0c, 0x06, 0x3c, 0x00, 0x00, 0x00,
	0x00, 0x10, 0x38, 0x6c, 0x00, 0x7c, 0xc6, 0xfe,
	0xc0, 0xc0, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xc6, 0x00, 0x00, 0x7c, 0xc6, 0xfe,
	0xc0, 0xc0, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x60, 0x30, 0x18, 0x00, 0x7c, 0xc6, 0xfe,
	0xc0, 0xc0, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x66, 0x00, 0x00, 0x38, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x18, 0x3c, 0x66, 0x00, 0x38, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x60, 0x30, 0x18, 0x00, 0x38, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xc6, 0x00, 0x10, 0x38, 0x6c, 0xc6, 0xc6,
	0xfe, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00,
	0x38, 0x6c, 0x38, 0x00, 0x38, 0x6c, 0xc6, 0xc6,
	0xfe, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00,
	0x18, 0x30, 0x60, 0x00, 0xfe, 0x66, 0x60, 0x7c,
	0x60, 0x60, 0x66, 0xfe, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x76, 0x36,
	0x7e, 0xd8, 0xd8, 0x6e, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3e, 0x6c, 0xcc, 0xcc, 0xfe, 0xcc,
	0xcc, 0xcc, 0xcc, 0xce, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x10, 0x38, 0x6c, 0x00, 0x7c, 0xc6, 0xc6,
	0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xc6, 0x00, 0x00, 0x7c, 0xc6, 0xc6,
	0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x60, 0x30, 0x18, 0x00, 0x7c, 0xc6, 0xc6,
	0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x30, 0x78, 0xcc, 0x00, 0xcc, 0xcc, 0xcc,
	0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x60, 0x30, 0x18, 0x00, 0xcc, 0xcc, 0xcc,
	0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xc6, 0x00, 0x00, 0xc6, 0xc6, 0xc6,
	0xc6, 0xc6, 0xc6, 0x7e, 0x06, 0x0c, 0x78, 0x00,
	0x00, 0xc6, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6,
	0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xc6, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6,
	0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x18, 0x18, 0x3c, 0x66, 0x60, 0x60, 0x60,
	0x66, 0x3c, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x38, 0x6c, 0x64, 0x60, 0xf0, 0x60, 0x60,
	0x60, 0x60, 0xe6, 0xfc, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x66, 0x66, 0x3c, 0x18, 0x7e, 0x18,
	0x7e, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xf8, 0xcc, 0xcc, 0xf8, 0xc4, 0xcc, 0xde,
	0xcc, 0xcc, 0xcc, 0xc6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x0e, 0x1b, 0x18, 0x18, 0x18, 0x7e, 0x18,
	0x18, 0x18, 0x18, 0x18, 0xd8, 0x70, 0x00, 0x00,
	0x00, 0x18, 0x30, 0x60, 0x00, 0x78, 0x0c, 0x7c,
	0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x0c, 0x18, 0x30, 0x00, 0x38, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x18, 0x30, 0x60, 0x00, 0x7c, 0xc6, 0xc6,
	0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x18, 0x30, 0x60, 0x00, 0xcc, 0xcc, 0xcc,
	0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x76, 0xdc, 0x00, 0xdc, 0x66, 0x66,
	0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00,
	0x76, 0xdc, 0x00, 0xc6, 0xe6, 0xf6, 0xfe, 0xde,
	0xce, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x3c, 0x6c, 0x6c, 0x3e, 0x00, 0x7e, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x38, 0x6c, 0x6c, 0x38, 0x00, 0x7c, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x30, 0x30, 0x00, 0x30, 0x30, 0x60,
	0xc0, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xc0,
	0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x06,
	0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xc0, 0xc0, 0xc2, 0xc6, 0xcc, 0x18, 0x30,
	0x60, 0xdc, 0x86, 0x0c, 0x18, 0x3e, 0x00, 0x00,
	0x00, 0xc0, 0xc0, 0xc2, 0xc6, 0xcc, 0x18, 0x30,
	0x66, 0xce, 0x9e, 0x3e, 0x06, 0x06, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18,
	0x3c, 0x3c, 0x3c, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x6c, 0xd8,
	0x6c, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x6c, 0x36,
	0x6c, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44,
	0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44,
	0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa,
	0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa,
	0xdd, 0x77, 0xdd, 0x77, 0xdd, 0x77, 0xdd, 0x77,
	0xdd, 0x77, 0xdd, 0x77, 0xdd, 0x77, 0xdd, 0x77,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xf8,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0xf8, 0x18, 0xf8,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0xf6,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x18, 0xf8,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x36, 0x36, 0x36, 0x36, 0x36, 0xf6, 0x06, 0xf6,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x06, 0xf6,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x36, 0x36, 0x36, 0xf6, 0x06, 0xfe,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0xfe,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x18, 0x18, 0x18, 0x18, 0x18, 0xf8, 0x18, 0xf8,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1f,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1f,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x1f, 0x18, 0x1f,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x37,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x37, 0x30, 0x3f,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x30, 0x37,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x36, 0x36, 0x36, 0xf7, 0x00, 0xff,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0xf7,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x37, 0x30, 0x37,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x36, 0x36, 0x36, 0x36, 0x36, 0xf7, 0x00, 0xf7,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x18, 0x18, 0x18, 0x18, 0x18, 0xff, 0x00, 0xff,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0xff,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x3f,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x1f, 0x18, 0x1f,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x18, 0x1f,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0xff,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x18, 0x18, 0x18, 0x18, 0x18, 0xff, 0x18, 0xff,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xf8,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0,
	0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xdc, 0xd8,
	0xd8, 0xd8, 0xdc, 0x76, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x78, 0xcc, 0xcc, 0xcc, 0xd8, 0xcc,
	0xc6, 0xc6, 0xc6, 0xcc, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xfe, 0xc6, 0xc6, 0xc0, 0xc0, 0xc0,
	0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xfe, 0x6c, 0x6c, 0x6c,
	0x6c, 0x6c, 0x6c, 0x6c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xfe, 0xc6, 0x60, 0x30, 0x18,
	0x30, 0x60, 0xc6, 0xfe, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0xd8, 0xd8,
	0xd8, 0xd8, 0xd8, 0x70, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66,
	0x66, 0x7c, 0x60, 0x60, 0xc0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x76, 0xdc, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7e, 0x18, 0x3c, 0x66, 0x66,
	0x66, 0x3c, 0x18, 0x7e, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x38, 0x6c, 0xc6, 0xc6, 0xfe,
	0xc6, 0xc6, 0x6c, 0x38, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x38, 0x6c, 0xc6, 0xc6, 0xc6, 0x6c,
	0x6c, 0x6c, 0x6c, 0xee, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1e, 0x30, 0x18, 0x0c, 0x3e, 0x66,
	0x66, 0x66, 0x66, 0x3c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0xdb, 0xdb,
	0xdb, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x03, 0x06, 0x7e, 0xdb, 0xdb,
	0xf3, 0x7e, 0x60, 0xc0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1c, 0x30, 0x60, 0x60, 0x7c, 0x60,
	0x60, 0x60, 0x30, 0x1c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6,
	0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0xfe,
	0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7e, 0x18,
	0x18, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x30, 0x18, 0x0c, 0x06, 0x0c,
	0x18, 0x30, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0c, 0x18, 0x30, 0x60, 0x30,
	0x18, 0x0c, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x0e, 0x1b, 0x1b, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0xd8, 0xd8, 0xd8, 0x70, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x7e,
	0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xdc, 0x00,
	0x76, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x38, 0x6c, 0x6c, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
	0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x0f, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xec,
	0x6c, 0x6c, 0x3c, 0x1c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xd8, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x70, 0xd8, 0x30, 0x60, 0xc8, 0xf8, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x7c, 0x7c, 0x7c, 0x7c,
	0x7c, 0x7c, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint64_t posx = 0, posy = 0;
uint32_t fc = 0, bc = 0xffffffff;

void clearscreen(uint32_t col) {

	//TODO this code assumes bpp of 32 ARGB, but the bpp and format may be different
	for(uint64_t i = 0; i < width * height; i++) {
		vmem[i] = col;
	}

}

void scroll() {

	uint64_t y0 = height - 16 - (height % 16);
	uint64_t n = (width * height - width * 16) * 4;
	memmove(vmem, vmem + width * 16, n);
	for(uint64_t y = 0; y < 16 + (height % 16); y++) {
		for(uint64_t x = 0; x < width; x++) {
			putpixel(x, y0 + y, bc);
		}
	}
}

void putpixel(uint64_t x, uint64_t y, uint32_t col) {

	//TODO same as clearscreen issue in comment.
	vmem[x + y * width] = col;

}

void putchar(char ch) {
	
	if(ch == '\n') {
		posx = 0;
		if(posy >= ((height - (height % 16)) / 16) - 1) {
			scroll();
		} else {
			posy++;
		}
		return;
	}
	if(ch == '\t') {
		for(uint64_t i = 0; i < TABSIZE; i++) {
			putchar(' ');
		}
		return;
	}

	uint64_t x0 = posx * 8;
	uint64_t y0 = posy * 16;

	for(uint64_t y = 0; y < 16; y++) {
		for(uint64_t x = 0; x < 8; x++) {
			if((font[ch * 16 + y]) >> (7 - x) & 1) {
				putpixel(x0 + x, y0 + y, fc);
			} else {
				putpixel(x0 + x, y0 + y, bc);
			}
		}
	}
	
	if(posx >= ((width - (width % 8)) / 8) - 1) {
		posx = 0;
		if(posy >= ((height - (height % 16)) / 16) - 1) {
			scroll();
		} else {
			posy++;
		}
	} else {
		posx++;
	}
}

void init_framebuffer() {

	boot_framebuffer_t** fb = get_framebuffers();

	vmem = fb[0]->address;
	width = fb[0]->width;
	height = fb[0]->height;

}