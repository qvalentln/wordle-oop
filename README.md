# miniWordle

<h4> Implementare offline a Wordle.</h4>

<h3>Descriere</h3>
Varianta implementata a jocului permite 3 nivele de dificultate (*Normal* - 6 incercari, *Hard* - 3 incercari, *IMPOSIBIL* - o singura incercare). 

Observa faptul ca *IMPOSIBIL* chiar inseamna *IMPOSIBIL* - poti bate nivelul decat daca ai prea mult "bulan" sau daca esti cu un pas in fata algoritmului de randomizare.

De asemenea, alfabetul (starea literelor - POZ CORECTA, POZ GRESITA, GRESIT, care este cel englez) nu apare in aceasta dificultate.

<h3>Safeguards</h3>
Ca safeguards, ne am asigurat de urmatoarele lucruri:

- _dictionarul_ (fisierul care contine cuvintele) este criptat folosind o cheie XOR, hardcodata in fisierul jocului.
- La schimbarea rezolutiei (manual, din setari, in caz ca cineva chiar se gandeste la asta), elementele grafice isi pastreaza structura (nu vor aparea probleme stretch/compress). 
- In acest sens, toata grafica jocului este gandita si conceputa pe un letterbox de dimensiune 800x600. Oricum, alte rezolutii mai mici (ex. 640x480) duc in cazuri extreme ce depasesc scopul acestui proiect. 
- In gameplay, in timpul jocului, fiecare cuvant dat este trecut prin dictionar. Daca nu apare in dictionar, consideram cuvantul "invalid". 

Acest lucru face imposibil inputurile de tipul HHHHH care ar putea da hint-uri "aiurea" despre starea literei in cuvant.

<h3>Interactiunea</h3>
Dificultatea jocului se poate alege din meniu. La finalul jocului, vei primi un popup de tipul AI CASTIGAT / AI PIERDUT, cu posibilitatea de a da restart (cu aceeasi dificultate) sau iesire in menu (eventual, schimband dificultatea).

<h3> What's next? </h3>
Chiar daca proiectul nu isi va atinge obiectivele sale maxime, in continuare apar lucruri interesante de implementat:

- adaugarea unui log (cu toate cuvintele ghicite - dificultatea in care a fost dat, precum si nr de incercari pentru a-l ghici)
- adaugarea unui Dark/Light Theme toggler.
- extinderea dictionarului (+ cuvinte din limba romana? Integrare LLM? - in sensul de generare de cuvinte, insa ar putea face dificila verificarea inputului)


gameplay demo - diff imposibil
<img src="https://i.ibb.co/vxvsqc2P/2026-05-2321-41-37online-video-cutter-com-ezgif-com-optimize.gif" alt="impossible gameplay"></img>



## Cerințe obligatorii 

Nerespectarea duce la nepunctarea proiectului

- programul va fi scris în C++
- programul va avea un meniu interactiv (doar pentru ilustrarea funcționalității)
- programul nu are erori de compilare
- fară variabile globale
- datele membre private(sau protected)
- GitHub Actions trecute
- commit-uri pe Git adecvate si punctuale
- folosirea a funcționalităților limbajului fără sens
- folosirea a funcționlităților limbajului cu scopul de a încălca "legal" o altă regulă
    - folosirea excesivă a claselor friend
    - folosirea excesviă a elementelor statice  
- fiecare cerinta va fi punctate 1/2 pe implementare + 1/2 pe faptul ca a fost implementata cu sens


