#include "D:\PBL2_MYSTORE\include\FileManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

FileManager::FileManager() {}
FileManager::~FileManager() {}

Customer* FileManager::loadCustomer(const std::string& fileCustomer, int &countCustomer)
{
    std::ifstream file(fileCustomer);
    Customer* customers = NULL;
    countCustomer = 0;

    if (file.is_open()) {
        std::string line;
        // Đếm số lượng khách hàng
        while(getline(file, line)) {
            ++countCustomer;
        }

        // Đếm xong và quay lại đầu file và cấp phát mảng và ghi dữ liệu vào mảng
        file.clear();
        file.seekg(0, std::ios::beg); // seekg dùng để di chuyển con trỏ về vị trí 0 đầu file
        customers = new Customer[countCustomer];
        int idx = 0;

        while(getline(file, line)) {
            std::istringstream iss(line);
            std::string customerID_str, lastName, middleName, firstName, email, phone, userName, password;

            // Tách các thành phần trong dòng bằng dấu |
            std::getline(iss, customerID_str, '|');
            std::getline(iss, lastName, '|');
            std::getline(iss, middleName, '|');
            std::getline(iss, firstName, '|');
            std::getline(iss, email, '|');
            std::getline(iss, phone, '|');
            std::getline(iss, userName, '|');
            std::getline(iss, password, '|');

            // Chuyển ID từ chuỗi thành số nguyên
            int customerID = std::stoi(customerID_str);

            // Tạo Customer và lưu vào mảng
            customers[idx++] = Customer(customerID, lastName, middleName, firstName, email, phone, userName, password);
        }
    }
    return customers;
}
void FileManager::saveCustomer(const std::string& fileCustomer, Customer& customer)
{
    std::ofstream file(fileCustomer, std::ios::app);
    if (file.is_open()) {
        file << customer.getCustomerID() << '|'
             << customer.getLastName() << '|'
             << customer.getMiddleName() << '|'
             << customer.getFirstName() << '|'
             << customer.getEmail() << '|'
             << customer.getPhone() << '|'
             << customer.getUserName() << '|'
             << customer.getPassword() << std::endl;
    }
}

// Hàm nạp danh sách sản phẩm vào trong mảng
Products* FileManager::loadSpecifications(const string &fileThongSoKyThuat, int &countProducts)
{
    ifstream file(fileThongSoKyThuat);
    if (!file.is_open()) {
        cerr << "Cannot open file!" << endl;
        return NULL;
    }

    Products *products = NULL;
    string line;
    countProducts = 0;
    bool isProductStarted = false;
    string productData;

    // Đọc số lượng sản phẩm trước
    while (getline(file, line)) {
        if (line == "*") {
            if (!isProductStarted) {
                // Bắt đầu sản phẩm mới
                isProductStarted = true;
                countProducts++;
            } else {
                // Kết thúc sản phẩm hiện tại
                isProductStarted = false;
            }
        }
    }

    // Cấp phát mảng sản phẩm
    file.clear();
    file.seekg(0, ios::beg); // Quay lại đầu file
    products = new Products [countProducts];
    int idx = 0;
    isProductStarted = false;

    while (getline(file, line)) {
        if (line == "*") {
            if (isProductStarted) {
                // Đã đọc xong một sản phẩm, xử lý dữ liệu sản phẩm
                istringstream in(productData);
                string productID_str, nameProduct, genre, priceProduct_str, manufacturer, operatingSystem, specifications, counts;

                // Đảm bảo tách đúng các phần của sản phẩm
                if (getline(in, productID_str, '|') &&
                    getline(in, specifications, '|')) {

                    // Chuyển đổi kiểu dữ liệu
                    try {
                        int productID = stoi(productID_str);
                        // Gán dữ liệu cho sản phẩm
                        products[idx++] = Products(productID, specifications);
                    } catch (const invalid_argument& e) {
                        cerr << "Error converting data: " << e.what() << endl;
                    }
                }

                // Reset dữ liệu cho sản phẩm tiếp theo
                productData.clear();
                isProductStarted = false;
            } else {
                // Bắt đầu sản phẩm mới
                isProductStarted = true;
            }
        } else if (isProductStarted) {
            // Thêm dòng dữ liệu vào sản phẩm
            productData += line + "\n";
        }
    }
    return products;
}

