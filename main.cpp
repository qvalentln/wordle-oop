/// NOTE: this include is needed for environment-specific fixes     //
/// You can remove this include and the call from main              //
/// if you have tested on all environments, and it works without it //
#include <SFML/Graphics.hpp>



#include "env_fixes.h"
#include <gameEngine.h> // Core game logic
#include <resman.h>     // Resource manager things


#include <chrono>
#include <thread>




/*
class AlphabetStatus : public BaseEntity{
private:
    std::map<char, std::unique_ptr<LetterTile> > letterTiles;
public:
    AlphabetStatus(sf::Vector2f startPos) {
        float size = 25.f;
        float padding = 5.f;

        LetterTile::setGlobalTileSize(size);

        for (int i = 0; i < 26; ++i) {
            char c = 'A' + i;

            float x = startPos.x + (i%13) * (size + padding);
            float y = startPos.y + (i/13) * (size + padding);

            letterTiles[c] = std::make_unique<LetterTile>(sf::Vector2f(x,y),c);
        }
        LetterTile::setGlobalTileSize(60.f);
    }

    void updateLetter(char c, LetterTile::TileStatus newStatus)
};*/


int main() {
    /// NOTE: this function call is needed for environment-specific fixes //
    init_threads();
    //2.6.1
    //std::cout<<SFML_VERSION_MAJOR<<'.'<<SFML_VERSION_MINOR<<'.'<<SFML_VERSION_PATCH<<std::endl;


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


    sf::Text text;
    FontManager::sharedInstance();


    text.setFont(FontManager::sharedInstance().font());
    text.setString("Hello World!");
    text.setCharacterSize(34);
    text.setFillColor(sf::Color(83,141,78)); //moss green
    text.setStyle(sf::Text::Italic);

    text.setPosition(15, 15);


    /*
     * logica jocului
     */

    auto& window = renderEngine::getInstance().getWindow();


    auto& gameState = wordleEngine::getInstance();

    while(window.isOpen() && !gameState.isFinished()) {

        sf::Event event{};
        while(window.pollEvent(event)) {
            gameState.handleEvent(event);
        }

        if (wordleEngine::getInstance().isFinished()) {
            window.close();
        }

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(300ms);

        //draw logic
        window.clear(sf::Color::White);
        window.draw(text);

        gameState.renderState();

    }

   // for (auto row : gameGrid) {
   //     delete row;
    //}
    //gameGrid.clear();
    return 0;
}
