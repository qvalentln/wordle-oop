
#include<wordle.h>
#include<string>
#include "SFML/Graphics.hpp"


LetterTile::LetterTile(const sf::Vector2f& pos, const sf::Color& color, float size, float thickness, char letter):
status(TileStatus::EMPTY), borderThickness(thickness) {

	const sf::Vector2f boxSize = sf::Vector2f(size,size);

	box.setSize(boxSize);
	box.setPosition(pos);
	box.setOutlineThickness(thickness);
	box.setOutlineColor(sf::Color::Black);
	box.setFillColor(color);

	try {
		text.setFont(FontManager::sharedInstance().font());
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		throw;
	}

	text.setCharacterSize(static_cast<unsigned int>(boxSize.x));
	text.setFillColor(sf::Color::Black);
	text.setString(letter);
	setLetter(letter);
}

LetterTile::LetterTile(const LetterTile &other): BaseEntity(other), box(other.box), text(other.text), status(other.status), borderThickness(other.borderThickness) {

	text.setFont(FontManager::sharedInstance().font());
	text.setFillColor(sf::Color::Black);
	//text.setFillColor(sf::Color(83,141,78));
}

LetterTile & LetterTile::operator=(const LetterTile &other) {
	if (this != &other) {
		this->box = other.box;
		this->text = other.text;
		this->status = other.status;
		this->borderThickness = other.borderThickness;

		this->text.setFont(FontManager::sharedInstance().font());

	}
	return *this;
}


///metode specifice LetterTile



void LetterTile::setLetter(char c) {
	if (c==' ')text.setString("");
	else text.setString(std::string(1,std::toupper(c)));

	//centrarea textului in patrat
	const sf::FloatRect textRect=text.getLocalBounds();
	const sf::Vector2f currentBoxSize = box.getSize();

	text.setCharacterSize(static_cast<unsigned int>(currentBoxSize.x));
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

	const sf::Vector2f currentBoxPos = box.getPosition();
	text.setPosition(currentBoxPos.x + currentBoxSize.x / 2.0f, currentBoxPos.y + currentBoxSize.y / 2.0f);
}

void LetterTile::setStatus(const TileStatus& s) {
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
	box.setOutlineThickness(borderThickness);
}





void LetterTile::draw(sf::RenderTarget& target, const sf::RenderStates states) const {
	target.draw(box,states);
	target.draw(text,states);

}


WordRow::WordRow(float yPos) {
	const float tileSize = 45.f;
	const float totalWidth = (5 * tileSize) + (4 * padding);
	const float startX = (800.f - totalWidth) / 2.f;

	//cele 5 patratele de pe rand
	for (int i = 0; i < 5; ++i) {
		//float xPos = 150.0f + i * (60.0f + padding);
		LetterTile tile = LetterTile(sf::Vector2f(startX + i * (tileSize + padding), yPos),
										sf::Color::Transparent, 45.f, 1.f);

		tiles.push_back(tile);
	}
}

WordRow::WordRow(const WordRow &other) : BaseEntity(other), tiles(other.tiles), currentWord(other.currentWord) {
	//
}

WordRow & WordRow::operator=(const WordRow &other) {

	if (this != &other) {
		this->currentWord = other.currentWord;
		this->tiles = other.tiles;
	}
	return *this;
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


AlphabetStatus::AlphabetStatus(const sf::Vector2f& startPos) {
	const float size = 35.f;
	const float padding = 5.f;

	for (int i = 0; i < 26; ++i) {
		char c = 'A' + i;

		float x = startPos.x + (i%13) * (size + padding);
		float y = startPos.y + (i/13) * (size + padding);

		letterTiles[c] = std::make_unique<LetterTile>(sf::Vector2f(x,y), sf::Color(60,60,60),size, 0.f, c);

		auto* tile = dynamic_cast<LetterTile*>(letterTiles[c].get());
		if (tile) {
			tile->setLetter(c);
		}
	}
}

void AlphabetStatus::updateLetter(char c, const TileStatus& newStatus) {


	auto it = letterTiles.find(c);
	if (it != letterTiles.end()) {
		auto* tile = dynamic_cast<LetterTile*>(it->second.get());
		if (tile) {
			TileStatus currentStatus = tile->getStatus();
			// nu dau overwrite la o stare mai buna
			if (newStatus > currentStatus) {
				tile->setStatus(newStatus);
			}
		}
	}
}
void AlphabetStatus::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (auto const& [chr, tilePtr] : letterTiles) {
		target.draw(*tilePtr, states);
	}
}

bool operator>(const LetterTile& tile1, const LetterTile& tile2) {
	// dau prioritati culorilor

	auto getPriority = [](TileStatus status) {
		if (status == TileStatus::CORRECT) return 3;
		else if (status == TileStatus::MISPLACED)  return 2;
		else if (status == TileStatus::WRONG) return 1;
		else return 0;
	};
	return getPriority(tile1.getStatus()) > getPriority(tile2.getStatus());

}
