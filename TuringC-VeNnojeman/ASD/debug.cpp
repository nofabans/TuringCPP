#include "../cpu.cpp"
int debug(char p, cpu c)
{
    char s;
    while (std::cin >> s)
    {
        if (s == 'd' /*look for a part of the memory*/)
        {
            int head, size;
            printf("head&size?> ");
            scanf("%d %d", &head, &size);
            unsigned char getret[size];
            memset(getret, 0, size);
            c.mm.mm_get(getret, head, head + size - 1);
            for (int i = 0; i < size; ++i)
            {
                printf("%02x ", getret[i]);
            }
            printf("\n");
            printf("\n");
        }
        else if (s == 'r' /*run code*/)
        {
            struct rgst &ir = c.rgls[4];
            struct rgst &pc = c.rgls[5];
            byte ins[4];
            while (1)
            {
                printf("%d\n", ins[0]);
                printf("pc:%d\n", pc.val);
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
                }
                if (ins[0] == 16 /*ret*/)
                    break;
                printf("%d\n", sizeof(ins));
                c.control_run(ins);
                system("pause");
            }
        }
        else if (s == ' ' || s == '\n' /*nothing*/)
        {
            continue;
        }
        else if (s == 'w' /*write bytes in the memory*/)
        {
            byte text[4];
            byte p_mem = 0;
            while (text[0] != 16)
            {
                printf("%d: ", p_mem);
                scanf("%02x %02x %02x %02x", &text[0], &text[1], &text[2], &text[3]);
                c.mm.mm_write(text, p_mem);
                p_mem = p_mem + 4;
            }
        }
        else
        {
            printf("Invalid control!\n");
        }
    }
}
int run_tdb(cpu c)
{
    char s;
    while (std::cin >> s)
    {
        debug(s,c);
        if(s=='e'){break;}
    }
}