#pragma once
#include <memory>
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
    static void Warn(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        _logger->warn(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Error(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        _logger->error(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Debug(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        _logger->debug(fmt, std::forward<Args>(args)...);
    }

private:
    static std::shared_ptr<spdlog::logger> _logger;
};