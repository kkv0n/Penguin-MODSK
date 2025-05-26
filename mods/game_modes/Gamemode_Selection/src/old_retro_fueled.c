#include "common.h"
#include "utils.h"

void EnableRetroFueled(){
    *(unsigned char*)0x8005AF76 = 0x0;
    *(unsigned char*)0x8005EC65 = 0x8;
    *(unsigned char*)0x8006224C = 0xBC;
    *(unsigned char*)0x8006224E = 0x22;
    *(unsigned char*)0x8006224F = 0x8E;
    *(unsigned char*)0x80062250 = 0x2;
    *(unsigned char*)0x80062252 = 0xB8;
    *(unsigned char*)0x80062253 = 0x30;
    *(unsigned char*)0x80062254 = 0x20;
    *(unsigned char*)0x80062256 = 0xB9;
    *(unsigned char*)0x80062257 = 0x30;
    *(unsigned char*)0x80062258 = 0x6;
    *(unsigned char*)0x8006225B = 0x17;
    *(unsigned char*)0x8006225C = 0x20;
    *(unsigned char*)0x8006225D = 0xC0;
    *(unsigned char*)0x8006225E = 0x19;
    *(unsigned char*)0x8006225F = 0x3;
    *(unsigned char*)0x80062260 = 0x4;
    *(unsigned char*)0x80062262 = 0x20;
    *(unsigned char*)0x80062263 = 0x13;
    *(unsigned char*)0x8006227C = 0x22;
    *(unsigned char*)0x8006227E = 0x19;
    *(unsigned char*)0x8006227F = 0x20;
    *(unsigned char*)0x8006228C = 0x3;
    *(unsigned char*)0x8006229C = 0x2;
    *(unsigned char*)0x8006229E = 0x19;
    *(unsigned char*)0x8006229F = 0x13;
    *(unsigned char*)0x800622A0 = 0x0;
    *(unsigned char*)0x800622A2 = 0x0;
    *(unsigned char*)0x800622A3 = 0x0;
    *(unsigned char*)0x800622A4 = 0x2;
    *(unsigned char*)0x800622A6 = 0x40;
    *(unsigned char*)0x800622A7 = 0x14;
    *(unsigned char*)0x800622A8 = 0x6;
    *(unsigned char*)0x800622AA = 0x2;
    *(unsigned char*)0x800622AB = 0x24;
    *(unsigned char*)0x800622AC = 0x1C;
    *(unsigned char*)0x800622AE = 0x82;
    *(unsigned char*)0x800622AF = 0x16;
}

void DisableRetroFueled(){
    *(unsigned char*)0x8005AF76 = 0x40;
    *(unsigned char*)0x8005EC65 = 0x1;
    *(unsigned char*)0x8006224C = 0x1C;
    *(unsigned char*)0x8006224E = 0xB8;
    *(unsigned char*)0x8006224F = 0x8F;
    *(unsigned char*)0x80062250 = 0x0;
    *(unsigned char*)0x80062252 = 0x0;
    *(unsigned char*)0x80062253 = 0x0;
    *(unsigned char*)0x80062254 = 0x7;
    *(unsigned char*)0x80062256 = 0x0;
    *(unsigned char*)0x80062257 = 0x13;
    *(unsigned char*)0x80062258 = 0x0;
    *(unsigned char*)0x8006225B = 0x0;
    *(unsigned char*)0x8006225C = 0xBC;
    *(unsigned char*)0x8006225D = 0x0;
    *(unsigned char*)0x8006225E = 0x22;
    *(unsigned char*)0x8006225F = 0x8E;
    *(unsigned char*)0x80062260 = 0x0;
    *(unsigned char*)0x80062262 = 0x0;
    *(unsigned char*)0x80062263 = 0x0;
    *(unsigned char*)0x8006227C = 0x0;
    *(unsigned char*)0x8006227E = 0x0;
    *(unsigned char*)0x8006227F = 0x0;
    *(unsigned char*)0x8006228C = 0x2;
    *(unsigned char*)0x8006229C = 0x0;
    *(unsigned char*)0x8006229E = 0x0;
    *(unsigned char*)0x8006229F = 0x0;
    *(unsigned char*)0x800622A0 = 0x3;
    *(unsigned char*)0x800622A2 = 0x40;
    *(unsigned char*)0x800622A3 = 0x14;
    *(unsigned char*)0x800622A4 = 0x6;
    *(unsigned char*)0x800622A6 = 0x2;
    *(unsigned char*)0x800622A7 = 0x24;
    *(unsigned char*)0x800622A8 = 0x1D;
    *(unsigned char*)0x800622AA = 0x82;
    *(unsigned char*)0x800622AB = 0x16;
    *(unsigned char*)0x800622AC = 0x0;
    *(unsigned char*)0x800622AE = 0x0;
    *(unsigned char*)0x800622AF = 0x0;
}