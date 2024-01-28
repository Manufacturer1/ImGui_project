
#include "ProductManager.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include <cstdlib> 
#include "Personalnfo.h"
#include <algorithm>
#include "VIP.h"
#include<set>



void ProductManager::AddProduct(const std::string& name, const Product& product) {
    products[name] = product;
}
void ProductManager::add_Voucheres(const std::string& name, const Vouchere& voucher)
{
    Cupoane[name] = voucher;
}
void ProductManager::add_bundle_product(const std::string& name, const Bundle& bundle) {
    bundles[name] = bundle;
}
bool ProductManager::RemoveProduct(const std::string& name) {
    auto it = products.find(name);
    if (it != products.end()) {
        products.erase(it);
        return true;
    }
    return false;
}

Product ProductManager::GetProduct(const std::string& name) {
    return (*this)[name];
}

void ProductManager::DisplayProducts() {
    for (const auto& pair : products) {
        ImGui::Text("%s - Cantitatea: %d, Pretul: %.2f", pair.first.c_str(), pair.second.quantity, pair.second.price);
    }
}
float ProductManager::GetProductPrice(const std::string& name) {
    auto it = products.find(name);
    if (it != products.end()) {
        return it->second.price;
    }
    return 0.0f; 
}
void ProductManager::DecreaseProductQuantity(const std::string& productName, int quantityToDecrease) {
    auto it = products.find(productName);
    if (it != products.end()) {
        it->second = it->second - quantityToDecrease;
    }
}
void ProductManager::IncreaseProductQuantity(const std::string& productName, int quantity) {
    auto it = products.find(productName);
    if (it != products.end()) {
        it->second = it->second + quantity;
    }
}
bool ProductManager::ProductHasDiscount(const std::string& name) {
    auto it = products.find(name);
    if (it != products.end()) {
        if (it->second.discountPercentage > 0.0f) {
            return true;
        }
    }
    return false;
}
void ProductManager::add_bogo_products(const std::string& name,const Bogo &bogo) {
    BogoProducts[name] = bogo;
}

bool ProductManager::is_promo(const std::string& productName) {
    const auto& it = Cupoane.find(productName);
    if (it != Cupoane.end()) {
        return true;
    }
    return false;
}
int ProductManager::get_quantity(const std::string& name) {
    auto it = products.find(name);
    if (it != products.end()) {
        return it->second.quantity;
    }
    return 0;
}
bool ProductManager::isBogo(const std::string& productName) {
    auto it = BogoProducts.find(productName);
    if (it != BogoProducts.end()) {
        return true;
    }
    return false;
}
bool ProductManager::isBundle(const std::string& name) {
    const auto& it = bundles.find(name);
    if (it != bundles.end()) {
        return true;
    }
    return false;
}
void ProductManager::display_with_discount() {
    for (const auto& product : products) {
        if (!product.second.new_products) {
            ImGui::PushID(product.first.c_str());

            if (ImGui::Button("+##add")) {

                if (product.second.quantity > 0) {
                    DecreaseProductQuantity(product.first, 1);
                    if (cart.find(product.first) != cart.end()) {
                        cart[product.first]++;  
                    }
                    else {
                        cart[product.first] = 1;
                    }
                }
            }

            ImGui::SameLine();
            auto itCup = Cupoane.find(product.first);
            float priceProduct = product.second.price;


            if (product.second.discountPercentage > 0.0f && !product.second.new_products) {
                float discount = (product.second.discountPercentage / 100.0f) * priceProduct;
                priceProduct -= discount;

                ImGui::TextColored(ImVec4(38.0f / 255.0f, 166.0f / 255.0f, 40.0f / 255.0f, 1.0f), "%s (Discount %.2f%%) - Cantitatea: %d, Pretul: %.2f", product.first.c_str(), product.second.discountPercentage, product.second.quantity, priceProduct);

            }

            else if (itCup != Cupoane.end() && !product.second.new_products) {
                float reduced_price = priceProduct - itCup->second.reduced_Price;
                ImGui::TextColored(ImVec4(255.0f / 255.0f, 140.0f / 255.0f, 0.0f / 255.0f, 1.0f), "%s (Produs Voucher) - Cantitatea: %d, Pretul: %.2f", product.first.c_str(), product.second.quantity, reduced_price);

            }
            else {
                ImGui::Text("%s - Cantitatea: %d, Pretul: %.2f", product.first.c_str(), product.second.quantity, priceProduct);
            }

            ImGui::PopID();
        }
    }
}
void ProductManager::display_without_discount() {

            for (const auto& product : products) {
                if (!product.second.new_products) {
                    ImGui::PushID(product.first.c_str());

                    if (ImGui::Button("+##add")) {

                        if (product.second.quantity > 0) {
                            DecreaseProductQuantity(product.first, 1);
                            if (cart.find(product.first) != cart.end()) {
                                cart[product.first]++;
                            }
                            else {
                                cart[product.first] = 1;
                            }
                        }
                    }

                    ImGui::SameLine();
                    float priceProduct = product.second.price;
                     ImGui::Text("%s - Cantitatea: %d, Pretul: %.2f", product.first.c_str(), product.second.quantity, priceProduct);
                    

                    ImGui::PopID();
                }
            }
}

