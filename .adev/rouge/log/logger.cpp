#include "logger.h"

#include <sstream>
#include "ilog_listener.h"

namespace am {
namespace log {

	Logger *Logger::sMainLogger = NULL;

	Logger::Logger()
	{

	}
	Logger::~Logger()
	{
		
	}

	void Logger::clearListeners()
	{
		LogListeners listeners = mListeners;
		LogListeners::iterator iter;
		for (iter = listeners.begin(); iter != listeners.end(); ++iter)
		{
			(*iter)->onLoggerDelete();
		}
		mListeners.clear();
		mEntries.clear();
	}

	void Logger::log(const char *type, const char *message)
	{
		LogEntry entry(type, message);
		log(entry);
	}
	void Logger::log(const char *type, const string &message)
	{
		LogEntry entry(type, message.c_str());
		log(entry);
	}
	void Logger::log(const char *type, const stringstream &message)
	{
		LogEntry entry(type, message.str().c_str());
		log(entry);
	}
	void Logger::log(const LogEntry &entry)
	{
		mEntries.push_back(entry);
		alertListeners(entry);
	}

	void Logger::log_verbose(const char *type, const char *message, const char *file, int line)
	{
		LogEntry entry(type, message);
		log_verbose(entry, file, line);
	}
	void Logger::log_verbose(const char *type, const string &message, const char *file, int line)
	{
		LogEntry entry(type, message.c_str());
		log_verbose(entry, file, line);
	}
	void Logger::log_verbose(const char *type, const stringstream &message, const char *file, int line)
	{
		LogEntry entry(type, message.str().c_str());
		log_verbose(entry, file, line);
	}
	void Logger::log_verbose(LogEntry &entry, const char *file, int line)
	{
		stringstream ss;
		ss << " @" << file << '[' << line << ']';
		entry.mMessage += ss.str();
		mEntries.push_back(entry);
		alertListeners(entry);
	}

	void Logger::addLogListener(ILogListener *listener)
	{
		LogListeners::iterator found = findListener(listener);
		if (found == mListeners.end())
		{
			mListeners.push_back(listener);
		}
	}
	void Logger::removeLogListener(ILogListener *listener)
	{
		LogListeners::iterator found = findListener(listener);
		if (found != mListeners.end())
		{
			mListeners.erase(found);
		}
	}
	bool Logger::hasLogListener(ILogListener *listener)
	{
		LogListeners::iterator found = findListener(listener);
		return found != mListeners.end();
	}

	Logger::LogListeners::iterator Logger::findListener(ILogListener *listener)
	{
		LogListeners::iterator iter;
		for (iter = mListeners.begin(); iter != mListeners.end(); ++iter)
		{
			if (*iter == listener)
			{
				break;
			}
		}
		return iter;
	}

	void Logger::alertListeners(const LogEntry &entry)
	{
		LogListeners::iterator iter;
		for (iter = mListeners.begin(); iter != mListeners.end(); ++iter)
		{
			(*iter)->onNewEntry(entry);
		}
	}

	void Logger::getEntries(LogEntries &entries, int num, int offset, bool fromStart)
	{
		if (fromStart)
		{
			int maxIndex = max(static_cast<int>(mEntries.size()), offset + num);
			for (int i = offset; i < maxIndex; i++)
			{
				entries.push_back(mEntries[i]);
			}
		}
		else
		{
			int startIndex = max(0, static_cast<int>(mEntries.size()) - offset - num);
			
			int maxIndex = min(static_cast<int>(mEntries.size()), startIndex + num);
			for (int i = startIndex; i < maxIndex; i++)
			{
				entries.push_back(mEntries[i]);
			}
		}
	}

	void Logger::setMainLogger(Logger *logger)
	{
		sMainLogger = logger;
	}
	Logger *Logger::getMainLogger()
	{
		return sMainLogger;
	}
	void Logger::clearMainLogger()
	{
		if (sMainLogger)
		{
			Logger *logger = sMainLogger;
			sMainLogger = NULL;
			logger->clearListeners();
		}
	}

	void _log_verbose(const char *type, const char *message, const char *file, int line)
	{
		Logger::getMainLogger()->log_verbose(type, message, file, line);
	}
	void _log_verbose(const char *type, const string &message, const char *file, int line)
	{
		Logger::getMainLogger()->log_verbose(type, message, file, line);
	}
	void _log_verbose(const char *type, const stringstream &message, const char *file, int line)
	{
		Logger::getMainLogger()->log_verbose(type, message, file, line);
	}
	void _log_verbose(LogEntry &entry, const char *file, int line)
	{
		Logger::getMainLogger()->log_verbose(entry, file, line);
	}

	void _log(const char *type, const char *message)
	{
		Logger::getMainLogger()->log(type, message);
	}
	void _log(const char *type, const string &message)
	{
		Logger::getMainLogger()->log(type, message);
	}
	void _log(const char *type, const stringstream &message)
	{
		Logger::getMainLogger()->log(type, message);
	}
	void _log(const LogEntry &entry)
	{
		Logger::getMainLogger()->log(entry);
	}
}
}
