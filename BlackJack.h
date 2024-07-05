/* 
 * File:   main.cpp
 * Author: Mehak Lohchan
 * Purpose: BlackJack Struct Implementation + Functions
 */

#ifndef BLACKJACK_H
#define BLACKJACK_H

struct Player {
    std::string name;
    float score;

    Player(const std::string& playerName, float playerScore) : name(playerName), score(playerScore) {}
};

    void inDeck(std::list<int>*);
    void shfDeck(std::list<int>*);
    void recursiveShuffle(list<int>::iterator, list<int>*, int);
    int dlCrd(std::list<int>*, std::set<int>*);
    int crdVal(int);
    int handVal(std::list<int>*, bool);
    void dspHnd(std::list<int>*, std::map<int, std::string>&, bool);
    void dspCrd(int value, const std::string&);
    void allocM(std::list<int>*, std::set<int>*, std::list<int>*, std::list<int>*, std::list<int>*);
    void dDown(std::list<int>*, std::list<int>*, std::set<int>*, std::map<int, std::string>&);
    void split(std::list<int>*, std::list<int>*, std::set<int>*, std::map<int, std::string>&);
    void insrnce(std::list<int>*, std::list<int>*, std::list<int>*, std::set<int>*);
    void hrzLine(int, char);
    void cntrTxt(const std::string&, int, char);
    void rules();
    void crdVles();
    float balance(float);
    void ldrboard(std::stack<Player>&);
    void playBlackjackRound(float&, list<int>*, set<int>*, list<int>*, list<int>*, list<int>*, map<int, string>&);
    void player1Turn(float&, list<int>*, set<int>*, list<int>*, list<int>*, map<int, string>&);
    void player2Turn(list<int>*, set<int>*, list<int>*, map<int, string>&);
    void dealerTurn(list<int>*, set<int>*, list<int>*, map<int, string>&);
    void determineWinner(float&, int, int, int, const string&);
    void resetHandsAndDealtCards(list<int>*, list<int>*, list<int>*, set<int>*);


#endif /* BLACKJACK_H */