void ProductManager::display_products(PersonalInfo& personal,float& totalPrice) {

    if (ImGui::Begin("Produsele disponibile in magazinul 'Metro'")) {
        if (ImGui::Checkbox("Afisati produsele care au au reducere", &checkbox)) {
            if (checkbox) {
                display_discount = true;
            }
            else {
                display_discount = false;
            }

        }
        if (display_discount) {
            display_with_discount();
        }
        else {
            display_without_discount();
        }
            ImGui::Separator();
            display_new_products();
            if (ImGui::Button("Vezi BOGO Products")) {
                showBogoWindow = true;
            }
            if (ImGui::Button("Vezi Pachetele Promotionale")) {
                showBundleWindow = true;
            }
            ImGui::Separator();
            ImGui::SetWindowFontScale(1.2f);
            ImGui::TextColored(ImVec4(80.0f / 255.0f, 50.0f / 255.0f, 168.0f / 255.0f, 1.0f), "Devino un client fidel si primeste un card de reducere!");
            ImGui::SetWindowFontScale(1.0f);
            data_button(personal);
            add_data(personal,totalPrice);

            ImGui::Separator();
            ImGui::SetWindowFontScale(1.2f);
            float red = 7.0f / 255.0f;
            float green = 133.0f / 255.0f;
            float blue = 32.0f / 255.0f;
            float alpha = 1.0f;
            ImGui::TextColored(ImVec4(red, green, blue, alpha), "Acum 'Metro' este disponibil acasa!'");
            ImGui::SetWindowFontScale(1.0f);
            deliver_service_button();
            ImGui::End();
        
    }
}

void ProductManager::deliver_items_button() {
    float windowWidth = ImGui::GetWindowSize().x;
    float buttonWidth = 100.0f; 
    float centerPosX = (windowWidth - buttonWidth) * 0.5f;

    ImGui::SetCursorPosX(centerPosX);
    if (ImGui::Button("Livreaza")) {
        if (is_price_bigger && can_submit) {
            deliver_submit = true;
            show_alert_window = true;
        }
        
    }
    if (!is_price_bigger && can_submit) {
        ImGui::Text("Nu aveti nici-un produs in cosul de cumparaturi!");
    }
}

void ProductManager::deliver_service_button() {
    if (ImGui::Button("Livrare Produse")) {
        deliver_button = true;
    }   
}
void ProductManager::buy_delivered_items(float& totalPrice) {
    if (deliver_submit) {
        if (show_alert_window) {
            if (ImGui::Begin("Alert!", &show_alert_window)) {
                ImGui::Text("Atentie! Livrarea va costa 40 de lei.");
                if (ImGui::Button("OK")) {
                    std::memset(street_name, 0, sizeof(street_name));
                    std::memset(number_street, 0, sizeof(number_street));
                    ok_button = true;
                }
                else {
                    ok_button = false;
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel")) {
                    cancel_button = true;

                }
                else {
                    cancel_button = false;
                }

                ImGui::End();
            }
        }
    }
    if (ok_button) {
        totalPrice += 40;
        show_alert_window = false;
        display_delivery_message = true;
    }
    if (cancel_button) {
        show_alert_window = false;
    }
   
}

void ProductManager::deliver_window(DeliverService<bool>& user_street, DeliverService<bool>& user_street_nr) {
    if (deliver_button) {
        if (ImGui::Begin("Livrare Produse", &deliver_button)) {
            street_input(user_street);
            street_number_input(user_street_nr);
            deliver_items_button();
            if (display_good_bye && display_delivery_message) {
                ImGui::Text("Livrarea dvs va ajunge curind!");
            }
            else {
                display_delivery_message = false;
            }
            street_validation();
            
            ImGui::End();
        }
       
    }

}

