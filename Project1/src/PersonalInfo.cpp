#include "ProductManager.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include "Personalnfo.h"
#include "VIP.h"
#include<cstring>
#include<iostream>
#include<regex>
#include <ctime>
#include "ProcessPersonalInfo.h"
#define  MAX_NAME_LENGTH 5
#define MAX_IDNP_LENGTH 5
#define MAX_LAST_NAME_LENGTH 5

int PersonalInfo::convertCharArrayToInt(const char* charArray) {
    int result = 0;
    int sign = 1;
    int i = 0;

    if (charArray[0] == '-') {
        sign = -1;
        i = 1;
    }

    while (charArray[i] != '\0' && std::isdigit(charArray[i])) {
        result = result * 10 + (charArray[i] - '0');
        ++i;
    }

    return result * sign;
}

void PersonalInfo::take_client_price(VIPS& vips) {
    for (const auto& it : personalData) {
        const Data& data = it.second;
        Invitations invite;
        invite.name = data.name;
        invite.last_name = data.last_name;

        if (data.price >= 500000.0f) {
            processClient(vips.getGoldVIP(), it.first, invite);
        }
        else if (data.price >= 250000.0f) {
            processClient(vips.getSilverVIP(), it.first, invite);
        }
        else if (data.price >= 50000.0f) {
            processClient(vips.getPlatinumVIP(), it.first, invite);
        }
    }
}
void PersonalInfo::processClient(VIP_Clients& vip, const std::string& idnp, Invitations& invite) {
    if (dynamic_cast<GoldVIP*>(&vip) != nullptr) {
        invite.date = vip.take_time();
        invite.time = vip.take_hours();
    }
    else if (dynamic_cast<SilverVIP*>(&vip) != nullptr) {
        invite.date = vip.take_time();
        invite.time = vip.take_hours();
    }
    else if (dynamic_cast<PlatinumVIP*>(&vip) != nullptr) {
        invite.date = vip.take_time();
        invite.time = vip.take_hours();
    }
    vip.add_vip_clients(idnp, invite);
    std::string name = invite.name;
    vip.has_enough_money = true;

    if (vip.has_enough_money && !name.empty()) {
        vip.make_invitations(name);

        const auto& it = vip.VIP.find(idnp);

        if (it != vip.VIP.end()) {
            vip.VIP.erase(idnp);
        }
    }
}



void PersonalInfo::show_invitational_btn(GoldVIP& gold,SilverVIP& silver,PlatinumVIP& platinum) {
        gold.btn_invitation();
        if (gold.has_enough_money) {
            gold.show_file();
        }
        else if (silver.has_enough_money) {
            silver.show_file();
        }
        else if (platinum.has_enough_money) {
            platinum.show_file();
        }
        

}
void add_apply_card_btn(PersonalInfo& personal,const std::string& idnp,bool& idnpValid, bool& nameValid, bool& lastNameValid) {
    float centerPosX = (ImGui::GetWindowSize().x - ImGui::CalcTextSize("Aplica cardul").x) * 0.5f;
    ImGui::SetCursorPosX(centerPosX);
     if (ImGui::Button("Aplica cardul")) {
         if (idnpValid && nameValid && lastNameValid) {
             const auto& it = personal.personalData.find(idnp);
             float price = 0.0f;
             if (it != personal.personalData.end()) {
                 unsigned int visit = it->second.visited;
                 price = it->second.price;
                 if (it->second.visited > 3 && price >= 4000.0f) {
                     personal.btn_apply = true;
                     personal.not_visited = false;
                     personal.total_visited_price = price;
                     personal.not_enough_money = false;
                     it->second.has_card = true;
                     personal.set_memory();
                 }
                  if(visit < 3){
                     personal.not_visited = true;
                 }
                 if(price < 4000.0){
                     personal.not_enough_money = true;
                 }
                 
             }
          }
     }
    
}

