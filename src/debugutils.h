#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H

class Logger
{
    public:

        Logger();
        virtual ~Logger();
        void logCurrentOpcode(char *buffer);

    private:

        void getCurrentTime(void);
        char currentTime[10];
};
#endif // DEBUGUTILS_H
