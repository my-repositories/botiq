#ifndef BOTIQ_H
#define BOTIQ_H

#include <Windows.h>
#include <iostream>

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
        DWORD _mainPid, _botPid;
        std::string _processName;
        static HWND g_botHwnd;

        void HandleStateToggling();
        void HandleKeyboard();
        void RegisterKey(int key);
        static BOOL CALLBACK EnumWindowsProcMy(HWND hwnd, LPARAM lParam);
    };
}

#endif
