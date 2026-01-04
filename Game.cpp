#include "Game.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cstdlib>

Game::Game(int difficulty) {
    maxLives = difficulty;
    lives = difficulty;
    score = 0;
    srand(time(0));
    loadHighScore();
}

string Game::toLowerStr(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

void Game::loadHighScore() {
    ifstream file("highscore.txt");
    file >> highScore;
    if (!file) highScore = 0;
    file.close();
}

void Game::saveHighScore() {
    if (score > highScore) {
        ofstream file("highscore.txt");
        file << score;
        file.close();
    }
}

void Game::chooseCategory() {
    string category;
    cout << "Kategori sec (hayvanlar / sehirler / meyveler): ";
    cin >> category;
    category = toLowerStr(category);

    ifstream file("words.txt");
    if (!file.is_open()) {
        cout << "words.txt dosyasi bulunamadi!\n";
        exit(1);
    }

    vector<string> words;
    string line;
    bool read = false;

    while (getline(file, line)) {
        line = toLowerStr(line);

        if (line == category) {
            read = true;
            continue;
        }

        if (read) {
            if (line.empty()) break;
            words.push_back(line);
        }
    }

    file.close();

    // 🔥 KRİTİK KORUMA
    if (words.empty()) {
        cout << "Bu kategori icin kelime bulunamadi!\n";
        exit(1);
    }

    word = words[rand() % words.size()];
    display = string(word.length(), '_');
}

void Game::drawHangman() {
    static string stages[] = {
        " +---+\n     |\n     |\n     |\n     |\n=======\n",
        " +---+\n |   |\n O   |\n     |\n     |\n=======\n",
        " +---+\n |   |\n O   |\n |   |\n     |\n=======\n",
        " +---+\n |   |\n O   |\n/|   |\n     |\n=======\n",
        " +---+\n |   |\n O   |\n/|\\  |\n     |\n=======\n",
        " +---+\n |   |\n O   |\n/|\\  |\n/    |\n=======\n",
        " +---+\n |   |\n O   |\n/|\\  |\n/ \\  |\n=======\n"
    };

    int index = maxLives - lives;
    if (index > 6) index = 6;
    cout << "\nADAM ASMACA:\n" << stages[index] << endl;
}

bool Game::isWordGuessed() {
    return display == word;
}

void Game::giveHint() {
    vector<int> hidden;
    for (int i = 0; i < word.length(); i++)
        if (display[i] == '_')
            hidden.push_back(i);

    if (hidden.empty()) {
        cout << "Acilacak harf kalmadi!\n";
        return;
    }

    int idx = hidden[rand() % hidden.size()];
    char c = word[idx];
    display[idx] = c;
    usedLetters.push_back(c);
    cout << "IPUCU: '" << c << "' harfi acildi!\n";
}

void Game::play() {
    cout << "\nKelime " << word.length() << " harfli.\n";

    while (lives > 0) {
        drawHangman();

        cout << "Kelime: ";
        for (char c : display) cout << c << ' ';
        cout << "\nKalan hak: " << lives << endl;

        cout << "Harf / Kelime gir (Ipucu icin Q): ";
        string guess;
        cin >> guess;
        guess = toLowerStr(guess);

        // İPUCU
        if (guess == "q") {
            giveHint();
            continue;
        }

        // TAM KELİME TAHMİNİ
        if (guess.length() > 1) {
            if (guess == word) {
                score += lives * 10;
                display = word;
                break;
            }
            else {
                cout << "Yanlis kelime!\n";
                lives -= 2;
                if (lives < 0) lives = 0;
                continue;
            }
        }

        char letter = guess[0];

        if (find(usedLetters.begin(), usedLetters.end(), letter) != usedLetters.end()) {
            cout << "Bu harf zaten girildi!\n";
            continue;
        }

        usedLetters.push_back(letter);
        bool found = false;

        for (int i = 0; i < word.length(); i++) {
            if (word[i] == letter) {
                display[i] = letter;
                found = true;
            }
        }

        if (found) {
            cout << "Dogru harf!\n";
            score += 5;
        }
        else {
            cout << "Yanlis harf!\n";
            lives--;
        }

        if (isWordGuessed()) break;
    }

    if (isWordGuessed()) {
        cout << "\nTEBRIKLER! Kelime: " << word << endl;
        cout << "Kazandigin puan: " << score << endl;
    }
    else {
        drawHangman();
        cout << "\nKaybettin! Dogru kelime: " << word << endl;
    }

    saveHighScore();
    cout << "High Score: " << max(score, highScore) << endl;
}
