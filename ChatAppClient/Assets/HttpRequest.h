#pragma once
#include "httplib.h"
namespace SignalSync {
	class HTTPRequest {
	private:
		httplib::Client m_client;
	public:
		HTTPRequest(std::string url) : m_client(url) {}
		std::pair<std::size_t, std::string> connectToServer(const std::string& username, const std::string& password) {
			std::string jsonBody = "{\"username\":\"" + username + "\", \"password\":\"" + password + "\"}";
			auto response = m_client.Post("/login", jsonBody, "application/json");
			if (response && response->status == 200) {
				return std::make_pair(1, response->body);
			}
			return std::make_pair(0, "");
		}

		std::size_t registerToServer(const std::string& username, const std::string& password, const std::string& api_key) {
			std::string jsonBody = "{\"username\":\"" + username + "\", \"password\":\"" + password + "\", \"api_key\":\"" + api_key + "\"}";
			auto response = m_client.Post("/register", jsonBody, "application/json");
			if (response && response->status == 200) {
				return 1;
			}
			return 0;
		}

		std::size_t verifySession(const std::string& username, const std::string& api_key) {
			std::string jsonBody = "{\"username\":\"" + username + "\", \"api_key\":\"" + api_key + "\"}";
			auto response = m_client.Post("/validateApiKey", jsonBody, "application/json");
			if (response && response->status == 200) {
				return 1;
			}
			return 0;
		}

		std::size_t deleteToServer(const std::string& username, const std::string& password) {
			std::string jsonBody = "{\"username\":\"" + username + "\", \"password\":\"" + password + "\"}";
			auto response = m_client.Post("/delete", jsonBody, "application/json");
			if (response && response->status == 200) {
				return 1;
			}
			return 0;
		}

		std::size_t changePassword(const std::string& username, const std::string& password, const std::string& api_key) {
			std::string jsonBody = "{\"username\":\"" + username + "\", \"password\":\"" + password + "\", \"api_key\":\"" + api_key + "\"}";
			auto response = m_client.Post("/changePassword", jsonBody, "application/json");
			if (response && response->status == 200) {
				return 1;
			}
			return 0;
		}
	};
}