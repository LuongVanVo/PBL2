#define _HAS_STD_BYTE 0
#include <cstddef>
#include <windows.h>
#undef byte
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "../include/Customer.h"
#include "../include/FileManager.h"
#include "../include/Products.h"
#include "../include/Order.h"
#include "../include/Discount.h"
#include "../include/Admin.h"
#include "../include/History.h"
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <cctype>
#include <array>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <locale>
#include <codecvt>
#include <curl/curl.h>
#include <json/json.h>
map<int, sf::Texture> productTextures; 

// Danh sách chứa ID sản phẩm và đường dẫn tới ảnh của sản phẩm
vector<pair<int, string>> productInfo = FileManager::readFromFile("text\\productID_pathPicture.txt");

int posY = 190;
float rightMargin = 650.0f;
float posProduct = 100; // Vị trí của sản phẩm đầu tiên trong giỏ hàng

enum ScreenState {
    MAIN_MENU,
    PAY_SCREEN,
    FORGETPASSWORD_SCREEN,
    EMAIL_SCREEN, // Nhập email khi quên mật khẩu
    INFOACCOUNT_SCREEN,
    HISTORYORDER_SCREEN,
    PRODUCTS_SCREEN2,
    VIEWCART_SCREEN,
    SIGNUP_SCREEN,
    PRODUCTS_SCREEN,
    ADMIN_SCREEN,
    DISPLAYCUSTOMER_SCREEN,
    DISPLAYPRODUCTS_SCREEN,
    DISCOUNTCODE_SCREEN,
    REVENUE_SCREEN,
    EDITPRODUCT_SCREEN,
    ADDPRODUCTS_SCREEN,
    DETAILSTATISTICS_SCREEN,
    PRODUCTS_SCREEN3,
    PRODUCTS_SCREEN4,
    XACTHUC_SCREEN
};

// Hàm tạo khối
sf::RectangleShape createButton(float width, float height, float posX, float posY, sf::Color color) {
    sf::RectangleShape button(sf::Vector2f(width, height));
    button.setPosition(posX, posY);
    button.setFillColor(color);
    return button;
}

// Tạo text cho khối
sf::Text createButtonText(const std::string& textString, const sf::Font& font, unsigned int fontSize, sf::Color color, const sf::RectangleShape& button) {
    // Chuyển từ std::string (UTF-8) sang std::wstring
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wideString = converter.from_bytes(textString);

    sf::Text text(wideString, font, fontSize);
    text.setFillColor(color);

    // Căn giữa text với nút
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        button.getPosition().x + (button.getSize().x - textBounds.width) / 2.0f,
        button.getPosition().y + (button.getSize().y - textBounds.height) / 2.0f - 5
    );
    return text;
}

// Tạo text
sf::Text createText(const std::string& textString, const sf::Font& font, unsigned int fontSize, sf::Color color, float posX, float posY) {

    // Chuyển từ std::string (UTF-8) sang std::wstring
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wideString = converter.from_bytes(textString);
    sf::Text text(wideString, font, fontSize);
    text.setFillColor(color);
    text.setPosition(posX, posY);
    return text;
}

// Vẽ con trỏ
void drawCursor(sf::RenderWindow& window, const sf::Text& inputText, float posX, float posY) {
    sf::RectangleShape cursor(sf::Vector2f(2, 30));
    cursor.setPosition(posX + inputText.getGlobalBounds().width, posY);
    cursor.setFillColor(sf::Color::Black);
    window.draw(cursor);
}

// Hàm tạo hình chữ nhật chỉ có màu viền
sf::RectangleShape createOutlinedRectangle(float width, float height, sf::Color outlineColor, float outlineThickness, sf::Vector2f position) {
    // Tạo hình chữ nhật
    sf::RectangleShape rectangle(sf::Vector2f(width, height)); // Kích thước hình chữ nhật
    rectangle.setFillColor(sf::Color::Transparent); // Màu nền trong suốt
    rectangle.setOutlineThickness(outlineThickness); // Độ dày viền
    rectangle.setOutlineColor(outlineColor); // Màu viền
    rectangle.setPosition(position); // Vị trí hình chữ nhật
    
    return rectangle; // Trả về hình chữ nhật
}

// Hàm tải hình ảnh sản phẩm
void loadProductImages()
{
    for (const auto& product : productInfo) {
        int productID = product.first;
        const std::string& filePath = product.second;

        sf::Texture texture;
        if (texture.loadFromFile(filePath)) {
            productTextures[productID] = texture;
        } else {
            std::cout << "Failed to load image for product ID: " << productID << " from " << filePath << std::endl;
        }
    }
}
// Hàm nhận biết và trả về hình ảnh sản phẩm tương ứng với ID sản phẩm
sf::Sprite getProductImageID(int productID) 
{
    sf::Sprite sprite;
    // Kiểm tra nếu ID tồn tại trong map, gán texture cho sprite
    if (productTextures.find(productID) != productTextures.end()) {
        sprite.setTexture(productTextures[productID]);
    } else {
        cout << "Product image with ID " << productID << " not found. \n";
    }
    return sprite;
}
// Hàm tạo nút giỏ hàng và biểu tượng (chờ thêm sản phẩm thêm vào rồi fix lại)
void createCartButtonsAndIcons(const sf::Vector2u& windowSize, float rightMargin, float posY, sf::RectangleShape cartButtons[], sf::Sprite cartSprites[], int COUNT_PRODUCT, sf::Texture& cartIcon) {
    // Tạo các nút giỏ hàng và biểu tượng
    for (int i = 0; i < COUNT_PRODUCT; ++i) {
        // Thiết lập nút giỏ hàng
        cartButtons[i] = createButton(50, 50, windowSize.x - rightMargin - 200 - 450 + 320 * i + 250, posY - 60 + 350, sf::Color::White);
        
        // Thiết lập sprite cho biểu tượng giỏ hàng
        cartSprites[i].setTexture(cartIcon);
        cartSprites[i].setScale(0.2f, 0.2f);
        cartSprites[i].setPosition(windowSize.x - rightMargin - 200 - 450 + 320 * i + 250, posY - 60 + 350);
    }
}

// Tạo các chi tiết trong màn hình chi tiết từng sản phẩm
void createProductDetail(sf::RectangleShape& detailProduct, sf::RectangleShape& muaNgayButton, 
                         sf::Text& muaNgayText, sf::RectangleShape& cartButton, 
                         sf::Sprite& cartSprite, const std::string& muaNgayTextString, 
                         const sf::Font& font_bold, const sf::Vector2u& windowSize, 
                         float rightMargin, float posY, sf::Texture& cartIcon) {
    
    // Chi tiết sản phẩm
    detailProduct = createButton(950, 550, windowSize.x - rightMargin - 200 - 470, posY - 45, sf::Color(242, 170, 148));
    
    // Nút "Mua Ngay"
    muaNgayButton = createButton(280, 50, 1010, 545, sf::Color(250, 50, 50));
    muaNgayText = createButtonText(muaNgayTextString, font_bold, 30, sf::Color::White, muaNgayButton);
    
    // Nút giỏ hàng
    cartButton = createButton(50, 50, 1300, 545, sf::Color::White);

    // Biểu tượng giỏ hàng
    cartSprite.setTexture(cartIcon);
    cartSprite.setScale(0.21f, 0.24f);
    cartSprite.setPosition(1300, 545);
}

// Tạo sản phẩm trong màn hình PRODUCT_SCREEN
void createProductDisplay(sf::RectangleShape& productShape, sf::Sprite& productSprite, 
                          sf::Text& nameText, sf::Text& priceText, 
                          sf::Texture& productTexture, const std::string& productName, 
                          const std::string& price, const sf::Font& font, float scaleX, 
                          float scaleY, float posX, float posY, float posPictureX, float posPictureY,
                          sf::RectangleShape &cartButton, sf::Sprite &cartSprite, sf::Texture &cartIcon,
                          float posCartX, float posCartY) {
    
    // Tạo hình chữ nhật đại diện cho sản phẩm
    productShape = createButton(300, 400, posX, posY, sf::Color::White);

    // Thiết lập sprite cho ảnh sản phẩm
    productSprite.setTexture(productTexture);
    productSprite.setScale(scaleX, scaleY);
    productSprite.setPosition(posPictureX, posPictureY);
    // Tạo tên sản phẩm
    nameText = createText(productName, font, 20, sf::Color::Black, posX + 20, posY + 270);

    // Tạo giá sản phẩm
    priceText = createText(price, font, 20, sf::Color(253, 79, 72), posX + 20, posY + 350);

    // Tạo nút giỏ hàng
    cartButton = createButton(50, 50, posCartX, posCartY, sf::Color::White);
    cartSprite.setTexture(cartIcon);
    cartSprite.setScale(0.21f, 0.24f);
    cartSprite.setPosition(posCartX, posCartY);

    if (posX >= 1366) posY += 450;
}

string formatTotalAmount(double totalAmountTemp) {
    // Tạo một ostringstream và định dạng số với 2 chữ số thập phân
    ostringstream oss;
    oss << fixed << setprecision(2) << totalAmountTemp;

    // Chuyển đổi giá trị double sang chuỗi
    string totalAmounTemptString = "$" + oss.str();

    // Xóa các chữ số '0' thừa ở cuối phần thập phân
    totalAmounTemptString.erase(totalAmounTemptString.find_last_not_of('0') + 1, string::npos);

    // Nếu ký tự cuối cùng là dấu '.' thì xóa nó đi
    if (totalAmounTemptString.back() == '.') {
        totalAmounTemptString.pop_back();
    }

    return totalAmounTemptString;
}

// Xử lý khi dùng nút log out và nút home
void handleMouseClickEvents(const sf::Event& event, 
                             const sf::Vector2f& mouseWorldPos, 
                             sf::RectangleShape& logOutButton,
                             sf::RectangleShape& homeButton,
                             ScreenState& screenState) {
    // Xử lý khi click vào nút Logout
    if (event.type == sf::Event::MouseButtonPressed && 
        event.mouseButton.button == sf::Mouse::Left) {
        if (logOutButton.getGlobalBounds().contains(mouseWorldPos)) {
            screenState = MAIN_MENU;
            std::cout << "Logout successfully. \n";
        }
    }

    // Xử lý khi click vào nút Home
    if (homeButton.getGlobalBounds().contains(mouseWorldPos)) {
        screenState = PRODUCTS_SCREEN;
        std::cout << "Return to the main screen. \n";
    }
}

// Tạo dòng kẻ
void drawLine(sf::RenderWindow &window, sf::Vector2f start, sf::Vector2f end, sf::Color color) {
    sf::Vertex line[] = {
        sf::Vertex(start, color),
        sf::Vertex(end, color)
    };
    window.draw(line, 2, sf::Lines);
}