Products* FileManager::loadProducts(const std::string& fileProducts, int &countProducts)
{
    std::ifstream file(fileProducts);
    Products *products = NULL;
    countProducts = 0;

    if (file.is_open()) {
        std::string line;
        // Đếm số lượng khách hàng
        while(getline(file, line)) {
            ++countProducts;
        }

        // Đếm xong và quay lại đầu file và cấp phát mảng và ghi dữ liệu vào mảng
        file.clear();
        file.seekg(0, std::ios::beg); // seekg dùng để di chuyển con trỏ về vị trí 0 đầu file
        products = new Products[countProducts];
        int idx = 0;

        while(getline(file, line)) {
            std::istringstream iss(line);
            std::string productID_str, nameProduct, genre, priceProduct_str, manufacturer, operatingSystem, count_str;

            // Tách các thành phần trong dòng bằng dấu |
            std::getline(iss, productID_str, '|');
            std::getline(iss, nameProduct, '|');
            std::getline(iss, genre, '|');
            std::getline(iss, priceProduct_str, '|');
            std::getline(iss, manufacturer, '|');
            std::getline(iss, operatingSystem, '|');
            std::getline(iss, count_str);
           
           try {
            // Chuyển ID từ chuỗi thành số nguyên
            int productID = std::stoi(productID_str);
            int count = std::stoi(count_str);
            double priceProduct = std::stod(priceProduct_str);
            // Tạo Customer và lưu vào mảng
            if (idx < countProducts) 
                products[idx++] = Products(productID, nameProduct, genre, priceProduct, manufacturer, operatingSystem, count);
           } catch(const invalid_argument &e) {
                cerr << "Invalid data in file " << e.what() << endl;
           } catch(const out_of_range &e) {
                cerr << "Number out of range in file: " << e.what() << endl;
           }
        }
    }
    return products;
}

void FileManager::saveProducts(const string fileProducts, Products* products, int& countProduct){
    ofstream outFile(fileProducts);
    if (outFile.is_open()) {
        for(int i = 0; i < countProduct; i++){
            outFile << products[i].getProductID() << "|"
                << products[i].getNameProduct() << "|"
                << products[i].getGenre() << "|"
                << products[i].getPriceProduct() << "|"
                << products[i].getManufacturer() << "|"
                << products[i].getOperatingSystem() << "|"
                << products[i].getCount() << endl;
        }
        outFile.close();
    }
    else {
        cout << "Unable to open file " << fileProducts << endl;
    }
}

