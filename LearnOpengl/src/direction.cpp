#include <exception>
#include <filesystem>
#include <fstream>
#include "direction.h"
#include "log.h"

Direction* Direction::m_direction = nullptr;

Direction::Direction() {
}

void Direction::loadConfig(const std::filesystem::path& path) {
    try {
        std::ifstream f(path.string());   
        config = njson::parse(f);
        m_projectRootPath = std::filesystem::path(std::string(config["projection_root_path"]));
        LOGD("projectRootPath: {}", m_projectRootPath);
    } catch (std::exception& e) {
        LOGE("{}", e.what());
    }
}
