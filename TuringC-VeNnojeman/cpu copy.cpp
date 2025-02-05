/*
/"cpu copy.cpp"
Copyright(C)2021,2024 Allen Nofa
Saturday,21st September,2024
Warning:this file is disused,don't try to compile it!
*/
/*#include <stdio.h>
#include "include/memory copy"
#define byte unsigned char

// 48 pins
byte *alu_run(byte code, byte xval, byte yval)
{
    byte tmp;
    byte ret[3] = {0, 0, 1};
    // main processing
    switch (code)
    {
    case 0x00:
        tmp = xval + yval;
        break;
    case 0x01:
        tmp = xval - yval;
        break;
    case 0x02:
        tmp = ++xval;
        break;
    case 0x03:
        tmp = --xval;
        break;
    case 0x04:
        tmp = xval * yval;
        break;
    case 0x05:
        tmp = xval / yval;
        break;
    case 0x06:
        tmp = xval && yval;
        break;
    case 0x07:
        tmp = xval || yval;
        break;
    case 0x08:
        tmp = xval << yval;
        break;
    case 0x09:
        tmp = xval >> yval;
        break;
    case 0x0a:
        tmp = xval ^ yval;
        break;
    default:
        throw "invalid code on alu pins 1~8";
        break;
    }
    // err processing
    ret[0] = tmp; // pins 25~32
    if (tmp == 0)
    { // err pins 17~24
        ret[1] = 1;
    }
    else if (tmp > 127)
    { // err pins 33~40
        ret[2] = 1;
    }
    else
    {
    }
    return ret; // ret pins 41~48
}
class main
{
public:
    struct rgst ax;
    struct rgst bx;
    struct rgst cx;
    struct rgst dx;
    stack stk;
    memory mm;
    byte control_run(byte *codes);
    rgst *register_match(byte data);
};
byte main::control_run(byte *codes)
{
    byte control = codes[0];
    switch (control)
    {
    case 0:
        break;
    // load
    case 1:
        rgst *rs = register_match(codes[1]);
        byte num = mm.mm_get(codes[2], 0)[0];
        rs->val = num;
        break;
    // store
    case 2:
        rgst *rs = register_match(codes[1]);
        byte data[1] = {rs->val};
        mm.mm_write(data, codes[2]);
        break;
    // push
    case 3:
        rgst *rs = register_match(codes[1]);
        stk.push(rs->val);
        break;
    // popto
    case 4:
        rgst *rs = register_match(codes[1]);
        rs->val = stk.pop();
        break;
    // popkl
    case 5:
        stk.pop();
        break;
    // let
    case 6:
        rgst *rs = register_match(codes[1]);
        rs->val = codes[2];
        break;
    // output
    case 7:
        break;
    // input
    case 8:
        break;
    // swap
    case 9:
        rgst *r1 = register_match(codes[1]);
        rgst *r2 = register_match(codes[2]);
        byte tmp = r1->val;
        r1->val = r2->val;
        r2->val = tmp;
        break;
        // more...
    }
    return 0;
}
rgst *main::register_match(byte data)
{
    rgst *rs = NULL;
    switch (data)
    {
    case 0:
        rs = &ax;
    case 1:
        rs = &bx;
    case 2:
        rs = &cx;
    case 3:
        rs = &dx;
    }
    return rs;
}
int main()
{
    printf("hi");
    return 0;
}*/
