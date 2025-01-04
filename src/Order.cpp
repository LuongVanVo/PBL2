#include "D:\PBL2_MYSTORE\include\Order.h"
#include "D:\PBL2_MYSTORE\include\Products.h"
#include "D:\PBL2_MYSTORE\include\Customer.h"
#include "D:\PBL2_MYSTORE\include\FileManager.h"

using namespace std;

int Order::countProduct = 0;

Products* Order::products = new Products[0];


int Order::getCountP(){
    return countProduct;
}

int Order::getCount()
{
    return count;
}

Products *Order::getData() const
{
    return data;
}

int Order::customerID = 0;
long Order::orderCode = 100000;

void Order::setID(int ID){
    this->customerID = ID;
}

Order::Order(){
    this->count = 0;
    data = new Products[maxSize];
}

Order::Order(const Order& other) {
    this->count = other.count;
    data = new Products[maxSize]; // Tạo mảng mới
    for (int i = 0; i < count; i++) {
        data[i] = other.data[i]; // Sao chép từng phần tử
    }
}

Order::~Order() {
    delete[] data; // phải phóng bộ nhớ
}

Order& Order::operator = (const Order& other) {
    if (this != &other) { // Kiểm tra tự gán        
        count = other.count;

        data = new Products[maxSize]; // Cấp phát lại bộ nhớ
        for (int i = 0; i < count; i++) {
            data[i] = other.data[i]; // Sao chép dữ liệu
        }
    }
    return *this;
}

void Order::update(){
    string nameFile = to_string(customerID) + "_order.txt";
    string nameFileOrder = "text\\Order\\" + nameFile;
    data = FileManager::loadOrder(nameFileOrder, count);
}

void Order::displayProduct(){
    if(countProduct == 0){
        products = FileManager::loadProducts("text\\Products.txt", countProduct);
    }
    cout << setw(3) << "STT" << setw(13) << "ID" << setw(30) << "Name Product" << setw(15) << "Genre" << setw(25) << "Manufacturer"
         << setw(25) << "Operating System" << setw(10) << "Price" << setw(10) << "Count" << endl;
    for(int i = 0; i < countProduct; i++){
        cout << setw(3) << i + 1
            << setw(13) << products[i].getProductID()
            << setw(30) << products[i].getNameProduct()
            << setw(15) << products[i].getGenre()
            << setw(25) << products[i].getManufacturer()
            << setw(25) << products[i].getOperatingSystem()
            << setw(10) << products[i].getPriceProduct()
            << setw(10) << products[i].getCount() << endl;
    }
}

void Order::addData(int ID) {
    bool productExists = false;
    int found = 1;
    
    // Kiểm tra xem sản phẩm đã có trong giỏ hàng hay chưa
    for (int j = 0; j < count; j++) {
        if (data[j].getProductID() == ID) {
            data[j].setCount(data[j].getCount() + 1); // Tăng số lượng nếu sản phẩm đã có
            productExists = true;
            found = 2;
            break;
        }
    }

    // Nếu sản phẩm chưa có trong giỏ hàng và số lượng sản phẩm khác nhau chưa đạt giới hạn
    if (!productExists && count < maxSize) {
        for (int i = 0; i < countProduct; i++) {
            if (products[i].getProductID() == ID) {
                data[count].setProductID(products[i].getProductID());
                data[count].setNameProduct(products[i].getNameProduct());
                data[count].setGenre(products[i].getGenre());
                data[count].setManufacturer(products[i].getManufacturer());
                data[count].setOperatingSystem(products[i].getOperatingSystem());
                data[count].setPriceProduct(products[i].getPriceProduct());
                data[count].setCount(1);
                count++;
                found = 3;
                break;
            }
        }
    }
    else if (!productExists) {
        cout << "Cannot add more unique products to the cart.\n";
        return;
    }

    // Lưu đơn hàng vào file nếu giỏ hàng đã cập nhật
    if (found == 2 || found == 3) {
        string nameFile = to_string(customerID) + "_order.txt";
        string nameFileOrder = "text\\Order\\" + nameFile;
        FileManager::saveOrder(nameFileOrder, data, count);
    }
}

