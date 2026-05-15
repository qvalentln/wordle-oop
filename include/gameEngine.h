// This will be focused
// on the game mechanics

#ifndef OOP_GAMEENGINE_H
#define OOP_GAMEENGINE_H

#include <wordle.h>
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

	static T& getInstance() {
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

	renderEngine(); //This will initialize the game window

public:

	sf::RenderWindow& getWindow();
	sf::View& getGameView();

	void resize(const sf::Event& event);
	void close();
};



class wordleEngine : public Singleton<wordleEngine> {
	friend class Singleton;
private:
	int currentRowIdx;
	bool shouldExit;
	std::string targetWord;
	std::vector<std::unique_ptr<BaseEntity>> gameGrid;
	SecureDictionary dict;

	wordleEngine();

public:

	bool isFinished() const { return shouldExit; }

	void handleEvent(const sf::Event& event);
	void handleKeyPress(const sf::Event& event);
	static void validateRow(WordRow& row, const std::string& target);

	void renderState() const;

	friend std::ostream& operator<<(std::ostream& os, const wordleEngine& self);
	friend void wordleDebugInfo();


};

#endif //OOP_GAMEENGINE_H