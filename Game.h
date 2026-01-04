#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

using namespace std;

class Game {
private:
    string word;
    string display;
    vector<char> usedLetters;
    int maxLives;
    int lives;
    int score;
    int highScore;

    void loadHighScore();
    void saveHighScore();
    void drawHangman();
    bool isWordGuessed();
    void giveHint();
    string toLowerStr(string s);

public:
    Game(int difficulty);
    void chooseCategory();
    void play();
};

#endif
