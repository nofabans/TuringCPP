#include "../cpu.cpp"
void runexe(cpu c, char *filename)
{
    FILE *exef = fopen(filename, "rb");
    if (!exef)
    {
        perror("invalid file:(");
    }
    byte contents[1024] = {0};
    fread(contents, 1, sizeof(contents), exef);
    c.mm.mm_write(contents, 0);
    struct rgst &ir = c.rgls[4];
    struct rgst &pc = c.rgls[5];
    byte ins[4];
    while (1)
    {
        printf("pc:%d",pc.val);
        memset(ins, 0, 4);
        c.mm.mm_get(ins, pc.val, pc.val + 4);
        if (ins[0] == 10 /*jmp*/)
        {
            pc.val = ins[1];
            continue;
        }
        else if (ins[0] == 11 /*jeq*/)
        {
            struct rgst &r2 = c.rgls[ins[2]];
            struct rgst &rs = c.rgls[ins[1]];
            if (r2.val == rs.val)
            {
                pc.val = ins[3];
            }
        }
        else if (ins[0] == 12)
        {
            struct rgst &r2 = c.rgls[ins[2]];
            struct rgst &rs = c.rgls[ins[1]];
            if (r2.val != rs.val)
            {
                pc.val = ins[3];
            }
        }
        else if (ins[0] == 13)
        {
            struct rgst &r2 = c.rgls[ins[2]];
            struct rgst &rs = c.rgls[ins[1]];
            if (r2.val < rs.val)
            {
                pc.val = ins[3];
            }
        }
        else if (ins[0] == 14)
        {
            struct rgst &r2 = c.rgls[ins[2]];
            struct rgst &rs = c.rgls[ins[1]];
            if (r2.val > rs.val)
            {
                pc.val = ins[3];
            }
        }
        else if (ins[0] == 15)
        {
            struct rgst &rs = c.rgls[ins[1]];
            if (rs.val == 0)
            {
                pc.val = ins[3];
            }
        }else{pc.val +=4;}
        if (ins[0] == 16 /*ret*/)
            break;
        c.control_run(ins);
    }
}
