#include <stdio.h>
#include <string.h>
#include <ctype.h>

void calcClass(char *ipAddr, char *class)
{
    //Calculate class from IP Address
    int block1;
    sscanf(ipAddr, "%d", &block1);
    if(block1 >= 1 && block1 <= 127)
        *class = 'A';
    else if(block1 >= 128 && block1 <= 191)
        *class = 'B';
    else if(block1 >= 192 && block1 <= 223)
        *class = 'C';
    else
        printf("Invalid IP Address\n");
}

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
    if(cidr == 8)
    {
        block1 = 255;  //256 - (1 << (8 - cidr))
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

void calcNetBroadRange(char *ipAddr, int cidr, char *netAddr, char *broadCAddr, char *usableRange)
{
    int ipBlock1, ipBlock2, ipBlock3, ipBlock4;
    sscanf(ipAddr, "%d.%d.%d.%d", &ipBlock1, &ipBlock2, &ipBlock3, &ipBlock4);

    int netmask1, netmask2, netmask3, netmask4;
    sscanf(calcnetmask(cidr), "%d.%d.%d.%d", &netmask1, &netmask2, &netmask3, &netmask4);

    //Calculate network address
    int netBlock1, netBlock2, netBlock3, netBlock4;
    netBlock1 = ipBlock1 & netmask1; //Bitwise AND
    netBlock2 = ipBlock2 & netmask2;
    netBlock3 = ipBlock3 & netmask3;
    netBlock4 = ipBlock4 & netmask4;
    sprintf(netAddr, "%d.%d.%d.%d", netBlock1, netBlock2, netBlock3, netBlock4);

    //Calculate broadcast address
    int broadBlock1, broadBlock2, broadBlock3, broadBlock4;
    broadBlock1 = netBlock1 | ~netmask1 & 0xFF; //Bitwise OR with inverted netmask and mask with 0xFF to get last octet
    broadBlock2 = netBlock2 | ~netmask2 & 0xFF;
    broadBlock3 = netBlock3 | ~netmask3 & 0xFF;
    broadBlock4 = netBlock4 | ~netmask4 & 0xFF;
    sprintf(broadCAddr, "%d.%d.%d.%d", broadBlock1, broadBlock2, broadBlock3, broadBlock4);

    //Calculate usable range 
    if(netBlock4 == 255)
    {
        if(netBlock3 == 255)
        {
            if(netBlock2 == 255)
            {
                if(netBlock1 == 255)
                {
                    printf("No usable range\n");
                }
                else
                {
                    netBlock1++;
                    netBlock2 = 0;
                    netBlock3 = 0;
                    netBlock4 = 0;
                }
            }
            else
            {
                netBlock2++;
                netBlock3 = 0;
                netBlock4 = 0;
            }
        }
        else
        {
            netBlock3++;
            netBlock4 = 0;
        }
    }
    else
    {
        netBlock4++;
    }

    if(broadBlock4 == 0)
    {
        if(broadBlock3 == 0)
        {
            if(broadBlock2 == 0)
            {
                if(broadBlock1 == 0)
                {
                    printf("No usable range\n");
                }
                else
                {
                    broadBlock1--;
                    broadBlock2 = 255;
                    broadBlock3 = 255;
                    broadBlock4 = 255;
                }
            }
            else
            {
                broadBlock2--;
                broadBlock3 = 255;
                broadBlock4 = 255;
            }
        }
        else
        {
            broadBlock3--;
            broadBlock4 = 255;
        }
    }
    else
    {
        broadBlock4--;
    }

    sprintf(usableRange, "%d.%d.%d.%d - %d.%d.%d.%d", netBlock1, netBlock2, netBlock3, netBlock4, broadBlock1, broadBlock2, broadBlock3, broadBlock4);
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
    char usableRange[34];
    char class;
    while(1)
    {
        printf("Enter Class (A, B, C) (press q to quit, x for automatic mode): \n");
        scanf(" %c", &class);
        class = toupper(class);
        switch(class)
        {
            case 'A':
            case 'B':
            case 'C':
            case 'X':
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

        if(class == 'X')
            calcClass(ipAddr, &class);

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
        
        printf("Number of Hosts: %d usable: %d\n", 1 << (32 - cidr), (1 << (32 - cidr)) - 2); // 2 subtracted for network and broadcast address

        calcNetBroadRange(ipAddr, cidr, netAddr, broadCAddr, usableRange);
        printf("Network Address: %s\n", netAddr);
        printf("Broadcast Address: %s\n", broadCAddr);
        printf("Usable Range: %s\n", usableRange);

        calcSubs(class, cidr, subnetptr);
        if(subnets < 2)
            printf("No Subnets, default network only\n");
        else
            printf("Number of Subnets: %d\n", subnets);
    }
    return 0;
}
