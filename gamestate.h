#pragma once

enum class GameState {
    MENU,
	PAUSED,
	COUNTDOWN,
    PLAYING
};

class GameStateManager {
private:
    GameState currentState;

public:
    GameStateManager() : currentState(GameState::MENU) {} 
    
    void setState(GameState newState) { currentState = newState; }
    GameState getState() const { return currentState; }
    bool isState(GameState state) const { return currentState == state; }
};