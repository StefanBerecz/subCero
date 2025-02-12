#ifndef INOUT_H
#define INOUT_H

void readFile(int *calcsAlltime, int *errorAlltime);
void readInput(char *ipAddr, int *cidr, int *error, int *errorSession);
void displayStats(int calcsAlltime, int errorAlltime, int calcSession, int error);
void displayResults(char netClass, char *ipAddr, int cidr, char *netAddr, char *broadCAddr, char *usableRange, int subnets);
void saveResults(char netClass, char *ipAddr, int cidr, char *netAddr, char *broadCAddr, char *usableRange, int subnets);
void saveStats(int calcsAlltime, int errorAlltime, int calcSession, int error);

#endif //INOUT_H
