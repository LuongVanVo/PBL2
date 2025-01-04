#include "D:\PBL2_MYSTORE\include\Admin.h"

Admin::Admin() {}
Admin::~Admin() {}

// Thêm sản phẩm
void Admin::addProduct(string fileName, string imagePath, int productID, string genre, string nameProduct, 
                        double priceProduct, string manufacturer, string operatingSystem, int quantity)
{
    string screenProduct, screenSpecification;
    string line;
    string fileProduct = "D:\\PBL2_MYSTORE\\text\\ThietBi\\" + fileName;
    screenProduct = nameProduct + "_SCREEN";
    screenSpecification = nameProduct + "_SPECIFICATION";
    transform(screenProduct.begin(), screenProduct.end(), screenProduct.begin(), ::toupper);
    transform(screenSpecification.begin(), screenSpecification.end(), screenSpecification.begin(), ::toupper);

    string image = "Graphics/Picture/" + imagePath;
    string tinhNangNoiBat = "D:\\PBL2_MYSTORE\\text\\TinhNangNoiBat\\" + fileName;
    // Kiểm tra xem productID đã tồn tại chưa
    ifstream checkFile("D:\\PBL2_MYSTORE\\text\\Products.txt");
    bool idExists = false;
    while (getline(checkFile, line)) {
        if (line.find(to_string(productID) + "|") != string::npos) {
            idExists = true;
            break;
        }
    }
    checkFile.close();

    if (idExists) {
        cout << "Product ID already exists. Please enter a unique ID.\n";
        return;
    }
    if (genre == "Dien Thoai") {
        ofstream outFileImagePaths("text\\DisplayDevice\\imagePaths1.txt", ios::app);
        outFileImagePaths << image << endl;
        ofstream outFileNameProduct("text\\DisplayDevice\\nameProducts1.txt", ios::app);    
        outFileNameProduct << nameProduct << endl;
        ofstream outFileNameProductScreen("text\\DisplayDevice\\nameProductScreen1.txt", ios::app);
        outFileNameProductScreen << nameProduct << endl;
        ofstream outFilePriceProduct("text\\DisplayDevice\\Price1.txt", ios::app);
        outFilePriceProduct << "$" << fixed << setprecision(2) << priceProduct << endl;
        ofstream outFileTinhNangNoiBat("text\\DisplayDevice\\tinhNangNoiBat1.txt", ios::app);
        outFileTinhNangNoiBat << tinhNangNoiBat << endl;
        ofstream outFileScreenProduct1("text\\DisplayDevice\\screenProduct1.txt", ios::app);
        outFileScreenProduct1 << screenProduct << endl;
        ofstream outFileScreenSpecification1("text\\DisplayDevice\\specificationProduct1.txt", ios::app);
        outFileScreenSpecification1 << screenSpecification << endl;
        ofstream outFileProductID ("text\\DisplayDevice\\ProductID1.txt", ios::app);
        outFileProductID << productID << endl;
    } else if (genre == "Laptop") {
        ofstream outFileImagePaths("text\\DisplayDevice\\imagePaths2.txt", ios::app);
        outFileImagePaths << image << endl;
        ofstream outFileNameProduct("text\\DisplayDevice\\nameProducts2.txt", ios::app);
        outFileNameProduct << nameProduct << endl;
        ofstream outFileNameProductScreen("text\\DisplayDevice\\nameProductScreen2.txt", ios::app);
        outFileNameProductScreen << nameProduct << endl;
        ofstream outFilePriceProduct("text\\DisplayDevice\\Price2.txt", ios::app);
        outFilePriceProduct << "$" << fixed << setprecision(2) << priceProduct << endl;
        ofstream outFileTinhNangNoiBat("text\\DisplayDevice\\tinhNangNoiBat2.txt", ios::app);
        outFileTinhNangNoiBat << tinhNangNoiBat << endl;
         ofstream outFileScreenProduct2("text\\DisplayDevice\\screenProduct2.txt", ios::app);
        outFileScreenProduct2 << screenProduct << endl;
        ofstream outFileScreenSpecification2("text\\DisplayDevice\\specificationProduct2.txt", ios::app);
        outFileScreenSpecification2 << screenSpecification << endl;
        ofstream outFileProductID ("text\\DisplayDevice\\ProductID2.txt", ios::app);
        outFileProductID << productID << endl;
    } else if (genre == "Tai nghe") {                                                                           
        ofstream outFileImagePaths("text\\DisplayDevice\\imagePaths3.txt", ios::app);
        outFileImagePaths << image << endl;
        ofstream outFileNameProduct("text\\DisplayDevice\\nameProducts3.txt", ios::app);
        outFileNameProduct << nameProduct << endl;
        ofstream outFileNameProductScreen("text\\DisplayDevice\\nameProductScreen3.txt", ios::app);
        outFileNameProductScreen << nameProduct << endl;
        ofstream outFilePriceProduct("text\\DisplayDevice\\Price3.txt", ios::app);
        outFilePriceProduct << "$" << fixed << setprecision(2) << priceProduct << endl;
        ofstream outFileTinhNangNoiBat("text\\DisplayDevice\\tinhNangNoiBat3.txt", ios::app);
        outFileTinhNangNoiBat << tinhNangNoiBat << endl;
         ofstream outFileScreenProduct2("text\\DisplayDevice\\screenProduct3.txt", ios::app);
        outFileScreenProduct2 << screenProduct << endl;
        ofstream outFileScreenSpecification2("text\\DisplayDevice\\specificationProduct3.txt", ios::app);
        outFileScreenSpecification2 << screenSpecification << endl;
        ofstream outFileProductID ("text\\DisplayDevice\\ProductID3.txt", ios::app);
        outFileProductID << productID << endl;
    } else if (genre == "Tablet") {
        ofstream outFileImagePaths("text\\DisplayDevice\\imagePaths4.txt", ios::app);
        outFileImagePaths << image << endl;
        ofstream outFileNameProduct("text\\DisplayDevice\\nameProducts4.txt", ios::app);
        outFileNameProduct << nameProduct << endl;
        ofstream outFileNameProductScreen("text\\DisplayDevice\\nameProductScreen4.txt", ios::app);
        outFileNameProductScreen << nameProduct << endl;
        ofstream outFilePriceProduct("text\\DisplayDevice\\Price4.txt", ios::app);
        outFilePriceProduct << "$" << fixed << setprecision(2) << priceProduct << endl;
        ofstream outFileTinhNangNoiBat("text\\DisplayDevice\\tinhNangNoiBat4.txt", ios::app);
        outFileTinhNangNoiBat << tinhNangNoiBat << endl;
         ofstream outFileScreenProduct2("text\\DisplayDevice\\screenProduct4.txt", ios::app);
        outFileScreenProduct2 << screenProduct << endl;
        ofstream outFileScreenSpecification2("text\\DisplayDevice\\specificationProduct4.txt", ios::app);
        outFileScreenSpecification2 << screenSpecification << endl;
        ofstream outFileProductID ("text\\DisplayDevice\\ProductID4.txt", ios::app);
        outFileProductID << productID << endl;
    }
    // Ghi thông tin vào file id + pathPicture
    ofstream outFileID_PathPicture("text\\productID_pathPicture.txt", ios::app);
    outFileID_PathPicture << productID << "|" << image << endl;

    // Mở file để lưu sản phẩm mới
    ofstream outFile("text\\Products.txt", ios::app);

    if (outFile.is_open()) {
        
        // Ghi thông tin sản phẩm vào file
        outFile << productID << '|' << nameProduct << '|' << genre << '|' << priceProduct << '|' << manufacturer << '|'
                << operatingSystem << '|' <<
                   quantity << endl;
        outFile.close();
    }

    // Ghi chi tiết thông số kĩ thuật vào file
    ifstream inFile(fileProduct);
    ofstream saveSpecification("text\\ThongSoKyThuat.txt", ios::app);

    if (!inFile.is_open()) {
        cout << "Cannot open " << fileProduct << " to read specifications.\n";
        return;
    }
    if (!saveSpecification.is_open()) {
        cout << "Cannot open ThongSoKyThuat.txt to save specifications.\n";
        return;
    }

    // Định dạng lưu thông số kỹ thuật
    saveSpecification << "\n*\n";
    saveSpecification << productID << "|\n";
    while (getline(inFile, line)) {
        saveSpecification << line << endl;
    }
    saveSpecification << "*";

    inFile.close();
    saveSpecification.close();

    cout << "Product " << nameProduct << " has been added successfully.\n";
}

