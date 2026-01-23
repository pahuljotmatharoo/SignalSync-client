#include "File.h"
#include <string>

File::~File() {
    delete[] m_data;
    m_data = nullptr;
}

void File::downloadFile(const std::string t_fileName) const {
    std::ofstream outputFile(t_fileName, std::ios::binary);
    outputFile.write(m_data, m_size);
    outputFile.close();
}