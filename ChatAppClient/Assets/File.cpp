#include "File.h"
#include <string>
namespace SignalSync {
    bool File::downloadFile(const std::string t_fileName, const std::string t_directory_path) const {
        std::ofstream outputFile(t_directory_path + "/" + t_fileName, std::ios::binary);
        outputFile.write(m_data, m_size);
        if (outputFile.fail() || outputFile.bad()) {
            outputFile.close();
            return false;
        }
        else {
            outputFile.close();
            return true;
        }
    }
}