#include <Windows.h>
#include <TlHelp32.h>

#include <proc.h>

namespace botiq
{
    namespace proc
    {
        DWORD GetMainProcessId(const std::string& processName, DWORD prevPid)
        {
            DWORD pid = GetMainProcessIdByName(processName, prevPid);

            return pid ? pid : ReadProcessIdFromInput();
        }

        DWORD ReadProcessIdFromInput()
        {
            DWORD pid;
            std::cout << "Enter process id: ";
            std::cin >> pid;
            return pid;
        }

        DWORD GetMainProcessIdByName(const std::string& processName, DWORD prevPid)
        {
            DWORD pid = 0;
            int countProcesses = 0;
            HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            PROCESSENTRY32 process;
            ZeroMemory(&process, sizeof(process));
            process.dwSize = sizeof(process);

            if (Process32First(snapshot, &process))
            {
                do
                {
                    if (countProcesses > 1 && prevPid == 0)
                    {
                        pid = 0;
                        break;
                    }

                    if (processName == static_cast<std::string>(process.szExeFile))
                    {
                        pid = process.th32ProcessID;
                        if (prevPid != 0)
                        {
                            break;
                        }
                        ++countProcesses;
                        continue;
                    }
                } while (Process32Next(snapshot, &process));
            }

            CloseHandle(snapshot);
            return pid;
        }

        DWORD GetBotProcessId(const std::string& processName, DWORD mainProcessId)
        {
            DWORD pid = 0;
            HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            PROCESSENTRY32 process;
            ZeroMemory(&process, sizeof(process));
            process.dwSize = sizeof(process);

            if (Process32First(snapshot, &process))
            {
                do
                {
                    if (processName == static_cast<std::string>(process.szExeFile) && process.th32ProcessID != mainProcessId)
                    {
                        pid = process.th32ProcessID;
                        break;
                    }
                } while (Process32Next(snapshot, &process));
            }

            CloseHandle(snapshot);
            return pid;
        }
    }
}
