#include <gameInterface.h>
#include <resman.h>


// resultPopup

resultPopup::resultPopup(bool hasWon, const std::string& targetWord) {
	// BG things

	background.setSize(sf::Vector2f(400.f, 200.f));
	background.setFillColor(sf::Color(30,30,30,240));
	background.setOutlineThickness(2.f);
	background.setOutlineColor(hasWon ? sf::Color::Green : sf::Color::Red);
	background.setPosition((800.f - 400.f) / 2.f, (600.f - 200.f) / 2.f);

	// title things
	title.setFont(FontManager::sharedInstance().font());
	title.setString(hasWon ? "AI CASTIGAT!!" : "AI PIERDUT!!");
	title.setCharacterSize(30);
	title.setFillColor(hasWon ? sf::Color::Green : sf::Color::Red);

	// center the title
	sf::FloatRect titleRect = title.getGlobalBounds();
	title.setOrigin(titleRect.left + titleRect.width / 2.f, titleRect.top + titleRect.height / 2.f);
	title.setPosition(400.f, 240.f);

	// set the mssg beneath the title

	msg.setFont(FontManager::sharedInstance().font());
	msg.setString(hasWon ? "felicitari!! " : "cuvantul era " + targetWord);
	msg.setCharacterSize(18);
	msg.setFillColor(sf::Color::White);

	sf::FloatRect msgRect = msg.getGlobalBounds();
	msg.setOrigin(msgRect.left + msgRect.width / 2.f, msgRect.top + msgRect.height / 2.f);
	msg.setPosition(400.f, 280.f);


	buttons.push_back(std::make_unique<Button>(sf::Vector2f(210.f, 320.f), sf::Vector2f(160.f, 45.f), "RESTART"));
	buttons.push_back(std::make_unique<Button>(sf::Vector2f(430.f, 320.f), sf::Vector2f(160.f, 45.f), "EXIT"));



}

void resultPopup::handleEvent(const sf::Event &event, sf::RenderWindow &window, appState &currentAppState,
	bool &shouldResetGame) {

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

		if (buttons[0]->isClicked(mousePos)) {
			shouldResetGame = true;
		}
		else if (buttons[1]->isClicked(mousePos)) {
			currentAppState = appState::MENU;
		}
	}

}

void resultPopup::update(sf::RenderWindow &window) {
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

	for (auto& btn : buttons) {
		btn->updateHover(mousePos);
	}

}

void resultPopup::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(background,states);
	target.draw(title,states);
	target.draw(msg,states);

	// draw cere sf::Drawable

	auto* window = dynamic_cast<sf::RenderWindow*>(&target);
	if (window) {
		for (auto& btn : buttons) {
			btn->draw(*window);
		}
	}

}

// Button

Button::Button(sf::Vector2f position, sf::Vector2f size, const std::string& label) : clickCount(0), isHovered(false) {

	box.setPosition(position);
	box.setSize(size);
	box.setFillColor(sf::Color(40,40,40));
	box.setOutlineThickness(2.f);
	box.setOutlineColor(sf::Color(100,100,100));

	text.setFont(FontManager::sharedInstance().font());
	text.setString(label);
	text.setFillColor(sf::Color::White);

	sf::FloatRect textRect = text.getGlobalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);

	// cat poate ocupa textul din box
	float maxWidth = size.x * 0.85f;
	float maxHeight = size.y * 0.85f;

	float scaleX = 1.f;
	float scaleY = 1.f;

	if (textRect.width > maxWidth) {
		scaleX = maxWidth / textRect.width;
	}
	if (textRect.height > maxHeight) {
		scaleY = maxHeight / textRect.height;
	}

	float scale = std::min(scaleX, scaleY);

	if (scale > 1.f) {
		scale = 1.f;
	}
	text.setScale(scale,scale);
	text.setPosition(position.x + size.x / 2.f, position.y + size.y / 2.f);
}


void Button::updateHover(sf::Vector2f mousePos) {
	if (box.getGlobalBounds().contains(mousePos)) {
		isHovered = true;
		box.setFillColor(sf::Color(60,60,60));
		box.setOutlineColor(sf::Color::Green);
	}
	else {
		isHovered = false;
		box.setFillColor(sf::Color(40,40,40));
		box.setOutlineColor(sf::Color(100,100,100));
	}
}

