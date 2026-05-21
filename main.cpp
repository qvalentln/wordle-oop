// SFML 2.6.1
#include <SFML/Graphics.hpp> // face urat daca nu il bag primul

#include "env_fixes.h"
#include <gameEngine.h> // Core game logic
#include <gameInterface.h>

#include <chrono>
#include <thread>

int main() {
    /// NOTE: this function call is needed for environment-specific fixes //
    init_threads();

    auto& renderState = renderEngine::sharedInstance();
    auto& window = renderEngine::sharedInstance().getWindow();

    std::unique_ptr<AppState> currentState = std::make_unique<mainMenu>();
    while(window.isOpen() && !renderState.shouldExit()) {

        sf::Event event{};
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // safeguard: ce se intampla daca schimb rezolutia
            // din nvidia control panel, de ex?
            if (event.type == sf::Event::Resized) {
                renderEngine::sharedInstance().resize(event);
            }
            if (currentState) {
                auto nextState = currentState->handleEvent(event);
                if (nextState) {
                    currentState = std::move(nextState);
                }
            }
        }
        if (currentState) {
            currentState->update(window);
        }
        if (renderState.shouldExit()) {
            window.close();
        }

        // "input lag"
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(30ms);

        if (currentState) {
            currentState->render(window);
        }
    }

    // Cumva am apasat butonul EXIT
    if (window.isOpen()) {
        window.close();
    }
    return 0;
}