void PersonalInfo::check_data(bool &idnpValid,bool &nameValid,bool &lastNameValid,
    const std::string& id,const std::string& n,const std::string& p) {
    const auto it = personalData.find(IDNP);
    if (submissionCompleted) {
        if (it != personalData.end()) {
            std::string name = it->second.name;
            std::string last_name = it->second.last_name;
            if (idnpValid && nameValid && lastNameValid) {
                if (id == IDNP && n == name && p == last_name) {
                    submit = true;
                    is_same_person = true;
                }
                else if(id == IDNP && (n != name || p != last_name)){
                    is_same_person = false;
                }

            }
            else {
                submit = false;
            }
        }

    }
    check_for_visit();
    check_for_window();
   
}
void PersonalInfo::check_for_visit() {
    const auto it = personalData.find(IDNP);
    if (it != personalData.end()) {
        if (submit && is_same_person) {
            ImGui::Text("Vizitat: %u", it->second.visited);
            ImGui::Text("Suma totala acumulata pina acum: %.2f", it->second.price);
        }

        if (not_visited) {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Vizitati magazinul de cel putin 5 ori!");
        }
        if (not_enough_money) {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Acumulati suma de 4000.0 lei sau mai mult!");
        }


    }
}
void PersonalInfo::check_for_window() {
    if (window_close) {
        show_reduced_card = false;
        update_window = true;
    }
    else if (update_window) {
        show_reduced_card = true;
    }
    if (card_given) {
        show_reduced_card = false;
        update_window = false;
    }
}

void show_card(PersonalInfo& personal) {
    if (personal.show_reduced_card) {
        if (ImGui::Begin("Card de reducere!",NULL,ImGuiWindowFlags_NoCollapse)) {
            ImGui::Text("Ati obtinut un card de reducere de 25%%");
            float centerPosX = (ImGui::GetWindowSize().x - ImGui::CalcTextSize("Close").x) * 0.5f;
            ImGui::SetCursorPosX(centerPosX);
            if (ImGui::Button("Close")) {
                personal.window_close = true;
            }

        }
        ImGui::End();
    } 
}

void store_personal_data(PersonalInfo& personal, const Data& data) {
        personal.personalData[std::string(personal.IDNP)] = data;
}
void data_button(PersonalInfo& personal) {
    if (ImGui::Button("Devino client fidel")) {
        personal.showPersonal = true;
    }
}
float make_card_discount(PersonalInfo& personal, float& totalPrice) {
    float allItemPrice = totalPrice * 0.25f;
    totalPrice -= allItemPrice;
    return totalPrice;
}

void Read_name_from_file(ProcessPersonalInfo<std::string>& user_name) {
    std::string names;
    std::string path = "C:\\Users\\Mihai\\Desktop\\ImGui_project\\Files\\Users.txt";
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cout << "Error Users!";
    }
    while (std::getline(file, names)) {

        user_name.add_valid_user_name(names);
    }
}

void Read_id_from_file(ProcessPersonalInfo<int>& user_id) {
    std::string id;
    std::string idnp_path = "C:\\Users\\Mihai\\Desktop\\ImGui_project\\Files\\IDNP.txt";
    std::ifstream id_file(idnp_path);
    if (!id_file.is_open()) {
        std::cout << "error IDNP";
    }

    while (std::getline(id_file, id)) {
        int int_id = std::stoi(id);
        user_id.add_valid_user_name(int_id);
    }
    id_file.close();
}

void PersonalInfo::check_for_banned(ProcessPersonalInfo<std::string>& banned_by_name, ProcessPersonalInfo<int>& banned_by_IDNP) {
    if (banned_by_name.isBanned() || banned_by_IDNP.isBanned()) {
        is_banned = true;
    }
    else {
        is_banned = false;
    }
}


