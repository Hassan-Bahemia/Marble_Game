#include "stdafx.h"
#include "MyGame.h"

CMarbleGame::CMarbleGame(void)
	: back(CVector(400, 200), "back.bmp", 0),
	  pane(CVector(700, 200), "pane.bmp", CColor::LightGray(), 0),
	  theGoal(CVector(600, 600), "holegr.gif", 0),
	  theMarble(CVector(0, 0), "marble.gif", 0),
	  theArrow(CVector(700, 200), "arrow.bmp", CColor::LightGray(), 0)
{
	MousePoint = CVector(700, 200);
	MouseRadius = 80;
	theArrow.SetPivotLocal(0, 13);

	ax = 0;
	ay = 0;
	xstart = 0;
	ystart = 0;
}

CMarbleGame::~CMarbleGame(void) {}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMarbleGame::OnUpdate()
{
	// Some constant values:
	const float r = 12;		  // radius of the marble
	const float R = 14;		  // radius of the hole
	const float RR = 18;	  // radius of the goal hole
	const float gain = 3.0f;  // gains the acceleration of the marble
	const float damp = 0.6f;  // damp factor for bouncing

	// Time variables:
	Uint32 t = GetTime();		 // current time (in milliseconds)
	Uint32 dt = GetDeltaTime();  // time since the last frame (in milliseconds)

	// NOTE: Because dt is given in milliseconds, for the actual displacement vector due to velocity
	// use: GetVelocity() * dt / 1000

	// 1. TODO: apply acceleration to the marble
	// The acceleration is given as (gain * ax, gain * ay)

	theMarble.Accelerate(gain * ax, gain * ay);

	// 2. TODO: Iterate through all the walls (theWalls collection of sprites)
	// For each wall, check the collision of the marble with the upper, lower, right and left surface of the wall
	// In case of a collision, use reflection to change the velocity;
	// then use damp to slow down the speed after the bounce

	for (CSprite* pWalls : theWalls)
	{
		CVector v = theMarble.GetVelocity() * dt / 1000;
		CVector t = pWalls->GetPos() - theMarble.GetPos();
		/*CVector n = CVector(sin(alpha), cos(alpha)); 
		if (Dot(v, n) < 0) 
		{	// Perpendicular component (oncoming)
			float vy= Dot(v, n);
			// velocity component
			CVector d = t + (Y + R) * n;
			// distance vector between edges
			float dy = Dot(d, n);
			// perpendicular space between
			float f1 = dy / vy;
			// Parallel component (breadth control)
			float vx= Cross(v, n);
			// velocity component
			float tx= Cross(t, n);
			// distance between centres
			float f2 = (tx-vx* f1) / (X + R);
			if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
				theMarble.SetVelocity(Reflect(theMarble.GetVelocity(), n));
		}
		*/

		Y = pWalls->GetHeight() / 2;
		X = pWalls->GetWidth() / 2;

		
		//bottom
		if (v.m_y < 0)
		{
			f1 = (t.m_y + Y + R) / v.m_y;
			f2 = (t.m_x - v.m_x * f1) / (X + R);

			if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
			{
				theMarble.SetVelocity(Reflect(theMarble.GetVelocity() * damp, CVector(0, 1)));
			}
		}

		
		//top
		if (v.m_y > 0)
		{
			f1 = (t.m_y - Y - r) / v.m_y;
			f2 = (t.m_x - v.m_x * f1) / (X + R);

			if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
			{
				theMarble.SetVelocity(Reflect(theMarble.GetVelocity() * damp, CVector(0, 1)));
			}
		}

		
		//right
		if (v.m_x > 0)
		{
			f1 = (t.m_x - X - r) / v.m_x;
			f2 = (t.m_y + v.m_y * f1) / (Y + R);

			if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
			{
				theMarble.SetVelocity(Reflect(theMarble.GetVelocity() * damp, CVector(1, 0)));
			}
		}

		//left
		if (v.m_x < 0)
		{
			f1 = (t.m_x + X + r) / v.m_x;
			f2 = (t.m_y - v.m_y * f1) / (Y + R);

			if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
			{
				theMarble.SetVelocity(Reflect(theMarble.GetVelocity() * damp, CVector(1, 0)));
			}
		}
		
	}

	for (CSprite* Hole : theHoles)
	{
		if (Hole->GetPosition().Distance(theMarble.GetPosition()) < R)
		{
				theMarble.SetPosition(xstart, ystart);
				theMarble.SetVelocity(0, 0);
				theMarble.Accelerate(0, 0);
		}
	}

	// 3. TODO: update the Marble

	theMarble.Update(t);

	// 4. TODO: check collisions with the holes
	// If theMarble is in a hole:
	// - set its position to the initial one (xstart, ystart),
	// - set acceleration to 0
	// - set velocity to 0

	// check collision with the goal hole
	if (theGoal.GetPosition().Distance(theMarble.GetPosition()) < RR)
		// go to the higher level
		NewLevel();
}

