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
	_logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
	_logger->set_level(spdlog::level::trace);

	spdlog::set_default_logger(_logger);
}
