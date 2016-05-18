#pragma once
#include <functional>

enum class eLogType
{
	Print,
	Error
};

class LogHandler
{
	public:
		static void setLog(const char *tag, const char *log, eLogType type = eLogType::Print, const char *file = "", int line = 0);
		static const std::string& getCurTag();
		static const std::string& getCurLog();
		static const std::string& getCurFileName();
		static int getCurLineNum();
		static void setLogEventCallback(std::function<void (const std::string&, const std::string&)> event);

	private:
		static std::string m_tag_buffer;
		static std::string m_log_buffer;
		static std::string m_log_srcfile;
		static int m_log_line;
		static std::function<void (const std::string&, const std::string&)> mc_log_callback;
};

