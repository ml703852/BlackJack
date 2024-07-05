/* 
 * File:   CardTree.cpp
 * Author: Mehak Lohchan
 * Purpose: Card Tree Implementation
 */

#include "CardTree.h"

CardTree::CardTree() : root(nullptr) {}

void destroyTree(CardNode* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

CardTree::~CardTree() {
    // Call the helper function to delete nodes
    destroyTree(root);
    root = nullptr;  // Reset root to nullptr after deleting all nodes
}


void CardTree::insert(CardNode*& node, int value, const std::string& cardName) {
    if (node == nullptr) {
        node = new CardNode(value, cardName);
    } else if (value < node->value) {
        insert(node->left, value, cardName);
    } else {
        insert(node->right, value, cardName);
    }
}

void CardTree::printInOrder(CardNode* node) {
    if (node != nullptr) {
        printInOrder(node->left);
        std::cout << "Card " << node->value << ": " << node->name << std::endl;
        printInOrder(node->right);
    }
}