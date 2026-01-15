#pragma once

#include <iostream>
#include <fstream>
#include <string_view>
#include <format>
#include <source_location>
#include <chrono>
#include <mutex>
#include <filesystem>

enum class LogLevel
{
	Info,
	Warning,
	Error
};

class Logger
{
public:
	template<typename... Args>
	static void Log(LogLevel level, const std::source_location& location, std::string_view format, Args&&... args)
	{
		static std::mutex logMutex;
		std::lock_guard<std::mutex> lock(logMutex);

		auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());

		std::string_view levelStr = "[INFO]";
		if (level == LogLevel::Warning)
			levelStr = "[WARN]";
		else if (level == LogLevel::Error)
			levelStr = "[ERR]";

		std::string userMessage;
		try {
			userMessage = std::vformat(format, std::make_format_args(args...));
		}
		catch (...) {
			userMessage = "LOG FORMAT ERROR";
		}

		std::string finalLogLine = std::format("{:%H:%M:%S} {} [{}:{}] {}\n",
			time, levelStr, GetFileName(location.file_name()), location.line(), userMessage);

		std::cout << finalLogLine;
		
		static std::ofstream logFile = OpenLogFile();
		if (logFile.is_open())
		{
			logFile << finalLogLine;
			logFile.flush();
		}
	}

	template <typename... Args>
	static void Info(std::string_view format, Args&&... args)
	{
		Log(LogLevel::Info, std::source_location::current(), format, std::forward<Args>(args)...);
	}

	template <typename... Args>
	static void Warn(std::string_view format, Args&&... args)
	{
		Log(LogLevel::Warning, std::source_location::current(), format, std::forward<Args>(args)...);
	}

	template <typename... Args>
	static void Error(std::string_view format, Args&&... args)
	{
		Log(LogLevel::Error, std::source_location::current(), format, std::forward<Args>(args)...);
	}

private:
	static constexpr std::string_view GetFileName(const char* path)
	{
		std::string_view p(path);
		auto pos = p.find_last_of("/\\");
		if (pos != std::string_view::npos)
			return p.substr(pos + 1);
		return p;
	}

	static std::ofstream OpenLogFile()
	{
		namespace fs = std::filesystem;

		try {
			if (!fs::exists("Logs"))
				fs::create_directories("Logs");

			std::ofstream file("Logs/server.log", std::ios::app);
			return file;
		}
		catch (...) {
			return std::ofstream();
		}
	}
};