void ProductManager::read_street_from_file(DeliverService<std::string>& street) {
    std::string names;
    std::string path = "C:\\Users\\Mihai\\Desktop\\ImGui_project\\Files\\Streets.txt";
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cout << "Error Streets!";
    }
    while (std::getline(file, names)) {
        street.add_valid_street(names);
    }

}

void ProductManager::street_input(DeliverService<bool>& deliver_street) {
    deliver_street.user_input_street(street_name);
    
}
void ProductManager::street_validation() {
    DeliverService<std::string> street;
    read_street_from_file(street);
    if (!street.isValid_street(street_name) && !display_delivery_message) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Strada este invalida!");
        show_alert_window = false;
        display_delivery_message = false;
    }
}
void ProductManager::street_number_input(DeliverService<bool>& deliver_street_nr) {
    deliver_street_nr.user_input_number_street(number_street);
}

void ProductManager::display_new_products() {
    ImVec4 textColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    ImGui::SetWindowFontScale(1.2f);
    ImGui::TextColored(textColor, "Acum puteti procura noile noaste produse:");
    ImGui::SetWindowFontScale(1.0f);
    for (const auto product : products) {
        if (product.second.new_products) {
            std::string name = product.first;
            ImGui::PushID(product.first.c_str());

            if (ImGui::Button("+##add")) {

                if (product.second.quantity > 0) {
                    DecreaseProductQuantity(name, 1);
                    if (cart.find(name) != cart.end()) {
                        cart[name]++;
                    }
                    else {
                        cart[name] = 1;
                    }
                }


            }
           
            ImGui::SameLine();
            float priceProduct = product.second.price;
            ImGui::Text("%s - Cantitatea: %d, Pretul: %.2f", product.first.c_str(), product.second.quantity, priceProduct);
            ImGui::PopID();
        }
        
    }
}
void ProductManager::buy_discount_products(const std::string& productName,int quantity,float &totalPrice,float& price) {
            const Product& product = GetProduct(productName);
             price = product.price;
            float discount = (product.discountPercentage / 100.0f) * price;
            price -= discount;

            float itemTotalPrice = price * quantity;
            totalPrice += itemTotalPrice;
            has_discount = true;

            ImGui::PushID(productName.c_str());
            ImGui::Text("%s - Cantitate: %d, Pretul: %.2f", productName.c_str(), quantity, price);
            ImGui::SameLine();
            if (ImGui::Button("-##remove")) {
                itemsToRemove.push_back(productName);
            }
            ImGui::PopID();
        
}


