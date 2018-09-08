#ifndef PROC_H
#define PROC_H

#include <Windows.h>
#include <iostream>

namespace botiq
{
    namespace proc
    {
        DWORD GetMainProcessId(const std::string& processName, DWORD prevPid);
        DWORD GetMainProcessIdByName(const std::string& processName, DWORD prevPid);
        DWORD GetBotProcessId(const std::string& processName, DWORD mainProcessId);
        DWORD ReadProcessIdFromInput();
    }
}

#endif
