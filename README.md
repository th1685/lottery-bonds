# lottery-bonds
Lottery Bond simulation

A simple stochastic simulation of a lottery bond.

Takes a duration (years), an effective annual interest rate, a compounding frequency, a maximum deposit (per account), and a maximum number of accounts.
Then randomly performs the lottery (at the end of) each compounding period, tracking all balances.

Finally outputs a CSV containing time series data for all accounts.