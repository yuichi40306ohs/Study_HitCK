#include    "main.h"
#include    "Application.h"

int main(void)
{
#if defined(DEBUG) || defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif//defined(DEBUG) || defined(_DEBUG)

    // �A�v���P�[�V�������s
    Application app(SCREEN_WIDTH,SCREEN_HEIGHT);
    app.Run();

    return 0;
}