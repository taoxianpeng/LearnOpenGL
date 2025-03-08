#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"

using njson = nlohmann::json;
class Direction {
private:
  Direction();
  ~Direction() = default;
  Direction(const Direction&) = delete;
  Direction& operator=(const Direction&) = delete;
  

  static Direction* m_direction;
  
public:
  static Direction* getInstance() {
    if (m_direction == nullptr) {
      m_direction = new Direction();
    }
    return m_direction;
  }
  
  void loadConfig(const std::filesystem::path& path);

  const std::filesystem::path& getProjectRootPath() const {
    return m_projectRootPath;
  }
  
private:
    std::filesystem::path m_projectRootPath;
    njson config;
};