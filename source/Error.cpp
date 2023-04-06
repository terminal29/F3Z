#include <Error.h>

Error::Error()
{
}

Error::~Error()
{
}

void Error::throwError(std::string errText)
{
	Log::instance().writeLine(Logfile::LOG_GENERAL, errText);
	// errorConf err;
	// errorInit(&err, errorType::ERROR_TEXT_WORD_WRAP, CFG_Language::CFG_LANGUAGE_EN);
	// errorText(&err, errText.c_str());
	// errorDisp(&err);
	// svcExitProcess();
	std::cout << errText << std::endl;
	Error::haltExec(); // for good measure
}

void Error::haltExec()
{
	while (true)
	{
		svcSleepThread(1000);
	}
}
