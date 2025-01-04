#include "D:\PBL2_MYSTORE\include\Products.h"
#include "D:\PBL2_MYSTORE\include\FileManager.h"
#include <iomanip>

Products::Products(int productID, string nameProduct, string genre, double priceProduct, string manufacturer, string operatingSystem, int Count){
    this->productID = productID;
    this->nameProduct = nameProduct;
    this->genre = genre;
    this->priceProduct = priceProduct;
    this->manufacturer = manufacturer;
    this->operatingSystem = operatingSystem;
    this->Count = Count;
}
Products::Products(int productID, string specifications)
{
    this->productID = productID;
    this->specifications = specifications;
}
Products::Products(int productID, string nameProduct, string genre, double priceProduct, string manufacturer, string operatingSystem, string specifications, int Count){
    this->productID = productID;
    this->nameProduct = nameProduct;
    this->genre = genre;
    this->priceProduct = priceProduct;
    this->manufacturer = manufacturer;
    this->operatingSystem = operatingSystem;
    this->specifications = specifications;
    this->Count = Count;
}

Products::Products(){}
Products::~Products(){}


// getter
int Products::getProductID(){
    return productID;
}
string Products::getNameProduct(){
    return nameProduct;
}
string Products::getGenre(){
    return genre;
}
double Products::getPriceProduct(){
    return priceProduct;
}
string Products::getManufacturer(){
    return manufacturer;
}
string Products::getOperatingSystem(){
    return operatingSystem;
}
string Products::getSpecifications(){
    return specifications;
}
int Products::getCount(){
    return Count;
}


// setting
void Products::setProductID(int productID){
    this->productID = productID;
}
void Products::setNameProduct(string nameProduct){
    this->nameProduct = nameProduct;
}
void Products::setGenre(string genre){
    this->genre = genre;
}
void Products::setPriceProduct(double priceProduct){
    this->priceProduct = priceProduct;
}
void Products::setManufacturer(string manufacturer){
    this->manufacturer = manufacturer;
}
void Products::setOperatingSystem(string operatingSystem){
    this->operatingSystem = operatingSystem;
}
void Products::setCount(int count){
    this->Count = count;
}


ostream& operator<<(ostream& out, const Products& product) 
{
    out << product.productID << "|" 
            << product.nameProduct << "|"
            << product.genre << "|"
            << product.priceProduct << "|"
            << product.manufacturer << "|"
            << product.operatingSystem << "|"
            << product.Count;
    return out;
}

// Hiển thị thông tin của sản phẩm // Author: Vo
void Products::displayProducts()
{
    int productSize;
    Products *products = FileManager::loadProducts("D:\\PBL2_MYSTORE\\text\\Products.txt", productSize);

    if (products == NULL || productSize == 0) {
        cout << "No product available. " << endl;
        return;
    }

    cout << setw(3) << "STT" << setw(10) << "ID" << setw(25) << "Name Product" << setw(15) << "Genre" << setw(25) << "Manufacturer"
        << setw(25) << "Operating System" << setw(10) << "Price" << setw(10) << "Count" << endl;
    for(int i = 0; i < productSize; i++){
        cout << setw(3) << i + 1
            << setw(10) << products[i].getProductID()
            << setw(25) << products[i].getNameProduct()
            << setw(15) << products[i].getGenre()
            << setw(25) << products[i].getManufacturer()
            << setw(25) << products[i].getOperatingSystem()
            << setw(10) << products[i].getPriceProduct()
            << setw(10) << products[i].getCount() << endl;
    }
    delete [] products;
}

string Products::displaySpecification(int productID)
{
    bool check = false;
    int productSize;
    
    Products *products = FileManager::loadSpecifications("D:\\PBL2_MYSTORE\\text\\ThongSoKyThuat.txt", productSize);
    if (products == NULL || productSize == 0) {
        return "No product available. \n";
    }

    ostringstream specificationStream; 
    for (int i = 0; i < productSize; i++) {
        if (products[i].getProductID() == productID) {
            specificationStream << products[i].getSpecifications() << endl;
            check = true;
        }
    }
    if (!check) {
        specificationStream << "Products not found ! \n"; // Thông báo nếu không tìm thấy sản phẩm
    }
    delete [] products;

    return specificationStream.str();
}

void Products::swapProducts(Products *product1, Products *product2)
{
    Products tmp = *product1;
    *product1 = *product2;
    *product2 = tmp;
}

int Products::compareNameProduct(Products *product1, Products *product2)
{
    string nameProduct1 = product1->getNameProduct();
    string nameProduct2 = product2->getNameProduct();

    // chuyển đổi sang chữ thường
    transform(nameProduct1.begin(), nameProduct1.end(), nameProduct1.begin(), ::tolower);
    transform(nameProduct2.begin(), nameProduct2.end(), nameProduct2.begin(), ::tolower);

    if (nameProduct1 != nameProduct2) {
        return nameProduct1 < nameProduct2 ? -1 : 1;
    }
    return 0;
}

