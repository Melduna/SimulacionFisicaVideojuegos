#pragma once
#include "PhysicsShip.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
enum State {
	MENU,
	INTRO,
	GAME,
	GAMEOVER
};
static std::vector<std::pair<custom::Vector3,bool>> wallPos = {
	{custom::Vector3(-400,300,0),false},
	{custom::Vector3(-400,-300,0),false},
	{custom::Vector3(-400,0,300),true},
	{custom::Vector3(-400,0,-300),true}
};
class GameManager {
public:
	GameManager(physx::PxScene* s);
	~GameManager();
	void resetGame();
	void readLevel(int level);
	void startRound();
	void startLevel();
	void endLevel(bool win);
	void gameOver();
	void setState(State s);
	void keyPressed(char c);
	void killEnemy();
	
	void step(double dt);
private:
	physx::PxScene* scene = nullptr;
	physics::PlayerShip* p_ship = nullptr;
	std::vector<custom::Vector3> positions;
	std::list<physics::EnemyShip*> e_ships;
	std::vector<physics::Wall*> walls;

	double zOffset = -700.0;
	double xyOffset = 200.0;

	const double baseTimeLimit = 6.0;
	const double levelTimeout = 3.0;
	const int maxLives = 4;

	int round = 0;
	double remainingTimeout;
	double currentTimeLimit;
	int lives;
	int currentLevel;
	int enemyCount;

	State gameState;

	std::uniform_int_distribution<std::mt19937::result_type> gen;
	std::mt19937 seed;
	
	
	physics::phys_particle_config player_config = physics::phys_particle_config(custom::Vector3(-400,0,0), custom::Vector3::blank(), 1.0, 20, {1,1,1,1});
	physics::phys_particle_config enemy_config = physics::phys_particle_config(custom::Vector3::blank(), custom::Vector3::blank(), 0.01, 80, { 1,0,0,1 });
	physics::phys_particle_config wall_config = physics::phys_particle_config(custom::Vector3(-400, 0, 0), custom::Vector3::blank(), 1.0, 1000, { 0,0,0,1 });
};