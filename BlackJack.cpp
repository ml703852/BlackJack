/* 
 * File:   main.cpp
 * Author: Mehak Lohchan
 * Purpose: BlackJack Game Implementation
 */

// System Libraries
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
using namespace std;

// User Defined Libraries
#include "BlackJack.h"

// Function Implementations

// Function to initialize the card deck with cards associated with blackjack
void inDeck(list<int>* deck) {
    for (int i = 1; i <= 13; ++i) {
        for (int j = 0; j < 4; ++j) {
            deck->push_back(i);
        }
    }
}

// Recursive function to shuffle the card deck
void recursiveShuffle(list<int>::iterator current, list<int>* deck, int shuffleInterval) {
    if (current == deck->end()) {
        // base case - reached the end of the deck
        return;
    }

    // Generate a random index in the remaining unshuffled portion of the deck
    int randIndex = rand() % distance(deck->begin(), deck->end());

    // Swap the current element with the randomly chosen element
    iter_swap(current, next(deck->begin(), randIndex));

    // Recursively shuffle the remaining portion of the deck
    recursiveShuffle(next(current), deck, shuffleInterval);
}

// function for the recursive shuffle
void shfDeck(list<int>* deck) {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));

    // Start the recursive shuffle from the beginning of the deck
    recursiveShuffle(deck->begin(), deck, 0);
}

int crdVal(int card) {
    return (card > 10) ? 10 : card;
}

// function to deal the card
int dlCrd(list<int>* deck, set<int>* dealtCards) {
    // initialize card variable
    int card;
    // loop to ensure the dealt card is not already in the set of dealt cards
    do {
        // get last card from the deck
        card = deck->back();
        // remove the last card from the deck
        deck->pop_back();
    } while (dealtCards->count(card) > 0);  // continue if the card has already been dealt

    // add the dealt card to the set of dealt cards
    dealtCards->insert(card);

    // return the value of the dealt card
    return card;
}

// function to display hand
void dspHnd(list<int>* hand, map<int, string>& cardNames, bool isDealer) {
    for (list<int>::iterator it = hand->begin(); it != hand->end(); ++it) {
        if (isDealer && it == hand->begin()) {
            // For the dealer's first card, show only the card value without the name
            cout << crdVal(*it) << " ";
        } else {
            cout << cardNames[*it] << " ";
        }
    }
    cout << endl;
}

// function to calculate total value of a hand in Blackjack
int handVal(list<int>* hand, bool isDealer) {
    int total = 0;       // store the total value of the hand
    int numAces = 0;     // count the number of Aces in the hand

    // iterate through each card in the hand
    for (list<int>::iterator it = hand->begin(); it != hand->end(); ++it) {
        int cardValue = crdVal(*it);  // get value of the card using the crdVal func
        total += cardValue;           // add card value to total

        if (cardValue == 1) {
            ++numAces;  // if the card is an Ace, increment Ace count
        }
    }

    // if the hand contains an Ace and it won't bust the hand, treat the Ace as 11
    while (numAces > 0 && total + 10 <= 21) {
        total += 10;   // Treat the Ace as 11
        --numAces;     // Decrement the Ace count
    }

    // return calculated total value of hand
    return total;
}

// double down function
void dDown(list<int>* hand, list<int>* deck, set<int>* dealtCards, map<int, string>& cardNames) {
    // Double the player's bet and draw one more card
    cout << "Doubling down! Your bet is doubled." << endl;

    // Check if the hand is empty
    if (hand->empty()) {
        cout << "Error: Empty hand." << endl;
        return;
    }
    // Check if an Ace is already in the hand using find
    list<int>::iterator aceIterator = find(hand->begin(), hand->end(), 1);
    bool hasAce = (aceIterator != hand->end());

    hand->push_back(dlCrd(deck, dealtCards));

    // Display the new card
    cout << "You drew a " << cardNames[hand->back()] << endl;

    // Check if an Ace was drawn and adjust total accordingly
    if (hand->back() == 1 && !hasAce) {
        cout << "The newly drawn Ace is valued as 11." << endl;
    }

    // Adjust total for the newly drawn Ace without recalculating the entire hand
    if (hand->back() == 1 && handVal(hand, false) + 10 <= 21) {
        cout << "The Ace is valued as 11, and your total is now: " << handVal(hand, false) << endl;
    }
}