// Hàm chuyển từ string về wstring
std::wstring convertToWstring(const std::string& utf8String) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(utf8String);
}
// Hàm chuyển từ wstring về string
std::string convertToString(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

// Hàm tạo mã OTP trả về
int generateRandomNumber() {
    // Tạo seed ngẫu nhiên từ thời gian hệ thống
    std::srand(std::time(0));

    // Sinh số ngẫu nhiên trong phạm vi từ 100000 đến 999999
    int randomNumber = std::rand() % 900000 + 100000;

    return randomNumber;
}

// Hàm gửi về email
void sendOTP(const std::string& apiKey, const std::string& apiSecret, const std::string& fromEmail, 
                const std::string& toEmail, const std::string& subject, const std::string& body) {
    CURL* curl;
    CURLcode res;

    // Khởi tạo cURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Tạo nội dung JSON để gửi tới Mailjet
        Json::Value emailData;
        emailData["Messages"][0]["From"]["Email"] = fromEmail;
        emailData["Messages"][0]["To"][0]["Email"] = toEmail;
        emailData["Messages"][0]["Subject"] = subject;
        emailData["Messages"][0]["TextPart"] = body;

        // Chuyển JSON sang chuỗi
        Json::StreamWriterBuilder writer;
        std::string emailJson = Json::writeString(writer, emailData);

        // Cấu hình cURL để gửi yêu cầu POST tới Mailjet API
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.mailjet.com/v3.1/send");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, emailJson.c_str());
        curl_easy_setopt(curl, CURLOPT_USERPWD, (apiKey + ":" + apiSecret).c_str()); // Basic Authentication

        // Thêm chứng chỉ SSL nếu cần
        curl_easy_setopt(curl, CURLOPT_CAINFO, "D:\\jsoncpp\\build\\cacert.pem");

        // Thực hiện yêu cầu HTTP
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "Lỗi cURL: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "\nEmail da duoc gui thanh cong!" << std::endl;
        }

        // Dọn dẹp
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

int main()
{
    std::byte myStdByte = std::byte{0x1};

    Customer *customers = new Customer;
    Products *products = new Products;
    Order *orders = new Order;
    Admin *admin = new Admin;
    Discount *discount = new Discount;
    History *history = new History;
    FileManager *fileManager = new FileManager;
    
    // Tạo cửa sổ
    sf::RenderWindow window(sf::VideoMode(1366, 768), "My Window");

    // Trạng thái của màn hình
    ScreenState screenState = MAIN_MENU;
    int lineCountProduct1, lineCountProduct2, lineCountProduct3, lineCountProduct4, lineCountSpecification1, lineCountSpecification2, lineCountSpecification3, lineCountSpecification4;
    int *screenProduct1 = fileManager->loadLineNumbersFromFile("text\\DisplayDevice\\screenProduct1.txt", lineCountProduct1);
    int *screenSpecification1 = fileManager->loadLineNumbersFromFile("text\\DisplayDevice\\specificationProduct1.txt", lineCountSpecification1);
    int *screenProduct2 = fileManager->loadLineNumbersFromFile("text\\DisplayDevice\\screenProduct2.txt", lineCountProduct2);
    int *screenSpecification2 = fileManager->loadLineNumbersFromFile("text\\DisplayDevice\\specificationProduct2.txt", lineCountSpecification2);
    int *screenProduct3 = fileManager->loadLineNumbersFromFile("text\\DisplayDevice\\screenProduct3.txt", lineCountProduct3);
    int *screenSpecification3 = fileManager->loadLineNumbersFromFile("text\\DisplayDevice\\specificationProduct3.txt", lineCountSpecification3);
    int *screenProduct4 = fileManager->loadLineNumbersFromFile("text\\DisplayDevice\\screenProduct4.txt", lineCountProduct4);
    int *screenSpecification4 = fileManager->loadLineNumbersFromFile("text\\DisplayDevice\\specificationProduct4.txt", lineCountSpecification4);

    // Load id sản phầm vào mảng
    int countProduct1, countProduct2, countProduct3, countProduct4;
    int *IDSanPham1 = fileManager->loadIDFromFile("text\\DisplayDevice\\ProductID1.txt", countProduct1);
    int *IDSanPham2 = fileManager->loadIDFromFile("text\\DisplayDevice\\ProductID2.txt", countProduct2);
    int *IDSanPham3 = fileManager->loadIDFromFile("text\\DisplayDevice\\ProductID3.txt", countProduct3);
    int *IDSanPham4 = fileManager->loadIDFromFile("text\\DisplayDevice\\ProductID4.txt", countProduct4);
    // Lấy kích thước cửa sổ
    sf::Vector2u windowSize = window.getSize();

    // Tạo font và text
    sf::Font font, font_bold;
    if (!font.loadFromFile("Graphics/Font Text/ARIAL.TTF"))
    {
        return -1;
    }
    if (!font_bold.loadFromFile("Graphics/Font Text/ARIALBD 1.TTF")) {
        return -1;
    }
    
    // Tạo các hình chữ nhật mô phỏng nút với kích thước và vị trí dựa trên kích thước cửa sổ
    sf::RectangleShape signupButton = createButton(50, 20, windowSize.x - rightMargin + 342 + 30, 457, sf::Color(245, 245, 220));

    // Text cho nút Sign up (signup)
    sf::Text signupText = createButtonText("Đăng ký", font_bold, 17, sf::Color(250, 50, 50), signupButton);

    // Nút quên mật khẩu 
    string emailForgetPassword, newPasswordForget, confirmNewPasswordForget;
    string emailForgetPasswordTmp; // chuỗi chứa sdt tạm thời
    sf::RectangleShape forgetPasswordButton = createButton(130, 20, windowSize.x - rightMargin + 330, 355, sf::Color(245, 245, 220));
    sf::Text forgetPasswordText = createButtonText("Quên mật khẩu?", font, 15, sf::Color::Black, forgetPasswordButton);

    // Text tiêu đề để nhập số điện thoại để đổi password
    sf::Text emailText_ChangePassword = createText("Email:", font, 20, sf::Color::Black, windowSize.x - rightMargin - 350, 270);
    sf::Text emailInputText_ChangePassword = createText("", font, 20, sf::Color::Black, windowSize.x - rightMargin - 350, posY + 2*60 + 7); // Text để hiển thị đầu vào
    bool isTypingPhoneChangePassword = false;
    // Nút xác nhận khi nhập số điện thoại xong
    sf::RectangleShape confirmEmailButton = createButton(600, 50, windowSize.x - rightMargin - 350, posY + 3*60, sf::Color(250, 50, 50));
    sf::Text confirmEmailText = createButtonText("Tiếp tục", font, 20, sf::Color::White, confirmEmailButton); // Text cho nút confirmPhone

    // Chi tiết trong màn hình XACTHUC_SCREEN
    string maXacThuc;
    bool isMaXacThuc = false;
    sf::RectangleShape maXacThucButton = createButton(600, 50, windowSize.x - rightMargin - 350, posY + 2*60, sf::Color::White);
    sf::Text maXacThucInputText = createText("", font, 20, sf::Color::Black, windowSize.x - rightMargin - 350 + 10, posY + 2*60 + 7);
    sf::RectangleShape maXacThucConfirmButton = createButton(600, 50, windowSize.x - rightMargin - 350, posY + 3*60, sf::Color(250, 50, 50));
    sf::Text maXacThucConfirmText = createButtonText("Tiếp tục", font, 20, sf::Color::White, maXacThucConfirmButton); // Text cho nút maXacThuc

    // Set up trong màn hình nhập và đổi mật khẩu mới
    sf::Text newPasswordText_Forget = createText("Mật khẩu mới:", font, 20, sf::Color::Black, windowSize.x - rightMargin - 350, 275);
    sf::Text newPasswordInputText_Forget = createText("", font, 20, sf::Color::Black, windowSize.x - rightMargin - 350, posY + 2*60 + 7); // Text để hiển thị đầu vào
    sf::Text confirmNewPasswordText_Forget = createText("Xác nhận mật khẩu mới:", font, 20, sf::Color::Black, windowSize.x - rightMargin - 350, 365);
    sf::Text confirmNewPasswordInputText_Forget =  createText("", font, 20, sf::Color::Black, windowSize.x - rightMargin - 350, posY + 3.5*60 + 7); // Text để hiển thị đầu vào
    bool isTypingNewPassword_Forget = false;
    bool isTypingConfirmNewPassWord_Forget = false;
    // Nút xác nhận khi nhập password mới xong
    sf::RectangleShape confirmNewPasswordButton = createButton(600, 50, windowSize.x - rightMargin - 350, posY + 4.5*60, sf::Color(250, 50, 50));
    sf::Text confirmNewPasswordTextButton = createButtonText("Xác nhận", font, 20, sf::Color::White, confirmNewPasswordButton); // Text cho nút confirmPhone
    
    std::string apiKey = "e8b0eb767ea98b7363b3df681baa17fe";  // API key của bạn
    std::string apiSecret = "794a7a85715c668e4ecc4c7c175ddbe6";  // Secret key của bạn
    std::string fromEmail = "luongvanvo29@gmail.com";  // Email người gửi
    std::string subject = "Email authentication code";
    int OTP_INT;

    // Chèn ảnh nền
    sf::Texture pictureTex1, pictureTex2;
    pictureTex1.loadFromFile("Graphics/Picture/logoBachKhoa.jpg");
    pictureTex2.loadFromFile("Graphics/Picture/logoKhoaCNTT.jpg");
    sf::Sprite sprite1, sprite2;
    sprite1.setTexture(pictureTex1);
    sprite1.setPosition(130, 200);
    sprite1.setScale(0.28125f, 0.28125f);
    sprite2.setTexture(pictureTex2);
    sprite2.setPosition(370, 200);
    
    // Text cho tiêu đề màn hình đăng nhập và đăng ký
    sf::Text usernameText = createText("Tên đăng nhập:", font, 20, sf::Color::Black, windowSize.x - rightMargin, 210);
    sf::Text passwordText = createText("Mật khẩu:", font, 20, sf::Color::Black, windowSize.x - rightMargin, 310);

    // Chuỗi nhập liệu
    wstring lastNameInput, middleNameInput, firstNameInput;
    string usernameInput, passwordInput, emailInput, phoneInput, usernameInput_SIGNUP, passwordInput_SIGNUP;
    
    // Đối tượng text để hiển thị đầu vào
    sf::Text usernameInputText = createText("", font, 20, sf::Color::Black, windowSize.x - rightMargin + 150, 200 + 7);
    sf::Text passwordInputText = createText("", font, 20, sf::Color::Black, windowSize.x - rightMargin + 150, 300 + 7);
    
    bool isTypingUsername = false;
    bool isTypingPassword = false;

    // Thời gian để quản lý con trỏ nhấp nháy
    sf::Clock clock;
    bool cursorVisible = true;

    // Tạo nút Sign in
    sf::RectangleShape signInButton = createButton(150, 50, windowSize.x - rightMargin + 200, 400, sf::Color(250, 50, 50));
    // Tạo nút Sign up
    sf::RectangleShape registerAccount = createButton(150, 50, windowSize.x - rightMargin + 300, 400, sf::Color(250, 50, 50));
    // Text cho nút Sign up
    sf::Text registerAccountText = createButtonText("Đăng ký", font, 20, sf::Color::Black, registerAccount);
    // Text cho nút Sign in
    sf::Text signInText = createButtonText("Đăng nhập", font, 20, sf::Color::Black, signInButton);
    
    // Tạo nút Back
    sf::RectangleShape backButton = createButton(30, 30, 40, 40, sf::Color::Transparent);
    // Ảnh nút back
    sf::Texture backPicture;
    backPicture.loadFromFile("Graphics/Picture/backButton.png");
    sf::Sprite spriteBack;
    spriteBack.setTexture(backPicture);
    spriteBack.setScale(0.02f, 0.02f);
    spriteBack.setPosition(36, 36);

    // Tạo nút logOut trong các màn hình
    sf::RectangleShape logOutButton = createButton(30, 30, 75, 42, sf::Color::Transparent);
    // Ảnh nút logout
    sf::Texture logOutPicture;
    logOutPicture.loadFromFile("Graphics/Picture/logout.png");
    sf::Sprite spriteLogOut;
    spriteLogOut.setTexture(logOutPicture);
    spriteLogOut.setScale(0.063f, 0.063f);
    spriteLogOut.setPosition(75, 44);

    // Tạo nút về trang chủ
    sf::RectangleShape homeButton = createButton(30, 30, 110, 42, sf::Color::Transparent);
    // Ảnh nút home
    sf::Texture homePicture;
    homePicture.loadFromFile("Graphics/Picture/home.png");
    sf::Sprite spriteHome;
    spriteHome.setTexture(homePicture);
    spriteHome.setScale(0.07f, 0.07f);
    spriteHome.setPosition(110, 42);

    // Nút xem giỏ hàng
    sf::RectangleShape viewCart = createButton(50, 50, windowSize.x - rightMargin - 200 + 730, 30, sf::Color::Black);
    sf::Texture viewCartIcon;
    viewCartIcon.loadFromFile("Graphics/Picture/CartView.jpg");
    sf::Sprite spriteViewCart;
    spriteViewCart.setTexture(viewCartIcon);
    spriteViewCart.setScale(0.051f, 0.051f);
    spriteViewCart.setPosition(windowSize.x - rightMargin - 200 + 730, 30);
    // Tạo các nút để đăng kí tài khoản
    // Text tiêu đề các thành phần để nhập liệu đăng kí
    sf::Text lastNameText = createText("Last Name:", font, 20, sf::Color::Black, windowSize.x - rightMargin - 350, 200);
    sf::Text middleNameText = createText("Middle Name:", font, 20, sf::Color::Black, windowSize.x - rightMargin - 350, 260);
    sf::Text firstNameText = createText("First Name:", font, 20, sf::Color::Black, windowSize.x - rightMargin - 350, 320);
    sf::Text emailText = createText("Email:", font, 20, sf::Color::Black, windowSize.x - rightMargin - 350, 380);
    sf::Text phoneText = createText("Phone:", font, 20, sf::Color::Black, windowSize.x - rightMargin - 350, 440);
    sf::Text usernameText_SIGNUP = createText("Username:", font, 20, sf::Color::Black, windowSize.x - rightMargin - 350, 500);
    sf::Text passwordText_SIGNUP = createText("Password:", font, 20, sf::Color::Black, windowSize.x - rightMargin - 350, 560);

    // Đối tượng text để hiển thị đầu vào
    sf::Text lastNameInputText = createText("", font, 20, sf::Color::Black, windowSize.x - rightMargin - 200, posY + 7);
    sf::Text middleNameInputText = createText("", font, 20, sf::Color::Black, windowSize.x - rightMargin - 200, posY + 60 + 7);
    sf::Text firstNameInputText = createText("", font, 20, sf::Color::Black, windowSize.x - rightMargin - 200, posY + 2*60 + 7); 
    sf::Text emailInputText = createText("", font, 20, sf::Color::Black, windowSize.x - rightMargin - 200, posY + 3*60 + 7);
    sf::Text phoneInputText = createText("", font, 20, sf::Color::Black, windowSize.x - rightMargin - 200, posY + 4*60 + 7);
    sf::Text usernameInputText_SIGNUP = createText("", font, 20, sf::Color::Black, windowSize.x - rightMargin - 200, posY + 5*60 + 7);
    sf::Text passwordInputText_SIGNUP = createText("", font, 20, sf::Color::Black, windowSize.x - rightMargin - 200, posY + 6*60 + 7);

    bool isLastName = false;
    bool isMiddleName = false;
    bool isFirstName = false;
    bool isEmail = false;
    bool isPhone = false;
    bool isUsername_SIGNUP = false;
    bool isPassword_SIGNUP = false;

    sf::Texture cartIcon;
    cartIcon.loadFromFile("Graphics/Picture/cart.png");

    int countProduct_1 = 0;
    string *imagePaths_1 = FileManager::readFromFile("text\\DisplayDevice\\imagePaths1.txt", countProduct_1);
    string *nameProducts_1 = FileManager::readFromFile("text\\DisplayDevice\\nameProducts1.txt", countProduct_1);
    string *nameProductScreens_1 = FileManager::readFromFile("text\\DisplayDevice\\nameProductScreen1.txt", countProduct_1);
    string *prices_1 = FileManager::readFromFile("text\\DisplayDevice\\Price1.txt", countProduct_1);
    string *tinhNangNoiBat_1 = FileManager::readFromFile("text\\DisplayDevice\\tinhNangNoiBat1.txt", countProduct_1);

    sf::RectangleShape productss_1[countProduct_1];
    sf::RectangleShape cartButtons_1[countProduct_1];
    sf::Sprite cartSprites_1[countProduct_1];
    sf::Sprite productSprites_1[countProduct_1];
    sf::Texture productTextures_1[countProduct_1];
    sf::Text nameProductss_1[countProduct_1];
    sf::Text priceProductss_1[countProduct_1];

    for (int i = 0; i < countProduct_1; i++) {
        int row = i / 4;  // Tính số hàng
        int col = i % 4;  // Tính số cột

        float posX = col * (320) + 66;  // Điều chỉnh chiều rộng giữa các sản phẩm
        float posY = row * (450) + 130;   // Điều chỉnh chiều cao giữa các hàng
        
        loadProductImages();
        productTextures_1[i].loadFromFile(imagePaths_1[i]);
        
        createProductDisplay(
            productss_1[i], productSprites_1[i], nameProductss_1[i], priceProductss_1[i], 
            productTextures_1[i], nameProducts_1[i], prices_1[i], font, 
            0.35f, 0.35f,
            posX, posY,  // Đặt vị trí sản phẩm
            posX + 50, posY + 50,  // Đặt vị trí hình ảnh sản phẩm
            cartButtons_1[i], cartSprites_1[i], cartIcon, 
            posX + 250, posY + 350  // Đặt vị trí nút giỏ hàng
        );
    }
    int countProduct_2 = 0;
    string *imagePaths_2 = FileManager::readFromFile("text\\DisplayDevice\\imagePaths2.txt", countProduct_2);
    string *nameProducts_2 = FileManager::readFromFile("text\\DisplayDevice\\nameProducts2.txt", countProduct_2);
    string *nameProductScreens_2 = FileManager::readFromFile("text\\DisplayDevice\\nameProductScreen2.txt", countProduct_2);
    string *prices_2 = FileManager::readFromFile("text\\DisplayDevice\\Price2.txt", countProduct_2);
    string *tinhNangNoiBat_2 = FileManager::readFromFile("text\\DisplayDevice\\tinhNangNoiBat2.txt", countProduct_2);

    sf::RectangleShape productss_2[countProduct_2];
    sf::RectangleShape cartButtons_2[countProduct_2];
    sf::Sprite cartSprites_2[countProduct_2];
    sf::Sprite productSprites_2[countProduct_2];
    sf::Texture productTextures_2[countProduct_2];
    sf::Text nameProductss_2[countProduct_2];
    sf::Text priceProductss_2[countProduct_2];
    
for (int i = 0; i < countProduct_2; ++i) {
    int row = i / 4;  // Tính số hàng
    int col = i % 4;  // Tính số cột

    float posX = col * (320) + 66;  // Điều chỉnh chiều rộng giữa các sản phẩm
    float posY = row * (450) + 130;   // Điều chỉnh chiều cao giữa các hàng
    
    loadProductImages();
    productTextures_2[i].loadFromFile(imagePaths_2[i]);
    
    createProductDisplay(
        productss_2[i], productSprites_2[i], nameProductss_2[i], priceProductss_2[i], 
        productTextures_2[i], nameProducts_2[i], prices_2[i], font, 
        0.35f, 0.35f,
        posX, posY,  // Đặt vị trí sản phẩm
        posX + 50, posY + 50,  // Đặt vị trí hình ảnh sản phẩm
        cartButtons_2[i], cartSprites_2[i], cartIcon, 
        posX + 250, posY + 350  // Đặt vị trí nút giỏ hàng
    );
}
    int countProduct_3 = 0;
    string *imagePaths_3 = FileManager::readFromFile("text\\DisplayDevice\\imagePaths3.txt", countProduct_3);
    string *nameProducts_3 = FileManager::readFromFile("text\\DisplayDevice\\nameProducts3.txt", countProduct_3);
    string *nameProductScreens_3 = FileManager::readFromFile("text\\DisplayDevice\\nameProductScreen3.txt", countProduct_3);
    string *prices_3 = FileManager::readFromFile("text\\DisplayDevice\\Price3.txt", countProduct_3);
    string *tinhNangNoiBat_3 = FileManager::readFromFile("text\\DisplayDevice\\tinhNangNoiBat3.txt", countProduct_3);

    sf::RectangleShape productss_3[countProduct_3];
    sf::RectangleShape cartButtons_3[countProduct_3];
    sf::Sprite cartSprites_3[countProduct_3];
    sf::Sprite productSprites_3[countProduct_3];
    sf::Texture productTextures_3[countProduct_3];
    sf::Text nameProductss_3[countProduct_3];
    sf::Text priceProductss_3[countProduct_3];

    for (int i = 0; i < countProduct_3; ++i) {
    int row = i / 4;  // Tính số hàng
    int col = i % 4;  // Tính số cột

    float posX = col * (320) + 66;  // Điều chỉnh chiều rộng giữa các sản phẩm
    float posY = row * (450) + 130;   // Điều chỉnh chiều cao giữa các hàng
    
    loadProductImages();
    productTextures_3[i].loadFromFile(imagePaths_3[i]);
    
    createProductDisplay(
        productss_3[i], productSprites_3[i], nameProductss_3[i], priceProductss_3[i], 
        productTextures_3[i], nameProducts_3[i], prices_3[i], font, 
        0.35f, 0.35f,
        posX, posY,  // Đặt vị trí sản phẩm
        posX + 50, posY + 50,  // Đặt vị trí hình ảnh sản phẩm
        cartButtons_3[i], cartSprites_3[i], cartIcon, 
        posX + 250, posY + 350  // Đặt vị trí nút giỏ hàng
    );
}
    int countProduct_4 = 0;
    string *imagePaths_4 = FileManager::readFromFile("text\\DisplayDevice\\imagePaths4.txt", countProduct_4);
    string *nameProducts_4 = FileManager::readFromFile("text\\DisplayDevice\\nameProducts4.txt", countProduct_4);
    string *nameProductScreens_4 = FileManager::readFromFile("text\\DisplayDevice\\nameProductScreen4.txt", countProduct_4);
    string *prices_4 = FileManager::readFromFile("text\\DisplayDevice\\Price4.txt", countProduct_4);
    string *tinhNangNoiBat_4 = FileManager::readFromFile("text\\DisplayDevice\\tinhNangNoiBat4.txt", countProduct_4);

    sf::RectangleShape productss_4[countProduct_4];
    sf::RectangleShape cartButtons_4[countProduct_4];
    sf::Sprite cartSprites_4[countProduct_4];
    sf::Sprite productSprites_4[countProduct_4];
    sf::Texture productTextures_4[countProduct_4];
    sf::Text nameProductss_4[countProduct_4];
    sf::Text priceProductss_4[countProduct_4];

    for (int i = 0; i < countProduct_4; ++i) {
    int row = i / 4;  // Tính số hàng
    int col = i % 4;  // Tính số cột

    float posX = col * (320) + 66;  // Điều chỉnh chiều rộng giữa các sản phẩm
    float posY = row * (450) + 130;   // Điều chỉnh chiều cao giữa các hàng
    
    loadProductImages();
    productTextures_4[i].loadFromFile(imagePaths_4[i]);
    
    createProductDisplay(
        productss_4[i], productSprites_4[i], nameProductss_4[i], priceProductss_4[i], 
        productTextures_4[i], nameProducts_4[i], prices_4[i], font, 
        0.35f, 0.35f,
        posX, posY,  // Đặt vị trí sản phẩm
        posX + 50, posY + 50,  // Đặt vị trí hình ảnh sản phẩm
        cartButtons_4[i], cartSprites_4[i], cartIcon, 
        posX + 250, posY + 350  // Đặt vị trí nút giỏ hàng
        );
    }
    sf::RectangleShape lapTopButton = createOutlinedRectangle(200, 30, sf::Color(200, 200, 200), 2, sf::Vector2f(windowSize.x - rightMargin - 200 - 200, posY - 100)); // Nút để chuyển sang danh mục mua Laptop
    sf::Text lapTopText = createButtonText("Laptop", font, 20, sf::Color::Black, lapTopButton);
    sf::RectangleShape dienThoaiButton = createOutlinedRectangle(200, 30, sf::Color(200, 200, 200), 2, sf::Vector2f(windowSize.x - rightMargin - 200 - 450, posY - 100));
    sf::Text dienThoaiText = createButtonText("Điện thoại", font, 20, sf::Color::Black, dienThoaiButton);
    sf::RectangleShape taiNgheButton = createOutlinedRectangle(200, 30, sf::Color(200, 200, 200), 2, sf::Vector2f(windowSize.x - rightMargin - 200 + 50, posY - 100));
    sf::Text taiNgheText = createButtonText("Tai nghe", font, 20, sf::Color::Black, taiNgheButton);
    sf::RectangleShape tabletButton = createOutlinedRectangle(200, 30, sf::Color(200, 200, 200), 2, sf::Vector2f(windowSize.x - rightMargin - 200 + 300, posY - 100));
    sf::Text tabletText = createButtonText("Tablet", font, 20, sf::Color::Black, tabletButton);
    
    // Chi tiết các sản phẩm 
    sf::RectangleShape detailProducts_1[countProduct_1];
    sf::RectangleShape muaNgayButtons_1[countProduct_1];
    sf::RectangleShape cartButtons_PS_1[countProduct_1];
    sf::Sprite cartSprites_PS_1[countProduct_1];
    sf::Text muaNgayTexts_1[countProduct_1];
    sf::RectangleShape detailSpecificationButtons_1[countProduct_1];
    sf::Text detailSpecificationTexts_1[countProduct_1];

    for (int i = 0; i < countProduct_1; i++) {
        createProductDetail(detailProducts_1[i], muaNgayButtons_1[i], muaNgayTexts_1[i], cartButtons_PS_1[i], cartSprites_PS_1[i], "MUA NGAY", font_bold, windowSize, rightMargin, posY, cartIcon);
        detailSpecificationButtons_1[i] = createButton(340, 40, 1010, 600, sf::Color(250, 50, 50));
        detailSpecificationTexts_1[i] = createButtonText("Xem cấu hình chi tiết", font, 20, sf::Color::White, detailSpecificationButtons_1[i]);
    }
    sf::RectangleShape detailProducts[countProduct_2];
    sf::RectangleShape muaNgayButtons[countProduct_2];
    sf::RectangleShape cartButtons_PS[countProduct_2];
    sf::Sprite cartSprites_PS[countProduct_2];
    sf::Text muaNgayTexts[countProduct_2];
    sf::RectangleShape detailSpecificationButtons[countProduct_2];
    sf::Text detailSpecificationTexts[countProduct_2];
    for (int i = 0; i < countProduct_2; i++) {
        createProductDetail(detailProducts[i], muaNgayButtons[i], muaNgayTexts[i], cartButtons_PS[i], cartSprites_PS[i], "MUA NGAY", font_bold, windowSize, rightMargin, posY, cartIcon);
        detailSpecificationButtons[i] = createButton(340, 40, 1010, 600, sf::Color(250, 50, 50));
        detailSpecificationTexts[i] = createButtonText("Xem cấu hình chi tiết", font, 20, sf::Color::White, detailSpecificationButtons[i]);
    }    
    sf::RectangleShape detailProducts_3[countProduct_3];
    sf::RectangleShape muaNgayButtons_3[countProduct_3];
    sf::RectangleShape cartButtons_PS_3[countProduct_3];
    sf::Sprite cartSprites_PS_3[countProduct_3];
    sf::Text muaNgayTexts_3[countProduct_3];
    sf::RectangleShape detailSpecificationButtons_3[countProduct_3];
    sf::Text detailSpecificationTexts_3[countProduct_3];
    for (int i = 0; i < countProduct_3; i++) {
        createProductDetail(detailProducts_3[i], muaNgayButtons_3[i], muaNgayTexts_3[i], cartButtons_PS_3[i], cartSprites_PS_3[i], "MUA NGAY", font_bold, windowSize, rightMargin, posY, cartIcon);
        detailSpecificationButtons_3[i] = createButton(340, 40, 1010, 600, sf::Color(250, 50, 50));
        detailSpecificationTexts_3[i] = createButtonText("Xem cấu hình chi tiết", font, 20, sf::Color::White, detailSpecificationButtons_3[i]);
    }    
    sf::RectangleShape detailProducts_4[countProduct_4];
    sf::RectangleShape muaNgayButtons_4[countProduct_4];
    sf::RectangleShape cartButtons_PS_4[countProduct_4];
    sf::Sprite cartSprites_PS_4[countProduct_4];
    sf::Text muaNgayTexts_4[countProduct_4];
    sf::RectangleShape detailSpecificationButtons_4[countProduct_4];
    sf::Text detailSpecificationTexts_4[countProduct_4];
    for (int i = 0; i < countProduct_4; i++) {
        createProductDetail(detailProducts_4[i], muaNgayButtons_4[i], muaNgayTexts_4[i], cartButtons_PS_4[i], cartSprites_PS_4[i], "MUA NGAY", font_bold, windowSize, rightMargin, posY, cartIcon);
        detailSpecificationButtons_4[i] = createButton(340, 40, 1010, 600, sf::Color(250, 50, 50));
        detailSpecificationTexts_4[i] = createButtonText("Xem cấu hình chi tiết", font, 20, sf::Color::White, detailSpecificationButtons_3[i]);
    }    
    // Tạo các khối trong màn hình chi tiết của các sản phẩm
    string specification_Product; // Chuỗi chứa cấu hình chi tiết của các sản phẩm

    // Nút thanh toán 
    sf::RectangleShape payButton = createButton(330, 50, windowSize.x - rightMargin + 290, 500, sf::Color::Black);
    sf::Text payText = createButtonText("Pay", font, 20, sf::Color::White, payButton);

    // Nút xóa sản phẩm khỏi giỏ hàng
    sf::RectangleShape removeProductButton_1 = createButton(50, 50, 50, posY + 150, sf::Color::Black);
    sf::Text removeProductText_1 = createButtonText("X", font, 20, sf::Color::White, removeProductButton_1);

    // Tạo view
    sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)); 
    float viewTopLimit = 0;
    float viewBottomLimit = 768 * 2;
    sf::Vector2f initialCenter = view.getCenter();

    // Các chi tiết trong màn hình PAY_SCREEN
    sf::RectangleShape addressCustomer = createOutlinedRectangle(680, 50, sf::Color(200, 200, 200), 3, sf::Vector2f(windowSize.x - rightMargin - 650, posY + 4*60 + 110));
    sf::RectangleShape confirmPay = createButton(280, 60, windowSize.x - rightMargin - 250, posY + 4*60 + 180, sf::Color(250, 50, 50));
    sf::Text confirmPayText = createButtonText("Pay", font, 20, sf::Color::White, confirmPay);
    // string addressInput;
    wstring addressInput;
    bool isAddress = false;
    sf::Text addressInputText = createText("", font, 20, sf::Color::Black, windowSize.x - rightMargin - 645, posY + 4*60 + 120); // Text để hiển thị đầu vào
    double priceDiscount = 0.0; // Giá khi dùng voucher

    bool isDiscountCode = false;
    string discountCodeInput;
    sf::Text discountInputText = createText("", font, 20, sf::Color::Black, 820, posY + 50);
    sf::RectangleShape useDiscountCodeButton = createButton(80, 45, 820, posY + 50, sf::Color(169, 169, 169));

    // Nút đăng xuất
    sf::RectangleShape logOut = createButton(110, 30, windowSize.x - rightMargin - 600, posY + 100, sf::Color::Transparent);
    sf::Text logOutText = createButtonText("Log out", font, 20, sf::Color(61, 169, 226), logOut);
    // Nút giỏ hàng khác ở trong màn hình PAY_SCREEN
    sf::RectangleShape cartButtonPAY = createButton(60, 30, windowSize.x - rightMargin - 660, posY + 4*60 + 195, sf::Color::Transparent);
    sf::Text cartButtonPAYText = createButtonText("Cart", font, 20, sf::Color(61, 169, 226), cartButtonPAY);
    
    // Nút xem thông tin tài khoản
    sf::RectangleShape infoAccountButton = createButton(50, 50, windowSize.x - rightMargin - 200 + 670, 30, sf::Color::Transparent);
    sf::Texture infoAccountIcon;
    infoAccountIcon.loadFromFile("Graphics/Picture/Portrait.png");
    sf::Sprite spriteInfoAccountIcon;
    spriteInfoAccountIcon.setTexture(infoAccountIcon);
    spriteInfoAccountIcon.setScale(0.09f, 0.09f);
    spriteInfoAccountIcon.setPosition(windowSize.x - rightMargin - 200 + 670, 30);

    // Các chi tiết trong màn hình INFO ACCOUNT SCREEN
    sf::RectangleShape dangXuatButton = createButton(80, 30, 40, 260, sf::Color::Transparent);
    sf::Text dangXuatText = createButtonText("Log out", font, 20, sf::Color(21, 91, 246), dangXuatButton);
    sf::RectangleShape changePasswordButton = createButton(160, 30, 40, 300, sf::Color::Transparent);
    sf::Text changPasswordText = createButtonText("Change password", font, 20, sf::Color(21, 91, 246), changePasswordButton);

    // Nút Kiểm tra lịch sử đơn hàng
    sf::RectangleShape historyOrderButton = createButton(50, 50,  windowSize.x - rightMargin - 200 + 610, 30, sf::Color::Transparent);
    sf::Texture historyOrderIcon;
    historyOrderIcon.loadFromFile("Graphics/Picture/historyOrder.png");
    sf::Sprite spriteHistoryOrder;
    spriteHistoryOrder.setTexture(historyOrderIcon);
    spriteHistoryOrder.setScale(0.098f, 0.098f);
    spriteHistoryOrder.setPosition(windowSize.x - rightMargin - 200 + 610, 30);

    // Các chi tiết trong màn hình HISTORY_ORDER
    string phoneHistoryOrderInput, orderCodeHistoryOrderInput;
    bool isOrderCodeHistoryOrder = false;
    sf::RectangleShape phoneHistoryOrder = createButton(425, 35, 140, 280, sf::Color::White);
    sf::RectangleShape orderCodeHistory = createButton(425, 35, 140, 360, sf::Color::White);
    sf::Text orderCodeHistoryInputText = createText("", font, 20, sf::Color::Black, 145, 363); // Text để hiển thị đầu vào
    sf::RectangleShape displayHistoryButton = createButton(140, 40, 425, 550, sf::Color(230, 126, 34));
    sf::Text displayHistoryText = createButtonText("Xem ngay", font, 20, sf::Color::White, displayHistoryButton);
    Products *data = new Products;
    int countProductsInOrder = 0; // Số sản phẩm có trong đơn hàng

    // Chi tiết trong màn hình ADMIN_SCREEN
    sf::RectangleShape displayCustomerButton = createOutlinedRectangle(180, 30, sf::Color(150, 150, 150), 2, sf::Vector2f(40, 140));
    sf::Text displayCustomerText = createButtonText("Khách hàng", font, 20, sf::Color::Black, displayCustomerButton);
    sf::RectangleShape displayProductButton = createOutlinedRectangle(180, 30, sf::Color(150, 150, 150), 2, sf::Vector2f(40, 200));
    sf::Text displayProductText = createButtonText("Sản phẩm", font, 20, sf::Color::Black, displayProductButton);
    sf::RectangleShape revenueButton = createOutlinedRectangle(180, 30, sf::Color(150, 150, 150), 2, sf::Vector2f(40, 260));
    sf::Text revenueText = createButtonText("Doanh thu", font, 20, sf::Color::Black, revenueButton);
    sf::RectangleShape discountCodeButton = createOutlinedRectangle(180, 30, sf::Color(150, 150, 150), 2, sf::Vector2f(40, 320));
    sf::Text discountCodeText = createButtonText("Giảm giá", font, 20, sf::Color::Black, discountCodeButton);
    sf::RectangleShape addProductButton = createOutlinedRectangle(180, 30, sf::Color(150, 150, 150), 2, sf::Vector2f(40, 380));
    sf::Text addProductText = createButtonText("Thêm sản phẩm", font, 20, sf::Color::Black, addProductButton);
    sf::RectangleShape detailStatisticsButton = createOutlinedRectangle(180, 30, sf::Color(150, 150, 150), 2, sf::Vector2f(40, 440));
    sf::Text detailStatisticsText = createButtonText("Thống kê chi tiết", font, 20, sf::Color::Black, detailStatisticsButton);

    // Các chi tiết trong màn hình DISPLAYCUSTOMER_SCREEN
    sf::RectangleShape searchCustomerButton = createButton(920, 40, 270, 90, sf::Color::White);
    sf::Text searchCustomerText = createButtonText("Search for customers.", font, 20, sf::Color(150, 150, 150), searchCustomerButton);
    searchCustomerText.setPosition(280, 95);
    sf::Text searchCustomerInputText = createText("", font, 20, sf::Color::Black, 280, 95);
    wstring customerInput;
    bool isSearchCustomer;
    sf::RectangleShape confirmSearchCustomerButton = createButton(80, 40, 1200, 90, sf::Color::White);
    sf::Texture searchIcon;
    searchIcon.loadFromFile("Graphics/Picture/search.png");
    sf::Sprite spriteSearchIcon;
    spriteSearchIcon.setTexture(searchIcon);
    spriteSearchIcon.setScale(0.3125f, 0.3125f);
    spriteSearchIcon.setPosition(1220, 90);

    int countFoundCustomer = 0; // Số lượng khách hàng tìm thấy
    Customer *resultFoundCustomer; // Mảng chứa khách hàng tìm được

    // Nút sắp xếp danh sách khách hàng
    sf::RectangleShape sortCustomerButton = createButton(20, 20, 510, 150, sf::Color::White);
    sf::Texture sortIcon;
    sortIcon.loadFromFile("Graphics/Picture/sort.png");
    sf::Sprite spriteSort;
    spriteSort.setTexture(sortIcon);
    spriteSort.setScale(0.04f, 0.04f);
    spriteSort.setPosition(510, 150);
    bool useAlgorithmSortCustomer = true;

    // Các chi tiết trong màn hình DISPLAYPRODUCT_SCREEN
    sf::RectangleShape searchProductButton = createButton(920, 40, 270, 90, sf::Color::White);
    sf::Text searchProductText = createButtonText("Search for products.", font, 20, sf::Color(150, 150, 150), searchProductButton);
    searchProductText.setPosition(280, 95);
    sf::Text searchProductInputText = createText("", font, 20, sf::Color::Black, 280, 95);
    string productInput;
    bool isSearchProduct;
    sf::RectangleShape confirmSearchProductButton = createButton(80, 40, 1200, 90, sf::Color::White);
    
    int countFoundProduct = 0; // Số lượng sản phẩm tìm thấy
    Products *resultFoundProducts; // Mảng chứa sản phẩm tìm được
    bool useAlgorithmSortProducts = true;
    int countProduct = countProduct_1 + countProduct_2 + countProduct_3 + countProduct_4;
    sf::RectangleShape removeProductButton[countProduct];
    sf::Text removeProductText[countProduct];
    sf::RectangleShape editProductButton[countProduct];
    sf::Text editProductText[countProduct];

    // Nút sắp xếp dánh sách sản phẩm theo ten
    sf::RectangleShape sortProductButton = createButton(20, 20, 510, 150, sf::Color::White);

    // Nút sắp xếp sản phẩm theo giá tiền
    sf::RectangleShape sortProductsPriceButton = createButton(20, 20, 660, 150, sf::Color::White);
    sf::Sprite spriteSortProductPrice;
    spriteSortProductPrice.setTexture(sortIcon);
    spriteSortProductPrice.setScale(0.04f, 0.04f);
    spriteSortProductPrice.setPosition(660, 150);
    bool useAlgorithmSortProductPrice = true;

    // Các chi tiết trong màn hình DISCOUNT_CODE
    sf::Text generateDiscountCodeText = createText("Mã giảm giá:", font, 20, sf::Color::Black, 300, 205);
    sf::Text priceDiscountText = createText("Tiền:", font, 20, sf::Color::Black, 300, 260);
    sf::Text quantityDiscountText = createText("Số lượng mã:", font, 20, sf::Color::Black, 300, 315);
    sf::RectangleShape discountCodeInputButton = createButton(600, 40, 450, 200, sf::Color::White);
    sf::RectangleShape priceDiscountInputButton = createButton(600, 40, 450, 250, sf::Color::White);
    sf::RectangleShape quantityDiscountInputButton = createButton(600, 40, 450, 300, sf::Color::White);
    sf::Text discountCodeInputText = createText("", font, 20, sf::Color::Black, 460, 205);
    sf::Text priceDiscountInputText = createText("", font, 20, sf::Color::Black, 460, 255);
    sf::Text quantityDiscountInputText = createText("", font, 20, sf::Color::Black, 460, 305);
    string maGiamGia, giaDuocGiam, soLuongMa;
    bool isMaGiamGia = false, isGiaDuocGiam = false, isSoLuongMa = false;
    double giaDuocGiamDouble;
    int soLuongMaInt;

    sf::RectangleShape confirmGenerateDiscountCodeButton = createButton(120, 30, 930, 350, sf::Color::Red);
    sf::Text confirmGenerateDiscountCodeText = createButtonText("Cấp", font, 20, sf::Color::White, confirmGenerateDiscountCodeButton);

    // Các chi tiết trong màn hình EDITPRODUCT_SCREEN
    sf::Text editNameProductText = createText("Tên sản phẩm:", font, 20, sf::Color::Black, 300, 200);
    sf::Text editGenreText = createText("Thể loại:", font, 20, sf::Color::Black, 300, 260);
    sf::Text editPriceProductText = createText("Giá tiền:", font, 20, sf::Color::Black, 300, 320);
    sf::Text editManufacturerText = createText("Nhà sản xuất:", font, 20, sf::Color::Black, 300, 380);
    sf::Text editOperatingSystemText = createText("Hệ điều hành:", font, 20, sf::Color::Black, 300, 440);
    sf::Text editQuantityText = createText("Số lượng:", font, 20, sf::Color::Black, 300, 500);

    sf::RectangleShape editNameProductInputButton = createButton(600, 40, 470, 200, sf::Color::White);
    sf::RectangleShape editGenreInputButton = createButton(600, 40, 470, 260, sf::Color::White);
    sf::RectangleShape editPriceProductInputButton = createButton(600, 40, 470, 320, sf::Color::White);
    sf::RectangleShape editManufacturerInputButton = createButton(600, 40, 470, 380, sf::Color::White);
    sf::RectangleShape editOperatingSystemInputButton = createButton(600, 40, 470, 440, sf::Color::White);
    sf::RectangleShape editQuantityInputButton = createButton(600, 40, 470, 500, sf::Color::White);

    sf::Text editNameProductInputText = createText("", font, 20, sf::Color::Black, 480, 205);
    sf::Text editGenreInputText = createText("", font, 20, sf::Color::Black, 480, 265);
    sf::Text editPriceProductInputText = createText("", font, 20, sf::Color::Black, 480, 325);
    sf::Text editManufacturerInputText = createText("", font, 20, sf::Color::Black, 480, 385);
    sf::Text editOperatingSystemInputText = createText("", font, 20, sf::Color::Black, 480, 445);
    sf::Text editQuantityInputText = createText("", font, 20, sf::Color::Black, 480, 505);
    
    bool isEditNameProduct = false, isEditGenre = false, isEditPriceProduct = false, isEditManufacturer = false, isEditOperatingSystem = false, isEditQuantity = false;
    string editNameProduct, editGenre, editPriceProduct, editManufacturer, editOperatingSystem, editQuantity;
    int editQuantityINT;
    double editPriceProductDouble;

    sf::RectangleShape confirmEditProductButton = createButton(120, 30, 950, 555, sf::Color::Red);
    sf::Text confirmEditProductText = createButtonText("Xác nhận", font_bold, 20, sf::Color::White, confirmEditProductButton);
    int editProductID; // mã khách hàng khi edit

    // Các chi tiết trong màn hình REVENUE
    const int numberOfStatisticalDays = 11; // Số ngày thống kê
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
    // Truy xuất đến ngày, tháng
    string day[numberOfStatisticalDays];
    string month[numberOfStatisticalDays];
    for (int i = 0; i < numberOfStatisticalDays; i++) {
        string date = recentDay[i];
        
        day[i] = date.substr(0, 2);
        month[i] = date.substr(3, 2);
    }

    sf::RectangleShape revenue[numberOfStatisticalDays]; // Mảng chứa các cột trong đồ thị
    sf::Text dayText[numberOfStatisticalDays]; // Mảng chứa ngày
    sf::Text monthText[numberOfStatisticalDays]; // Mảng chứa tháng

    // Các chi tiết trong màn hình ADDPRODUCT
    sf::Text fileNameProductText = createText("File sản phẩm: ", font, 20, sf::Color::Black, 300, 200);
    sf::Text fileImageProductText = createText("File ảnh sản phẩm: ", font, 20, sf::Color::Black, 300, 260);
    sf::Text productIDText = createText("Mã sản phẩm: ", font, 20, sf::Color::Black, 300, 320);
    sf::Text genreText = createText("Thể loại: ", font, 20, sf::Color::Black, 300, 380);
    sf::Text nameProductText = createText("Tên sản phẩm: ", font, 20, sf::Color::Black, 300, 440);
    sf::Text priceProductText = createText("Giá sản phẩm: ", font, 20, sf::Color::Black, 300, 500);
    sf::Text manufacturerText = createText("Nhà sản xuất: ", font, 20, sf::Color::Black, 300, 560);
    sf::Text operatingSystemText = createText("Hệ điều hành: ", font, 20, sf::Color::Black, 300, 620);
    sf::Text quantityText = createText("Số lượng: ", font, 20, sf::Color::Black, 300, 680);

    sf::RectangleShape fileNameProductInputButton = createButton(600, 40, 550, 200, sf::Color::White);
    sf::RectangleShape fileImageProductInputButton = createButton(600, 40, 550, 260, sf::Color::White);
    sf::RectangleShape productIDInputButton = createButton(600, 40, 550, 320, sf::Color::White);
    sf::RectangleShape genreInputButton = createButton(600, 40, 550, 380, sf::Color::White);
    sf::RectangleShape nameProductInputButton = createButton(600, 40, 550, 440, sf::Color::White);
    sf::RectangleShape priceProductInputButton = createButton(600, 40, 550, 500, sf::Color::White);
    sf::RectangleShape manufacturerInputButton = createButton(600, 40, 550, 560, sf::Color::White);
    sf::RectangleShape operatingSystemInputButton = createButton(600, 40, 550, 620, sf::Color::White);
    sf::RectangleShape quantityInputButton = createButton(600, 40, 550, 680, sf::Color::White);

    sf::Text fileNameProductInputText = createText("", font, 20, sf::Color::Black, 555, 205);
    sf::Text fileImageProductInputText = createText("", font, 20, sf::Color::Black, 555, 265);
    sf::Text productIDInputText = createText("", font, 20, sf::Color::Black, 555, 325);
    sf::Text genreInputText = createText("", font, 20, sf::Color::Black, 555, 385);
    sf::Text nameProductInputText = createText("", font, 20, sf::Color::Black, 555, 445);
    sf::Text priceProductInputText = createText("", font, 20, sf::Color::Black, 555, 505);
    sf::Text manufacturerInputText = createText("", font, 20, sf::Color::Black, 555, 565);
    sf::Text operatingSystemInputText = createText("", font, 20, sf::Color::Black, 555, 625);
    sf::Text quantityInputText = createText("", font, 20, sf::Color::Black, 555, 685);

    bool isFileNameProduct = false, isFileImageProduct = false,  isProductID = false, isGenre = false,
         isNameProduct = false, isPriceProduct = false, isManufacturer = false, isOperatingSystem = false, isQuantity = false;
    wstring nameProduct;
    string fileNameProduct, fileImageProduct,  genre, manufacturer, operatingSystem, productID, quantity, priceProduct;
    int productID_int, quantity_int;
    double priceProduct_double;

    sf::RectangleShape confirmAddProductButton = createButton(120, 30, 1030, 740, sf::Color::Red);
    sf::Text confirmAddProductText = createButtonText("Xác nhận", font_bold, 20, sf::Color::White, confirmAddProductButton);
    
    // Các chi tiết trong màn hình Thống kê chi tiết DETAILSTATISTIC
    sf::RectangleShape thongKeTheoNgayButton = createButton(920, 40, 270, 150, sf::Color::White);
    sf::Text thongKeTheoNgayText = createButtonText("dd-mm-yyyy.", font, 20, sf::Color(150, 150, 150), thongKeTheoNgayButton);
    thongKeTheoNgayText.setPosition(280, 155);
    sf::RectangleShape confirmStatisticButton = createButton(80, 40, 1200, 150, sf::Color::White);
    sf::Sprite spriteStatistic;
    spriteStatistic.setTexture(searchIcon);
    spriteStatistic.setScale(0.3125f, 0.3125f);
    spriteStatistic.setPosition(1220, 150);
    History *lichSuDonHang = nullptr;
    int countPay = 0;
    bool isDate = false;
    string dateToFind;
    double totalPrice = 0.0;
    sf::Text thongKeTheoNgayInputText = createText("", font, 20, sf::Color::Black, 275, 155);