// Sắp xếp theo tên của sản phẩm
int Products::partitionNameProduct(Products *products, int low, int high, bool ascending)
{
    Products pivot = products[high]; // Chọn pivot là phần tử cuối
    int i = low - 1; // Chỉ mục ban đầu

    for (int j = low; j < high; j++) {
        // Điều kiện so sánh dựa trên hướng sắp xếp
        bool condition = ascending 
                            ? (compareNameProduct(&products[j], &pivot) < 0)  // Tăng dần
                            : (compareNameProduct(&products[j], &pivot) > 0); // Giảm dần

        if (condition) {
            i++;
            swapProducts(&products[i], &products[j]);
        }
    }
    swapProducts(&products[i + 1], &products[high]); // Đưa pivot về đúng vị trí
    return i + 1;
}

void Products::quicksortNameProduct(Products *products, int low, int high, bool ascending)
{
    if (low < high) {
        int p = partitionNameProduct(products, low, high, ascending); // Phân hoạch
        quicksortNameProduct(products, low, p - 1, ascending); // Đệ quy phần bên trái
        quicksortNameProduct(products, p + 1, high, ascending); // Đệ quy phần bên phải
    }
}

Products* Products::searchNameProducts(const string& nameProduct, int& foundCount) {
    int countProduct;
    Products* products = FileManager::loadProducts("text\\Products.txt", countProduct);
    if (products == nullptr || countProduct == 0) {
        foundCount = 0;
        return nullptr;
    }

    if (nameProduct.empty()) {
        foundCount = countProduct;
        return products;
    }
    string searchNameProduct = nameProduct;
    transform(searchNameProduct.begin(), searchNameProduct.end(), searchNameProduct.begin(), ::tolower);

    quicksortNameProduct(products, 0, countProduct - 1, true);

    // Khởi tạo mảng động để lưu các sản phẩm tìm thấy
    foundCount = 0;
    Products* foundProducts = new Products[countProduct];

    // Thực hiện tìm kiếm nhị phân
    int left = 0, right = countProduct - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        string midNameProduct = products[mid].getNameProduct();
        transform(midNameProduct.begin(), midNameProduct.end(), midNameProduct.begin(), ::tolower);

        if (midNameProduct.find(searchNameProduct) != string::npos) {
            // Thêm sản phẩm tìm thấy vào mảng kết quả
            foundProducts[foundCount++] = products[mid];

            // Tìm kiếm các sản phẩm xung quanh `mid`
            int i = mid - 1;
            while (i >= 0) {
                string name = products[i].getNameProduct();
                transform(name.begin(), name.end(), name.begin(), ::tolower);
                if (name.find(searchNameProduct) != string::npos) {
                    foundProducts[foundCount++] = products[i];
                } else {
                    break;
                }
                i--;
            }

            i = mid + 1;
            while (i < countProduct) {
                string name = products[i].getNameProduct();
                transform(name.begin(), name.end(), name.begin(), ::tolower);
                if (name.find(searchNameProduct) != string::npos) {
                    foundProducts[foundCount++] = products[i];
                } else {
                    break;
                }
                i++;
            }
            break;
        } else if (midNameProduct < searchNameProduct) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    // Nếu không tìm thấy, xóa mảng động và trả về nullptr
    if (foundCount == 0) {
        delete[] foundProducts;
        return nullptr;
    }

    return foundProducts;
}

string Products::getSpecificationByID(const string &productFileName, const string &indexFileName, int productID)
{
    ifstream indexFile(indexFileName);
    if (!indexFile.is_open()) {
        cout << "ERROR: Cann't open file. \n";
        return "No product available.";
    }

    int id;
    long position;
    bool found = false;
    string line;
    // Tìm kiếm id sản phẩm trong file chỉ mục
    while (getline(indexFile, line)) {
        // Dùng istringstream để phân tách id và position
        istringstream iss(line);
        char separator;
        
        // Đọc id và dấu phân cách '|'
        if (iss >> id >> separator >> position && separator == '|') {
            // Kiểm tra id có trùng với productID không
            if (id == (productID)) {
                found = true;
                break;
            }
        }
    }
    indexFile.close();

    if (!found) {
        return "No product available.";
    }
    
    // Mở file thông số kỹ thuật và di chuyển đến vị trí byte đã tìm thấy
    ifstream productFile(productFileName);
    if (!productFile.is_open()) {
        return "ERROR: Cann't open file.";
    }
    productFile.seekg(position - 69);
    string result;
    // Đọc và in thông tin của sản phẩm đến khi gặp dấu "*" kết thúc
    cout << "Thong so ky thuat cua san pham co ID " << productID << endl;
    while(getline(productFile, line) && line != "*") {
        result += line;
        result += "\n";
    }
    productFile.close();
    return result;
}

// Sắp xếp sản phẩm theo giá tiền từ cao đến thấp
int Products::partitionPrice(Products *products, int low, int high, bool ascending)
{
    Products pivot = products[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if ((ascending && products[j].getPriceProduct() < pivot.getPriceProduct()) ||
            (!ascending && products[j].getPriceProduct() > pivot.getPriceProduct())) {
            i++;
            swapProducts(&products[i], &products[j]);
        }
    }
    swapProducts(&products[i + 1], &products[high]);
    return i + 1;
}

void Products::quicksortPrice(Products *products, int low, int high, bool ascending)
{
    if (low < high) {
        int p = partitionPrice(products, low, high, ascending);
        quicksortPrice(products, low, p - 1, ascending);
        quicksortPrice(products, p + 1, high, ascending);
    }
}