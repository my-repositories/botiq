#include <Windows.h>

#include <misc.h>
#include <resource.h>

#pragma comment(lib,"Winmm.lib")

namespace botiq
{
    namespace misc
    {
        LPSTR sound[2];

        void Init(const std::string& title)
        {
            SetConsoleTitle(("botiq(" + title + ")").c_str());
            system("color a");

            sound[0] = GetResource(SND_OFF);
            sound[1] = GetResource(SND_ON);
        }

        void Speech(Sound type)
        {
            sndPlaySound(sound[type], SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
        }

        LPSTR GetResource(int resourceId)
        {
            HMODULE hInst = GetModuleHandle(NULL);
            if (!hInst)
            {
                return FALSE;
            }
            LPSTR lpRes;
            HANDLE hResInfo, hRes;

            // Find the WAVE resource. 

            hResInfo = FindResource(hInst, MAKEINTRESOURCE(resourceId), "WAVE");
            if (hResInfo == NULL)
                return FALSE;

            // Load the WAVE resource. 

            hRes = LoadResource(hInst, (HRSRC)hResInfo);
            if (hRes == NULL)
                return FALSE;

            // Lock the WAVE resource and play it. 

            lpRes = (LPSTR)LockResource(hRes);
            if (lpRes != NULL)
            {
                UnlockResource(hRes);
            }

            // Free the WAVE resource and return success or failure. 

            FreeResource(hRes);
            return lpRes;
        }
    }
}