Products* FileManager::loadOrder(const string &fileOrder, int& countOrder)
{
    ifstream inFile(fileOrder);
    countOrder = 0;
    Products* orders = nullptr; // Sử dụng nullptr thay cho NULL

    if (!inFile.is_open()) {
        cout << "Could not open the file. \n";
        return nullptr;
    }

    string line;
    while (getline(inFile, line)) {
        ++countOrder;
    }
    inFile.clear();
    inFile.seekg(0, ios::beg);

    orders = new (std::nothrow) Products[7]; // Sử dụng std::nothrow để tránh ngoại lệ
    if (orders == nullptr) {
        std::cerr << "Memory allocation failed." << std::endl;
        return nullptr; // Trả về nullptr nếu cấp phát thất bại
    }

    int index = 0;
    while (getline(inFile, line)) {
        istringstream iss(line);
        string productID_str, nameProduct, genre, priceProduct_str, manufacturer, operatingSystem, count_str;

        getline(iss, productID_str, '|');
        getline(iss, nameProduct, '|');
        getline(iss, genre, '|');
        getline(iss, priceProduct_str, '|');
        getline(iss, manufacturer, '|');
        getline(iss, operatingSystem, '|');
        getline(iss, count_str);
        
        try{
            int productID = stoi(productID_str);
            double priceProduct = stod(priceProduct_str);
            int count = stoi(count_str);
            
            // Đảm bảo chỉ số không vượt quá countOrder
            if (index < countOrder) {
                orders[index++] = Products(productID, nameProduct, genre, priceProduct, manufacturer, operatingSystem, count);
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid data in file: " << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Number out of range in file: " << e.what() << std::endl;
        }
        
    }
    return orders; 
}
void FileManager::saveOrder(const string& filename, Products* data, int& count){
    ofstream outFile(filename, ios::out);
    if (outFile.is_open()) {
        for (int i = 0; i < count; i++) {
            // outFile << i + 1
            outFile << data[i].getProductID() << "|"
                    << data[i].getNameProduct() << "|"
                    << data[i].getGenre() << "|"
                    << data[i].getPriceProduct() << "|"
                    << data[i].getManufacturer() << "|"
                    << data[i].getOperatingSystem() << "|"
                    << data[i].getCount() << endl;
        }
        outFile.close();
    }
    else {
        cout << "Unable to open file " << filename << endl;
    }
}
int FileManager::loadOrderCode(const string& filename){
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot open file!" << endl;
        return 0;
    }
    string line;
    getline(file, line);
    // try {
        int orderCode = stoi(line);
    // } catch (const invalid_argument& e) {
    //     cerr << "Error: Invalid data format in file. " << e.what() << endl;
    // } catch (const out_of_range& e) {
    //     cerr << "Error: Number out of range in file. " << e.what() << endl;
    // }
    return orderCode;
}

void FileManager::saveOrderCoder(const string& filename, long& orderCode){
    ofstream outFile(filename, ios::out);
    if (outFile.is_open()) {
        outFile << orderCode;
        outFile.close();
    }
    else {
        cout << "Unable to open file " << filename << endl;
    }
}

string FileManager::getCurrentDate()
{
    time_t now = time(nullptr);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y", localtime(&now));
    return string(buffer);
}

History* FileManager::loadPay(const string& filename, int& countPay, int& customer_ID) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot open file!" << endl;
        return nullptr;
    }

    string line;
    countPay = 0;
    History* tempHistory = new History[100];
    int idx = 0;

    while (getline(file, line)) {
        if (line == "*") {  // Bắt đầu một khối mới
            string currentDate, customerIDStr, orderCodeStr, addressOrder, totalPriceStr;

            // Đọc dòng thông tin giao dịch của khối
            if (getline(file, line) && !line.empty()) {
                istringstream iss(line);

                // Đọc từng phần dữ liệu và kiểm tra dòng không rỗng
                if (getline(iss, currentDate, '|') && !currentDate.empty() &&
                    getline(iss, customerIDStr, '|') && !customerIDStr.empty() &&
                    getline(iss, orderCodeStr, '|') && !orderCodeStr.empty() &&
                    getline(iss, addressOrder, '|') && !addressOrder.empty() &&
                    getline(iss, totalPriceStr) && !totalPriceStr.empty()) {

                    try {
                        int customerID = stoi(customerIDStr);
                        long orderCode = stol(orderCodeStr);
                        double totalPrice = stod(totalPriceStr);
                        string items;
                        while (getline(file, line) && line != "*") {  // Đọc các dòng sản phẩm
                            items += line + "\n";
                        }
                        if (customerID == customer_ID) {
                            tempHistory[idx++] = History(currentDate, customerID, orderCode, addressOrder, totalPrice, items);
                            countPay++;
                        }
                    } catch (const invalid_argument& e) {
                        cerr << "Error: Invalid data format in file. " << e.what() << endl;
                        continue;
                    } catch (const out_of_range& e) {
                        cerr << "Error: Number out of range in file. " << e.what() << endl;
                        continue;
                    }
                }
            }
        }
    }

    // Tạo mảng chính xác để trả về kết quả
    History* resultArray = new History[countPay];
    for (int i = 0; i < countPay; ++i) {
        resultArray[i] = tempHistory[i];
    }

    delete[] tempHistory;  // Giải phóng mảng tạm thời
    return resultArray;
}

