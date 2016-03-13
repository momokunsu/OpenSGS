#include "LogHandler.h"

#define __Open_Log__


void LogHandler::setLog(const char *tag, const char *log)
{
#ifdef __Open_Log__
	m_tag_buffer = tag;
	m_log_buffer = log;
	if (mc_log_callback != nullptr)
		mc_log_callback(tag, log);
#endif 
}

const char * LogHandler::getCurTag()
{
#ifdef __Open_Log__
	return m_tag_buffer.c_str();
#else
	return nullptr;
#endif 
}

const char * LogHandler::getCurLog()
{
#ifdef __Open_Log__
	return m_log_buffer.c_str();
#else
	return nullptr;
#endif
}

void LogHandler::setLogEventCallback(std::function<void*(std::string, std::string)> event)
{
	mc_log_callback = event;
}
