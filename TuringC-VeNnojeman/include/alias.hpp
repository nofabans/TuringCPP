#pragma once
class Alias
{
public:
    virtual unsigned char arg_in(unsigned char args[]) = 0;
    virtual void arg_out(unsigned char args[]) = 0;
};