// Xóa sản phẩm (1/2)
void Admin::deleteProduct(int productID) 
{
    // Mở file để đọc thông số kỹ thuật
    ifstream inFile_specification("D:\\PBL2_MYSTORE\\text\\ThongSoKyThuat.txt");
    if (!inFile_specification.is_open()) {
        cout << "Error: Can't open specification file.\n";
        return;
    }

    string content, line;
    bool isInsideProduct = false; // Biến đánh dấu đã vào một sản phẩm
    bool removeProduct = false;   // Biến để đánh dấu sản phẩm cần xóa
    bool skipNextStar = false;    // Biến để đánh dấu bỏ qua dấu * đầu tiên của sản phẩm cần xóa

    while (getline(inFile_specification, line)) {
        if (line == "*") {
            if (isInsideProduct) {
                // Nếu đang trong một sản phẩm, kiểm tra xem có phải sản phẩm cần xóa không
                if (removeProduct) {
                    // Nếu sản phẩm cần xóa, bỏ qua dấu * và không ghi lại sản phẩm này
                    isInsideProduct = false;  // Kết thúc sản phẩm cần xóa
                    removeProduct = false;    // Đặt lại biến
                    continue; // Bỏ qua dấu *
                } else {
                    // Nếu sản phẩm không cần xóa, ghi dấu * vào content
                    content += "*\n";
                    isInsideProduct = false; // Kết thúc sản phẩm không cần xóa
                }
            } else {
                // Khi gặp dấu *, bắt đầu một sản phẩm mới
                isInsideProduct = true;
                if (skipNextStar) {
                    // Nếu dấu * cần bỏ qua (do sản phẩm cần xóa), bỏ qua dấu *
                    skipNextStar = false;
                    continue; // Bỏ qua dấu *
                } else {
                    content += "*\n"; // Bắt đầu sản phẩm mới
                }
            }
            continue; // Bỏ qua dấu *
        }

        if (isInsideProduct && line.find(std::to_string(productID) + "|") != string::npos) {
            // Nếu ID sản phẩm cần xóa, đặt dấu flag để bỏ qua sản phẩm này
            removeProduct = true;
            skipNextStar = true; // Đánh dấu bỏ qua dấu * đầu tiên của sản phẩm cần xóa
            continue; // Bỏ qua sản phẩm cần xóa
        }

        // Nếu không xóa, ghi lại dòng vào content
        if (!removeProduct) {
            content += line + "\n"; // Ghi lại nội dung
        }
    }

    inFile_specification.close();

    // Ghi lại nội dung đã cập nhật vào file
    ofstream outFile_specification("D:\\PBL2_MYSTORE\\text\\ThongSoKyThuat.txt");
    if (!outFile_specification.is_open()) {
        cout << "Error: Can't open specification file to write.\n";
        return;
    }
    outFile_specification << content;
    outFile_specification.close();

    // Xóa trong file Products
    ifstream inFile("D:\\PBL2_MYSTORE\\text\\Products.txt");
    if (!inFile.is_open()) {
        cout << "Error: Can't open product file.\n";
        return;
    }

    Products* product = nullptr;
    int countProduct = 0;

    // Đếm số lượng sản phẩm
    while (getline(inFile, line)) {
        ++countProduct;
    }

    inFile.clear();
    inFile.seekg(0, ios::beg);

    product = new Products[countProduct];
    int index = 0;
    bool productFound = false;

    while (getline(inFile, line)) {
        // Đọc toàn bộ file và lưu vào mảng động, trừ sản phẩm có ID cần xóa
        if (line.find(std::to_string(productID) + "|") != string::npos) {
            productFound = true;
            continue; // Bỏ qua sản phẩm cần xóa
        }

        istringstream iss(line);
        string productID_str, nameProduct, genre, priceProduct_str, manufacturer, operatingSystem, count;
        getline(iss, productID_str, '|');
        getline(iss, nameProduct, '|');
        getline(iss, genre, '|');
        getline(iss, priceProduct_str, '|');
        getline(iss, manufacturer, '|');
        getline(iss, operatingSystem, '|');
        getline(iss, count);

        int id = stoi(productID_str);
        double priceProduct = stod(priceProduct_str);
        int Count = stoi(count);

        product[index++] = Products(id, nameProduct, genre, priceProduct, manufacturer, operatingSystem, Count);
    }

    // Ghi lại các sản phẩm vào file (đã xóa sản phẩm cần xóa)
    ofstream outFile("D:\\PBL2_MYSTORE\\text\\Products.txt");
    if (!outFile.is_open()) { cout << "Error: Can't open product file to write.\n"; delete[] product; // Giải phóng bộ nhớ trước khi kết thúc
        return;
    }

    for (int i = 0; i < index; i++) {
        outFile << product[i] << endl; // Định nghĩa toán tử << cho Products
    }

    outFile.close();
    inFile.close();
    delete[] product; // Giải phóng bộ nhớ

    if (productFound) {
        cout << "Product with ID " << productID << " deleted successfully.\n";
    } else {
        cout << "Product with ID " << productID << " not found.\n";
    }
}

