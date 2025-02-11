#include <stdio.h>
#include <ctype.h>
#include "calculations.h"
#include "inOut.h"

int main() {
    int cidr, subnets;
    int error = 0;
    char ipAddr[16], netAddr[16], broadCAddr[16], usableRange[34], netClass;

    while(1) {
        printf("Press C to continue or Q to quit\n");
        scanf(" %c", &netClass);
        netClass = toupper(netClass);
        if(netClass == 'Q') return 0;
        if(netClass != 'C') 
        {
            printf("Invalid Input\n");
            continue;
        }

        readInput(ipAddr, &cidr, &error);
        if(error) continue;

        calcClass(ipAddr, &netClass, &error);
        if(error) continue;

        calcNetBroadRange(ipAddr, cidr, netAddr, broadCAddr, usableRange, &error);
        if(error) continue;

        calcSubs(netClass, cidr, &subnets);
        
        displayResults(netClass, ipAddr, cidr, netAddr, broadCAddr, usableRange, subnets);
        saveResults(netClass, ipAddr, cidr, netAddr, broadCAddr, usableRange, subnets);
    }
    return 0;
}
