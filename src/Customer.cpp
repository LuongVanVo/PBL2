#include "D:\PBL2_MYSTORE\include\Customer.h"
#include "D:\PBL2_MYSTORE\include\FileManager.h"


Customer::Customer(int customerID, string lastName, string middleName, string firstName, std::string email, std::string phone, std::string userName, std::string password)
{
    this->customerID = customerID;
    this->lastName = lastName;
    this->middleName = middleName;
    this->firstName = firstName;
    this->email = email;
    this->phone = phone;
    this->userName = userName;
    this->password = password;
}


Customer::Customer() {};

int Customer::getCustomerID()
{
    return customerID;
}

string Customer::getLastName()
{
    return lastName;
}

string Customer::getMiddleName()
{
    return middleName;
}

string Customer::getFirstName()
{
    return firstName;
}

std::string Customer::getEmail()
{
    return email;
}

std::string Customer::getPhone()
{
    return phone;
}

std::string Customer::getUserName()
{
    return userName;
}

std::string Customer::getPassword()
{
    return password;
}

void Customer::setPassword(string &password)
{
    this->password = password;
}

bool Customer::verifyLogin(const std::string &userName, const std::string &password)
{
    return (this->userName == userName && this->password == password);
}

void Customer::resizeCustomerArray(Customer*& customers, int &size, const Customer& newCustomer)
{
    // Tạo mảng mới với kích thước lớn hơn
    Customer* newCustomers = new Customer[size + 1];

    for (int i = 0; i < size; i++) {
        newCustomers[i] = customers[i];
    }

    // Thêm khách hàng mới
    newCustomers[size] = newCustomer;
    
    // Giải phóng bộ nhớ và cập nhật mảng và kích thước
    delete [] customers;

    // Gán mảng mới và tăng kích thước
    customers = newCustomers;
    ++size;
}

// Đăng nhập

// login voi SFML
int Customer::login(const std::string& username, const std::string& password, Customer*& loggedInCustomer)
{
    int customerSize;
    Customer* customers = FileManager::loadCustomer("text\\Customers.txt", customerSize);

    bool found = false;
    bool checkAdmin = false;

    for (int i = 0; i < customerSize; i++) {
        if (customers[i].verifyLogin(username, password)) {
            loggedInCustomer = &customers[i];
            found = true;
            break;
        }
    }
    
    // Nếu người login vào là Admin
    if (found && loggedInCustomer->getCustomerID() < 0) {
        checkAdmin = true;
    }

    if (checkAdmin && found) {
        std::cout << "Welcome to admin, " << loggedInCustomer->getLastName() << " " << loggedInCustomer->getMiddleName() << " " << loggedInCustomer->getFirstName() << "!\n";
        // delete [] customers;
        return 2;
    }
    else if (found) {
        std::cout << "Login successful! Welcome, " << loggedInCustomer->getLastName() << " " << loggedInCustomer->getMiddleName() << " " << loggedInCustomer->getFirstName() << "!\n";
        // delete [] customers;    
        return 1;
    }
    else {
        delete [] customers;
        return 0;
    }
}

void Customer::displayCustomer()
{
    int customerSize;
    Customer *customers = FileManager::loadCustomer("D:\\PBL2_MYSTORE\\text\\Customers.txt", customerSize);

    if (customers == NULL || customerSize == 0) {
        std::cout << "No customer available. " << std::endl;
        return;
    }

    // Hiển thị danh sách khách hàng
    std::cout << "List customers \n";
    std::cout << "customerID\t" << "customerName\t" << "Email\t" << "Phone\t" << std::endl;
    for (int i = 0; i < customerSize; i++) {
        // chỉ in ra thông tin của khách hàng
        if (customers[i].getCustomerID() < 0) {
            continue;
        }
        std::cout << customers[i].getCustomerID() << "\t" << customers[i].getLastName() << " " << customers[i].getMiddleName() << " " << customers[i].getFirstName() << "\t"
                  << "\t" << customers[i].getEmail() << "\t" << customers[i].getPhone() << std::endl;
    }
    delete [] customers;
}

