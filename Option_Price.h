//
// Created by Pradhyumn Singh on 05-10-2022.
//

#ifndef PROJECT1_DELTAHEDGING_OPTION_PRICE_H
#define PROJECT1_DELTAHEDGING_OPTION_PRICE_H

#include "Option.h"
#include <vector>
using namespace std;



class Option_Price: public Option {
private:
    double sigma;

public:
    Option_Price();
    Option_Price(double K, double S, double r, double T, char type, double sigma);
    ~Option_Price();
    double getVolatility () ;
    vector<double> BSM_Pricer();
};

#endif //PROJECT1_DELTAHEDGING_OPTION_PRICE_H

