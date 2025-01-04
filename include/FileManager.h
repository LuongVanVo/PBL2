#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Products.h"
#include "Discount.h"
#include "History.h"
#include "Customer.h"
#include <ctime>
#include <chrono>
#include <string>
#include <vector>
using namespace std;

class FileManager
{
public:
    FileManager();
    ~FileManager();
    static Customer *loadCustomer(const std::string &fileCustomer, int &countCustomer); 
    static void saveCustomer(const std::string &fileCustomer, Customer &customer);     

    static Products *loadSpecifications(const string &fileThongSoKyThuat, int &countProducts);
    static Products *loadProducts(const std::string &fileProducts, int &countProducts);
    static void saveProducts(const string fileProducts, Products *products, int &countProduct);

    static Products *loadOrder(const string &fileOrder, int &countOrder);
    static void saveOrder(const string &filename, Products *data, int &count);

    static int loadOrderCode(const string &filename);
    static void saveOrderCoder(const string &filename, long &orderCode);

    static History *loadPay(const string &filename, int &countPay, int &customerID);
    static History *loadPay(const string &filename, const string &currentDate, int &countPay, double &totalPrice);
    static void savePay(const string &filename, Products *pay, int &count, int customerID,
     string addressOrder, long orderCode, double &priceDiscount);

    static string getCurrentDate(); // Lấy thời gian hiện tại

    static void saveDiscount(const string &filename, Discount *discount, int &count);
    static Discount *loadDiscount(const string &filename, int &count);

    // Đọc file trong TinhNangNoiBat
    string readTextTinhNangNoiBat(const string &filePath);

    // Đọc file và trả về mảng string
    static string *readFromFile(const string &filePath, int &count);
    // Đọc file chứa id và path đường dẫn sản phẩm
    static vector<pair<int, string>> readFromFile(const string &fileName);

    // Xóa 1 dòng trong file
    void deleteLineFromFile(const string &filePath, int lineToDelete);

    // Tạo file chỉ mục
    void createIndexFile(const string &productFileName, const string &indexFileName);

    static int *loadLineNumbersFromFile(const string &filePath, int &lineCount);

    // load ID từ file thành 1 mảng số nguyên
    static int *loadIDFromFile(const string &filePath, int &count);
};

#endif