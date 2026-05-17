
#ifndef OOP_RESMAN_H
#define OOP_RESMAN_H

#include <map>
#include <string>
#include <SFML/Graphics.hpp>

enum ResourceType {
    FONT
};

class ResourceManager {
private:
    std::map<std::string,sf::Font> fonts;

    ResourceManager() {}
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

public:
    static std::string getPathTo(const ResourceType type, const std::string &resource);

};

class FontManager {
private:
    sf::Font _font;
    FontManager() {
        _font.loadFromFile(ResourceManager::getPathTo(ResourceType::FONT, "Roboto-Medium.ttf"));
    }
public:
    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;


    [[nodiscard]] const sf::Font& font() const {
        return _font;
    }


    static FontManager& sharedInstance() {
        static FontManager instance;
        return instance;
    }

    friend std::ostream& operator<<(std::ostream& os, FontManager& self);
};

class DisplayManager {
public:
    static void syncResolution(sf::RenderWindow& window, sf::View& gameView);
    static sf::Vector2f getCenterPos(const sf::RenderWindow& window);
    static sf::View computeLetterbox(const sf::Vector2u& windowSize, float virtualWidth, float virtualHeight);

};





#endif //OOP_RESMAN_H