// function for player insurance - need to implement into main 
void insrnce(list<int>* playerHand, list<int>* dealerHand, list<int>* deck, set<int>* dealtCards) {
    // Offer insurance to the player
    cout << "Do you want to buy insurance? (y/n): ";
    char choice;
    cin >> choice;

    if (choice == 'y') {
        // Check if the dealer has an Ace face-up
        list<int>::iterator it = dealerHand->begin();  // bi-directional iterator for dealerHand

        if (*it == 1) {
            // Player buys insurance
            cout << "You bought insurance. If the dealer has a natural 21, you will be protected." << endl;
        } else {
            cout << "Insurance declined. Continue playing." << endl;
        }
    }
}

// split function
void split(list<int>* hand, list<int>* deck, set<int>* dealtCards, map<int, string>& cardNames) {
    // Split the hand into two separate hands
    cout << "Splitting! Your hand is now two separate hands." << endl;
    list<int> newHand;
    newHand.push_back(hand->back());
    hand->pop_back();
    hand->push_back(dlCrd(deck, dealtCards));
    newHand.push_back(dlCrd(deck, dealtCards));

    // first Hand
    cout << "First Hand: " << endl;
    list<int> firstHand;
    firstHand.push_back(hand->front());
    hand->pop_front();
    dspHnd(&firstHand, cardNames, false);
    char choice;
    cout << "Do you want to hit (h) or stand (s) for the first hand? ";
    cin >> choice;
    while (choice == 'h') {
        firstHand.push_back(dlCrd(deck, dealtCards));
        cout << "First Hand: ";
        dspHnd(&firstHand, cardNames, false);
        int total = handVal(&firstHand, false);
        cout << "Total: " << total << endl;
        if (total > 21) {
            cout << "Bust! First hand loses." << endl;
            break;
        }
        cout << "Do you want to hit (h) or stand (s) for the first hand? ";
        cin >> choice;
    }

    // second Hand
    cout << "Second Hand: " << endl;
    list<int> secondHand;
    secondHand.push_back(newHand.front());
    newHand.pop_front();
    dspHnd(&secondHand, cardNames, false);
    cout << "Do you want to hit (h) or stand (s) for the second hand? ";
    cin >> choice;
    while (choice == 'h') {
        secondHand.push_back(dlCrd(deck, dealtCards));
        cout << "Second Hand: ";
        dspHnd(&secondHand, cardNames, false);
        int total = handVal(&secondHand, false);
        cout << "Total: " << total << endl;
        if (total > 21) {
            cout << "Bust! Second hand loses." << endl;
            break;
        }
        cout << "Do you want to hit (h) or stand (s) for the second hand? ";
        cin >> choice;
    }

    // compare results of each hand
    int firstHandTotal = handVal(&firstHand, false);
    int secondHandTotal = handVal(&secondHand, false);

    cout << "First Hand Total: " << firstHandTotal << endl;
    cout << "Second Hand Total: " << secondHandTotal << endl;

    // Determine winners and losers for each hand
    if (firstHandTotal > 21 && secondHandTotal > 21) {
        cout << "Both hands bust. You lose both hands." << endl;
    } else if (firstHandTotal > 21) {
        cout << "First hand busts. You lose the first hand." << endl;
        // Handle logic for the second hand winning or losing
    } else if (secondHandTotal > 21) {
        cout << "Second hand busts. You lose the second hand." << endl;
        // Handle logic for the first hand winning or losing
    } else {
        if (firstHandTotal > secondHandTotal) {
            cout << "First hand wins!" << endl;
            // Handle logic for the second hand losing
        } else if (secondHandTotal > firstHandTotal) {
            cout << "Second hand wins!" << endl;
            // Handle logic for the first hand losing
        } else {
            cout << "It's a tie for both hands." << endl;
        }
    }
}

// function to manage the player's balance
float balance(float currentBalance) {
    cout << "\nYour current balance: $" << currentBalance << std::endl;

    // Base case: Check if the player has at least $5 to play
    if (currentBalance >= 5.0) {
        return currentBalance;
    } else {
        cout << "You need at least $5 to play. Please add funds to your balance.\n" << std::endl;
        cout << "Enter the amount to add: $";
        float amountToAdd;
        cin >> amountToAdd;
        currentBalance += amountToAdd;

        // Recursive call
        return balance(currentBalance);
    }
}


