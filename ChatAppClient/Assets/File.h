#pragma once
#include <fstream>
#include <cstdlib>
#include <QString>
namespace SignalSync {
	class File {
	private:
		QString m_userFrom;
		char* m_data;
		uint32_t m_size;
	public:
		File() {}
		File(QString t_userFrom, char* t_data, uint32_t t_size) : m_userFrom(t_userFrom), m_data((t_data)), m_size(t_size) {}
		File(File&& other_file) noexcept {
			m_userFrom = other_file.getUserFrom();
			other_file.setUserFrom("");
			m_data = other_file.getData();
			other_file.setData(nullptr);
			m_size = other_file.getSize();
			other_file.setSize(0);
		}
		File& operator=(File&& other_file) noexcept {
			m_userFrom = other_file.getUserFrom();
			other_file.setUserFrom("");
			m_data = other_file.getData();
			other_file.setData(nullptr);
			m_size = other_file.getSize();
			other_file.setSize(0);
			return *this;
		}
		~File() {
			delete[] m_data;
			m_data = nullptr;
		}
		QString getUserFrom() const { return m_userFrom; }
		void setUserFrom(const QString& t_userFrom) { m_userFrom = t_userFrom; }
		uint32_t getSize() const { return m_size; }
		void setSize(const uint32_t& t_size) { m_size = t_size; }
		char* getData() const { return m_data; }
		void setData(char* t_data) { m_data = t_data; }
		bool downloadFile(const std::string t_fileName, const std::string t_directory_path) const;
	};
}