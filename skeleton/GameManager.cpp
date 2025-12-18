#include "GameManager.h"

GameManager::GameManager(physx::PxScene* s) 
{
	std::random_device dev;
	seed = std::mt19937(dev());
	gen = std::uniform_int_distribution<std::mt19937::result_type>(0, 9);
	p_ship = new physics::PlayerShip(s, player_config);
	scene = s;
	resetGame();
	walls = std::vector<physics::Wall*>();
	physics::phys_particle_config aux = wall_config;
	for (auto p : wallPos) {
		aux.position = p.first;
		walls.push_back(new physics::Wall(s, aux, p.second));
	}
};

GameManager::~GameManager()
{
	delete p_ship;
	for (auto w : walls) delete w;
	deleteTargets();
}

void GameManager::deleteTargets()
{
	auto it = e_ships.begin();
	while (it != e_ships.end()) {
		delete* it;
		it = e_ships.erase(it);
	}
}

void GameManager::resetGame()
{
	setState(MENU);
	p_ship->setOpacity(0);

	round = 0;
	lives = maxLives;
	currentLevel = -1;
}

void GameManager::readLevel(int level)
{
	positions = std::vector<custom::Vector3>();
	std::string filename = "../layouts/level" + std::to_string(level) + ".txt";
	std::ifstream levelfile(filename);
	enemyCount = 0;
	int i = 0;
	char temp;
	levelfile >> temp;
	while (levelfile) {
		switch (temp) {
		case'o':
			positions.push_back(custom::Vector3(zOffset, xyOffset * (i % 3 - 1), xyOffset * (i / 3 - 1)));
			enemyCount++;
		case 'x':
		default:
			break;
		}
		levelfile >> temp;
		i++;
	}

}

void GameManager::startRound()
{
	p_ship->setOpacity(0);
	remainingTimeout = levelTimeout;
	setState(INTRO);
}

void GameManager::startLevel()
{
	p_ship->reset();
	int newlevel = -1;
	while (newlevel == -1 || newlevel == currentLevel) newlevel = gen(seed);
	currentLevel = newlevel;
	readLevel(currentLevel);
	int i = 0;
	for (custom::Vector3 pos : positions) {
		physics::phys_particle_config aux = enemy_config;
		aux.position = pos;
		e_ships.push_back(new physics::EnemyShip(scene, aux,i));
		i++;
	}
	currentTimeLimit = max(baseTimeLimit - (0.5 * round), 2.0);
	//p_ship->setOpacity(1.0);
	setState(GAME);
}

void GameManager::endLevel(bool win)
{
	deleteTargets();
	p_ship->reset();
	p_ship->setOpacity(0.0);
	if (win)
	{
		round++;
	}
	else {
		lives--;
	}
	if (lives <= 0) gameOver();
	else startRound();
}

void GameManager::gameOver()
{
	setState(GAMEOVER);
}

void GameManager::setState(State s)
{
	gameState = s;
	std::cout << gameState << "\n";
}

void GameManager::keyPressed(char c)
{
	switch (gameState) {
	case MENU:
		switch (c) {
		case 'Z':
			startRound();
			break;
		default: break;
		}
		break;
	case INTRO:
		break;
	case GAME:
		switch (c) {
		case 'I':
			p_ship->set_accel(physics::UP);
			break;
		case 'J':
			p_ship->set_accel(physics::LEFT);
			break;
		case 'K':
			p_ship->set_accel(physics::DOWN);
			break;
		case 'L':
			p_ship->set_accel(physics::RIGHT);
			break;
		case 'Z':
			p_ship->fire();
			break;
		default: break;
		}
		break;
	case GAMEOVER:
		switch (c) {
		case 'Z':
			resetGame();
			break;
		default: break;
		}
		break;
	}
}

void GameManager::killShipAt(int i)
{
	int j = 0;
	for (auto s : e_ships) {
		if (i == j && s->die()) enemyDown();
		j++;
	}
}

void GameManager::enemyDown()
{
	enemyCount--;
	if (enemyCount <= 0) endLevel(true);
}

void GameManager::step(double dt)
{
	//std::cout << gameState << "\n";
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	switch (gameState) {
	case MENU:
		drawText("RONDA", glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2 - 50);
		break;
	case INTRO:
		drawText("RONDA" + std::to_string(round), glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2 - 50);
		drawText("VIDAS:" + std::to_string(lives), glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
		drawText("Empieza en:" + std::to_string(int(remainingTimeout)), glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2 + 50);
		remainingTimeout -= dt;
		if (remainingTimeout <= 0.0) startLevel();
		break;
	case GAME:
		currentTimeLimit -= dt;
		if (currentTimeLimit <= 0.0) endLevel(false);
		p_ship->step(dt);
		for (auto s : e_ships) s->step(dt);
		break;
	case GAMEOVER:
		break;
	}

}