// Cập nhật thông tin khách hàng
void Customer::updateCustomerInfo() {
    std::string newLastName, newMiddleName, newFirstName, newEmail, newPhone;
    std::cin.ignore(); // Để xử lý ký tự newline còn lại trong buffer

    std::cout << "Enter your last name: ";
    std::getline(std::cin, newLastName);
    cout << "Enter your middle name: ";
    getline(cin, newMiddleName);
    cout << "Enter your first name: ";
    getline(cin, newFirstName);
    std::cout << "Enter new email: ";
    std::cin >> newEmail;
    std::cout << "Enter new phone: ";
    std::cin >> newPhone;

    // Đọc file và cập nhật thông tin khách hàng
    std::ifstream fileIn("D:\\PBL2_MYSTORE\\text\\Customers.txt");
    if (!fileIn.is_open()) {
        std::cout << "Unable to open file for reading.\n";
        return;
    }

    std::string line, fileContent;
    bool found = false;

    // Đọc từng dòng và xử lý
    while (std::getline(fileIn, line)) {
        std::istringstream iss(line);
        std::string id, lastName, middleName, firstName, emailCustomer, phoneCustomer, username, password;

        // Phân tách dòng hiện tại bằng dấu phân cách '|'
        std::getline(iss, id, '|');
        std::getline(iss, lastName, '|');
        std::getline(iss, middleName, '|');
        std::getline(iss, firstName, '|');
        std::getline(iss, emailCustomer, '|');
        std::getline(iss, phoneCustomer, '|');
        std::getline(iss, username, '|');
        std::getline(iss, password);

        // Chỉ cập nhật thông tin của khách hàng hiện tại
        if (std::stoi(id) == this->getCustomerID()) {
            lastName = newLastName;
            middleName = newMiddleName;
            firstName = newFirstName;
            emailCustomer = newEmail;
            phoneCustomer = newPhone;
            found = true;
        }

        // Xây dựng lại nội dung file với giá trị đã cập nhật
        fileContent += id + '|' + lastName + '|' + middleName + '|' + firstName + '|' + emailCustomer + '|' + phoneCustomer + '|' + username + '|' + password + '\n';
    }
    fileIn.close();

    if (!found) {
        std::cout << "Customer not found.\n";
        return;
    }

    // Ghi đè lại file với nội dung mới
    std::ofstream fileOut("D:\\PBL2_MYSTORE\\text\\Customers.txt", std::ios::trunc);
    if (!fileOut.is_open()) {
        std::cout << "Unable to open file for writing.\n";
        return;
    }

    fileOut << fileContent;
    fileOut.close();

    std::cout << "Update information successfully.\n";
}

// Thay đổi mật khẩu người dùng 
void Customer::changePassword()
{
    string oldPassword, newPassword, confirmPassword;
    std::cout << "Enter the old password: "; cin >> oldPassword;
    
    if (oldPassword != this->getPassword()) {
        cout << "Old password is incorrect. \n";
        return;
    }
    if (oldPassword == this->getPassword()) {
        cout << "Enter the new password: "; cin >> newPassword;
        cout << "Enter the confirm password: "; cin >> confirmPassword;

        if (newPassword != confirmPassword) {
            cout << "Confirmation password is incorrect. \n";
            return;
        } else {
            ifstream fileIn("D:\\PBL2_MYSTORE\\text\\Customers.txt");
            if (!fileIn.is_open()) {
                cout << "Unable to open file for reading. \n";
                return;
            }

            string line, fileContent;
            bool found = false;

            // Doc tung dong va xu ly
            while(getline(fileIn, line)) {
                istringstream iss(line);
                string id, lastName, middleName, firstName, email, phone, username, password;

                // Phan tich dong hien tai bang dau phan cach '|'
                getline(iss, id, '|');
                getline(iss, lastName, '|');
                getline(iss, middleName, '|');
                getline(iss, firstName, '|');
                getline(iss, email, '|');
                getline(iss, phone, '|');
                getline(iss, username, '|');
                getline(iss, password);

                if (stoi(id) == customerID) {
                    password = newPassword;
                    found = true;
                }

                // Xay dung lai noi dung file voi gia tri da cap nhat
                fileContent += id + '|' + lastName + '|' + middleName + '|' + firstName + '|' + email + '|' + phone + '|' + username + '|' + password + '\n'; 
            }
            fileIn.close();
            if (!found) {
                cout << "Not found customer. \n";
                return;
            }

            // Ghi de lai file voi noi dung moi
            ofstream fileOut("D:\\PBL2_MYSTORE\\text\\Customers.txt", ios::trunc);
            if (!fileOut.is_open()) {
                cout << "Unable to open file for writing. \n";
                return;
            }
            fileOut << fileContent;
            fileOut.close();
            cout << "Change password successfully. \n";
        }
    }   
}

