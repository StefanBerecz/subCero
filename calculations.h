#ifndef CALCULATIONS_H
#define CALCULATIONS_H

void calcClass(char *ipAddr, char *netClass, int *error);
char* calcnetmask(int cidr);
void calcNetBroadRange(char *ipAddr, int cidr, char *netAddr, char *broadCAddr, char *usableRange, int *error);
void calcSubs(char netClass, int cidr, int *subnets);

#endif //CALCULATIONS_H
