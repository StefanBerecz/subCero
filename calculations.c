#include <stdio.h>
#include "calculations.h"

//Calculate network class from IP Address
void calcClass(char *ipAddr, char *netClass, int *error, int *errorSession)
{
    int block1;
    sscanf(ipAddr, "%d", &block1);
    if(block1 >= 0 && block1 <= 127) 
        *netClass = 'A';
    else if(block1 >= 128 && block1 <= 191) 
        *netClass = 'B';
    else if(block1 >= 192 && block1 <= 223)
        *netClass = 'C';
    else if(block1 >= 224 && block1 <= 239)
        *netClass = 'D';
    else if(block1 >= 240 && block1 <= 255)
        *netClass = 'E';
    else
    {
        printf("Invalid IP Address\n");
        (*errorSession)++;
        (*error)++;
    }
}

//Calculate netmask from CIDR
char* calcnetmask(int cidr)
{
    static char netmask[16];
    int octets[4] = {0, 0, 0, 0}; 
    int fullOctets = cidr / 8; //Number of full octets (each with 8 bits)
    int remainingBits = cidr % 8; //Remaining bits in next octet

    //Set full octets to 255
    for (int i = 0; i < fullOctets; i++) {
        octets[i] = 255;
    }

    //Set remaining bits in next octet
    if (remainingBits > 0)
        octets[fullOctets] = (256 - (1 << (8 - remainingBits)));

    sprintf(netmask, "%d.%d.%d.%d", octets[0], octets[1], octets[2], octets[3]);
    return netmask;
}

//Calculate network address, broadcast address and usable hosts range
void calcNetBroadRange(char *ipAddr, int cidr, char *netAddr, char *broadCAddr, char *usableRange, int *error, int *errorSession)
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
    netBlock4++;
    if(netBlock4 == 256) 
    {
        netBlock4 = 0;
        netBlock3++;
        if(netBlock3 == 256) 
        {
            netBlock3 = 0;
            netBlock2++;
            if(netBlock2 == 256) 
            {
                netBlock2 = 0;
                netBlock1++;
                if(netBlock1 == 256) 
                {
                    printf("Error: Network address out of range\n");
                    (*errorSession)++;
                    (*error)++;
                    return;
                }
            }
        }
    }

    broadBlock4--;
    if(broadBlock4 == -1) 
    {
        broadBlock4 = 255;
        broadBlock3--;
        if(broadBlock3 == -1) 
        {
            broadBlock3 = 255;
            broadBlock2--;
            if(broadBlock2 == -1) 
            {
                broadBlock2 = 255;
                broadBlock1--;
                if(broadBlock1 == -1) 
                {
                    printf("Error: Broadcast address out of range\n");
                    (*errorSession)++;
                    (*error)++;
                    return;
                }
            }
        }
    }
    sprintf(usableRange, "%d.%d.%d.%d - %d.%d.%d.%d", netBlock1, netBlock2, netBlock3, netBlock4, broadBlock1, broadBlock2, broadBlock3, broadBlock4);
}

//Calculate number of subnets
void calcSubs(char netClass, int cidr, int *subnets)
{
    //Calculate number of subnets
    switch(netClass)
    {
        case 'A':
            *subnets = 1 << (cidr - 8); //8 is the minimum CIDR for Class A subnetting
            break;
        case 'B':
            *subnets = 1 << (cidr - 16); //16 is the minimum CIDR for Class B subnetting
            break;
        case 'C':
            *subnets = 1 << (cidr - 24); //24 is the minimum CIDR for Class C subnetting
            break;
        case 'D':
        case 'E':
            *subnets = 0;
            break;
    }
}

//Most common network class in saved data (data.csv)
char mostCommonClass()
{
    FILE *fp = fopen("data.csv", "r");
    if(fp == NULL) 
    {
        printf("Error reading save file\n");
        return 'X';
    }

    char dataBuffer[100];
    int classA = 0, classB = 0, classC = 0, classD = 0, classE = 0;
    while(fgets(dataBuffer, 100, fp) != NULL) 
    {
        char netClass;
        sscanf(dataBuffer, "%*d,%*[^,],%*d,%c,%*[^,],%*[^,],%*[^,],%*d,%*d", &netClass);
        switch(netClass)
        {
            case 'A':
                classA++;
                break;
            case 'B':
                classB++;
                break;
            case 'C':
                classC++;
                break;
            case 'D':
                classD++;
                break;
            case 'E':
                classE++;
                break;
        }
    }

    fclose(fp);

    int maxCount = classA;
    char mostClass = 'A';

    if(classB > maxCount) 
    {
        maxCount = classB;
        mostClass = 'B';
    }
    if(classC > maxCount) 
    {
        maxCount = classC;
        mostClass = 'C';
    }
    if(classD > maxCount) 
    {
        maxCount = classD;
        mostClass = 'D';
    }
    if(classE > maxCount) 
    {
        maxCount = classE;
        mostClass = 'E';
    }

    return mostClass;
}