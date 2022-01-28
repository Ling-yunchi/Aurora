#include "Logger.h"
#include <ctime>
#include <iostream>

Logger logger(DEFAULT_LOG_PATH);

std::string Logger::get_now_time() {
	time(&now_);
	time_info_ = localtime(&now_);
	strftime(buffer_, 255, "%Y-%m-%d %H:%M:%S", time_info_);
	return std::string(buffer_);
}

void Logger::out(type type, std::string& message) {
	std::string msg(get_now_time());
	switch (type) {
	case type::INFO:msg += " [info] "; break;
	case type::WARN:msg += " [warn] "; break;
	case type::ERROR:msg += " [error] "; break;
	}
	msg += message;
	out_ << msg << std::endl;
	if (enable_stdout_)
		std::cout << msg << std::endl;
}

Logger::Logger(std::string filename) {
	out_.open(filename, std::ios::app);
	if (!out_.is_open()) {
		std::cout << "Logger ³õÊ¼»¯Ê§°Ü" << std::endl;
	}
	enable_stdout(false);
}

Logger::~Logger() {
	out_.close();
}

void Logger::enable_stdout(bool enable) {
	enable_stdout_ = enable;
}

void Logger::info(std::string info) {
	out(type::INFO, info);
}
void Logger::warn(std::string warn) {
	out(type::WARN, warn);
}
void Logger::error(std::string error) {
	out(type::ERROR, error);
}

Logger& Logger::operator<<(std::string msg) {
	out_ << msg << std::flush;
	if (enable_stdout_)
		std::cout << msg << std::flush;
	return *this;
}

Logger& Logger::operator<<(int num) {
	out_ << num << std::flush;
	if (enable_stdout_)
		std::cout << num << std::flush;
	return *this;
}

Logger& Logger::operator<<(lg::Endl) {
	out_ << std::endl;
	if (enable_stdout_)
		std::cout << std::endl;
	return *this;
}