// Đăng kí tài khoản
// sign up voi SFML
void Customer::registerAccount(string lastName, string middleName, string firstName, string email, 
                                string phone, string userName, string password)
{
    int customerSize;
    Customer* customers = FileManager::loadCustomer("text\\Customers.txt", customerSize);
    Customer *customer = new Customer;

    int newID = customerSize + 1;

    Customer newCustomer(newID, lastName, middleName, firstName, email, phone, userName, password);
    // Kiểm tra để đảm bảo tên đăng nhập giữa các tài khoản là duy nhất (Không trùng nhau)
    for (int i = 0; i < customerSize; i++) {
        if (customers[i].getUserName() == userName) {
            cout << "Username already exists. \n";
            return;
        }
    }
    resizeCustomerArray(customers, customerSize, newCustomer); // Thêm khách hàng mới vào mảng động 
    FileManager::saveCustomer("text\\Customers.txt", newCustomer); // Lưu vào file 

    std::cout << "Account created successfully!\n";
    customer = &customers[customerSize - 1];

    // Tạo file order của chính khách hàng vứa đăng kí 
    string nameFile = to_string(newID) + "_order.txt";
    string fileOrderPath = "text\\Order\\" + nameFile;

    ofstream fileOrder(fileOrderPath, ios::app);
    
    fileOrder.close();
}

int Customer::verifyPhone(const string &phone)
{
    int customerSize;
    Customer *customers = FileManager::loadCustomer("text\\Customers.txt", customerSize);
    for (int i = 0; i < customerSize; i++) {
        if (customers[i].getPhone() == phone) {
            return i;
        }
    }
    return -1; // Không tìm thấy
}

int Customer::verifyEmail(const string &email)
{
    int customerSize;
    Customer *customers = FileManager::loadCustomer("text\\Customers.txt", customerSize);
    for (int i = 0; i < customerSize; i++) {
        if (customers[i].getEmail() == email) {
            return i;
        }
    }
    return -1; // Không tìm thấy
}
// Hàm quên mật khẩu => đổi mật khẩu lại thông qua số điện thoại
void Customer::forgetPassword(const string& email, string &newPassword, string& confirmNewPassword)
{
        int customerSize;
        Customer* customers = FileManager::loadCustomer("D:\\PBL2_MYSTORE\\text\\Customers.txt", customerSize);

        // Kiểm tra xem số điện thoại có tồn tại không
        int index = Customer::verifyEmail(email);
        if (index == -1) {
            cout << "Email not found.\n";
            delete[] customers;  // Giải phóng bộ nhớ
            return;
        }

        if (confirmNewPassword != newPassword) {
            cout << "Confirmation password is incorrect.\n";
            delete[] customers;  // Giải phóng bộ nhớ
            return;
        }

        // Cập nhật mật khẩu mới
        customers[index].setPassword(newPassword);

        // Ghi lại dữ liệu vào file
        ofstream fileOut("D:\\PBL2_MYSTORE\\text\\Customers.txt", ios::trunc);
        if (!fileOut.is_open()) {
            cout << "Unable to open file for writing.\n";
            delete[] customers;  // Giải phóng bộ nhớ
            return;
        }

        // Ghi lại tất cả khách hàng với mật khẩu đã cập nhật
        for (int i = 0; i < customerSize; i++) {
            fileOut << customers[i].getCustomerID() << "|" << customers[i].getLastName() << "|" << customers[i].getMiddleName() 
                    << "|" << customers[i].getFirstName() << "|" << customers[i].getEmail() << "|" << customers[i].getPhone() << "|" 
                    << customers[i].getUserName() << "|" << customers[i].getPassword() << "\n";
        }

        fileOut.close();
        cout << "Change password successfully.\n";
        delete[] customers;  // Giải phóng bộ nhớ
}

