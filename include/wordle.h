//
// Created by Valentin on 08.04.2026.
//

#ifndef OOP_WORDLE_H
#define OOP_WORDLE_H

#include<resman.h>
#include <iostream>

class BaseEntity : public sf::Drawable {
public:
    ~BaseEntity() override = default;
    //virtual void update(float dt) = 0;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
};

enum TileStatus {
    EMPTY,
    WRONG,
    MISPLACED,
    CORRECT
};

class LetterTile : public BaseEntity {
private:

    sf::RectangleShape box;
    sf::Text text;
    TileStatus status;
    inline static sf::Vector2f defaultSize = sf::Vector2f(60.0f,60.0f);

public:


    //constructor
    LetterTile(sf::Vector2f pos, char letter=' ');
    LetterTile(const LetterTile& other);

    void setLetter(char c);
    void setStatus(TileStatus s);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    //static void setGlobalTileSize(float size);
};

class WordRow : public BaseEntity {
private:
    std::vector<LetterTile> tiles;
    std::string currentWord;
    const float padding = 10.0f;

public:

    explicit WordRow(float yPos);

    void addLetter(char c);
    void removeLetter();
    const std::string& getWord() const;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    //void update(float dt) override {}

    LetterTile& operator[](int idx);
    const LetterTile& operator[](int idx) const;


};




#endif //OOP_WORDLE_H


