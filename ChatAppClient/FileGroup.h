#include "FileUser.h"
#pragma once
class FileGroup : public FileUser {
private:
	std::string m_group_name;
public:
	FileGroup(std::string t_filename, std::string t_username, std::string t_group_name) : FileUser(t_filename, t_username), m_group_name{ t_group_name } {}
	FileGroup(FileGroup&& fp) : FileUser{ fp.getFilename(), fp.getUsername() } {
		m_group_name = fp.getGroupName();
	}
	std::string getGroupName() const { return m_group_name; }
};