void ProductManager::buy_bogo_products(const std::string& productName,int quantity,float &totalPrice,float& price) {
        const auto& b = BogoProducts.find(productName);
        ImGui::PushID(productName.c_str());
        if (b != BogoProducts.end()) {
            has_discount = true;
            price = GetProductPrice(b->second.product);
            
            totalPrice += price * quantity;
            
            ImGui::Text("%s - Cantitate: %d, Pretul: %.2f", productName.c_str(), quantity,price);
            
        }
        ImGui::PopID();
        ImGui::SameLine();
        if (ImGui::Button("-##remove")) {
            
            itemsToRemove.push_back(productName);
        }
     
}
void ProductManager::buy_bundle_products(const std::string& productName,int quantity,float &totalPrice,float& price) {
        const auto& bundle = bundles.find(productName);
        if (bundle != bundles.end()) {
            has_discount = true;
            ImGui::PushID(productName.c_str());
            std::string first_product = bundle->second.products[0];
            std::string second_product = bundle->second.products[1];
            price = bundle->second.discountedPrice;
            totalPrice += price * quantity;
            ImGui::Text("%s - Cantitate: %d, Pretul: %.2f", productName.c_str(), quantity, price);
            ImGui::SameLine();

            if (ImGui::Button("-##remove")) {
                itemsToRemove.push_back(productName);
            }
            ImGui::PopID();
        }
}
void ProductManager::remove_items() {
            
    for (const auto& itemName : itemsToRemove) {
        cart[itemName] -= 1;
        if (cart[itemName] <= 0) {
            cart.erase(itemName);
        }
           
        IncreaseProductQuantity(itemName, 1);
        if (isBundle(itemName)) {
            auto it = bundles.find(itemName);
            std::string first_product = it->second.products[0];
            std::string second_product = it->second.products[1];
            IncreaseProductQuantity(first_product, 1);
            IncreaseProductQuantity(second_product, 1);
        }
        if (isBogo(itemName)) {
            auto it = BogoProducts.find(itemName);
            IncreaseProductQuantity(it->second.product,1);
            IncreaseProductQuantity(it->second.freeProduct, 1);
        }
        promoCodeEntered = true;

    }
    
    itemsToRemove.clear();
}
void ProductManager::buy_non_reduced_products(const std::string& productName,int quantity,float &totalPrice,float& price) {
    const auto& it = products.find(productName);
    if (it != products.end()) {
        ImGui::PushID(productName.c_str());
        totalPrice += price * quantity;
        ImGui::Text("%s - Cantitate: %d, Pretul: %.2f", productName.c_str(), quantity, price);
        ImGui::SameLine();

        if (ImGui::Button("-##remove")) {
            itemsToRemove.push_back(productName);
        }
        ImGui::PopID();

    }
}
void ProductManager::prompt_window(const std::string& productName) {
    if (windowVisible) {
        ImGui::Begin("PromoCode", &windowVisible);
        const auto& it = Cupoane.find(productName);
        if (it != Cupoane.end()) {
            float price = it->second.reduced_Price;
            std::string promoCode = it->second.promoCode;
            ImVec4 textColor = ImVec4(66.0f / 255.0f, 245.0f / 255.0f, 170.0f / 255.0f, 1.0f);
            set_memory();
            ImGui::TextColored(ImVec4(50.0f / 255.0f, 62.0f / 255.0f, 168.0f / 255.0f, 1.0f), "Ati cistigat un cupon cu reducere la %s.\nFolositi acest promocod '%s' pentru a primi reducerea de %.2f lei data viitoare!", productName.c_str(), promoCode.c_str(), price);
            voucherTaken = true;
            if (timerRunning) {
                float currentTime = ImGui::GetTime();
                float elapsedTime = currentTime - timerStartTime;
                if (elapsedTime >= timerDuration) {
                    windowVisible = false;
                    
                }
                else {

                    float remainingTime = timerDuration - elapsedTime;
                    ImGui::Text("Time remaining: %.2f seconds", remainingTime);
                    
                }
            }
        }
        ImGui::End();
    }

}

void ProductManager::take_products_without_disc() {
    for (const auto& pair : products) {
        auto it = std::find(elements.begin(), elements.end(), pair.first);
        if (!ProductHasDiscount(pair.first) && !isBogo(pair.first) && !isBundle(pair.first) && !is_promo(pair.first) && !has_discount) {
            if (it == elements.end()) {
                elements.push_back(pair.first.c_str());
            }
        }
    }
}

void ProductManager::choose_product() {
    if (display_choose_window) {
        if (ImGui::Begin("Product Selection", &choose_window)) {
            if (ImGui::Combo("Select Product", &selectedElementIndex, elements.data(), static_cast<int>(elements.size()))) {
                std::string selected_element = elements[selectedElementIndex];
                /*if (!cart.empty()) {
                    IncreaseProductQuantity(cart.begin()->first, 1);
                }*/

                // Increase quantity for the newly selected product
                DecreaseProductQuantity(selected_element, 1);
                for (auto it : cart) {
                    IncreaseProductQuantity(it.first, it.second);
                }
               
                if (!cart.empty()) {
                    cart.erase(cart.begin()->first);
                    
                }
                
                cart[selected_element]++;
                
            }
            
            if (ImGui::Button("Select")) {
                likeButtonPressed = true;
                display_choose_window = false;
                
            }

            ImGui::End();
        }
    }
    


}

