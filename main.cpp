// SFML 2.6.1
#include <SFML/Graphics.hpp> // face urat daca nu il bag primul

#include "env_fixes.h"
#include <gameEngine.h> // Core game logic

#include <chrono>
#include <thread>

int main() {
    /// NOTE: this function call is needed for environment-specific fixes //
    init_threads();

    /////////////////////////////////////////////////////////////////////////
    /// Observație: dacă aveți nevoie să citiți date de intrare de la tastatură,
    /// dați exemple de date de intrare folosind fișierul tastatura.txt
    /// Trebuie să aveți în fișierul tastatura.txt suficiente date de intrare
    /// (în formatul impus de voi) astfel încât execuția programului să se încheie.
    /// De asemenea, trebuie să adăugați în acest fișier date de intrare
    /// pentru cât mai multe ramuri de execuție.
    /// Dorim să facem acest lucru pentru a automatiza testarea codului, fără să
    /// mai pierdem timp de fiecare dată să introducem de la zero aceleași date de intrare.
    ///
    /// Pe GitHub Actions (bife), fișierul tastatura.txt este folosit
    /// pentru a simula date introduse de la tastatură.
    /// Bifele verifică dacă programul are erori de compilare, erori de memorie și memory leaks.
    ///
    /// Dacă nu puneți în tastatura.txt suficiente date de intrare, îmi rezerv dreptul să vă
    /// testez codul cu ce date de intrare am chef și să nu pun notă dacă găsesc vreun bug.
    /// Impun această cerință ca să învățați să faceți un demo și să arătați părțile din
    /// program care merg (și să le evitați pe cele care nu merg).
    ///
    /////////////////////////////////////////////////////////////////////////

    /*
     * logica jocului
     */
    auto& renderState = renderEngine::sharedInstance();
    auto& window = renderEngine::sharedInstance().getWindow();
    auto& gameState = wordleEngine::sharedInstance();

    auto& menu= mainMenu::sharedInstance();

    appState state = appState::MENU;


    while(window.isOpen() && state != appState::EXIT && !renderState.shouldExit()) {

        sf::Event event{};
        while(window.pollEvent(event)) {

            if (state == appState::MENU) {
                menu.handleEvent(event,window,state);
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }
            else if (state == appState::GAME) {
                gameState.handleEvent(event, state);
            }
        }

        if (state == appState::MENU) {
            menu.update(window);
        }

        if (renderEngine::sharedInstance().shouldExit()) {
            window.close();
        }

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(300ms);

        //draw logic
        window.clear(sf::Color::White);

        if (state == appState::MENU) {
            window.setView(renderEngine::sharedInstance().getGameView());
            menu.draw(window);
            window.display();
        }
        else if (state == appState::GAME) {
            gameState.renderState();
        }
    }

    // Cumva am apasat butonul EXIT
    if (window.isOpen()) {
        window.close();
    }
    return 0;
}
