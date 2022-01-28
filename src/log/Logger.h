#pragma once
#include <string>
#include <fstream>
# define DEFAULT_LOG_PATH "./log.txt"

namespace lg
{
	enum Endl { endl };
};


class Logger
{
	std::ofstream out_;
	bool enable_stdout_;
	char buffer_[255];
	time_t now_;
	tm* time_info_;

	enum class type { INFO, WARN, ERROR };
	void out(type type, std::string& message);
public:
	Logger(std::string filename);
	~Logger();
	void enable_stdout(bool enable);
	void info(std::string info);
	void warn(std::string warn);
	void error(std::string error);
	std::string get_now_time();
	Logger& operator << (std::string msg);
	Logger& operator << (int num);
	Logger& operator << (lg::Endl endl);

};

extern Logger logger;
