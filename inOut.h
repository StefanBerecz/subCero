#ifndef INOUT_H
#define INOUT_H

void readInput(char *ipAddr, int *cidr, int *error);
void displayResults(char netClass, char *ipAddr, int cidr, char *netAddr, char *broadCAddr, char *usableRange, int subnets);
void saveResults(char netClass, char *ipAddr, int cidr, char *netAddr, char *broadCAddr, char *usableRange, int subnets);

#endif //INOUT_H
