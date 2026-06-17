# lottery-bonds

[Latest release — v0.1](https://github.com/th1685/lottery-bonds/releases)

A simple stochastic simulation of a lottery bond.

Inputs:
- `(double) duration` in years,
- `(double) effective annual interest rate` percentage as decimal,
- `(int) compounding frequency` times per year,
- `(int) batches` number of simulation runs,
- `(bool) random` randomise initial account layout (otherwise read from config),
  - `(double) maximum deposit per account`,
  - `(int) maximum number of accounts`.

Randomly performs the lottery (at the end of) each compounding period, tracking all balances.\
Outputs a CSV containing time series data for all accounts and generates a graph.
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
There are two configuration files. The syntax looks like
```bash
qty amt
```
for both. For the accounts,
```bash
1 60  means 60 x £1,
2 25        25 x £22,
5 10        10 x £5,
10 14       14 x £10.
```
For the prizes,
```bash
1 0.25   means 1 x 25% of prize fund,
2 0.125        2 x 12.5%,
4 0.1          4 x 10%,
5 0.02         5 x 2%.
```
Build & run with
```bash
$ cmake --build build
$ ./build/lottery -maturity -convertible -effectiverate\
                  -batchsize -random ? (-maxdeposit -maxaccounts : null)
```
Example output for `lottery 10 12 0.043 1 false` :
```bash
prizes are initialised correctly.
annual effective rate: 4.30%
  convertible 12 times per year for 10 years
nominal rate: ~0.35%

total prize draws: 120
accounts quantity: 100

initial book volume: £10000.00
final book volume: £15235.02
  matches simple interest: £15235.02

final balances:
  188.10,   135.64, ... 311.42,   148.53, 

median (annual) increase: 41.74% (4.56%)
average (annual) increase: 52.35% (4.88%)
  book: 52.35%
standard deviation: 55.90%
maximum amount: 311.42
minimum amount: 100.00

balances data written to ./data-output/balances2026-05-31 T205117.csv
chart saved to ./data-output/graph2026-05-31 T205117.jpg
```
<img width="560" height="420" alt="graph2026-05-31 T205117" src="https://github.com/user-attachments/assets/ede1cdb5-8cb9-4be2-bb35-7cb5141a544b" />
