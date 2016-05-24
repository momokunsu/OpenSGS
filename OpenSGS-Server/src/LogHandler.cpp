#include "LogHandler.h"

std::string LogHandler::m_tag_buffer = "";
std::string LogHandler::m_log_buffer = "";
std::string LogHandler::m_log_srcfile;
int LogHandler::m_log_line;
eLogType LogHandler::m_log_type;

std::function<void(const std::string&, const std::string&)> LogHandler::mc_log_callback = nullptr;

void LogHandler::setLog(const char *tag, const char *log, eLogType type, const char *file, int line)
{
	m_tag_buffer = tag;
	m_log_buffer = log;
	m_log_srcfile = file;
	m_log_line = line;
	m_log_type = type;
	if (mc_log_callback != nullptr)
		mc_log_callback(tag, log);
}
