#include <stdio.h>
#include <ctype.h>
#include "calculations.h"
#include "inOut.h"

int main() {
    int cidr, subnets;
    int calcSession = 0;
    int calcAllTime = 0;
    int error = 0; //Error counter for termination conditions
    int errorSession = 0; // Error counter for session
    int errorAllTime = 0; 
    readFile(&calcAllTime, &errorAllTime); //Read all time calculations and errors
    char ipAddr[16], netAddr[16], broadCAddr[16], usableRange[34], netClass;

    while(1) {
        printf("Press C to continue, S for statistics or Q to quit\n");
        scanf(" %c", &netClass);
        netClass = toupper(netClass);
        switch(netClass)
        {
            case 'C':
                //Save stats so errors are saved correctly before resetting
                saveStats(calcAllTime, errorAllTime, calcSession, error);
                error = 0;
                break;
            case 'Q':
                saveStats(calcAllTime, errorAllTime, calcSession, error);
                return 0;
            case 'S':
                displayStats(calcAllTime, errorAllTime, calcSession, errorSession);
                continue;
            default:
                printf("Invalid Input\n");
                continue;
        }

        readInputIP(ipAddr, &error, &errorSession);
        if(error != 0) continue;

        //Calculate class from IP Address before CIDR to validate CIDR
        calcClass(ipAddr, &netClass, &error, &errorSession);
        if(error != 0) continue;
        
        readInputCIDR(&cidr, netClass, &error, &errorSession);
        if(error != 0) continue;

        //Calculate network address, broadcast address and usable hosts range
        calcNetBroadRange(ipAddr, cidr, netAddr, broadCAddr, usableRange, &error, &errorSession);
        if(error != 0) continue;

        //Calculate number of usable subnets
        calcSubs(netClass, cidr, &subnets);
        
        calcSession++; //Increment calculations this session

        displayResults(netClass, ipAddr, cidr, netAddr, broadCAddr, usableRange, subnets);
        saveResults(netClass, ipAddr, cidr, netAddr, broadCAddr, usableRange, subnets);
    }
    return 0;
}
