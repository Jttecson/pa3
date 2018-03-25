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
                redCol[x] += pixel->r;
                blueCol[x] += pixel->b;
                greenCol[x] += pixel->g;

                redsqCol[x] += pow(pixel->r,2);
                bluesqCol[x] += pow(pixel->b,2);
                greensqCol[x] += pow(pixel->g,2);
                
            }
            // if x != 0, add rectangle shifted one left + current column total
            if(x != 0) {
                sumRed[x].push_back(sumRed[x-1][y] + redCol[x]);
                sumBlue[x].push_back(sumBlue[x-1][y] + blueCol[x]);
                sumGreen[x].push_back(sumGreen[x-1][y] + greenCol[x]);

                sumsqRed[x].push_back(sumsqRed[x-1][y] + redsqCol[x]);
                sumsqBlue[x].push_back(sumsqBlue[x-1][y] + bluesqCol[x]);
                sumsqGreen[x].push_back(sumsqGreen[x-1][y] + greensqCol[x]);
            // if x == 0, push column total
            } else {
                sumRed[x].push_back(redCol[x]);
                sumBlue[x].push_back(blueCol[x]);
                sumGreen[x].push_back(greenCol[x]);

                sumsqRed[x].push_back(redsqCol[x]);
                sumsqBlue[x].push_back(bluesqCol[x]);
                sumsqGreen[x].push_back(greensqCol[x]);
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


    long red = getSumSq('r',ul,lr) - pow(getSum('r',ul,lr),2)/abs(rectArea(ul,lr));
    long blue = getSumSq('b',ul,lr) - pow(getSum('b',ul,lr),2)/abs(rectArea(ul,lr));
    long green = getSumSq('g',ul,lr) - pow(getSum('g',ul,lr),2)/abs(rectArea(ul,lr));

    return red+blue+green;


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
