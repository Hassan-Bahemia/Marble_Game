#include "stdafx.h"
#include "MyGame.h"

CMarbleGame game;
CGameApp app;

int main(int argc, char* argv[])
{
	app.OpenWindow(800, 400, "MARBLE (based on HTC Teeter game)");
	//app.OpenFullScreen(800, 600, 24);
	app.SetClearColor(CColor::White());
	app.Run(&game);
	return(0);
}
