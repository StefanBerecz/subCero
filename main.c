#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
    //Broadcast address
    //Network address
    //Host range
    //Class A and B
    //separate functions for each class

    int cidr;
    char ipAddr[16];
    char netmask[16];
    char class;
    while(1)
    {
        printf("Enter Class (A, B, C): \n");
        scanf(" %c", &class);
        if(class != 'A' && class != 'B' && class != 'C' && class != 'a' && class != 'b' && class != 'c')
        {
            printf("Invalid Class\n");
            break;
        }
        class = toupper(class);

        printf("Enter IP Address: \n");
        scanf("%s", ipAddr);
        if(strlen(ipAddr)>15 || strlen(ipAddr)<7)
        {
            printf("Invalid IP Address\n");
            break;
        }

        printf("Enter CIDR: \n");
        scanf("%2d", &cidr);
        if(cidr < 24 || cidr > 32)
        {
            printf("Invalid CIDR for Class C\n");
            break;
        }
        printf("Class %c\n", class);
        printf("IP Address: %s\n", ipAddr);
        strcpy(netmask,calcnetmask(cidr));
        printf("Netmask: %s\n", netmask);
        printf("Number of Hosts: %d usable: %d\n", 1 << (32 - cidr), (1 << (32 - cidr)) - 2); // 2 subtracted for network and broadcast address
        printf("Number of Subnets: %d\n", 1 << (cidr - 24)); //24 is the minimum CIDR for subnetting

    }
    return 0;
}