void ProductManager::like_button(float &totalPrice) {
    ImGui::Separator();
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Vom aprecia daca veti promova produsele noatre pe retelele sociale!");
  
    if (ImGui::Button("Like")) {
        display_choose_window = true;
    }
    choose_product();
    if (likeButtonPressed) {
        ImVec4 textColor = ImVec4(66.0f / 255.0f, 206.0f / 255.0f, 245.0f / 255.0f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, textColor);

        ImGui::SetWindowFontScale(1.1f);


        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Va multumim pentru like! Bucurativa de o reducere de 15%%");


        ImGui::SetWindowFontScale(1.0f);

        ImGui::PopStyleColor();

    }

}
void ProductManager::like_discount(const std::string& productName, int quantity, float& totalPrice,float& price) {
    const auto& it = products.find(productName);
    if (it != products.end() && is_like_products(productName)) {
        ImGui::PushID(productName.c_str());
        float discount = 0.15f * price;
        
         if (likeButtonPressed) {
             if (productName == elements[selectedElementIndex]) {
                 price -= discount;
                 likeApplied = true;
             }
            }
        
         totalPrice += price * quantity;
        ImGui::Text("%s - Cantitate: %d, Pretul: %.2f", productName.c_str(), quantity, price);
        ImGui::SameLine();

        if (ImGui::Button("-##remove")) {
            itemsToRemove.push_back(productName);
           
        }
        ImGui::PopID();


    }
}
void ProductManager::input_promo_area(const std::string& productName, int quantity, float& totalPrice, float& price) {
    ImGui::Text("Introduceti PromoCodul");
    ImGui::SameLine();
    //std::memset(myTextBuffer, 0, sizeof(myTextBuffer));
    if (ImGui::InputText("##PromoCodulInput", myTextBuffer, sizeof(myTextBuffer))) {
              
    }
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Submit").x) * 0.5f);
    const auto& it = Cupoane.find(productName);
    if (ImGui::Button("Submit")) {
        submit_pressed = true;
    }

        if (promoCodeApplied) {
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Ati Introdus promocodul cu succes!").x) * 0.5f);
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Ati Introdus promocodul cu succes.");
            set_memory();
            promoCodeEntered = true;
            submit_pressed = false;
            
        }
}
void ProductManager::buy_promo_products(const std::string& productName, int quantity, float& totalPrice,float& price) {
    const auto& it = Cupoane.find(productName);
    if (it != Cupoane.end()) {
        ImGui::PushID(productName.c_str());
        has_discount = true;
        price = GetProductPrice(productName);
        totalPrice += price * quantity;
            if (voucherTaken && submit_pressed) {
                std::string promoCode = it->second.promoCode;
                if (std::string(myTextBuffer) == promoCode) {
                    promoCodeEntered = true;
                    it->second.has_promo_applied = true;
                    promoCodeApplied = true;
                }
            }
         if (promoCodeEntered) {
             if (it->second.has_promo_applied == true) {
                 float discountedPrice = it->second.reduced_Price;
                 totalPrice -= discountedPrice * quantity;
                 price -= discountedPrice;
             }
        }     

         

       
        if (!timerRunning) {
            timerRunning = true;
            timerStartTime = ImGui::GetTime();
        }
        ImGui::Text("%s - Cantitate: %d, Pretul: %.2f", productName.c_str(), quantity, price);
        ImGui::SameLine();
        if (ImGui::Button("-##remove")) {
            itemsToRemove.push_back(productName);
            promoCodeEntered = true;
            
        }
        ImGui::PopID();
       
    }
}
void ProductManager::display_cart(VIPS &vips,PersonalInfo& personal,GoldVIP& gold, SilverVIP& silver, PlatinumVIP& platinum) {
    if (true) {
        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Cos de cumparaturi")) {
            float totalPrice = 0.0f;
            int quantity = 0;
            std::string productName = "";
            float price = 0.0f;
            for (const auto item : cart) {
                productName = item.first;
                price = GetProductPrice(productName);
                  quantity = item.second;
                const auto& it = products.find(item.first);
                
               
                if (ProductHasDiscount(item.first)) {
                    buy_discount_products(productName, quantity,totalPrice,price);
                }
                if (isBogo(productName)) {
                    buy_bogo_products(productName, quantity,totalPrice,price);
                }
                if (isBundle(productName)) {
                    buy_bundle_products(productName, quantity,totalPrice,price);
                }
                if (is_promo(productName)) {
                    prompt_window(productName);
                    buy_promo_products(productName, quantity, totalPrice,price);
                    
                }
             
            
                if (is_like_products(productName)) {
                        like_discount(productName, quantity, totalPrice, price);
                 }
               
                else if (!ProductHasDiscount(productName) && !isBogo(productName) && !isBundle(productName) && !is_promo(productName) && has_discount) {
                    buy_non_reduced_products(productName, quantity,totalPrice,price);
                  
                }
                if (totalPrice > 0) {
                    is_price_bigger = true;
                }
                else {
                    is_price_bigger = false;
                }
                display_good_bye = false;
                buy_delivered_items(totalPrice);
        
            }
           
            take_products_without_disc();



            remove_items();
            display_total_price(personal, totalPrice);
            total = totalPrice;
            input_promo_area(productName, quantity, totalPrice, price);
      
            buy_button(personal,totalPrice);
            like_button(totalPrice);
            ImGui::Separator();
            submit_and_buy(personal, totalPrice);

            personal.take_client_price(vips);
            personal.show_invitational_btn(gold,silver,platinum);
        
        }

        ImGui::End();

    }

}

