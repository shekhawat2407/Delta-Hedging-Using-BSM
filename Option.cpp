//
// Created by Pradhyumn Singh on 05-10-2022.
//

#include "Option.h"
#include<iostream>

using namespace std;

void Option::init() {
    K=0;
    S=0;
    T=0;
    r=0;
    type='c';
}

Option::Option() {
    init();
}

Option::~Option() {}


Option::Option(double K, double S, double r, double T, char type)  {
    this->K = K;
    this->S = S;
    this->r = r;
    this->T = T;
    this->type = type;
}

double  Option::getStrikePrice(){
    return K;
}
double Option::getStockPrice() {
    return S;
}
double Option::getRiskFreeRate(){
    return r;
}
double Option::getMaturityTime() {
    return T;
}
char Option::getOptionType(){
    /*if (type=='C' || type=='c'){
    cout<< "Option Type is Call"<<endl;}
    else{
      cout<<"Option Type is Put"<<endl;
    }*/
    return type;
}


