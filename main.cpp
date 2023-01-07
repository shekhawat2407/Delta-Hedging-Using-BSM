#include <iostream>
#include <vector>
#include <random>
#include "option.h"
#include "Option_Price.h"
#include "Traded_Option.h"
#include "Unit_Test.h"
#include <fstream>
#include <cmath>

using namespace std;
default_random_engine generator;

void calculate_simulated_result();
void calculate_market_result(string start, string end, string maturity,string strike );
pair <vector<string>,vector<double>> read_file(string filename, string strike,string start,string end,string maturity,bool is_rate);
pair <vector<double>,vector<double>> calculate_error(vector<double> delta,vector<double> stock, vector <double> call,vector <double> rate,double delta_t);
double Date_diff(string date1, string date2);


int main() {
    calculate_simulated_result();
    cout<<"Excel Files for Part I successfully generated"<<endl;
    calculate_market_result("2011-07-05","2011-07-29","2011-09-17","500");
    cout<<endl;
    cout<<"Excel File for Part II successfully generated"<<endl;
    cout<<endl;
    Unit_Test test;
    test.BSM_Test();
    return 0;
}

void calculate_simulated_result(){
    double S0 =100;
    double T=0.4;
    double mue=0.05;
    double sigma=0.24;
    double r=0.025;
    double strike=105;
    int N=100;

    int simulations=1000;
    double delta_T = T/N;

    normal_distribution<double> distribution(0.0, 1.0);
    double randomNumber = distribution(generator);

    vector<double> stock_path(N+1,0);
    vector<double> call_path(N+1,0);
    vector<double> delta_path(N+1,0);
    vector<double> hedging_error_path(N+1,0);
    vector<double> rf_path(N+1,r);

    vector<vector<double>> stock_lattice;
    vector<vector<double>> call_lattice;
    vector<vector<double>> delta_lattice;
    vector<vector<double>> error_lattice;

    for (int j = 0; j < simulations; j++)
    {
        for (int i = 0; i < N+1; i++){
            if (i==0){
                stock_path[i]=S0;
            }else{
                stock_path[i]=stock_path[i-1]*(1+mue*delta_T+sigma*distribution(generator)*sqrt(delta_T));
            }
            Option_Price call_sim(strike, stock_path[i], r,T-delta_T*i, 'C', sigma);
            call_path[i]=call_sim.BSM_Pricer()[0];
            delta_path[i]=call_sim.BSM_Pricer()[1];
        }
        hedging_error_path=calculate_error(delta_path,stock_path,call_path,rf_path,delta_T).second;
        stock_lattice.push_back(stock_path);
        call_lattice.push_back(call_path);
        error_lattice.push_back(hedging_error_path);
    }

    //Creating the CSV file for Stock Price:
    ofstream stock_output;
    stock_output.open("../stock_path_simulation.csv");
    stock_output<<", ";
    for (int i = 0; i < N+1; i++){
        stock_output<<"t="<<T-delta_T*(N-i);
        stock_output<<", ";
    }
    stock_output<<endl;
    for (int i = 0; i < simulations; i++){
        stock_output<<"Stock Price for path "<<i+1<<", ";
        for (int j = 0; j < N+1; j++) {
            stock_output << stock_lattice[i][j] << ",";
        }
        stock_output<<endl;
    }
    stock_output.close();

    //Creating the CSV file for Call Price:
    ofstream call_output;
    call_output.open("../call_path_simulation.csv");
    call_output<<", ";
    for (int i = 0; i < N+1; i++){
        call_output<<"t="<<T-delta_T*(N-i);
        call_output<<", ";
    }
    call_output<<endl;
    for (int i = 0; i < simulations; i++){
        call_output<<"Call Price for path "<<i+1<<", ";
        for (int j = 0; j < N+1; j++) {
            call_output << call_lattice[i][j] << ",";
        }
        call_output<<endl;
    }
    call_output.close();

    //Creating the CSV file for Hedging Error:
    ofstream error_output;
    error_output.open("../hedging_error_simulation.csv");
    error_output<<", ";
    for (int i = 0; i < N+1; i++){
        error_output<<"t="<<T-delta_T*(N-i);
        error_output<<", ";
    }
    error_output<<endl;
    for (int i = 0; i < simulations; i++){
        error_output<<"Cumulative Hedging Error for path "<<i+1<<", ";
        for (int j = 0; j < N+1; j++) {
            error_output << error_lattice[i][j] << ",";
        }
        error_output<<endl;
    }
    error_output.close();
}