void hrzLine(int width, char borderChar) {
    cout << string(width, borderChar) << "\n";
}

void cntrTxt(const string& text, int width) {
    int padding = (width - text.length()) / 2;
    cout << string(padding, ' ') << text << "\n";
}

void rules() {
    int lineWidth = 50;

    hrzLine(lineWidth, '=');
    cntrTxt(" Blackjack Rules ", lineWidth);
    hrzLine(lineWidth, '=');

    cout << "\nObjective:\n";
    cout << "   - Beat the dealer by having a hand closer to 21.\n";
    cout << "   - Exceeding 21 results in a bust, leading to a loss.\n\n";

    cout << "Card Values:\n";
    cout << "   - Numbered cards: Face value.\n";
    cout << "   - Face cards (J, Q, K): 10 points.\n";
    cout << "   - Ace: 1 or 11 points, based on hand benefit.\n\n";

    cout << "Gameplay:\n";
    cout << "   - Each player, including the dealer, gets two cards.\n";
    cout << "   - Players 'hit' (draw) or 'stand' (keep).\n";
    cout << "   - Exceeding 21 results in a bust.\n";
    cout << "   - Dealer draws until 17 or higher.\n\n";

    cout << "Winning Conditions:\n";
    cout << "   - Closer to 21 than the dealer: Win.\n";
    cout << "   - Dealer busts: Players win.\n";
    cout << "   - Tie is a 'push,' bet returned.\n\n";

    cout << "Blackjack (Natural 21):\n";
    cout << "   - Ace and a 10-value card.\n";
    cout << "   - Player wins unless the dealer also has Blackjack.\n\n";

    cout << "Options:\n";
    cout << "   - Variations: 'Double Down,' 'Split,' 'Insurance.'\n";
    cout << "   - 'Double Down': Double bet, commit to stand.\n";
    cout << "   - 'Split': Split same-value cards into two hands.\n";
    cout << "   - 'Insurance': Side bet on dealer's natural 21.\n\n";

    cout << "Remember:\n";
    cout << "   - Chance involved, but strategy matters.\n";
    cout << "   - Enjoy the game and have fun!\n\n";
}

// Function to display the leaderboard
void ldrboard(stack<Player>& leaderboard) {
    cout << "\n\n------------ Leaderboard ------------" << endl;

    if (!leaderboard.empty()) {
        Player player1 = leaderboard.top();
        cout << setw(2) << "1. Name: " << setw(10) << player1.name << setw(10)
                  << "Score: $" << setw(8) << player1.score << endl;
        leaderboard.pop();
    }

    if (!leaderboard.empty()) {
        Player player2 = leaderboard.top();
        cout << setw(2) << "2. Name: " << setw(10) << player2.name << setw(10)
                  << "Score: $" << setw(8) << player2.score << endl;
        leaderboard.pop();
    }

    cout << "-------------------------------------" << endl;
}

void playBlackjackGame() {
    float playerBalance = 0.0;  // Initial player balance
    string pName; // Player's name

    // Initialize deck, dealt cards, and hands
    list<int>* deck = new list<int>();
    set<int>* dealtCards = new set<int>();
    list<int>* player1Hand = new list<int>();
    list<int>* player2Hand = new list<int>();
    list<int>* dealerHand = new list<int>();

    // Map to store card names
    map<int, string> cardNames;
    inDeck(deck);

    // ***** GAME LOOP *****
    while (true) {
        // Ensure the player has enough balance to play
        playerBalance = balance(playerBalance);

        // Shuffle the deck
        shfDeck(deck);

        // Initial deal - not putting in cards, check for 2nd round
        player1Hand->push_back(dlCrd(deck, dealtCards));
        player2Hand->push_back(dlCrd(deck, dealtCards));
        dealerHand->push_back(dlCrd(deck, dealtCards));
        
        // Print the hands of player2 and the dealer (showing only the first card)
        cout << "\n\nPlayer 2's hand (first dealt card): " << crdVal(player2Hand->front()) << "\n";
        cout << "\nDealer's hand (first dealt card): " << crdVal(dealerHand->front()) << " \n\n";

        // Player 1's turn
        player1Turn(playerBalance, deck, dealtCards, player1Hand, dealerHand, cardNames);

        // Player 2's turn
        player2Turn(deck, dealtCards, player2Hand, cardNames);

        // Dealer's turn
        dealerTurn(deck, dealtCards, dealerHand, cardNames);

        // Determine the winner
        determineWinner(playerBalance, handVal(player1Hand, false), handVal(player2Hand, false), handVal(dealerHand, true), pName);

        // Display the updated balance
        cout << "\n\nYour current balance: $" << playerBalance << endl;

        // Ask if the user wants to play again
        cout << "\nDo you want to play again? (y/n): ";
        char playAgain;
        cin >> playAgain;

        if (playAgain != 'y') {
            break;
        }

        // Clear hands and dealt cards for the next round
        resetHandsAndDealtCards(player1Hand, player2Hand, dealerHand, dealtCards);
    } // ***end of game loop***

    // De-allocate memory
    allocM(deck, dealtCards, player1Hand, player2Hand, dealerHand);
}


