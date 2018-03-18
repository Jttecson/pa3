
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"
#include "stats.h"

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

twoDtree::twoDtree(PNG & imIn){
    height = imIn.height();
    width = imIn.width();
    stats s = stats(imIn);
    root = buildTree(s, make_pair (0,0), make_pair (width - 1, height - 1));
}

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {
    if(ul == lr) {
        return NULL;
    }
    // Split the thing
    long min = -1;
    pair<int, int> p1;
    pair<int, int> p2;
    pair<int, int> minPoint1;
    pair<int, int> minPoint2;
    // Vertical splits
    for(int x = ul.first; x < lr.first; x++) {
        p1.first = x;
        p1.second = lr.second;
        p2.first = x + 1;
        p2.second = ul.second;
        long score = getScore(ul, p1) + getScore(p2, lr);
        if(score < min || min == -1) {
            min = score;
            minPoint1 = p1;
            minPoint2 = p2;
        }
    }
    // Horizontal splits
    for(int y = ul.second; y < lr.second; y++) {
        p1.first = lr.first;
        p1.second = y;
        p2.first = ul.first;
        p2.second = y + 1;
        long score = getScore(ul, p1) + getScore(p2, lr);
        if(score < min || min == -1) {
            min = score;
            minPoint1 = p1;
            minPoint2 = p2;
        }
    }
    // Make tree
    Node* newNode = new Node(ul, lr, getAvg(ul, lr));
    newNode->left = buildTree(s, ul, minPoint1);
    newNode->right = buildTree(s, minPoint2, lr);
    return newNode;
}

PNG twoDtree::render(){
    /* your code here */
}

void twoDtree::prune(double pct, int tol) {
    recursivePrune(root, pct, tol);
}

void recursivePrune(Node* node, double pct, int tol) {
    vector<RBGAPixel> pixels = leafValues(node);
    int num = 0;
    int denom = pixels.size();
    for(int x = 0; x < pixels.size(); x++) {
        if(difference(node->a, pixels.at(x)) > tol) {
            num++;
        }
    }
    if((double)num / (double)denom > pct) {
        recursiveClear(node->left);
        recursiveClear(node->right);
        node->left = NULL;
        node->right = NULL;
        return;
    } else {
        recursivePrune(node->left);
        recursivePrune(node->right);
    }
}

int difference(RBGAPixel a, RGBAPixel b) {

}

vector<RGBAPixel> leafValues(Node* node) {
    vector<RGBAPixel> vec;
    if(node == NULL) {
        return vec;
    }
    vec.push_back(node->a);
    vector<RGBAPixel> left = leafValues(node->left);
    vector<RGBAPixel> right = leafValues(node->right);
    vec.insert(vec.end(), left.begin(), left.end());
    vec.insert(vec.end(), right.begin(), right.end());
    return vec;
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

