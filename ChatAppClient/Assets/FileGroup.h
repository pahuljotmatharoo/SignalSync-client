#pragma once
#include "FileUser.h"
namespace SignalSync {
	class FileGroup : public FileUser {
	private:
		std::string m_group_name;
	public:
		FileGroup() {}
		FileGroup(std::string t_filename, std::string t_username, std::string t_group_name) : FileUser(t_filename, t_username), m_group_name{ t_group_name } {}
		FileGroup(FileGroup&& fp) noexcept : FileUser(std::move(fp)) {
			m_group_name = fp.getGroupName();
			fp.setGroupName("");
		}
		std::string getGroupName() const { return m_group_name; }
		void setGroupName(const std::string t_group_name) { m_group_name = t_group_name; }
	};
}