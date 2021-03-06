#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include "grlib/grlib.h"
#include "inimigos.h"
unsigned char barco[] =
{
    IMAGE_FMT_4BPP_COMP,
    28, 0,
    9, 0,

    3,
    0x00, 0x00, 0x00,
    0x2f, 0x0f, 0xef,
    0x04, 0xe7, 0xfb,
    0xff, 0xff, 0xff,

    0xc3, 0x07, 0x07, 0x00, 0x00, 0x33, 0x33, 0x97, 0x97, 0x84, 0x36, 0x33,
    0x33, 0x33, 0x30, 0x14, 0x01, 0x11, 0x26, 0x11, 0x11, 0xe9, 0x11, 0x11,
    0xa7, 0x7d, 0x10, 0x0c, 0x02, 0x22, 0x22, 0x22, 0xe9, 0xd4, 0x00, 0x00,
    0xdc, 0x9f, 0x9a, 0x00, 0xe9, 0xd4, 0xa2,
};
	
unsigned char helicoptero[] =
	{
    IMAGE_FMT_4BPP_UNCOMP,
    11, 0,
    6, 0,

    11,
    0x00, 0x00, 0x00,
    0x40, 0x20, 0x20,
    0x40, 0x40, 0x40,
    0x00, 0x60, 0x80,
    0x80, 0x80, 0x80,
    0xa4, 0xa0, 0xa0,
    0x00, 0xc0, 0xe0,
    0xc0, 0xc0, 0xc0,
    0x40, 0xe0, 0xe0,
    0x80, 0xe0, 0xe0,
    0xf0, 0xfb, 0xff,
    0xff, 0xff, 0xff,

    0xbb, 0xbb, 0xaa, 0xa8, 0x88, 0x90, 0xbb, 0xbb, 0x99, 0x36, 0x7b, 0xb0,
    0xa0, 0x7b, 0x72, 0x00, 0x00, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x40,
    0xb2, 0xab, 0xba, 0x20, 0x1a, 0xb0, 0xbb, 0xbb, 0xb5, 0x54, 0x54, 0xb0,
};
unsigned char ponte[] =
	{
    IMAGE_FMT_8BPP_COMP,
    24, 0,
    19, 0,

    171,
    0xff, 0x00, 0x00,
    0x04, 0x2e, 0x6f,
    0x04, 0x2e, 0x71,
    0x03, 0x2d, 0x74,
    0x4c, 0x2f, 0x56,
    0x4e, 0x2f, 0x56,
    0x66, 0x2d, 0x53,
    0x64, 0x2e, 0x53,
    0x37, 0x2e, 0x67,
    0x38, 0x2f, 0x67,
    0x64, 0x30, 0x55,
    0x64, 0x2f, 0x57,
    0x36, 0x2f, 0x68,
    0x3c, 0x2d, 0x6a,
    0x3d, 0x2d, 0x6a,
    0x0a, 0x2f, 0x79,
    0x36, 0x30, 0x67,
    0x3f, 0x2d, 0x6a,
    0x0c, 0x2f, 0x79,
    0x59, 0x2f, 0x5e,
    0x57, 0x36, 0x51,
    0x65, 0x31, 0x56,
    0x5d, 0x32, 0x57,
    0x65, 0x30, 0x58,
    0x40, 0x34, 0x5e,
    0x5b, 0x2f, 0x5e,
    0x1d, 0x35, 0x69,
    0x59, 0x33, 0x57,
    0x59, 0x31, 0x5b,
    0x20, 0x2f, 0x74,
    0x5d, 0x2f, 0x5e,
    0x40, 0x35, 0x5d,
    0x1d, 0x32, 0x70,
    0x09, 0x65, 0x66,
    0x09, 0x64, 0x68,
    0x04, 0x68, 0x63,
    0x02, 0x69, 0x62,
    0x0a, 0x65, 0x6c,
    0x08, 0x66, 0x6b,
    0x0b, 0x65, 0x6c,
    0x06, 0x67, 0x6a,
    0x04, 0x69, 0x67,
    0x0a, 0x66, 0x6b,
    0x06, 0x69, 0x67,
    0x04, 0x68, 0x6a,
    0x00, 0x6c, 0x65,
    0x00, 0x6b, 0x67,
    0x0e, 0x69, 0x66,
    0x0e, 0x68, 0x68,
    0x0b, 0x69, 0x68,
    0x04, 0x69, 0x6b,
    0x02, 0x66, 0x72,
    0x0b, 0x6b, 0x65,
    0x04, 0x66, 0x72,
    0x13, 0x68, 0x6a,
    0x11, 0x69, 0x69,
    0x00, 0x6f, 0x65,
    0x09, 0x6a, 0x6d,
    0x07, 0x6b, 0x6d,
    0x0e, 0x6b, 0x6c,
    0x0a, 0x6b, 0x6e,
    0x0e, 0x6b, 0x6e,
    0x0b, 0x6c, 0x6f,
    0x07, 0x6d, 0x72,
    0x05, 0x6d, 0x74,
    0x01, 0x6f, 0x73,
    0x00, 0x70, 0x72,
    0x03, 0x6f, 0x73,
    0x01, 0x70, 0x72,
    0x00, 0x70, 0x73,
    0x03, 0x70, 0x72,
    0x0c, 0x6e, 0x74,
    0x00, 0x71, 0x73,
    0x13, 0x6e, 0x72,
    0x10, 0x6f, 0x72,
    0x00, 0x71, 0x74,
    0x0c, 0x6e, 0x76,
    0x66, 0x6b, 0x69,
    0x67, 0x6b, 0x6c,
    0x67, 0x6a, 0x6e,
    0x67, 0x6c, 0x6d,
    0x67, 0x6d, 0x6c,
    0x6a, 0x6e, 0x69,
    0x18, 0x81, 0x7e,
    0x1a, 0x81, 0x7e,
    0x19, 0x81, 0x80,
    0x14, 0x82, 0x80,
    0x1b, 0x80, 0x82,
    0x17, 0x82, 0x80,
    0x1b, 0x83, 0x82,
    0x1b, 0x82, 0x84,
    0x1d, 0x83, 0x82,
    0x18, 0x83, 0x86,
    0x18, 0x7f, 0x8e,
    0x23, 0x7f, 0x8a,
    0x0f, 0x87, 0x82,
    0x1f, 0x80, 0x8a,
    0x11, 0x88, 0x80,
    0x21, 0x80, 0x8a,
    0x1c, 0x80, 0x8c,
    0x1a, 0x82, 0x89,
    0x12, 0x87, 0x84,
    0x16, 0x87, 0x83,
    0x19, 0x86, 0x84,
    0x1f, 0x80, 0x8e,
    0x10, 0x88, 0x84,
    0x16, 0x87, 0x84,
    0x24, 0x80, 0x8d,
    0x23, 0x7f, 0x90,
    0x1c, 0x84, 0x89,
    0x17, 0x85, 0x89,
    0x20, 0x84, 0x88,
    0x13, 0x8a, 0x81,
    0x1e, 0x83, 0x8b,
    0x1c, 0x85, 0x88,
    0x1a, 0x86, 0x87,
    0x27, 0x7f, 0x90,
    0x15, 0x86, 0x89,
    0x20, 0x84, 0x89,
    0x20, 0x83, 0x8b,
    0x11, 0x8b, 0x81,
    0x19, 0x86, 0x88,
    0x1c, 0x86, 0x87,
    0x1c, 0x84, 0x8b,
    0x19, 0x87, 0x87,
    0x21, 0x83, 0x8d,
    0x24, 0x81, 0x90,
    0x1a, 0x83, 0x90,
    0x1d, 0x84, 0x8d,
    0x28, 0x80, 0x91,
    0x26, 0x83, 0x8c,
    0x24, 0x83, 0x8d,
    0x0f, 0x8c, 0x83,
    0x22, 0x84, 0x8c,
    0x1a, 0x83, 0x91,
    0x28, 0x83, 0x8c,
    0x26, 0x82, 0x8f,
    0x21, 0x83, 0x8f,
    0x20, 0x88, 0x87,
    0x1e, 0x87, 0x8a,
    0x1c, 0x88, 0x89,
    0x22, 0x87, 0x89,
    0x19, 0x89, 0x89,
    0x1b, 0x89, 0x89,
    0x18, 0x8a, 0x8a,
    0x17, 0x8a, 0x8d,
    0x17, 0x89, 0x8f,
    0xa9, 0xa8, 0xaa,
    0xa7, 0xa9, 0xaa,
    0xab, 0xaa, 0xac,
    0xa9, 0xa8, 0xb1,
    0xa7, 0xaa, 0xae,
    0xad, 0xab, 0xaa,
    0xad, 0xaa, 0xac,
    0xad, 0xa9, 0xae,
    0xa9, 0xab, 0xac,
    0xaf, 0xaa, 0xac,
    0xaf, 0xa9, 0xae,
    0xa7, 0xaa, 0xaf,
    0xad, 0xa8, 0xb1,
    0xab, 0xaa, 0xae,
    0x33, 0xb9, 0xbd,
    0x31, 0xba, 0xbc,
    0x34, 0xb9, 0xbd,
    0x33, 0xba, 0xbc,
    0xb3, 0xab, 0xac,
    0xb1, 0xac, 0xab,
    0x31, 0xbb, 0xc1,
    0x8a, 0xb4, 0xb9,
    0x8c, 0xb4, 0xb9,
    0x8a, 0xb7, 0xba,
    0x54, 0xea, 0xeb,

    0x4d, 0x52, 0x01, 0x15, 0x17, 0x07, 0x01, 0x17, 0x92, 0x00, 0x52, 0x4e,
    0x0b, 0x16, 0x04, 0x03, 0x01, 0x1a, 0x00, 0x14, 0x1e, 0x1e, 0x13, 0x13,
    0x13, 0x13, 0x1e, 0x00, 0x19, 0x1c, 0x20, 0x02, 0x02, 0x05, 0x1b, 0x0a,
    0x00, 0x4f, 0xa6, 0x6e, 0x88, 0x60, 0x6b, 0x68, 0x85, 0x00, 0x82, 0x82,
    0x82, 0x87, 0x87, 0x87, 0x87, 0x82, 0x00, 0x82, 0x81, 0x7e, 0x68, 0x68,
    0x60, 0x83, 0x79, 0x00, 0xa5, 0x9c, 0x92, 0x84, 0x80, 0x8e, 0x8b, 0x8a,
    0x00, 0x7d, 0x8f, 0x8f, 0x8c, 0x8c, 0x8c, 0x8c, 0x8f, 0x00, 0x8e, 0x89,
    0x8d, 0x8b, 0x8b, 0x80, 0x84, 0x91, 0x00, 0x9d, 0x9c, 0x22, 0x38, 0x36,
    0x32, 0x32, 0x30, 0x00, 0x31, 0x35, 0x35, 0x33, 0x33, 0x33, 0x33, 0x35,
    0x00, 0x35, 0x34, 0x2f, 0x32, 0x32, 0x37, 0x38, 0x21, 0x00, 0x9d, 0x9d,
    0x90, 0x7c, 0x73, 0x7a, 0x7a, 0x73, 0xb0, 0xe1, 0x7a, 0xe9, 0xb4, 0x73,
    0x7c, 0x90, 0x9d, 0x00, 0x97, 0x65, 0x5a, 0x59, 0x59, 0x59, 0x59, 0x59,
    0x38, 0x5b, 0x5b, 0xe2, 0xd9, 0xb3, 0x69, 0x9e, 0xa0, 0x07, 0x2c, 0x3a,
    0x39, 0x39, 0x39, 0xe9, 0xd4, 0xa4, 0x00, 0x3a, 0x2c, 0xa0, 0xaa, 0x3f,
    0x44, 0x43, 0x41, 0x00, 0x44, 0x46, 0x43, 0x41, 0x41, 0x44, 0x44, 0x41,
    0xc0, 0xd9, 0xb2, 0x46, 0x42, 0x40, 0xaa, 0xab, 0xa7, 0x08, 0xa4, 0xa3,
    0xa1, 0xa1, 0xe2, 0xa4, 0xa4, 0xa1, 0xc0, 0xda, 0x91, 0xa1, 0xa2, 0xa7,
    0xab, 0xa9, 0x47, 0x01, 0x4b, 0x4b, 0x4b, 0x4b, 0x48, 0x48, 0x48, 0xe9,
    0xc0, 0xd4, 0x81, 0x45, 0x4c, 0xa8, 0x94, 0x4a, 0x3c, 0x03, 0x3d, 0x3d,
    0x3d, 0x3b, 0x3b, 0x3b, 0xe9, 0xd4, 0x80, 0x89, 0x3e, 0x49, 0x93, 0x9b,
    0x56, 0x53, 0x54, 0x38, 0x54, 0x53, 0xe1, 0xdb, 0xb5, 0x54, 0x58, 0x95,
    0x00, 0x96, 0x55, 0x67, 0x6a, 0x6a, 0x6a, 0x66, 0x66, 0x70, 0x66, 0xe9,
    0xd4, 0x89, 0x67, 0x57, 0x9f, 0x99, 0x00, 0x24, 0x2e, 0x27, 0x28, 0x26,
    0x2c, 0x27, 0x2b, 0x00, 0x2b, 0x29, 0x29, 0x29, 0x29, 0x2b, 0x2b, 0x25,
    0x00, 0x2c, 0x2a, 0x2a, 0x27, 0x2d, 0x23, 0x9a, 0x9c, 0x00, 0x75, 0x5e,
    0x64, 0x61, 0x5f, 0x63, 0x5d, 0x5c, 0x20, 0x5c, 0x5c, 0xe9, 0x5c, 0x5c,
    0x5d, 0x63, 0x5f, 0x00, 0x5f, 0x64, 0x62, 0x75, 0x9d, 0x98, 0x7f, 0x70,
    0x00, 0x6f, 0x6c, 0x74, 0x6d, 0x79, 0x77, 0x77, 0x7b, 0x00, 0x7b, 0x71,
    0x71, 0x77, 0x71, 0x72, 0x76, 0x74, 0x00, 0x74, 0x6f, 0x78, 0x86, 0x98,
    0x51, 0x1f, 0x0c, 0x00, 0x08, 0x0f, 0x12, 0x1d, 0x10, 0x11, 0x11, 0x0d,
    0x00, 0x0d, 0x0e, 0x0e, 0x11, 0x0e, 0x09, 0x1d, 0x12, 0x00, 0x12, 0x08,
    0x10, 0x18, 0x50, 0x4d, 0x00, 0x00, 0x1c, 0x00, 0x07, 0x06, 0xd9, 0xc1,
    0xd4, 0x07, 0x07, 0x80, 0x69, 0x4d,
};