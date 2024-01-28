#pragma once
#include "Personalnfo.h"
#include "ProductManager.h"
#include <unordered_map>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
    class PersonalInfo;

    struct Invitations {
        std::string name;
        std::string last_name;
        std::string date;
        std::string file_content;
        std::string time;

        Invitations(const std::string& name, const std::string& last_name, const std::string& date,const std::string& file_content,const std::string& time)
            :name(name), last_name(last_name), date(date) ,file_content(file_content),time(time){}
        Invitations():name(""),last_name(""),date(""),file_content(""),time(""){}

    };


    class VIP_Clients {
        friend class PersonalInfo;
    protected:
        bool fileIconClicked = false;
        bool showFileContent = false;
        bool open_folder_btn = false;
        std::unordered_map<std::string, Invitations> VIP;
        std::string filename = "Metro_Invitation";
        float total_vip_price = 0.0f;
        bool has_enough_money = false;
        std::string fileContent = "";

    public:
         void add_vip_clients(const std::string& idnp, Invitations& invite) {
            try {
                VIP[idnp] = invite;
            }
            catch (const std::length_error& le) {
                std::cerr << "Error in add_vip_clients (length_error): " << le.what() << std::endl;
            }
            catch (const std::out_of_range& oor) {
                std::cerr << "Error in add_vip_clients (out_of_range): " << oor.what() << std::endl;
            }
            catch (const std::exception& e) {
                std::cerr << "Error in add_vip_clients: " << e.what() << std::endl;
            }
        }
        void btn_invitation() {
            float centerPosX = (ImGui::GetWindowSize().x - ImGui::CalcTextSize("Invitatii").x) * 0.5f;
            ImGui::SetCursorPosX(centerPosX);
            if (ImGui::Button("Invitatii")) {
                std::string desktopPath = path_to_folder();
                if (!desktopPath.empty()) {
                    std::string folderPath = desktopPath;
                    try {
                        system(("start explorer.exe \"" + folderPath + "\"").c_str());
                        showFileContent = true;
                    }
                    catch (const std::system_error& se) {
                        std::cerr << "Error starting explorer.exe (system_error): " << se.what() << std::endl;
                    }
                    catch (const std::runtime_error& re) {
                        std::cerr << "Error starting explorer.exe (runtime_error): " << re.what() << std::endl;
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Error starting explorer.exe: " << e.what() << std::endl;
                    }
                }
            }
        }   
        std::string path_to_folder() {
            std::string desktopPath = "C:\\Users\\Mihai\\Desktop\\ImGui_project\\Invitatii";
            if (!desktopPath.empty()) {
                return desktopPath;
            }
            return "";
        }
        void open_files() {
                if (fileIconClicked) {
                    std::string desktopPath = path_to_folder();
                    if (!desktopPath.empty()) {
                        std::string folderPath = desktopPath;
                        std::string command = "start explorer.exe \"" + folderPath + "\"";
                        system(command.c_str());
                    }
                    fileIconClicked = false;
                }
        
        }


         void show_file() {
                if (showFileContent) {
                    if (ImGui::Begin("Invitations Display", &showFileContent)) {
                        ImGui::Text("Invitations Content:");
                        ImGui::TextWrapped("%s", fileContent.c_str());

                        if (ImGui::Button("Open Folder")) {
                            fileIconClicked = true;
                        }

                        if (ImGui::Button("Close Invitation")) {
                            showFileContent = false;
                            fileContent.clear();
                        }

                        ImGui::End();
                    }
                }
        }

        virtual std::string get_desktop_path() = 0;
        virtual void make_invitations(std::string& name) = 0;
        virtual std::string take_hours() = 0;
        virtual std::string take_time() = 0;
    };

    class GoldVIP :  public VIP_Clients {
    public:
        std::string get_desktop_path() override {
            std::string desktopPath = "C:\\Users\\Mihai\\Desktop\\ImGui_project\\Invitatii\\GoldVIP";
            if (!desktopPath.empty()) {
                return desktopPath;
            }
            return "";
        }

        void make_invitations(std::string& name) override {
            std::string uniqueFilename = filename + "_GoldVIP" + "__" + name + ".txt";
            std::string desktopPath = get_desktop_path();
            if (desktopPath.empty()) {
                return;
            }
            std::string filePath = desktopPath + "\\" + uniqueFilename;
            std::ofstream file(filePath);
            if (!file.is_open()) {
                return;
            }
            try {
                for (const auto& entry : VIP) {
                    const Invitations& invite = entry.second;
                    file << "Draga " << invite.name << " " << invite.last_name << "," << std::endl;
                    file << "Magazinul Metro va este recunoscator ca sunteti un client fidel.\nCu aceasta ocazie, administratia magazinului 'METRO' va invita sa faceti parte din ELITA magazinului nostru, devenind un membru GoldVIP.\n";
                    file << "Va invitam sa veniti pe data de " << invite.date << " la orele " << invite.time << " pentru a participa la un eveniment organizat special pentru dvs!\n";
                    file << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n";
                    file << "Cu mult respect, administratia magazinului 'METRO', pentru onoratul nostru invitat,Domnul " << invite.name << " " << invite.last_name << ".\n";
                    file << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
                }
                file.close();
            }
            catch (const std::exception& e) {
                std::cerr << "Error in writing invitations to file: " << e.what() << std::endl;
            }

        }
        std::string take_time() override {
            time_t now = time(0);
            tm localTime;
            if (localtime_s(&localTime, &now) != 0) {

                return "";
            }
            char dateStr[80];
            if (strftime(dateStr, 80, "%d-%m-%Y", &localTime) == 0) {

                return "";
            }
            return dateStr;
        }
    
        std::string take_hours() override {
            time_t now = time(0);
            tm localTime;
            if (localtime_s(&localTime, &now) != 0) {
                return "";
            }
            char timeStr[9];
            if (strftime(timeStr, 9, "%H:%M:%S", &localTime) == 0) {
                return "";
            }
            return timeStr;
        }
    };
    class SilverVIP :public VIP_Clients {
    public:
        std::string get_desktop_path() override {
            std::string desktopPath = "C:\\Users\\Mihai\\Desktop\\ImGui_project\\Invitatii\\SilverVIP";
            if (!desktopPath.empty()) {
                return desktopPath;
            }
            return "";
        }

        void make_invitations(std::string& name) override {
            std::string uniqueFilename = filename + "_SilverVIP" + "__" + name + ".txt";
            std::string desktopPath = get_desktop_path();
            if (desktopPath.empty()) {
                return;
            }
            std::string filePath = desktopPath + "\\" + uniqueFilename;
            std::ofstream file(filePath);
            if (!file.is_open()) {
                return;
            }
            try {
                for (const auto& entry : VIP) {
                    const Invitations& invite = entry.second;
                    file << "Draga " << invite.name << " " << invite.last_name << "," << std::endl;
                    file << "Magazinul Metro va este recunoscator ca sunteti un client fidel.\nCu aceasta ocazie, administratia magazinului 'METRO' va invita sa faceti parte din ELITA magazinului nostru, devenind un membru SilverVIP.\n";
                    file << "Va invitam sa veniti pe data de " << invite.date << " la orele " << invite.time << " pentru a participa la un eveniment organizat special pentru dvs!\n";
                    file << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n";
                    file << "Cu mult respect, administratia magazinului 'METRO', pentru onoratul nostru invitat,Domnul " << invite.name << " " << invite.last_name << ".\n";
                    file << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error in writing invitations to file: " << e.what() << std::endl;
            }
            file.close();
        }

        std::string take_time() override {
            const std::chrono::seconds oneWeek(7 * 24 * 60 * 60);

           
            std::time_t now = std::time(0);

           
            now += oneWeek.count();

           
            std::tm localTime;
            if (localtime_s(&localTime, &now) != 0) {
                return "";
            }

            char dateStr[80];
            if (std::strftime(dateStr, 80, "%d-%m-%Y", &localTime) == 0) {
                return "";
            }
            return dateStr;
        }

        std::string take_hours() override {
           
            std::time_t now = std::time(0);

            
            now += 2 * 60 * 60;  

            std::tm localTime;
            if (localtime_s(&localTime, &now) != 0) {
                return "";
            }

            char timeStr[9];
            if (std::strftime(timeStr, 9, "%H:%M:%S", &localTime) == 0) {
                return "";
            }

            return timeStr;
        }


    };
    class PlatinumVIP :  public VIP_Clients {
    public:
        std::string get_desktop_path() override {
            std::string desktopPath = "C:\\Users\\Mihai\\Desktop\\ImGui_project\\Invitatii\\PlatinumVIP";
            if (!desktopPath.empty()) {
                return desktopPath;
            }
            return "";
        }

        void make_invitations(std::string& name) override {
            std::string uniqueFilename = filename + "_PlatinumVIP" + "__" + name + ".txt";
            std::string desktopPath = get_desktop_path();
            if (desktopPath.empty()) {
                return;
            }
            std::string filePath = desktopPath + "\\" + uniqueFilename;
            std::ofstream file(filePath);
            if (!file.is_open()) {
                return;
            }
            try {
                for (const auto& entry : VIP) {
                    const Invitations& invite = entry.second;
                    file << "Draga " << invite.name << " " << invite.last_name << "," << std::endl;
                    file << "Magazinul Metro va este recunoscator ca sunteti un client fidel.\nCu aceasta ocazie, administratia magazinului 'METRO' va invita sa faceti parte din ELITA magazinului nostru, devenind un membru PlatinumVIP.\n";
                    file << "Va invitam sa veniti pe data de " << invite.date << " la orele " << invite.time << " pentru a participa la un eveniment organizat special pentru dvs!\n";
                    file << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n";
                    file << "Cu mult respect, administratia magazinului 'METRO', pentru onoratul nostru invitat,Domnul " << invite.name << " " << invite.last_name << ".\n";
                    file << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error in writing invitations to file: " << e.what() << std::endl;
            }
            file.close();
        }

        std::string take_time() override {
            const std::chrono::seconds oneMonth(30 * 24 * 60 * 60);

            std::time_t now = std::time(0);

            now += oneMonth.count();

            std::tm localTime;
            if (localtime_s(&localTime, &now) != 0) {
                return "";
            }

            char dateStr[80];
            if (std::strftime(dateStr, 80, "%d-%m-%Y", &localTime) == 0) {
                return "";
            }
            return dateStr;
        }

        std::string take_hours() override {
           
            std::time_t now = std::time(0);

          
            now += 5 * 60 * 60;  

            std::tm localTime;
            if (localtime_s(&localTime, &now) != 0) {
                return "";
            }

            char timeStr[9];
            if (std::strftime(timeStr, 9, "%H:%M:%S", &localTime) == 0) {
                return "";
            }

            return timeStr;
        }


    };
    class VIPS :public GoldVIP, public  SilverVIP, public PlatinumVIP {
    public:
        void add_vip_clients(const std::string& idnp, Invitations& invite)  {
            VIP_Clients::VIP[idnp] = invite;
        }



         void open_files(){
            if (VIP_Clients::fileIconClicked) {
                std::string desktopPath = VIP_Clients::path_to_folder();
                if (!desktopPath.empty()) {
                    std::string folderPath = desktopPath;
                    std::string command = "start explorer.exe \"" + folderPath + "\"";
                    system(command.c_str());
                }
               VIP_Clients::fileIconClicked = false;
           }

        }

         PlatinumVIP& getPlatinumVIP() {
             return *this;
         }
         SilverVIP& getSilverVIP() {
             return *this;
         }
         GoldVIP& getGoldVIP() {
             return *this;
         }

         void show_file() {
            if (VIP_Clients::showFileContent) {
                if (ImGui::Begin("Invitations Display", &(VIP_Clients::showFileContent))) {
                    ImGui::Text("Invitations Content:");
                    ImGui::TextWrapped("%s", VIP_Clients::fileContent.c_str());

                    if (ImGui::Button("Open Folder")) {
                        VIP_Clients::fileIconClicked = true;
                    }

                    if (ImGui::Button("Close Invitation")) {
                        VIP_Clients::showFileContent = false;
                        VIP_Clients::fileContent.clear();
                    }

                    ImGui::End();
               }
            }
        }
    };



