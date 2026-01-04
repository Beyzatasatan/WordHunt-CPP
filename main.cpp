#include <iostream>
#include "Game.h"

using namespace std;

int main() {
    while (true) {
        cout << "\n=== WORD HUNT GAME ===\n";
        cout << "1- Yeni Oyun\n";
        cout << "2- Cikis\n";
        cout << "Secim: ";

        int secim;
        cin >> secim;

        if (secim == 2) break;

        int difficulty;
        cout << "Zorluk Sec (Kolay=8, Orta=6, Zor=4): ";
        cin >> difficulty;

        Game game(difficulty);
        game.chooseCategory();
        game.play();
    }

    return 0;
}
