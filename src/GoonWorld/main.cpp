#include <GoonEngine/debug.h>
#include <GoonEngine/game.h>
#include <GoonEngine/content/bgm.h>
#include <GoonEngine/content/content.h>
void Update(double deltatime)
{
}

void Draw()
{
}

int main()
{
    geInitializeEngine();
    geGameSetUpdateFunc(Update);
    geGameSetDrawFunc(Draw);
    geInitializeBgmContentType();
    geBgm *bgm = geBgmNew("1-2", 8.727f, 0.0f);
    geLoadAllContent();

    geBgmPlay(bgm, 1.0, -1);
    gePlayLoop();
    geUnloadAllContent();

    return 0;
}