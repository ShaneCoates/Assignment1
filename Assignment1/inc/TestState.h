/*
	Author: Shane Coates
	Description: Test Game State - Remove before release
*/

#ifndef _TEST_GAMESTATE_H_
#define	_TEST_GAMESTATE_H_

#include "GameStateManager.h"
struct GLFWwindow;
class FlyCamera;
class FollowCamera;
class ObjectOBJ;
class ParticleEmitter;
class DebugGUI;
class Fire;
class RenderTarget;
class Skybox;
class TestState : public IGameState
{
public:
	//Constructor inherits from IGameState and calls the Init function
	TestState(GLFWwindow* _window, GameStateManager* _gameStateManager) : IGameState()
	{
		Init(_window, _gameStateManager);
	}
	//Destructor
	~TestState();

	//Initialise the gamestate
	void Init(GLFWwindow* _window, GameStateManager* _gameStateManager);

	//Update/Draw functions
	void Update(double _dt);
	void Draw();

private:

	GLFWwindow* m_window;
	GameStateManager* m_gameStateManager;

	FlyCamera* m_camera;
	double m_timeLeft;

	DebugGUI* m_debugGUI;

	Fire* m_fire;
};

#endif