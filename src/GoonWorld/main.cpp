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
    geBgm *bgm = geBgmNew("loading", 0.0f, 0.0f);
    geBgm *bgm2 = geBgmNew("loading", 0.0f, 0.0f);
    geLoadAllContent();
    geBgmPlay(bgm, 1.0, -1);

    gePlayLoop();

    geBgmDelete(bgm);

    geUnloadAllContent();

    return 0;
}