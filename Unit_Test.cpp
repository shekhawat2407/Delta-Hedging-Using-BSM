//
// Created by Pradhyumn Singh on 14-10-2022.
//

#include "Unit_Test.h"
#include <iostream>
#include <cmath>
#include "Option_Price.h"
#include "Traded_Option.h"


Unit_Test::Unit_Test() {}

Unit_Test::~Unit_Test() {}

void Unit_Test::BSM_Test() {

    double stock_price = 100;
    double strike_price = 120;
    double rf_rate = 0.05;
    double t_maturity = 1;
    double sigma = 0.2;
    double option_price = 3.247;
    double option_delta = 0.287;
    char option_type='c';

    double tolerance_level=pow(10,-3);

    Option_Price call1(strike_price,stock_price,rf_rate,t_maturity,option_type,sigma);
    Traded_Option call2(strike_price,stock_price,rf_rate,t_maturity,option_type,option_price);


    double BSM_Price = call1.BSM_Pricer()[0];
    double BSM_Delta = call1.BSM_Pricer()[1];
    double initial_sig=call2.Initial_Sigma();
    double BSM_ImpliedVol=call2.Volatility_Calculator(initial_sig,tolerance_level);

    if (abs(BSM_Price - option_price) < 0.01) {
        cout << "Option price is correctly calculated" << endl;
    } else {
        cout << "Option price is incorrectly calculated" <<endl;
    }
    if (abs(BSM_Delta - option_delta) < 0.01) {
        cout << "Delta Value is correctly calculated" << endl;
    } else {
        cout << "Delta Value is incorrectly calculated" <<endl;
    }
    if (abs(BSM_ImpliedVol - sigma) < 0.01) {
        cout << "Implied Volatility is correctly calculated" << endl;
    } else {
        cout << "Implied Volatility is incorrectly calculated" <<endl;
    }
}