History* FileManager::loadPay(const string& filename, const string& currdate, int &countPay, double& total_Price){
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot open file!" << endl;
        return nullptr;
    }

    string line;
    countPay = 0;
    total_Price = 0.0;
    History* tempHistory = new History[100];
    int idx = 0;

    while (getline(file, line)) {
        if (line == "*") {  // Bắt đầu một khối mới
            string currentDate, customerIDStr, orderCodeStr, addressOrder, totalPriceStr;

            // Đọc dòng thông tin giao dịch của khối
            if (getline(file, line) && !line.empty()) {
                istringstream iss(line);

                // Đọc từng phần dữ liệu và kiểm tra dòng không rỗng
                if (getline(iss, currentDate, '|') && !currentDate.empty() &&
                    getline(iss, customerIDStr, '|') && !customerIDStr.empty() &&
                    getline(iss, orderCodeStr, '|') && !orderCodeStr.empty() &&
                    getline(iss, addressOrder, '|') && !addressOrder.empty() &&
                    getline(iss, totalPriceStr) && !totalPriceStr.empty()) {

                    try {
                        int customerID = stoi(customerIDStr);
                        long orderCode = stol(orderCodeStr);
                        double totalPrice = stod(totalPriceStr);
                        string items;
                        while (getline(file, line) && line != "*") {  // Đọc các dòng sản phẩm
                            items += line + "\n";
                        }
                        if (currdate == currentDate) {
                            total_Price += totalPrice;
                            tempHistory[idx++] = History(currentDate, customerID, orderCode, addressOrder, totalPrice, items);
                            countPay++;
                        }
                    } catch (const invalid_argument& e) {
                        cerr << "Error: Invalid data format in file. " << e.what() << endl;
                        continue;
                    } catch (const out_of_range& e) {
                        cerr << "Error: Number out of range in file. " << e.what() << endl;
                        continue;
                    }
                }
            }
        }
    }

    // Tạo mảng chính xác để trả về kết quả
    History* resultArray = new History[countPay];
    for (int i = 0; i < countPay; ++i) {
        resultArray[i] = tempHistory[i];
    }

    delete[] tempHistory;  // Giải phóng mảng tạm thời
    return resultArray;
}

void FileManager::savePay(const string& filename, Products* pay, int& count, int customerID, string addressOrder, long orderCode, double &priceDiscount)
{
    ofstream outFile(filename, ios::app);
    if (outFile.is_open()) {
        string currentDate = getCurrentDate();
        outFile << "*" << endl;
        outFile << currentDate << "|";
        outFile << customerID << "|";
        outFile << orderCode << "|";
        outFile << addressOrder << "|";
        double totalPrice = -priceDiscount;
        for (int i = 0; i < count; i++) {
            totalPrice += pay[i].getPriceProduct();
        }
        outFile << totalPrice << endl;
        for (int i = 0; i < count; i++) {
            totalPrice += pay[i].getPriceProduct();
            outFile << pay[i].getProductID() << "|"
                    << pay[i].getNameProduct() << "|"
                    << pay[i].getGenre() << "|"
                    << pay[i].getPriceProduct() << "|"
                    << pay[i].getManufacturer() << "|"
                    << pay[i].getOperatingSystem() << "|"
                    << pay[i].getCount() << endl;
        }
        outFile << "*" << endl;
        outFile.close();
    }
    else {
        cout << "Unable to open file " << filename << endl;
    }
}

void FileManager::saveDiscount(const string& filename, Discount* discount, int& count){
    ofstream outFile(filename, ios:: out);
    if (outFile.is_open()) {
        for (int i = 0; i < count; i++) {
            outFile << discount[i].getDisCountCode() << "|"
                    << discount[i].getPrice() << "|"
                    << discount[i].getAmount() << endl;
        }
        outFile.close();
    }
    else {
        cout << "Unable to open file " << filename << endl;
    }
}

