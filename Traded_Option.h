//
// Created by Pradhyumn Singh on 05-10-2022.
//

#ifndef PROJECT1_DELTAHEDGING_TRADED_OPTION_H
#define PROJECT1_DELTAHEDGING_TRADED_OPTION_H
#include "Option.h"

class Traded_Option : public Option {
private:
    double price;
public:
    Traded_Option();
    Traded_Option(double K, double S, double r, double T, char type,double price);
    ~Traded_Option();
    double getprice();
    double Initial_Sigma();
    double Volatility_Calculator(double sigma_i,double tolerance_level);
    double Delta_Calculator(double sigma);
};



#endif //PROJECT1_DELTAHEDGING_TRADED_OPTION_H
