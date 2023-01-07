//
// Created by Pradhyumn Singh on 05-10-2022.
//

#ifndef PROJECT1_DELTAHEDGING_OPTION_H
#define PROJECT1_DELTAHEDGING_OPTION_H


class Option{
private:
        double K;    // Option strike price
        double S;   // stock current price
        double r;    // risk-free rate
        double T;    // maturity time
        char type;
        void init ();

    public:
        Option() ;
        Option(double K, double S, double r, double T, char type) ;
        ~Option() ;
        double getStrikePrice();
        double getStockPrice() ;
        double getRiskFreeRate() ;
        double getMaturityTime() ;
        char getOptionType() ;
    };


#endif //PROJECT1_DELTAHEDGING_OPTION_H
