#include "Game.h"

QString Game::root = "F:/Train Simulator";
QString Game::route = "bbb";
//QString Game::route = "traska";
//QString Game::route = "cmk";
int Game::allowObjLag = 1000;
int Game::maxObjLag = 10;
float Game::objectLod = 3000;

Game::Game() {
}

Game::Game(const Game& orig) {
}

Game::~Game() {
}

