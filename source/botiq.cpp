#include <botiq.h>
#include <misc.h>
#include <proc.h>

namespace botiq
{
    HWND Botiq::g_botHwnd = 0;

    Botiq::Botiq()
        :
        _currentState(STATE_DEACTIVATED),
        _prevState(STATE_DEACTIVATED),
        _mainPid(0),
        _botPid(0)
    {}

    Botiq::~Botiq() {}

    void Botiq::Init(int argc, char** argv)
    {
        if (argc == 2)
        {
            _processName = argv[1];
        }
        else
        {
            std::cout << "process: ";
            std::getline(std::cin, _processName);
        }

        botiq::misc::Init(_processName);
    }

    void Botiq::MainLoop()
    {
        for (;; Sleep(50))
        {
            HandleStateToggling();
            if (_currentState == STATE_DISABLED)
            {
                Sleep(1000);
                continue;
            }

            _mainPid = botiq::proc::GetMainProcessId(_processName, _mainPid);
            _botPid = botiq::proc::GetBotProcessId(_processName, _mainPid);

            if (_mainPid != 0 && _botPid != 0)
            {
                _currentState = STATE_ACTIVATED;
            }

            if (_currentState != _prevState)
            {
                _prevState = _currentState;
                botiq::misc::Speech(_currentState == STATE_ACTIVATED ? botiq::misc::SOUND_ON : botiq::misc::SOUND_OFF);
                system("cls");
                std::cout << "mainPid: " << _mainPid << std::endl;
                std::cout << "botPid: " << _botPid << std::endl;
            }

            g_botHwnd = 0;
            EnumWindows(EnumWindowsProcMy, _botPid);

            if (_mainPid == 0 || _botPid == 0)
            {
                _currentState = STATE_DEACTIVATED;
                Sleep(1000);
                continue;
            }

            HandleKeyboard();
        }
    }

    void Botiq::HandleStateToggling()
    {
        if (GetAsyncKeyState(VK_F12) & 1)
        {
            _currentState = _currentState == STATE_DISABLED ? STATE_DEACTIVATED : STATE_DISABLED;
            botiq::misc::Speech(_currentState == STATE_DISABLED ? botiq::misc::SOUND_OFF : botiq::misc::SOUND_ON);
        }
    }

    void Botiq::HandleKeyboard()
    {
        for (int key = VK_F1; key < VK_F11; ++key)
        {
            RegisterKey(key);
        }

        RegisterKey(0x31); // 1
        RegisterKey(0x32); // 2
        RegisterKey(0x36); // 6
        RegisterKey(VK_SPACE); // SPACEBAR

        RegisterKey(0x46); // F
        RegisterKey(0x47); // G
        RegisterKey(0x51); // Q
        RegisterKey(0x52); // R
        RegisterKey(0x53); // S
        RegisterKey(0x56); // V
        RegisterKey(0x57); // W
        RegisterKey(0x58); // X
        RegisterKey(0x5A); // Z
    }

    void Botiq::RegisterKey(int key)
    {
        if (GetAsyncKeyState(key) & 1)
        {
            SendMessage(g_botHwnd, WM_KEYDOWN, key, NULL);
            Sleep(100);
            SendMessage(g_botHwnd, WM_KEYUP, key, NULL);
        }
    }

    BOOL CALLBACK Botiq::EnumWindowsProcMy(HWND hwnd, LPARAM lParam)
    {
        DWORD lpdwProcessId;
        GetWindowThreadProcessId(hwnd, &lpdwProcessId);
        if (lpdwProcessId == lParam)
        {
            g_botHwnd = hwnd;
            return FALSE;
        }
        return TRUE;
    }
}