bool ProductManager::is_like_products(const std::string& productName) {
    const auto& it = std::find(elements.begin(), elements.end(), productName);
    if (it != elements.end()) {
        return true;
    }
    return false;
}



void ProductManager::submit_and_buy(PersonalInfo& personal, float& totalPrice) {
    personal.accepted_price_per_visit = totalPrice;
    if (personal.clear_cart) {
        total_update(totalPrice);
        personal.clear_cart = false;
        personal.accepted_price_per_visit = 0.0f;
        personal.total_visited_price = 0.0f;
    }
     
    
}
void ProductManager::total_update(float &totalPrice) {
    islike = false;
    likeApplied = false;
    likeButtonPressed = false;
    has_discount = false;
    bought = false;
    timerRunning = false;
    voucherTaken = false;
    promoCodeEntered = false;
    can_submit = false;
    deliver_submit = false;
    windowVisible = true;
    is_price_bigger = false;
    promoCodeApplied = false;
    submit_pressed = false;

    ok_button = false;
    cancel_button = false;
    show_alert_window = false;
    timerStartTime = 0.0f;
    timerDuration = 5.0f;
    if (totalPrice > 0.0f) {
        cart.clear();
        totalPrice = 0.0f;
    }

}
void ProductManager::buy_button(PersonalInfo&personal,float& totalPrice) {
    if (ImGui::Button("Achita")) {
        if (totalPrice > 0.0f) {
            bought = true;
            personal.btn_apply = false;
            personal.apply_card = false;
            display_good_bye = true;
        }
    }
    if (bought) {
        total_update(totalPrice);
    }
    if (display_good_bye) {
        ImGui::TextColored(ImVec4(0x11 / 255.0f, 0x7C / 255.0f, 0x6F / 255.0f, 1.0f), "Va multumim pentru cumparaturi! Va asteptam cu drag data viitoare!");
    }

}
    void ProductManager::set_memory() {
        memset(myTextBuffer, 0, sizeof(myTextBuffer));
    }
    void ProductManager::display_bundle_products() {
        if (showBundleWindow) {
            if (ImGui::Begin("Pachete Promotionale", &showBundleWindow, ImGuiWindowFlags_NoCollapse)) {
                for (const auto& bundleProducts : bundles) {
                    const Bundle& bundle = bundleProducts.second;
                    std::string name = bundleProducts.first;
                    const std::string first_product = bundle.products[0];
                    const std::string second_product = bundle.products[1];

                    int quantity1 = get_quantity(first_product);
                    int quantity2 = get_quantity(second_product);
                    float price1 = bundle.discountedPrice;

                    ImGui::PushID(name.c_str());

                    if (ImGui::Button("+##add")) {
                        DecreaseProductQuantity(first_product, 1);
                        DecreaseProductQuantity(second_product, 1);
                        cart[name]++;
                    }
                    ImGui::PopID();
                    ImGui::SameLine();
                    ImGui::Text("%s - Cantitatea: %d, Pretul: %.2f", name.c_str(), quantity1, price1);
                }
            }

            ImGui::End();
        }
    }
    void ProductManager::display_bogo_products() {
        if (showBogoWindow) {
            if (ImGui::Begin("BOGO Products",&showBogoWindow)) {
                for (const auto& bogoProduct : BogoProducts) {
                    const Bogo& bogo = bogoProduct.second;
                    const std::string& mainProduct = bogo.product;


                    int quantity = get_quantity(mainProduct);
                    float price = GetProductPrice(mainProduct);

                    ImGui::PushID(bogoProduct.first.c_str());

                    if (ImGui::Button(("+##add"))) {

                        DecreaseProductQuantity(mainProduct, 1);
                        std::string name = bogoProduct.first;
                        DecreaseProductQuantity(bogo.freeProduct, 1);
                        cart[name]++;
                    }

                    ImGui::PopID();
                    ImGui::SameLine();
                    ImGui::Text("%s - Cantitatea: %d, Pretul: %.2f", bogoProduct.first.c_str(), quantity, price);
                }
            }


            ImGui::End();
        }
   }
  
    void ProductManager::display_total_price(PersonalInfo& personal,float &totalPrice){
        ImGui::Separator();
        ImGui::Text("Pret total: %.2f", totalPrice);
        if (personal.btn_apply) {
            ImGui::Separator();
            totalPrice = make_card_discount(personal, totalPrice);
            ImGui::Text("Pret total dupa aplicarea cardului de reducere: %.2f", totalPrice);
            ImGui::Separator();

        }
    }