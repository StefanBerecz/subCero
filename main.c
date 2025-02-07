#include <stdio.h>
#include <string.h>

char* calcnetmask(int cidr)
{
    static char netmask[16];
    int block1, block2, block3, block4;
    //Convert CIDR to netmask
    if(cidr <= 8)
    {
        block1 = 256 - (1 << (8 - cidr));
        block2 = 0;
        block3 = 0;
        block4 = 0;
    }
    else if(cidr <= 16)
    {
        block1 = 255;
        block2 = 256 - (1 << (16 - cidr));
        block3 = 0;
        block4 = 0;
    }
    else if(cidr <= 24)
    {
        block1 = 255;
        block2 = 255;
        block3 = 256 - (1 << (24 - cidr));
        block4 = 0;
    }
    else
    {
        block1 = 255;
        block2 = 255;
        block3 = 255;
        block4 = 256 - (1 << (32 - cidr));
    }
    sprintf(netmask, "%d.%d.%d.%d", block1, block2, block3, block4);
    return netmask;
}

int main()
{
    // Main Code missing:
    //string for netmask
    //scan
    //ausgabe auf konsole

    int cidr;
    char netmask[16];
    while(1)
    {
        printf("Enter CIDR: \n");
        scanf("%2d", &cidr);
        if(cidr < 0 || cidr > 32)
        {
            printf("Invalid CIDR\n");
            break;
        }
        else 
        {
            strcpy(netmask,calcnetmask(cidr)); 
            printf("Netzmask: %s\n", netmask);
        }
    }
    return 0;
}