void playBlackjackRound(float& playerBalance, list<int>* deck, set<int>* dealtCards, list<int>* player1Hand,
    list<int>* player2Hand, list<int>* dealerHand, map<int, string>& cardNames) {
    // Ensure the player has enough balance to play
    playerBalance = balance(playerBalance);

    // Shuffle the deck
    shfDeck(deck);

    // Initial deal - not putting in cards, check for 2nd round
    player1Hand->push_back(dlCrd(deck, dealtCards));
    player2Hand->push_back(dlCrd(deck, dealtCards));
    dealerHand->push_back(dlCrd(deck, dealtCards));
    dealerHand->push_back(dlCrd(deck, dealtCards));

    // Print the hands of player2 and the dealer (showing only the first card)
    cout << "\n\nPlayer 2's hand (first dealt card): " << crdVal(player2Hand->front()) << "\n";
    cout << "\nDealer's hand (first dealt card): " << crdVal(dealerHand->front()) << " \n\n";

    // Player 1's turn
    player1Turn(playerBalance, deck, dealtCards, player1Hand, dealerHand, cardNames);

    // Player 2's turn
    player2Turn(deck, dealtCards, player2Hand, cardNames);

    // Dealer's turn
    dealerTurn(deck, dealtCards, dealerHand, cardNames);

    // Determine the winner
    determineWinner(playerBalance, handVal(player1Hand, false), handVal(player2Hand, false), handVal(dealerHand, true), "Player 1");

    // Display the updated balance
    cout << "\n\nYour current balance: $" << playerBalance << endl; 

    // Ask if the user wants to play again
    cout << "\nDo you want to play again? (y/n): ";
    char playAgain;
    cin >> playAgain;

    if (playAgain != 'y') {
        // Clear hands and dealt cards only if the user chooses not to play again
        resetHandsAndDealtCards(player1Hand, player2Hand, dealerHand, dealtCards);
    }
}


void player1Turn(float& playerBalance, list<int>* deck, set<int>* dealtCards, list<int>* player1Hand, list<int>* dealerHand, map<int, string>& cardNames) {
    int p1total = 0;
    bool gotAce = false;  // Flag to track whether the player got an Ace

    while (p1total < 21) {
        // Sort the player's hand before displaying
        player1Hand->sort();

        // Display the sorted hand
        cout << "Player 1's hand (Sorted): ";
        dspHnd(player1Hand, cardNames, false);

        p1total = handVal(player1Hand, false);
        cout << "Player 1's total: " << p1total;

        // Player's Game Choices - hit, stay, double down, split
        cout << "\nDo you want to hit (h), stand (s), split (t), or double down (d)? ";
        char choice;
        cin >> choice;
        cout << endl;

        // Player draws a card if they choose to hit
        if (p1total != 21) {
            if (choice == 'h') {
                // Draw a card for Player 1
                int drawnCard = dlCrd(deck, dealtCards);
                player1Hand->push_back(drawnCard);

                // Display the drawn card for Player 1
                cout << "Player 1 drew: " << cardNames[drawnCard] << endl;

                // Update Player 1's total
                p1total = handVal(player1Hand, false);
                cout << "Player 1's total: " << p1total << endl;

                // Check if Player 1 busts
                if (p1total > 21) {
                    cout << "Player 1 busted! You lose!" << endl;
                    playerBalance -= 5.0;
                    break;
                }
            } else if (choice == 't') {
                split(player1Hand, deck, dealtCards, cardNames);
                break; // Exit the loop after splitting
            } else if (choice == 'd') {
                // Double down logic
                dDown(player1Hand, deck, dealtCards, cardNames);
                break; // Exit the loop after doubling down
            } else if (choice == 's') {
                // Player chooses to stand
                break;
            }

        }

        // If the player got an Ace and it was treated as 11, update the total
        if (gotAce) {
            int updatedTotal = handVal(player1Hand, false);
            cout << "Player 1's updated total after getting an Ace: " << updatedTotal << endl;
        }
    }
}