void add_data(PersonalInfo& personal,float& totalPrice) {
    if (!personal.showPersonal) {
        return;
    }

    ProcessPersonalInfo<std::string> user_name(personal.name);
    Read_name_from_file(user_name);

    ProcessPersonalInfo<int> ID(personal.convertCharArrayToInt(personal.IDNP));
    Read_id_from_file(ID);

    ProcessPersonalInfo<std::string> banned_by_name(personal.name);
    ProcessPersonalInfo<int> banned_by_IDNP(personal.convertCharArrayToInt(personal.IDNP));
    banned_by_IDNP.bannUser(10);
    banned_by_name.bannUser("Ana");
    personal.check_for_banned(banned_by_name, banned_by_IDNP);


   

    if (ImGui::Begin("Personal Data", &personal.showPersonal)) {
        personal.make_id_input();
        personal.make_name_input();
        personal.make_last_name_input();

        float centerPosX = (ImGui::GetWindowSize().x - ImGui::CalcTextSize("Submit and Buy").x) * 0.5f;
        ImGui::SetCursorPosX(centerPosX);

        if (ImGui::Button("Submit and Buy")) {
            if (personal.is_same_person) {
                if (user_name.Is_valid_user(personal.name) && !ID.Is_valid_user(personal.convertCharArrayToInt(personal.IDNP))) {
                    personal.is_invalid_name = false;
                    if (!personal.is_banned && personal.idnpValid && personal.nameValid && personal.lastNameValid && personal.accepted_price_per_visit >= 500.0f) {
                        Data& data = personal[personal.IDNP];
                        const auto& it = personal.personalData.find(personal.IDNP);
                       
                        if (!personal.is_invalid_name) {
                            it->second.price += personal.accepted_price_per_visit;
                            personal.turn_on_true();

                            if (data.visited > 3 && it->second.price >= 4000.0f) {
                                personal.turn_on_true_visit();
                                it->second.has_card = true;
                            }
                            else if (personal.accepted_price_per_visit < 500.0f) {
                                personal.turn_off_visit();
                            }
                        }
                      
                    }
                }
                else {
                    personal.is_invalid_name = true;
                }
      
            }
      
        }

        add_apply_card_btn(personal, personal.IDNP, personal.idnpValid, personal.nameValid, personal.lastNameValid);

        personal.validateInput(totalPrice);
      
        personal.check_data(personal.idnpValid, personal.nameValid,personal.lastNameValid, personal.IDNP, personal.name,personal.last_name);

        ImGui::End();
    }
    

    show_card(personal);
}



void PersonalInfo::check_for_card() {
    const auto it = personalData.find(IDNP);
    if (it != personalData.end()) {
        if (it->second.has_card) {
            card_given = true;
        }
        else {
            card_given = false;
        }
    }
}
void PersonalInfo::set_memory() {
    std::memset(IDNP, 0, sizeof(IDNP));
    std::memset(name, 0, sizeof(name));
    std::memset(last_name, 0, sizeof(last_name));
}

void PersonalInfo::validateInput(float& totalPrice) {
    if (!idnpValid) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "IDNP-ul trebuie sa contina doar cifre");
    }

    if (!nameValid) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Numele trebuie sa contina doar litere");
    }

    if (!lastNameValid) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Prenumele trebuie sa contina doar litere");
    }
    if (totalPrice < 500.0f) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Suma trebuie sa fie mai mare de 500 lei");
    }

    if (is_banned) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Utilizatorul cu acest nume sau IDNP a fost blocat!");
    }

    if (is_invalid_name) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Ati introdus un nume sau IDNP invalid");
    }
    if (!is_same_person) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Numele sau prenumle dvs nu coincide cu acest IDNP");
    }
}


bool validateIDNP(const char* idnp, bool& idnpValid) {
    try {
        std::string n = idnp;

        
        if (n.length() > MAX_NAME_LENGTH) {
            throw std::length_error("Input length exceeds maximum allowed length");
        }

       
        if (std::any_of(n.begin(), n.end(), ::isalpha)) {
            throw std::invalid_argument("IDNP must contain only digits");
        }

        
        if (std::any_of(n.begin(), n.end(), ::isspace)) {
            throw std::invalid_argument("IDNP must not contain spaces");
        }

        idnpValid = true;
        return true;
    }
    catch (const std::length_error& le) {
        std::cerr << "Length error caught for IDNP: " << le.what() << std::endl;
      
        idnpValid = false;
        return false;
    }
    catch (const std::invalid_argument& ia) {
        std::cerr << "Invalid argument caught for IDNP: " << ia.what() << std::endl;

        
        if (std::string(ia.what()) == "IDNP must not contain spaces") {
            std::cerr << "Input contains spaces" << std::endl;
        }

        idnpValid = false;
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught for IDNP: " << e.what() << std::endl;
        return false;
    }
}

