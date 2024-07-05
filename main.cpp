/* 
 * File:   main.cpp
 * Author: Mehak Lohchan
 * Purpose: BlackJack Version 9
 */

// System Libraries
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
using namespace std;

// User-Defined Libraries
#include "BlackJack.h"
#include "CardTree.h"

int main(int argc, char** argv) {
    // Set Random Number Seed Here
    
    // Initialize Variables
    // Initialize the player's balance
    float playerBalance = 0.0;  // Initialize the player's balance

    string pName; // player name
    cout << "Enter player name: " << endl;
    cin >> pName;
    
    // Initialize player accounts using unordered_map
    unordered_map<string, float> playerAccounts;

    // Check if the player already exists in the accounts
    if (playerAccounts.find(pName) == playerAccounts.end()) {
        // If not, add the player to the accounts with an initial balance
        playerAccounts[pName] = playerBalance;
    } else {
        // If the player already exists, retrieve their balance
        playerBalance = playerAccounts[pName];
    }
    
    // Display the outputs
    // Welcome Message
    cout << "\t\tWelcome to Blackjack!\n\n";

    // Manage player's balance
    playerBalance = balance(playerBalance);

    // Wait for Enter key press
    cout << "Press Enter to view the rules and play...\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    // Display the Game Rules
    rules();

    // Ask the player if they want to play the game
    cout << "\nDo you want to play Blackjack? (y/n): ";
    char playGame;
    cin >> playGame;

    if (playGame != 'y') {
        cout << "Goodbye!" << endl;
        return 0;
    }

    // Begin Game Implementation
    
    // Containers using pointers
    list<int>* deck = new list<int>();
    set<int>* dealtCards = new set<int>();
    list<int>* player1Hand = new list<int>();
    list<int>* player2Hand = new list<int>();
    list<int>* dealerHand = new list<int>();

    
    // Card names using map container
    map<int, string> cardNames;
    cardNames[1] = "Ace";
    cardNames[2] = "2";
    cardNames[3] = "3";
    cardNames[4] = "4";
    cardNames[5] = "5";
    cardNames[6] = "6";
    cardNames[7] = "7";
    cardNames[8] = "8";
    cardNames[9] = "9";
    cardNames[10] = "10";
    cardNames[11] = "Jack";  
    cardNames[12] = "Queen"; 
    cardNames[13] = "King";
        
       
    // Print card names
    cout << "\n\nCard Names Using Map Container:\n";
    for (int i = 1; i <= 13; ++i) {
        cout << "Card " << i << ": " << cardNames[i] << endl;
    }
    cout << "\n";
    
    
    // Card names using a tree
    CardTree cardTree;

    // insert card names into the tree
    cardTree.insert(cardTree.root, 1, "Ace");
    cardTree.insert(cardTree.root, 2, "2");
    cardTree.insert(cardTree.root, 3, "3");
    cardTree.insert(cardTree.root, 4, "4");
    cardTree.insert(cardTree.root, 5, "5");
    cardTree.insert(cardTree.root, 6, "6");
    cardTree.insert(cardTree.root, 7, "7");
    cardTree.insert(cardTree.root, 8, "8");
    cardTree.insert(cardTree.root, 9, "9");
    cardTree.insert(cardTree.root, 10, "10");
    cardTree.insert(cardTree.root, 11, "Jack");
    cardTree.insert(cardTree.root, 12, "Queen");
    cardTree.insert(cardTree.root, 13, "King");

    // display the card names
    cout << "\n\nCard Names Using A Tree:\n";
    cardTree.printInOrder(cardTree.root);
    cout << "\n";

    
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));

    // Initialize the deck with cards associated with blackjack
    inDeck(deck);
                
    // Display the sorted deck
    cout << "Sorted Deck: ";
    for (list<int>::iterator it = deck->begin(); it != deck->end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // Shuffle the deck
    shfDeck(deck);

    // Display the unsorted (shuffled) deck
    cout << "\nShuffled Deck Using Recursive Shuffle Function: " << endl;
    for (list<int>::iterator it = deck->begin(); it != deck->end(); ++it) {
        cout << *it << " ";
    }
    cout << endl << endl;
    
    std::queue<char> playerActions;

    // Initialize the leaderboard stack
    std::stack<Player> leaderboard;

    // clear list before the game loop
    player1Hand->clear();
    player2Hand->clear();
    dealerHand->clear();
    

    // Game loop
    while (true) {
        // Ensure the player has enough balance to play
        playerBalance = balance(playerBalance);

        // Play a round of Blackjack
        playBlackjackRound(playerBalance, deck, dealtCards, player1Hand, player2Hand, dealerHand, cardNames);

        // Ask if the user wants to play again
        char playAgain;

        if (playAgain != 'y') {
            break;
        }
        else if (playAgain == 'n') {
            exit(0);
        }

    } // ***end of game loop***

        
    // Display player 1's updated balance
    cout << "\n\nYour final balance: $" << playerBalance << endl;

    // Release dynamically allocated memory outside of the loop
    allocM(deck, dealtCards, player1Hand, player2Hand, dealerHand);

 
    leaderboard.push(Player("Player2", 150.0));
    leaderboard.push(Player(pName, playerBalance));

    // Display the initial leaderboard
    ldrboard(leaderboard);
    
    
    return 0;
}