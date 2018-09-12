#ifndef BOTIQ_H
#define BOTIQ_H

#include <Windows.h>
#include <iostream>
#include <vector>

namespace botiq
{
    enum State { STATE_DEACTIVATED, STATE_ACTIVATED, STATE_DISABLED };

    class Botiq
    {
    public:
        Botiq();
        ~Botiq();
        void Init(int argc, char** argv);
        void MainLoop();

    private:
        State _currentState, _prevState;
        std::string _processName;
        static const size_t g_windowNameSize = 64;
        static DWORD g_mainPid;
        static size_t g_gameWindowsCount;
        static char g_windowName[g_windowNameSize];
        static std::vector<HWND> g_botHwnds;

        void HandleStateToggling();
        void HandleKeyboard();
        void RegisterKey(int key);
        static BOOL CALLBACK EnumWindowsProcMy(HWND hwnd, LPARAM lParam);
    };
}

#endif
