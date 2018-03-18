#include <stdio.h>
#include "stats.h"
#include <vector>
#include <cmath>
#include <numeric>


stats::stats(PNG & im){

    // Keeps track of rgb values in columns, each element corresponding to one column
    vector<long> redCol;
    vector<long> blueCol;
    vector<long> greenCol;

    vector<long> redsqCol;
    vector<long> bluesqCol;
    vector<long> greensqCol;
    for(unsigned int x = 0; x < im.width(); x++){
        
        for(unsigned int y = 0; y < im.height(); y++){
            // If y == 0, push new column onto vector
            if(y == 0) {
                RGBAPixel *pixel = im.getPixel(x,y);
                redCol.push_back(pixel->r);
                blueCol.push_back(pixel->b);
                greenCol.push_back(pixel->g);

                redsqCol.push_back(pow(pixel->r,2));
                bluesqCol.push_back(pow(pixel->b,2));
                greensqCol.push_back(pow(pixel->g,2));

                // Create new column in sumVectors
                sumRed.push_back(vector<long>());
                sumBlue.push_back(vector<long>());
                sumGreen.push_back(vector<long>());

                sumsqRed.push_back(vector<long>());
                sumsqBlue.push_back(vector<long>());
                sumsqGreen.push_back(vector<long>());
            // else add in value
            } else {
                RGBAPixel *pixel = im.getPixel(x,y);
                redCol.at(x) += pixel->r;
                blueCol.at(x) += pixel->b;
                greenCol.at(x) += pixel->g;

                redsqCol.at(x) += pow(pixel->r,2);
                bluesqCol.at(x) += pow(pixel->b,2);
                greensqCol.at(x) += pow(pixel->g,2);
                
            }
            // if x != 0, add rectangle shifted one left + current column total
            if(x != 0) {
                sumRed.at(x).push_back(sumRed.at(x-1).at(y) + redCol.at(x));
                sumBlue.at(x).push_back(sumBlue.at(x-1).at(y) + blueCol.at(x));
                sumGreen.at(x).push_back(sumGreen.at(x-1).at(y) + greenCol.at(x));

                sumsqRed.at(x).push_back(sumsqRed.at(x-1).at(y) + redsqCol.at(x));
                sumsqBlue.at(x).push_back(sumsqBlue.at(x-1).at(y) + bluesqCol.at(x));
                sumsqGreen.at(x).push_back(sumsqGreen.at(x-1).at(y) + greensqCol.at(x));
            // if x == 0, push column total
            } else {
                sumRed.at(x).push_back(redCol.at(x));
                sumBlue.at(x).push_back(blueCol.at(x));
                sumGreen.at(x).push_back(greenCol.at(x));

                sumsqRed.at(x).push_back(redsqCol.at(x));
                sumsqBlue.at(x).push_back(bluesqCol.at(x));
                sumsqGreen.at(x).push_back(greensqCol.at(x));
            }
        }
    }
}


long stats::getSum(char channel, pair<int,int> ul, pair<int,int> lr){
    long sum = 0;

    // True if sections exist which need to be subtracted from total
    bool left = true;
    bool top = true;

    bool topAndLeft = false;

    if(ul.first == 0) {
        left = false;
    }
    if(ul.second == 0) {
        top = false;
    }
    if(left && top) {
        topAndLeft = true;
    }
    if(channel == 'r'){
        sum += sumRed[lr.first][lr.second];
        if(top) {
            sum -= sumRed[lr.first][ul.second - 1];
        }
        if(left) {
            sum -= sumRed[ul.first - 1][lr.second];
        }
        if(topAndLeft) {
            sum += sumRed[ul.first - 1][ul.second - 1];
        }
    }else if(channel == 'b'){
        sum += sumBlue[lr.first][lr.second];
        if(top) {
            sum -= sumBlue[lr.first][ul.second - 1];
        }
        if(left) {
            sum -= sumBlue[ul.first - 1][lr.second];
        }
        if(topAndLeft) {
            sum += sumBlue[ul.first - 1][ul.second - 1];
        }
    }else{
        sum += sumGreen[lr.first][lr.second];
        if(top) {
            sum -= sumGreen[lr.first][ul.second - 1];
        }
        if(left) {
            sum -= sumGreen[ul.first - 1][lr.second];
        }
        if(topAndLeft) {
            sum += sumGreen[ul.first - 1][ul.second - 1];
        }
    }
    return sum;
}

long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr){
    long sum = 0;

    // True if sections exist which need to be subtracted from total
    bool left = true;
    bool top = true;

    bool topAndLeft = false;

    if(ul.first == 0) {
        left = false;
    }
    if(ul.second == 0) {
        top = false;
    }
    if(left && top) {
        topAndLeft = true;
    }
    if(channel == 'r'){
        sum += sumsqRed[lr.first][lr.second];
        if(top) {
            sum -= sumsqRed[lr.first][ul.second - 1];
        }
        if(left) {
            sum -= sumsqRed[ul.first - 1][lr.second];
        }
        if(topAndLeft) {
            sum += sumsqRed[ul.first - 1][ul.second - 1];
        }
    }else if(channel == 'b'){
        sum += sumsqBlue[lr.first][lr.second];
        if(top) {
            sum -= sumsqBlue[lr.first][ul.second - 1];
        }
        if(left) {
            sum -= sumsqBlue[ul.first - 1][lr.second];
        }
        if(topAndLeft) {
            sum += sumsqBlue[ul.first - 1][ul.second - 1];
        }
    }else{
        sum += sumsqGreen[lr.first][lr.second];
        if(top) {
            sum -= sumsqGreen[lr.first][ul.second - 1];
        }
        if(left) {
            sum -= sumsqGreen[ul.first - 1][lr.second];
        }
        if(topAndLeft) {
            sum += sumsqGreen[ul.first - 1][ul.second - 1];
        }
    }
    return sum;
}

long stats::getScore(pair<int,int> ul, pair<int,int> lr){
    long sum = getSum('r', ul, lr) + getSum('b', ul, lr) + getSum('g', ul, lr);
    long sumsq = getSumSq('r', ul, lr) + getSumSq('b', ul, lr) + getSumSq('g', ul, lr);
    return sumsq - (pow(sum,2)/rectArea(ul,lr));
}
RGBAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
    long red = getSum('r', ul, lr)/rectArea(ul,lr);
    long blue = getSum('b', ul, lr)/rectArea(ul,lr);
    long green =getSum('g', ul, lr)/rectArea(ul,lr);

    return RGBAPixel(red,green,blue);
}
long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
    return (lr.first-ul.first + 1)*(lr.second-ul.second + 1);
}
