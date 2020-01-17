#include <cstring>
#include <cstdio>
#include <ctime>

#include "debugutils.h"

DebugUtils::DebugUtils()
{
    getCurrentTime();
}

DebugUtils::~DebugUtils()
{

}

void DebugUtils::logCurrentOpcode(char *data)
{
    char buffer[20] = "opcode_log";
    strcat(buffer, currentTime);
    char *finalName = strcat(buffer, "_.txt");

    FILE *fp = NULL;

    fp = fopen(finalName, "a+");

    fprintf(fp, data);

    fclose(fp);
}

void DebugUtils::getCurrentTime(void)
{
    time_t timer;
    struct tm *timeInfo;

    time(&timer);
    timeInfo = localtime(&timer);

    strftime(currentTime, 10, "%H%M%S", timeInfo);
}
