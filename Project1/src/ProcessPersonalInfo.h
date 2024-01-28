#pragma once
#include <string>
#include <unordered_map>
#include<imgui.h>
#include <cstring>
#include <set>
#include "Personalnfo.h"
#include "VIP.h"






// Template classa care va procesa datele introduse de utilizator

template <typename T> 
class ProcessPersonalInfo{
protected:
	T data;
	std::vector<T> banned_users;
	std::set<T> valid_users_name;

public:
	ProcessPersonalInfo(T data) :data(data){}
	ProcessPersonalInfo() : data(T()) {}
	bool show_invalid_window = false;
	


	bool isBanned() {
		for (const auto user : banned_users) {
			if (data == user) {
				return true;
			}
		}
		return false;
	}
	template <typename U>
	void bannUser(U user) {
		banned_users.push_back(user);
	}
	template <typename L>
	bool Is_valid_user(L name) {
		auto it = valid_users_name.find(name);
		if (it != valid_users_name.end()) {
			return true;
		}
		return false;
	}
	template <typename N>
	void add_valid_user_name(N user) {
		valid_users_name.insert(user);
	}

};