void Button::updateLabel(const std::string &label) {
	// updateLabel duh
	text.setString(label);

	// rescale bazat pe box
	text.setScale(1.f, 1.f);

	// recentrez textul
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);

	sf::Vector2f boxSize = box.getSize();

	// cat poate ocupa textul din box
	float maxWidth = boxSize.x * 0.85f;
	float maxHeight = boxSize.y * 0.85f;

	float scaleX = 1.f;
	float scaleY = 1.f;

	if (textRect.width > maxWidth) {
		scaleX = maxWidth / textRect.width;
	}
	if (textRect.height > maxHeight) {
		scaleY = maxHeight / textRect.height;
	}

	float scale = std::min(scaleX, scaleY);

	if (scale > 1.f) {
		scale = 1.f;
	}
	text.setScale(scale,scale);

	sf::Vector2f boxPos = box.getPosition();
	text.setPosition(boxPos.x + boxSize.x / 2.f, boxPos.y + boxSize.y / 2.f);

}

bool Button::isClicked(sf::Vector2f mousePos){
	if (box.getGlobalBounds().contains(mousePos)) {
		clickCount++;
		return true;
	}
	else return false;
}

void Button::draw(sf::RenderWindow& window) const {
	window.draw(box);
	window.draw(text);

}

// mainMenu

mainMenu::mainMenu() {

	gameTitle.setFont(FontManager::sharedInstance().font());
	gameTitle.setString("miniWordle");
	gameTitle.setCharacterSize(60);
	gameTitle.setFillColor(sf::Color::Magenta);
	gameTitle.setStyle(sf::Text::Bold);

	sf::FloatRect titleRect = gameTitle.getLocalBounds();
	gameTitle.setOrigin(titleRect.left + titleRect.width / 2.f, titleRect.top + titleRect.height / 2.f);
	gameTitle.setPosition(400.f, 100.f);

	buttons.push_back(std::make_unique<Button>(sf::Vector2f(300.f,250.f), sf::Vector2f(200.f, 50.f), "PLAY"));
	buttons.push_back(std::make_unique<Button>(sf::Vector2f(300.f,330.f), sf::Vector2f(200.f,50.f), "DIFFICULTY : NORMAL"));
	buttons.push_back(std::make_unique<Button>(sf::Vector2f(300.f,410.f), sf::Vector2f(200.f,50.f), "EXIT"));

}



// hover logic
void mainMenu::update(sf::RenderWindow& window) {
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
	for (auto &bttn : buttons) {
		bttn->updateHover(mousePos);
	}
}

mainMenu & mainMenu::sharedInstance() {
	static mainMenu menu;
	return menu;

}



void mainMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window, appState& currentAppState) {
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);


		if (buttons[0]->isClicked(mousePos)) {
			// start game
			int count = buttons[1]->getClickCount();
			gameDifficulty difficulty;
			if (count % 3 == 0) {
				difficulty = gameDifficulty::NORMAL;
			}
			else if (count % 3 == 1) {
				difficulty = gameDifficulty::HARD;
			}
			if (count % 3 == 2) {
				difficulty = gameDifficulty::IMPOSSIBLE;
			}
			wordleEngine::sharedInstance().initGame(difficulty);


			currentAppState = appState::GAME;
		}
		else if (buttons[1]->isClicked(mousePos)) {
			int count = buttons[1]->getClickCount();
			if (count % 3 == 0) {
				buttons[1]->updateLabel("DIFFICULTY: NORMAL");
			}
			else if (count % 3 == 1) {
				buttons[1]->updateLabel("DIFFICULTY: HARD");
			}
			if (count % 3 == 2) {
				buttons[1]->updateLabel("DIFFICULTY: IMPOSSIBLE");
			}
		}
		else if (buttons[2]->isClicked(mousePos)) {
			// iesi
			currentAppState = appState::EXIT;
		}
	}
}

void mainMenu::draw(sf::RenderWindow &window) const {
	window.draw(gameTitle);
	for (auto& bttn : buttons) {
		bttn->draw(window);
	}

}

