#pragma once
#include <string>
#include <unordered_map>
#include<imgui.h>
#include <cstring>
#include <set>
#include "Personalnfo.h"
#include "VIP.h"
#include "Deliver.h"

class PersonalInfo;


struct Product {
    int quantity;
    float price;
    float discountPercentage;
    bool new_products;  // Valoarea implicită va fi inițializată aici

    Product() : quantity(0), price(0.0f), discountPercentage(0.0f), new_products(false) {}  // Inițializare implicită pentru new_products
    Product(int qty, float p, float discount, bool new_product) : quantity(qty), price(p), discountPercentage(discount), new_products(new_product) {}

    Product& operator+(int quantity) {
        this->quantity += quantity;
        return *this;
    }
    Product& operator-(int quantity) {
        this->quantity -= quantity;
        return *this;
    }
   
};


struct Bogo {
    std::string product;
    std::string freeProduct;
    Bogo():product(""),freeProduct(""){}

    Bogo(const std::string &product, const std::string &freeProduct):product(product),freeProduct(freeProduct){}
};
struct Bundle
{
    std::vector<std::string> products;
    float discountedPrice;

    Bundle() : discountedPrice(0.0) {}

    Bundle(const std::vector<std::string>& products, float discountedPrice)
        : products(products), discountedPrice(discountedPrice) {}
};
struct Vouchere
{
    std::string product;
    std::string promoCode;
    float reduced_Price;
    bool has_promo_applied = false;
    Vouchere() : reduced_Price(0.0), has_promo_applied(false) {}

    Vouchere(const std::string& product, const std::string& promoCode, float reducedPrice,bool has_promo_applied) : product(product), promoCode(promoCode), reduced_Price(reducedPrice),has_promo_applied(has_promo_applied){}
};


class ProductManager {
private:
    std::set<std::string> voucherProducts;
    char myTextBuffer[256];
    char street_name[256];
    char number_street[256];
    bool showBogoWindow = false;
    bool promoCodeEntered = false;
    bool showBundleWindow = false;
    bool promoCodeApplied = false;
    bool voucherTaken = false;
    bool windowVisible = true;
    bool likeApplied = false;
    bool islike = false;
    bool has_discount = false;
    bool likeButtonPressed = false;
    bool timerRunning = false;
    float timerDuration = 5.0f; 
    float timerStartTime = 0.0f;
    bool has_quantity = false;
    bool bought = false;
    bool checkbox = false;
    bool display_discount = false;
    bool display_good_bye = false;
    bool open_website = false;

    bool deliver_button = false;
    bool deliver_submit = false;
    bool is_price_bigger = false;
    bool ok_button = false;
    bool cancel_button = false;
    bool display_delivery_message = false;
    bool show_alert_window = false;


    int selectedElementIndex = 0;
    bool choose_window = false;
    bool display_choose_window = false;
    std::string productSelected;

    bool submit_pressed = false;
   
    
private:

    std::vector<std::string> itemsToRemove;
    std::unordered_map<std::string, int> cart;
    std::unordered_map<std::string, Bogo> BogoProducts;
    std::unordered_map<std::string, Bundle> bundles;
    std::unordered_map<std::string, Vouchere> Cupoane;
    std::unordered_map<std::string, Product> products;
    std::vector<const char*>elements;
   
    
    
   
public:
    float total = 0.0f;
    ProductManager() :myTextBuffer(),street_name(),number_street(){
        std::memset(street_name, 0, sizeof(street_name));
        std::memset(number_street, 0, sizeof(number_street));
    }
    bool can_submit = false;
    void AddProduct(const std::string& name, const Product& product);
    bool RemoveProduct(const std::string& name);
    Product GetProduct(const std::string& name);
    float GetProductPrice(const std::string& name);
    void DisplayProducts();
    void DecreaseProductQuantity(const std::string& productName, int quantityToDecrease);
    void IncreaseProductQuantity(const std::string& productName, int quantity);
    bool ProductHasDiscount(const std::string& name);
    void add_bogo_products(const std::string& name, const Bogo& bogo);
    int get_quantity(const std::string& name);
    bool isBogo(const std::string& productName);
    void add_bundle_product(const std::string& name, const Bundle& bundle);
    bool isBundle(const std::string& name);
    void add_Voucheres(const std::string& name, const Vouchere& voucher);
    void display_products(PersonalInfo& personal,float&totalPrice);
    void display_cart(VIPS& vips,PersonalInfo& personal,GoldVIP& gold, SilverVIP& silver, PlatinumVIP& platinum);
    void display_bundle_products();
    void display_bogo_products();
    void display_new_products();
    void buy_discount_products(const std::string& productName,int quantity,float &totalPrice,float& price);
    void buy_bogo_products(const std::string& productName,int quantity,float &totalPrice,float& price);
    void buy_bundle_products(const std::string& productName,int quantity,float &totalPrice,float& price);
    void remove_items();
    void submit_and_buy(PersonalInfo& personal,float &totalPrice);
    void buy_non_reduced_products(const std::string& productName,int quantity,float &totalPrice,float& price);
    void buy_promo_products(const std::string& productName, int quantity, float& totalPrice,float& price);
    void display_total_price(PersonalInfo&personal,float &totalPrice);
    void input_promo_area(const std::string& productName, int quantity, float& totalPrice, float& price);
    void prompt_window(const std::string& productName);
    bool is_promo(const std::string& productName);
    void buy_button(PersonalInfo&personal,float &totalPrice);
    void like_button(float &totalPrice);
    void like_discount(const std::string& productName, int quantity, float& totalPrice,float& price);
    void total_update(float &totalPrice);
    void set_memory();
    void display_without_discount();
    void display_with_discount();
  

    void choose_product();
    void take_products_without_disc();
    bool is_like_products(const std::string& productName);

    void deliver_service_button();
    void deliver_window(DeliverService<bool>& user_street, DeliverService<bool>& user_street_number);
    void street_input(DeliverService<bool>& deliver_street);
    void street_number_input(DeliverService<bool>& deliver_street_nr);
    void read_street_from_file(DeliverService<std::string>& street);
    void street_validation();
    void deliver_items_button();
    void buy_delivered_items(float& totalPrice);
    

    Product& operator[](const std::string& name) {
        auto it = products.find(name);
        if (it != products.end()) {
            return it->second;
        }
        else {
            return products[name] = Product();
        }
    }
};
