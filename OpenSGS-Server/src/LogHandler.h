#pragma once
#include <functional>


class LogHandler
{
	public:
		static void setLog(const char *tag, const char *log);
		static const char *getCurTag();
		static const char *getCurLog();
		static void setLogEventCallback(std::function<void*(std::string, std::string)> event);

	private:
		static std::string m_tag_buffer;
		static std::string m_log_buffer;
		static std::function<void*(std::string, std::string)> mc_log_callback;
};

