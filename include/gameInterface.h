
#ifndef OOP_GAMEINTERFACE_H
#define OOP_GAMEINTERFACE_H

#include <SFML/Graphics.hpp>
#include <wordle.h> // baseEntity
#include <types.h>
#include <gameEngine.h>



class Button {
private:
	sf::RectangleShape box;
	sf::Text text;
	int clickCount;
	bool isHovered;

public:
	Button(sf::Vector2f position, sf::Vector2f size, const std::string& label);

	void draw(sf::RenderWindow& window) const;
	void updateHover(sf::Vector2f mousePos);
	void updateLabel(const std::string& label);

	bool isClicked(sf::Vector2f mousePos);
	int getClickCount() const {return clickCount;};

};

class resultPopup : public BaseEntity {
private:
	sf::RectangleShape background;
	sf::Text title;
	sf::Text msg;
	std::vector<std::unique_ptr<Button> > buttons;
public:
	resultPopup(bool hasWon, const std::string& targetWord);

	void handleEvent(const sf::Event& event, sf::RenderWindow& window,
					appState& currentAppState, bool& shouldResetGame);
	void update(sf::RenderWindow& window);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};

class mainMenu {
private:
	sf::Text gameTitle;
	std::vector<std::unique_ptr<Button>> buttons;
	mainMenu();

public:
	mainMenu(const mainMenu&) = delete;
	mainMenu& operator=(const mainMenu&) = delete;

	static mainMenu& sharedInstance();

	void handleEvent(const sf::Event& event, sf::RenderWindow& window, appState& currentAppState);
	void update(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window) const;
};

#endif //OOP_GAMEINTERFACE_H