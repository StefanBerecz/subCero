#include "inOut.h"
#include <stdio.h>
#include <string.h>
#include "calculations.h"

//Read save file before starting program
void readFile(int *calcsAlltime, int *errorAllTime, int *ID)
{
    //Read calculations and errors from file
    FILE *fp;
    fp = fopen("stats.csv", "r");
    if(fp == NULL)
    {
        printf("Save file not found. New save file will be created\n");
        return;
    }

    // Skip the header line
    char statsBuffer[256];
    if (fgets(statsBuffer, sizeof(statsBuffer), fp) == NULL)
    {
        printf("Error reading save file\n");
        *calcsAlltime = 0;
        *errorAllTime = 0;
    }
    else
    {
        // Read the last line of the file
        while (fgets(statsBuffer, sizeof(statsBuffer), fp) != NULL)
        {
            // Do nothing, just move to the last line
        }

        // Parse the last line
        if (sscanf(statsBuffer, "%d,%d,%*d,%*d", calcsAlltime, errorAllTime) != 2) 
        {
            printf("Error reading save file\n");
            *calcsAlltime = 0;
            *errorAllTime = 0;
        }
    }

    fclose(fp);

    //Read ID from file
    fp = fopen("data.csv", "r");
    if(fp == NULL)
    {
        printf("Save file not found. New save file will be created\n");
        return;
    }

    // Skip the header line
    char dataBuffer[256];
    if (fgets(dataBuffer, sizeof(dataBuffer), fp) == NULL)
    {
        printf("Error reading save file\n");
        *ID = 0;
        fclose(fp);
        return;
    }

    // Read the last line of the file
    while (fgets(dataBuffer, sizeof(dataBuffer), fp) != NULL)
    {
        // Do nothing, just move to the last line
    }

    // Parse the last line
    if (sscanf(dataBuffer, "%d,%*[^,],%*d,%*c,%*[^,],%*[^,],%*[^,],%*d", ID) != 1) 
    {
        printf("Error reading save file\n");
        *ID = 0;
    }
    fclose(fp);
}

//Read IP Address from user
void readInputIP(char *ipAddr, int *error, int *errorSession, int *ipVersion)
{
    printf("Enter IP Address: \n");
    scanf(" %39s", ipAddr);

    if(strchr(ipAddr, ':') != NULL)
    {
        int blocks = 1;
        for(int i = 0; i < strlen(ipAddr); i++)
        {
            if(ipAddr[i] == ':')
                blocks++;
        }

        if(strlen(ipAddr) > 39 || strlen(ipAddr) < 2 || blocks != 8)
        {
            printf("Invalid IPv6 Address\n");
            (*errorSession)++;
            (*error)++;
            return;
        }

        (*ipVersion) = 6;
    }
    else
    {
        //Check if IP Address consists of 4 blocks
        int blocks = 1;

        for(int i = 0; i < strlen(ipAddr); i++)
        {
            if(ipAddr[i] == '.')
                blocks++;
        }   

        //Check if IP Address has correct length and number of blocks
        if(strlen(ipAddr) > 15 || strlen(ipAddr) < 7 || blocks != 4) {
            printf("Invalid IP Address\n");
            (*errorSession)++;
            (*error)++;
            return;
        }
        (*ipVersion) = 4;
    }
}


//Read CIDR from user
void readInputCIDR(int *cidr, char netClass, int *error, int *errorSession)
{
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
    printf("Calculations this Session: %d\nCalculations all time: %d\nErrors this Session: %d\nErrors all time: %d\n", calcSession, calcsAlltime += calcSession, errorSession, errorAlltime += errorSession);
    //Rudimentary statistics
    //Might be expanded in future
}

//Display results to user after calculations
void displayResults(int ipVersion ,char netClass, char *ipAddr, int cidr, char *netAddr, char *broadCAddr, char *usableRange, int subnets) 
{
    if(ipVersion == 4)
    {
        printf("IPv4 Address\n");
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
    else if(ipVersion == 6)
    {
        
            printf("Warning: IPv6 calculations are not supported!\n");
    }
}

void saveData(int ID, char *ipAddr, int cidr, char netClass, char *netAddr, char *broadCAddr, char *usableRange, int subnets)
{
    //Save calculated data to file
    FILE *fp;
    fp = fopen("data.csv", "a");
    if(fp == NULL)
    {
        printf("Error creating save file. Check permissions\n");
        return;
    }

    //If file is empty, add headers
    if(ftell(fp) == 0) 
        fprintf(fp, "ID,IP Address,CIDR,Class,IP Version,Network Address,Broadcast Address,Usable Range,Subnets\n"); 
    
    fprintf(fp, "%d,%s,%d,%c,%s,%s,%s,%d\n", ID, ipAddr, cidr, netClass, netAddr, broadCAddr, usableRange, subnets);
    fclose(fp);
}

void saveStats(int calcAllTime, int errorAllTime, int calcSession, int errorSession)
{
    FILE *fp;
    fp = fopen("stats.csv", "w");
    if(fp == NULL)
    {
        printf("Error creating save file. Check permissions\n");
        return;
    }

    //If file is empty, add headers
    if(ftell(fp) == 0) 
        fprintf(fp, "Calculations All Time,Errors All Time, Calculations last Session, Errors last Session\n"); 
    
    //Update all time calculations and errors
    calcAllTime += calcSession;
    errorAllTime += errorSession;

    fprintf(fp, "%d,%d,%d,%d\n", calcAllTime, errorAllTime, calcSession, errorSession);
    fclose(fp);
}