
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"

/* given */
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
{}

/* given */
twoDtree::~twoDtree(){
    clear();
}

/* given */
twoDtree::twoDtree(const twoDtree & other) {
    copy(other);
}

/* given */
twoDtree & twoDtree::operator=(const twoDtree & rhs){
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

twoDtree::twoDtree(PNG & imIn){ /* your code here */ }

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {
    /* your code here */
}

PNG twoDtree::render(){
    /* your code here */
}

void twoDtree::prune(double pct, int tol){
    /* your code here */
}

void clear() {
    recursiveClear(root);
    delete root;
    root = NULL;
}

void twoDtree::recursiveClear(Node* &subRoot) {
    if(subRoot == NULL) {
        return;
    }
    recursiveClear(subRoot->left);
    recursiveClear(subRoot->right);
    delete left;
    delete right;
}


void twoDtree::copy(const twoDtree & orig) {
    recursiveCopy(root, otherRoot);
}

Node* twoDtree::recursiveCopy(Node* root, const Node* otherRoot) {
    if(otherRoot == NULL) {
        root = NULL;
        return NULL;
    }
    root = new Node(otherRoot.upLeft, otherRoot.upRight, otherRoot.avg);
    root->left = recursiveCopy(otherRoot->left);
    root->right = recursiveCopy(otherRoot->right);
    return root;
}

