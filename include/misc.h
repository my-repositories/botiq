#ifndef MISC_H
#define MISC_H

#include <string>

namespace botiq
{
    namespace misc
    {
        enum Sound { SOUND_OFF, SOUND_ON };

        void Init(const std::string& title);
        void Speech(Sound type);
        LPSTR GetResource(int resourceId);
    }
}

#endif
