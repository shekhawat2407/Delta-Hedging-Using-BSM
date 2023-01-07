//
// Created by Pradhyumn Singh on 05-10-2022.
//

#include "Traded_Option.h"
#include <cmath>
#include <iostream>
using namespace std ;

Traded_Option::Traded_Option() {
    price=0;
}

Traded_Option::Traded_Option(double K, double S, double r, double T, char type, double price) : Option(K,S,r,T,type) {
    this->price=price;
}
Traded_Option::~Traded_Option() {}

double Traded_Option::getprice() {
    return price;
}


double Traded_Option::Initial_Sigma() {
    double K = getStrikePrice();
    double S = getStockPrice();
    double r = getRiskFreeRate();
    double T = getMaturityTime();

    //return pow((2*abs(log(S/K)+r*T)/T),0.5);
    return sqrt(2*abs(log(S/K)+r*T)/T);
}

double Traded_Option::Volatility_Calculator(double sigma_i,double tolerance_level) {
    double K = getStrikePrice();
    double S = getStockPrice();
    double r = getRiskFreeRate();
    double T = getMaturityTime();
    char type= getOptionType();
    double root_value;
    double root_derivative;
    double sigma;
    double temp;
    double d1 = (log(S / K) + r * T) / (sigma_i * pow(T, 0.5)) + (sigma_i * pow(T, 0.5)) * 0.5;
    double d2 = (log(S / K) + r * T) / (sigma_i * pow(T, 0.5)) - (sigma_i * pow(T, 0.5)) * 0.5;
    if (type=='C' || type=='c') {
        root_value = price- S * erfc(-d1 / sqrt(2)) / 2 + K * exp(-1 * r * T) * erfc(-d2 / sqrt(2)) / 2;
        root_derivative=-1*sqrt(T)*(S*exp(-1*pow(d1,2)/2))/(sqrt(2*M_PI));
    } else{
        root_value= price- K * exp(-1 * r * T) * erfc(d2 / sqrt(2)) / 2 + S * erfc(d1 / sqrt(2)) / 2;
        root_derivative=-1*sqrt(T)*(S*exp(-1*pow(d1,2)/2))/(2* sqrt(M_PI));   //Need to check for put
    }
    sigma= sigma_i-root_value/root_derivative;

    if (abs(sigma-sigma_i)<tolerance_level){
        return sigma;
    }
    else{
        temp= Volatility_Calculator(sigma,tolerance_level);
        return temp;
    }
}

double Traded_Option::Delta_Calculator(double sigma){
    double K = getStrikePrice();
    double S = getStockPrice();
    double r = getRiskFreeRate();
    double T = getMaturityTime();
    char type= getOptionType();
    double delta;
    double d1 = (log(S / K) + r * T) / (sigma * pow(T, 0.5)) + (sigma * pow(T, 0.5)) * 0.5;

    if (type=='C' || type=='c') {
        delta=erfc(-d1 / sqrt(2)) / 2 ;
    } else{
        delta=erfc(-d1 / sqrt(2)) / 2 -1;
    }
    return delta;
}