#ifndef CUSTOMERS_H
#define CUSTOMERS_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
using namespace std; 
class Customer {
private:
    int customerID;
    string lastName;
    string firstName;
    string middleName;
    string email;
    string phone;
    string userName;
    string password;
public:
    Customer(int customerID, string lastName, string middleName, string firstName, 
            string email, string phone, string userName, string password);
    Customer();
    // Getter
    int getCustomerID();
    string getLastName();
    string getMiddleName();
    string getFirstName();
    string getEmail();
    string getPhone();
    string getUserName();
    string getPassword();
    string getFullName() const;

    void resizeCustomerArray(Customer*& customers, int &size, const Customer& newCustomer);

    void setPassword(string &password);
    // Verify login
    bool verifyLogin(const string &userName, const string &password);

    int login(const string& username, const string &password, Customer *&loggedInCustomer);
    void registerAccount(string lastName, string middleName, string firstName, string email, 
                         string phone, string userName, string password);
        
    int verifyPhone(const string &phone); // Hàm kiểm tra số điện thoại nhập vào có đúng
    int verifyEmail(const string &email); // Hàm kiểm tra email nhập vào có đúng
    // Quên mật khẩu => đổi lại mật khẩu thông qua số điện thoại
    void forgetPassword(const string& phone, string &newPassword, string& confirmNewPassword); 
    void changePassword();
    void updateCustomerInfo();
    void displayCustomer();

    // Hoán đổi hai khách hàng với nhau
    static void swapCustomer(Customer *customer1, Customer *customer2);
    static int compareNameCustomer(Customer *customer1, Customer *customer2);

    // Sắp xếp theo tên khách hàng
    static int partitionNameCustomer(Customer *customers, int low, int high, bool ascending);
    static void quicksortNameCustomer(Customer *customers, int low, int high, bool ascending);
    
    static Customer* searchNameCustomer(string nameCustomer, int &foundCount);
};

#endif