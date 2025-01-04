#include "D:\PBL2_MYSTORE\include\Customer.h"
#include "D:\PBL2_MYSTORE\include\FileManager.h"
#include "D:\PBL2_MYSTORE\include\Products.h"
#include "D:\PBL2_MYSTORE\include\Order.h"
#include "D:\PBL2_MYSTORE\include\Admin.h"
#include "D:\PBL2_MYSTORE\include\Discount.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
    Customer *customers = new Customer;
    Products *products = new Products;
    // Product *product = new Product;
    Order *orders = new Order;
    Admin *admin = new Admin;
    FileManager *fileManager = new FileManager;

    fileManager->createIndexFile("D:\\PBL2_MYSTORE\\text\\ThongSoKyThuat.txt", "D:\\PBL2_MYSTORE\\text\\indexFileProduct.txt");
    cout << "Enter ID PRoduct: ";
    int ID; cin >> ID;
    cout << products->getSpecificationByID("text\\ThongSoKyThuat.txt", "text\\indexFileProduct.txt", ID);
    return 0;
}