Discount* FileManager::loadDiscount(const string& filename, int& count){
    std::ifstream file(filename);
    Discount* discount = NULL;
    count = 0;

    if (file.is_open()) {
        std::string line;
        // Đếm số lượng khách hàng
        while(getline(file, line)) {
            ++count;
        }

        // Đếm xong và quay lại đầu file và cấp phát mảng và ghi dữ liệu vào mảng
        file.clear();
        file.seekg(0, std::ios::beg); // seekg dùng để di chuyển con trỏ về vị trí 0 đầu file
        discount = new Discount[count];
        int idx = 0;

        while(getline(file, line)) {
            std::istringstream iss(line);
            std::string discountCode, priceD, count_str;

            // Tách các thành phần trong dòng bằng dấu |
            std::getline(iss, discountCode, '|');
            std::getline(iss, priceD, '|');
            std::getline(iss, count_str);
           
            // Chuyển ID từ chuỗi thành số nguyên
            int count = std::stoi(count_str);
            double price = std::stod(priceD);
            // Tạo Customer và lưu vào mảng
            discount[idx++] = Discount(discountCode, price, count);
        }
    }
    return discount;
}

string FileManager::readTextTinhNangNoiBat(const string& filePath)
{
    ifstream file(filePath);
    string content;

    // Đọc toàn bộ nội dung file
    string line;
    while(getline(file, line)) {
        content += line + "\n";
    }
    file.close();
    return content;
}

string* FileManager::readFromFile(const string &filePath, int &count)
{
    count = 0;
    // Mở file để đếm số dòng
    ifstream file(filePath);
    if (!file.is_open()) {
        return nullptr;
    }
    
    string line;

    // Đếm số dòng
    while(getline(file, line)) {
        if (!line.empty()) {
            count++;
        }
    }

    // Cấp phát mảng động để chứa các dòng trong file
    string *arr = new string[count];

    // Đọc lại file và lưu vào mảng
    file.clear();
    file.seekg(0, ios::beg);

    int index = 0;
    while(getline(file, line)) {
        if (!line.empty()) {
            // Thay thế tất cả các ký tự "|\\n" thành '\n'
            size_t pos = 0;
            while ((pos = line.find("\\n", pos)) != string::npos) {
                line.replace(pos, 2, "\n");  // Thay thế "|\\n" với '\n'
            }
            arr[index++] = line;
        }
    }
    file.close();
    return arr;
}

vector<pair<int, string>> FileManager::readFromFile(const string &fileName)
{
    vector<pair<int, string>> productInfo;
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "ERROR: Cann't open file for reading. \n";
        return productInfo;
    }

    string line;
    while(getline(file, line)) {
        istringstream iss(line);
        string productID_str, pathPicture;

        // Tách ID và đường dẫn hình ảnh bằng kí tự '|'
        if (getline(iss, productID_str, '|') && getline(iss, pathPicture, '|')) {
            int productID = stoi(productID_str);
            productInfo.emplace_back(productID, pathPicture);
        }
    }
    file.close();
    return productInfo;
}

// Xóa 1 dòng trong file
void FileManager::deleteLineFromFile(const string &filePath, int lineToDelete)
{
    ifstream fileIn(filePath);
    if (!fileIn.is_open()) {
        cout << "ERROR: Cann't open file. \n";
        return;
    }

    const int MAX_LINES = 20;
    string lines[MAX_LINES];
    int totalLines = 0;

    // Đọc file vào mảng
    while(getline(fileIn, lines[totalLines]) && totalLines < MAX_LINES) {
        totalLines++;
    }
    fileIn.close();
    
    if (lineToDelete < 0 || lineToDelete >= totalLines) {
        cout << "ERROR: Invalid line number to delete: " << lineToDelete << endl;
        return;
    }
    // Xóa dòng cần xóa bằng cách dịch các phần tử trong mảng
    for (int i = lineToDelete; i < totalLines - 1; i++) {
        lines[i] = lines[i + 1];
    }
    totalLines--;

    // Ghi lại mảng vào file
    ofstream fileOut(filePath, ios::trunc); // Ghi đè
    if (!fileOut.is_open()) {
        cout << "ERROR: Cann't open file. \n";
        return;
    }
    for (int i = 0; i < totalLines; i++) {
        fileOut << lines[i] << "\n";
    }
    fileOut.close();

    cout << "Da xoa thanh cong san pham ra khoi file. \n";
}

