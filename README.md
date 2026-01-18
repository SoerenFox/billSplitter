# billSplitter

A C program for tracking and hopefully soon settling shared expenses and calculating how much each person owes or is owed in a group.

## Features

- **Account Management**: Create accounts for multiple people
- **Expense Tracking**: Record expenses with descriptions and amounts
- **Debt Calculation**: Automatically calculate each person's debt based on shared expenses
- **Payment Settlement**: Generate payment entries to settle debts fairly
- **Persistent Storage**: Save and load expense data from `billsplitter.txt`

## How It Works

The program:
1. Creates accounts for each person in the group
2. Records individual expenses made by each person
3. Calculates the fair share by dividing total expenses among all participants
4. Determines who owes money and how much
5. Generates settlement payments to balance debts
6. Saves all transactions to a file for future reference

## Building & Running

Compile the program:
```sh
gcc billSplitter.c -o billSplitter
```

Run the program:
```sh
./billSplitter
```

## Data Format

The program stores data in `billsplitter.txt` using a tab-separated format:

```
ACCOUNT	PersonName
ENTRY	Amount	Description
ACCOUNT	AnotherPerson
ENTRY	Amount	Description
```

## License

See [LICENSE](LICENSE) for details.