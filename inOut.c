#include "inOut.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "calculations.h"

//Read save file before starting program
void readFile(int *calcsAlltime, int *errorAllTime, int *ID)
{
    //Read calculations and errors from file
    FILE *fp = fopen("stats.csv", "r");
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
    if (sscanf(dataBuffer, "%d,%*[^,],%*d,%*c,%*[^,],%*[^,],%*[^,],%*d,%*d", ID) != 1) 
    {
        printf("Error reading save file\n");
        *ID = 0;
    }
    fclose(fp);
}

void readManual()
{
    printf("\n--------------------------------------------\n");
    printf("This program calculates network information based on IP address and CIDR.\n");
    printf("You can enter an IP address and CIDR to get the network address, broadcast address, usable range, and number of subnets.\n");
    printf("Basic mode will only calculate netmask or network class and not save any data.\n");
    printf("Network classes overview:\n");
    printf("Class A: 0.0.0.0 - 127.255.255.255\n");
    printf("Class B: 128.0.0.0 - 191.255.255.255\n");
    printf("Class C: 192.0.0.0 - 223.255.255.255\n");
    printf("Class D: 224.0.0.0 - 239.255.255.255 (Multicast)\n");
    printf("Class E: 240.0.0.0 - 255.255.255.255 (Experimental)\n");
    printf("--------------------------------------------\n\n");
}

void basicMode(char *ipAddr, int *error, int *errorSession, int *ipVersion)
{
    printf("Basic mode selected. Enter C for network class or N for netmask\n");
    char input;
    scanf(" %c", &input);
    input = toupper(input);
    if(input == 'C')
    {
        printf("\n--------------------------------------------\n");
        readInputIP(ipAddr, error, errorSession, ipVersion);
        if(*error != 0) 
        {
            //No data will be saved in basic mode
            (*errorSession)--;
            (*error)--;
            return;
        }
        char netClass;
        calcClass(ipAddr, &netClass, error, errorSession);
        if(*error != 0) 
        {
            //No data will be saved in basic mode
            (*errorSession)--;
            (*error)--;
            return;
        }
        printf("\nNetwork Class: %c\n", netClass);
        printf("--------------------------------------------\n\n");
    }
    else if(input == 'N')
    {
        printf("\n--------------------------------------------\n");
        int cidr;
        printf("Enter CIDR: \n");
        scanf("%2d", &cidr);
        if(cidr < 1 || cidr > 32)
        {
            printf("Invalid CIDR\n");
            return;
        }
        printf("\nNetmask: %s\n", calcnetmask(cidr));
        printf("--------------------------------------------\n\n");
    }
    else
    {
        printf("\nInvalid input\n");
        printf("--------------------------------------------\n");
    }
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
        if(strlen(ipAddr) > 15 || strlen(ipAddr) < 7 || blocks != 4) 
        {
            printf("Invalid IP Address\n");
            (*errorSession)++;
            (*error)++;
            return;
        }

        // Check if each block is between 0 and 255
        int block;
        char *token = strtok(ipAddr, ".");
        while (token != NULL)
        {
            block = atoi(token); //Convert block to integer
            if (block < 0 || block > 255)
            {
                printf("Invalid IP Address\n");
                (*errorSession)++;
                (*error)++;
                return;
            }
            token = strtok(NULL, ".");
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
    printf("\n--------------------------------------------\n");
    printf("Calculations this Session: %d\nCalculations all time: %d\nErrors this Session: %d\nErrors all time: %d\n", calcSession, calcsAlltime += calcSession, errorSession, errorAlltime += errorSession);
    //Most common network class
    char mostClass = 'X';
    mostClass = mostCommonClass();
    if(mostClass == 'X')
        printf("No data saved\n");
    else
        printf("Most common network class: %c\n", mostCommonClass());
    
    printf("--------------------------------------------\n\n");
    //Rudimentary statistics
    //Might be expanded in future
}

//Display results to user after calculations
void displayResults(int ipVersion ,char netClass, char *ipAddr, int cidr, char *netAddr, char *broadCAddr, char *usableRange, int subnets) 
{
    printf("\n--------------------------------------------\n");
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
    printf("--------------------------------------------\n\n");
}

void saveData(int ID, char *ipAddr, int cidr, char netClass, char *netAddr, char *broadCAddr, char *usableRange, int subnets)
{
    //Save calculated data to file
    FILE *fp = fopen("data.csv", "a");
    if(fp == NULL)
    {
        printf("Error creating save file. Check permissions\n");
        return;
    }
    int usableHosts = (1 << (32 - cidr)) - 2;
    //If file is empty, add headers
    if(ftell(fp) == 0) 
        fprintf(fp, "ID,IP Address,CIDR,Class,Network Address,Broadcast Address,Usable Range,Usable Hosts,Subnets\n"); 
    
    fprintf(fp, "%d,%s,%d,%c,%s,%s,%s,%d,%d\n", ID, ipAddr, cidr, netClass, netAddr, broadCAddr, usableRange, usableHosts, subnets);
    fclose(fp);
}

void saveStats(int calcAllTime, int errorAllTime, int calcSession, int errorSession)
{
    FILE *fp = fopen("stats.csv", "w");
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