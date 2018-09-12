#define _CRT_SECURE_NO_WARNINGS

#include <botiq.h>
#include <misc.h>
#include <proc.h>

namespace botiq
{
    DWORD Botiq::g_mainPid;
    size_t Botiq::g_gameWindowsCount;
    char Botiq::g_windowName[g_windowNameSize];
    std::vector<HWND> Botiq::g_botHwnds;

    Botiq::Botiq()
        :
        _currentState(STATE_DEACTIVATED),
        _prevState(STATE_DEACTIVATED)
    {}

    Botiq::~Botiq() {}

    void Botiq::Init(int argc, char** argv)
    {
        if (argc == 4)
        {
            _processName = argv[1];
            strcpy(g_windowName, argv[2]);
            g_gameWindowsCount = atoi(argv[3]);
        }
        else
        {
            std::cout << "process: ";
            std::getline(std::cin, _processName);

            std::cout << "window name: ";
            std::cin >> g_windowName;

            std::cout << "game windows count: ";
            std::cin >> g_gameWindowsCount;
        }

        botiq::misc::Init(_processName);
    }

    void Botiq::MainLoop()
    {
        g_mainPid = botiq::proc::GetMainProcessId(_processName, g_mainPid);

        for (size_t iteration = 0;; Sleep(50))
        {
            HandleStateToggling();
            if (_currentState == STATE_DISABLED)
            {
                Sleep(1000);
                continue;
            }

            if (g_mainPid != 0 && g_botHwnds.size() == g_gameWindowsCount)
            {
                _currentState = STATE_ACTIVATED;
            }

            if (_currentState != _prevState)
            {
                _prevState = _currentState;
                botiq::misc::Speech(_currentState == STATE_ACTIVATED ? botiq::misc::SOUND_ON : botiq::misc::SOUND_OFF);
                system("cls");
                std::cout << "mainPid: " << g_mainPid << std::endl;
                std::cout << std::endl;
            }

            if (iteration == 0)
            {
                g_botHwnds.clear();
                EnumWindows(EnumWindowsProcMy, 0);
            }

            if (g_mainPid == 0 || g_botHwnds.size() != g_gameWindowsCount)
            {
                _currentState = STATE_DEACTIVATED;
                Sleep(1000);
                continue;
            }

            HandleKeyboard();

            if (iteration++ == 200)
            {
                iteration = 0;
            }
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

        for (int key = VK_NUMPAD0; key < VK_NUMPAD9; ++key)
        {
            RegisterKey(key);
        }

        RegisterKey(0x31); // 1
        RegisterKey(0x32); // 2
        RegisterKey(0x36); // 6

        RegisterKey(0x46); // F
        RegisterKey(0x47); // G
        RegisterKey(0x51); // Q
        RegisterKey(0x52); // R
        RegisterKey(0x53); // S
        RegisterKey(0x56); // V
        RegisterKey(0x57); // W
        RegisterKey(0x58); // X
        RegisterKey(0x5A); // Z

        RegisterKey(VK_SPACE); // SPACEBAR
    }

    void Botiq::RegisterKey(int key)
    {
        if (GetAsyncKeyState(key) & 1)
        {
            for (HWND hwnd : g_botHwnds)
            {
                SendMessage(hwnd, WM_KEYDOWN, key, NULL);
                Sleep(100);
                SendMessage(hwnd, WM_KEYUP, key, NULL);
            }
        }
    }

    BOOL CALLBACK Botiq::EnumWindowsProcMy(HWND hwnd, LPARAM lParam)
    {
        DWORD lpdwProcessId;
        GetWindowThreadProcessId(hwnd, &lpdwProcessId);

        if (g_mainPid == lpdwProcessId)
        {
            (void)lParam;
            return TRUE;
        }

        char windowName[g_windowNameSize];
        GetWindowTextA(hwnd, windowName, g_windowNameSize);

        if (!strcmp(g_windowName, windowName))
        {
            g_botHwnds.push_back(hwnd);
            return g_botHwnds.size() != g_gameWindowsCount;
        }
        return TRUE;
    }
}
