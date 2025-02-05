/*
/cpu.cpp
Copyright(C)2021,2024 Allen Nofa
Sunday,6th October,2024
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include "include/memory.hpp"
#include "include/alias.hpp"
#include "include/register"
typedef unsigned char byte;

// section of CPU

// 48 pins
static byte alu_run(byte code, byte xval, byte yval)
{
    byte tmp;
    // main processing
    switch (code)
    {
    case 0x00: // ADD
        tmp = xval + yval;
        break;
    case 0x01: // SUB
        tmp = xval - yval;
        break;
    case 0x02: // INC
        tmp = ++xval;
        break;
    case 0x03: // DEC
        tmp = --xval;
        break;
    case 0x04: // MUL
        tmp = xval * yval;
        break;
    case 0x05: // DIV
        tmp = xval / yval;
        break;
    case 0x06: // AND
        tmp = xval & yval;
        break;
    case 0x07: // OR
        tmp = xval | yval;
        break;
    case 0x08: // LST
        tmp = xval << yval;
        break;
    case 0x09: // RST
        tmp = xval >> yval;
        break;
    case 0x0a: // XOR
        tmp = xval ^ yval;
        break;
    default: // UNKOWN
        throw "invalid code on alu pins 1~8";
        break;
    }
    // err processing
    return tmp; // ret pins 41~48
}
class cpu
{
public:
    struct rgst rgls[8]; // ax,bx,cx,dx,ir,pc,(zr),(fl)
    struct rgst cs;
    struct rgst ds;
    stack stk;
    memory mm;
    unsigned char control_run(byte codes[4]);
};
byte cpu::control_run(byte codes[4])
{
    byte control = codes[0];
    struct rgst &rs = rgls[codes[1]];
    // printf("%d\n",control);
    switch (control)
    {
    case 0:
        break;
    // load
    case 1:
    {
        byte num[1];
        mm.mm_get(num, codes[2], codes[2]);
        rs.val = num[0];
        break;
    }
    // store
    case 2:
    {
        byte data[1] = {rs.val};
        mm.mm_write(data, codes[2]);
        break;
    }
    // push
    case 3:
    {
        stk.push(rs.val);
        break;
    }
    // popto
    case 4:
    {
        rs.val = stk.pop();
        break;
    }
    // popkl
    case 5:
    {
        stk.pop();
        break;
    }
    // let
    case 6:
    {
        rs.val = codes[2];
        // printf("%d\n",rs.val);
        break;
    }
    // output
    case 8:
    {
        byte io_num = codes[1];
        byte data[codes[3] - codes[2] + 1];
        memset(data, 0, sizeof(data));
        mm.mm_get(data, codes[2], codes[3]);
        for (int i = 0; i < (sizeof(data) / sizeof(data[0])); ++i)
        {
            if (io_num == 0)
            {
                printf("%d\n", data[i]);
            }
            else if (io_num == 1)
            {
                printf("%x", data[i]);
            }
            else if (io_num == 2)
            {
                printf("%c", data[i]);
            }
        }
        printf("\n");
        break;
    }
    // swap
    case 7:
    {
        struct rgst &r2 = rgls[codes[2]];
        byte tmp = rs.val;
        rs.val = r2.val;
        r2.val = tmp;
        break;
    }
    // alc
    case 9:
    {
        struct rgst &r2 = rgls[codes[2]];
        byte alu_num = codes[3];
        // printf("%d,%d",rs.val,r2.val);
        rs.val = alu_run(alu_num, rs.val, r2.val);
        break;
    }
    // jmp
    case 10:
    {
        rgls[5].val += codes[1];
    }
    // jeq
    case 11:
    {
        struct rgst &r2 = rgls[codes[2]];
        if (r2.val == rs.val)
        {
            rgls[5].val += codes[3];
        }
    }
    // jne
    case 12:
    {
        struct rgst &r2 = rgls[codes[2]];
        if (r2.val != rs.val)
        {
            rgls[5].val += codes[3];
        }
    }
    // jgt
    case 13:
    {
        struct rgst &r2 = rgls[codes[2]];
        if (r2.val < rs.val)
        {
            rgls[5].val += codes[3];
        }
    }
    // jlt
    case 14:
    {
        struct rgst &r2 = rgls[codes[2]];
        if (r2.val > rs.val)
        {
            rgls[5].val += codes[3];
        }
    }
    // jzr
    case 15:
    {
        if (rs.val == 0)
        {
            rgls[5].val += codes[2];
        }
    }
    // ret(not in here)
    case 16:
    {
    }
    // in(alias)
    case 17:
    {
        byte alias_num = codes[3];
        Alias *alias_object = &mm;
        if (alias_num == 0xf0)
        {
            alias_object = &mm;
        }
        else if (alias_num == 0xf1)
        {
            alias_object = &stk;
        }
        byte args_package[2] = {codes[2], rs.val};
        rs.val = alias_object->arg_in(args_package);
    }
    // out(alias)
    case 18:
    {
        byte alias_num = codes[3];
        Alias *alias_object;
        if (alias_num == 0xf0)
        {
            alias_object = &mm;
        }
        else if (alias_num == 0xf1)
        {
            alias_object = &stk;
        }
        byte args_package[2] = {codes[2], rs.val};
        alias_object->arg_out(args_package);
    }
    }
    return 0;
}
// section of BIOS & trun on

class bios
{
public:
    int POST(cpu c);
    void INIT(cpu c);
};
int bios::POST(cpu c)
{
    // memory-error
    int e = -1;
    if (c.mm.rw_flag == false)
    {
        e = 0;
        return e;
    }
    printf("size of mem:%d \n", sizeof(c.mm.vals));
    printf("size of stk:%d \n", sizeof(c.stk.vals));
    return e;
}
void bios::INIT(cpu c)
{
    for (int i = 0; i <= 6; ++i)
    {
        c.rgls[i].val = 0;
    }
    c.rgls[7].val = 255;
    c.cs.val = 0;
    c.ds.val = 120;
    // printf("Test Success!\n");
}