#include "LogHandler.h"

#define __Open_Log__

std::string LogHandler::m_tag_buffer = "";
std::string LogHandler::m_log_buffer = "";
std::string LogHandler::m_log_srcfile;
int LogHandler::m_log_line;

std::function<void(const std::string&, const std::string&)> LogHandler::mc_log_callback = nullptr;

void LogHandler::setLog(const char *tag, const char *log, eLogType type, const char *file, int line)
{
#ifdef __Open_Log__
	m_tag_buffer = tag;
	m_log_buffer = log;
	if (mc_log_callback != nullptr)
		mc_log_callback(tag, log);
#endif 
}

const std::string& LogHandler::getCurTag()
{
#ifdef __Open_Log__
	return m_tag_buffer;
#else
	return "";
#endif 
}

const std::string& LogHandler::getCurLog()
{
#ifdef __Open_Log__
	return m_log_buffer;
#else
	return "";
#endif
}

const std::string & LogHandler::getCurFileName()
{
	#ifdef __Open_Log__
		return m_log_srcfile;
	#else
		return "";
	#endif
}

int LogHandler::getCurLineNum()
{
	return m_log_line;
}

void LogHandler::setLogEventCallback(std::function<void (const std::string&, const std::string&)> event)
{
	mc_log_callback = event;
}