void Order::deleteData(int ID) {
    bool found = false;
    for (int i = 0; i < count; i++) {
        if (data[i].getProductID() == ID) {
            // Giảm count trước khi xóa để tránh lỗi tràn
            found = true;  
            products[i].setCount(products[i].getCount() + 1); // Cập nhật số lượng sản phẩm
            for (int j = i; j < count - 1; j++) {
                data[j] = data[j + 1]; // Dịch chuyển phần tử về bên trái
            }
            count--; // Giảm số lượng sản phẩm
            break; // Thoát khỏi vòng lặp khi đã tìm thấy sản phẩm để xóa
        }
    }
    
    if (found) {
        // Lưu trạng thái mới vào file
        string nameFile = to_string(customerID) + "_order.txt";
        string nameFileOrder = "text\\Order\\" + nameFile;
        FileManager::saveOrder(nameFileOrder, data, count);
    } else {
        cout << "Not found in order" << endl;
    }
}

void Order::displayOrder() 
{
    // Hiển thị danh sách sản phẩm trong giỏ hàng
    cout << "Order details:" << endl;
    cout << setw(3) << "STT" << setw(13) << "ID" << setw(30) << "Name Product" << setw(15) << "Genre" << setw(25) << "Manufacturer"
         << setw(25) << "Operating System" << setw(10) << "Price" << setw(10) << "Count" << endl;

    double sum = 0;
    for (int i = 0; i < count; i++) {
        sum += data[i].getCount() * data[i].getPriceProduct();
        cout << setw(3) << i + 1
             << setw(13) << data[i].getProductID()
             << setw(30) << data[i].getNameProduct()
             << setw(15) << data[i].getGenre()
             << setw(25) << data[i].getManufacturer()
             << setw(25) << data[i].getOperatingSystem()
             << setw(10) << data[i].getPriceProduct()
             << setw(10) << data[i].getCount() << endl;
    }
    cout << "Total Price: " << sum << endl;
}

int Order::findData(int& ID) const {
    for (int i = 0; i < count; i++) {
        if (data[i].getProductID() == ID) {
            return i; // Trả về vị trí của mặt hàng
        }
    }
    return -1; // Không tìm thấy
}

int Order::findData(const string& name) const {
    for (int i = 0; i < count; i++) {
        if (data[i].getNameProduct() == name) {
            return i; // Trả về vị trí của mặt hàng
        }
    }
    return -1; // Không tìm thấy
}

void Order::updateData(int& ID, const string& name, const string& genre, const string& manu, const string& oper, double newPrice, int count) {
    int index = findData(ID);
    if (index != -1) {
        data[index].setProductID(ID);
        data[index].setNameProduct(name);
        data[index].setGenre(genre);
        data[index].setPriceProduct(newPrice);
        data[index].setManufacturer(manu);
        data[index].setOperatingSystem(oper);
        data[index].setCount(count);
    }
    else {
        cout << "Item not found!" << endl;
    }
}

void Order::clearOrder() {
    if (data != nullptr) {  // Kiểm tra NULL trước khi xóa
        delete[] data; // giải phóng bộ nhớ củ
    }
    data = new Products[maxSize]; // Khởi tạo lại mảng mới
    count = 0;
}

void Order::payment(string &addressOrder, string discountCode, double &priceDiscount){
    int k;
    orderCode = FileManager::loadOrderCode("text\\orderCode.txt");
    if (countProduct == 0) {
        products = FileManager::loadProducts("text\\Products.txt", countProduct);
    }
    Products* pay = new Products[maxSize];
    int h = 0;
    for(int i = 0; i < count; i++){
        for(int j = 0; j < countProduct; j++){
            if(data[i].getProductID() == products[j].getProductID()){
                k = products[j].getCount() - data[i].getCount();
                if(k >= 0){
                    products[j].setCount(k);
                    pay[h++] = data[i];
                    count--;
                    for (int f = i; f < count; f++) {
                        data[f] = data[f + 1];
                    }
                    i--;
                }
                else{
                    cout << "Sorry, the product: " << products[j].getNameProduct() << " is out of stock." << endl;
                }
            }
        }
    }
    if (pay != nullptr) {
        priceDiscount = Discount::discount(discountCode);
        FileManager::savePay("text\\Pay.txt", pay, h, customerID, addressOrder, orderCode, priceDiscount);
        ++orderCode;
        FileManager::saveOrderCoder("text\\orderCode.txt", orderCode);
    }
    delete [] pay; 
    FileManager::saveProducts("text\\Products.txt", products, countProduct);
    string nameFile = to_string(customerID) + "_order.txt";
    string nameFileOrder = "text\\Order\\" + nameFile;
    FileManager::saveOrder(nameFileOrder,data, count);
}

