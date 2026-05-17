
#ifndef OOP_GAMEINTERFACE_H
#define OOP_GAMEINTERFACE_H

#include <SFML/Graphics.hpp>
#include <wordle.h> // baseEntity
#include <types.h>


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
	resultPopup(const resultPopup& other) = delete;
	resultPopup& operator=(const resultPopup& other) = delete;

	std::string handleEvent(const sf::Event& event, sf::RenderWindow& window);
	void update(sf::RenderWindow& window);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};

class mainMenu : public AppState {
private:
	sf::Text gameTitle;
	std::vector<std::unique_ptr<Button>> buttons;

public:

	mainMenu();

	mainMenu(const mainMenu& other) = delete;
	mainMenu& operator=(const mainMenu& other) = delete;

	std::unique_ptr<AppState> handleEvent(const sf::Event& event);
	void update(sf::RenderWindow& window);
	void render(sf::RenderWindow& window);
};

#endif //OOP_GAMEINTERFACE_H