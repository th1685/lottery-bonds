# lottery-bonds
A simple stochastic simulation of a lottery bond.

- `duration` (years),
- `effective annual interest rate`,
- `compounding frequency`,
- `maximum deposit` (per account),
- `maximum number of accounts`.

Randomly performs the lottery (at the end of) each compounding period, tracking all balances.
Outputs a CSV containing time series data for all accounts.

`$ cmake --build build`
`$ ./build/lottery -maturity -effective rate -convertible -max deposit -max accounts`