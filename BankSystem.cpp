#include <iostream>
#include <windows.h>
#include <locale.h>
#include <string>
using namespace std;

class Account {
protected:
	string id;
	double balance;
public:
	Account(const string& id, double balance) : id(id), balance(balance) {}

	void deposit(double sum) {
		balance += sum;
	}

	void withdraw(double sum) {
		balance -= sum;
	}

	virtual double getComission(double sum) const = 0;

	virtual double getBalance() const = 0;

	virtual ~Account() {}
};

class CheckingAccount : public Account {
public:
	CheckingAccount(const string& id, double balance) : Account(id, balance) {}

	double getComission(double sum) const override {
		return sum * 0.01;
	}

	double getBalance() const override {
		return balance;
	}
};

class SavingAccount : public Account {
public:
	SavingAccount(const string& id, double balance) : Account(id, balance) {}

	double getComission(double sum) const override {
		return sum * 0.005;
	}

	double getBalance() const override {
		return balance;
	}
};

void Transfer(Account* from, Account* to, double sum) {
	cout << "What the sum you want to transfer: ";

	cin >> sum;

	if ((sum + from->getComission(sum)) > from->getBalance()) {
		cout << "No enough money\n";
		return;
	}
	else if (sum <= 0) {
		cout << "The amount has to be positive\n";
		return;
	}
	from->withdraw(sum + from->getComission(sum));

	to->deposit(sum);

	cout << "The transfer is done: " << sum << " rubles." <<
		" Comission: " << from->getComission(sum) << " rubles.\n";
}

int menu() {
	int choice;
	do {
		cout << "1-WITHDRAW CASH    ";
		cout << "2-TRANSFER\t";
		cout << "3-SHOW THE BALANCE\t";
		cout << "4-QUIT\n";
		cout << "Your choice: ";
		cin >> choice;
		if (choice < 1 || choice > 4) cout << "Wrong choice! Try again.\n";
	} while (choice < 1 || choice > 4);
	return choice;
}

void WithdrawCash(Account* from, Account* to, double sum) {
	string choice;

	cout << "What the amount you want to get: ";
	cin >> sum;

	cout << "From which account you want to take money: ";
	cin >> choice;

	if (choice == "check") {
		if (sum > from->getBalance()) {
			cout << "No enough money\n";
			return;
		}
		else if (sum == 0) {
			cout << "The amount has not to be empty\n";
			return;
		}
		else if (sum < 0) {
			cout << "The amount has to be positive\n";
			return;
		}
		else {
			from->withdraw(sum);
			cout << "Withdraw completed\n";
		}
	}

	if (choice == "save") {
		if (sum > to->getBalance()) {
			cout << "No enough money\n";
			return;
		}
		else if (sum == 0) {
			cout << "The amount has not to be empty\n";
			return;
		}
		else if (sum < 0) {
			cout << "The amount has to be positive\n";
			return;
		}
		else {
			to->withdraw(sum);
			cout << "Withdraw completed\n";
		}
	}

	else {
		cout << "Wrong account! Try again: ";
		cin >> choice;
	}

	
}

void ShowTheBalance(Account* check, Account* save) {
	string choice;
	cout << "Which the account balance you want to see: ";
	cin >> choice;

	if (choice == "check") {
		cout << "Balance: " << check->getBalance() << '\n';
		return;
	}
	else if (choice == "save") {
		cout << "Balance: " << save->getBalance() << '\n';
		return;
	}
	else {
		cout << "Wrong account! Try again: ";
		cin >> choice;
	}
}

void Quit(Account* check, Account* save) {
	delete check;
	delete save;
}

int main() {
	double amount = 0;

	Account* check = new CheckingAccount("check", 30000);
	Account* save = new SavingAccount("save", 50000);

	while (true) {
		int choice = menu();
		switch (choice) {
		case 1: WithdrawCash(check, save, amount); break;
		case 2: Transfer(check, save, amount); break;
		case 3: ShowTheBalance(check, save); break;
		case 4: cout << "Wait...\n";
			Quit(check, save);
			return 0;
		}
	}
}