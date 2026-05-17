

#ifndef OOP_TYPES_H
#define OOP_TYPES_H

#include <SFML/Graphics.hpp>
#include <memory>
// All the enums the .h files will use
enum gameDifficulty {
	NORMAL,
	HARD,
	IMPOSSIBLE
};

enum appState {
	MENU,
	GAME,
	EXIT
};

enum TileStatus {
	EMPTY,
	WRONG,
	MISPLACED,
	CORRECT
};


// State pattern
class AppState {
public:
	virtual ~AppState() = default;

	virtual std::unique_ptr<AppState> handleEvent(const sf::Event& event) = 0;

	virtual void update(sf::RenderWindow& window) = 0;
	virtual void render(sf::RenderWindow& window) = 0;
};


class gameState : public AppState {
private:
	bool requestMenu = false;

public:
	gameState() = default;

	std::unique_ptr<AppState> handleEvent(const sf::Event& event);

	void update(sf::RenderWindow& window);
	void render(sf::RenderWindow& window);
};


#endif //OOP_TYPES_H