while (window.isOpen())
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
        
        // Xử lý chuột
        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            // Lấy vị trí chuột trên màn hình
            sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
            // Chuyển đổi vị trí chuột sang không gian của View
            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePixelPos, view);

            // Kiểm tra click vào nút Login
            if (screenState == MAIN_MENU)
            {
                // Kiểm tra click vào nút Signup
                if (signupButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    screenState = SIGNUP_SCREEN;  // Chuyển sang màn hình đăng ký
                }
                // Kiểm tra khi click vào nút Forget Password
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (forgetPasswordButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            screenState = EMAIL_SCREEN;
                        }
                    }
                }
                // Kiểm tra click vào ô nhập username
                if (sf::FloatRect(rightMargin + 200, 200, 300, 50).contains(mousePos.x, mousePos.y))
                {
                    isTypingUsername = true;
                    isTypingPassword = false;
                }
                else
                {
                    isTypingUsername = false; // Nếu click ra ngoài thì ẩn con trỏ
                }

                // Kiểm tra click vào ô nhập password
                if (sf::FloatRect(rightMargin + 200, 300, 300, 50).contains(mousePos.x, mousePos.y))
                {
                    isTypingPassword = true;
                    isTypingUsername = false;
                }
                else
                {
                    isTypingPassword = false; // Nếu click ra ngoài thì ẩn con trỏ
                }

                // Kiểm tra click vào nút Submit
                if (signInButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    if (!usernameInput.empty() && !passwordInput.empty())
                    {
                        std::cout << "Username: " << usernameInput << std::endl;
                        std::cout << "Password: " << passwordInput << std::endl;
                        // Thực hiện đăng nhập
                        int loginResult = customers->login(usernameInput, passwordInput, customers);
                        if (loginResult == 1) {
                            orders->setID(customers->getCustomerID());
                            std::cout << "Login successful!" << " Welcome, " << customers->getLastName() << " " << customers->getMiddleName() << " " << customers->getFirstName() <<  std::endl;
                            cout << "Customer ID: " << customers->getCustomerID() << endl;
                            orders->update();
                            screenState = PRODUCTS_SCREEN;
                        }
                        else if (loginResult == 2) {
                            std::cout << "Admin login successful!" << std::endl;
                            screenState = ADMIN_SCREEN;
                        }
                        else {
                            std::cout << "Login failed. Try again." << std::endl;
                        }
                    }
                    usernameInput.clear();
                    passwordInput.clear();
                    usernameInputText.setString("");
                    passwordInputText.setString("");
                }
            }
            else if (screenState == SIGNUP_SCREEN)
            {
                // Kiểm tra click vào nút Back
                if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    screenState = MAIN_MENU;  // Quay lại màn hình chính
                    lastNameInputText.setString("");
                    middleNameInputText.setString("");
                    firstNameInputText.setString("");
                    emailInputText.setString("");
                    phoneInputText.setString("");
                    usernameInputText_SIGNUP.setString("");
                    passwordInputText_SIGNUP.setString("");
                    lastNameInput.clear();
                    middleNameInput.clear();
                    firstNameInput.clear();
                    emailInput.clear();
                    phoneInput.clear();
                    usernameInput_SIGNUP.clear();
                    passwordInput_SIGNUP.clear();
                }

                // Kiểm tra click vào ô nhập lastName
                if (sf::FloatRect(windowSize.x - rightMargin - 200, posY, 300, 50).contains(mousePos.x, mousePos.y))
                {
                    isLastName = true;
                    isMiddleName = false;
                    isFirstName = false;
                    isEmail = false;
                    isPhone = false;
                    isUsername_SIGNUP = false;
                    isPassword_SIGNUP = false;
                }
                else
                {
                    isLastName = false; // Nếu click ra ngoài thì ẩn con trỏ
                }

                // Kiểm tra click vào ô nhập middleName
                if (sf::FloatRect(windowSize.x - rightMargin - 200, posY + 60, 300, 50).contains(mousePos.x, mousePos.y))
                {
                    isLastName = false;
                    isMiddleName = true;
                    isFirstName = false;
                    isEmail = false;
                    isPhone = false;
                    isUsername_SIGNUP = false;
                    isPassword_SIGNUP = false;
                }
                else
                {
                    isMiddleName = false; // Nếu click ra ngoài thì ẩn con trỏ
                }

                // Kiểm tra click vào ô nhập firstName
                if (sf::FloatRect(windowSize.x - rightMargin - 200, posY + 2*60, 300, 50).contains(mousePos.x, mousePos.y))
                {
                    isLastName = false;
                    isMiddleName = false;
                    isFirstName = true;
                    isEmail = false;
                    isPhone = false;
                    isUsername_SIGNUP = false;
                    isPassword_SIGNUP = false;
                }
                else
                {
                    isFirstName = false; // Nếu click ra ngoài thì ẩn con trỏ
                }

                // Kiểm tra click vào ô nhập email
                if (sf::FloatRect(windowSize.x - rightMargin - 200, posY + 3*60, 300, 50).contains(mousePos.x, mousePos.y))
                {
                    isLastName = false;
                    isMiddleName = false;
                    isFirstName = false;
                    isEmail = true;
                    isPhone = false;
                    isUsername_SIGNUP = false;
                    isPassword_SIGNUP = false;
                }
                else
                {
                    isEmail = false; // Nếu click ra ngoài thì ẩn con trỏ
                }

                // Kiểm tra click vào ô nhập phone
                if (sf::FloatRect(windowSize.x - rightMargin - 200, posY + 4*60, 300, 50).contains(mousePos.x, mousePos.y))
                {
                    isLastName = false;
                    isMiddleName = false;
                    isFirstName = false;
                    isEmail = false;
                    isPhone = true;
                    isUsername_SIGNUP = false;
                    isPassword_SIGNUP = false;
                }
                else
                {
                    isPhone = false; // Nếu click ra ngoài thì ẩn con trỏ
                }

                // Kiểm tra click vào ô nhập userName_SIGNUP
                if (sf::FloatRect(windowSize.x - rightMargin - 200, posY + 5*60, 300, 50).contains(mousePos.x, mousePos.y))
                {
                    isLastName = false;
                    isMiddleName = false;
                    isFirstName = false;
                    isEmail = false;
                    isPhone = false;
                    isUsername_SIGNUP = true;
                    isPassword_SIGNUP = false;
                }
                else
                {
                    isUsername_SIGNUP = false; // Nếu click ra ngoài thì ẩn con trỏ
                }

                // Kiểm tra click vào ô nhập password_SIGNUP
                if (sf::FloatRect(windowSize.x - rightMargin - 200, posY + 6*60, 300, 50).contains(mousePos.x, mousePos.y))
                {
                    isLastName = false;
                    isMiddleName = false;
                    isFirstName = false;
                    isEmail = false;
                    isPhone = false;
                    isUsername_SIGNUP = false;
                    isPassword_SIGNUP = true;
                }
                else
                {
                    isPassword_SIGNUP = false; // Nếu click ra ngoài thì ẩn con trỏ
                }

                // Kiểm tra click vào nút Submit
                if (registerAccount.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    if (!lastNameInput.empty() && !middleNameInput.empty() && !firstNameInput.empty() && !emailInput.empty() && !phoneInput.empty() && !usernameInput_SIGNUP.empty() && !passwordInput_SIGNUP.empty())
                    {
                        if (screenState == SIGNUP_SCREEN) {
                            string lastNameInputString = convertToString(lastNameInput);
                            string middleNameInputString = convertToString(middleNameInput);
                            string firstNameInputString = convertToString(firstNameInput);
                            customers->registerAccount(lastNameInputString, middleNameInputString, firstNameInputString, emailInput, phoneInput, usernameInput_SIGNUP, passwordInput_SIGNUP);
                            lastNameInput.clear();
                            lastNameInputText.setString("");
                            middleNameInput.clear();
                            middleNameInputText.setString("");
                            firstNameInput.clear();
                            firstNameInputText.setString("");
                            emailInput.clear();
                            emailInputText.setString("");
                            phoneInput.clear();
                            phoneInputText.setString("");
                            usernameInput_SIGNUP.clear();
                            usernameInputText_SIGNUP.setString("");
                            passwordInput_SIGNUP.clear();
                            passwordInputText_SIGNUP.setString("");
                            screenState = MAIN_MENU;
                       }
                    }
                }
            }
        else if (screenState == PRODUCTS_SCREEN) {
            // Xử lí khi click vào nút home và nút logoutButton 
            handleMouseClickEvents(event, mouseWorldPos, logOutButton, homeButton, screenState);
            // Xử lí khi click  vào nút Back (tạm thời)
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN;
                    }
                }
            }
            // Xử lí khi bấm vào nút dienThoaiButton để chuyển sang trang để xem sản phẩm là điện thoại
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (lapTopButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN2;
                    }
                }
            }
            // Xử lý khi click vào LoaButton để chuyển sang trang để sản xem sản phẩm là Loa
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (taiNgheButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN3;
                    }
                }
            }
            // Xử lý khi click vào TabletButton để chuyển sang trang để xem sản phẩm là Tablet
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (tabletButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN4;
                    }
                }
            }
            // Xử lý khi click vào nút xemm thông tin tài khoản
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (infoAccountButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = INFOACCOUNT_SCREEN;
                        cout << "Switch to the screen INFO ACCOUNT. \n";
                    }
                }
            }
            // Kiểm tra khi bấm vào nút ViewCart (Xem giỏ hàng)
            if (viewCart.getGlobalBounds().contains(mouseWorldPos)) {
                screenState = VIEWCART_SCREEN;
                orders->displayOrder();
                cout << "Clicked display orders. \n";
            }
            // Xử lí khi click vào nút kiểm tra lịch sử đơn hàng
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (historyOrderButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = HISTORYORDER_SCREEN;
                        cout << "Welcome to HISTORYORDER_SCREEN. \n";
                    }
                }
            }
            // Xử lí chuột khi click vào sản phẩm (dienthoai)
            for (int i = 0; i < countProduct_1; i++) {
                // Kiểm tra sự kiện nhấn chuột và chỉ xử lý một lần
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    // Kiểm tra nếu nhấn vào nút nhỏ trước
                    if (cartButtons_1[i].getGlobalBounds().contains(mouseWorldPos)) {
                        orders->displayProduct();
                        orders->addData(IDSanPham1[i]);
                        std::cout << "Add product " << nameProductScreens_1[i] << " view cart" << std::endl;
                    }
                    // Nếu không nhấn vào nút nhỏ, kiểm tra nút lớn
                    else if (productss_1[i].getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = static_cast<ScreenState>(screenProduct1[i]);
                    }
                }
            }
        }
        else if (screenState == PRODUCTS_SCREEN2) {
            // Xử lí chuột khi click vào sản phẩm (laptop)
            for (int i = 0; i < countProduct_2; i++) {
                // Kiểm tra sự kiện nhấn chuột và chỉ xử lý một lần
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    // Kiểm tra nếu nhấn vào nút nhỏ trước
                    if (cartButtons_2[i].getGlobalBounds().contains(mouseWorldPos)) {
                        orders->displayProduct();
                        orders->addData(IDSanPham2[i]);
                        std::cout << "Add product " << nameProductScreens_2[i] << " view cart" << std::endl;
                    }
                    // Nếu không nhấn vào nút nhỏ, kiểm tra nút lớn
                    else if (productss_2[i].getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = static_cast<ScreenState>(screenProduct2[i]);
                        std::cout << "Switch to the screen " << screenProduct2[i] << std::endl;
                    }
                }
            }
            // Xử lí khi click vào nút kiểm tra lịch sử đơn hàng
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (historyOrderButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = HISTORYORDER_SCREEN;
                        cout << "Welcome to HISTORYORDER_SCREEN. \n";
                    }
                }
            }
            // Xử lí khi click vào nút home và nút logoutButton 
            handleMouseClickEvents(event, mouseWorldPos, logOutButton, homeButton, screenState);
            // Xử lí khi click  vào nút Back (tạm thời)
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN;
                    }
                }
            }
            // Xử lí khi bấm vào nút dienThoaiButton để chuyển sang trang để xem sản phẩm là điện thoại
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (dienThoaiButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN;
                    }
                }
            }
            // Xử lý khi click vào LoaButton để chuyển sang trang để sản xem sản phẩm là Loa
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (taiNgheButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN3;
                    }
                }
            }
            // Xử lý khi click vào TabletButton để chuyển sang trang để xem sản phẩm là Tablet
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (tabletButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN4;
                    }
                }
            }
            // Xử lý khi click vào nút xemm thông tin tài khoản
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (infoAccountButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = INFOACCOUNT_SCREEN;
                        cout << "Switch to the screen INFO ACCOUNT. \n";
                    }
                }
            }
            // Kiểm tra khi bấm vào nút ViewCart (Xem giỏ hàng)
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (viewCart.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = VIEWCART_SCREEN;
                        orders->displayOrder();
                    }
                }
            }
        }
        else if (screenState == PRODUCTS_SCREEN3) {
            // Xử lí chuột khi click vào sản phẩm (dienthoai)
            for (int i = 0; i < countProduct_3; i++) {
                // Kiểm tra sự kiện nhấn chuột và chỉ xử lý một lần
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    // Kiểm tra nếu nhấn vào nút nhỏ trước
                    if (cartButtons_3[i].getGlobalBounds().contains(mouseWorldPos)) {
                        orders->displayProduct();
                        orders->addData(IDSanPham3[i]);
                        std::cout << "Add product " << nameProductScreens_3[i] << " view cart" << std::endl;
                    }
                    // Nếu không nhấn vào nút nhỏ, kiểm tra nút lớn
                    else if (productss_3[i].getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = static_cast<ScreenState>(screenProduct3[i]);
                    }
                }
            }
            // Xử lý chuột khi click vào nút back
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN;
                    }
                }
            }
            // Xử lí khi click vào nút home và nút logoutButton 
            handleMouseClickEvents(event, mouseWorldPos, logOutButton, homeButton, screenState);
            // Xử lý chuột khi click vào ô HISTORY ORDER
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (historyOrderButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = HISTORYORDER_SCREEN;
                        cout << "Welcome to HISTORYORDER_SCREEN. \n";
                    }
                }
            }
            // Xử lý chuột khi click vào ô INFOACCOUNT
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (infoAccountButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = INFOACCOUNT_SCREEN;
                        cout << "Switch to the screen INFO ACCOUNT. \n";
                    }
                }
            }
            // Xử lí chuột khi click vào ô giỏ hàng
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (viewCart.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = VIEWCART_SCREEN;
                        orders->displayOrder();
                    }
                }
            }
            // Xử lý chuột khi click vào nút DienThoaiButton và LaptopButton
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (dienThoaiButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN;
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (lapTopButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN2;
                    }
                }
            }
            // Xử lý khi click vào TabletButton để chuyển sang trang để xem sản phẩm là Tablet
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (tabletButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN4;
                    }
                }
            }
        }
        else if (screenState == PRODUCTS_SCREEN4) {
            // Xử lí chuột khi click vào sản phẩm (dienthoai)
            for (int i = 0; i < countProduct_4; i++) {
                // Kiểm tra sự kiện nhấn chuột và chỉ xử lý một lần
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    // Kiểm tra nếu nhấn vào nút nhỏ trước
                    if (cartButtons_4[i].getGlobalBounds().contains(mouseWorldPos)) {
                        orders->displayProduct();
                        orders->addData(IDSanPham4[i]);
                        std::cout << "Add product " << nameProductScreens_4[i] << " view cart" << std::endl;
                    }
                    // Nếu không nhấn vào nút nhỏ, kiểm tra nút lớn
                    else if (productss_3[i].getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = static_cast<ScreenState>(screenProduct4[i]);
                    }
                }
            }
            // Xử lý chuột khi click vào nút back
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN;
                    }
                }
            }
            // Xử lí khi click vào nút home và nút logoutButton 
            handleMouseClickEvents(event, mouseWorldPos, logOutButton, homeButton, screenState);
            // Xử lý khi click vào dienThoaiButton để chuyển sang trang để xem sản phẩm là Điện thoại
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (dienThoaiButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN;
                    }
                }
            }
            // Xử lý khi click vào lapTopButton để chuyển sang trang để xem sản phẩm là Laptop
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (lapTopButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN2;
                    }
                }
            }
            // Xử lý khi click vào taiNgheButton để chuyển sang trang để xem sản phẩm là Tai nghe
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (taiNgheButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN3;
                    }
                }
            }
            // Xử lý chuột khi click vào ô HISTORY ORDER
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (historyOrderButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = HISTORYORDER_SCREEN;
                        cout << "Welcome to HISTORYORDER_SCREEN. \n";
                    }
                }
            }
            // Xử lý chuột khi click vào ô INFOACCOUNT
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (infoAccountButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = INFOACCOUNT_SCREEN;
                        cout << "Switch to the screen INFO ACCOUNT. \n";
                    }
                }
            }
            // Xử lí chuột khi click vào ô giỏ hàng
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (viewCart.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = VIEWCART_SCREEN;
                        orders->displayOrder();
                    }
                }
            }
        }
            if (screenState == VIEWCART_SCREEN) {
                // Xử lí khi nhấp chuột vào nút Back 
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN;
                    }
                }
                // Xử lí khi click vào nút home và nút logoutButton 
                handleMouseClickEvents(event, mouseWorldPos, logOutButton, homeButton, screenState);
                // Xử lí chuột khi click vào nút Pay
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (payButton.getGlobalBounds().contains(mouseWorldPos)) {
                        if (orders->getCount() == 0) {
                            cout << "Cart is empty. \n";
                        } else screenState = PAY_SCREEN;
                    }
                }
            }
            if (screenState == EMAIL_SCREEN) {
                // Xử lí chuột khi click vào nút Back
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (backButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        screenState = MAIN_MENU;
                        emailForgetPassword.clear();
                        emailInputText_ChangePassword.setString("");
                    }
                }
                // Xử lí khi click vào nút Logout
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (logOutButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            screenState = MAIN_MENU;
                            cout << "Logout successfully. \n";
                        }
                    }
                }
                // Kiểm tra click vào ô nhập phone
                if (sf::FloatRect(windowSize.x - rightMargin - 350, 310 + 7, 600, 50).contains(mousePos.x, mousePos.y))
                {
                    isTypingPhoneChangePassword = true;
                }
                else
                {
                    isTypingPhoneChangePassword = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                // Kiểm tra khi click vào nút Continue
                if (!emailForgetPassword.empty()) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (confirmEmailButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            int checkEmail = customers->verifyEmail(emailForgetPassword);
                            emailForgetPasswordTmp = emailForgetPassword;
                            if (checkEmail == -1) {
                                cout << "Not found email. \n";
                            } 
                            else {
                                screenState = XACTHUC_SCREEN;
                                OTP_INT = generateRandomNumber();
                                std::string body = "Mã xác thực của bạn là: " + std::to_string(OTP_INT);
                                sendOTP(apiKey, apiSecret, fromEmail, emailForgetPassword, subject, body);
                                emailForgetPassword.clear();
                                emailInputText_ChangePassword.setString("");
                            }
                        }
                    }
                }
            }
            if (screenState == XACTHUC_SCREEN) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = EMAIL_SCREEN;
                            cout << "Logout successfully. \n";
                        }
                    }
                }
                // Kiểm tra click vào ô nhập Mã xác thực
                if (sf::FloatRect(windowSize.x - rightMargin - 350, posY + 2*60, 600, 50).contains(mousePos.x, mousePos.y))
                {
                    isMaXacThuc = true;
                }
                else
                {
                    isMaXacThuc = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (!maXacThuc.empty()) {
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                                if (maXacThucConfirmButton.getGlobalBounds().contains(mouseWorldPos)) {
                                        if (maXacThuc == to_string(OTP_INT)) {
                                            screenState = FORGETPASSWORD_SCREEN;
                                            cout << "Ma xac thuc hop le. \n";
                                            maXacThuc.clear();
                                            maXacThucInputText.setString("");

                                    } else {
                                    cout << "Ma xac thuc khong hop le hoac khong chinh xac. \n";
                                }
                            } 
                        }
                    }
                }
            }
            if (screenState == FORGETPASSWORD_SCREEN) {
                // Xử lí chuột khi click vào nút Back
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (backButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        newPasswordForget.clear();
                        confirmNewPasswordForget.clear();
                        newPasswordInputText_Forget.setString("");
                        confirmNewPasswordInputText_Forget.setString("");
                        screenState = EMAIL_SCREEN;
                    }
                }
                // Xử lí khi click vào ô nhập newPassword
                if (sf::FloatRect(windowSize.x - rightMargin - 350, posY + 2*60 + 7, 500, 50).contains(mousePos.x, mousePos.y))
                {
                    isTypingNewPassword_Forget = true;
                }
                else
                {
                    isTypingNewPassword_Forget = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                // Xử lí khi click vào ô confirm New Password
                if (sf::FloatRect(windowSize.x - rightMargin - 350, posY + 3.5*60 + 7, 500, 50).contains(mousePos.x, mousePos.y))
                {
                    isTypingConfirmNewPassWord_Forget = true;
                }
                else
                {
                    isTypingConfirmNewPassWord_Forget = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (!newPasswordForget.empty() && !confirmNewPasswordForget.empty()) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (confirmNewPasswordButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            cout << "Click confirm new password. \n";
                            customers->forgetPassword(emailForgetPasswordTmp, newPasswordForget, confirmNewPasswordForget);
                            screenState = MAIN_MENU;
                            newPasswordForget.clear();
                            newPasswordInputText_Forget.setString("");
                            confirmNewPasswordForget.clear();
                            confirmNewPasswordInputText_Forget.setString("");
                        }
                    }
                }
            }
            if (screenState == PAY_SCREEN) {
                // Xử lí chuột khi click vào nút back
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                        addressInput.clear();
                        addressInputText.setString("");
                        screenState = VIEWCART_SCREEN;
                    }
                }
                // Xử lí chuột khi click vào nút giỏ hàng
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (cartButtonPAY.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = VIEWCART_SCREEN;
                        addressInput.clear();
                        addressInputText.setString("");
                    }
                }
                // Xử lí chuột khi bấm vào nút Log out
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (logOut.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = MAIN_MENU;
                        addressInput.clear();
                        addressInputText.setString("");
                        cout << "Log out successfully. \n";
                    }
                }
                // Xử lí khi click vào nút home và nút logoutButton 
                handleMouseClickEvents(event, mouseWorldPos, logOutButton, homeButton, screenState);
                // Xử lí khi click vào ô click vào ô address
                if (sf::FloatRect(windowSize.x - rightMargin - 650, posY + 4*60 + 110, 500, 50).contains(mouseWorldPos))
                {
                    isAddress = true;
                }
                else
                {
                    isAddress = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (!discountCodeInput.empty()) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (useDiscountCodeButton.getGlobalBounds().contains(mouseWorldPos)) {
                            priceDiscount = discount->discount(discountCodeInput); // Giá được giảm khi dùng voucher 
                        }
                    }
                }

                // Khi bấm vào nút thanh toán (pay)
                if (!addressInput.empty() && orders->getCount() > 0) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (confirmPay.getGlobalBounds().contains(mouseWorldPos)) {
                            string addressInputString = convertToString(addressInput);
                            orders->payment(addressInputString, discountCodeInput, priceDiscount);
                            priceDiscount = 0.0;
                            cout << "Pay successfully. \n";
                            addressInput.clear();
                            addressInputText.setString("");
                            discountCodeInput.clear();
                            discountInputText.setString("");
                        }
                    }
                }
            }
            if (screenState == INFOACCOUNT_SCREEN) {
                // Xử lí chuột khi click vào nút back
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = PRODUCTS_SCREEN;
                    }
                }
                // Xử lí chuột khi click vào nút giỏ hàng
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (viewCart.getGlobalBounds().contains(mouseWorldPos)) {
                        screenState = VIEWCART_SCREEN;
                    }
                }
                // Xử lí khi click vào nút home và nút logoutButton 
                handleMouseClickEvents(event, mouseWorldPos, logOutButton, homeButton, screenState);
                // Xử lý khi click vào nút dangXuat
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (dangXuatButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                            cout << "Return the main menu. \n";
                        }
                    }
                }
                // Xử lý khi click vào nút Change Password
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (changePasswordButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = EMAIL_SCREEN;
                        }
                    }
                }
            }
            if (screenState == HISTORYORDER_SCREEN) { 
                // Xử lí chuột khi click vào nút back
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = PRODUCTS_SCREEN;
                            orderCodeHistoryOrderInput.clear();
                            orderCodeHistoryInputText.setString("");
                        }
                    }
                }
                // Xử lí khi click vào nút home và nút logoutButton 
               if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (homeButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = PRODUCTS_SCREEN;
                            orderCodeHistoryOrderInput.clear();
                            orderCodeHistoryInputText.setString("");
                        }
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (logOutButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                            orderCodeHistoryOrderInput.clear();
                            orderCodeHistoryInputText.setString("");
                        }
                    }
                }
                // Xử lí khi click vào nút xem giỏ hàng (viewCart)
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (viewCart.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = VIEWCART_SCREEN;
                            orderCodeHistoryOrderInput.clear();
                            orderCodeHistoryInputText.setString("");
                        }
                    }
                }
                // Xử lý khi click vào nút xemm thông tin tài khoản
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (infoAccountButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = INFOACCOUNT_SCREEN;
                            orderCodeHistoryOrderInput.clear();
                            orderCodeHistoryInputText.setString("");
                            cout << "Switch to the screen INFO ACCOUNT. \n";
                        }
                    }
                }
                // Xử lí khi click vào ô OrderCode
                if (sf::FloatRect(140, 360, 425, 35).contains(mousePos.x, mousePos.y))
                {
                    isOrderCodeHistoryOrder = true;
                }
                else
                {
                    isOrderCodeHistoryOrder = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                phoneHistoryOrderInput = customers->getPhone();
                if (!phoneHistoryOrderInput.empty()) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (displayHistoryButton.getGlobalBounds().contains(mouseWorldPos)) {
                            if (customers->getPhone() == phoneHistoryOrderInput) {
                                long orderCodeLong = stol(orderCodeHistoryOrderInput);
                                orders->displayDetailsHistory(orderCodeLong, data, countProductsInOrder);
                            } else cout << "Phone number is wrong. \n";
                        }
                    }
                }
            }
            if (screenState == ADMIN_SCREEN) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                        }
                    }
                }
                handleMouseClickEvents(event, mouseWorldPos, logOutButton, homeButton, screenState);
                // Xử lý khi click vào ô DISPLAYCUSTOMER_SCREEN
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (displayCustomerButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISPLAYCUSTOMER_SCREEN;
                        }
                    }
                }   
                // Xử lí khi click vào ô DISPLAYPRODUCT
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (displayProductButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISPLAYPRODUCTS_SCREEN;
                        }
                    }
                } 
                // Xử lí khi click vào ô DISCOUNTCODE
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (discountCodeButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISCOUNTCODE_SCREEN;
                        }
                    }
                } 
                // Xử lí khi click vào nút REVENUE 
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (revenueButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = REVENUE_SCREEN;
                        }
                    }
                }
                // Xử lý khi click vào nút ADDPRODUCT
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (addProductButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = ADDPRODUCTS_SCREEN;
                            cout << "Welcome to add product screen. \n";
                        }
                    }
                }
                // Xử lý khi click vào nút DetailStatistic
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (detailStatisticsButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DETAILSTATISTICS_SCREEN;
                            cout << "Welcome to DETAILSTATISTICS_SCREEN. \n";
                        }
                    }
                }
            }
            if (screenState == DISPLAYCUSTOMER_SCREEN) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                            customerInput.clear();
                            searchCustomerInputText.setString("");
                            countFoundCustomer = 0;
                        }
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (logOutButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                            customerInput.clear();
                            searchCustomerInputText.setString("");
                            countFoundCustomer = 0;
                        }
                    }
                } 
                // Xử lí khi click vào ô searchCustomer
                if (sf::FloatRect(270, 90, 920, 40).contains(mouseWorldPos))
                {
                    isSearchCustomer = true;
                    searchCustomerText.setString("");
                }
                else
                {
                    isSearchCustomer = false; // Nếu click ra ngoài thì ẩn con trỏ
                    if (customerInput.empty())
                        searchCustomerText.setString("Search for customers.");
                }         
                // Xử lí khi click vào ô DISPLAYPRODUCT
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (displayProductButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISPLAYPRODUCTS_SCREEN;
                            customerInput.clear();
                            searchCustomerInputText.setString("");
                        }
                    }
                } 
                // Xử lí khi click vào ô searchCustomer
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (confirmSearchCustomerButton.getGlobalBounds().contains(mouseWorldPos)) {
                            cout << "Clicked into searchCustomerButton. \n";
                            string customerInput_String = convertToString(customerInput);
                            resultFoundCustomer = Customer::searchNameCustomer(customerInput_String, countFoundCustomer);
                        }
                    }
                }
                // Xử lý khi click vào ô sortCustomer
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (sortCustomerButton.getGlobalBounds().contains(mouseWorldPos)) {
                            cout << "Clicked into sort customer. \n";
                            if (useAlgorithmSortCustomer) {
                                Customer::quicksortNameCustomer(resultFoundCustomer, 0, countFoundCustomer - 1, false);
                            } else {
                                Customer::quicksortNameCustomer(resultFoundCustomer, 0, countFoundCustomer - 1, true);
                            }
                            useAlgorithmSortCustomer = !useAlgorithmSortCustomer;
                        }
                    }
                }
                // Xử lí khi click vào ô DISCOUNTCODE
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (discountCodeButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISCOUNTCODE_SCREEN;
                        }
                    }
                } 
                // Xử lí khi click vào nút REVENUE 
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (revenueButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = REVENUE_SCREEN;
                        }
                    }
                }
                // Xử lý khi click vào nút ADDPRODUCT
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (addProductButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = ADDPRODUCTS_SCREEN;
                            cout << "Welcome to add product screen. \n";
                        }
                    }
                }
                // Xử lý khi click vào nút DetailStatistic
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (detailStatisticsButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DETAILSTATISTICS_SCREEN;
                            cout << "Welcome to DETAILSTATISTICS_SCREEN. \n";
                        }
                    }
                }
            }

            if (screenState == DISPLAYPRODUCTS_SCREEN) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                            productInput.clear();
                            searchProductInputText.setString("");
                            countFoundProduct = 0;
                        }
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (logOutButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                            productInput.clear();
                            searchProductInputText.setString("");
                            countFoundProduct = 0;
                        }
                    }
                }  
                // Xử lý khi click vào ô DISPLAYCUSTOMER_SCREEN
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (displayCustomerButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISPLAYCUSTOMER_SCREEN;
                        }
                    }
                }   
                // Xử lí khi click vào ô searchCustomer
                if (sf::FloatRect(270, 90, 920, 40).contains(mouseWorldPos))
                {
                    isSearchProduct = true;
                    searchProductText.setString("");
                }
                else
                {
                    isSearchProduct = false; // Nếu click ra ngoài thì ẩn con trỏ
                    if (productInput.empty())
                        searchProductText.setString("Search for products.");
                }   
                // Xử lí khi click vào ô searchCustomer
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (confirmSearchProductButton.getGlobalBounds().contains(mouseWorldPos)) {
                            cout << "Clicked into confirm search Products. \n";
                            resultFoundProducts = Products::searchNameProducts(productInput, countFoundProduct);
                        }
                    }
                }   
                // Xử lý khi click vào ô sortProduct
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (sortProductButton.getGlobalBounds().contains(mouseWorldPos)) {
                            cout << "Clicked into sort products for name products. \n";
                            if (useAlgorithmSortProducts) {
                                Products::quicksortNameProduct(resultFoundProducts, 0, countFoundProduct - 1, false);
                            } else {
                                Products::quicksortNameProduct(resultFoundProducts, 0, countFoundProduct - 1, true);
                            }
                            useAlgorithmSortProducts = !useAlgorithmSortProducts;
                        }
                    }
                }
                // Xử lí khi click vào ô sortProductPrice
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (sortProductsPriceButton.getGlobalBounds().contains(mouseWorldPos)) {
                            cout << "Clicked into sort products for price. \n";
                            if (useAlgorithmSortProductPrice) {
                                Products::quicksortPrice(resultFoundProducts, 0, countFoundProduct - 1, false);
                            } else {
                                Products::quicksortPrice(resultFoundProducts, 0, countFoundProduct - 1, true);
                            }
                            useAlgorithmSortProductPrice = !useAlgorithmSortProductPrice;
                        } 
                    }
                }
                // Xử lí khi click vào ô DISCOUNTCODE
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (discountCodeButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISCOUNTCODE_SCREEN;
                        }
                    }
                } 
                // Xử lí khi click vào nút REVENUE 
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (revenueButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = REVENUE_SCREEN;
                        }
                    }
                }
                // Xử lý khi click vào nút ADDPRODUCT
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (addProductButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = ADDPRODUCTS_SCREEN;
                            cout << "Welcome to add product screen. \n";
                        }
                    }
                }
                // Xử lý khi click vào nút DetailStatistic
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (detailStatisticsButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DETAILSTATISTICS_SCREEN;
                            cout << "Welcome to DETAILSTATISTICS_SCREEN. \n";
                        }
                    }
                }
            }
            if (screenState == DISCOUNTCODE_SCREEN) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                            maGiamGia.clear();
                            discountCodeInputText.setString("");
                            giaDuocGiam.clear();
                            priceDiscountInputText.setString("");
                            soLuongMa.clear();
                            quantityDiscountInputText.setString("");
                        }
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (logOutButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                            maGiamGia.clear();
                            discountCodeInputText.setString("");
                            giaDuocGiam.clear();
                            priceDiscountInputText.setString("");
                            soLuongMa.clear();
                            quantityDiscountInputText.setString("");
                        }
                    }
                }  
                // Xử lý khi click vào ô DISPLAYCUSTOMER_SCREEN
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (displayCustomerButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISPLAYCUSTOMER_SCREEN;
                            maGiamGia.clear();
                            discountCodeInputText.setString("");
                            giaDuocGiam.clear();
                            priceDiscountInputText.setString("");
                            soLuongMa.clear();
                            quantityDiscountInputText.setString("");
                        }
                    }
                }   
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (displayProductButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISPLAYPRODUCTS_SCREEN;
                            maGiamGia.clear();
                            discountCodeInputText.setString("");
                            giaDuocGiam.clear();
                            priceDiscountInputText.setString("");
                            soLuongMa.clear();
                            quantityDiscountInputText.setString("");
                        }
                    }
                }
                // Xử lí khi click vào các ô nhập liệu 
                if (sf::FloatRect(450, 200, 600, 40).contains(mouseWorldPos))
                {
                    isMaGiamGia = true;
                }
                else
                {
                    isMaGiamGia = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (sf::FloatRect(450, 250, 600, 40).contains(mouseWorldPos))
                {
                    isGiaDuocGiam = true;
                }
                else
                {
                    isGiaDuocGiam = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (sf::FloatRect(450, 300, 600, 40).contains(mouseWorldPos))
                {
                    isSoLuongMa = true;
                }
                else
                {
                    isSoLuongMa = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (!maGiamGia.empty() && !giaDuocGiam.empty() && !soLuongMa.empty()) {
                    if (event.type == sf::Event::MouseButtonPressed) {
                            if (event.mouseButton.button == sf::Mouse::Left) {
                                if (confirmGenerateDiscountCodeButton.getGlobalBounds().contains(mouseWorldPos)) {
                                    // Chuyển đổi các chuỗi giaDuocGiam và soLuongMa trước khi gọi hàm
                                    try {
                                        // Chuyển giaDuocGiam từ chuỗi thành double
                                        giaDuocGiamDouble = std::stod(giaDuocGiam);  // Sử dụng stod để chuyển chuỗi thành double
                                    } catch (const std::invalid_argument& e) {
                                        std::cout << "Invalid price input for giaDuocGiam!" << std::endl;
                                        // return; // Tránh gọi hàm nếu giá không hợp lệ
                                    }

                                    try {
                                        // Chuyển soLuongMa từ chuỗi thành int
                                        soLuongMaInt = std::stoi(soLuongMa);  // Sử dụng stoi để chuyển chuỗi thành int
                                    } catch (const std::invalid_argument& e) {
                                        std::cout << "Invalid quantity input for soLuongMa!" << std::endl;
                                        // return; // Tránh gọi hàm nếu số lượng không hợp lệ
                                    }

                                // Nếu tất cả dữ liệu hợp lệ, gọi hàm generateDiscountCode
                                admin->generateDiscountCode(maGiamGia, giaDuocGiamDouble, soLuongMaInt);
                                maGiamGia.clear();
                                discountCodeInputText.setString("");
                                giaDuocGiam.clear();
                                priceDiscountInputText.setString("");
                                soLuongMa.clear();
                                quantityDiscountInputText.setString("");
                            }
                        }
                    }
                }
                // Xử lí khi click vào nút REVENUE 
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (revenueButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = REVENUE_SCREEN;
                        }
                    }
                }
                // Xử lý khi click vào nút ADDPRODUCT
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (addProductButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = ADDPRODUCTS_SCREEN;
                            cout << "Welcome to add product screen. \n";
                        }
                    }
                }
                // Xử lý khi click vào nút DetailStatistic
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (detailStatisticsButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DETAILSTATISTICS_SCREEN;
                            cout << "Welcome to DETAILSTATISTICS_SCREEN. \n";
                        }
                    }
                }
            }
            if (screenState == REVENUE_SCREEN) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                        }
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (logOutButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                        }
                    }
                }  
                // Xử lý khi click vào ô DISPLAYCUSTOMER_SCREEN
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (displayCustomerButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISPLAYCUSTOMER_SCREEN;
                        }
                    }
                }   
                // Xử lí khi click vào ô DISCOUNTCODE
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (discountCodeButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISCOUNTCODE_SCREEN;
                        }
                    }
                } if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                            productInput.clear();
                            searchProductInputText.setString("");
                            countFoundProduct = 0;
                        }
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (logOutButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                            productInput.clear();
                            searchProductInputText.setString("");
                            countFoundProduct = 0;
                        }
                    }
                }  
                // Xử lý khi click vào ô DISPLAYCUSTOMER_SCREEN
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (displayCustomerButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISPLAYCUSTOMER_SCREEN;
                        }
                    }
                }   
                // Xử lí khi click vào ô DISCOUNTCODE
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (discountCodeButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISCOUNTCODE_SCREEN;
                        }
                    }
                } 
                // Xử lí khi click vào ô DISPLAYPRODUCT
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (displayProductButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISPLAYPRODUCTS_SCREEN;
                            customerInput.clear();
                            searchCustomerInputText.setString("");
                        }
                    }
                } 
                // Xử lý khi click vào nút ADDPRODUCT
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (addProductButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = ADDPRODUCTS_SCREEN;
                            cout << "Welcome to add product screen. \n";
                        }
                    }
                }
                // Xử lý khi click vào nút DetailStatistic
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (detailStatisticsButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DETAILSTATISTICS_SCREEN;
                            cout << "Welcome to DETAILSTATISTICS_SCREEN. \n";
                        }
                    }
                }
            }

            if (screenState == EDITPRODUCT_SCREEN) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISPLAYPRODUCTS_SCREEN;
                            editNameProduct.clear();
                            editNameProductInputText.setString("");
                            editGenre.clear();
                            editGenreInputText.setString("");
                            editPriceProduct.clear();
                            editPriceProductInputText.setString("");
                            editManufacturer.clear();
                            editManufacturerInputText.setString("");
                            editOperatingSystem.clear();
                            editOperatingSystemInputText.setString("");
                            editQuantity.clear();
                            editQuantityInputText.setString("");
                        }
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (logOutButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                            editNameProduct.clear();
                            editNameProductInputText.setString("");
                            editGenre.clear();
                            editGenreInputText.setString("");
                            editPriceProduct.clear();
                            editPriceProductInputText.setString("");
                            editManufacturer.clear();
                            editManufacturerInputText.setString("");
                            editOperatingSystem.clear();
                            editOperatingSystemInputText.setString("");
                            editQuantity.clear();
                            editQuantityInputText.setString("");
                        }
                    }
                } 
                if (sf::FloatRect(470, 200, 600, 40).contains(mouseWorldPos))
                {
                    isEditNameProduct = true;
                    isEditGenre = false;
                    isEditPriceProduct = false;
                    isEditManufacturer = false;
                    isEditOperatingSystem = false;
                    isEditQuantity = false;
                }
                else
                {
                    isEditNameProduct = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (sf::FloatRect(470, 260, 600, 40).contains(mouseWorldPos))
                {
                    isEditNameProduct = false;
                    isEditGenre = true;
                    isEditPriceProduct = false;
                    isEditManufacturer = false;
                    isEditOperatingSystem = false;
                    isEditQuantity = false;
                }
                else
                {
                    isEditGenre = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (sf::FloatRect(470, 320, 600, 40).contains(mouseWorldPos))
                {
                    isEditNameProduct = false;
                    isEditGenre = false;
                    isEditPriceProduct = true;
                    isEditManufacturer = false;
                    isEditOperatingSystem = false;
                    isEditQuantity = false;
                }
                else
                {
                    isEditPriceProduct = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (sf::FloatRect(470, 380, 600, 40).contains(mouseWorldPos))
                {
                    isEditNameProduct = false;
                    isEditGenre = false;
                    isEditPriceProduct = false;
                    isEditManufacturer = true;
                    isEditOperatingSystem = false;
                    isEditQuantity = false;
                }
                else
                {
                    isEditManufacturer = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (sf::FloatRect(470, 440, 600, 40).contains(mouseWorldPos))
                {
                    isEditNameProduct = false;
                    isEditGenre = false;
                    isEditPriceProduct = false;
                    isEditManufacturer = false;
                    isEditOperatingSystem = true;
                    isEditQuantity = false;
                }
                else
                {
                    isEditOperatingSystem = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (sf::FloatRect(470, 500, 600, 40).contains(mouseWorldPos))
                {
                    isEditNameProduct = false;
                    isEditGenre = false;
                    isEditPriceProduct = false;
                    isEditManufacturer = false;
                    isEditOperatingSystem = false;
                    isEditQuantity = true;
                }
                else
                {
                    isEditQuantity = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
            }
            if (screenState == ADDPRODUCTS_SCREEN) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                            fileNameProduct.clear();
                            fileNameProductInputText.setString("");
                            fileImageProduct.clear();
                            fileImageProductInputText.setString("");
                            productID.clear();
                            productIDInputText.setString("");
                            genre.clear();
                            genreInputText.setString("");
                            nameProduct.clear();
                            nameProductInputText.setString("");
                            priceProduct.clear();
                            priceProductInputText.setString("");
                            manufacturer.clear();
                            manufacturerInputText.setString("");
                            operatingSystem.clear();
                            operatingSystemInputText.setString("");
                            quantity.clear();
                            quantityInputText.setString("");
                        }
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (logOutButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                            fileNameProduct.clear();
                            fileNameProductInputText.setString("");
                            fileImageProduct.clear();
                            fileImageProductInputText.setString("");
                            productID.clear();
                            productIDInputText.setString("");
                            genre.clear();
                            genreInputText.setString("");
                            nameProduct.clear();
                            nameProductInputText.setString("");
                            priceProduct.clear();
                            priceProductInputText.setString("");
                            manufacturer.clear();
                            manufacturerInputText.setString("");
                            operatingSystem.clear();
                            operatingSystemInputText.setString("");
                            quantity.clear();
                            quantityInputText.setString("");
                        }
                    }
                }  
                // Xử lý khi click vào ô DISPLAYCUSTOMER_SCREEN
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (displayCustomerButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISPLAYCUSTOMER_SCREEN;
                            fileNameProduct.clear();
                            fileNameProductInputText.setString("");
                            fileImageProduct.clear();
                            fileImageProductInputText.setString("");
                            productID.clear();
                            productIDInputText.setString("");
                            genre.clear();
                            genreInputText.setString("");
                            nameProduct.clear();
                            nameProductInputText.setString("");
                            priceProduct.clear();
                            priceProductInputText.setString("");
                            manufacturer.clear();
                            manufacturerInputText.setString("");
                            operatingSystem.clear();
                            operatingSystemInputText.setString("");
                            quantity.clear();
                            quantityInputText.setString("");
                        }
                    }
                }   
                // Xử lý khi click vào ô DISPLAYPRODUCTS_SCREEN
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (displayProductButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISPLAYPRODUCTS_SCREEN;
                            fileNameProduct.clear();
                            fileNameProductInputText.setString("");
                            fileImageProduct.clear();
                            fileImageProductInputText.setString("");
                            productID.clear();
                            productIDInputText.setString("");
                            genre.clear();
                            genreInputText.setString("");
                            nameProduct.clear();
                            nameProductInputText.setString("");
                            priceProduct.clear();
                            priceProductInputText.setString("");
                            manufacturer.clear();
                            manufacturerInputText.setString("");
                            operatingSystem.clear();
                            operatingSystemInputText.setString("");
                            quantity.clear();
                            quantityInputText.setString("");
                        }
                    }
                }
                // Xử lý khi click vào ô REVENUE_SCREEN
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (revenueButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = REVENUE_SCREEN;
                            fileNameProduct.clear();
                            fileNameProductInputText.setString("");
                            fileImageProduct.clear();
                            productID.clear();
                            productIDInputText.setString("");
                            genre.clear();
                            genreInputText.setString("");
                            nameProduct.clear();
                            nameProductInputText.setString("");
                            priceProduct.clear();
                            priceProductInputText.setString("");
                            manufacturer.clear();
                            manufacturerInputText.setString("");
                            operatingSystem.clear();
                            operatingSystemInputText.setString("");
                            quantity.clear();
                            quantityInputText.setString("");
                        }
                    }
                }
                // Xử lý khi click vào ô DISCOUNTCODE_SCREEN
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (discountCodeButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISCOUNTCODE_SCREEN;
                            fileNameProduct.clear();
                            fileNameProductInputText.setString("");
                            fileImageProduct.clear();
                            productID.clear();
                            productIDInputText.setString("");
                            genre.clear();
                            genreInputText.setString("");
                            nameProduct.clear();
                            nameProductInputText.setString("");
                            priceProduct.clear();
                            priceProductInputText.setString("");
                            manufacturer.clear();
                            manufacturerInputText.setString("");
                            operatingSystem.clear();
                            operatingSystemInputText.setString("");
                            quantity.clear();
                            quantityInputText.setString("");
                        }
                    }
                }   
                // Xử lý khi click vào nút DetailStatistic
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (detailStatisticsButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DETAILSTATISTICS_SCREEN;
                            cout << "Welcome to DETAILSTATISTICS_SCREEN. \n";
                            fileNameProduct.clear();
                            fileNameProductInputText.setString("");
                            fileImageProduct.clear();
                            productID.clear();
                            productIDInputText.setString("");
                            genre.clear();
                            genreInputText.setString("");
                            nameProduct.clear();
                            nameProductInputText.setString("");
                            priceProduct.clear();
                            priceProductInputText.setString("");
                            manufacturer.clear();
                            manufacturerInputText.setString("");
                            operatingSystem.clear();
                            operatingSystemInputText.setString("");
                            quantity.clear();
                            quantityInputText.setString("");
                        }
                    }
                }

                // Xử lý khi click vào các ô nhập liệu
                if (sf::FloatRect(550, 200, 600, 40).contains(mouseWorldPos))
                {
                    isFileNameProduct = true;
                    isFileImageProduct = false;
                    isProductID = false;
                    isGenre = false;
                    isNameProduct = false;
                    isPriceProduct = false;
                    isManufacturer = false;
                    isOperatingSystem = false;
                    isQuantity = false;
                }
                else
                {
                    isFileNameProduct = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (sf::FloatRect(550, 260, 600, 40).contains(mouseWorldPos))
                {
                    isFileNameProduct = false;
                    isFileImageProduct = true;
                    isProductID = false;
                    isGenre = false;
                    isNameProduct = false;
                    isPriceProduct = false;
                    isManufacturer = false;
                    isOperatingSystem = false;
                    isQuantity = false;
                }
                else
                {
                    isFileImageProduct = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (sf::FloatRect(550, 320, 600, 40).contains(mouseWorldPos))
                {
                    isFileNameProduct = false;
                    isFileImageProduct = false;
                    isProductID = true;
                    isGenre = false;
                    isNameProduct = false;
                    isPriceProduct = false;
                    isManufacturer = false;
                    isOperatingSystem = false;
                    isQuantity = false;
                }
                else
                {
                    isProductID = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (sf::FloatRect(550, 380, 600, 40).contains(mouseWorldPos))
                {
                    isFileNameProduct = false;
                    isFileImageProduct = false;
                    isProductID = false;
                    isGenre = true;
                    isNameProduct = false;
                    isPriceProduct = false;
                    isManufacturer = false;
                    isOperatingSystem = false;
                    isQuantity = false;
                }
                else
                {
                    isGenre = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (sf::FloatRect(550, 440, 600, 40).contains(mouseWorldPos))
                {
                    isFileNameProduct = false;
                    isFileImageProduct = false;
                    isProductID = false;
                    isGenre = false;
                    isNameProduct = true;
                    isPriceProduct = false;
                    isManufacturer = false;
                    isOperatingSystem = false;
                    isQuantity = false;
                }
                else
                {
                    isNameProduct = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (sf::FloatRect(550, 500, 600, 40).contains(mouseWorldPos))
                {
                    isFileNameProduct = false;
                    isFileImageProduct = false;
                    isProductID = false;
                    isGenre = false;
                    isNameProduct = false;
                    isPriceProduct = true;
                    isManufacturer = false;
                    isOperatingSystem = false;
                    isQuantity = false;
                }
                else
                {
                    isPriceProduct = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (sf::FloatRect(550, 560, 600, 40).contains(mouseWorldPos))
                {
                    isFileNameProduct = false;
                    isFileImageProduct = false;
                    isProductID = false;
                    isGenre = false;
                    isNameProduct = false;
                    isPriceProduct = false;
                    isManufacturer = true;
                    isOperatingSystem = false;
                    isQuantity = false;
                }
                else
                {
                    isManufacturer = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (sf::FloatRect(550, 620, 600, 40).contains(mouseWorldPos))
                {
                    isFileNameProduct = false;
                    isFileImageProduct = false;
                    isProductID = false;
                    isGenre = false;
                    isNameProduct = false;
                    isPriceProduct = false;
                    isManufacturer = false;
                    isOperatingSystem = true;
                    isQuantity = false;
                }
                else
                {
                    isOperatingSystem = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
                if (sf::FloatRect(550, 680, 600, 40).contains(mouseWorldPos))
                {
                    isFileNameProduct = false;
                    isFileImageProduct = false;
                    isProductID = false;
                    isGenre = false;
                    isNameProduct = false;
                    isPriceProduct = false;
                    isManufacturer = false;
                    isOperatingSystem = false;
                    isQuantity = true;
                }
                else
                {
                    isQuantity = false; // Nếu click ra ngoài thì ẩn con trỏ
                }
            // Xử lý khi click vào nút ConfirmAddProduct
            if (!fileNameProduct.empty() && !fileImageProduct.empty() && !genre.empty() && !nameProduct.empty()
                && !manufacturer.empty() && !operatingSystem.empty() && !productID.empty() && !quantity.empty() && !priceProduct.empty()) 
                {
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (confirmAddProductButton.getGlobalBounds().contains(mouseWorldPos)) {
                                bool validInput = true;

                                // Kiểm tra các chuỗi cần chuyển thành số
                                if (!productID.empty()) {
                                    try {
                                        productID_int = stoi(productID);
                                    } catch (const invalid_argument &e) {
                                        cout << "Please enter a valid number. \n";
                                        validInput = false;
                                    }
                                } else {
                                    cout << "ProductID field is empty. Please enter a valid productID.\n";
                                    validInput = false;
                                }
                                if (!quantity.empty()) {
                                    try {
                                        quantity_int = stoi(quantity);
                                    } catch (const invalid_argument &e) {
                                        cout << "Please enter a valid number. \n";
                                        validInput = false;
                                    }
                                } else {
                                    cout << "Quantity field is empty. Please enter a valid quantity. \n";
                                    validInput = false;
                                }
                                if (!priceProduct.empty()) {
                                    try {
                                        priceProduct_double = stod(priceProduct);
                                    } catch (const invalid_argument &e) {
                                        cout << "Please enter a valid number. \n";
                                        validInput = false;
                                    }
                                } else {
                                    cout << "Price product field is empty. Please enter a valid price product. \n";
                                    validInput = false;
                                }

                                // Chỉ gọi addProduct khi các giá trị đều hợp lệ
                                if (validInput) {
                                    string nameProductString = convertToString(nameProduct);
                                    admin->addProduct(fileNameProduct, fileImageProduct, productID_int, genre, nameProductString, priceProduct_double, manufacturer, operatingSystem, quantity_int);
                                    
                                    // Xóa hết dữ liệu sau khi nhập
                                    fileNameProduct.clear();
                                    fileNameProductInputText.setString("");
                                    fileImageProduct.clear();
                                    fileImageProductInputText.setString("");
                                    productID.clear();
                                    productIDInputText.setString("");
                                    genre.clear();
                                    genreInputText.setString("");
                                    nameProduct.clear();
                                    nameProductInputText.setString("");
                                    priceProduct.clear();
                                    priceProductInputText.setString("");
                                    manufacturer.clear();
                                    manufacturerInputText.setString("");
                                    operatingSystem.clear();
                                    operatingSystemInputText.setString("");
                                    quantity.clear();
                                    quantityInputText.setString("");
                                }
                            }
                        }
                    }
                }
            }
            if (screenState == DETAILSTATISTICS_SCREEN) {
                // Xử lí khi click vào nút back và logout
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                            dateToFind.clear();
                            thongKeTheoNgayInputText.setString("");
                        }
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (logOutButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = MAIN_MENU;
                            cout << "Log out successfully. \n";
                            dateToFind.clear();
                            thongKeTheoNgayInputText.setString("");
                        }
                    }
                }  
                // Xử lí khi click vào nút Display customer
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (displayCustomerButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISPLAYCUSTOMER_SCREEN;
                            dateToFind.clear();
                            thongKeTheoNgayInputText.setString("");
                        }
                    }
                }
                // Xử lí khi click vào nút Display products
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (displayProductButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISPLAYPRODUCTS_SCREEN;
                            dateToFind.clear();
                            thongKeTheoNgayInputText.setString("");
                        }
                    }
                }
                // Xử lý khi click vào nút REVENUE
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (revenueButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = REVENUE_SCREEN;
                            dateToFind.clear();
                            thongKeTheoNgayInputText.setString("");
                        }
                    }
                }
                // Xử lý khi click vào nút DISCOUNTCODE
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (discountCodeButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = DISCOUNTCODE_SCREEN;
                            dateToFind.clear();
                            thongKeTheoNgayInputText.setString("");
                        }
                    }
                }
                // Xử lý khi click vào nút ADDPRODUCT
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (addProductButton.getGlobalBounds().contains(mouseWorldPos)) {
                            screenState = ADDPRODUCTS_SCREEN;
                            dateToFind.clear();
                            thongKeTheoNgayInputText.setString("");
                        }
                    }
                }
                // Xử lý khi click vào nút confirmStatistic
                if (!dateToFind.empty()) {
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (confirmStatisticButton.getGlobalBounds().contains(mouseWorldPos)) {
                                cout << "Detail statistic for date. \n";
                                lichSuDonHang = FileManager::loadPay("text\\Pay.txt", dateToFind, countPay, totalPrice);
                            }
                        }
                    }
                } else {
                    countPay = 0;
                }
                if (sf::FloatRect(170, 150, 920, 40).contains(mouseWorldPos))
                {
                    isDate = true;
                    thongKeTheoNgayText.setString("");
                }
                else
                {
                    isDate = false;
                    if (dateToFind.empty()) 
                        thongKeTheoNgayText.setString("dd-mm-yyyy.");  // Nếu click ra ngoài thì ẩn con trỏ
                }
            }
            for (int i = 0; i < countProduct_2; i++) {
                if (screenState == screenProduct2[i]) {
                // Xử lí chuột khi click vào nút back của các màn hình
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                                screenState = PRODUCTS_SCREEN2;
                                cout << "Return product screen 2 of the screen " << screenProduct2[i] << endl;
                            }
                        }
                    }
                    // Xử lí khi click vào nút logout và nút Home
                    handleMouseClickEvents(event, mouseWorldPos, logOutButton, homeButton, screenState);
                    // Xử lí khi click vào nút xem giỏ hàng (viewCart)
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (viewCart.getGlobalBounds().contains(mouseWorldPos)) {
                                screenState = VIEWCART_SCREEN;
                                cout << "View cart from the screen " << screenProduct2[i] << endl;
                            }
                        }
                    }
                    // Xử lí khi bấm nút thêm vào giỏ hàng và MUANGAY
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (cartButtons_PS[i].getGlobalBounds().contains(mouseWorldPos)) {
                                orders->displayProduct();
                                orders->addData(IDSanPham2[i]);
                                cout << "Add product " << nameProductScreens_2[i] << " view cart" << endl;
                            }
                        }
                    }
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (muaNgayButtons[i].getGlobalBounds().contains(mouseWorldPos)) {
                                orders->displayProduct();
                                orders->addData(IDSanPham2[i]);
                                screenState = VIEWCART_SCREEN;
                                cout << "Buy product " << nameProductScreens_2[i] << endl;
                            }
                        }
                    }
                
                    // Xử lí chuột khi click vào nút Xem cấu hình chi tiết
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (detailSpecificationButtons[i].getGlobalBounds().contains(mouseWorldPos)) {
                                screenState = static_cast<ScreenState>(screenSpecification2[i]);
                                specification_Product = products->displaySpecification(IDSanPham2[i]);
                                cout << "Look detail specification of " << nameProductScreens_2[i] << endl;
                            }
                        }
                    }
                }
                // Xử lí trong màn hình xem cấu hình chi tiết của từng sản phẩm (laptop)
                if (screenState == screenSpecification2[i]) {
                    // Xử lí chuột khi click vào nút back của các màn hình
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                                screenState = static_cast<ScreenState>(screenProduct2[i]);
                                cout << "Return product screen 2 of the screen " << screenProduct2[i] << endl;
                            }
                        }
                    }
                    // Xử lí khi click vào nút logout và nút Home
                    handleMouseClickEvents(event, mouseWorldPos, logOutButton, homeButton, screenState);
                }
            }
            for (int i = 0; i < countProduct_1; i++) {
                if (screenState == screenProduct1[i]) {
                    // Xử lí chuột khi click vào nút back của các màn hình
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                                screenState = PRODUCTS_SCREEN;
                                cout << "Return product screen 1 of the screen " << screenProduct1[i] << endl;
                            }
                        }
                    }
                    // Xử lí khi click vào nút logout và nút Home
                    handleMouseClickEvents(event, mouseWorldPos, logOutButton, homeButton, screenState);
                    // Xử lí khi click vào nút xem giỏ hàng (viewCart)
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (viewCart.getGlobalBounds().contains(mouseWorldPos)) {
                                screenState = VIEWCART_SCREEN;
                                cout << "View cart from the screen " << screenProduct1[i] << endl;
                            }
                        }
                    }
                    // Xử lí khi bấm nút thêm vào giỏ hàng và MUANGAY
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (cartButtons_PS_1[i].getGlobalBounds().contains(mouseWorldPos)) {
                                orders->displayProduct();
                                orders->addData(IDSanPham1[i]);
                                cout << "Add product " << nameProductScreens_1[i] << " view cart" << endl;
                            }
                        }
                    }
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (muaNgayButtons_1[i].getGlobalBounds().contains(mouseWorldPos)) {
                                orders->displayProduct();
                                orders->addData(IDSanPham1[i]);
                                screenState = VIEWCART_SCREEN;
                                cout << "Buy product " << nameProductScreens_1[i] << endl;
                            }
                        }
                    }
                
                    // Xử lí chuột khi click vào nút Xem cấu hình chi tiết
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (detailSpecificationButtons_1[i].getGlobalBounds().contains(mouseWorldPos)) {
                                screenState = static_cast<ScreenState>(screenSpecification1[i]);
                                specification_Product = products->displaySpecification(IDSanPham1[i]);
                                cout << "Look detail specification of " << nameProductScreens_1[i] << endl;
                            }
                        }
                    }
                }
                if (screenState ==  screenSpecification1[i]) {
                    // Xử lí chuột khi click vào nút back của các màn hình
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                                screenState = static_cast<ScreenState>(screenProduct1[i]);
                                cout << "Return product screen of the screen " <<   screenSpecification1[i] << endl;
                            }
                        }
                    }
                    // Xử lí khi click vào nút logout và nút Home
                    handleMouseClickEvents(event, mouseWorldPos, logOutButton, homeButton, screenState);
                }
            } 
            
            for (int i = 0; i < countProduct_3; i++) {
                if (screenState == screenProduct3[i]) {
                    // Xử lí chuột khi click vào nút back của các màn hình
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                                screenState = PRODUCTS_SCREEN3;
                                cout << "Return product screen 3 of the screen " << screenProduct3[i] << endl;
                            }
                        }
                    }
                    // Xử lí khi click vào nút logout và nút Home
                    handleMouseClickEvents(event, mouseWorldPos, logOutButton, homeButton, screenState);
                    // Xử lí khi click vào nút xem giỏ hàng (viewCart)
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (viewCart.getGlobalBounds().contains(mouseWorldPos)) {
                                screenState = VIEWCART_SCREEN;
                                cout << "View cart from the screen " << screenProduct3[i] << endl;
                            }
                        }
                    }
                    // Xử lí khi bấm nút thêm vào giỏ hàng và MUANGAY
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (cartButtons_PS_3[i].getGlobalBounds().contains(mouseWorldPos)) {
                                orders->displayProduct();
                                orders->addData(IDSanPham3[i]);
                                cout << "Add product " << nameProductScreens_3[i] << " view cart" << endl;
                            }
                        }
                    }
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (muaNgayButtons_3[i].getGlobalBounds().contains(mouseWorldPos)) {
                                orders->displayProduct();
                                orders->addData(IDSanPham3[i]);
                                screenState = VIEWCART_SCREEN;
                                cout << "Buy product " << nameProductScreens_3[i] << endl;
                            }
                        }
                    }
                
                    // Xử lí chuột khi click vào nút Xem cấu hình chi tiết
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (detailSpecificationButtons_3[i].getGlobalBounds().contains(mouseWorldPos)) {
                                screenState = static_cast<ScreenState>(screenSpecification3[i]);
                                specification_Product = products->displaySpecification(IDSanPham3[i]);
                                cout << "Look detail specification of " << nameProductScreens_3[i] << endl;
                            }
                        }
                    }
                }
                if (screenState ==  screenSpecification3[i]) {
                    // Xử lí chuột khi click vào nút back của các màn hình
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                                screenState = static_cast<ScreenState>(screenProduct3[i]);
                                cout << "Return product screen of the screen " <<   screenSpecification3[i] << endl;
                            }
                        }
                    }
                    // Xử lí khi click vào nút logout và nút Home
                    handleMouseClickEvents(event, mouseWorldPos, logOutButton, homeButton, screenState);
                }
            } 
            for (int i = 0; i < countProduct_4; i++) {
                if (screenState == screenProduct4[i]) {
                    // Xử lí chuột khi click vào nút back của các màn hình
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                                screenState = PRODUCTS_SCREEN4;
                                cout << "Return product screen 4 of the screen " << screenProduct4[i] << endl;
                            }
                        }
                    }
                    // Xử lí khi click vào nút logout và nút Home
                    handleMouseClickEvents(event, mouseWorldPos, logOutButton, homeButton, screenState);
                    // Xử lí khi click vào nút xem giỏ hàng (viewCart)
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (viewCart.getGlobalBounds().contains(mouseWorldPos)) {
                                screenState = VIEWCART_SCREEN;
                                cout << "View cart from the screen " << screenProduct4[i] << endl;
                            }
                        }
                    }
                    // Xử lí khi bấm nút thêm vào giỏ hàng và MUANGAY
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (cartButtons_PS_4[i].getGlobalBounds().contains(mouseWorldPos)) {
                                orders->displayProduct();
                                orders->addData(IDSanPham4[i]);
                                cout << "Add product " << nameProductScreens_4[i] << " view cart" << endl;
                            }
                        }
                    }
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (muaNgayButtons_4[i].getGlobalBounds().contains(mouseWorldPos)) {
                                orders->displayProduct();
                                orders->addData(IDSanPham4[i]);
                                screenState = VIEWCART_SCREEN;
                                cout << "Buy product " << nameProductScreens_4[i] << endl;
                            }
                        }
                    }
                
                    // Xử lí chuột khi click vào nút Xem cấu hình chi tiết
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (detailSpecificationButtons_4[i].getGlobalBounds().contains(mouseWorldPos)) {
                                screenState = static_cast<ScreenState>(screenSpecification4[i]);
                                specification_Product = products->displaySpecification(IDSanPham4[i]);
                                cout << "Look detail specification of " << nameProductScreens_4[i] << endl;
                            }
                        }
                    }
                }
                if (screenState ==  screenSpecification4[i]) {
                    // Xử lí chuột khi click vào nút back của các màn hình
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (backButton.getGlobalBounds().contains(mouseWorldPos)) {
                                screenState = static_cast<ScreenState>(screenProduct4[i]);
                                cout << "Return product screen of the screen " <<   screenSpecification4[i] << endl;
                            }
                        }
                    }
                    // Xử lí khi click vào nút logout và nút Home
                    handleMouseClickEvents(event, mouseWorldPos, logOutButton, homeButton, screenState);
                }
            } 
        }
        // Xử lí cuộn chuột
        if (screenState == PRODUCTS_SCREEN || screenState == PRODUCTS_SCREEN2 || screenState == VIEWCART_SCREEN || screenState == PAY_SCREEN || screenState == HISTORYORDER_SCREEN || 
            screenState == ADMIN_SCREEN || screenState == DISPLAYCUSTOMER_SCREEN || screenState == EDITPRODUCT_SCREEN || screenState == ADDPRODUCTS_SCREEN || screenState == PRODUCTS_SCREEN3 || screenState == PRODUCTS_SCREEN4) {
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    view.move(0, -100); // Cuộn lên
                    // Đặt giới hạn cuộn lên
                    if (view.getCenter().y - (768 / 2) < viewTopLimit) {
                        view.setCenter(view.getCenter().x, 768 / 2);
                    }
                } else {
                    view.move(0, 100); // Cuộn xuống
                    // Đặt giới hạn cuộn xuống
                    if (view.getCenter().y + (768 / 2) > viewBottomLimit) {
                        view.setCenter(view.getCenter().x, viewBottomLimit - (768 / 2));
                    }
                }
            }
        }
        if (screenState == DISPLAYPRODUCTS_SCREEN) {
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    view.move(0, -100); // Cuộn lên
                    // Đặt giới hạn cuộn lên
                    if (view.getCenter().y - (768 / 2) < viewTopLimit) {
                        view.setCenter(view.getCenter().x, 768 / 2);
                    }
                } else {
                    view.move(0, 100); // Cuộn xuống
                    // Đặt giới hạn cuộn xuống
                    if (view.getCenter().y + (768 / 2) > viewBottomLimit) {
                        view.setCenter(view.getCenter().x, viewBottomLimit + 3*768);
                    }
                }
            }
        }
        // Xử lí cuộn chuột cho các màn hình (laptop)
        for (int i = 0; i < countProduct_1; i++) {
            if (screenState == screenProduct1[i] || screenState == screenSpecification1[i]) {
                if (event.type == sf::Event::MouseWheelScrolled) {
                    if (event.mouseWheelScroll.delta > 0) {
                        view.move(0, -100); // cuộn lên
                        // Đặt giới hạn cuộn
                        if (view.getCenter().y - (768/2) < viewTopLimit) {
                            view.setCenter(view.getCenter().x, 768/2);
                        }
                    } else {
                        view.move(0, 100); // Cuộn xuống
                        // Đặt giới hạn cuộn xuống
                        if (view.getCenter().y + (768/2) > viewBottomLimit) {
                            view.setCenter(view.getCenter().x, viewBottomLimit - (768 / 2));
                        }
                    }
                }
            }
        }
        // Xử lí cuộn chuột cho các màn hình (laptop)
        for (int i = 0; i < countProduct_2; i++) {
            if (screenState == screenProduct2[i] || screenState == screenSpecification2[i]) {
                if (event.type == sf::Event::MouseWheelScrolled) {
                    if (event.mouseWheelScroll.delta > 0) {
                        view.move(0, -100); // cuộn lên
                        // Đặt giới hạn cuộn
                        if (view.getCenter().y - (768/2) < viewTopLimit) {
                            view.setCenter(view.getCenter().x, 768/2);
                        }
                    } else {
                        view.move(0, 100); // Cuộn xuống
                        // Đặt giới hạn cuộn xuống
                        if (view.getCenter().y + (768/2) > viewBottomLimit) {
                            view.setCenter(view.getCenter().x, viewBottomLimit - (768 / 2));
                        }
                    }
                }
            }
        }
        // Xử lí cuộn chuột cho các màn hình (laptop)
        for (int i = 0; i < countProduct_3; i++) {
            if (screenState == screenProduct3[i] || screenState == screenSpecification3[i]) {
                if (event.type == sf::Event::MouseWheelScrolled) {
                    if (event.mouseWheelScroll.delta > 0) {
                        view.move(0, -100); // cuộn lên
                        // Đặt giới hạn cuộn
                        if (view.getCenter().y - (768/2) < viewTopLimit) {
                            view.setCenter(view.getCenter().x, 768/2);
                        }
                    } else {
                        view.move(0, 100); // Cuộn xuống
                        // Đặt giới hạn cuộn xuống
                        if (view.getCenter().y + (768/2) > viewBottomLimit) {
                            view.setCenter(view.getCenter().x, viewBottomLimit - (768 / 2));
                        }
                    }
                }
            }
        }
        // Xử lí cuộn chuột cho các màn hình (laptop)
        for (int i = 0; i < countProduct_4; i++) {
            if (screenState == screenProduct4[i] || screenState == screenSpecification4[i]) {
                if (event.type == sf::Event::MouseWheelScrolled) {
                    if (event.mouseWheelScroll.delta > 0) {
                        view.move(0, -100); // cuộn lên
                        // Đặt giới hạn cuộn
                        if (view.getCenter().y - (768/2) < viewTopLimit) {
                            view.setCenter(view.getCenter().x, 768/2);
                        }
                    } else {
                        view.move(0, 100); // Cuộn xuống
                        // Đặt giới hạn cuộn xuống
                        if (view.getCenter().y + (768/2) > viewBottomLimit) {
                            view.setCenter(view.getCenter().x, viewBottomLimit - (768 / 2));
                        }
                    }
                }
            }
        }
        // Xử lý nhập liệu
        if (event.type == sf::Event::TextEntered)
        {
            // Trong màn hình đăng nhập
            if (screenState == MAIN_MENU) {
                if (isTypingUsername)
                {
                    if (event.text.unicode == '\b' && !usernameInput.empty())  // Backspace
                    {
                        usernameInput.pop_back();
                    }
                    else if (event.text.unicode < 128 && event.text.unicode != '\b')
                    {
                        usernameInput += static_cast<char>(event.text.unicode);
                    }
                    usernameInputText.setString(usernameInput);
                }

                if (isTypingPassword)
                {
                    if (event.text.unicode == '\b' && !passwordInput.empty())  // Backspace
                    {
                        passwordInput.pop_back();
                    }
                    else if (event.text.unicode < 128 && event.text.unicode != '\b')
                    {
                        passwordInput += static_cast<char>(event.text.unicode);
                    }
                    passwordInputText.setString(std::string(passwordInput.size(), '*')); // Hiển thị mật khẩu như dấu sao
                }
            }
            // Trong màn hinh đăng kí
            else if (screenState == SIGNUP_SCREEN) {
                if (isLastName) {
                    if (event.type == sf::Event::TextEntered) {
                        if (event.text.unicode == '\b' && !lastNameInput.empty())  // Backspace
                        {
                            lastNameInput.pop_back();
                        }
                        else if (event.text.unicode > 31 && event.text.unicode != '\b')
                        {
                            lastNameInput += static_cast<wchar_t>(event.text.unicode);
                        }
                        lastNameInputText.setString(lastNameInput);
                    }
                }
                if (isMiddleName) {
                    if (event.type == sf::Event::TextEntered) {
                        if (event.text.unicode == '\b' && !middleNameInput.empty())  // Backspace
                        {
                            middleNameInput.pop_back();
                        }
                        else if (event.text.unicode > 31 && event.text.unicode != '\b')
                        {
                            middleNameInput += static_cast<wchar_t>(event.text.unicode);
                        }
                        middleNameInputText.setString(middleNameInput);
                    }
                }
                if (isFirstName) {
                    if (event.type == sf::Event::TextEntered) {
                        if (event.text.unicode == '\b' && !firstNameInput.empty())  // Backspace
                        {
                            firstNameInput.pop_back();
                        }
                        else if (event.text.unicode > 31 && event.text.unicode != '\b')
                        {
                            firstNameInput += static_cast<wchar_t>(event.text.unicode);
                        }
                        firstNameInputText.setString(firstNameInput);
                    }
                }
               if (isEmail) {
                    if (event.text.unicode == '\b')  // Backspace
                    {
                        if (!emailInput.empty()) // Chỉ xóa nếu emailInput không rỗng
                        {
                            emailInput.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) // Chỉ thêm ký tự nếu nó nằm trong dải ASCII
                    {
                        emailInput += static_cast<char>(event.text.unicode);
                    }
                    emailInputText.setString(emailInput);
                }
                if (isPhone) {
                    if (event.text.unicode == '\b' && !phoneInput.empty())  // Backspace
                    {
                        phoneInput.pop_back();
                    }
                    else if (event.text.unicode < 128 && event.text.unicode != '\b')
                    {
                        phoneInput += static_cast<char>(event.text.unicode);
                    }
                    phoneInputText.setString(phoneInput);
                }
                if (isUsername_SIGNUP) {
                    if (event.text.unicode == '\b' && !usernameInput_SIGNUP.empty())  // Backspace
                    {
                        usernameInput_SIGNUP.pop_back();
                    }
                    else if (event.text.unicode < 128 && event.text.unicode != '\b')
                    {
                        usernameInput_SIGNUP += static_cast<char>(event.text.unicode);
                    }
                    usernameInputText_SIGNUP.setString(usernameInput_SIGNUP);
                }
                if (isPassword_SIGNUP) {
                    if (event.text.unicode == '\b' && !passwordInput_SIGNUP.empty())  // Backspace
                    {
                        passwordInput_SIGNUP.pop_back();
                    }
                    else if (event.text.unicode < 128 && event.text.unicode != '\b')
                    {
                        passwordInput_SIGNUP += static_cast<char>(event.text.unicode);
                    }
                    passwordInputText_SIGNUP.setString(std::string(passwordInput_SIGNUP.size(), '*')); // Hiển thị mật khẩu như dấu sao
                }
            }
            else if (screenState == EMAIL_SCREEN) {
                if (isTypingPhoneChangePassword) {
                    if (event.text.unicode == '\b' && !emailForgetPassword.empty())  // Backspace
                    {
                        emailForgetPassword.pop_back();
                    }
                    else if (event.text.unicode < 128 && event.text.unicode != '\b')
                    {
                        emailForgetPassword += static_cast<char>(event.text.unicode);
                    }
                    emailInputText_ChangePassword.setString(emailForgetPassword);
                }
            }
            else if (screenState == XACTHUC_SCREEN) {
                if (isMaXacThuc) {
                    if (event.text.unicode == '\b' && !maXacThuc.empty()) {
                        maXacThuc.pop_back();
                    }
                    else if (event.text.unicode < 128 && !event.text.unicode != '\b') {
                        maXacThuc += static_cast<char>(event.text.unicode);
                    }
                    maXacThucInputText.setString(maXacThuc);
                }
            }
            else if (screenState == FORGETPASSWORD_SCREEN) {
                if (isTypingNewPassword_Forget) {
                    if (event.text.unicode == '\b' && !newPasswordForget.empty())  // Backspace
                    {
                        newPasswordForget.pop_back();
                    }
                    else if (event.text.unicode < 128 && event.text.unicode != '\b')
                    {
                        newPasswordForget += static_cast<char>(event.text.unicode);
                    }
                    newPasswordInputText_Forget.setString(std::string(newPasswordForget.size(), '*')); // Hiển thị mật khẩu như dấu sao                
                    }
                if (isTypingConfirmNewPassWord_Forget) {
                    if (event.text.unicode == '\b' && !confirmNewPasswordForget.empty())  // Backspace
                    {
                        confirmNewPasswordForget.pop_back();
                    }
                    else if (event.text.unicode < 128 && event.text.unicode != '\b')
                    {
                        confirmNewPasswordForget += static_cast<char>(event.text.unicode);
                    }
                    confirmNewPasswordInputText_Forget.setString(std::string(confirmNewPasswordForget.size(), '*')); // Hiển thị mật khẩu như dấu sao 
                }
            }
            else if (screenState == PAY_SCREEN) {
                if (isAddress) {
                    if (event.type == sf::Event::TextEntered) {
                        if (event.text.unicode == '\b' && !addressInput.empty())  // Backspace
                        {
                            addressInput.pop_back();
                        }
                        else if (event.text.unicode > 31)
                        {
                            addressInput += static_cast<wchar_t>(event.text.unicode);
                        }
                        addressInputText.setString(addressInput);
                    }
                }
                // Xử lý nhập liệu vào ô mã giảm giá
                if (isDiscountCode) {
                    if (event.text.unicode == '\b')  // Backspace
                    {
                        if (!discountCodeInput.empty()) // Chỉ xóa nếu discountCodeInput không rỗng
                        {
                            discountCodeInput.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) // Chỉ thêm ký tự nếu nó nằm trong dải ASCII
                    {
                        char newChar = static_cast<char>(event.text.unicode);
                        discountCodeInput += static_cast<char>(toupper(newChar));
                    }
                    discountInputText.setString(discountCodeInput);
                }
            }
            else if (screenState == HISTORYORDER_SCREEN) {
                if (isOrderCodeHistoryOrder) {
                    if (event.text.unicode == '\b') // Backspace
                    {
                        if (!orderCodeHistoryOrderInput.empty()) 
                        {
                            orderCodeHistoryOrderInput.pop_back();
                        }
                    } 
                    else if (event.text.unicode < 128) {
                        orderCodeHistoryOrderInput += static_cast<char>(event.text.unicode);
                    }
                    orderCodeHistoryInputText.setString(orderCodeHistoryOrderInput);
                }
            }
            else if (screenState == DISPLAYCUSTOMER_SCREEN) {
                if (isSearchCustomer) {
                    if (event.type == sf::Event::TextEntered) {
                        if (event.text.unicode == '\b') { // Backspace
                            if (event.text.unicode == '\b' && !customerInput.empty()) {
                                customerInput.pop_back();
                            }
                        }
                        else if (event.text.unicode > 31) {
                            customerInput += static_cast<wchar_t>(event.text.unicode);
                        }
                        searchCustomerInputText.setString(customerInput);
                    }
                }
            }
            else if (screenState == DISPLAYPRODUCTS_SCREEN) {
                if (isSearchProduct) {
                    if (event.text.unicode == '\b') {
                        if (!productInput.empty()) {
                            productInput.pop_back();
                        }
                    } 
                    else if (event.text.unicode < 128) {
                        productInput += static_cast<char>(event.text.unicode);
                    }
                    searchProductInputText.setString(productInput);
                }
            }
            else if (screenState == DISCOUNTCODE_SCREEN) {
                if (isMaGiamGia) {
                    if (event.text.unicode == '\b') {
                        if (!maGiamGia.empty()) {
                            maGiamGia.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        maGiamGia += static_cast<char>(event.text.unicode);
                    }
                    discountCodeInputText.setString(maGiamGia);
                }
                if (isGiaDuocGiam) {
                    if (event.text.unicode == '\b') {
                        if (!giaDuocGiam.empty()) {
                            giaDuocGiam.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        giaDuocGiam += static_cast<char>(event.text.unicode);
                    }
                    priceDiscountInputText.setString(giaDuocGiam);
                }
                if (isSoLuongMa) {
                    if (event.text.unicode == '\b') {
                        if (!soLuongMa.empty()) {
                            soLuongMa.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        soLuongMa += static_cast<char>(event.text.unicode);
                    }
                    quantityDiscountInputText.setString(soLuongMa);
                }
            }
            else if (screenState == EDITPRODUCT_SCREEN) {
                if (isEditNameProduct) {
                    if (event.text.unicode == '\b') {
                        if (!editNameProduct.empty()) {
                            editNameProduct.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        editNameProduct += static_cast<char>(event.text.unicode);
                    }
                    editNameProductInputText.setString(editNameProduct);
                }
                if (isEditGenre) {
                    if (event.text.unicode == '\b') {
                        if (!editGenre.empty()) {
                            editGenre.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        editGenre += static_cast<char>(event.text.unicode);
                    }
                    editGenreInputText.setString(editGenre);
                }
                if (isEditPriceProduct) {
                    if (event.text.unicode == '\b') {
                        if (!editPriceProduct.empty()) {
                            editPriceProduct.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        editPriceProduct += static_cast<char>(event.text.unicode);
                    }
                    editPriceProductInputText.setString(editPriceProduct);
                }
                if (isEditManufacturer) {
                    if (event.text.unicode == '\b') {
                        if (!editManufacturer.empty()) {
                            editManufacturer.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        editManufacturer += static_cast<char>(event.text.unicode);
                    }
                    editManufacturerInputText.setString(editManufacturer);
                }
                if (isEditOperatingSystem) {
                    if (event.text.unicode == '\b') {
                        if (!editOperatingSystem.empty()) {
                            editOperatingSystem.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        editOperatingSystem += static_cast<char>(event.text.unicode);
                    }
                    editOperatingSystemInputText.setString(editOperatingSystem);
                }
                if (isEditQuantity) {
                    if (event.text.unicode == '\b') {
                        if (!editQuantity.empty()) {
                            editQuantity.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        editQuantity += static_cast<char>(event.text.unicode);
                    }
                    editQuantityInputText.setString(editQuantity);
                }
            }
            else if (screenState == ADDPRODUCTS_SCREEN) {
                if (isFileNameProduct) {
                    if (event.text.unicode == '\b') {
                        if (!fileNameProduct.empty()) {
                            fileNameProduct.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        fileNameProduct += static_cast<char>(event.text.unicode);
                    }
                    fileNameProductInputText.setString(fileNameProduct);
                }
                if (isFileImageProduct) {
                    if (event.text.unicode == '\b') {
                        if (!fileImageProduct.empty()) {
                            fileImageProduct.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        fileImageProduct += static_cast<char>(event.text.unicode);
                    }
                    fileImageProductInputText.setString(fileImageProduct);
                }
                if (isProductID) {
                    if (event.text.unicode == '\b') {
                        if (!productID.empty()) {
                            productID.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        productID += static_cast<char>(event.text.unicode);
                    }
                    productIDInputText.setString(productID);
                }
                if (isGenre) {
                    if (event.text.unicode == '\b') {
                        if (!genre.empty()) {
                            genre.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        genre += static_cast<char>(event.text.unicode);
                    }
                    genreInputText.setString(genre);
                }
                if (isNameProduct) {
                    if (event.type == sf::Event::TextEntered) {
                        if (event.text.unicode == '\b') {
                            if (!nameProduct.empty()) {
                                nameProduct.pop_back();
                            }
                        }
                        else if (event.text.unicode > 31) {
                            nameProduct += static_cast<wchar_t>(event.text.unicode);
                        }
                        nameProductInputText.setString(nameProduct);
                    }
                }
                if (isPriceProduct) {
                    if (event.text.unicode == '\b') {
                        if (!priceProduct.empty()) {
                            priceProduct.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        priceProduct += static_cast<char>(event.text.unicode);
                    }
                    priceProductInputText.setString(priceProduct);
                }
                if (isManufacturer) {
                    if (event.text.unicode == '\b') {
                        if (!manufacturer.empty()) {
                            manufacturer.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        manufacturer += static_cast<char>(event.text.unicode);
                    }
                    manufacturerInputText.setString(manufacturer);
                }
                if (isOperatingSystem) {
                    if (event.text.unicode == '\b') {
                        if (!operatingSystem.empty()) {
                            operatingSystem.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        operatingSystem += static_cast<char>(event.text.unicode);
                    }
                    operatingSystemInputText.setString(operatingSystem);
                }
                if (isQuantity) {
                    if (event.text.unicode == '\b') {
                        if (!quantity.empty()) {
                            quantity.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        quantity += static_cast<char>(event.text.unicode);
                    }
                    quantityInputText.setString(quantity);
                }
            }
            else if (screenState == DETAILSTATISTICS_SCREEN) {
                if (isDate) {
                    if (event.text.unicode == '\b') {
                        if (!dateToFind.empty()) {
                            dateToFind.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128) {
                        dateToFind += static_cast<char>(event.text.unicode);
                    }
                    thongKeTheoNgayInputText.setString(dateToFind);
                }
            }
        }
    }

        // Cập nhật trạng thái con trỏ nhấp nháy
        if (clock.getElapsedTime().asMilliseconds() > 500) // cứ sau 500ms
        {
            cursorVisible = !cursorVisible; // Đổi trạng thái hiển thị con trỏ
            clock.restart(); // Đặt lại đồng hồ
        }

        // Xóa màn hình
        window.clear(sf::Color(245, 245, 220));

        // Hiển thị màn hình dựa vào trạng thái
        if (screenState == MAIN_MENU)
        {
            window.draw(usernameText);
            window.draw(passwordText);
            window.draw(usernameText);
            window.draw(passwordText);

            sf::Text title("WELCOME TO MY STORE", font, 30);
            title.setFillColor(sf::Color::Black);
            title.setPosition(rightMargin + 200, 100);
            window.draw(title);

            sf::RectangleShape usernameBox(sf::Vector2f(300, 50));
            usernameBox.setPosition(windowSize.x - rightMargin - usernameBox.getSize().x + 450, 200);
            usernameBox.setFillColor(sf::Color::White);

            sf::RectangleShape passwordBox(sf::Vector2f(300, 50));
            passwordBox.setPosition(windowSize.x - rightMargin - passwordBox.getSize().x + 450, 300);
            passwordBox.setFillColor(sf::Color::White);

            sf::Text youDontAccount = createText("Bạn chưa có tài khoản?\n", font, 17, sf::Color::Black, windowSize.x - rightMargin + 175, 455);

            window.draw(usernameBox);
            window.draw(passwordBox);

            window.draw(usernameInputText);
            window.draw(passwordInputText);
            window.draw(forgetPasswordButton);
            window.draw(forgetPasswordText);
            window.draw(youDontAccount);

        if (cursorVisible) {   
            // Vẽ con trỏ cho username
            if (isTypingUsername)
            {
                sf::RectangleShape cursor(sf::Vector2f(2, 30));
                cursor.setPosition(rightMargin + 220 + usernameInputText.getGlobalBounds().width, 200 + 7); // rightMargin + 10 để căn con trỏ với ô nhập
                cursor.setFillColor(sf::Color::Black);
                window.draw(cursor);
            }

            // Vẽ con trỏ cho password
            if (isTypingPassword)
            {
                sf::RectangleShape cursor(sf::Vector2f(2, 30));
                cursor.setPosition(rightMargin + 220 + passwordInputText.getGlobalBounds().width, 300 + 7); // Tương tự với ô password
                cursor.setFillColor(sf::Color::Black);
                window.draw(cursor);
            }
        }
            window.draw(signInButton);
            window.draw(signInText);
            window.draw(signupButton);
            window.draw(signupText);
            window.draw(sprite1);
            window.draw(sprite2);
        }
        else if (screenState == SIGNUP_SCREEN)
        {
            sf::Text tieuDe = createText("Register Account. \n", font_bold, 30, sf::Color::Black, windowSize.x - rightMargin - 350 + 200, 510 - 400);

            window.draw(lastNameText);
            window.draw(middleNameText);
            window.draw(firstNameText);
            window.draw(emailText);
            window.draw(phoneText);
            window.draw(usernameText_SIGNUP);
            window.draw(passwordText_SIGNUP);
            window.draw(tieuDe);

            sf::RectangleShape lastNameBox = createButton(400, 50, windowSize.x - rightMargin - 200, posY, sf::Color::White);
            sf::RectangleShape middlleNameBox = createButton(400, 50, windowSize.x - rightMargin - 200, posY + 60, sf::Color::White);
            sf::RectangleShape firstNameBox = createButton(400, 50, windowSize.x - rightMargin - 200, posY + 2*60, sf::Color::White);
            sf::RectangleShape emailBox = createButton(400, 50, windowSize.x - rightMargin - 200, posY + 3*60, sf::Color::White);
            sf::RectangleShape phoneBox = createButton(400, 50, windowSize.x - rightMargin - 200, posY + 4*60, sf::Color::White);
            sf::RectangleShape userNameBox_SIGNUP = createButton(400, 50, windowSize.x - rightMargin - 200, posY + 5*60, sf::Color::White);
            sf::RectangleShape passwordBox_SIGNUP = createButton(400, 50, windowSize.x - rightMargin - 200, posY + 6*60, sf::Color::White);

            window.draw(lastNameBox);
            window.draw(middlleNameBox);
            window.draw(firstNameBox);
            window.draw(emailBox);
            window.draw(phoneBox);
            window.draw(userNameBox_SIGNUP);
            window.draw(passwordBox_SIGNUP);

            window.draw(lastNameInputText);
            window.draw(middleNameInputText);
            window.draw(firstNameInputText);
            window.draw(emailInputText);
            window.draw(phoneInputText);
            window.draw(usernameInputText_SIGNUP);
            window.draw(passwordInputText_SIGNUP);

            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isLastName)
                {
                    drawCursor(window, lastNameInputText,  windowSize.x - rightMargin - 200, posY + 7);
                    
                }
                if (isMiddleName)
                {
                    drawCursor(window, middleNameInputText,  windowSize.x - rightMargin - 200, posY + 60 + 7);
                }
                if (isFirstName)
                {
                    drawCursor(window, firstNameInputText,  windowSize.x - rightMargin - 200, posY + 2*60 + 7);
                }
                if (isEmail)
                {
                    drawCursor(window, emailInputText,  windowSize.x - rightMargin - 200, posY + 3*60 + 7);
                }
                if (isPhone) 
                {
                    drawCursor(window, phoneInputText,  windowSize.x - rightMargin - 200, posY + 4*60 + 7);
                }
                if (isUsername_SIGNUP) 
                {
                    drawCursor(window, usernameInputText_SIGNUP,  windowSize.x - rightMargin - 200, posY + 5*60 + 7);
                }
                if (isPassword_SIGNUP) 
                {
                    drawCursor(window, passwordInputText_SIGNUP,  windowSize.x - rightMargin - 200, posY + 6*60 + 7);
                }
            }

            window.draw(registerAccount);
            window.draw(registerAccountText);
            window.draw(backButton);
            // window.draw(backText);
            window.draw(spriteBack);
        }
        else if (screenState == VIEWCART_SCREEN) {
            window.setView(view);
            sf::Text tieuDe = createText("CART", font_bold, 30, sf::Color::Black,  windowSize.x - rightMargin - 200 - 430, posY - 90);
            sf::RectangleShape viewCartBackGround = createButton(930, 1300, windowSize.x - rightMargin - 200 - 470, posY - 45, sf::Color::White);
            sf::RectangleShape payBackGround = createButton(350, 550, windowSize.x - rightMargin + 280, posY - 45, sf::Color::White);
            sf::Text totalPay = createText("TOTAL", font, 20, sf::Color::Black, windowSize.x - rightMargin + 290, 460);

            window.draw(payBackGround);
            window.draw(viewCartBackGround);
            window.draw(tieuDe);
            window.draw(backButton);
            window.draw(spriteBack);
            window.draw(homeButton);
            window.draw(spriteHome);
            window.draw(logOutButton);
            window.draw(spriteLogOut);
            window.draw(totalPay);
            window.draw(payButton);
            window.draw(payText);
            // scrollbarProductScreen.draw(window);

            loadProductImages();
            // Vẽ danh sách  sản phẩm trong giỏ hàng
            double totalAmount = 0.0; // Tổng tiền sản phẩm

            // Khởi tạo vị trí theo chiều dọc
            double posY = 160.0f; // Bắt đầu từ trên cùng
            sf::RectangleShape removeProductButtons[orders->getCount()]; // mảng chứa nút xóa
            sf::RectangleShape increaseProductButtons[orders->getCount()];
            sf::RectangleShape decreaseProductButtons[orders->getCount()];
            for (int i = 0; i < orders->getCount(); i++) {
                int productID = orders->getData()[i].getProductID();
                
                // Hiển thị hình ảnh sản phẩm
                sf::Sprite productSprite = getProductImageID(productID);
                
                // Hiển thị thông tin sản phẩm
                sf::Text productName(orders->getData()[i].getNameProduct(), font, 20);
                productName.setFillColor(sf::Color::Black);
                
                // Lấy giá trị priceProduct
                double priceProduct = orders->getData()[i].getPriceProduct();
                
                // Cập nhật tổng tiền
                totalAmount += priceProduct * orders->getData()[i].getCount();
                
                // Chuyển đổi thành chuỗi với định dạng
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(2) << priceProduct;
                
                std::string priceString = "$" + std::to_string(priceProduct);
                priceString.erase(priceString.find_last_not_of('0') + 1, std::string::npos);
                if (priceString.back() == '.') {
                    priceString.pop_back();
                }
                
                sf::Text productPrice(priceString, font, 20);
                productPrice.setFillColor(sf::Color(21, 91, 246));
                
                // Số lượng sản phẩm
                sf::Text productQuantity(to_string(orders->getData()[i].getCount()), font, 20);
                productQuantity.setFillColor(sf::Color::Black);

                // Tạo hình chữ nhật cho nút số lượng sản phẩm
                sf::RectangleShape productQuantityButton = createOutlinedRectangle(100, 40, sf::Color(160, 160, 160), 1, sf::Vector2f(850, posY + 40)); 

                // Đặt vị trí cho hình ảnh sản phẩm và văn bản
                if (productID == 102) {
                    productSprite.setScale(0.2f, 0.19f);
                } else productSprite.setScale(0.2f, 0.2f);
                productSprite.setPosition(100, posY);
                productName.setPosition(300, posY + 50);
                productPrice.setPosition(700, posY + 50);
                productQuantity.setPosition(895, posY + 50);
                
                // Kẻ đường thẳng nằm ngang ngăn cách giữa các sản phẩm trong giỏ hàng
                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(100, posY + 150)), // Điểm đầu
                    sf::Vertex(sf::Vector2f(950, posY + 150)) // Điểm cuối
                };
                line[0].color = sf::Color(160, 160, 160);
                line[1].color = sf::Color(160, 160, 160);
                
                // Vẽ chi tiết sản phẩm
                window.draw(productSprite);
                window.draw(productName);
                window.draw(productPrice);
                window.draw(productQuantityButton);
                window.draw(productQuantity);
                window.draw(line, 2, sf::PrimitiveType::Lines);
                
                // Tăng posY cho sản phẩm tiếp theo
                posY += 170; // Điều chỉnh giá trị này theo nhu cầu để tạo khoảng cách

                // Tạo nút xóa sản phẩm 
                double buttonPosY = posY - 120; // Tính vị trí nút xóa tương ứng
                removeProductButtons[i] = createButton(50, 50, 60, buttonPosY, sf::Color::White);
                sf::Text removeProductText = createButtonText("X", font, 20, sf::Color::Black, removeProductButtons[i]);

                // Vẽ nút xóa lên màn hình
                window.draw(removeProductButtons[i]);
                window.draw(removeProductText);

                // Tạo nút + và - sản phẩm
                increaseProductButtons[i] = createButton(20, 20, 920, buttonPosY, sf::Color::White);
                decreaseProductButtons[i] = createButton(20, 20, 860, buttonPosY - 5, sf::Color::White);
                sf::Text increaseProductText = createButtonText("+", font, 20, sf::Color::Black, increaseProductButtons[i]);
                sf::Text decreaseProductText = createButtonText("-", font, 20, sf::Color::Black, decreaseProductButtons[i]);
                // Vẽ lên màn hình nút + và -
                window.draw(increaseProductButtons[i]);
                window.draw(increaseProductText);
                window.draw(decreaseProductButtons[i]);
                window.draw(decreaseProductText);
            }
            static bool mousePressedRemove = false;
                // Kiểm tra sự kiện cho nút xóa
                if (event.type == sf::Event::MouseButtonPressed && !mousePressedRemove) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        mousePressedRemove = true;
                        // sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                        sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
                        // Chuyển đổi vị trí chuột sang không gian của View
                        sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePixelPos, view);
                        for (int i = 0; i < orders->getCount(); i++) {
                            if (removeProductButtons[i].getGlobalBounds().contains(mouseWorldPos)) {
                                int productID = orders->getData()[i].getProductID();
                                orders->deleteData(productID); // Gọi hàm xóa sản phẩm

                                // Cập nhật lại giao diện sau khi xóa sản phẩm
                                orders->displayOrder(); // Gọi hàm để hiển thị lại giỏ hàng
                                break; // Thoát khỏi vòng lặp sau khi xóa
                            }
                        }
                    }
                }
                if (event.type == sf::Event::MouseButtonReleased) {
                    mousePressedRemove = false;
                }
                // Kiểm tra sự kiện cho nút + và -
                static bool mousePressedInDe = false; // Sử dụng cờ để theo dõi trạng thái chuột
                if (event.type == sf::Event::MouseButtonPressed && !mousePressedInDe) {
                    mousePressedInDe = true; // Đánh dấu là chuột được nhấn
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
                        // Chuyển đổi vị trí chuột sang không gian của View
                        sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePixelPos, view);
                        for (int i = 0; i < orders->getCount(); i++) {
                            // Kiểm tra nút +
                            if (increaseProductButtons[i].getGlobalBounds().contains(mouseWorldPos)) {
                                orders->increaseProductCount(orders->getData()[i].getProductID());
                                cout << "Click button increase product .\n";
                                break;
                            }
                            // Kiểm tra nút -
                            if (decreaseProductButtons[i].getGlobalBounds().contains(mouseWorldPos)) {
                                orders->decreaseProductCount(orders->getData()[i].getProductID());
                                cout << "Click button decrease product .\n";
                                break;
                            }
                        }
                    }
                }
                if (event.type == sf::Event::MouseButtonReleased) {
                    mousePressedInDe = false; // Đặt lại cờ khi chuột được nhả ra
                }
            
            // Hiển thị tổng số tiền ở cuối
            std::string totalAmountString = "$" + std::to_string(totalAmount);
            totalAmountString.erase(totalAmountString.find_last_not_of('0') + 1, std::string::npos);
            if (totalAmountString.back() == '.') {
                totalAmountString.pop_back();
            }
            // Sử dụng priceString để tạo sf::Text
            sf::Text totalProductPrice(totalAmountString, font, 20);
            totalProductPrice.setFillColor(sf::Color(21, 91, 246));
            totalProductPrice.setPosition(windowSize.x - rightMargin + 530, 460);
            window.draw(totalProductPrice);
        }
        else if (screenState == EMAIL_SCREEN) {
            sf::RectangleShape phoneBox_ChangePassword = createButton(600, 50, windowSize.x - rightMargin - 350, posY + 2*60, sf::Color::White);
            sf::Text tieuDeForgetPassword = createText("Quên mật khẩu\n", font_bold, 30, sf::Color::Black, windowSize.x - rightMargin - 350 + 200, 510 - 350);
            sf::Text tutorialInput = createText("Hãy nhập email của bạn vào bên dưới để bắt đầu quá trình khôi phục mật khẩu.\n", font, 15, sf::Color::Black, windowSize.x - rightMargin - 350 + 60, 510 - 300);
            window.draw(backButton);
            window.draw(spriteBack);
            window.draw(emailText_ChangePassword);
            window.draw(phoneBox_ChangePassword);
            window.draw(emailInputText_ChangePassword);
            window.draw(confirmEmailButton);
            window.draw(confirmEmailText);
            window.draw(tieuDeForgetPassword);
            window.draw(tutorialInput);
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isTypingPhoneChangePassword)
                {
                    drawCursor(window, emailInputText_ChangePassword, windowSize.x - rightMargin - 350, posY + 2*60 + 7);
                    
                }
            }
        }
        else if (screenState == XACTHUC_SCREEN) {
            sf::Text tieuDe = createText("Xác thực Email\n", font_bold, 30, sf::Color::Black, windowSize.x - rightMargin - 350 + 200, 510 - 350);
            sf::Text tutorialInput = createText("Chúng tôi đã gửi mã xác thực đến địa chỉ email của bạn. Vui lòng kiểm tra hộp thư đến để tiếp tục.\n", font, 15, sf::Color::Black, windowSize.x - rightMargin - 350, 510 - 300);
            
            window.draw(tieuDe);
            window.draw(tutorialInput);
            window.draw(backButton);
            window.draw(spriteBack);
            window.draw(maXacThucButton);
            window.draw(maXacThucInputText);
            window.draw(maXacThucConfirmButton);
            window.draw(maXacThucConfirmText);
            if (cursorVisible) {
                // Vẽ con trỏ
                if (isMaXacThuc) {
                    drawCursor(window, maXacThucInputText, windowSize.x - rightMargin - 350 + 10, posY + 2*60 + 7);
                }
            }
        }
        else if (screenState == FORGETPASSWORD_SCREEN) {
            sf::RectangleShape newPassWordForgetBox = createButton(600, 50, windowSize.x - rightMargin - 350, posY + 2*60, sf::Color::White);
            sf::RectangleShape confirmNewPassWordForgetBox = createButton(600, 50, windowSize.x - rightMargin - 350, posY + 3.5*60, sf::Color::White);
            sf::Text tieuDeForgetPassword = createText("Tạo mật khẩu mới. \n", font_bold, 30, sf::Color::Black, windowSize.x - rightMargin - 350 + 150, 510 - 350);
            window.draw(newPassWordForgetBox);
            window.draw(confirmNewPassWordForgetBox);
            window.draw(newPasswordText_Forget);
            window.draw(confirmNewPasswordText_Forget);
            window.draw(newPasswordInputText_Forget);
            window.draw(confirmNewPasswordInputText_Forget);
            window.draw(confirmNewPasswordButton);
            window.draw(confirmNewPasswordTextButton);
            window.draw(tieuDeForgetPassword);
            window.draw(backButton);
            window.draw(spriteBack);

            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isTypingNewPassword_Forget) 
                {
                    drawCursor(window, newPasswordInputText_Forget, windowSize.x - rightMargin - 350, posY + 2*60 + 7);
                    
                }
                if (isTypingConfirmNewPassWord_Forget) 
                {
                    drawCursor(window, confirmNewPasswordInputText_Forget, windowSize.x - rightMargin - 350, posY + 3.5*60 + 7);
                    
                }
            }
        }
        else if (screenState == PAY_SCREEN) {
            sf::RectangleShape payBackGround = createButton(730, 550, windowSize.x - rightMargin - 200 - 470, posY - 45, sf::Color::White);
            sf::RectangleShape payInfomation = createButton(550, 1300, windowSize.x - rightMargin + 80, posY - 45, sf::Color::White);
            sf::RectangleShape nameCustomer = createOutlinedRectangle(680, 50, sf::Color(200, 200, 200), 3, sf::Vector2f(windowSize.x - rightMargin - 650, posY + 60 + 120));
            sf::Text nameCustomerText = createButtonText(customers->getLastName() + " " + customers->getMiddleName() + " " + customers->getFirstName(), font, 20, sf::Color::Black, nameCustomer);
            nameCustomerText.setPosition(windowSize.x - rightMargin - 645, posY + 60 + 133);
            sf::RectangleShape phoneCustomer = createOutlinedRectangle(680, 50, sf::Color(200, 200, 200), 3, sf::Vector2f(windowSize.x - rightMargin - 650, posY + 2*60 + 120));
            sf::Text phoneCustomerText = createButtonText(customers->getPhone(), font, 20, sf::Color::Black, phoneCustomer);
            phoneCustomerText.setPosition(windowSize.x - rightMargin - 645, posY + 2*60 + 133);
            sf::Texture avatarDog;
            avatarDog.loadFromFile("Graphics/Picture/Dog_2.jpg");
            sf::Sprite spriteAvatarDog;
            spriteAvatarDog.setTexture(avatarDog);
            spriteAvatarDog.setScale(0.25f, 0.25f);
            spriteAvatarDog.setPosition(windowSize.x - rightMargin - 650, posY + 70);
            string name = customers->getLastName() + " " + customers->getMiddleName() + " " + customers->getFirstName() + " (" + customers->getEmail() + ")";
            sf::Text infoCustomer = createText(name, font, 20, sf::Color::Black, windowSize.x - rightMargin - 580, posY + 70);
            sf::Text addressText = createText("Address:", font, 20, sf::Color::Black, windowSize.x - rightMargin - 650, posY + 4*60 + 70);

            sf::Text tieuDe = createText("Payment infomation", font, 30, sf::Color::Black, windowSize.x - rightMargin - 650, posY - 20);
            window.draw(payBackGround);
            window.draw(payInfomation);
            window.draw(backButton);
            window.draw(spriteBack);
            window.draw(homeButton);
            window.draw(spriteHome);
            window.draw(logOutButton);
            window.draw(spriteLogOut);
            window.draw(nameCustomer);
            window.draw(nameCustomerText);
            window.draw(phoneCustomer);
            window.draw(phoneCustomerText);
            window.draw(addressCustomer);
            window.draw(confirmPay);
            window.draw(confirmPayText);
            window.draw(spriteAvatarDog);
            window.draw(infoCustomer);
            window.draw(addressText);
            window.draw(addressInputText);
            window.draw(logOut);
            window.draw(logOutText);
            window.draw(cartButtonPAY);
            window.draw(cartButtonPAYText);
            window.draw(tieuDe);
            window.setView(view);

            // Vẽ con trỏ
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isAddress) 
                {
                    drawCursor(window, addressInputText, windowSize.x - rightMargin - 645, posY + 4*60 + 117);
                    
                }
            }

            // Khởi tạo các sản phẩm để thực hiện thanh toán
            double posY = 170.0f; // Bắt đầu từ trên cùng
            double totalAmountTemp = 0.0;
            double totalPriceLast = 0.0; // Giá cuối cùng khách hàng phải thanh toán
            for (int i = 0; i < orders->getCount(); i++) {
                int productID = orders->getData()[i].getProductID();
                // Hiển thị hình ảnh sản phẩm có trong giỏ hàng
                sf::Sprite productSprite = getProductImageID(productID);
                
                // Hiển thị thông tin sản phẩm
                sf::Text productName(orders->getData()[i].getNameProduct(), font, 15);
                productName.setFillColor(sf::Color::Black);
                // Lấy giá của từng sản phẩm có trong giỏ hàng
                double priceProduct = orders->getData()[i].getPriceProduct();

                // Tổng tiền của sản phẩm (chưa có mã giảm giá / voucher)
                totalAmountTemp += priceProduct * orders->getData()[i].getCount();

                // Chuyển đổi thành chuỗi với định dạng
                string priceProductString = formatTotalAmount(priceProduct);
                sf::Text productPrice(priceProductString, font, 15);
                productPrice.setFillColor(sf::Color(21, 91, 246));
                // Số lượng sản phẩm 
                sf::Text productQuantity("x" + to_string(orders->getData()[i].getCount()), font, 15);
                productQuantity.setFillColor(sf::Color::Black);

                productSprite.setScale(0.1f, 0.1f);
                productSprite.setPosition(820, posY);
                productName.setPosition(900, posY + 20);
                productPrice.setPosition(1250, posY + 20);
                productQuantity.setPosition(900, posY + 40);

                // Kẻ đường thẳng nằm ngang ngăn cách giữa 2 sản phẩm trong giỏ hàng
                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(820, posY + 80)),
                    sf::Vertex(sf::Vector2f(1300, posY + 80))
                };
                line[0].color = sf::Color(160, 160, 160);
                line[1].color = sf::Color(160, 160, 160);

                window.draw(productSprite);
                window.draw(productName);
                window.draw(productPrice);
                window.draw(productQuantity);
                window.draw(line, 2, sf::PrimitiveType::Lines);

                posY += 90;
            }
            sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
            // Chuyển đổi vị trí chuột sang không gian của View
            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePixelPos, view);

            // Ô nhập mã giảm giá
            sf::Text discountCodeText = createText("Discount", font, 15, sf::Color::Black, 820, posY + 10);
            sf::RectangleShape discountCodeButton = createOutlinedRectangle(400, 40, sf::Color(200, 200, 200), 3, sf::Vector2f(820, posY + 40));
            // Xử lý khi click vào ô discountCode
            if (sf::FloatRect(820, posY + 40, 400, 40).contains(mouseWorldPos)) {
                isDiscountCode = true;
            }
            else {
                isDiscountCode = false;
            }
            // Vẽ con trỏ
            discountInputText.setPosition(820, posY + 47);
            useDiscountCodeButton.setPosition(1230, posY + 37);
            sf::Text useDiscountCodeText = createButtonText("Use", font, 20, sf::Color::White, useDiscountCodeButton);
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isDiscountCode) 
                {
                    drawCursor(window, discountInputText, 820, posY + 43);
                }
            }
            window.draw(discountCodeText);
            window.draw(discountCodeButton);
            window.draw(discountInputText);
            window.draw(useDiscountCodeButton);
            window.draw(useDiscountCodeText);

            sf::Text totalPriceProductText = createText("Price product", font, 20, sf::Color::Black, 820, posY + 100);
            sf::Text discount = createText("Discount", font, 20, sf::Color::Black, 820, posY + 140);
            sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(820, posY + 180)),
                    sf::Vertex(sf::Vector2f(1300, posY + 180))
                };
                line[0].color = sf::Color(160, 160, 160);
                line[1].color = sf::Color(160, 160, 160);
            
            // Chuyển đổi thành chuỗi với định dạng
            string totalAmountTempString = formatTotalAmount(totalAmountTemp);
            sf::Text totalPriceTempText(totalAmountTempString, font, 20);
            totalPriceTempText.setFillColor(sf::Color(21, 91, 246));
            totalPriceTempText.setPosition(1230, posY + 100);

            // Xử lí discount
            string priceDiscountString = formatTotalAmount(priceDiscount);
            sf::Text priceDiscountText("- " + priceDiscountString, font, 20);
            priceDiscountText.setFillColor(sf::Color(21, 91, 246));
            priceDiscountText.setPosition(1230, posY + 140);

            sf::Text totalPriceText = createText("TOTAL", font, 20, sf::Color::Black, 820, posY + 210);
            totalPriceLast = totalAmountTemp - priceDiscount; // Giá cuối cùng
            string totalPriceLastString = formatTotalAmount(totalPriceLast);
            sf::Text totalPriceLastText(totalPriceLastString, font, 20);
            totalPriceLastText.setFillColor(sf::Color(21, 91, 246));
            totalPriceLastText.setPosition(1230, posY + 210);
            // Chuyển đổi thành chuỗi định dạng
            window.draw(totalPriceProductText);
            window.draw(totalPriceTempText);
            window.draw(discount);
            window.draw(line, 2, sf::PrimitiveType::Lines);
            window.draw(totalPriceText);
            window.draw(priceDiscountText);
            window.draw(totalPriceLastText);
        }
        else if (screenState == INFOACCOUNT_SCREEN) {
            sf::Text tieuDe = createText("TÀI KHOẢN", font, 30, sf::Color::Black, 40, 120);
            string name = customers->getLastName() + ' ' + customers->getMiddleName() + ' ' + customers->getFirstName();
            sf::Text nameCustomer = createText("Hello, " + name + "!", font, 20, sf::Color::Black, 40, 170);
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(370, 120)), // Điểm đầu
                sf::Vertex(sf::Vector2f(370, 340)) // Điểm cuối
            };
                line[0].color = sf::Color(160, 160, 160);
                line[1].color = sf::Color(160, 160, 160);
            sf::Text tieuDe2 = createText("TÀI KHOẢN", font, 30, sf::Color::Black, 400, 120);
            sf::Text nameCustomer2 = createText("Tên tài khoản: " + name, font, 20, sf::Color::Black, 400, 170);

            sf::Texture homeIcon;
            homeIcon.loadFromFile("Graphics/Picture/home.png");
            sf::Sprite spriteHomeIcon;
            spriteHomeIcon.setTexture(homeIcon);
            spriteHomeIcon.setScale(0.05f, 0.05f);
            spriteHomeIcon.setPosition(400, 200);

            sf::Text diaChi = createText("Quốc tịch: Việt Nam", font, 20, sf::Color::Black, 440, 202);

            sf::Texture phoneIcon;
            phoneIcon.loadFromFile("Graphics/Picture/phoneIcon.png");
            sf::Sprite spritePhoneIcon;
            spritePhoneIcon.setTexture(phoneIcon);
            spritePhoneIcon.setScale(0.1f, 0.1f);
            spritePhoneIcon.setPosition(400, 250);

            sf::Text dienThoai = createText("Số điện thoại: " + customers->getPhone(), font, 20, sf::Color::Black, 440, 252);

            sf::Text tieuDe3 = createText("ĐƠN HÀNG CỦA BẠN", font, 30, sf::Color::Black, 400, 290);

            sf::Vertex line2[] = {
                sf::Vertex(sf::Vector2f(400, 340)), // Điểm đầu
                sf::Vertex(sf::Vector2f(1300, 340)) // Điểm cuối
            };
                line2[0].color = sf::Color(160, 160, 160);
                line2[1].color = sf::Color(160, 160, 160);

            sf::Text customerID = createText("Ngày đặt hàng", font_bold, 20, sf::Color::Black, 400, 350);
            sf::Text orderDate = createText("Mã đơn hàng", font_bold, 20, sf::Color::Black, 560, 350);
            sf::Text totalAmount = createText("Thành tiền", font_bold, 20, sf::Color::Black, 700, 350);
            sf::Text address = createText("Địa chỉ", font_bold, 20, sf::Color::Black, 920, 350);

            int countPay = 0;
            int countCheck = 0;
            int maKhachHang = customers->getCustomerID();
            History *historyPay = FileManager::loadPay("text\\Pay.txt", countPay, maKhachHang);
            float posFirst = 390.0f;
            for (int i = countPay - 1; i >= 0; i--) {
                countCheck++;
                sf::Text ngayDatHang = createText(historyPay[i].getCurrentDate(), font, 20, sf::Color::Black, 400, posFirst);
                sf::Text maDonHang = createText(to_string(historyPay[i].getOrderCode()), font, 20, sf::Color::Black, 560, posFirst);
                double totalPrice = historyPay[i].getPriceTotal();
                string tongTienDonHang = formatTotalAmount(totalPrice);
                sf::Text thanhTien = createText(tongTienDonHang, font, 20, sf::Color::Black, 700, posFirst);
                sf::Text diaChi = createText(historyPay[i].getAddressOrder(), font, 20, sf::Color::Black, 920, posFirst);

                window.draw(ngayDatHang);
                window.draw(maDonHang);
                window.draw(thanhTien);
                window.draw(diaChi);
                posFirst += 40;
                if (countCheck == 9) break;
            }
            if (countPay == 0) {
                sf::Text payEmpty = createText("No orders available.", font, 20, sf::Color::Red, 400, posFirst + 30);
                window.draw(payEmpty);
            }
            sf::Vertex line3[] = {
                sf::Vertex(sf::Vector2f(400, posFirst)), // Điểm đầu
                sf::Vertex(sf::Vector2f(1300, posFirst)) // Điểm cuối
            };
                line3[0].color = sf::Color(160, 160, 160);
                line3[1].color = sf::Color(160, 160, 160);
            window.draw(logOutButton);
            window.draw(spriteLogOut);
            window.draw(homeButton);
            window.draw(spriteHome);
            window.draw(backButton);
            window.draw(spriteBack);
            window.draw(viewCart);
            window.draw(spriteViewCart);
            window.draw(tieuDe);
            window.draw(nameCustomer);
            window.draw(dangXuatButton);
            window.draw(dangXuatText);
            window.draw(changePasswordButton);
            window.draw(changPasswordText);
            window.draw(line, 2, sf::PrimitiveType::Lines);
            window.draw(tieuDe2);
            window.draw(nameCustomer2);
            window.draw(spriteHomeIcon);
            window.draw(diaChi);
            window.draw(spritePhoneIcon);
            window.draw(dienThoai);
            window.draw(tieuDe3);
            window.draw(line2, 2, sf::PrimitiveType::Lines);
            window.draw(customerID);
            window.draw(orderDate);
            window.draw(totalAmount);
            window.draw(address);
            window.draw(line3, 2, sf::PrimitiveType::Lines);

        } 
        else if (screenState == PRODUCTS_SCREEN) {
            for (int i = 0; i < countProduct_1; i++) {
                window.draw(productss_1[i]);
                window.draw(nameProductss_1[i]);
                window.draw(priceProductss_1[i]);
                window.draw(productSprites_1[i]);
                window.draw(cartButtons_1[i]);
                window.draw(cartSprites_1[i]);
            }
            window.setView(view);
            window.draw(infoAccountButton);
            window.draw(historyOrderButton);
            window.draw(spriteHistoryOrder);
            window.draw(spriteInfoAccountIcon);
            window.draw(logOutButton);
            window.draw(spriteLogOut);
            window.draw(homeButton);
            window.draw(spriteHome);
            window.draw(backButton);
            window.draw(spriteBack);
            window.draw(viewCart);
            window.draw(spriteViewCart);   
            window.draw(dienThoaiButton);
            window.draw(dienThoaiText); 
            window.draw(lapTopButton);
            window.draw(lapTopText); 
            window.draw(taiNgheButton);
            window.draw(taiNgheText);
            window.draw(tabletButton);
            window.draw(tabletText);
        }
        else if (screenState == PRODUCTS_SCREEN2) {
            for (int i = 0; i < countProduct_2; i++) {
                window.draw(productss_2[i]);
                window.draw(nameProductss_2[i]);
                window.draw(priceProductss_2[i]);
                window.draw(productSprites_2[i]);
                window.draw(cartButtons_2[i]);
                window.draw(cartSprites_2[i]);
            }
            window.setView(view);
            window.draw(infoAccountButton);
            window.draw(historyOrderButton);
            window.draw(spriteHistoryOrder);
            window.draw(spriteInfoAccountIcon);
            window.draw(logOutButton);
            window.draw(spriteLogOut);
            window.draw(homeButton);
            window.draw(spriteHome);
            window.draw(backButton);
            window.draw(spriteBack);
            window.draw(viewCart);
            window.draw(spriteViewCart);   
            window.draw(dienThoaiButton);
            window.draw(dienThoaiText); 
            window.draw(lapTopButton);
            window.draw(lapTopText);   
            window.draw(taiNgheButton);
            window.draw(taiNgheText);
            window.draw(tabletButton);
            window.draw(tabletText);
        }        
        else if (screenState == PRODUCTS_SCREEN3) {
            for (int i = 0; i < countProduct_3; i++) {
                window.draw(productss_3[i]);
                window.draw(nameProductss_3[i]);
                window.draw(priceProductss_3[i]);
                window.draw(productSprites_3[i]);
                window.draw(cartButtons_3[i]);
                window.draw(cartSprites_3[i]);
            }
            window.setView(view);
            window.draw(backButton);
            window.draw(spriteBack);
            window.draw(logOutButton);
            window.draw(spriteLogOut);
            window.draw(homeButton);
            window.draw(spriteHome);
            window.draw(viewCart);
            window.draw(spriteViewCart);   
            window.draw(dienThoaiButton);
            window.draw(dienThoaiText); 
            window.draw(lapTopButton);
            window.draw(lapTopText);   
            window.draw(taiNgheButton);
            window.draw(taiNgheText);
            window.draw(infoAccountButton);
            window.draw(spriteInfoAccountIcon);
            window.draw(historyOrderButton);
            window.draw(spriteHistoryOrder);
            window.draw(tabletButton);
            window.draw(tabletText);
        }
        else if (screenState == PRODUCTS_SCREEN4) {
            for (int i = 0; i < countProduct_4; i++) {
                window.draw(productss_4[i]);
                window.draw(nameProductss_4[i]);
                window.draw(priceProductss_4[i]);
                window.draw(productSprites_4[i]);
                window.draw(cartButtons_4[i]);
                window.draw(cartSprites_4[i]);
            }
            window.setView(view);
            window.draw(backButton);
            window.draw(spriteBack);
            window.draw(logOutButton);
            window.draw(spriteLogOut);
            window.draw(homeButton);
            window.draw(spriteHome);
            window.draw(viewCart);
            window.draw(spriteViewCart);   
            window.draw(dienThoaiButton);
            window.draw(dienThoaiText); 
            window.draw(lapTopButton);
            window.draw(lapTopText);   
            window.draw(taiNgheButton);
            window.draw(taiNgheText);
            window.draw(infoAccountButton);
            window.draw(spriteInfoAccountIcon);
            window.draw(historyOrderButton);
            window.draw(spriteHistoryOrder);
            window.draw(tabletButton);
            window.draw(tabletText);
        }
        // màn hình chi tiết của từng sản phẩm (laptop)
        for (int i = 0; i < countProduct_2; i++) {
            if (screenState == screenProduct2[i]) {
                view.setCenter(initialCenter); // Đưa màn hình về vị trí ban đầu
                window.setView(view);
                loadProductImages();
                sf::Texture pictureScreen;
                pictureScreen.loadFromFile(imagePaths_2[i]);
                sf::Sprite spritePictureScreen;
                spritePictureScreen.setTexture(pictureScreen);
                spritePictureScreen.setScale(0.65f, 0.65f);
                spritePictureScreen.setPosition(windowSize.x - rightMargin - 200 - 450, posY + 35);
                // Mô tả sản phẩm
                sf::Text tenTieuDeSP = createText(nameProductScreens_2[i], font_bold, 30, sf::Color::Black, windowSize.x - rightMargin - 200 - 470, posY - 90);
                sf::Text tieuDe = createText("TÍNH NĂNG NỔI BẬT", font_bold, 30, sf::Color::White, windowSize.x - rightMargin - 350 + 200, 510 - 275);
                sf::Text describeProduct = createText(fileManager->readTextTinhNangNoiBat(tinhNangNoiBat_2[i]), font, 20, sf::Color::White, windowSize.x - rightMargin - 350 + 110, 510 - 215);
                window.draw(detailProducts[i]);
                window.draw(muaNgayButtons[i]);
                window.draw(muaNgayTexts[i]);
                window.draw(muaNgayTexts[i]);
                window.draw(cartButtons_PS[i]);
                window.draw(cartSprites_PS[i]);
                window.draw(detailSpecificationButtons[i]);
                window.draw(detailSpecificationTexts[i]);
                window.draw(spritePictureScreen);
                window.draw(tenTieuDeSP);
                window.draw(tieuDe);
                window.draw(describeProduct);

                window.draw(logOutButton);
                window.draw(spriteLogOut);
                window.draw(homeButton);
                window.draw(spriteHome);
                window.draw(backButton);
                window.draw(spriteBack);
                window.draw(viewCart);
                window.draw(spriteViewCart); 
                
            }
            if (screenState == screenSpecification2[i]) {
                window.setView(view);
                sf::Text productSpecification = createText(specification_Product, font, 15, sf::Color::Black, windowSize.x - rightMargin - 350 + 300, 510 - 500);
                loadProductImages();
                sf::Texture pictureSpecification;
                pictureSpecification.loadFromFile(imagePaths_2[i]);
                sf::Sprite spritePictureSpecification;
                spritePictureSpecification.setTexture(pictureSpecification);
                spritePictureSpecification.setScale(0.8f, 0.8f);
                spritePictureSpecification.setPosition(windowSize.x - rightMargin - 200 - 400, posY);
                sf::Text tenTieuDeSP = createText(nameProducts_2[i], font_bold, 30, sf::Color::Black, windowSize.x - rightMargin - 200 - 400, posY - 120);

                window.draw(tenTieuDeSP);
                window.draw(logOutButton);
                window.draw(spriteLogOut);
                window.draw(homeButton);
                window.draw(spriteHome);
                window.draw(backButton);
                window.draw(spriteBack);
                window.draw(spritePictureSpecification);
                window.draw(productSpecification);
            }
        }
        for (int i = 0; i < countProduct_1; i++) {
            if (screenState == screenProduct1[i]) {
                view.setCenter(initialCenter); // Đưa màn hình về vị trí ban đầu
                window.setView(view);
                loadProductImages();
                sf::Texture pictureScreen;
                pictureScreen.loadFromFile(imagePaths_1[i]);
                sf::Sprite spritePictureScreen;
                spritePictureScreen.setTexture(pictureScreen);
                spritePictureScreen.setScale(0.65f, 0.65f);
                spritePictureScreen.setPosition(windowSize.x - rightMargin - 200 - 450, posY + 35);
                // Mô tả sản phẩm
                sf::Text tenTieuDeSP = createText(nameProductScreens_1[i], font_bold, 30, sf::Color::Black, windowSize.x - rightMargin - 200 - 470, posY - 90);
                sf::Text tieuDe = createText("TÍNH NĂNG NỔI BẬT", font_bold, 30, sf::Color::White, windowSize.x - rightMargin - 350 + 200, 510 - 275);
                sf::Text describeProduct = createText(fileManager->readTextTinhNangNoiBat(tinhNangNoiBat_1[i]), font, 20, sf::Color::White, windowSize.x - rightMargin - 350 + 110, 510 - 215);
                window.draw(detailProducts_1[i]);
                window.draw(muaNgayButtons_1[i]);
                window.draw(muaNgayTexts_1[i]);
                window.draw(muaNgayTexts_1[i]);
                window.draw(cartButtons_PS_1[i]);
                window.draw(cartSprites_PS_1[i]);
                window.draw(detailSpecificationButtons_1[i]);
                window.draw(detailSpecificationTexts_1[i]);
                window.draw(spritePictureScreen);
                window.draw(tenTieuDeSP);
                window.draw(tieuDe);
                window.draw(describeProduct);

                window.draw(logOutButton);
                window.draw(spriteLogOut);
                window.draw(homeButton);
                window.draw(spriteHome);
                window.draw(backButton);
                window.draw(spriteBack);
                window.draw(viewCart);
                window.draw(spriteViewCart); 
            }
            if (screenState ==  screenSpecification1[i]) {
                window.setView(view);
                sf::Text productSpecification = createText(specification_Product, font, 15, sf::Color::Black, windowSize.x - rightMargin - 350 + 300, 510 - 500);
                loadProductImages();
                sf::Texture pictureSpecification;
                pictureSpecification.loadFromFile(imagePaths_1[i]);
                sf::Sprite spritePictureSpecification;
                spritePictureSpecification.setTexture(pictureSpecification);
                spritePictureSpecification.setScale(0.8f, 0.8f);
                spritePictureSpecification.setPosition(windowSize.x - rightMargin - 200 - 400, posY);

                sf::Text tenTieuDeSP = createText(nameProducts_1[i], font_bold, 30, sf::Color::Black, windowSize.x - rightMargin - 200 - 400, posY - 90);
                
                window.draw(tenTieuDeSP);
                window.draw(logOutButton);
                window.draw(spriteLogOut);
                window.draw(homeButton);
                window.draw(spriteHome);
                window.draw(backButton);
                window.draw(spriteBack);
                window.draw(spritePictureSpecification);
                window.draw(productSpecification);
            }
        }
        for (int i = 0; i < countProduct_3; i++) {
            if (screenState == screenProduct3[i]) {
                view.setCenter(initialCenter); // Đưa màn hình về vị trí ban đầu
                window.setView(view);
                loadProductImages();
                sf::Texture pictureScreen;
                pictureScreen.loadFromFile(imagePaths_3[i]);
                sf::Sprite spritePictureScreen;
                spritePictureScreen.setTexture(pictureScreen);
                spritePictureScreen.setScale(0.65f, 0.65f);
                spritePictureScreen.setPosition(windowSize.x - rightMargin - 200 - 450, posY + 35);
                // Mô tả sản phẩm
                sf::Text tenTieuDeSP = createText(nameProductScreens_3[i], font_bold, 30, sf::Color::Black, windowSize.x - rightMargin - 200 - 470, posY - 90);
                sf::Text tieuDe = createText("TÍNH NĂNG NỔI BẬT", font_bold, 30, sf::Color::White, windowSize.x - rightMargin - 350 + 200, 510 - 275);
                sf::Text describeProduct = createText(fileManager->readTextTinhNangNoiBat(tinhNangNoiBat_3[i]), font, 20, sf::Color::White, windowSize.x - rightMargin - 350 + 110, 510 - 215);
                window.draw(detailProducts_3[i]);
                window.draw(muaNgayButtons_3[i]);
                window.draw(muaNgayTexts_3[i]);
                window.draw(muaNgayTexts_3[i]);
                window.draw(cartButtons_PS_3[i]);
                window.draw(cartSprites_PS_3[i]);
                window.draw(detailSpecificationButtons_3[i]);
                window.draw(detailSpecificationTexts_3[i]);
                window.draw(spritePictureScreen);
                window.draw(tenTieuDeSP);
                window.draw(tieuDe);
                window.draw(describeProduct);

                window.draw(logOutButton);
                window.draw(spriteLogOut);
                window.draw(homeButton);
                window.draw(spriteHome);
                window.draw(backButton);
                window.draw(spriteBack);
                window.draw(viewCart);
                window.draw(spriteViewCart); 
            }
            if (screenState ==  screenSpecification3[i]) {
                window.setView(view);
                sf::Text productSpecification = createText(specification_Product, font, 15, sf::Color::Black, windowSize.x - rightMargin - 350 + 300, posY - 90);
                loadProductImages();
                sf::Texture pictureSpecification;
                pictureSpecification.loadFromFile(imagePaths_3[i]);
                sf::Sprite spritePictureSpecification;
                spritePictureSpecification.setTexture(pictureSpecification);
                spritePictureSpecification.setScale(0.8f, 0.8f);
                spritePictureSpecification.setPosition(windowSize.x - rightMargin - 200 - 400, posY);

                sf::Text tenTieuDeSP = createText(nameProducts_3[i], font_bold, 30, sf::Color::Black, windowSize.x - rightMargin - 200 - 400, posY - 90);
                
                window.draw(tenTieuDeSP);
                window.draw(logOutButton);
                window.draw(spriteLogOut);
                window.draw(homeButton);
                window.draw(spriteHome);
                window.draw(backButton);
                window.draw(spriteBack);
                window.draw(spritePictureSpecification);
                window.draw(productSpecification);
            }
        }
        for (int i = 0; i < countProduct_4; i++) {
            if (screenState == screenProduct4[i]) {
                view.setCenter(initialCenter); // Đưa màn hình về vị trí ban đầu
                window.setView(view);
                loadProductImages();
                sf::Texture pictureScreen;
                pictureScreen.loadFromFile(imagePaths_4[i]);
                sf::Sprite spritePictureScreen;
                spritePictureScreen.setTexture(pictureScreen);
                spritePictureScreen.setScale(0.65f, 0.65f);
                spritePictureScreen.setPosition(windowSize.x - rightMargin - 200 - 450, posY + 35);
                // Mô tả sản phẩm
                sf::Text tenTieuDeSP = createText(nameProductScreens_4[i], font_bold, 30, sf::Color::Black, windowSize.x - rightMargin - 200 - 470, posY - 90);
                sf::Text tieuDe = createText("TÍNH NĂNG NỔI BẬT", font_bold, 30, sf::Color::White, windowSize.x - rightMargin - 350 + 200, 510 - 275);
                sf::Text describeProduct = createText(fileManager->readTextTinhNangNoiBat(tinhNangNoiBat_4[i]), font, 20, sf::Color::White, windowSize.x - rightMargin - 350 + 110, 510 - 215);
                window.draw(detailProducts_4[i]);
                window.draw(muaNgayButtons_4[i]);
                window.draw(muaNgayTexts_4[i]);
                window.draw(muaNgayTexts_4[i]);
                window.draw(cartButtons_PS_4[i]);
                window.draw(cartSprites_PS_4[i]);
                window.draw(detailSpecificationButtons_4[i]);
                window.draw(detailSpecificationTexts_4[i]);
                window.draw(spritePictureScreen);
                window.draw(tenTieuDeSP);
                window.draw(tieuDe);
                window.draw(describeProduct);

                window.draw(logOutButton);
                window.draw(spriteLogOut);
                window.draw(homeButton);
                window.draw(spriteHome);
                window.draw(backButton);
                window.draw(spriteBack);
                window.draw(viewCart);
                window.draw(spriteViewCart); 
            }
            if (screenState ==  screenSpecification4[i]) {
                window.setView(view);
                sf::Text productSpecification = createText(specification_Product, font, 15, sf::Color::Black, windowSize.x - rightMargin - 350 + 300, 510 - 500);
                loadProductImages();
                sf::Texture pictureSpecification;
                pictureSpecification.loadFromFile(imagePaths_4[i]);
                sf::Sprite spritePictureSpecification;
                spritePictureSpecification.setTexture(pictureSpecification);
                spritePictureSpecification.setScale(0.8f, 0.8f);
                spritePictureSpecification.setPosition(windowSize.x - rightMargin - 200 - 400, posY);

                sf::Text tenTieuDeSP = createText(nameProducts_4[i], font_bold, 30, sf::Color::Black, windowSize.x - rightMargin - 200 - 400, posY - 90);
                
                window.draw(tenTieuDeSP);
                window.draw(logOutButton);
                window.draw(spriteLogOut);
                window.draw(homeButton);
                window.draw(spriteHome);
                window.draw(backButton);
                window.draw(spriteBack);
                window.draw(spritePictureSpecification);
                window.draw(productSpecification);
            }
        }
        if (screenState == HISTORYORDER_SCREEN) {
            sf::RectangleShape backGround = createButton(470, 450, 120, 150, sf::Color(37, 178, 226));
            sf::Texture magnifyingGlass;
            magnifyingGlass.loadFromFile("Graphics/Picture/MagnifyingGlass.png");
            sf::Sprite spriteMagnifyingGlass;
            spriteMagnifyingGlass.setTexture(magnifyingGlass);
            spriteMagnifyingGlass.setScale(0.06f, 0.06f);
            spriteMagnifyingGlass.setPosition(220, 173);
            sf::Text tieuDe = createText("Chi tiết lịch sử đơn hàng", font, 40, sf::Color::Black, 120, 90);
            sf::Text tieuDe2 = createText("Kiểm tra đơn hàng", font_bold, 30, sf::Color::White, 250, 170);
            sf::Text soDienThoai = createText("Số điện thoại:", font, 20, sf::Color::White, 140, 250);
            sf::Text orderCode = createText("Mã đơn hàng:", font, 20, sf::Color::White, 140, 330);
            sf::Text notification = createText("Nếu quý khách có bất kỳ thắc mắc nào, xin vui\nlòng gọi ", font, 20, sf::Color::White, 140, 470);
            sf::Text soDienThoaiLienHe = createText("0865321921", font_bold, 17, sf::Color::White, 220, 497);

            sf::Text phoneHistoryOrderInputText = createText(phoneHistoryOrderInput, font, 20, sf::Color::Black, 145, 283); // Text để hiển thị đầu vào
            window.draw(logOutButton);
            window.draw(spriteLogOut);
            window.draw(homeButton);
            window.draw(spriteHome);
            window.draw(backButton);
            window.draw(spriteBack);
            window.draw(infoAccountButton);
            window.draw(spriteInfoAccountIcon);
            window.draw(viewCart);
            window.draw(spriteViewCart);
            window.draw(tieuDe);
            window.draw(backGround);
            window.draw(spriteMagnifyingGlass);
            window.draw(tieuDe2);
            window.draw(soDienThoai);
            window.draw(phoneHistoryOrder);
            window.draw(phoneHistoryOrderInputText);
            window.draw(displayHistoryButton);
            window.draw(displayHistoryText);
            window.draw(orderCode);
            window.draw(orderCodeHistory);
            window.draw(orderCodeHistoryInputText);
            window.draw(notification);
            window.draw(soDienThoaiLienHe);
            window.setView(view);

            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isOrderCodeHistoryOrder) 
                {
                    drawCursor(window, orderCodeHistoryInputText, 145, 363);
                }
            }
            // Load sản phẩm tương ứng ra màn hình ứng với mỗi đơn hàng
            sf::Text soThuTu = createText("STT", font, 20, sf::Color::Black, 620, 170);
            sf::Text tenSanPham = createText("Tên sản phẩm", font, 20, sf::Color::Black, 720, 170);
            sf::Text giaSanPham = createText("Tiền", font, 20, sf::Color::Black, 1050, 170);
            sf::Text soLuong = createText("Số lượng", font, 20, sf::Color::Black, 1200, 170);

            window.draw(soThuTu);
            window.draw(tenSanPham);
            window.draw(giaSanPham);
            window.draw(soLuong);

            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(620, 200)), // Điểm đầu
                sf::Vertex(sf::Vector2f(1300, 200)) // Điểm cuối
            };
                line[0].color = sf::Color(160, 160, 160);
                line[1].color = sf::Color(160, 160, 160);
            window.draw(line, 2, sf::PrimitiveType::Lines);
            
            float posFirst = 220.0f;
            double totalPriceInOrder = 0.0;
            int totalQuantity = 0;
            for (int i = 0; i < countProductsInOrder; i++) {
                sf::Text STT = createText(to_string(i + 1), font, 20, sf::Color::Black, 620, posFirst);
                // Kiểm tra độ dài của tên sản phẩm và cắt bớt nếu cần
                string tenSP = data[i].getNameProduct();
                if (tenSP.size() > 26) {
                    tenSP = tenSP.substr(0, 26) + "...";
                }
                sf::Text tenSanPhamText = createText(tenSP, font, 20, sf::Color::Black, 720, posFirst);
                string price = formatTotalAmount(data[i].getPriceProduct());
                sf::Text giaSanPhamText = createText(price, font, 20, sf::Color::Black, 1050, posFirst);
                sf::Text soLuongText = createText(to_string(data[i].getCount()), font, 20, sf::Color::Black, 1200, posFirst);

                totalPriceInOrder += data[i].getPriceProduct() * data[i].getCount(); // Tổng số tiền của đơn hàng
                totalQuantity += data[i].getCount(); // Tổng số lượng có trong đơn hàng
                window.draw(STT);
                window.draw(tenSanPhamText);
                window.draw(giaSanPhamText);
                window.draw(soLuongText);
                posFirst += 30.0f;
            }
        }
        if (screenState == ADMIN_SCREEN) {
            window.draw(displayCustomerButton);
            window.draw(displayCustomerText);
            window.draw(displayProductButton);
            window.draw(displayProductText);
            window.draw(revenueButton);
            window.draw(revenueText);
            window.draw(discountCodeButton);
            window.draw(discountCodeText);
            window.draw(logOutButton);
            window.draw(spriteLogOut);
            window.draw(backButton);
            window.draw(spriteBack);
            window.draw(detailStatisticsButton);
            window.draw(detailStatisticsText);
            drawLine(window, sf::Vector2f(250, 140), sf::Vector2f(250, 700), sf::Color(160, 160, 160));
            window.setView(view);
            window.draw(addProductButton);
            window.draw(addProductText);
        }
        if (screenState == DISPLAYCUSTOMER_SCREEN) {
            window.draw(searchCustomerButton);
            window.draw(searchCustomerText);
            window.draw(searchCustomerInputText);
            window.draw(displayCustomerButton);
            window.draw(displayCustomerText);
            window.draw(displayProductButton);
            window.draw(displayProductText);
            window.draw(revenueButton);
            window.draw(revenueText);
            window.draw(discountCodeButton);
            window.draw(discountCodeText);
            window.draw(logOutButton);
            window.draw(spriteLogOut);
            window.draw(backButton);
            window.draw(spriteBack);
            window.draw(confirmSearchCustomerButton);
            window.draw(spriteSearchIcon);
            window.draw(addProductButton);
            window.draw(addProductText);
            window.draw(detailStatisticsButton);
            window.draw(detailStatisticsText);
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(250, 140)), // Điểm đầu
                sf::Vertex(sf::Vector2f(250, 700)) // Điểm cuối
            };
                line[0].color = sf::Color(160, 160, 160);
                line[1].color = sf::Color(160, 160, 160);
            window.draw(line, 2, sf::PrimitiveType::Lines);
            window.setView(view);
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isSearchCustomer) 
                {
                    drawCursor(window, searchCustomerInputText, 280, 95);
                }
            }

            sf::Text hoVaTen = createText("Tên", font, 20, sf::Color::Black, 270, 150);
            sf::Text emailText = createText("Email", font, 20, sf::Color::Black, 600, 150);
            sf::Text phoneText = createText("Số điện thoại", font, 20, sf::Color::Black, 950, 150);

            float posFirst = 190.0f;
            for (int i = 0; i < countFoundCustomer; i++) {

                sf::Text tenKhachHang = createText(resultFoundCustomer[i].getFullName(), font, 20, sf::Color::Black, 270, posFirst);

                // Kiểm tra độ dài của email và cắt bớt nếu cần
                std::string email = resultFoundCustomer[i].getEmail();
                if (email.size() > 25) {
                    email = email.substr(0, 25) + "...";
                }
                sf::Text emailKhachHang = createText(email, font, 20, sf::Color::Black, 600, posFirst);

                sf::Text phoneKhachHang = createText(resultFoundCustomer[i].getPhone(), font, 20, sf::Color::Black, 950, posFirst);

                // Vẽ tên, email và số điện thoại khách hàng lên màn hình   
                window.draw(hoVaTen);
                window.draw(emailText);
                window.draw(phoneText);
                drawLine(window, sf::Vector2f(265, 180), sf::Vector2f(1300, 180), sf::Color(160, 160, 160));
                window.draw(tenKhachHang);
                window.draw(emailKhachHang);
                window.draw(phoneKhachHang);
                window.draw(sortCustomerButton);
                window.draw(spriteSort);

                posFirst += 50;
                drawLine(window, sf::Vector2f(265, 145), sf::Vector2f(1300, 145), sf::Color(160, 160, 160));
                // Kẻ các đường dọc
                drawLine(window, sf::Vector2f(265, posFirst - 10), sf::Vector2f(1300, posFirst - 10), sf::Color(160, 160, 160));
                drawLine(window, sf::Vector2f(265, 145), sf::Vector2f(265, posFirst - 10), sf::Color(160, 160, 160));
                drawLine(window, sf::Vector2f(550, 145), sf::Vector2f(550, posFirst - 10), sf::Color(160, 160, 160));
                drawLine(window, sf::Vector2f(900, 145), sf::Vector2f(900, posFirst - 10), sf::Color(160, 160, 160));
                drawLine(window, sf::Vector2f(1150, 145), sf::Vector2f(1150, posFirst - 10), sf::Color(160, 160, 160));
                drawLine(window, sf::Vector2f(1300, 145), sf::Vector2f(1300, posFirst - 10), sf::Color(160, 160, 160));
                
            }
           

        }
        if (screenState == DISPLAYPRODUCTS_SCREEN) {
            sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
            // Chuyển đổi vị trí chuột sang không gian của View
            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePixelPos, view);
            window.draw(searchProductButton);
            window.draw(searchProductText);
            window.draw(searchProductInputText);
            window.draw(confirmSearchCustomerButton);
            window.draw(spriteSearchIcon);
            window.draw(displayCustomerButton);
            window.draw(displayCustomerText);
            window.draw(displayProductButton);
            window.draw(displayProductText);
            window.draw(revenueButton);
            window.draw(revenueText);
            window.draw(discountCodeButton);
            window.draw(discountCodeText);
            window.draw(logOutButton);
            window.draw(spriteLogOut);
            window.draw(backButton);
            window.draw(spriteBack);
            window.draw(detailStatisticsButton);
            window.draw(detailStatisticsText);
            drawLine(window, sf::Vector2f(250, 140), sf::Vector2f(250, 5*768 + 180), sf::Color(160, 160, 160));
            window.setView(view);
            window.draw(addProductButton);
            window.draw(addProductText);
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isSearchProduct) 
                {
                    drawCursor(window, searchProductInputText, 280, 95);
                }
            }

            sf::Text tenSanPhamText = createText("Tên sản phẩm", font, 15, sf::Color::Black, 320, 150);
            sf::Text giaSanPhamText = createText("Giá tiền", font, 15, sf::Color::Black, 600, 150);
            sf::Text theLoaiSanPhamText = createText("Thể loại", font, 15, sf::Color::Black, 730, 150);
            sf::Text hangSanXuatText = createText("Nhà sản xuất", font, 15, sf::Color::Black, 850, 150);
            sf::Text quantityText = createText("Số lượng", font, 15, sf::Color::Black, 1010, 150);


            float posFirst = 190.0f;    
            for (int i = 0; i < countFoundProduct; i++) {
                string nameSP = resultFoundProducts[i].getNameProduct();
                if (nameSP.size() > 20) {
                    nameSP = nameSP.substr(0, 20) + "...";
                }
                int productID = resultFoundProducts[i].getProductID();
                sf::Sprite productSprite = getProductImageID(productID);
                productSprite.setScale(0.1f, 0.1f);
                productSprite.setPosition(270, posFirst);
                sf::Text nameProduct = createText(nameSP, font, 15, sf::Color::Black, 340, posFirst + 20);
                string priceProductTMP = formatTotalAmount(resultFoundProducts[i].getPriceProduct());
                sf::Text priceProduct = createText(priceProductTMP, font, 15, sf::Color::Black, 600, posFirst + 20);
                sf::Text genreProduct = createText(resultFoundProducts[i].getGenre(), font, 15, sf::Color::Black, 720, posFirst + 20);
                sf::Text manufacturerProduct = createText(resultFoundProducts[i].getManufacturer(), font, 15, sf::Color::Black, 870, posFirst + 20);
                sf::Text quantityProduct = createText(to_string(resultFoundProducts[i].getCount()), font, 15, sf::Color::Black, 1030, posFirst + 20);
                window.draw(tenSanPhamText);
                window.draw(giaSanPhamText);
                window.draw(theLoaiSanPhamText);
                window.draw(hangSanXuatText);
                window.draw(quantityText);
                drawLine(window, sf::Vector2f(265, 180), sf::Vector2f(1300, 180), sf::Color(160, 160, 160));
                window.draw(sortProductButton);
                window.draw(spriteSort);
                window.draw(sortProductsPriceButton);
                window.draw(spriteSortProductPrice);
                window.draw(productSprite);
                window.draw(nameProduct);
                window.draw(priceProduct);
                window.draw(genreProduct);
                window.draw(manufacturerProduct);
                window.draw(quantityProduct);

                removeProductButton[i] = createButton(60, 20, 1145, posFirst + 20, sf::Color::Red);
                removeProductText[i] = createButtonText("Xóa", font_bold, 15, sf::Color::White, removeProductButton[i]);
                editProductButton[i] = createButton(40, 20, 1215, posFirst + 20, sf::Color(20, 42, 94));
                editProductText[i] = createButtonText("Sửa", font_bold, 15, sf::Color::White, editProductButton[i]);

                posFirst += 80;
                drawLine(window, sf::Vector2f(265, 145), sf::Vector2f(1300, 145), sf::Color(160, 160, 160));
                // Kẻ các đường dọc
                drawLine(window, sf::Vector2f(265, posFirst - 10), sf::Vector2f(1300, posFirst - 10), sf::Color(160, 160, 160));
                drawLine(window, sf::Vector2f(265, 145), sf::Vector2f(265, posFirst - 10), sf::Color(160, 160, 160));
                drawLine(window, sf::Vector2f(540, 145), sf::Vector2f(540, posFirst - 10), sf::Color(160, 160, 160));
                drawLine(window, sf::Vector2f(690, 145), sf::Vector2f(690, posFirst - 10), sf::Color(160, 160, 160));
                drawLine(window, sf::Vector2f(820, 145), sf::Vector2f(820, posFirst - 10), sf::Color(160, 160, 160));
                drawLine(window, sf::Vector2f(970, 145), sf::Vector2f(970, posFirst - 10), sf::Color(160, 160, 160));
                drawLine(window, sf::Vector2f(1100, 145), sf::Vector2f(1100, posFirst - 10), sf::Color(160, 160, 160));
                drawLine(window, sf::Vector2f(1300, 145), sf::Vector2f(1300, posFirst - 10), sf::Color(160, 160, 160));

                window.draw(removeProductButton[i]);
                window.draw(removeProductText[i]);
                window.draw(editProductButton[i]);
                window.draw(editProductText[i]);
        }
            static bool mousePressEdit = false;
            // Kiểm tra sự kiện cho nút Edit
            if (event.type == sf::Event::MouseButtonPressed && !mousePressEdit) {
                if ( event.mouseButton.button == sf::Mouse::Left) {
                    mousePressEdit = true;
                    for (int i = 0; i < countFoundProduct; i++) {
                        if (editProductButton[i].getGlobalBounds().contains(mouseWorldPos)) {
                            int productID = resultFoundProducts[i].getProductID();
                            editProductID = productID;
                            screenState = EDITPRODUCT_SCREEN;
                            cout << "Edit product has ID " << resultFoundProducts[i].getProductID() << endl;
                        }
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                mousePressEdit = false;
            }

            // Kiểm tra sự kiện cho nút Remove
            static bool mousePressRemoveProduct = false;
            if (event.type == sf::Event::MouseButtonPressed && !mousePressRemoveProduct) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    mousePressRemoveProduct = true;
                    for (int i = 0; i < countFoundProduct; i++) {
                        if (removeProductButton[i].getGlobalBounds().contains(mouseWorldPos)) {
                            cout << "Remove product has ID " << resultFoundProducts[i].getProductID() << ": " << resultFoundProducts[i].getNameProduct() << endl;
                            int rowInFile = 1e9;
                            if (resultFoundProducts[i].getGenre() == "Dien Thoai") {
                                rowInFile = resultFoundProducts[i].getProductID() - 100;
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\imagePaths1.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\nameProducts1.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\nameProductScreen1.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\Price1.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\screenProduct1.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\specificationProduct1.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\tinhNangNoiBat1.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\ProductID1.txt", rowInFile - 1);
                            } 
                            else if (resultFoundProducts[i].getGenre() == "Laptop") {
                                rowInFile = resultFoundProducts[i].getProductID() - 112;
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\imagePaths2.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\nameProducts2.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\nameProductScreen2.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\Price2.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\screenProduct2.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\specificationProduct2.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\tinhNangNoiBat2.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\ProductID2.txt", rowInFile - 1);
                            } 
                            else if (resultFoundProducts[i].getGenre() == "Tai nghe") {
                                rowInFile = resultFoundProducts[i].getProductID() - 124;
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\imagePaths3.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\nameProducts3.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\nameProductScreen3.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\Price3.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\screenProduct3.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\specificationProduct3.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\tinhNangNoiBat3.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\ProductID3.txt", rowInFile - 1);
                            }
                            else if (resultFoundProducts[i].getGenre() == "Tablet") {
                                rowInFile = resultFoundProducts[i].getProductID() - 136;
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\imagePaths4.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\nameProducts4.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\nameProductScreen4.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\Price4.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\screenProduct4.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\specificationProduct4.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\tinhNangNoiBat4.txt", rowInFile - 1);
                                fileManager->deleteLineFromFile("text\\DisplayDevice\\ProductID4.txt", rowInFile - 1);
                            } 
                            // Xóa sản phẩm trong productInfo
                            int productID = resultFoundProducts[i].getProductID();
                            productInfo.erase(remove_if(productInfo.begin(), productInfo.end(), [productID](const pair<int, string>& p) {
                                return p.first == productID;
                            }), productInfo.end());
                            // Ghi lại vector vào file
                            ofstream outputFile("text\\productID_pathPicture.txt");
                            for (const auto& p : productInfo) {
                                outputFile << p.first << "|" << p.second << endl;
                            }
                            outputFile.close();
                            
                            // Xóa trong file Products.txt
                            admin->deleteProduct(resultFoundProducts[i].getProductID());
                            // countFoundProduct--;
                        }
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                mousePressRemoveProduct = false;
            }
        }
        if (screenState == DISCOUNTCODE_SCREEN) {
            window.draw(discountCodeButton);
            window.draw(discountCodeText);
            window.draw(displayCustomerButton);
            window.draw(displayCustomerText);
            window.draw(displayProductButton);
            window.draw(displayProductText);
            window.draw(revenueButton);
            window.draw(revenueText);
            window.draw(logOutButton);
            window.draw(spriteLogOut);
            window.draw(backButton);
            window.draw(spriteBack);
            window.draw(generateDiscountCodeText);
            window.draw(priceDiscountText);
            window.draw(quantityDiscountText);
            window.draw(discountCodeInputButton);
            window.draw(priceDiscountInputButton);
            window.draw(quantityDiscountInputButton);
            window.draw(discountCodeInputText);
            window.draw(priceDiscountInputText);
            window.draw(quantityDiscountInputText);
            window.draw(confirmGenerateDiscountCodeButton);
            window.draw(confirmGenerateDiscountCodeText);
            window.draw(addProductButton);
            window.draw(addProductText);
            window.draw(detailStatisticsButton);
            window.draw(detailStatisticsText);
            drawLine(window, sf::Vector2f(250, 140), sf::Vector2f(250, 700), sf::Color(160, 160, 160));

            sf::Text tieuDe = createText("TẠO MÃ GIẢM GIÁ", font_bold, 30, sf::Color::Black, 570, 90);
            window.draw(tieuDe);
            // Vẽ con trỏ
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isMaGiamGia) 
                {
                    drawCursor(window, discountCodeInputText, 460, 205);
                }
            }
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isGiaDuocGiam) 
                {
                    drawCursor(window, priceDiscountInputText, 460, 255);
                }
            }
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isSoLuongMa) 
                {
                    drawCursor(window, quantityDiscountInputText, 460, 305);
                }
            }
        }
        if (screenState == REVENUE_SCREEN) {
            window.draw(discountCodeButton);
            window.draw(discountCodeText);
            window.draw(displayCustomerButton);
            window.draw(displayCustomerText);
            window.draw(displayProductButton);
            window.draw(displayProductText);
            window.draw(revenueButton);
            window.draw(revenueText);
            window.draw(logOutButton);
            window.draw(spriteLogOut);
            window.draw(backButton);
            window.draw(spriteBack);
            window.draw(addProductButton);
            window.draw(addProductText);
            window.draw(detailStatisticsButton);
            window.draw(detailStatisticsText);
            drawLine(window, sf::Vector2f(250, 140), sf::Vector2f(250, 700), sf::Color(160, 160, 160));

            drawLine(window, sf::Vector2f(300, 700), sf::Vector2f(1300, 700), sf::Color(160, 160, 160));
            drawLine(window, sf::Vector2f(300, 700), sf::Vector2f(300, 80), sf::Color(160, 160, 160));

            drawLine(window, sf::Vector2f(300, 80), sf::Vector2f(290, 90), sf::Color(160, 160, 160));
            drawLine(window, sf::Vector2f(300, 80), sf::Vector2f(310, 90), sf::Color(160, 160, 160));
            drawLine(window, sf::Vector2f(1300, 700), sf::Vector2f(1290, 690), sf::Color(160, 160, 160));
            drawLine(window, sf::Vector2f(1300, 700), sf::Vector2f(1290, 710), sf::Color(160, 160, 160));

            sf::Text revenueText = createText("REVENUE", font, 15, sf::Color::Black, 310, 60);
            sf::Text dateText = createText("DATE", font, 15, sf::Color::Black, 1310, 700);

            window.draw(revenueText);
            window.draw(dateText);

            int posFirstX = 320; // Vị trí cơ sở của cột trên trục Y (có thể là đáy của biểu đồ)
            int columnWidth = 50; // Độ rộng của mỗi cột
            int gap = 90; // Khoảng cách giữa các cột

            for (int i = numberOfStatisticalDays - 1; i >= 0; i--) {
                dayText[i] = createText(day[i] + "/" + month[i], font, 15, sf::Color::Black, posFirstX + 10, 720);
                // Tính chiều cao cột dựa trên dữ liệu (giả sử là doanh thu)
                double columnHeight = orders->statistic(recentDay[i]) / 10; // Chia cho 10 để quy đổi

                // Tạo cột với chiều cao tính toán
                revenue[i] = createButton(columnWidth, columnHeight, posFirstX, 700 - columnHeight, sf::Color(169, 30, 58));

                // Vẽ cột
                window.draw(revenue[i]);
                window.draw(dayText[i]);
                // Di chuyển vị trí tiếp theo cho cột kế tiếp trên trục X
                posFirstX += gap;
            }
            drawLine(window, sf::Vector2f(295, 600), sf::Vector2f(305, 600), sf::Color(160, 160, 160));
            drawLine(window, sf::Vector2f(295, 500), sf::Vector2f(305, 500), sf::Color(160, 160, 160));
            drawLine(window, sf::Vector2f(295, 500), sf::Vector2f(305, 500), sf::Color(160, 160, 160));
            drawLine(window, sf::Vector2f(295, 400), sf::Vector2f(305, 400), sf::Color(160, 160, 160));
            drawLine(window, sf::Vector2f(295, 300), sf::Vector2f(305, 300), sf::Color(160, 160, 160));
            drawLine(window, sf::Vector2f(295, 200), sf::Vector2f(305, 200), sf::Color(160, 160, 160));
            drawLine(window, sf::Vector2f(295, 100), sf::Vector2f(305, 100), sf::Color(160, 160, 160));
            
            sf::Text mocGia1 = createText("$1000", font, 15, sf::Color::Black, 250, 590);
            sf::Text mocGia2 = createText("$2000", font, 15, sf::Color::Black, 250, 490);
            sf::Text mocGia3 = createText("$3000", font, 15, sf::Color::Black, 250, 390);
            sf::Text mocGia4 = createText("$4000", font, 15, sf::Color::Black, 250, 290);
            sf::Text mocGia5 = createText("$5000", font, 15, sf::Color::Black, 250, 190);
            sf::Text mocGia6 = createText("$6000", font, 15, sf::Color::Black, 250, 90);

            window.draw(mocGia1);
            window.draw(mocGia2);
            window.draw(mocGia3);
            window.draw(mocGia4);
            window.draw(mocGia5);
            window.draw(mocGia6);
        }
        if (screenState == EDITPRODUCT_SCREEN) {
            view.setCenter(initialCenter); // Đưa màn hình về vị trí ban đầu
            window.setView(view);
            sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
            // Chuyển đổi vị trí chuột sang không gian của View
            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePixelPos, view);
            sf::Text tieuDe = createText("CHỈNH SỬA THÔNG TIN SẢN PHẨM", font_bold, 30, sf::Color::Black, 500, 90);
            window.draw(tieuDe);
            window.draw(logOutButton);
            window.draw(spriteLogOut);
            window.draw(backButton);
            window.draw(spriteBack);

            window.draw(editNameProductInputButton);
            window.draw(editNameProductText);
            window.draw(editNameProductInputText);

            window.draw(editGenreInputButton);
            window.draw(editGenreText);
            window.draw(editGenreInputText);

            window.draw(editPriceProductInputButton);
            window.draw(editPriceProductText);
            window.draw(editPriceProductInputText);

            window.draw(editManufacturerInputButton);
            window.draw(editManufacturerText);
            window.draw(editManufacturerInputText);

            window.draw(editOperatingSystemInputButton);
            window.draw(editOperatingSystemText);
            window.draw(editOperatingSystemInputText);

            window.draw(editQuantityInputButton);
            window.draw(editQuantityText);
            window.draw(editQuantityInputText);

            window.draw(confirmEditProductButton);
            window.draw(confirmEditProductText);
            
            // Vẽ con trỏ
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isEditNameProduct) 
                {
                    drawCursor(window, editNameProductInputText, 480, 205);
                }
            }
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isEditGenre) 
                {
                    drawCursor(window, editGenreInputText, 480, 265);
                }
            }
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isEditPriceProduct) 
                {
                    drawCursor(window, editPriceProductInputText, 480, 325);
                }
            }

            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isEditManufacturer) 
                {
                    drawCursor(window, editManufacturerInputText, 480, 385);
                }
            }
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isEditOperatingSystem) 
                {
                    drawCursor(window, editOperatingSystemInputText, 480, 445);
                }
            }
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isEditQuantity) 
                {
                    drawCursor(window, editGenreInputText, 480, 505);
                }
            }
            if (!editNameProduct.empty() && !editGenre.empty() && !editPriceProduct.empty() && !editManufacturer.empty() && !editOperatingSystem.empty() && !editQuantity.empty()) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (confirmEditProductButton.getGlobalBounds().contains(mouseWorldPos)) {
                            bool validInput = true;

                            // Kiểm tra chuỗi editPriceProduct không rỗng trước khi chuyển đổi
                            if (!editPriceProduct.empty()) {
                                try {
                                    editPriceProductDouble = stod(editPriceProduct);
                                } catch (const invalid_argument& e) {
                                    cout << "Invalid price input for editPriceProduct. Please enter a valid number.\n";
                                    validInput = false;
                                }
                            } else {
                                cout << "Price field is empty. Please enter a valid price.\n";
                                validInput = false;
                            }

                            // Kiểm tra chuỗi editQuantity không rỗng trước khi chuyển đổi
                            if (!editQuantity.empty()) {
                                try {
                                    editQuantityINT = stoi(editQuantity);
                                } catch (const invalid_argument& e) {
                                    cout << "Invalid quantity input for editQuantity. Please enter a valid integer.\n";
                                    validInput = false;
                                }
                            } else {
                                cout << "Quantity field is empty. Please enter a valid quantity.\n";
                                validInput = false;
                            }

                            // Chỉ gọi editProduct nếu cả hai giá trị hợp lệ
                            if (validInput) {
                                admin->editProduct(editProductID, editNameProduct, editGenre, editPriceProductDouble, editManufacturer, editOperatingSystem, editQuantityINT);

                                // Xóa dữ liệu sau khi chỉnh sửa
                                editNameProduct.clear();
                                editNameProductInputText.setString("");
                                editGenre.clear();
                                editGenreInputText.setString("");
                                editPriceProduct.clear();
                                editPriceProductInputText.setString("");
                                editManufacturer.clear();
                                editManufacturerInputText.setString("");
                                editOperatingSystem.clear();
                                editOperatingSystemInputText.setString("");
                                editQuantity.clear();
                                editQuantityInputText.setString("");

                                cout << "Product edit confirmed.\n";
                            } else {
                                cout << "Product edit failed due to invalid input.\n";
                            }
                        }
                    }
                }
            }
        }
        if (screenState == ADDPRODUCTS_SCREEN) {
            sf::Text tieuDe = createText("THÊM SẢN PHẨM.", font_bold, 30, sf::Color::Black, 600, 90);
            window.draw(tieuDe);
            window.draw(discountCodeButton);
            window.draw(discountCodeText);
            window.draw(displayCustomerButton);
            window.draw(displayCustomerText);
            window.draw(displayProductButton);
            window.draw(displayProductText);
            window.draw(revenueButton);
            window.draw(revenueText);
            window.draw(logOutButton);
            window.draw(spriteLogOut);
            window.draw(backButton);
            window.draw(spriteBack);
            window.draw(addProductButton);
            window.draw(addProductText);
            window.draw(detailStatisticsButton);
            window.draw(detailStatisticsText);
            drawLine(window, sf::Vector2f(250, 140), sf::Vector2f(250, 700), sf::Color(160, 160, 160));
            window.setView(view);

            window.draw(fileNameProductInputButton);
            window.draw(fileNameProductText);
            window.draw(fileNameProductInputText);

            window.draw(fileImageProductInputButton);
            window.draw(fileImageProductText);
            window.draw(fileImageProductInputText);

            window.draw(productIDInputButton);
            window.draw(productIDText);
            window.draw(productIDInputText);

            window.draw(genreInputButton);
            window.draw(genreText);
            window.draw(genreInputText);

            window.draw(nameProductInputButton);
            window.draw(nameProductText);
            window.draw(nameProductInputText);

            window.draw(priceProductInputButton);
            window.draw(priceProductText);
            window.draw(priceProductInputText);

            window.draw(manufacturerInputButton);
            window.draw(manufacturerText);
            window.draw(manufacturerInputText);

            window.draw(operatingSystemInputButton);
            window.draw(operatingSystemText);
            window.draw(operatingSystemInputText);

            window.draw(quantityInputButton);
            window.draw(quantityText);
            window.draw(quantityInputText);

            window.draw(confirmAddProductButton);
            window.draw(confirmAddProductText);

            // Vẽ con trỏ
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isFileNameProduct) 
                {
                    drawCursor(window, fileNameProductInputText, 555, 205);
                }
            }
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isFileImageProduct) 
                {
                    drawCursor(window, fileImageProductInputText, 555, 265);
                }
            }
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isProductID) 
                {
                    drawCursor(window, productIDInputText, 555, 325);
                }
            }
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isGenre) 
                {
                    drawCursor(window, genreInputText, 555, 385);
                }
            }
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isNameProduct) 
                {
                    drawCursor(window, nameProductInputText, 555, 445);
                }
            }
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isPriceProduct) 
                {
                    drawCursor(window, priceProductInputText, 555, 505);
                }
            }
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isManufacturer) 
                {
                    drawCursor(window, manufacturerInputText, 555, 565);
                }
            }
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isOperatingSystem) 
                {
                    drawCursor(window, operatingSystemInputText, 555, 625);
                }
            }
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isQuantity) 
                {
                    drawCursor(window, operatingSystemInputText, 555, 685);
                }
            }
        }
        if (screenState == DETAILSTATISTICS_SCREEN) {
            sf::Text tieuDe = createText("THỐNG KÊ CHI TIẾT DOANH SỐ", font_bold, 30, sf::Color::Black, 500, 90);
            window.draw(tieuDe);
            window.draw(discountCodeButton);
            window.draw(discountCodeText);
            window.draw(displayCustomerButton);
            window.draw(displayCustomerText);
            window.draw(displayProductButton);
            window.draw(displayProductText);
            window.draw(revenueButton);
            window.draw(revenueText);
            window.draw(logOutButton);
            window.draw(spriteLogOut);
            window.draw(backButton);
            window.draw(spriteBack);
            window.draw(addProductButton);
            window.draw(addProductText);
            window.draw(detailStatisticsButton);
            window.draw(detailStatisticsText);
            drawLine(window, sf::Vector2f(250, 140), sf::Vector2f(250, 700), sf::Color(160, 160, 160));
            window.draw(thongKeTheoNgayButton);
            window.draw(thongKeTheoNgayText);
            window.draw(confirmStatisticButton);
            window.draw(spriteStatistic);
            window.draw(thongKeTheoNgayInputText);
            if (cursorVisible)
            {
                // Vẽ con trỏ
                if (isDate) 
                {
                    drawCursor(window, thongKeTheoNgayInputText, 275, 155);
                }
            }
            sf::Text STT = createText("STT", font, 20, sf::Color::Black, 270, 210);
            sf::Text maKhachHang = createText("Mã khách hàng", font, 20, sf::Color::Black, 390, 210);
            sf::Text maDonHang = createText("Mã đơn hàng", font, 20, sf::Color::Black, 590, 210);
            sf::Text diaChi = createText("Địa chỉ", font, 20, sf::Color::Black, 820, 210);
            sf::Text thanhTien = createText("Thành tiền", font, 20, sf::Color::Black, 1180, 210);
            window.draw(STT);
            window.draw(maKhachHang);
            window.draw(maDonHang);
            window.draw(diaChi);
            window.draw(thanhTien);
            drawLine(window, sf::Vector2f(265, 240), sf::Vector2f(1295, 240), sf::Color(160, 160, 160));
            float posFirst = 250.0f;
            
            for (int i = 0; i < countPay; i++) {

                if (dateToFind == lichSuDonHang[i].getCurrentDate()) {
                    drawLine(window, sf::Vector2f(265, 210), sf::Vector2f(1295, 210), sf::Color(160, 160, 160));

                    sf::Text soThuTuText = createText(to_string(i + 1), font, 20, sf::Color::Black, 275, posFirst);
                    sf::Text maKhachHangText = createText(to_string(lichSuDonHang[i].getCustomerID()), font, 20, sf::Color::Black, 440, posFirst);
                    sf::Text maDonHangText = createText(to_string(lichSuDonHang[i].getOrderCode()), font, 20, sf::Color::Black, 610, posFirst);

                    // Kiểm tra độ dài của địa chỉ nếu cần
                    string diaChi = lichSuDonHang[i].getAddressOrder();

                    // Cắt chuỗi nếu dài hơn 27 ký tự Unicode
                    size_t charCount = 0; // Đếm số lượng ký tự Unicode
                    size_t pos = 0;       // Vị trí để cắt chuỗi
                    for (auto it = diaChi.begin(); it != diaChi.end() && charCount < 27; ++charCount) {
                        pos = it - diaChi.begin();           // Lưu lại vị trí cuối cùng của ký tự
                        it = next(it);                  // Duyệt qua ký tự kế tiếp
                        while (it != diaChi.end() && (*it & 0xC0) == 0x80) ++it; // Bỏ qua byte tiếp theo của ký tự UTF-8
                    }

                    // Nếu chuỗi vượt quá 27 ký tự, cắt và thêm dấu "..."
                    if (charCount >= 27) {
                        diaChi = diaChi.substr(0, pos) + "...";
                    }

                    sf::Text diaChiText = createText(diaChi, font, 20, sf::Color::Black, 820, posFirst);

                    double tien = lichSuDonHang[i].getPriceTotal();
                    string totalPrice = formatTotalAmount(tien);
                    sf::Text totalPriceText = createText(totalPrice, font, 20, sf::Color::Black, 1180, posFirst);

                    window.draw(soThuTuText);
                    window.draw(maKhachHangText);
                    window.draw(maDonHangText);
                    window.draw(diaChiText);
                    window.draw(totalPriceText);
                    posFirst += 50;
                    drawLine(window, sf::Vector2f(265, posFirst - 10), sf::Vector2f(1295, posFirst - 10), sf::Color(160, 160, 160));
                    drawLine(window, sf::Vector2f(265, 210), sf::Vector2f(265, posFirst - 10), sf::Color(160, 160, 160));
                    drawLine(window, sf::Vector2f(1295, 210), sf::Vector2f(1295, posFirst - 10), sf::Color(160, 160, 160));

                    drawLine(window, sf::Vector2f(360, 210), sf::Vector2f(360, posFirst - 10), sf::Color(160, 160, 160));
                    drawLine(window, sf::Vector2f(570, 210), sf::Vector2f(570, posFirst - 10), sf::Color(160, 160, 160));
                    drawLine(window, sf::Vector2f(780, 210), sf::Vector2f(780, posFirst - 10), sf::Color(160, 160, 160));
                    drawLine(window, sf::Vector2f(1140, 210), sf::Vector2f(1140, posFirst - 10), sf::Color(160, 160, 160));
                } 
            }
            if (countPay == 0) {
                sf::Text khongCoSanPham = createText("Không có đơn hàng nào. ", font, 20, sf::Color::Red, 270, 250);
                window.draw(khongCoSanPham);
            }
        }
        window.display();
    }
    delete customers;
    delete admin;
    delete products;
    delete orders;
    delete history;
    delete discount;
    delete fileManager;
    return 0;
}