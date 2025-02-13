#include "inOut.h"
#include <stdio.h>
#include <string.h>
#include "calculations.h"

//Read save file before starting program
void readFile(int *calcsAlltime, int *errorAllTime)
{
    FILE *fp;
    fp = fopen("saves.txt", "r");
    if(fp == NULL)
    {
        printf("Save file not found. New save file will be created\n");
        return;
    }

    //Set values and if file is not in correct format set to 0
    if(fscanf(fp, "Calculations: %d, Errors: %d", calcsAlltime, errorAllTime) != 2)
    {
        printf("Error reading save file\n");
        *calcsAlltime = 0;
        *errorAllTime = 0;
    }
    fclose(fp);
}

//Read IP Address from user
void readInputIP(char *ipAddr, int *error, int *errorSession) {
    printf("Enter IP Address: \n");
    scanf(" %15s", ipAddr);

    //Check if IP Address consists of 4 blocks
    int blocks = 0;
    for(int i = 0; i < strlen(ipAddr); i++)
    {
        if(ipAddr[i] == '.')
            blocks++;
    }   

    //Check if IP Address has correct length and number of blocks
    if(strlen(ipAddr) > 15 || strlen(ipAddr) < 7 || blocks != 3) {
        printf("Invalid IP Address\n");
        (*errorSession)++;
        (*error)++;
        return;
    }
}

//Read CIDR from user
void readInputCIDR(int *cidr, char netClass, int *error, int *errorSession) {
    printf("Enter CIDR: \n");
    scanf("%2d", cidr);

    int minCIDR;
    switch(netClass) {
        case 'A':
            minCIDR = 8;
            break;
        case 'B':
            minCIDR = 16;
            break;
        case 'C':
            minCIDR = 24;
            break;
        default:
            minCIDR = 0;
            break;
    }

    //Check if CIDR is within range for given class
    if(*cidr < minCIDR || *cidr > 30) {
        printf("You entered a class %c address. For a usable class %c network CIDR should be between %d and 30\n", netClass, netClass, minCIDR);
        (*errorSession)++;
        (*error)++;
        return;
    }
}

//Display statistics if user inputs S
void displayStats(int calcsAlltime, int errorAlltime, int calcSession, int errorSession)
{
    printf("Calculations this Session: %d\nCalculations all time: %d\nErrors this Session: %d\nErrors all time: %d\n", calcSession, calcsAlltime, errorSession, errorAlltime);
}

//Display results to user after calculations
void displayResults(char netClass, char *ipAddr, int cidr, char *netAddr, char *broadCAddr, char *usableRange, int subnets) 
{
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

//Save results to file after calculations
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
        fprintf(fp, "Number of usable Subnets: %d\n", subnets);
    fprintf(fp, "\n----------------------------------------\n\n");
    fclose(fp);    
}

//Save statistics to file before new calculation or quitting
void saveStats(int calcsAlltime, int errorAlltime, int calcSession, int error)
{
    FILE *fp;
    fp = fopen("saves.txt", "w");
    if(fp == NULL)
    {
        printf("Error creating save file\n");
        return;
    }
    fprintf(fp, "Calculations: %d, Errors: %d\n", calcsAlltime + calcSession, errorAlltime + error);
    fclose(fp);
}