void CMarbleGame::OnDraw(CGraphics* g)
{
	back.Draw(g);
	pane.Draw(g);
	for each(CSprite * pWall in theWalls) pWall->Draw(g);
	for each(CSprite * pHole in theHoles) pHole->Draw(g);
	theGoal.Draw(g);
	theMarble.Draw(g);
	theArrow.Draw(g);

	if (IsGameOver())
		*g << font(24) << endl << "  CONGRATULATIONS!" << endl << "  YOU WON!!!";
	else
		*g << font(11) << bottom << right << strInfo2 << endl << strInfo1;
}

/////////////////////////////////////////////////////
// Game Life Cycle

// one time initialisation
void CMarbleGame::OnInitialize() {}

// called at the start of a new game - display menu here
void CMarbleGame::OnDisplayMenu()
{
	StartGame();
}

// called when Game Mode entered
void CMarbleGame::OnStartGame() {}

void CMarbleGame::OnStartLevel(Sint16 nLevel)
{
	// destroy the old playfield
	for each(CSprite * pWall in theWalls) delete pWall;
	theWalls.clear();
	for each(CSprite * pHole in theHoles) delete pHole;
	theHoles.clear();

	// build the walls around
	theWalls.push_back(new CSprite(CRectangle(0, 0, 600, 10), "wallhorz.bmp", GetTime()));
	theWalls.push_back(new CSprite(CRectangle(0, 391, 600, 10), "wallhorz.bmp", GetTime()));
	theWalls.push_back(new CSprite(CRectangle(0, 0, 10, 400), "wallvert.bmp", GetTime()));
	theWalls.push_back(new CSprite(CRectangle(591, 0, 10, 400), "wallvert.bmp", GetTime()));

	switch (nLevel)
	{
		case 0:
			// Menu Mode only
			break;
		case 1:
			strInfo1 = "LEVEL 1";
			strInfo2 = "by HASSAN";

			// build the walls inside
			theWalls.push_back(new CSprite(CRectangle(0, 95, 500, 10), "wallhorz.bmp", GetTime()));
			theWalls.push_back(new CSprite(CRectangle(100, 195, 500, 10), "wallhorz.bmp", GetTime()));
			theWalls.push_back(new CSprite(CRectangle(0, 295, 500, 10), "wallhorz.bmp", GetTime()));

			// dig the holes
			theHoles.push_back(new CSprite(CVector(150, 328), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(300, 348), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(450, 368), "hole.gif", GetTime()));

			theHoles.push_back(new CSprite(CVector(150, 230), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(300, 250), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(450, 270), "hole.gif", GetTime()));

			theHoles.push_back(new CSprite(CVector(150, 130), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(300, 140), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(450, 170), "hole.gif", GetTime()));

			theHoles.push_back(new CSprite(CVector(570, 100), "hole.gif", GetTime()));

			theHoles.push_back(new CSprite(CVector(150, 33), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(300, 53), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(450, 73), "hole.gif", GetTime()));

			// set up the goal
			theGoal.SetPosition(30, 55);

			// set up the marble
			xstart = 55;
			ystart = 345;
			theMarble.SetPosition(xstart, ystart);
			theMarble.SetVelocity(0, 0);
			ax = ay = 0;

			break;

		case 2:
			strInfo1 = "LEVEL 2";
			strInfo2 = "by HASSAN";

			// build the walls inside
			theWalls.push_back(new CSprite(CRectangle(295, 80, 10, 240), "wallvert.bmp", GetTime()));
			theWalls.push_back(new CSprite(CRectangle(495, 150, 10, 100), "wallvert.bmp", GetTime()));

			// dig the holes
			theHoles.push_back(new CSprite(CVector(150, 30), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(150, 115), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(150, 200), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(150, 285), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(150, 370), "hole.gif", GetTime()));

			theHoles.push_back(new CSprite(CVector(300, 30), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(300, 370), "hole.gif", GetTime()));

			theHoles.push_back(new CSprite(CVector(470, 150), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(470, 250), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(420, 175), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(420, 225), "hole.gif", GetTime()));

			// set up the goal
			theGoal.SetPosition(470, 200);

			// set up the marble
			xstart = 55;
			ystart = 200;
			theMarble.SetPosition(xstart, ystart);
			theMarble.SetVelocity(0, 0);
			ax = ay = 0;

			break;

		case 3:
			strInfo1 = "LEVEL 3";
			strInfo2 = "BY HASSAN";

			//Horizontal Walls
			theWalls.push_back(new CSprite(CRectangle(0, 95, 500, 10), "wallhorz.bmp", GetTime()));
			theWalls.push_back(new CSprite(CRectangle(100, 195, 500, 10), "wallhorz.bmp", GetTime()));
			theWalls.push_back(new CSprite(CRectangle(0, 295, 500, 10), "wallhorz.bmp", GetTime()));


			//Vertical Walls
			//theWalls.push_back(new CSprite(CRectangle(295, 80, 10, 240), "wallvert.bmp", GetTime()));
			//theWalls.push_back(new CSprite(CRectangle(495, 150, 10, 100), "wallvert.bmp", GetTime()));

			// dig the holes
			theHoles.push_back(new CSprite(CVector(25, 30), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(25, 80), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(25, 120), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(25, 180), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(25, 220), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(25, 280), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(25, 380), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(25, 320), "hole.gif", GetTime()));

			theHoles.push_back(new CSprite(CVector(85, 30), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(85, 80), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(85, 120), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(85, 180), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(85, 220), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(85, 280), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(85, 380), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(85, 320), "hole.gif", GetTime()));

			theHoles.push_back(new CSprite(CVector(150, 30), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(150, 80), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(150, 120), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(150, 180), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(150, 220), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(150, 280), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(150, 380), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(150, 320), "hole.gif", GetTime()));

			theHoles.push_back(new CSprite(CVector(215, 30), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(215, 80), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(215, 120), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(215, 180), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(215, 220), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(215, 280), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(215, 380), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(215, 320), "hole.gif", GetTime()));

			theHoles.push_back(new CSprite(CVector(300, 30), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(300, 80), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(300, 120), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(300, 180), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(300, 220), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(300, 280), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(300, 380), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(300, 320), "hole.gif", GetTime()));

			theHoles.push_back(new CSprite(CVector(365, 30), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(365, 80), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(365, 120), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(365, 180), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(365, 220), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(365, 280), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(365, 380), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(365, 320), "hole.gif", GetTime()));

			theHoles.push_back(new CSprite(CVector(470, 30), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(470, 80), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(470, 120), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(470, 180), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(470, 220), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(470, 280), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(470, 380), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(470, 320), "hole.gif", GetTime()));

			theHoles.push_back(new CSprite(CVector(565, 30), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(565, 80), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(565, 120), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(565, 180), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(565, 220), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(565, 280), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(565, 380), "hole.gif", GetTime()));
			theHoles.push_back(new CSprite(CVector(565, 320), "hole.gif", GetTime()));


			// set up the goal
			theGoal.SetPosition(55, 55);

			// set up the marble
			xstart = 55;
			ystart = 345;
			theMarble.SetPosition(xstart, ystart);
			theMarble.SetVelocity(0, 0);
			ax = ay = 0;

			break;

		default:
			theGoal.SetPosition(30, 1345);
			GameOver();
			break;  // you won!
	}
}