void calculate_market_result(string start, string end, string maturity,string strike) {
    double epsilon=pow(10,-3); // Newton's method tolerance level
    double delta_t=1.0/252.0;

    vector<string> date_v;
    vector<double> stock_v,option_price_v,interest_rate_v, implied_vol_v, delta_v,hedge_error_v, PNL_v, Hedged_PNL_v;

    date_v= read_file("../sec_GOOG.csv","NA",start,end,maturity,false).first;
    stock_v= read_file("../sec_GOOG.csv","NA",start,end,maturity,false).second;
    interest_rate_v= read_file("../interest.csv","NA",start,end,maturity,true).second;
    option_price_v= read_file("../op_GOOG.csv",strike,start,end,maturity,false).second;

    for (int i = 0; i < date_v.size(); i++){
        Traded_Option call(stod(strike),stock_v[i],interest_rate_v[i], Date_diff (date_v[i],maturity),'c',option_price_v[i]);
        double Implied_Vol=call.Volatility_Calculator(call.Initial_Sigma(),epsilon);
        implied_vol_v.push_back(Implied_Vol);
        delta_v.push_back(call.Delta_Calculator(Implied_Vol));
        PNL_v.push_back(option_price_v[0]-option_price_v[i]);
        }
    hedge_error_v=calculate_error(delta_v,stock_v,option_price_v,interest_rate_v,delta_t).first;
    Hedged_PNL_v=calculate_error(delta_v,stock_v,option_price_v,interest_rate_v,delta_t).second;

    //Creating the CSV file:

    ofstream fout;
    fout.open("../results.csv");
    fout<<"Date" << ", "<<"Stock Price" << ", "<<"Option Price" << ", "<<"Implied Volatility" << ", "<<"Delta" << ", ";
    fout<<"Hedging Error" << ", "<<"PNL" << ", "<<"PNL(with hedge)"<<endl;

    for (int i = 0; i < date_v.size(); i++) {
        fout<<date_v[i]<<", ";
        fout<<stock_v[i]<<", ";
        fout<<option_price_v[i]<<", ";
        fout<<implied_vol_v[i]<<", ";
        fout<<delta_v[i]<<", ";
        fout<<hedge_error_v[i]<<", ";
        fout<<PNL_v[i]<<", ";
        fout<<Hedged_PNL_v[i]<<endl;
    }
}

pair <vector<string>,vector<double>> read_file(string filename, string strike,string start,string end,string maturity,bool is_rate) {
    pair<vector<string>, vector<double>> required_col;
    vector<string> date_col;
    vector<double> field_col;
    ifstream data_file(filename);
    string l;
    getline(data_file, l); //skip header
    while (data_file.good()) {
        if (strike == "NA") {
            string date;
            string field;
            getline(data_file, date, ',');
            getline(data_file, field, '\n');
            if (Date_diff(start, date) >= 0 && Date_diff(date, end) >= 0) {
                date_col.push_back(date);
                if (is_rate){
                    field_col.push_back(stod(field)/100);
                }
                else{
                field_col.push_back(stod(field));}
            }
        } else {
            string date;
            string exdate;
            string cp_flag;
            string strike_price;
            string best_bid;
            string best_offer;

            getline(data_file, date, ',');
            getline(data_file, exdate, ',');
            getline(data_file, cp_flag, ',');
            getline(data_file, strike_price, ',');
            getline(data_file, best_bid, ',');
            getline(data_file, best_offer, '\n');

            if (Date_diff(start, date) >= 0 && Date_diff(date, end) >= 0 && cp_flag == "C" &&
                strike_price == strike && exdate==maturity) {
                date_col.push_back(date);
                field_col.push_back((stod(best_bid) + stod(best_offer)) / 2);
            }
        }
    }
    required_col.first=date_col;
    required_col.second=field_col;
    return required_col;
}

pair <vector<double>,vector<double>> calculate_error(vector<double> delta,vector<double> stock, vector <double> call,vector <double> rate,double delta_t){

    pair <vector<double>,vector<double>> errors;
    vector<double> hedging_error;
    vector<double> cum_hedging_error;
    double b=call[0]-delta[0]*stock[0];
    //double delta_t= 1.0/252.0;
    for(int i=0;i<stock.size();i++){
        if (i==0) {
            hedging_error.push_back(0);
            cum_hedging_error.push_back(0);
        }
        else{
            hedging_error.push_back(delta[i-1]*stock[i]-call[i]+(call[i-1]-delta[i-1]*stock[i-1])*exp(rate[i-1]*delta_t));
            cum_hedging_error.push_back(delta[i-1]*stock[i]+b*exp(rate[i-1]*delta_t)-call[i]);
            b=delta[i-1]*stock[i]+b*exp(rate[i-1]*delta_t)-delta[i]*stock[i];
            //cum_hedging_error.push_back(cum_hedging_error[i-1]+hedging_error[i]);
        }
    }
    errors.first=hedging_error;
    errors.second=cum_hedging_error;
    return errors;
}

tm ttmd(int day, int month, int year)
{
    tm tm = {0};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    return tm;
}

double Date_diff (string date1, string date2) {
    string d1 = date1.substr(8, 2);
    string m1 = date1.substr(5, 2);
    string y1 = date1.substr(0, 4);
    string d2 = date2.substr(8, 2);
    string m2 = date2.substr(5, 2);
    string y2 = date2.substr(0, 4);

    tm tm1 = ttmd(stoi(d1), stoi(m1), stoi(y1));
    tm tm2 = ttmd(stoi(d2), stoi(m2), stoi(y2));

    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);
    return difftime(t2, t1) / (365*60*60*24);
}