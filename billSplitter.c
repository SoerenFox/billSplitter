#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define NAME_LENGTH 32
#define DESCRIPTION_LENGTH 256
#define SAFE_FILE "billsplitter.txt"

struct Entry {
    char description[DESCRIPTION_LENGTH];
    float debt;
};

struct Account {
    int accountIndex;
    char person[NAME_LENGTH];
    struct Entry* pEntries[16];
    int entryCounter;
};

struct Account* pAccounts[8]; 
int accountCounter = 0;

void createAccount(char person[]);
void addEntry(struct Account* pAccount, float value, char description[]);
struct Account* findAccount(char name[]);
float accountDebt(struct Account* pAccount);
void pay(struct Account* pAccount);

void endProgram();
void init();

char* toLowerCase(char str[]);

int main() {
    bool choice = false;
    if (choice) init();
    else {
        createAccount("Patrick");
        addEntry(findAccount("Patrick"), 40.0f, "Tank");
        createAccount("Till");
        addEntry(findAccount("Till"), 5.0f, "Parkplatz");
        createAccount("Lina");
        addEntry(findAccount("Lina"), 150.0f, "Tickets");
        createAccount("Lars");
        addEntry(findAccount("Lars"), 200.0f, "Tickets");
    }
    float debt, totalDebt = 0;
    for (int i = 0; i < accountCounter; i++) {
        debt = accountDebt(pAccounts[i]);
        if (debt < 0) totalDebt += debt;
        printf("%s's debts sum up to %.2f Euro.\n", pAccounts[i]->person, debt);
    }
    for (int i = 0; i < accountCounter; i++) pay(pAccounts[i]);
    endProgram();

    return 0;
}

void createAccount(char name[]) {
    struct Account* pAccount = (struct Account*)malloc(sizeof(struct Account));
    pAccount->entryCounter = 0;
    pAccount->accountIndex = accountCounter;
    pAccounts[accountCounter++] = pAccount;
    strcpy(pAccount->person, name);
    printf("Account for %s was created.\n", pAccount->person);
}

void addEntry(struct Account* pAccount, float value, char description[]) {
    struct Entry* pEntry = (struct Entry*)malloc(sizeof(struct Entry));
    strcpy(pEntry->description, description);
    pEntry->debt = value;
    pAccount->pEntries[pAccount->entryCounter++] = pEntry;
    printf("Entry '%s' with %.2f Euro added to %s's account.\n", pEntry->description, pEntry->debt, pAccount->person);
}

struct Account* findAccount(char name[]) {
    char lowerCase[NAME_LENGTH];
    strcpy(lowerCase, toLowerCase(name));

    for (int i = 0; i < accountCounter; i++) {
        if (strstr(toLowerCase(pAccounts[i]->person), lowerCase) != NULL) { 
            return pAccounts[i];
        }
    }
    return NULL;
}

float accountDebt(struct Account* pAccount) {
    float debt = 0, assets = 0;
    for (int i = 0; i < accountCounter; i++) {
        if (pAccounts[i] == pAccount) continue;
        for (int j = 0; j < pAccounts[i]->entryCounter; j++) {
            debt += pAccounts[i]->pEntries[j]->debt;
        }
    }
    for (int i = 0; i < pAccount->entryCounter; i++) {
        assets += pAccount->pEntries[i]->debt;
    }
    return debt / (accountCounter - 1) - assets;
}

void endProgram() {
  FILE* saveFile = fopen(SAFE_FILE, "w");
  for (int i = 0; i < accountCounter; i++) {
    fprintf(saveFile, "ACCOUNT\t%s\n", pAccounts[i]->person);
    for (int j = 0; j < pAccounts[i]->entryCounter; j++) {
        fprintf(saveFile, "ENTRY\t%.2f\t%s\n",
                pAccounts[i]->pEntries[j]->debt,
                pAccounts[i]->pEntries[j]->description);
        free(pAccounts[i]->pEntries[j]);
    }
    free(pAccounts[i]);
  }
  fclose(saveFile);
}

void init() {
  FILE* saveFile = fopen(SAFE_FILE, "r");
  char line[512], pName[NAME_LENGTH];

  while (fgets(line, sizeof(line), saveFile)) {
    char* pCell = strtok(line, "\t");
    if (pCell == NULL) continue;
    
    if (!strcmp(pCell, "ACCOUNT")) {
        pCell = strtok(NULL, "\t");
        if (pCell == NULL) continue;

        createAccount(pCell);
    } else {
        pCell = strtok(NULL, "\t");
        if (pCell == NULL) continue;
        float value = atof(pCell);

        pCell = strtok(NULL, "\t");
        if (pCell == NULL) continue;

        char description[DESCRIPTION_LENGTH];
        strcpy(description, pCell);

        addEntry(pAccounts[accountCounter - 1], value, description);
    }
  }
  fclose(saveFile);
}

void pay(struct Account* pAccount) {
    float debt = accountDebt(pAccount);

    if (debt < 0) return;

    char description[64];
    float individualDebt, totalDebt = 0;
    for (int i = 0; i < accountCounter; i++) {
        individualDebt = accountDebt(pAccounts[i]);
        if (individualDebt < 0) totalDebt += individualDebt;
    }
    for (int i = 0; i < accountCounter; i++) {
        individualDebt = accountDebt(pAccounts[i]);
        if (individualDebt < 0) {
            individualDebt = fabs(debt * individualDebt / totalDebt);
            printf("Settled debt of %.2f Euros by %s.\n", individualDebt, pAccount->person);
        }
    }
}

char* toLowerCase(char str[]) {
  static char strLow[256]; // saved in buffer (available outside of this function this way); gets overwritten at next function call
  int i = 0;
  // goes through every character in given string
  for (i; str[i] != '\0'; i++) strLow[i] = tolower((unsigned char)str[i]); // tolower() returns small letter if given char is a big letter, otherwise the char stays unchanged
  strLow[i] = '\0';
  return strLow;
}
