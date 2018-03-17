#include <stdio.h>
#include "stats.h"
#include <vector>
#include <cmath>
#include <numeric>


stats::stats(PNG & im){
    
    
    for(int x = 0; x < im.width(); x++){
        vector<long> redCol = vector<long>();
        vector<long> blueCol = vector<long>();
        vector<long> greenCol = vector<long>();
        
        vector<long> redsqCol = vector<long>();
        vector<long> bluesqCol = vector<long>();
        vector<long> greensqCol = vector<long>();
        
        for(int y = 0; y < im.height(); y++){
            if(y !=0){
              redCol.push_back(im.getPixel(x,y)->r+sumRed[x-1][y]);
              blueCol.push_back(im.getPixel(x,y)->b+sumBlue[x-1][y]);
              greenCol.push_back(im.getPixel(x,y)->g+sumGreen[x-1][y]);
                
              redsqCol.push_back(pow(im.getPixel(x,y)->r,2)+sumsqRed[x-1][y]);
              bluesqCol.push_back(pow(im.getPixel(x,y)->b,2)+sumsqBlue[x-1][y]);
              greensqCol.push_back(pow(im.getPixel(x,y)->g,2)+sumsqGreen[x-1][y]);
                
            }else if(x != 0){
              redCol.push_back(im.getPixel(x,y)->r+sumRed[x][y-1]);
              blueCol.push_back(im.getPixel(x,y)->b+sumBlue[x][y-1]);
              greenCol.push_back(im.getPixel(x,y)->g+sumGreen[x][y-1]);
                
              redsqCol.push_back(pow(im.getPixel(x,y)->r,2)+sumsqRed[x][y-1]);
              bluesqCol.push_back(pow(im.getPixel(x,y)->b,2)+sumsqBlue[x][y-1]);
              greensqCol.push_back(pow(im.getPixel(x,y)->g,2)+sumsqGreen[x][y-1]);
                
            }else{
                
              redCol.push_back(im.getPixel(x,y)->r+sumRed[x-1][y]+sumRed[x][y-1]);
              blueCol.push_back(im.getPixel(x,y)->b+sumBlue[x-1][y]+sumBlue[x][y-1]);
              greenCol.push_back(im.getPixel(x,y)->g+sumGreen[x-1][y]+sumGreen[x][y-1]);
                
              redsqCol.push_back(pow(im.getPixel(x,y)->r,2)+sumsqRed[x-1][y]+sumsqRed[x][y-1]);
              bluesqCol.push_back(pow(im.getPixel(x,y)->b,2)+sumsqBlue[x-1][y]+sumsqBlue[x][y-1]);
              greensqCol.push_back(pow(im.getPixel(x,y)->g,2)+sumsqGreen[x-1][y]+sumsqGreen[x][y-1]);
                
            }
            
        }
        
        sumRed.push_back(redCol);
        sumBlue.push_back(blueCol);
        sumGreen.push_back(greenCol);
        sumsqRed.push_back(redsqCol);
        sumsqBlue.push_back(bluesqCol);
        sumsqGreen.push_back(greensqCol);
    }
    
}


long stats::getSum(char channel, pair<int,int> ul, pair<int,int> lr){
    long sum = 0;

    if(strcmp(&channel,"r") == 0){
        sum+= sumRed[lr.first][lr.second] - sumRed[ul.first][lr.second] - sumRed[lr.first][ul.second] + sumRed[ul.first][ul.second] ;
    }else if(strcmp(&channel,"b") == 0){
        sum+= sumBlue[lr.first][lr.second] - sumBlue[ul.first][lr.second] - sumBlue[lr.first][ul.second] + sumBlue[ul.first][ul.second] ;
    }else{
        sum+= sumGreen[lr.first][lr.second] - sumGreen[ul.first][lr.second] - sumGreen[lr.first][ul.second] + sumGreen[ul.first][ul.second] ;
    }

    return sum;
}

long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr){
    long sum = 0;
    
    if(strcmp(&channel,"r") == 0){
        sum+= sumsqRed[lr.first][lr.second] - sumsqRed[ul.first][lr.second] - sumsqRed[lr.first][ul.second] + sumsqRed[ul.first][ul.second] ;
    }else if(strcmp(&channel,"b") == 0){
        sum+= sumsqBlue[lr.first][lr.second] - sumsqBlue[ul.first][lr.second] - sumsqBlue[lr.first][ul.second] + sumsqBlue[ul.first][ul.second] ;
    }else{
        sum+= sumsqGreen[lr.first][lr.second] - sumsqGreen[ul.first][lr.second] - sumsqGreen[lr.first][ul.second] + sumsqGreen[ul.first][ul.second] ;
    }
    
    return sum;
}

long stats::getScore(pair<int,int> ul, pair<int,int> lr){
    long sum = getSum('r', ul, lr) + getSum('b', ul, lr) + getSum('g', ul, lr);
    long sumsq = getSumSq('r', ul, lr) + getSumSq('b', ul, lr) + getSumSq('g', ul, lr);
    return sum - (pow(sumsq,2)/rectArea(ul,lr));
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
