#include <stdio.h>
#include "stats.h"
#include <vector>
#include <cmath>
#include <numeric>


stats::stats(PNG & im){

    sumRed = vector<vector<long>>()
    sumGreen = vector<vector<long>>()
    sumBlue = vector<vector<long>>()
    sumsqRed = vector<vector<long>>()
    sumsqBlue = vector<vector<long>>()
    sumsqGreen = vector<vector<long>>()

    for(int x = 0; x < im.width(); x++){
        // Keeps track of rgb values in columns, each element corresponding to one column
        vector<long> redCol = vector<long>();
        vector<long> blueCol = vector<long>();
        vector<long> greenCol = vector<long>();
        
        vector<long> redsqCol = vector<long>();
        vector<long> bluesqCol = vector<long>();
        vector<long> greensqCol = vector<long>();
        
        for(int y = 0; y < im.height(); y++){
            // If y == 0, push new column onto vector
            if(y == 0) {
                redCol.push_back(im.getPixel(x,y)->r);
                blueCol.push_back(im.getPixel(x,y)->r);
                greenCol.push_back(im.getPixel(x,y)->r);

                redsqCol.push_back(im.getPixel(x,y)->r);
                bluesqCol.push_back(im.getPixel(x,y)->r);
                greensqCol.push_back(im.getPixel(x,y)->r);
            // else add in value
            } else {
                redCol.at(x) += im.getPixel(x.y)->r;
                blueCol.at(x) += im.getPixel(x,y)->r;
                greenCol.at(x) += im.getPixel(x,y)->r;

                redsqCol.at(x) += im.getPixel(x.y)->r;
                bluesqCol.at(x) += im.getPixel(x,y)->r;
                greensqCol.at(x) += im.getPixel(x,y)->r;
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
                sumRed.push_back(vector<long>());
                sumRed.at(x).push_back(redCol.at(x));
                sumBlue.push_back(vector<long>());
                sumBlue.at(x).push_back(blueCol.at(x));
                sumGreen.push_back(vector<long>());
                sumGreen.at(x).push_back(greenCol.at(x));

                sumsqRed.push_back(vector<long>());
                sumsqRed.at(x).push_back(redsqCol.at(x));
                sumsqBlue.push_back(vector<long>());
                sumsqBlue.at(x).push_back(bluesqCol.at(x));
                sumsqGreen.push_back(vector<long>());
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
            sum -= sumRed[lr.first][ul.second];
        }
        if(left) {
            sum -= sumRed[ul.first][lr.second];
        }
        if(topAndLeft) {
            sum += sumRed[ul.first][ul.second];
        }
    }else if(channel == 'b'){
        sum += sumBlue[lr.first][lr.second];
        if(top) {
            sum -= sumBlue[lr.first][ul.second];
        }
        if(left) {
            sum -= sumBlue[ul.first][lr.second];
        }
        if(topAndLeft) {
            sum += sumBlue[ul.first][ul.second];
        }
    }else{
        sum += sumGreen[lr.first][lr.second];
        if(top) {
            sum -= sumGreen[lr.first][ul.second];
        }
        if(left) {
            sum -= sumGreen[ul.first][lr.second];
        }
        if(topAndLeft) {
            sum += sumGreen[ul.first][ul.second];
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
            sum -= sumsqRed[lr.first][ul.second];
        }
        if(left) {
            sum -= sumsqRed[ul.first][lr.second];
        }
        if(topAndLeft) {
            sum += sumsqRed[ul.first][ul.second];
        }
    }else if(channel == 'b'){
        sum += sumsqBlue[lr.first][lr.second];
        if(top) {
            sum -= sumsqBlue[lr.first][ul.second];
        }
        if(left) {
            sum -= sumsqBlue[ul.first][lr.second];
        }
        if(topAndLeft) {
            sum += sumsqBlue[ul.first][ul.second];
        }
    }else{
        sum += sumsqGreen[lr.first][lr.second];
        if(top) {
            sum -= sumsqGreen[lr.first][ul.second];
        }
        if(left) {
            sum -= sumsqGreen[ul.first][lr.second];
        }
        if(topAndLeft) {
            sum += sumsqGreen[ul.first][ul.second];
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

    return RGBAPixel(red,blue,green);
}
long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
    return (lr.first-ul.first)*(lr.second-ul.second);
}
