// This will be focused
// on the game mechanics

#ifndef OOP_GAMEENGINE_H
#define OOP_GAMEENGINE_H

#include <wordle.h>
#include <types.h>
#include <gameInterface.h>
#include <decrypt.h>
#include <memory>




template<typename T>

class Singleton {
protected:
	Singleton() = default;
	virtual ~Singleton() = default;
public:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	static T& sharedInstance() {
		static T instance;
		return instance;
	}
};


class renderEngine : public Singleton<renderEngine> {
	friend class Singleton;
private:
	sf::RenderWindow window;
	sf::VideoMode desktop;
	sf::View gameView;
	static constexpr float vWidth = 800.f;
	static constexpr float vHeight = 600.f;
	bool keepOpen;

	renderEngine(); //This will initialize the game window

public:

	sf::RenderWindow& getWindow();
	sf::View& getGameView();
	void signalExit() {keepOpen = false;}
	bool shouldExit() const {return !keepOpen;}

	void resize(const sf::Event& event);

};



class wordleEngine : public Singleton<wordleEngine> {
	friend class Singleton;
private:
	int currentRowIdx;
	int rowCount;
	bool showAlphabet;
	bool isGameOver;
	std::string targetWord;
	std::unique_ptr<BaseEntity> endPopup;
	std::vector<std::unique_ptr<BaseEntity>> gameGrid;
	std::unique_ptr<AlphabetStatus> alphabet;
	SecureDictionary dict;

	wordleEngine();

public:
	bool isFinished() const { return isGameOver; }

	void initGame(gameDifficulty &difficulty);
	void handleEvent(const sf::Event& event, appState& gameState);
	void handleKeyPress(const sf::Event& event);
	static void validateRow(WordRow& row, const std::string& target);
	gameDifficulty getGameDifficulty() const;
	void renderState() const;

	friend std::ostream& operator<<(std::ostream& os, const wordleEngine& self);
	friend void wordleDebugInfo();


};

#endif //OOP_GAMEENGINE_H