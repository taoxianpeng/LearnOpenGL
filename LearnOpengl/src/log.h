#pragma once
#include <spdlog/spdlog.h>
#include <filesystem> 

inline std::string get_filename(const char* path) {
  return std::filesystem::path(path).filename().string();
}

// 设置全局日志级别为 debug

inline void init_logger() {
  spdlog::set_level(spdlog::level::debug);
}
// 定义LOG宏，包含文件名、函数名和行号

#define LOGD(...) spdlog::debug("{}:{} [{}] {}", get_filename(__FILE__), __LINE__, __FUNCTION__, fmt::format(__VA_ARGS__))
#define LOGE(...) spdlog::error("{}:{} [{}] {}", get_filename(__FILE__), __LINE__, __FUNCTION__, fmt::format(__VA_ARGS__))
#define LOGI(...) spdlog::info("{}:{} [{}] {}", get_filename(__FILE__), __LINE__, __FUNCTION__, fmt::format(__VA_ARGS__))