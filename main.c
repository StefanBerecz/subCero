#include <stdio.h>
#include <ctype.h>
#include "calculations.h"
#include "inOut.h"

typedef struct
{
    int ID;
    int cidr;
    int ipVersion;
    int subnets;
    int calcSession;
    int calcAllTime;
    int error; //Error counter for termination conditions
    int errorSession; // Error counter for session
    int errorAllTime;

} CalculationData;

int main() { 
    CalculationData data = {0, 0, 0, 0, 0, 0, 0, 0}; //Initialize struct
    char ipAddr[40], netAddr[16], broadCAddr[16], usableRange[34], netClass;
    readFile(&data.calcAllTime, &data.errorAllTime, &data.ID); //Read all time calculations, errors and ID from csv files

    while(1) {
        char input;
        printf("Press C to continue, S for statistics, B for basic mode, M for manual or Q to quit\n");
        scanf(" %c", &input);
        input = toupper(input);
        switch(input)
        {
            case 'B':
                data.error = 0; //Reset error counter for termination conditions
                basicMode(ipAddr, &data.error, &data.errorSession, &data.ipVersion);
                continue;
            case 'C':
                data.error = 0; //Reset error counter for termination conditions
                break;
            case 'M':
                readManual();
                continue;;
            case 'Q':
                //Save all time calculations and errors to file
                saveStats(data.calcAllTime, data.errorAllTime, data.calcSession, data.errorSession);
                return 0;
            case 'S':
                displayStats(data.calcAllTime, data.errorAllTime, data.calcSession, data.errorSession);
                continue;
            default:
                printf("Invalid Input\n");
                continue;
        }

        readInputIP(ipAddr, &data.error, &data.errorSession, &data.ipVersion);
        if(data.error != 0) continue;

        if(data.ipVersion == 4)
        {
            //Calculate class from IP Address before CIDR to validate CIDR
            calcClass(ipAddr, &netClass, &data.error, &data.errorSession);
            if(data.error != 0) continue;
            
            readInputCIDR(&data.cidr, netClass, &data.error, &data.errorSession);

            //Calculate network address, broadcast address and usable hosts range
            calcNetBroadRange(ipAddr, data.cidr, netAddr, broadCAddr, usableRange, &data.error, &data.errorSession);
            if(data.error != 0) continue;

            //Calculate number of usable subnets
            calcSubs(netClass, data.cidr, &data.subnets);
            if(data.error != 0) continue;
            
            data.calcSession++; //Increment calculations this session

        }

        if(data.ipVersion == 6)
        {
            data.errorSession++;
            data.error++;
        }
        
        displayResults(data.ipVersion, netClass, ipAddr, data.cidr, netAddr, broadCAddr, usableRange, data.subnets);
        if(data.error != 0) continue;

        data.ID++;
        saveData(data.ID, ipAddr, data.cidr, netClass, netAddr, broadCAddr, usableRange, data.subnets);
    }
    return 0;
}
