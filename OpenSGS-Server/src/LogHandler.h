#pragma once
#include <functional>


class LogHandler
{
	public:
		static void setLog(const char *tag, const char *log);
		static const std::string& getCurTag();
		static const std::string& getCurLog();
		static void setLogEventCallback(std::function<void*(const std::string&, const std::string&)> event);

	private:
		static std::string m_tag_buffer;
		static std::string m_log_buffer;
		static std::function<void*(const std::string&, const std::string&)> mc_log_callback;
};

