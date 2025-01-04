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
    Product *product = new Product;
    Order *orders = new Order;
    Admin *admin = new Admin;
    FileManager *fileManager = new FileManager;

    int option;
    int ID; // ID cua nguoi dung
    int id; // id cua san pham
    bool InOut = true;
    while(InOut) {
        cout << "\t\t\t THE GIOI CONG NGHE \t\t\t";
        cout << "\n\n";
        cout << "1. Login. \n";
        cout << "2. Register. \n";
        cout << "0. Exit. \n";
        cout << "Enter choice: ";
        cin >> option;

        bool tinhNang = true;
        switch(option) {
            case 1:
            {
                int dangNhap = customers->login(customers);
                if (dangNhap == 2) {
                    // day la admin
                    while(tinhNang) {
                        cout << endl;
                        cout << "1. Display customers. \n";
                        cout << "2. Display orders. \n";
                        cout << "3. Add product. \n";
                        cout << "4. Delete product. \n";
                        cout << "5. Edit product. \n";
                        cout << "6. Statistics. \n";
                        cout << "7. Daily Statistics. \n";
                        cout << "8. Search customer. \n";
                        cout << "9. Search products. \n";
                        cout << "10. Generate discount code. \n";
                        cout << "11. Create index file. \n";
                        cout << "0. Logout. \n";
                        cout << "Enter choice: ";
                        int choice; cin >> choice;

                        switch(choice) {
                            case 1: 
                            {
                                customers->displayCustomer();
                            }
                                break;
                            case 2:
                            {
                                orders->displayOrder();
                            }
                                break;
                            case 3:
                            {
                                // admin->addProduct();
                                // int lineDelete;
                                // cout << "Enter the line delete: ";
                                // cin >> lineDelete;
                                // fileManager->deleteLineFromFile("D:\\PBL2_MYSTORE\\text\\DisplayDevice\\imagePaths1.txt", lineDelete - 1);
                                int count = 0; 
                                int *arr = fileManager->loadIDFromFile("D:\\PBL2_MYSTORE\\text\\DisplayDevice\\ProductID1.txt", count);
                                for (int i =0; i < count; i++) {
                                    cout << arr[i] << endl;
                                }
                                break;
                            }
                            case 4:
                                int productID;
                                cout << "Enter the product ID: ";
                                cin >> productID;
                                admin->deleteProduct(productID);
                                break;
                            case 5:
                            {
                                int productID;
                                cout << "Enter product ID: ";
                                cin >> productID;
                                // admin->editProduct(productID);
                                break;
                            }
                            case 6:
                            {
                               const int numberOfStatisticalDays = 10; // Số ngày thống kê
                                string recentDay[numberOfStatisticalDays];
                                time_t now = time(nullptr);

                                for (int i = 0; i < numberOfStatisticalDays; i++) {
                                    // Tính thời gian của ngày trước đó
                                    time_t timeBefore = now - i * 24 * 60 * 60;
                                    
                                    // Sử dụng localtime_s để lấy thông tin thời gian
                                    tm date;
                                    localtime_s(&date, &timeBefore);

                                    // Định dạng ngày thành chuỗi 
                                    ostringstream oss;
                                    oss << put_time(&date, "%d-%m-%Y");
                                    recentDay[i] = oss.str();
                                }
                                for (int i = 0; i < numberOfStatisticalDays; i++) {
                                    cout << orders->statistic(recentDay[i]) << endl;
                                }
                                break;
                            }
                            case 7:
                            {
                                // orders->dailyStatistics();
                                // string date;
                                // cout << "Enter date: "; cin >> date;
                                // int countPay = 0;
                                // History *lichSuDonHang = nullptr;
                                // double a = orders->statistic(date, lichSuDonHang, countPay);
                                // for (int i = 0; i < countPay; i++) {
                                //     cout << lichSuDonHang[i].getPriceTotal() << endl;
                                // }
                                // cout << a << endl;
                                break;
                            }
                            case 8:
                            {
                                cin.ignore();
                                string tenKhachHang;
                                cout << "Enter name customer: ";
                                getline(cin, tenKhachHang);
                                int fountCount = 0;
                                Customer *result = Customer::searchNameCustomer(tenKhachHang, fountCount);
                                cout << fountCount << endl;
                                if (result != nullptr) {
                                    for (int i = 0; i < fountCount; i++) {
                                        cout << "Found Customer: " << result[i].getFullName() << endl;
                                    }
                                    delete[] result;
                                } else {
                                    cout << "No customers found. \n";
                                }
                                break;
                            }
                            case 9:
                            {
                                cin.ignore();
                                string tenSanPham;
                                cout << "Enter name product: ";
                                getline(cin ,tenSanPham);
                                int foundCount = 0;
                                Products *result = Products::searchNameProducts(tenSanPham, foundCount);
                                cout << foundCount << endl;
                                if (result != nullptr) {
                                    for (int i = 0; i < foundCount; i++) {
                                        cout << "Found customer: " << result[i].getNameProduct() << endl;
                                   }
                                   delete [] result;
                                } else {
                                    cout << "Not found products .\n";
                                }
                                break;
                            }
                            case 10:
                            {
                                string discountCode;
                                double price;
                                int quanityDiscountCode;
                                admin->generateDiscountCode(discountCode, price, quanityDiscountCode);
                                break;
                            }
                            case 11:
                            {
                                // fileManager->createIndexFile("D:\\PBL2_MYSTORE\\text\\ThongSoKyThuat.txt", "D:\\PBL2_MYSTORE\\text\\indexFileProduct.txt");
                                // int productID;
                                // cout << "Enter productID to find specification: "; cin >> productID;
                                // products->getSpecificationByID("D:\\PBL2_MYSTORE\\text\\ThongSoKyThuat.txt", "D:\\PBL2_MYSTORE\\text\\indexFileProduct.txt", productID);
                                int lineCount1, lineCount2, lineCount3, lineCount4;
                                int *screenProduct1 = fileManager->loadLineNumbersFromFile("D:\\PBL2_MYSTORE\\text\\DisplayDevice\\screenProduct1.txt", lineCount1);
                                int *screenSpecification1 = fileManager->loadLineNumbersFromFile("D:\\PBL2_MYSTORE\\text\\DisplayDevice\\specificationProduct1.txt", lineCount2);
                                int *screenProduct2 = fileManager->loadLineNumbersFromFile("D:\\PBL2_MYSTORE\\text\\DisplayDevice\\screenProduct2.txt", lineCount3);
                                int *screenSpecification2 = fileManager->loadLineNumbersFromFile("D:\\PBL2_MYSTORE\\text\\DisplayDevice\\specificationProduct2.txt", lineCount4);
                                for (int i = 0; i < lineCount1; i++) {
                                    cout << screenProduct1[i] << " ";
                                }
                                cout << endl;
                                for (int i = 0; i < lineCount2; i++) {
                                    cout << screenSpecification1[i] << " ";
                                }
                                cout << endl;
                                for (int i = 0; i < lineCount3; i++) {
                                    cout << screenProduct2[i] << " ";
                                }
                                cout << endl;
                                for (int i = 0; i < lineCount4; i++) {
                                    cout << screenSpecification2[i] << " ";
                                }
                                cout << endl;
                                break;
                            }
                            case 0:
                            {
                                cout << "Logout successfully. \n";
                                tinhNang = false;
                            }
                                break;
                            default:
                                cout << "Invalid choice !\n";
                                break;
                        }
                    }
                }
                else if (dangNhap == 1) {
                    // Nếu là khách hàng 
                    orders->setID(customers->getCustomerID());
                    orders->update();
                    while(tinhNang) {
                        cout << endl;
                        cout << "1. Update info. \n";
                        cout << "2. Display Order. \n";
                        cout << "3. Change password. \n";
                        cout << "4. Add to cart. \n";
                        cout << "5. Display specification. \n";
                        cout << "6. Display products. \n";
                        cout << "7. Forget password. \n";
                        cout << "8. Increase Count. \n";
                        cout << "9. Decrease Count. \n";
                        cout << "10. Display history pay.\n";
                        cout << "0. Logout. \n";
                        cout << "Enter choice: ";
                        int choice; cin >> choice;

                        switch(choice) {
                            case 1:
                                customers->updateCustomerInfo();
                                break;
                            case 2:
                                orders->displayOrder();
                                orders->pay();
                                break;
                            case 3:
                                customers->changePassword();
                                break;
                            case 4:
                                do{
                                    product->displayProduct();
                                    cout << "Nhap so ID san pham ban muon mua: ";
                                    cin >> id;
                                    orders->addData(id);

                                }while (id != 0);
                                break;
                            case 5:
                                int productID;
                                cout << "Which product do you want to see the technical specifications of ? \n";
                                cout << "Enter choice: ";
                                cin >> productID;
                                cout << products->displaySpecification(productID);
                                break;
                            case 6:
                                products->displayProducts();
                                break;
                            case 7:
                            {
                                string phoneInput;
                                cout << "Enter your phone: ";
                                cin >> phoneInput;
                                // customers->forgetPassword(phoneInput);
                                break;
                            }
                            case 8:
                            {
                                int productID;
                                cout << "ENter id product: "; cin >> productID;
                                orders->increaseProductCount(productID);
                            break;
                            }
                            case 9:
                            {
                                 int productID;
                                cout << "ENter id product: "; cin >> productID;
                                orders->decreaseProductCount(productID);
                                break;
                            }
                            case 10:
                            {
                                int k;
                                Products *data;
                                int count = 0;
                                orders->displayHistory();
                                do {
                                    cout << "1. Display detail history. \n";
                                    cout << "0. Exit. \n";
                                    cout << "Enter choice: ";
                                    cin >> k;
                                    if (k == 1) {
                                        cout << "Enter order code: ";
                                        cin >> k;
                                        orders->displayDetailsHistory(k, data, count);
                                    }
                                } while(k != 0);
                                cout << count << endl;
                                break;
                            }
                            case 0:
                                cout << "Logout successfully.\n";
                                tinhNang = false;
                                break;
                            default:
                                cout << "Invalid choice !\n";
                                break;
                        }
                    }
                } else {
                    cout << "Account not found. \n";
                }
            }
                break;
            case 2:
            {
                customers->registerAccount();
                break;
            }

            case 0:
            {
                cout << "Bye bye.\n";
                InOut = false;
                break;
            }
            default:
                cout << "Invalid option. \n";
                break;
        }
    }

    cout << customers->getCustomerID() << endl;
    cout << orders->getCount() << endl;
    cout << orders->getCountP() << endl;
    delete orders;
    delete admin;
    return 0;
}