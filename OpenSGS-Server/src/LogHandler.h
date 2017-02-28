#pragma once
#include <string>
#include <functional>

enum class eLogType
{
	Info,
	Debug,
	Warn,
	Error
};

class LogHandler
{
	public:
		static void setLog(const char *tag, const char *log, eLogType type = eLogType::Info, const char *file = "", int line = 0);
		static const std::string& getCurTag() { return m_tag_buffer; }
		static const std::string& getCurLog() { return m_log_buffer; }
		static const std::string& getCurFileName() { return m_log_srcfile; }
		static int getCurLineNum() { return m_log_line; }
		static void setLogEventCallback(std::function<void(const std::string&, const std::string&)> event) { mc_log_callback = event; }

	private:
		static std::string m_tag_buffer;
		static std::string m_log_buffer;
		static std::string m_log_srcfile;
		static int m_log_line;
		static eLogType m_log_type;
		static std::function<void (const std::string&, const std::string&)> mc_log_callback;
};

#define __LOG_INFO_ENABLE__ 1
#define __LOG_DEBUG_ENABLE__ 1
#define __LOG_WARN_ENABLE__ 1
#define __LOG_ERROR_ENABLE__ 1

#ifdef __LOG_INFO_ENABLE__
	#define LogInfo(tag, log) (LogHandler::setLog(tag, log, eLogType::Info, __FILE__, __LINE__))
#else
	#define LogInfo(tag, log)
#endif

#ifdef __LOG_DEBUG_ENABLE__
	#define LogDebug(tag, log) (LogHandler::setLog(tag, log, eLogType::Debug, __FILE__, __LINE__))
#else
	#define LogDebug(tag, log)
#endif

#ifdef __LOG_WARN_ENABLE__
	#define LogWarn(tag, log) (LogHandler::setLog(tag, log, eLogType::Warn, __FILE__, __LINE__))
#else
	#define LogWarn(tag, log)
#endif

#ifdef __LOG_ERROR_ENABLE__
	#define LogError(tag, log) (LogHandler::setLog(tag, log, eLogType::Error, __FILE__, __LINE__))
#else
	#define LogError(tag, log)
#endif



