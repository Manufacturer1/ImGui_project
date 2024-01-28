#pragma once
#include <string>
#include <unordered_map>
#include<imgui.h>
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include <cstdlib> 
#include <cstring>
#include <set>
#include "Personalnfo.h"
#include "VIP.h"
#include "ProcessPersonalInfo.h"
#include "ProductManager.h"
#include<regex>

template <typename T>
class DeliverService {
public:
	T user_data;
	std::set<T> valid_street;
public:
	DeliverService() :user_data(T()) {}
	DeliverService(T user_data) :user_data(user_data){}
	
	template <size_t Size>
	void user_input_street(char(&valid_user_street)[Size]) {
		user_data = std::regex_match(valid_user_street, std::regex("^[A-Za-z ]+$"));

		ImGui::Text("Introduceti Strada:					");
		ImGui::SameLine();
		if (ImGui::InputText("##street", valid_user_street, sizeof(valid_user_street))) {
			user_data = std::regex_match(valid_user_street, std::regex("^[A-Za-z ]+$"));
		}
		if (!user_data) {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Numele strazii trebuie sa contina doar litere!");
		}
	}
	template <size_t Size>
	void user_input_number_street(char(&valid_user_street_nr)[Size]) {
		user_data = std::regex_match(valid_user_street_nr, std::regex("^[0-9]+$"));

		ImGui::Text("Introduceti numarul strazii:	");
		ImGui::SameLine();
		if (ImGui::InputText("##streetNumber", valid_user_street_nr, sizeof(valid_user_street_nr))) {
			user_data = std::regex_match(valid_user_street_nr, std::regex("^[0-9]+$"));
		}
		if (!user_data) {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Numarul strazii trebuie sa contina doar cifre!");
		}
	}
	template <typename N>
	void add_valid_street(N street) {
		valid_street.insert(street);
	}
	template <typename L>
	bool isValid_street(L street) {
		auto it = valid_street.find(street);
		if (it != valid_street.end()) {
			return true;
		}
		return false;
	}
	
};