// Sửa thông tin sản phẩm
void Admin::editProduct(int &productID, string newNameProduct, string newGenre, double newPriceProduct,
                         string newManufacturer, string newOperatingSystem, int newQuantity)
{
    int productSize;
    Products *product = FileManager::loadProducts("text\\Products.txt", productSize);

    if (product == NULL || productSize == 0) {
        cout << "No product available. \n";
        return;
    }

    bool productFound = false;

    for (int i = 0; i < productSize; i++) {
        if (productID == product[i].getProductID()) {
            // Đọc file và cập nhật thông tin sản phẩm
            ifstream inFile("text\\Products.txt");
            if (!inFile.is_open()){
                cout << "Unable to open file for reading. \n";
                delete [] product;
                return;
            }

            string line, fileContent;
            
            // Đọc từng dòng và xử lí
            while(getline(inFile, line)) {
                istringstream iss(line);
                string id, nameProduct, genre, priceProduct_str, manufacturer, operatingSystem, quantity_str;
                // Phân tách dòng hiện tại bằng dấu phân cách |
                getline(iss, id, '|');
                getline(iss, nameProduct, '|');
                getline(iss, genre, '|');
                getline(iss, priceProduct_str, '|');
                getline(iss, manufacturer, '|');
                getline(iss, operatingSystem, '|');
                getline(iss, quantity_str);


                // Cập nhật nếu đúng sản phẩm
                if (stoi(id) == productID) {
                    nameProduct = newNameProduct;
                    genre = newGenre;
                    // Định dạng giá trị priceProduct với 2 chữ số thập phân
                    stringstream priceStream;
                    // Cập nhật giá trị mới với 2 chữ số thập phân
                    priceStream << fixed << setprecision(2) << newPriceProduct;  
                    priceProduct_str = priceStream.str();  // Chuyển thành chuỗi
                    manufacturer = newManufacturer;
                    operatingSystem = newOperatingSystem;
                    quantity_str = to_string(newQuantity);
                }


                fileContent += id + '|' + nameProduct + '|' + genre + '|' + priceProduct_str + '|' + manufacturer + '|' 
                                + operatingSystem + '|' + quantity_str + "\n";
            }
            inFile.close();

            ofstream outFile("text\\Products.txt", ios::trunc);
            if (!outFile.is_open()) {
                cout << "Unable to open file for writing. \n";
                delete [] product;
                return;
            }

            outFile << fileContent;
            outFile.close();
            cout << "Update information product successfully. \n";
            productFound = true;
            break;
        } 
    }
    delete [] product;
}

// Cấp mã giảm giá
void Admin::generateDiscountCode(string& discountCode, double &price, int &quantity)
{
    ofstream outFile("text\\Discount.txt", ios::app);
    if (!outFile.is_open()) {
        cout << "ERROR: Cann't open file !\n";
        return;
    }
    transform(discountCode.begin(), discountCode.end(), discountCode.begin(), ::toupper);

    outFile << discountCode << "|" << fixed << setprecision(2) << price << "|" << quantity << endl;
    outFile.close();
    cout << "Discount code has been successfully generated and saved.\n";
}