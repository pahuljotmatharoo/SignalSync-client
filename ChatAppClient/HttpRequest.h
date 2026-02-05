#pragma once
#include "httplib.h"
class HTTPRequest {
private:
	httplib::Client m_client;
public:
	HTTPRequest(std::string url) : m_client(url) {}
	std::size_t connectToServer(const std::string& username, const std::string& password) {
		std::string jsonBody = "{\"username\":\"" + username + "\", \"password\":\"" + password + "\"}";
		auto response = m_client.Post("/login", jsonBody, "application/json");
		if (response && response->status == 200) {
			return 1;
		}
		return 0;
	};

	std::size_t registerToServer(const std::string& username, const std::string& password) {
		std::string jsonBody = "{\"username\":\"" + username + "\", \"password\":\"" + password + "\"}";
		auto response = m_client.Post("/register", jsonBody, "application/json");
		if (response && response->status == 200) {
			return 1;
		}
		return 0;
	}
};