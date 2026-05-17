#include <resman.h>
#include <filesystem>


/// Opps FontManager

std::ostream& operator<<(std::ostream& os, FontManager& self){
    os << self._font.loadFromFile(ResourceManager::getPathTo(ResourceType::FONT, "Roboto-Medium.ttf"));
return os;
}



/// metode ResourceManager
std::string ResourceManager::getPathTo(const ResourceType type, const std::string &resource) {
    // The logic here should be more complicated, when building with CLion the executable is located in a directory
    // named cmake-build-debug and that's how we get these paths
    auto path = std::filesystem::current_path().parent_path().append("resources");
    switch (type) {
        case FONT:
            path.append("fonts");
            break;
    }
    return path.append(resource).string();
}



/// Metode DisplayManager
/// tot jocul il gandesc pe un canvas 800x600

void DisplayManager::syncResolution(sf::RenderWindow& window, sf::View& gameView) {
    // Preiau rezolutia actuala
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    if (!window.isOpen()) {
        window.create(desktop, "Wordle", sf::Style::Fullscreen);
    }

    gameView.setSize(static_cast<float>(desktop.width),
        static_cast<float>(desktop.height));

    gameView.setCenter(static_cast<float>(desktop.width) / 2.f,
        static_cast<float>(desktop.height) / 2.f);

    window.setView(gameView);
    // Setez inca o data vsync ptr safety
    window.setVerticalSyncEnabled(true);

}

sf::Vector2f DisplayManager::getCenterPos(const sf::RenderWindow& window) {
    sf::Vector2u size = window.getSize();
    return {static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f};
}


sf::View DisplayManager::computeLetterbox(const sf::Vector2u& windowSize, float virtualWidth, float virtualHeight) {
    const float windowWidth = static_cast<float>(windowSize.x);
    const float windowHeight = static_cast<float>(windowSize.y);
    const float windowRatio = windowWidth / windowHeight;
    const float viewRatio = virtualWidth / virtualHeight;

    float sizeX = 1.f, sizeY = 1.f;
    float posX = 0.f, posY = 0.f;

    // Ecranul mai lat decat jocul
    if (windowRatio > viewRatio) {
        sizeX = viewRatio / windowRatio;
        posX = (1.f - sizeX) / 2.f;
    }

    // Ecranul mai inalt decat jocul
    else {
        sizeY = windowRatio / viewRatio;
        posY = (1.f - sizeY) / 2.f;
    }

    sf::View view(sf::FloatRect(0.f,0.f,virtualWidth,virtualHeight));
    view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
    return view;
}