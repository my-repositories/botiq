#include <botiq.h>

int main(int argc, char** argv)
{
    botiq::Botiq bot;
    bot.Init(argc, argv);
    bot.MainLoop();

    return 0;
}
