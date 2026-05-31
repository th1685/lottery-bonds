# lottery-bonds
A simple stochastic simulation of a lottery bond.

Inputs:
- `(double) duration` in years,
- `(double) effective annual interest rate` percentage as decimal,
- `(int) compounding frequency` times per yeat,
- `(bool) random` randomise initial account layout (otherwise read from config)
  - `(double) maximum deposit per account`,
  - `(int) maximum number of accounts`.

Randomly performs the lottery (at the end of) each compounding period, tracking all balances.\
Outputs a CSV containing time series data for all accounts, and generates a graph.
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
$ ./build/lottery -maturity -effective rate -convertible -random ? (-max deposit -max accounts :)
```
Example output for `lottery 10 12 0.043 false` :
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
  100.00,   145.89,   100.00,   182.82,   100.00,   100.00,   194.06,   275.11,   137.70,   100.00, 
  100.00,   100.00,   100.00,   223.46,   100.00,   100.00,   100.00,   140.58,   100.00,   178.58, 
  100.00,   144.93,   198.44,   100.00,   152.25,   100.00,   100.00,   100.00,   360.10,   100.00, 
  200.75,   186.34,   210.63,   253.45,   100.00,   136.92,   100.00,   100.00,   100.00,   138.10, 
  262.61,   193.75,   235.74,   100.00,   188.32,   183.63,   241.40,   237.33,   143.84,   188.00, 
  100.00,   139.05,   100.00,   100.00,   100.00,   141.45,   100.00,   229.34,   187.72,   189.00, 
  244.00,   140.44,   140.02,   137.05,   363.58,   137.57,   100.00,   186.32,   233.77,   152.43, 
  150.98,   100.00,   100.00,   100.00,   100.00,   228.64,   100.00,   100.00,   138.91,   226.30, 
  150.09,   100.00,   151.52,   151.34,   193.67,   140.73,   233.06,   224.53,   149.22,   100.00, 
  193.25,   141.30,   100.00,   223.09,   100.00,   194.26,   100.00,   151.16,   136.53,   100.00, 

median increase: 40.44%
average (annual) increase: 52.35% (4.88%)
  book: 52.35%
standard deviation: 58.56%
maximum amount: 363.58
minimum amount: 100.00

balances data written to ./data-output/balances2026-05-31 T204742.csv
chart saved to ./data-output/graph2026-05-31 T204742.jpg
```
<img width="560" height="420" alt="graph2026-05-31 T201851" src="https://github.com/user-attachments/assets/0101f5d0-336a-4d28-943b-5b7158e7f338" />