#include <types.h>
#include <gameEngine.h>
#include <gameInterface.h>

gameState::gameState() : AppState(), requestMenu(false) {}

std::unique_ptr<AppState> gameState::handleEvent(const sf::Event &event) {

	std::string engineSignal = wordleEngine::sharedInstance().handleEvent(event);



	if (engineSignal == "MENU" || requestMenu) {
		return std::make_unique<mainMenu>();
	}
	return nullptr;
}

void gameState::update(sf::RenderWindow &window) {
	wordleEngine::sharedInstance().updatePopup(window);
	//
}

void gameState::render(sf::RenderWindow &) {

	wordleEngine::sharedInstance().renderState();
}
