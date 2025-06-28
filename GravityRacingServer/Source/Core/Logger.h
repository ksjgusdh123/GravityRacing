#pragma once
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class FLogger
{
public:
	static void Init();

	static void Info(const std::string& msg);
	static void Warn(const std::string& msg);
	static void Error(const std::string& msg);

private:
	static std::shared_ptr<spdlog::logger> _logger;
};