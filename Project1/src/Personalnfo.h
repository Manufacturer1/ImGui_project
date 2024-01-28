    #pragma once
    #include <string>
    #include <unordered_map>
    #include<imgui.h>
    #include <cstring>
    #include "ProductManager.h"
    #include "VIP.h"
    #include "ProcessPersonalInfo.h"

class GoldVIP;
class SilverVIP;
class PlatinumVIP;
template <typename T>
class ProcessPersonalInfo;

bool validateIDNP(const char* idnp, bool& idnpValid);
bool validateName(const char* name, bool& nameValid);
bool validateLastName(const char* lastName, bool& lastNameValid);


    struct Data {
        char name[256];
        char last_name[256];
        unsigned int visited;
        float price = 0.0f;
        bool has_card = false;
    public:
        Data():visited(0),price(0.0f),has_card(false){
            strncpy_s(name, sizeof(name), "", sizeof(name) - 1);
            strncpy_s(last_name, sizeof(last_name), "", sizeof(last_name) - 1);
        }

        Data(const char* n, const char* ln,unsigned int visited,float price,bool has_card):visited(visited),price(price),has_card(has_card){
            strncpy_s(name, sizeof(name), n, sizeof(name) - 1);
            strncpy_s(last_name, sizeof(last_name), ln, sizeof(last_name) - 1);
        }
    };

    class PersonalInfo {
        friend class ProductManager;
    protected:
        std::unordered_map<std::string, Data> personalData;
        bool showPersonal = false;
        char IDNP[256];
        char name[256];
        char last_name[256];
        

        bool submit = false;
        bool clear_cart = false;
        bool submited = false;
        bool is_banned = false;

        bool submissionCompleted = false;
        bool accepted_price = false;
        float accepted_price_per_visit = 0.0f;
        float total_visited_price = 0.0f;
        bool apply_card = false;
        bool btn_apply = false;
        bool update_window = false;
        bool not_visited = false;
        bool not_enough_money = false;
        bool submit_pressed = false;
     
        bool is_invalid_name = false;
        bool idnpValid = false;
        bool nameValid = false;
        bool lastNameValid = false;
        bool is_same_person = true;
        bool card_given = false;

        
    public:
        bool show_reduced_card = false;
        bool window_close = false;
      
        friend class GoldVIP;
        friend class SilverVIP;
        friend class PlatinumVIP;
        friend float make_card_discount(PersonalInfo&personal,float &totalPrice);
        friend void add_apply_card_btn(PersonalInfo& personal,const std::string& idnp,bool& idnpValid, bool& nameValid, bool& lastNameValid);
        friend void add_data(PersonalInfo& personal,float& totalPrice);
        friend void data_button(PersonalInfo& personal);
        friend void show_card(PersonalInfo& personal);
        friend void store_personal_data(PersonalInfo& personal, const Data& data);
        void check_data(bool &idnpValid,bool &nameValid,bool &lastNameValid, const std::string& id,const std::string& n,const std::string& p);
        void take_client_price(VIPS& vips);
        void show_invitational_btn(GoldVIP& gold,SilverVIP& silver,PlatinumVIP& platinum);
        int convertCharArrayToInt(const char* charArray);
        void validateInput(float& totalPrice);
        void check_for_visit();
        void check_for_window();
        void check_for_card();
        void set_memory();

        void check_for_banned(ProcessPersonalInfo<std::string>& banned_by_name, ProcessPersonalInfo<int>& banned_by_IDNP);

        void make_id_input();
        void make_name_input();
        void make_last_name_input();

        void turn_on_true();
        void turn_on_true_visit();
        void turn_off_visit();
        void processClient(VIP_Clients& vip, const std::string& idnp, Invitations& invite);


        PersonalInfo(){
            std::memset(IDNP, 0, sizeof(IDNP));
            std::memset(name, 0, sizeof(name));
            std::memset(last_name, 0, sizeof(last_name));
        }
        
      
       Data& operator[](const std::string& idnp) {
           auto it = personalData.find(idnp);
           if (it != personalData.end()) {
               it->second.visited++;
               return it->second;
           }
           else {
               Data data(name, last_name, 2,0.0f,false);
               store_personal_data(*this, data);
               return personalData[idnp];
           }
       } 
       PersonalInfo& operator+(float price) {
           for (auto& entry : personalData) {
               entry.second.price += price;
           }
           return *this;
       }
    };


 