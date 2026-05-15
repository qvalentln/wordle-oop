
#include<wordle.h>
#include<string>
#include "SFML/Graphics.hpp"


LetterTile::LetterTile(sf::Vector2f pos, char letter): status(TileStatus::EMPTY) {
	box.setSize(defaultSize);
	box.setPosition(pos);
	box.setOutlineThickness(1.0f);
	box.setOutlineColor(sf::Color::Black);
	try {
		text.setFont(FontManager::sharedInstance().font());
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		throw;
	}
	text.setCharacterSize(35);
	text.setFillColor(sf::Color::White);
	text.setString(letter);


}

LetterTile::LetterTile(const LetterTile &other): box(other.box), text(other.text), status(other.status) {
	//FontManager::sharedInstance().font();

	text.setFont(FontManager::sharedInstance().font());
	text.setFillColor(sf::Color::Black);
	//text.setFillColor(sf::Color(83,141,78));
}


///metode specifice LetterTile



void LetterTile::setLetter(char c) {
	if (c==' ')text.setString("");
	else text.setString(std::string(1,std::toupper(c)));

	//centrarea textului in patrat
	sf::FloatRect textRect=text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(box.getPosition().x + defaultSize.x / 2.0f, box.getPosition().y + defaultSize.y / 2.0f);
}

void LetterTile::setStatus(TileStatus s) {
	status=s;
	switch (status) {
		case TileStatus::WRONG:
			box.setFillColor(sf::Color(58,58,60));
			break;
		case TileStatus::MISPLACED:
			box.setFillColor(sf::Color(181,159,59));
			break;
		case TileStatus::CORRECT:
			box.setFillColor(sf::Color(83,141,78));
			break;
		default:
			box.setFillColor(sf::Color::Red);

	}
	box.setOutlineThickness(status == TileStatus::EMPTY ? 2.0f : 1.0f);
}

/*
void LetterTile::setGlobalTileSize(float size) {
	if (size > 0)
		defaultSize = sf::Vector2f(size,size);
}
*/
void LetterTile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(box,states);
	target.draw(text,states);

}





WordRow::WordRow(float yPos) {
	float tileSize = 60.f;
	float totalWidth = (5 * tileSize) + (4 * padding);
	float startX = (800.f - totalWidth) / 2.f;

	//cele 5 patratele de pe rand
	for (int i = 0; i < 5; ++i) {
		//float xPos = 150.0f + i * (60.0f + padding);
		tiles.emplace_back(LetterTile(sf::Vector2f(startX + i * (tileSize + padding), yPos)));
	}
}


///metode specifice WordRow
const std::string& WordRow::getWord() const {
	return currentWord;
}

void WordRow::removeLetter() {
	if (!currentWord.empty()) {
		tiles[currentWord.size()-1].setLetter(' ');
		currentWord.pop_back();
	}
}



void WordRow::addLetter(char c) {
	if (currentWord.size() < 5) {
		currentWord+=c;
		tiles[currentWord.size()-1].setLetter(c);
	}

}
void WordRow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (const auto& tile : tiles) {
		tile.draw(target,states);
	}
}


//operatori WordRow

LetterTile& WordRow::operator[](int idx) {
	return tiles[idx];
}
const LetterTile& WordRow::operator[](int idx) const {
	return tiles[idx];
}
