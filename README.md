# Delta-hedging-using-BSM

## Objective
The objective of the project is to test the delta hedging strategy employed using the Black Scholes pricing model for the Call/Put Option. 
Part I of the project caters to the performance of the strategy with the simulated paths of the stock price at constant volatility and risk-free rate. While Part II evaluates the strategy on the market observed option prices and thereby derived implied volatility.
Appendixes A & B discuss in detail the implementation of the work in C++ programming language and unit test results, respectively.

 
## File Info

### Main.cpp
Main.cpp files contain the main function that calls the calculate_simulated_result & calculate_market_result functions utilized for part I & part II respectively. Other key functions implemented are as follows,

#### calculate_simulated_result(): 
This function sets the initial parameters provided in the part I and generate a simulated stock price path (random normal distribution). Now for this path, option prices & delta values are calculated for every point in the path. This is done by creating an object of Option_Price class. Once corresponding values over a path are calculated, the final step calls in the calculate_error function to return cumulative hedging error. This process is repeated for 1000 times and the corresponding 2d lattices are exported to excel files (“stock_path_simulation.csv”, “call_path_simulation.csv”, “hedging_error_simulation.csv”)

#### calculate_market_result(a,b,c,d)
This function takes in start date, end date, maturity date and strike price as input parameters and calls in read file function to create vector of stock price, call price, rates & relevant dates. In the next step, for each date, a Traded Option class object is created, and its functions are called to get implied volatility and thereby the delta from BSM model. Finally, these vectors are passed on to calculate error function to generate Hedged_PNL vector. PNL without hedge is calculated using the call price vector. All these vectors are then exported to excel file “results.csv”.

#### read_file(a,b,c,d,e)
This function takes in input for the filename, strike price, start date, end date and a Boolean variable (to check if rate file is being read). The output from this function is either vector of stock prices, relevant dates, risk free rates or option prices (based on condition set on maturity date and strike price).

#### calculate_error(a,b,c,d,e)
This function takes the vector of values for stock price, delta, call price, risk free rates and ∆t double to return the pair of vector for  hedging error on each trading day & cumulative hedging error.

#### Date_diff(a,b)
his function takes in two dates in string and gives the output as difference in years between those two dates. This function is mainly used to calculate the time to maturity & set out conditions to read the related values from excel for the relevant dates.

### Option (CPP & Header file)
This class is used to create an option object with private parameters (Stock price-S, risk free rate-r, Time to maturity-T, Strike price -K and option type-type)
The only functions implemented in this class are getter functions for the private variables.


### Option_Price (CPP & Header file)
This class inherits the ‘Option’ class and adds an additional private variable for volatility- sigma
This class implements a getter function for the private variable & below function:
#### BSM_Pricer()
This function return pair of output – Option price & Delta for the class object created.
 

### Traded_Option (CPP & Header file)
This class inherits the ‘Option’ class and adds an additional private variable for Market Option Price-price
This class implements a getter function for the private variable & below functions:
#### Initial_Sigma()
This function initializes the sigma such that newton Raphson method converges with guarantee. For Black Scholes equation such σ0 can be obtained as their exists a unique value for which in interval (0, σ0], there is a root as f’ (σ)<0 and f’’(σ)<0 in the interval.
#### Volatility_Calculator(a,b)
This function takes in initial value of sigma & provided tolerance level (10-3) to return the Implied volatility for the class object. Note: The function is recursively called until the stated precision level is reached.
#### Delta_Calculator(a)
This function takes in the calculated implied volatility & calculate the delta for the class object.
