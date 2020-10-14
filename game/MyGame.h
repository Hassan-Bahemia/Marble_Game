#pragma once

class CMarbleGame : public CGame
{
public:
	CMarbleGame(void);
	~CMarbleGame(void);

	// Information about the Marble
	float xstart, ystart;	// Initial position of the marble
	float ax, ay;			// Acceleration of the marble
	float X;
	float Y;
	float f1;
	float f2;

	// Geometry of the Control Circle
	CVector MousePoint;		// center of the control circle
	int MouseRadius;		// size of the control circle

	// Sprites
	CSprite back;			// background
	CSprite pane;			// right side pane
	CSprite theArrow;

	CSprite theMarble;		// the marble
	CSprite theGoal;		// goal

	CSpriteList theWalls;	// the walls (a sprite list)
	CSpriteList theHoles;	// the holes (a sprite list)

	string strInfo1, strInfo2;


	// Per-Frame Callback Funtions (must be implemented!)
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);

	// Game Life Cycle
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnStartGame();
	virtual void OnStartLevel(Sint16 nLevel);
	virtual void OnGameOver();
	virtual void OnTerminate();

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse Events Handlers
	void ControlTilt(Uint16 x, Uint16 y);
	virtual void OnMouseMove(Uint16 x, Uint16 y, Sint16 relx, Sint16 rely, bool bLeft, bool bRight, bool bMiddle);
	virtual void OnLButtonDown(Uint16 x, Uint16 y);
	virtual void OnLButtonUp(Uint16 x, Uint16 y);
	virtual void OnRButtonDown(Uint16 x, Uint16 y);
	virtual void OnRButtonUp(Uint16 x, Uint16 y);
	virtual void OnMButtonDown(Uint16 x, Uint16 y);
	virtual void OnMButtonUp(Uint16 x, Uint16 y);
};
