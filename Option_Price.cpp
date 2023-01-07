//
// Created by Pradhyumn Singh on 05-10-2022.
//

#include "Option_Price.h"
#include <vector>
#include <iostream>
#include <math.h>
using namespace std;

Option_Price::Option_Price() {
    sigma=0;
}

Option_Price::Option_Price (double K, double S, double r, double T,char type,double sigma):Option(K, S, r, T,type){
    this->sigma= sigma;
}

double Option_Price::getVolatility() {
    return sigma;
}

Option_Price::~Option_Price() {}

vector<double> Option_Price:: BSM_Pricer() {
    double K = getStrikePrice();
    double S = getStockPrice();
    double r = getRiskFreeRate();
    double T = getMaturityTime();
    char type= getOptionType();
    double sigma = getVolatility();

    vector<double> solution;
    double price;
    double delta;

    double d1 = (log(S / K) + r * T) / (sigma * pow(T, 0.5)) + (sigma * pow(T, 0.5)) * 0.5;
    double d2 = (log(S / K) + r * T) / (sigma * pow(T, 0.5)) - (sigma * pow(T, 0.5)) * 0.5;
    if (type=='C' || type=='c') {
        price = S * erfc(-d1 / sqrt(2)) / 2 - K * exp(-1 * r * T) * erfc(-d2 / sqrt(2)) / 2;
        delta=erfc(-d1 / sqrt(2)) / 2 ;
    } else{
        price = K * exp(-1 * r * T) * erfc(d2 / sqrt(2)) / 2 - S * erfc(d1 / sqrt(2)) / 2;
        delta=erfc(-d1 / sqrt(2)) / 2 -1;
    }

    solution.push_back(price);
    solution.push_back(delta);

    return solution;
}




