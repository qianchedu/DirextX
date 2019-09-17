#pragma once



#define WINDOW_CLASS "StrandedGame"
#define WINDOW_TITLE "Stranded"
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define FULLSCREEN 0


bool InitializeEngine();
void ShutdownEngine();

bool GameInitialize();
void GameLoop();
void GameShutdown();


