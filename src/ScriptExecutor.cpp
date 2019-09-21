#include "ScriptExecutor.hpp"

void    ScriptExecutor::sendSetting(
            std::string pass,
            std::string login,
            std::string ip)
{
    std::string     scriptPath = ScriptExecutor::PathToScript;
    std::string     script;

    scriptPath += "sendSetting.sh";
    script = scriptPath + " " + pass + " " + login + " " + ip;
    system(script.c_str());
}

void    ScriptExecutor::sendSelfInfo(
            std::string serrial_number,
            std::string ip)
{
    serrial_number = serrial_number;
    ip = ip;
}

void     ScriptExecutor::execute(std::string script) {
    system(script.c_str());
}

// with output

std::string ScriptExecutor::getOutput::checkWAN() {
    std::string     scriptPath = ScriptExecutor::PathToScript;
    std::string     line;
    char            buff[64];
    FILE            *pipe;

    scriptPath += "checkConnectInternet.sh";
    pipe = popen(scriptPath.c_str(), "r");
    while (fgets(buff, sizeof(buff), pipe))
        line += buff;
    pclose(pipe);
    return line;
}

std::string ScriptExecutor::getOutput::execute(std::string script) {
    std::string     line;
    char            buff[64];
    FILE            *pipe;

    pipe = popen(script.c_str(), "r");
    while (fgets(buff, sizeof(buff), pipe))
        line += buff;
    pclose(pipe);
    return line;
}

std::string 	ScriptExecutor::PathToScript = "/home/user/Public/anteniti_kcm_service/scripts/";