string Customer::getFullName() const 
{
    return lastName + " " + middleName + " " + firstName;
}

void Customer::swapCustomer(Customer *customer1, Customer *customer2) {
    Customer tmp = *customer1;
    *customer1 = *customer2;
    *customer2 = tmp;
}

int Customer::compareNameCustomer(Customer *customer1, Customer *customer2) {
    std::string firstname1 = customer1->getFirstName(), firstname2 = customer2->getFirstName();
    std::string middlename1 = customer1->getMiddleName(), middlename2 = customer2->getMiddleName();
    std::string lastname1 = customer1->getLastName(), lastname2 = customer2->getLastName();

    // Chuyển đổi sang chữ thường
    std::transform(firstname1.begin(), firstname1.end(), firstname1.begin(), ::tolower);
    std::transform(firstname2.begin(), firstname2.end(), firstname2.begin(), ::tolower);
    std::transform(middlename1.begin(), middlename1.end(), middlename1.begin(), ::tolower);
    std::transform(middlename2.begin(), middlename2.end(), middlename2.begin(), ::tolower);
    std::transform(lastname1.begin(), lastname1.end(), lastname1.begin(), ::tolower);
    std::transform(lastname2.begin(), lastname2.end(), lastname2.begin(), ::tolower);

    if (firstname1 != firstname2) {
        return firstname1 < firstname2 ? -1 : 1;
    }
    if (middlename1 != middlename2) {
        return middlename1 < middlename2 ? -1 : 1;
    }
    if (lastname1 != lastname2) {
        return lastname1 < lastname2 ? -1 : 1;
    }
    return 0;
}

// Sắp xếp theo tên khách hàng
int Customer::partitionNameCustomer(Customer *customers, int low, int high, bool ascending) {
    Customer pivot = customers[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        bool condition = ascending 
                            ? (compareNameCustomer(&customers[j], &pivot) < 0) // Tang dan
                            : (compareNameCustomer(&customers[j], &pivot) > 0); // Giam dan
        if (condition) {
            i++;
            swapCustomer(&customers[i], &customers[j]);
        }
    }
    swapCustomer(&customers[i + 1], &customers[high]);
    return i + 1;
}

void Customer::quicksortNameCustomer(Customer *customers, int low, int high, bool ascending) {
    if (low < high) {
        int p = partitionNameCustomer(customers, low, high, ascending);
        quicksortNameCustomer(customers, low, p - 1, ascending);
        quicksortNameCustomer(customers, p + 1, high, ascending);
    }
}


Customer* Customer::searchNameCustomer(string nameCustomer, int& foundCount) {
        std::string searchNameLower = nameCustomer;
        std::transform(searchNameLower.begin(), searchNameLower.end(), searchNameLower.begin(), ::tolower);

        int countCustomer;
        Customer* customers = FileManager::loadCustomer("text\\Customers.txt", countCustomer);

        // Đếm số lượng khách hàng tìm thấy trước
        foundCount = 0;
        for (int i = 0; i < countCustomer; i++) {
            std::string fullName = customers[i].getFullName();
            std::string fullNameLower = fullName;
            std::transform(fullNameLower.begin(), fullNameLower.end(), fullNameLower.begin(), ::tolower);

            if (fullNameLower.find(searchNameLower) != std::string::npos) {
                foundCount++;
            }
        }

        // Tạo mảng động chứa các khách hàng tìm thấy
        Customer* foundCustomers = new Customer[foundCount];
        int index = 0;
        for (int i = 0; i < countCustomer; i++) {
            std::string fullName = customers[i].getFullName();
            std::string fullNameLower = fullName;
            std::transform(fullNameLower.begin(), fullNameLower.end(), fullNameLower.begin(), ::tolower);

            if (fullNameLower.find(searchNameLower) != std::string::npos) {
                foundCustomers[index++] = customers[i];
            }
        }

        delete[] customers;
        return foundCustomers; 
    }
