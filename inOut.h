#ifndef INOUT_H
#define INOUT_H

void readFile(int *calcsAlltime, int *errorAlltime, int *ID);
void readManual();
void basicMode(char *ipAddr, int *error, int *errorSession, int *ipVersion);
void readInputIP(char *ipAddr, int *error, int *errorSession, int *ipVersion);
void readInputCIDR(int *cidr, char netClass, int *error, int *errorSession);
void readInputIPv6Prefix(int *cidr, int *error, int *errorSession);
void displayStats(int calcsAlltime, int errorAlltime, int calcSession, int errorSession);
void displayResults(int ipVersion ,char netClass, char *ipAddr, int cidr, char *netAddr, char *broadCAddr, char *usableRange, int subnets);
void saveData(int ID, char *ipAddr, int cidr, char netClass, char *netAddr, char *broadCAddr, char *usableRange, int subnets);
void saveStats(int calcsAlltime, int errorAlltime, int calcSession, int error);

#endif //INOUT_H
