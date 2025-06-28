#include "PCH.h"
#include "Logger.h"

std::shared_ptr<spdlog::logger> FLogger::_logger;

void FLogger::Init()
{
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/server.log", true);

	console_sink->set_level(spdlog::level::trace);
	file_sink->set_level(spdlog::level::trace);

	std::vector<spdlog::sink_ptr> sinks{ console_sink, file_sink };
	_logger = std::make_shared<spdlog::logger>("GravityLogger", sinks.begin(), sinks.end());
	_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
	_logger->set_level(spdlog::level::trace);

	spdlog::set_default_logger(_logger);
}

void FLogger::Info(const std::string& msg)
{
	_logger->info(msg);
}

void FLogger::Warn(const std::string& msg)
{
	_logger->warn(msg);
}

void FLogger::Error(const std::string& msg)
{
	_logger->error(msg);
}