// Tạo file chỉ mục 
void FileManager::createIndexFile(const string &productFileName, const string &indexFileName)
{
    ifstream productFile(productFileName);
    ofstream indexFile(indexFileName);

    if (!productFile.is_open()) {
        cout << "ERROR: Can't open product file: " << productFileName << endl;
        return;
    }

    if (!indexFile.is_open()) {
        cout << "ERROR: Can't open index file: " << indexFileName << endl;
        return;
    }

    string line, productID;
    streampos idPosition;

    while (getline(productFile, line)) {
        // Xác định dấu "*" để bắt đầu 1 khối sản phẩm mới
        if (line == "*") {
            // Đọc dòng tiếp theo để lấy ID sản phẩm
            if (getline(productFile, line)) {
                size_t pos = line.find("|");
                if (pos != string::npos) {
                    productID = line.substr(0, pos); // Lấy ID sản phẩm trước dấu "|"

                    // Lưu vị trí byte ngay sau dòng ID sản phẩm, tức là dòng kế tiếp
                    idPosition = productFile.tellg();  // Vị trí byte sau khi đọc ID sản phẩm
                    cout << "Position for ID " << productID << ": " << idPosition << endl;

                    // Ghi vào file chỉ mục
                    indexFile << productID << "|" << idPosition << endl;

                    // Bỏ qua các dòng thông số cho đến khi gặp dấu "*" thứ hai của khối sản phẩm
                    while (getline(productFile, line) && line != "*") {
                        // Không cần xử lý, chỉ tiếp tục đọc
                    }
                } else {
                    cout << "ERROR: Invalid product ID format." << endl;
                    return;
                }
            } else {
                cout << "ERROR: Failed to read product ID." << endl;
                return;
            }
        }
    }

    productFile.close();
    indexFile.close();

    cout << "File chỉ mục được tạo thành công.\n";
}

int* FileManager::loadLineNumbersFromFile(const string &filePath, int &lineCount) {
    static int lastNumber = 21; // Biến tĩnh lưu trạng thái số cuối cùng

    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Không thể mở file: " << filePath << endl;
        return nullptr;
    }

    // Đếm số dòng trong file
    lineCount = 0;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            lineCount++;
        }
    }

    // Kiểm tra file có dòng hợp lệ không
    if (lineCount == 0) {
        file.close();
        return nullptr;
    }

    // Cấp phát mảng động để chứa số thứ tự dòng
    int* lineNumbers = new int[lineCount];
    file.clear();
    file.seekg(0, ios::beg);  // Đưa con trỏ về đầu file

    // Lưu số thứ tự dòng với giá trị bắt đầu từ `lastNumber + 1`
    int index = 0;
    for (int i = 0; i < lineCount; i++) {
        lineNumbers[i] = lastNumber + i + 1;
    }

    // Cập nhật biến tĩnh
    lastNumber += lineCount;

    file.close();
    return lineNumbers;
}

// Hàm đọc id từ  file
int * FileManager::loadIDFromFile(const string &filePath, int &count)
{
    ifstream inFile(filePath);
    if (!inFile.is_open()) {
        cout << "ERROR: Cann't open file. \n";
        return NULL;
    }

    // Đếm số dòng trong file
    count = 0;
    string line;
    while(getline(inFile, line)) {
        if (!line.empty()) {
            count++;
        }
    }

    // Kiểm tra xem file có dòng hợp lệ không
    if (count == 0) {
        inFile.close();
        return NULL;
    }

    // Cấp phát mảng đọng để chứa ID sản phẩm
    int *productID = new int [count];
    inFile.clear();
    inFile.seekg(0, ios::beg);

    int index = 0;
    while (getline(inFile, line)) {
        if (!line.empty()) {
            try {
                productID[index++] = stoi(line);
            } catch (const invalid_argument &e) {
                cout << "ERROR: Invalid ID format in file.\n";
                delete[] productID;
                count = 0;
                inFile.close();
                return nullptr;
            }
        }
    }
    inFile.close();
    return productID;
}