void player2Turn(list<int>* deck, set<int>* dealtCards, list<int>* player2Hand, map<int, string>& cardNames) {
    // Player 2's turn (automated)
    while (handVal(player2Hand, false) < 17) {
        player2Hand->push_back(dlCrd(deck, dealtCards));

        // Display Player 2's hand during each iteration of Player 1's turn
        cout << "\nPlayer 2's hand: ";
        dspHnd(player2Hand, cardNames, false);
        cout << "Player 2's total: " << handVal(player2Hand, false) << endl;

        // Check if Player 2 busts
        if (handVal(player2Hand, false) > 21) {
            cout << "Player 2 busted!" << endl;
            break;
        }
    }
}


void dealerTurn(list<int>* deck, set<int>* dealtCards, list<int>* dealerHand, map<int, string>& cardNames) {
    float dealerTotal = 0.0;  // Initialize dealer's total

    // Dealer's turn
    while (dealerTotal < 17) {
        dealerHand->push_back(dlCrd(deck, dealtCards));

        // Sort the dealer's hand after drawing a new card
        dealerHand->sort();

        // Display the dealer's hand
        dealerTotal = handVal(dealerHand, true);
        cout << "Dealer's hand (Sorted): ";
        dspHnd(dealerHand, cardNames, false);
        cout << "Dealer's total: " << dealerTotal << endl;

        // Check if the dealer busts or reaches 21
        if (dealerTotal > 21) {
            cout << "Dealer busts!" << endl;
            break;
        }
    }
}

void determineWinner(float& playerBalance, int p1total, int player2Total, int dealerTotal, const string& pName) {
    // Check for Blackjack (Natural 21) conditions
    bool p1HasBlackjack = (p1total == 21 && player2Total != 21 && dealerTotal != 21);
    bool player2HasBlackjack = (player2Total == 21 && dealerTotal != 21);
    bool dealerHasBlackjack = (dealerTotal == 21);

    // Determine the winner based on Blackjack and total values
    if (p1HasBlackjack || (p1total <= 21 && (p1total > player2Total || player2Total > 21))) {
        cout << "\n" << pName << " Wins!" << endl;
        playerBalance += 5.0;
    } else if (player2HasBlackjack || (player2Total <= 21 && (player2Total > p1total || p1total > 21))) {
        cout << "\nPlayer 2 Wins!" << endl;
        playerBalance -= 5.0;
    } else if (dealerHasBlackjack || (dealerTotal <= 21 && (dealerTotal > p1total || dealerTotal > player2Total))) {
        cout << "\nDealer Wins!" << endl;
        playerBalance -= 5.0;
    } else {
        // No one wins in case of a tie or all bust
        cout << "\nIt's a tie or all players bust. No one wins." << endl;
    }

    // Display the updated balance
    cout << "\nYour current balance: $" << playerBalance << endl;
}

void resetHandsAndDealtCards(list<int>* player1Hand, list<int>* player2Hand, list<int>* dealerHand, set<int>* dealtCards) {
    // Clear hands and dealt cards for the next round
    player1Hand->clear();
    player2Hand->clear();
    dealerHand->clear();
    dealtCards->clear();
}

// function to allocate memory
void allocM(list<int>* deck, set<int>* dealtCards, list<int>* playerHand, list<int>* player2Hand, list<int>* dealerHand) {
    delete deck;
    delete dealtCards;
    delete playerHand;
    delete player2Hand;
    delete dealerHand;
}