## Cerințe
- [X] definirea a minim **2-3 ieararhii de clase** care sa interactioneze in cadrul temei alese (fie prin compunere, agregare sau doar sa apeleze metodele celeilalte intr-un mod logic) (6p)
  - minim o clasa cu:
    - [X] constructori de inițializare
    - [X] constructor supraîncărcat
    - [X] constructori de copiere
    - [X] `operator=` de copiere
    - [X] destructor
    - [X] `operator<<` pentru afișare (std::ostream)
    - [X] `operator>>` pentru citire (std::istream)
    - [X] alt operator supraîncărcat ca funcție membră
    - [X] alt operator supraîncărcat ca funcție non-membră
  - in derivate
      - [X] implementarea funcționalităților alese prin [upcast](https://github.com/Ionnier/poo/tree/main/labs/L04#solu%C8%9Bie-func%C8%9Bii-virtuale-late-binding) și [downcast](https://github.com/Ionnier/poo/tree/main/labs/L04#smarter-downcast-dynamic-cast)
        - aceasta va fi făcută prin **2-3** metode specifice temei alese
        - funcțiile pentru citire / afișare sau destructorul nu sunt incluse deși o să trebuiască să le implementați 
      - [X] apelarea constructorului din clasa de bază din [constructori din derivate](https://github.com/Ionnier/poo/tree/main/labs/L04#comportamentul-constructorului-la-derivare)
      - [X] suprascris [cc](https://github.com/Ionnier/poo/tree/main/labs/L04#comportamentul-constructorului-de-copiere-la-derivare)/op= pentru copieri/atribuiri corecte
      - [X] destructor [virtual](https://github.com/Ionnier/poo/tree/main/labs/L04#solu%C8%9Bie-func%C8%9Bii-virtuale-late-binding)
  - pentru celelalte clase se va definii doar ce e nevoie
  - minim o ierarhie mai dezvoltata (cu 2-3 clase dintr-o clasa de baza)
  - ierarhie de clasa se considera si daca exista doar o clasa de bază însă care nu moștenește dintr-o clasă din altă ierarhie
- [X] cât mai multe `const` [(0.25p)](https://github.com/Ionnier/poo/tree/main/labs/L04#reminder-const-everywhere)
- [X] funcții și atribute `static` (în clase) [0.5p](https://github.com/Ionnier/poo/tree/main/labs/L04#static)
  - [X] 1+ atribute statice non-triviale 
  - [X] 1+ funcții statice non-triviale
- [X] excepții [0.5p](https://github.com/Ionnier/poo/tree/main/labs/L04#exception-handling)
  - porniți de la `std::exception`
  - ilustrați propagarea excepțiilor
  - ilustrati upcasting-ul în blocurile catch
  - minim folosit într-un loc în care tratarea erorilor în modurile clasice este mai dificilă
- [X] folosirea unei clase abstracte [(0.25p)](https://github.com/Ionnier/poo/tree/main/labs/L04#clase-abstracte)
 - [X] clase template
   - [X] crearea unei clase template [(1p)](https://github.com/Ionnier/poo/tree/main/labs/L08)
   - [X] 2 instanțieri ale acestei clase (0.5p)
 - STL [(0.25p)](https://github.com/Ionnier/poo/tree/main/labs/L07#stl)
   - [X] utilizarea a două structuri (containere) diferite (vector, list sau orice alt container care e mai mult sau mai putin un array)
   - [X] utilizarea a unui algoritm cu funcție lambda (de exemplu, sort, transform)
 - Design Patterns [(0.75p)](https://github.com/Ionnier/poo/tree/main/labs/L08)
   - [X] utilizarea a două șabloane de proiectare

### Observații

* Pot exista depunctări până la 2p pentru diferite aspecte precum:
  - memory leak-uri
  - nefolosirea destructorului virtual la nevoie
  - abuzarea de diferite concepte (toate funcțiile declarate virtual)
  - apelarea de funcții virtual în constructori

* În general, acestea sunt prezente în [CppCoreGuideline](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md), dar nu e nevoie să parcurgeți documentul, doar să scrieți codul suficient de organizat

## Instrucțiuni de compilare

Proiectul este configurat cu CMake.

Instrucțiuni pentru terminal:

0. Biblioteci necesare pe Linux (presupunem sistem de operare bazat pe Debian)
```sh
sudo apt-get update && \
  sudo apt-get install libxrandr-dev \
    libxcursor-dev \
    libudev-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libdrm-dev \
    libgbm-dev \
    libfreetype6-dev
```

Dacă lipsesc și alte biblioteci, ștergeți folder-ul de build de la pasul 1 și reconfigurați proiectul după ce ați instalat ce lipsea.

1. Pasul de configurare
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
```

Sau pe Windows cu GCC:
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
```

La acest pas putem cere să generăm fișiere de proiect pentru diverse medii de lucru.


2. Pasul de compilare
```sh
cmake --build build --config Debug --parallel 6
```

Cu opțiunea `parallel` specificăm numărul de fișiere compilate în paralel.

3. Pasul de instalare (opțional)
```sh
cmake --install build --config Debug --prefix install_dir
```

Vezi și [`scripts/cmake.sh`](scripts/cmake.sh).

## Resurse

- [SFML](https://github.com/SFML/SFML/tree/2.6.1) (Zlib)
  - [OpenAL](https://openal-soft.org/) (LGPL): din cauza licenței, trebuie distribuită ca shared library
- adăugați trimiteri către resursele externe care v-au ajutat sau pe care le-ați folosit
