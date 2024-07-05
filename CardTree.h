/* 
 * File:   CardTree.h
 * Author: Mehak Lohchan
 * Purpose: Card Tree Class
 */

#ifndef CARDTREE_H
#define CARDTREE_H

#include <iostream>
#include <map>
#include <string>

class CardNode {
    public:
        int value;
        std::string name;
        CardNode* left;
        CardNode* right;

        CardNode(int val, const std::string& cardName) : value(val), name(cardName), left(nullptr), right(nullptr) {}
};

class CardTree {
    public:
        CardNode* root;

        CardTree();
        ~CardTree();

        void insert(CardNode*& node, int value, const std::string& cardName);
        void printInOrder(CardNode* node);
};

#endif // CARDTREE_H