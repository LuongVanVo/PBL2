#include "../include\History.h"

History::History() {}

History::History(string currentDate, int customerID, long orderCode, string addressOrder, double priceTotal){
    this->currentDate = currentDate;
    this->customerID = customerID;
    this->orderCode = orderCode;
    this->addressOrder = addressOrder;
    this->priceTotal = priceTotal;
}

History::History(string currentDate, int customerID, long orderCode, string addressOrder, double priceTotal, string item){
    this->currentDate = currentDate;
    this->customerID = customerID;
    this->orderCode = orderCode;
    this->addressOrder = addressOrder;
    this->priceTotal = priceTotal;
    this->item = item;
}

History::~History() {}

string History::getCurrentDate(){
    return this->currentDate;
}
int History::getCustomerID(){
    return this->customerID;
}
long History::getOrderCode(){
    return this->orderCode;
}
string History::getAddressOrder(){
    return this->addressOrder;
}
double History::getPriceTotal(){
    return this->priceTotal;
}
string History::getItem(){
    return this->item;
}