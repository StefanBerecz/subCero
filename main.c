#include <stdio.h>
#include <string.h>
#include <ctype.h>

int checkCidrClass(char class, int cidr)
{
    //Check if CIDR is valid for the class
    switch(class)
    {
        case 'A':
            if(cidr < 8 || cidr > 30) //30 as 2 addresses are reserved for network and broadcast
                return 1;
            break;
        case 'B':
            if(cidr < 16 || cidr > 30)
                return 1;
            break;
        case 'C':
            if(cidr < 24 || cidr > 30)
                return 1;
            break;
        default:
            return 0;
    }
    return 0;
}

char* calcnetmask(int cidr)
{
    //Calculate netmask from CIDR
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

void calcSubs(char class, int cidr, int *subnetptr)
{
    //Calculate number of subnets
    switch(class)
    {
        case 'A':
            *subnetptr = 1 << (cidr - 8); //8 is the minimum CIDR for Class A subnetting
            break;
        case 'B':
            *subnetptr = 1 << (cidr - 16); //16 is the minimum CIDR for Class B subnetting
            break;
        case 'C':
            *subnetptr = 1 << (cidr - 24); //24 is the minimum CIDR for Class C subnetting
            break; 
    }
}

int main()
{
    int cidr;
    int hosts, subnets;
    int *subnetptr = &subnets;
    char ipAddr[16];
    char netAddr[16];
    char broadCAddr[16];
    char netmask[16];
    char class;
    while(1)
    {
        printf("Enter Class (A, B, C) (press q to quit): \n");
        scanf(" %c", &class);
        class = toupper(class);
        switch(class)
        {
            case 'A':
            case 'B':
            case 'C':
                break;
            case 'Q':
                return 0;
            default:
                printf("Invalid Class\n");
                continue;
        }

        printf("Enter IP Address: \n");
        scanf("%s", ipAddr);
        if(strlen(ipAddr)>15 || strlen(ipAddr)<7)
        {
            printf("Invalid IP Address\n");
            continue;
        }

        printf("Enter CIDR: \n");
        scanf("%2d", &cidr);
        if(checkCidrClass(class, cidr) == 1)
        {
            printf("Invalid CIDR for Class %c or too few usable hosts\n", class);
            continue;
        }

        printf("Class %c\n", class);
        printf("IP Address: %s\n", ipAddr);
        strcpy(netmask,calcnetmask(cidr));
        printf("Netmask: %s\n", netmask);
        
        calcSubs(class, cidr, subnetptr);
        printf("Number of Hosts: %d usable: %d\n", 1 << (32 - cidr), (1 << (32 - cidr)) - 2); // 2 subtracted for network and broadcast address

        if(subnets < 2)
            printf("No Subnets, default network only\n");
        else
            printf("Number of Subnets: %d\n", subnets);
    }
    return 0;
}
