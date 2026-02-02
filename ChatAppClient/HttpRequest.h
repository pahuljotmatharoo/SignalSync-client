#pragma once
#include "httplib.h"
class HTTPRequest {
private:
	httplib::Client m_client;
public:
	HTTPRequest(std::string url) : m_client(url) {}
	std::size_t connectToServer(const std::string& username, const std::string& password) { return 1; };
};