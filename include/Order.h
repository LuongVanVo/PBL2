#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "D:\PBL2_MYSTORE\include\Products.h"
#include "D:\PBL2_MYSTORE\include\Customer.h"
#include "D:\PBL2_MYSTORE\include\Discount.h"
#include "D:\PBL2_MYSTORE\include\FileManager.h"
#include "D:\PBL2_MYSTORE\include\History.h"
using namespace std;

const int maxSize = 7;
class Order : public Customer, public FileManager, public Discount, public History {
    static Products* products;
    static int countProduct;
    Products* data;   // con trỏ động chứ danh sách
    int count;    // số lượng hiện tại
    static int customerID; // ID của khách hàng
    string addressOrder; // Địa chỉ của đơn hàng
    static long orderCode; // Mã đơn hàng
public:
    Products *getData() const;
    Order();
    Order(const Order& other);
    Order& operator=(const Order& other);
    ~Order();
    int getCountP();
    void displayProduct();
    void saveToFile(const string fileProducts);
    // lấy ID của khách hàng
    void setID(int ID);
    void update();
    // thêm vào
    void addData(int ID);  
    // xóa
    void deleteData(int ID); 
    void deleteData(string name);
    // in ra
    void displayOrder(); 
    // tìm kiếm đơn hàng trong order
    int findData(int& ID) const;
    int findData(const string& name) const;
    //cập nhập thông tin đơn hàng khí có sự thay đổi
    void updateData(int&, const string&, const string&, const string&, const string&, double, int); 
    void clearOrder(); // xóa toàn bộ đơn hàng

    // thanh toán
    void payment(string &addressOrder, string discountCode, double &priceDiscount);

    // Xem lịch sử giao dịch 
    void displayHistory();
    void displayDetailsHistory(long orderCode, Products*& data, int &count);
    
    // Thống kê doanh thu
    double statistic(const string& date); // 1 ngày
 
    int getCount();
    void increaseProductCount(int productID); 
    void decreaseProductCount(int productID); 
    
    void setAddressOrder(string addressOrder);
    string getAddressOrder();
};

#endif