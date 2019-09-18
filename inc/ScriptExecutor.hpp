#ifndef SCRIPT_EXECUTOR
# define SCRIPT_EXECUTOR

# include "lib.h"

class ScriptExecutor
{
public:
    static void    sendSetting(std::string pass, std::string login, std::string ip);

    static void     sendSelfInfo(std::string serrial_number, std::string ip);

    static void     execute(int count, ...);

	static std::string 	PathToScript;

    class getOutput
    {
    public:
        static std::string  checkWAN();
        static std::string  execute(int count, ...);
    };
};

#endif
