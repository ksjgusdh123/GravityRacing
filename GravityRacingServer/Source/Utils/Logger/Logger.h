#pragma once
#include <memory>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class FLogger
{
public:
    static void Init();

    template<typename... Args>
    static void Info(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        _logger->info(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Debug(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        _logger->debug(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Warn(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        _logger->warn(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Error(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        _logger->error(fmt, std::forward<Args>(args)...);
    }

private:
    static std::shared_ptr<spdlog::logger> _logger;
};

#define __FILENAME__ std::filesystem::path(__FILE__).filename().string()

#define LOG(...) FLogger::Info(__VA_ARGS__)
#define LOGD(...) FLogger::Debug(__VA_ARGS__)
#define LOGW(...) \
    FLogger::Warn("[{}:{} {}] {}", __FILENAME__, __LINE__, __func__, std::format(__VA_ARGS__))
#define LOGE(...) \
    FLogger::Error("[{}:{} {}] {}", __FILENAME__, __LINE__, __func__, std::format(__VA_ARGS__))