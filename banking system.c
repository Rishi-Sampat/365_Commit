#include <stdio.h>
#include <string.h>
#include <time.h>

// Data structure to store transaction details
typedef struct
{
    char date[11];
    char time[6];
    double amount;
    char type[10]; // "credit" or "debit"
} Transaction;

typedef struct
{
    int accountID;
    char name[50];
    char password[20];
    double balance;
    Transaction transactions[100];
    int transactionCount;
} Account;

Account accounts[100];
int accountCount = 0;

void createAccount()
{
    printf("Enter Account ID: ");
    scanf("%d", &accounts[accountCount].accountID);
    printf("Enter Account Holder's Name: ");
    scanf("%s", accounts[accountCount].name);
    printf("Set Password: ");
    scanf("%s", accounts[accountCount].password);
    accounts[accountCount].balance = 0;
    accounts[accountCount].transactionCount = 0;
    accountCount++;
    printf("Account Created Successfully!\n");
}

Account *authenticate(int accountID, char *password)
{
    for (int i = 0; i < accountCount; i++)
    {
        if (accounts[i].accountID == accountID && strcmp(accounts[i].password, password) == 0)
        {
            return &accounts[i];
        }
    }
    return NULL;
}

void deposit(Account *account, double amount)
{
    account->balance += amount;
    Transaction t;
    time_t now = time(NULL);
    struct tm *tm_struct = localtime(&now);
    strftime(t.date, sizeof(t.date), "%Y-%m-%d", tm_struct);
    strftime(t.time, sizeof(t.time), "%H:%M", tm_struct);
    t.amount = amount;
    strcpy(t.type, "credit");
    account->transactions[account->transactionCount++] = t;
    printf("Deposit successful! New Balance: %.2f\n", account->balance);
}

void withdraw(Account *account, double amount)
{
    if (amount > account->balance)
    {
        printf("Insufficient balance!\n");
    }
    else
    {
        account->balance -= amount;
        Transaction t;
        time_t now = time(NULL);
        struct tm *tm_struct = localtime(&now);
        strftime(t.date, sizeof(t.date), "%Y-%m-%d", tm_struct);
        strftime(t.time, sizeof(t.time), "%H:%M", tm_struct);
        t.amount = amount;
        strcpy(t.type, "debit");
        account->transactions[account->transactionCount++] = t;
        printf("Withdrawal successful! New Balance: %.2f\n", account->balance);
    }
}

void transfer(Account *sender, int receiverID, double amount)
{
    Account *receiver = NULL;
    for (int i = 0; i < accountCount; i++)
    {
        if (accounts[i].accountID == receiverID)
        {
            receiver = &accounts[i];
            break;
        }
    }
    if (receiver == NULL)
    {
        printf("Receiver account not found!\n");
    }
    else if (sender->balance < amount)
    {
        printf("Insufficient balance!\n");
    }
    else
    {
        sender->balance -= amount;
        receiver->balance += amount;

        Transaction t1 = {0}, t2 = {0};
        time_t now = time(NULL);
        struct tm *tm_struct = localtime(&now);
        strftime(t1.date, sizeof(t1.date), "%Y-%m-%d", tm_struct);
        strftime(t1.time, sizeof(t1.time), "%H:%M", tm_struct);
        t1.amount = amount;
        strcpy(t1.type, "debit");
        sender->transactions[sender->transactionCount++] = t1;

        t2 = t1;
        strcpy(t2.type, "credit");
        receiver->transactions[receiver->transactionCount++] = t2;

        printf("Transfer successful! New Balance: %.2f\n", sender->balance);
    }
}

void printStatement(Account *account)
{
    printf("Transaction Statement for Account ID: %d\n", account->accountID);
    printf("Date       | Time  | Type   | Amount\n");
    printf("------------------------------------\n");
    for (int i = 0; i < account->transactionCount; i++)
    {
        printf("%s | %s | %-6s | %.2f\n",
               account->transactions[i].date,
               account->transactions[i].time,
               account->transactions[i].type,
               account->transactions[i].amount);
    }
    printf("Current Balance: %.2f\n", account->balance);
}

int main()
{
    int choice, accountID;
    char password[20];
    Account *loggedInAccount = NULL;

    while (1)
    {
        printf("\n--- Banking System ---\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Deposit\n");
        printf("4. Withdraw\n");
        printf("5. Transfer Money\n");
        printf("6. Print Statement\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            createAccount();
            break;
        case 2:
            printf("Enter Account ID: ");
            scanf("%d", &accountID);
            printf("Enter Password: ");
            scanf("%s", password);
            loggedInAccount = authenticate(accountID, password);
            if (loggedInAccount)
            {
                printf("Login Successful!\n");
            }
            else
            {
                printf("Invalid credentials!\n");
            }
            break;
        case 3:
            if (loggedInAccount)
            {
                double amount;
                printf("Enter amount to deposit: ");
                scanf("%lf", &amount);
                deposit(loggedInAccount, amount);
            }
            else
            {
                printf("Please login first.\n");
            }
            break;
        case 4:
            if (loggedInAccount)
            {
                double amount;
                printf("Enter amount to withdraw: ");
                scanf("%lf", &amount);
                withdraw(loggedInAccount, amount);
            }
            else
            {
                printf("Please login first.\n");
            }
            break;
        case 5:
            if (loggedInAccount)
            {
                int receiverID;
                double amount;
                printf("Enter receiver Account ID: ");
                scanf("%d", &receiverID);
                printf("Enter amount to transfer: ");
                scanf("%lf", &amount);
                transfer(loggedInAccount, receiverID, amount);
            }
            else
            {
                printf("Please login first.\n");
            }
            break;
        case 6:
            if (loggedInAccount)
            {
                printStatement(loggedInAccount);
            }
            else
            {
                printf("Please login first.\n");
            }
            break;
        case 7:
            printf("Exiting...\n");
            return 0;
        default:
            printf("Invalid choice!\n");
        }
    }
    return 0;
}
