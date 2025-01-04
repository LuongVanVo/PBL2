#ifndef ADMIN_H
#define ADMIN_H

#include "Customer.h"
#include "Order.h"
#include "Products.h"
#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Admin {
    public:
        Admin();
        ~Admin();
        void addProduct(string fileName, string imagePath, int productID, string genre, 
        string nameProduct, double priceProduct, string manufacturer, string operatingSystem, int quantity);
        void editProduct(int &productID, string newNameProduct, string newGenre, 
        double newPriceProduct, string newManufacturer, string newOperatingSystem, int newQuantity);
        void deleteProduct(int ProductID);

        void generateDiscountCode(string &discountCode, double &price, int &quantity);
};

#endif