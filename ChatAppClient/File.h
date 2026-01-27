#pragma once
#include <QMessageBox>
#include <fstream>
#include <cstdlib>
class File {
private:
	QString m_userFrom;
	char* m_data;
	uint32_t m_size;
public:
	File(QString t_userFrom, char* t_data, uint32_t t_size) : m_userFrom(t_userFrom), m_data((t_data)), m_size(t_size) {}
	~File();
	QString getUserFrom() const { return m_userFrom; }
	void setUserFrom(const QString& t_userFrom) { m_userFrom = t_userFrom; }
	uint32_t getSize() const { return m_size; }
	void setSize(const uint32_t& t_size) { m_size = t_size; }
	char* getData() const { return m_data; }
	void setData(char* t_data) { m_data = t_data; }
	bool downloadFile(const std::string t_fileName, const std::string t_directory_path) const;
};