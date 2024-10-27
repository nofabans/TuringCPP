/*
/include/memory.hpp
Copyright(C)2023,2024 Allen Nofa
Sunday,6th October,2024
*/
#pragma once
#include "register"
#include "alias.hpp"

class memory : public Alias
{
public:
    unsigned char vals[256];
    void mm_get(unsigned char swsbc[], unsigned char start, unsigned char stop);
    void mm_write(unsigned char *datas, unsigned char start);
    bool rw_flag = true;

public:
    unsigned char arg_in(unsigned char args[]) override;
    void arg_out(unsigned char args[]) override;
};
void memory::mm_get(unsigned char swsbc[], unsigned char start, unsigned char stop)
{
    unsigned char length = stop - start + 1;
    for (int i = 0; i < length; ++i)
        swsbc[i] = vals[start + i];
}
void memory::mm_write(unsigned char *datas, unsigned char start)
{
    unsigned char length = sizeof(datas) / sizeof(datas[0]);
    for (int i = 0; i < length; ++i)
        vals[start + i] = datas[i];
}
unsigned char memory::arg_in(unsigned char args[])
{
    unsigned char address = args[0];
    return vals[address];
}
void memory::arg_out(unsigned char args[])
{
    unsigned char address = args[0];
    unsigned char val = args[1];
    vals[address] = val;
}

class stack : public Alias
{
public:
    struct rgst sp;
    struct rgst bp;
    unsigned char vals[100];
    unsigned char pop();
    void push(unsigned char data);

public:
    unsigned char arg_in(unsigned char args[]) override;
    void arg_out(unsigned char args[]) override;
};
unsigned char stack::pop()
{
    if (sp.val <= 0)
        throw "stack overflow";
    --sp.val;
    return vals[sp.val];
}
void stack::push(unsigned char data)
{
    if (sp.val == 100)
        throw "stack overflow";
    vals[sp.val] = data;
    ++sp.val;
}
unsigned char stack::arg_in(unsigned char args[])
{
    return pop();
}
void stack::arg_out(unsigned char args[])
{
    push(args[1]);
}