// called when Game is Over
void CMarbleGame::OnGameOver() {}

// one time termination code
void CMarbleGame::OnTerminate() {}

/////////////////////////////////////////////////////
// Keyboard Event Handlers

void CMarbleGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	if (sym == SDLK_SPACE)
		PauseGame();
	if (sym == SDLK_F2)
		NewGame();
}

void CMarbleGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {}

/////////////////////////////////////////////////////
// Mouse Events Handlers

void CMarbleGame::ControlTilt(Uint16 x, Uint16 y)
{
	float dist = CVector(x, y).Distance(MousePoint);
	if (dist > MouseRadius)
		return;

	theArrow.SetPivotFromCenter(0, -13);
	theArrow.SetPosition(x, y);
	theArrow.SetRotation(x - MousePoint.GetX(), y - MousePoint.GetY());
	ax = (float)(x - MousePoint.GetX()) / MouseRadius;
	ay = (float)(y - MousePoint.GetY()) / MouseRadius;
}

void CMarbleGame::OnMouseMove(Uint16 x, Uint16 y, Sint16 relx, Sint16 rely, bool bLeft, bool bRight, bool bMiddle)
{
	ControlTilt(x, y);
}

void CMarbleGame::OnLButtonDown(Uint16 x, Uint16 y)
{
	ControlTilt(x, y);
}

void CMarbleGame::OnLButtonUp(Uint16 x, Uint16 y) {}

void CMarbleGame::OnRButtonDown(Uint16 x, Uint16 y) {}

void CMarbleGame::OnRButtonUp(Uint16 x, Uint16 y) {}

void CMarbleGame::OnMButtonDown(Uint16 x, Uint16 y) {}

void CMarbleGame::OnMButtonUp(Uint16 x, Uint16 y) {}