void Order::displayHistory(){
    History* history = nullptr;
    int countPay;
    history = FileManager::loadPay("text\\Pay.txt", countPay, customerID);
    for(int i = countPay - 1; i >= 0; i--){
        cout << setw(10) << history[i].getCurrentDate()
            << setw(5) << history[i].getCustomerID()
            << setw(8) << history[i].getOrderCode()
            << setw(30) << history[i].getAddressOrder()
            << setw(10) << history[i].getPriceTotal() << endl;
    }
    delete [] history;
}

Products* tach(const string& items, int& index){
    Products* products = new Products [20];
    index = 0;
    // Tách từng dòng từ items
    istringstream iss(items);
    string line;
    while (getline(iss, line)) {  // Đọc từng dòng
        // Tách các trường trong dòng
        string productID_str, nameProduct, genre, priceProduct_str, manufacturer, operatingSystem, count_str;
        istringstream lineStream(line);
        getline(lineStream, productID_str, '|');
        getline(lineStream, nameProduct, '|');
        getline(lineStream, genre, '|');
        getline(lineStream, priceProduct_str, '|');
        getline(lineStream, manufacturer, '|');
        getline(lineStream, operatingSystem, '|');
        getline(lineStream, count_str);
        try{
            int productID = stoi(productID_str);
            double priceProduct = stod(priceProduct_str);
            int count = stoi(count_str);
            
            // Đảm bảo chỉ số không vượt quá countOrder
            
            products[index++] = Products(productID, nameProduct, genre, priceProduct, manufacturer, operatingSystem, count);

        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid data in file: " << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Number out of range in file: " << e.what() << std::endl;
        }
    }
    Products* product = new Products [index];
    for(int i = 0; i < index; i++){
        product[i] = products[i];
    }
    delete [] products;
    return product;
}

void Order::displayDetailsHistory(long orderCode, Products*& data, int &count) {
    History* history = nullptr;
    int countPay;
    history = FileManager::loadPay("text\\Pay.txt", countPay, customerID);
    bool found = false;
    for(int i = 0; i < countPay; i++){
            if(history[i].getOrderCode() == orderCode){
                found = true;
                count = i; // lấy chỉ số
                break;
            }
    }
    if(found == false){
        cout << "Not found.\n";
        delete [] history;
        return;
    }
    data = tach(history[count].getItem(), count);
    cout << "Order details:" << endl;
    cout << setw(3) << "STT" << setw(13) << "ID" << setw(30) << "Name Product" << setw(15) << "Genre" << setw(25) << "Manufacturer"
         << setw(25) << "Operating System" << setw(10) << "Price" << setw(10) << "Count" << endl;

    for (int i = 0; i < count; i++) {
        cout << setw(3) << i + 1
             << setw(13) << data[i].getProductID()
             << setw(30) << data[i].getNameProduct()
             << setw(15) << data[i].getGenre()
             << setw(25) << data[i].getManufacturer()
             << setw(25) << data[i].getOperatingSystem()
             << setw(10) << data[i].getPriceProduct()
             << setw(10) << data[i].getCount() << endl;
    }
    delete [] history;
}

double Order::statistic(const string& date){
    History* history = nullptr;
    double totalPrice;
    int countPay;
    history = FileManager::loadPay("text\\Pay.txt", date, countPay, totalPrice);    
    return totalPrice;
    delete [] history;
}

// Tăng số lượng sản phẩm trong giỏ hàng
void Order::increaseProductCount(int productID)
{
    for (int i = 0; i < count; i++) {
        if (data[i].getProductID() == productID) {
            data[i].setCount(data[i].getCount() + 1); // Tăng số lượng
            string nameFile = to_string(customerID) + "_order.txt";
            string nameFileOrder = "text\\Order\\" + nameFile;
            FileManager::saveOrder(nameFileOrder, data, count);
            break;
        }
    }
}

// Giảm số lượng sản phẩm trong giỏ hàng
void Order::decreaseProductCount(int productID) {
    for (int i = 0; i < count; i++) {
        if (data[i].getProductID() == productID) {
            data[i].setCount(data[i].getCount() - 1); // Giảm số lượng
            if (data[i].getCount() == 0) {
                deleteData(data[i].getProductID());
                break;
            } 
            string nameFile = to_string(customerID) + "_order.txt";
            string nameFileOrder = "text\\Order\\" + nameFile;
            FileManager::saveOrder(nameFileOrder, data, count);
            break;
        }
    }
}

void Order::setAddressOrder(string addressOrder)
{
    this->addressOrder = addressOrder;
}

string Order::getAddressOrder()
{
    return addressOrder;
}