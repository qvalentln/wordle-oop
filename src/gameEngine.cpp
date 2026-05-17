#include <gameEngine.h>


// This will initialize the game window
// Tine de randare




renderEngine::renderEngine () : desktop(sf::VideoMode::getDesktopMode()), keepOpen(true) {

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



// Wordle Engine



std::ostream& operator<<(std::ostream &os, const wordleEngine &self) {

	os << self.currentRowIdx << std::endl;
	return os;
}

void wordleDebugInfo() {
	std::cout << "Debug info:" << std::endl;
	std::cout << wordleEngine::sharedInstance().currentRowIdx << std::endl;
}


wordleEngine::wordleEngine() : currentRowIdx(0), rowCount(6), showAlphabet(true), isGameOver(false), targetWord("MACOS")  {
	// dummy values, initGame does all the work
}

void wordleEngine::initGame(gameDifficulty &difficulty) {

	isGameOver = false;
	currentRowIdx = 0;
	endPopup.reset();

	switch (difficulty) {
		case gameDifficulty::NORMAL:
			this->rowCount = 6;
			this->showAlphabet = true;
			break;
		case gameDifficulty::HARD:
			this->rowCount = 3;
			this->showAlphabet = true;
			break;
		case gameDifficulty::IMPOSSIBLE:
			this->rowCount = 1;
			this->showAlphabet = false;
			break;
		default:
			break;
	}

	try {
		dict.loadFromBinary("resources/words.bin");
		this->targetWord = dict.getRandomWord();
		std::cout << "CUVANT: " << targetWord << std::endl;

	}
	catch (const std::exception&) {
		// Cumva nu am putut lua cuvant din dict
		isGameOver = true;
	}

	gameGrid.clear();

	float startY = 50.f;
	float padding = 55.f;

	float lastY = startY;
	for (int i = 0; i < rowCount; ++i) {
		lastY = startY + i * padding;
		gameGrid.push_back(std::make_unique<WordRow>(lastY));
	}
	if (showAlphabet) {
		constexpr float alphabetOffset = 80.f;
		// 800x600 virtual screen
		// num_litere * (size + padding)
		constexpr float alphabetWidth = 13 * (35.f + 5.f);
		constexpr float startX = (800.f - alphabetWidth) / 2.f;

		sf::Vector2f alphabetPos(startX, lastY + alphabetOffset);

		alphabet = std::make_unique<AlphabetStatus>(alphabetPos);
	}
	else {
		alphabet.reset();
	}


}



// Tine de logica jocului



void wordleEngine::handleKeyPress(const sf::Event &event) {

	if (currentRowIdx >= static_cast<int>(gameGrid.size())){
		isGameOver = true;
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

				isGameOver = true;
				endPopup = std::make_unique<resultPopup>(true, targetWord);
				std::cout << "ai castigat!" << std::endl;

			}
			else currentRowIdx++;

			// Print some debug info

		}
	}
	else if (event.key.code == sf::Keyboard::Escape) {
		renderEngine::sharedInstance().signalExit();
	}
}



void wordleEngine::handleEvent(const sf::Event& event, appState& gameState) {
	auto& window = renderEngine::sharedInstance().getWindow();
	if (isGameOver && endPopup) {
		bool shouldReset = false;
		auto* popup = dynamic_cast<resultPopup*>(endPopup.get());
		if (popup) {
			popup->handleEvent(event,window,gameState,shouldReset);
		}

		if (shouldReset) {
			gameDifficulty currDiff = getGameDifficulty();
			initGame(currDiff);
		}
		return;
	}

	if (isGameOver){
		if (event.type == sf::Event::Closed) {
			window.close();
			renderEngine::sharedInstance().signalExit();
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
			renderEngine::sharedInstance().signalExit();
		}
		return;
	}

	switch (event.type) {
		case sf::Event::Closed:
			window.close();
			renderEngine::sharedInstance().signalExit();
			break;

		case sf::Event::Resized:
			renderEngine::sharedInstance().resize(event);
			break;

		case sf::Event::TextEntered: {

			if (currentRowIdx >= static_cast<int>(gameGrid.size())){
				// trebuie sa inchid fereastra
				isGameOver = true;
				endPopup = std::make_unique<resultPopup>(false, targetWord);
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


// Update the board row,
// but ensure that the alphabetStatus
// is also kept uptodate
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

			if(sharedInstance().showAlphabet) {
				sharedInstance().alphabet->updateLetter(currentLetter, TileStatus::CORRECT);
			}
		}
		else {
			auto it = std::find(upperTarget.begin(), upperTarget.end(), currentLetter);
			if (it != upperTarget.end() && ) {
				row[i].setStatus(TileStatus::MISPLACED);

				if (sharedInstance().showAlphabet) {
					sharedInstance().alphabet->updateLetter(currentLetter, TileStatus::MISPLACED);
				}

				*it = '*';
			}
			else {
				row[i].setStatus(TileStatus::WRONG);

				if (sharedInstance().showAlphabet) {
					sharedInstance().alphabet->updateLetter(currentLetter, TileStatus::WRONG);
				}
			}
		}
	}


}

gameDifficulty wordleEngine::getGameDifficulty() const {
	if (this->rowCount == 6) return gameDifficulty::NORMAL;
	else if (this->rowCount == 3) return gameDifficulty::HARD;
	else return gameDifficulty::IMPOSSIBLE;
}


void wordleEngine::renderState() const {
	auto& window = renderEngine::sharedInstance().getWindow();
	window.clear(sf::Color::White);
	window.setView(renderEngine::sharedInstance().getGameView());

	for (auto& row : gameGrid) {
		window.draw(*row);
	}

	if (alphabet && showAlphabet) {
		window.draw(*alphabet);
	}
	if (isFinished() && endPopup) {

		auto* popup = dynamic_cast<resultPopup*>(endPopup.get());
		if (popup) {
			popup->update(renderEngine::sharedInstance().getWindow());
		}
		window.draw(*endPopup);
	}

	window.display();

}

