#pragma once
#include <string>
class FileUser {
protected:
	std::string m_filename;
	std::string m_username;
public:
	FileUser(std::string t_filename, std::string t_username) : m_filename{ t_filename }, m_username{ t_username } {}
	FileUser(FileUser&& fu) {
		m_filename = fu.getFilename();
		m_username = fu.getUsername();
	}
	std::string getFilename() const { return m_filename; }
	std::string getUsername() const { return m_username; }
};