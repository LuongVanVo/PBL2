#ifndef DISCOUNT_H
#define DISCOUNT_H

#include <string>
#include <iostream>
using namespace std;

class Discount{
    protected:
        string discountCode; // mã giảm giá
        double price; // số tiền giảm
        int amount; // số lượng phiếu giảm giá
    public:
        Discount();
        Discount(string, double, int);
        ~Discount() {}
        string getDisCountCode();
        double getPrice();
        int getAmount();
        void setDiscountCode(string&);
        void setPrice(double&);
        void setAmount(int&);
        double discount(string discountCode);
};

#endif