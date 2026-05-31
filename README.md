# lottery-bonds
A simple stochastic simulation of a lottery bond.

Inputs:
- `duration` (years),
- `effective annual interest rate`,
- `compounding frequency`,
- `maximum deposit` (per account),
- `maximum number of accounts`.

Randomly performs the lottery (at the end of) each compounding period, tracking all balances.\
Outputs a CSV containing time series data for all accounts, and generates a graph.\

```bash
lottery-bonds/
├── main.cpp
├── CMakeLists.txt
├── README.md
├── config/
│   ├── accounts.txt
│   └── p-structure.txt
└── data-output/
    ├── graph
    └── CSV data
```
There are two configuration files. The syntax looks like\
\
qty amt\
\
for both. For the accounts,\
\
1 60  means 60 x £1,\
2 25        25 x £22,\
5 10        10 x £5,\
10 14       14 x £10.\
\
For the prizes,\
\
1 0.25   means 1 x 25% of prize fund,\
2 0.125        2 x 12.5%,\
4 0.1          4 x 10%,\
5 0.02         5 x 2%.\
\
Build with\
`$ cmake --build build`\
`$ ./build/lottery -maturity -effective rate -convertible -max deposit -max accounts`
