#include "inOut.h"
#include <stdio.h>
#include <string.h>
#include "calculations.h"

void readInput(char *ipAddr, int *cidr, int *error) {
    printf("Enter IP Address: \n");
    scanf(" %15s", ipAddr);

    //Check if IP Address consists of 4 blocks
    int blocks = 0;
    for(int i = 0; i < strlen(ipAddr); i++)
    {
        if(ipAddr[i] == '.')
            blocks++;
    }   

    if(strlen(ipAddr) > 15 || strlen(ipAddr) < 7 || blocks != 3) {
        printf("Invalid IP Address\n");
        *error = 1;
        return;
    }

    printf("Enter CIDR: \n");
    scanf("%2d", cidr);
    if(*cidr < 8 || *cidr > 30) {
        printf("Invalid CIDR or too few usable hosts\n");
        *error = 1;
        return;
    }
}

void displayResults(char netClass, char *ipAddr, int cidr, char *netAddr, char *broadCAddr, char *usableRange, int subnets) {
    printf("Class %c\n", netClass);
    printf("IP Address: %s\n", ipAddr);
    printf("Netmask: %s\n", calcnetmask(cidr));
    printf("Network Address: %s\n", netAddr);
    printf("Broadcast Address: %s\n", broadCAddr);
    printf("Usable Range: %s\n", usableRange);
    printf("Number of Hosts: %d usable: %d\n", 1 << (32 - cidr), (1 << (32 - cidr)) - 2);
    if(netClass == 'D')
        printf("Class D is reserved for multicast\n");
    else if (netClass == 'E')
        printf("Class E is reserved for experimental use\n");
    else if(subnets < 2)
        printf("No Subnets, default network only\n");
    else
        printf("Number of Subnets: %d\n", subnets);
}

void saveResults(char netClass, char *ipAddr, int cidr, char *netAddr, char *broadCAddr, char *usableRange, int subnets)
{
    FILE *fp;
    fp = fopen("results.txt", "a");
    if(fp == NULL)
    {
        printf("Error opening file\n");
        return;
    }
    fprintf(fp, "Class %c\n", netClass);
    fprintf(fp, "IP Address: %s\n", ipAddr);
    fprintf(fp, "Netmask: %s\n", calcnetmask(cidr));
    fprintf(fp, "Network Address: %s\n", netAddr);
    fprintf(fp, "Broadcast Address: %s\n", broadCAddr);
    fprintf(fp, "Usable Range: %s\n", usableRange);
    fprintf(fp, "Number of Hosts: %d usable: %d\n", 1 << (32 - cidr), (1 << (32 - cidr)) - 2);
    if(netClass == 'D')
        fprintf(fp, "Class D is reserved for multicast\n");
    else if (netClass == 'E')
        fprintf(fp, "Class E is reserved for experimental use\n");
    else if(subnets < 2)
        fprintf(fp, "No Subnets, default network only\n");
    else
        fprintf(fp, "Number of Subnets: %d\n", subnets);
    fprintf(fp, "\n----------------------------------------\n\n");
    fclose(fp);    
}