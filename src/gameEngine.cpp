#include <gameEngine.h>


// This will initialize the game window
// Tine de randare




renderEngine::renderEngine () : desktop(sf::VideoMode::getDesktopMode()) {

	//DisplayManager::syncResolution(window, gameView);
	window.create(desktop, "miniWordle", sf::Style::Fullscreen);

	/// This is needed so we do not burn the GPU
	window.setVerticalSyncEnabled(true);

	gameView = DisplayManager::computeLetterbox(window.getSize(), vWidth, vHeight);
	window.setView(gameView);


}

sf::RenderWindow & renderEngine::getWindow() {
	return window;
}

sf::View & renderEngine::getGameView() {
	return gameView;
}

void renderEngine::resize(const sf::Event& event) {
	gameView = DisplayManager::computeLetterbox(
						sf::Vector2u(event.size.width,event.size.height), vWidth, vHeight);
	window.setView(gameView);
}

void renderEngine::close() {
	window.close();
}

// Wordle Engine



std::ostream& operator<<(std::ostream &os, const wordleEngine &self) {

	os << self.currentRowIdx << std::endl;
	return os;
}

void wordleDebugInfo() {
	std::cout << "Debug info:" << std::endl;
	std::cout << wordleEngine::getInstance().currentRowIdx << std::endl;
}


wordleEngine::wordleEngine() : currentRowIdx(0), shouldExit(false), targetWord("") {
	try {
		dict.loadFromBinary("resources/words.bin");
		targetWord = dict.getRandomWord();
		std::cout << "CUVANT: " << targetWord << std::endl;

	}
	catch (const std::exception&) {
		// Cumva nu am putut lua cuvant din dict
		shouldExit = true;
	}
	for (int i = 0; i < 6; ++i) {
		float yPos = 100.f + i * 75.f;
		gameGrid.push_back(std::make_unique<WordRow>(yPos));
	}

}



// Tine de logica jocului



void wordleEngine::handleKeyPress(const sf::Event &event) {

	if (currentRowIdx >= static_cast<int>(gameGrid.size())){
		shouldExit = true;
		return;
	}

	WordRow* currentRow = dynamic_cast<WordRow*>(gameGrid[currentRowIdx].get());
	if (event.key.code == sf::Keyboard::Backspace) {
		currentRow->removeLetter();
	}
	else if (event.key.code == sf::Keyboard::Enter) {
		// Adauga logica de verificare si colorare tiles

		std::string input = currentRow->getWord();
		std::cout << "Cuvant introdus: " << currentRow->getWord() << std::endl;
		wordleDebugInfo();
		if (input.size() == 5) {
			validateRow(*currentRow, targetWord);
			if (input == targetWord) {
				shouldExit = true;
				std::cout << "ai castigat!" << std::endl;

			}
			else currentRowIdx++;

			// Print some debug info




		}
	}
	else if (event.key.code == sf::Keyboard::Escape) {
		shouldExit = true;
	}
}

void wordleEngine::handleEvent(const sf::Event& event) {
	auto& window = renderEngine::getInstance().getWindow();

	switch (event.type) {
		case sf::Event::Closed:
			window.close();
			shouldExit = true;
			break;

		case sf::Event::Resized:
			renderEngine::getInstance().resize(event);
			break;

		case sf::Event::TextEntered: {

			if (currentRowIdx >= static_cast<int>(gameGrid.size())){
				shouldExit = true;
				std::cout<<"ai pierdut! :(" << std::endl;
			}
			else {
				WordRow* currentRow = dynamic_cast<WordRow*>(gameGrid[currentRowIdx].get());

				if (event.text.unicode < 128) {
					char c = static_cast<char>(event.text.unicode);
					if (std::isalpha(c)) {
						currentRow->addLetter(c);
					}
				}
			}
			break;
		}

		case sf::Event::KeyPressed: {

			handleKeyPress(event);
			break;
		}
		default:
			break;
	}
}


void wordleEngine::validateRow(WordRow &row, const std::string &target) {
	std::string guess = row.getWord();
	std::string upperTarget = target;
	for (auto &c : upperTarget) {
		c = std::toupper(c);
	}

	for (int i=0;i<5;++i) {
		char currentLetter = std::toupper(guess[i]);
		if (currentLetter == upperTarget[i]) {
			row[i].setStatus(TileStatus::CORRECT);
			//am consumat litera
			//upperTarget[i] = '*';
		}
	}

	for (int i=0;i<5;++i) {
		char currentLetter = std::toupper(guess[i]);
		if (currentLetter == std::toupper(target[i])) {
			continue;
		}
		auto it = std::find(upperTarget.begin(), upperTarget.end(), currentLetter);
		if (it != upperTarget.end()) {
			row[i].setStatus(TileStatus::MISPLACED);
			*it = '*';
		}
		else {
			row[i].setStatus(TileStatus::WRONG);
		}

	}
}


void wordleEngine::renderState() const {
	auto& window = renderEngine::getInstance().getWindow();

	for (auto& row : gameGrid) {
		window.draw(*row);
	}
	window.setView(renderEngine::getInstance().getGameView());
	window.display();

}

