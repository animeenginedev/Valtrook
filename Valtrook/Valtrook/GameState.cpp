#include "GameState.h"

#include "Game.h"
#include "RenderingEngine.h"

Val::GameState::GameState(Game * const game) : game(game) {
}

Val::GameState::~GameState() {
}

void Val::GameState::initialise() {
}

void Val::GameState::rawEvent(const SDL_Event & e) {
}

void Val::GameState::update(const TimingType & deltaTime) {
}

void Val::GameState::render(const TimingType & deltaTime, Val::RenderingEngine * const engine) {
}
