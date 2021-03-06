
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
#include <cmath>

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
        return new Node(ul, lr, s.getAvg(ul, lr));
    }
    // Split the thing
    long min = -1;
    pair<int, int> p1;
    pair<int, int> p2;
    pair<int, int> minPoint1;
    pair<int, int> minPoint2;

    // Horizontal splits
    for(int y = ul.second; y < lr.second; y++) {
        p1.first = lr.first;
        p1.second = y;
        p2.first = ul.first;
        p2.second = y + 1;
        long score = s.getScore(ul, p1) + s.getScore(p2, lr);
        if(score < min || min == -1) {
            min = score;
            minPoint1 = p1;
            minPoint2 = p2;
        }
    }

    // Vertical splits
    for(int x = ul.first; x < lr.first; x++) {
        p1.first = x;
        p1.second = lr.second;
        p2.first = x + 1;
        p2.second = ul.second;
        long score = s.getScore(ul, p1) + s.getScore(p2, lr);
        if(score < min || min == -1) {
            min = score;
            minPoint1 = p1;
            minPoint2 = p2;
        }
    }

    // Make tree
    Node* newNode = new Node(ul, lr, s.getAvg(ul, lr));
    newNode->left = buildTree(s, ul, minPoint1);
    newNode->right = buildTree(s, minPoint2, lr);
    return newNode;
}

PNG twoDtree::render(){

    PNG png = PNG(root->lowRight.first + 1, root->lowRight.second + 1);
    vector<Node*> nodes = leafNodes(root);
    cout<<nodes.size()<<endl;
    for(unsigned int i = 0; i < nodes.size(); i++) {
        for(int x = nodes[i]->upLeft.first; x <= nodes[i]->lowRight.first; x++) {
            for(int y = nodes[i]->upLeft.second; y <= nodes[i]->lowRight.second; y++) {
                RGBAPixel *pixel = png.getPixel(x,y);
                *pixel = nodes[i]->avg;
            }
        }
    }
   return png;
}




void twoDtree::prune(double pct, int tol) {
    recursivePrune(root,pct, tol);
}

void twoDtree::recursivePrune(twoDtree::Node *node,double pct, int tol) {
    if(node == NULL) {
        return;
    }
  // vector<Node*> pixels = leafNodes(node);
    int num = 0;
    int denom = 0;
   recursivePruneH(node,node,tol,num, denom);
    // for(unsigned int x = 0; x < pixels.size(); x++) {
    //     if(difference(node->avg, pixels[x]->avg) <= tol) {
    //         num++;
    //     }
    // }
    if((double)num / (double)denom >= pct) {
        recursiveClear(node->left);
        recursiveClear(node->right);
        return;
    } else {
        recursivePrune(node->left, pct, tol);
        recursivePrune(node->right, pct, tol);
    }
}

void twoDtree::recursivePruneH(twoDtree::Node *node, twoDtree::Node *noder, int tol, int &num, int &denom){
    if(node== NULL){
        return;
    }
    if(node->left == NULL && node->right == NULL) {
        if(difference(noder->avg, node->avg) <= tol) {
            num++;
        }
        denom++;
    }
    recursivePruneH(node->left, noder,tol, num, denom);
    recursivePruneH(node->right, noder,tol, num, denom);

}

int twoDtree::difference(RGBAPixel a, RGBAPixel b) {
    return pow(a.r-b.r,2) + pow(a.b-b.b,2) + pow(a.g-b.g,2);
}


vector<twoDtree::Node *> twoDtree::leafNodes(Node* node) {
    vector<Node *> vec;
    if(node == NULL) {
        return vec;
    }
    if(node->left == NULL && node->right == NULL) {
        vec.push_back(node);
        return vec;
    }
    vector<Node*> left = leafNodes(node->left);
    vector<Node*> right = leafNodes(node->right);
    vec.insert(vec.end(), left.begin(), left.end());
    vec.insert(vec.end(), right.begin(), right.end());
    return vec;
}

void twoDtree::clear() {
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
    delete subRoot;
    subRoot = NULL;
}


void twoDtree::copy(const twoDtree & orig) {
    root = recursiveCopy(orig.root);
}

twoDtree::Node* twoDtree::recursiveCopy(const Node* otherRoot) {
    if(otherRoot == NULL) {
        return NULL;
    }
    Node* node = new Node(otherRoot->upLeft, otherRoot->lowRight, otherRoot->avg);
    node->left = recursiveCopy(otherRoot->left);
    node->right = recursiveCopy(otherRoot->right);
    return node;
}

