//
// Created by Valentin on 08.04.2026.
//

#ifndef OOP_WORDLE_H
#define OOP_WORDLE_H


#include <resman.h>
#include <types.h>
#include <memory>
#include <iostream>



class BaseEntity : public sf::Drawable {
public:
    virtual ~BaseEntity() = default;
    //virtual void update(float dt) = 0;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const  = 0;
};


class LetterTile : public BaseEntity {
private:

    sf::RectangleShape box;
    sf::Text text;
    TileStatus status;
    inline static sf::Vector2f defaultSize = sf::Vector2f(45.0f,45.0f);
    float borderThickness;

public:


    //constructor
    LetterTile(sf::Vector2f pos, sf::Color color, float size, float thickness, char letter=' ');
    LetterTile(const LetterTile& other);

    void setLetter(const char& c);
    void setStatus(const TileStatus s);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};

class WordRow : public BaseEntity {
private:
    std::vector<LetterTile> tiles;
    std::string currentWord;
    const float padding = 5.0f;

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


class AlphabetStatus : public BaseEntity{
private:
    std::map<char, std::unique_ptr<BaseEntity> > letterTiles;
public:
    AlphabetStatus(sf::Vector2f startPos);

    void updateLetter(char c, TileStatus newStatus);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};




#endif //OOP_WORDLE_H


