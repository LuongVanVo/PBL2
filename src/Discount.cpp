#include "D:\PBL2_MYSTORE\include\Discount.h"
#include "D:\PBL2_MYSTORE\include\FileManager.h"

Discount::Discount() {}
Discount::Discount(string discountCode, double price, int amount) {
    this->discountCode = discountCode;
    this->price = price;
    this->amount = amount;
}
string Discount::getDisCountCode(){
    return discountCode;
}
double Discount::getPrice(){
    return price;
}
int Discount::getAmount(){
    return amount;
}
void Discount::setDiscountCode(string& discountCode){
    this->discountCode = discountCode;
}
void Discount::setPrice(double& price){
    this->price = price;
}
void Discount::setAmount(int& amount){
    this->amount = amount;
}

double Discount::discount(string discountCode){
    Discount* discount = nullptr;
    int countD;
    double price = 0.0;
    discount = FileManager::loadDiscount("D:\\PBL2_MYSTORE\\text\\Discount.txt", countD);
    bool found = false;
    for(int i = 0; i < countD; i++){
        if (discount[i].getDisCountCode() == discountCode){
            found = true;
            int k = discount[i].getAmount() - 1;
            if(k >= 0){
                discount[i].setAmount(k); // giảm đi 1 mã giảm giá
                FileManager::saveDiscount("D:\\PBL2_MYSTORE\\text\\Discount.txt", discount, countD);
                price = discount[i].getPrice();
            
            } else {
                cout << "Discount code not available.\n";
            }
            break;
        }
    }
    if (found == false) {
        cout << "No discount code " << discountCode << endl;
    }
    delete [] discount;
    return price;
}