bool validateName(const char* name, bool& nameValid) {
    try {
        std::string n = name;
        if (n.length() > MAX_NAME_LENGTH) {
            throw std::length_error("Input length exceeds maximum allowed length");
        }
        if (std::any_of(n.begin(), n.end(), ::isdigit)) {
            throw std::invalid_argument("Name must contain only alphabetic characters");
        }
        if (std::any_of(n.begin(), n.end(), ::isspace)) {
            throw std::invalid_argument("Name must not contain spaces");
        }

        nameValid = true;
        return true;
    }
    catch (const std::length_error& le) {
        std::cerr << "Length error caught for Name: " << le.what() << std::endl;
        
        nameValid = false;
        return false;
    }
    catch (const std::invalid_argument& ia) {
       
        if (std::string(ia.what()) == "Name must not contain spaces") {
            std::cerr << "Invalid argument caught for Name: Input contains spaces" << std::endl;
           
        }
        else {
            std::cerr << "Invalid argument caught for Name: " << ia.what() << std::endl;
            
        }
        nameValid = false;
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught for Name: " << e.what() << std::endl;
        return false;
    }
}
bool validateLastName(const char* lastName, bool& lastNameValid) {
    try {
        std::string last = lastName;
        if (last.length() > MAX_LAST_NAME_LENGTH) {
            throw std::length_error("Input length exceeds maximum allowed length");
        }
        if (std::any_of(last.begin(), last.end(), ::isdigit)) {
            throw std::invalid_argument("Last name must contain only alphabetic characters");
        }
        if (std::any_of(last.begin(), last.end(), ::isspace)) {
            throw std::invalid_argument("Last name must not contain spaces");
        }

        lastNameValid = true;
        return true;
    }
    catch (const std::length_error& le) {
        std::cerr << "Length error caught for Last Name: " << le.what() << std::endl;
        
        lastNameValid = false;
        return false;
    }
    catch (const std::invalid_argument& ia) {
       
        if (std::string(ia.what()) == "Last name must not contain spaces") {
            std::cerr << "Invalid argument caught for Last Name: Input contains spaces" << std::endl;
            
        }
        else {
            std::cerr << "Invalid argument caught for Last Name: " << ia.what() << std::endl;
           
        }
        lastNameValid = false;
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught for Last Name: " << e.what() << std::endl;
        return false;
    }
}


void PersonalInfo::turn_on_true() {
    total_visited_price += accepted_price_per_visit;
    check_for_card();
    set_memory();
    submissionCompleted = true;
    submit = true;
    accepted_price = true;

    apply_card = false;
    clear_cart = true;
    submit_pressed = true;
}
void PersonalInfo::turn_on_true_visit() {
    show_reduced_card = true;
    apply_card = true;
    update_window = false;
    window_close = false;
}
void PersonalInfo::turn_off_visit() {
    accepted_price = false;
    clear_cart = false;
    submited = false;
    submissionCompleted = false;
    submit_pressed = false;
}

void PersonalInfo::make_id_input() {
    ImGui::Text("Introduceti IDNP-ul dvs:       ");
    ImGui::SameLine();
    if (ImGui::InputText("##IDNP", IDNP, sizeof(IDNP))) {
        idnpValid = validateIDNP(IDNP, idnpValid);

    }
}

void PersonalInfo::make_name_input() {
    ImGui::Text("Introduceti numele dvs:       ");
    ImGui::SameLine();
    if (ImGui::InputText("##name", name, sizeof(name))) {
        nameValid = validateName(name, nameValid);
    }
}

void PersonalInfo::make_last_name_input() {
    ImGui::Text("Introduceti prenumele dvs:");
    ImGui::SameLine();
    if (ImGui::InputText("##lastName", last_name, sizeof(last_name))) {
        lastNameValid = validateLastName(last_name, lastNameValid);
    }
}





