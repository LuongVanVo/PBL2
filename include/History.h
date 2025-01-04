#ifndef HISTORY_H
#define HISTORY_H

#include <string.h>
#include <iostream>
using namespace std;

class History {
    protected:
        string currentDate;
        int customerID;
        long orderCode;
        string addressOrder;
        double priceTotal;
        string item;
    public:
        History();
        History(string currentDate, int customerID, long orderCode, 
        string addressOrder, double priceTotal);
        History(string currentDate, int customerID, long orderCode, 
        string addressOrder, double priceTotal, string item);
        ~History();
        string getCurrentDate();
        int getCustomerID();
        long getOrderCode();
        string getAddressOrder();
        double getPriceTotal();
        string getItem();
};

#endif