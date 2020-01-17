#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H

class DebugUtils
{
    public:

        DebugUtils();
        ~DebugUtils();
        void logCurrentOpcode(char *buffer);

    private:

        void getCurrentTime(void);
        char currentTime[10];
        bool firstExecution;
};
#endif // DEBUGUTILS_H
