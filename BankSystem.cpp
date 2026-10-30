#include <iostream>
#include <windows.h>
#include <locale.h>
#include <string>
using namespace std;

class Account {
protected:
	string id;
	double balance;
	int pin;
public:
	Account(const string& id, double balance, int pin) : id(id), balance(balance), pin(pin) {}

	void deposit(double sum) {
		balance += sum;
	}

	void withdraw(double sum) {
		if (sum > balance) {
			cout << "Error: this sum is more than balance!\n";
			return;
		}
		balance -= sum;
	}

	virtual double getComission(double sum) const = 0;

	virtual double getBalance() const = 0;

	virtual bool operator==(const Account&) const = 0;

	virtual int getPin() const = 0;

	virtual ~Account() {}
};

class CheckingAccount : public Account {
public:
	CheckingAccount(const string& id, double balance, int pin) : Account(id, balance, pin) {}

	double getComission(double sum) const override {
		return sum * 0.01;
	}

	double getBalance() const override {
		return balance;
	}

	int getPin() const override {
		return pin;
	}

	bool operator==(const Account& right) const override {
		return this->pin == right.getPin();
	}
};

class SavingAccount : public Account {
public:
	SavingAccount(const string& id, double balance, int pin) : Account(id, balance, pin) {}

	double getComission(double sum) const override {
		return sum * 0.005;
	}

	double getBalance() const override {
		return balance;
	}

	int getPin() const override {
		return pin;
	}

	bool operator==(const Account& right) const override {
		return this->pin == right.getPin();
	}
};

void Transfer(Account* from, Account* to) {
	double sum;
	cout << "What the sum you want to transfer: ";

	cin >> sum;

	if ((sum + from->getComission(sum)) > from->getBalance()) {
		cout << "No enough money\n";
		cout << "Press 'enter' to go back\n";
		cin.ignore();
		cin.get();
		return;
	}
	else if (sum <= 0) {
		cout << "The amount has to be positive\n";
		cout << "Press 'enter' to go back\n";
		cin.ignore();
		cin.get();
		return;
	}
	from->withdraw(sum + from->getComission(sum));

	to->deposit(sum);

	cout << "The transfer is done: " << sum << " rubles." <<
		" Comission: " << from->getComission(sum) << " rubles.\n";
	cout << "Press 'enter' to go back\n";
	cin.ignore();
	cin.get();
}

void WithdrawCash(Account* account) {
	double sum;

	cout << "What the amount you want to get: ";
	cin >> sum;

	while (true) {
		if (sum < 0) {
			cout << "The amount has to be positive\n";
			cout << "Press 'enter' to go back\n";
			cin.ignore();
			cin.get();
			return;
		}
		else if (sum == 0) {
			cout << "The amount has not to be empty\n";
			cout << "Press 'enter' to go back\n";
			cin.ignore();
			cin.get();
			return;
		}
		else if (sum > account->getBalance()) {
			cout << "No enough money\n";
			cout << "Press 'enter' to go back\n";
			cin.ignore();
			cin.get();
			return;
		}
		else {
			account->withdraw(sum);
			cout << "Withdraw completed!\n";
			cout << "Press 'enter' to go back\n";
			cin.ignore();
			cin.get();
			return;
		}
	}
}

void ShowTheBalance(Account* account) {
	cout << "Balance: " << account->getBalance() << endl;
	cout << "Press 'enter' to go back\n";
	cin.ignore();
	cin.get();
}

void Quit(Account* check, Account* save) {
	delete check;
	delete save;
}

bool PinCheck(Account* acc, int attempt) {
	int count = 0;
	while (true) {
		cout << "Enter a pin: ";
		cin >> attempt;
		if (acc->getPin() == attempt) return true;
		else {
			count++;
			if (count >= 3) {
				cout << "This account was blocked!\n";
				return false;
			}
			cout << "Wrong pin-code! Try again. ";
		}
	}
}

void PutMoney(Account* account) {
	double sum;
	cout << "How much money you want to put: ";
	cin >> sum;

	if (sum <= 0) {
		cout << "The amount has to be positive\n";
		cout << "Press 'enter' to go back\n";
		cin.ignore();
		cin.get();
		return;
	}
	else {
		account->deposit(sum);
		cout << "Deposit was successful!\n";
		cout << "Press 'enter' to go back\n";
		cin.ignore();
		cin.get();
		return;
	}
}

int menu() {
	int choice;
	do {
		cout << "1-WITHDRAW CASH    ";
		cout << "2-PUT MONEY    ";
		cout << "3-TRANSFER\t";
		cout << "4-SHOW THE BALANCE\t";
		cout << "5-QUIT\n";
		cout << "Your choice: ";
		cin >> choice;
		if (choice < 1 || choice > 5) cout << "Wrong choice! Try again.\n";
	} while (choice < 1 || choice > 5);
	return choice;
}

int main() {
	Account* check = new CheckingAccount("check", 30000, 8108);
	Account* save = new SavingAccount("save", 50000, 7777);

	string account;
	int pin = 0;
	cout << "With what account you want to work? ";
	cin >> account;
	cout << endl;

	bool access = false;

	if (account == "check") {
		access = PinCheck(check, pin);
	}
	else if (account == "save") {
		access = PinCheck(save, pin);
	}
	if (!access) {
		cout << "Access denied! We called a police!\n";
		delete check;
		delete save;
		return 0;
	}

	while (true) {
		int choice = menu();
		switch (choice) {
		case 1: if (account == "check") {
			WithdrawCash(check); break;
		}
			  else if (account == "save") {
			WithdrawCash(save); break;
		}
		case 2: if (account == "check") {
			PutMoney(check); break;
		}
			  else if (account == "save") {
			PutMoney(save); break;
		}
		case 3: if (account == "check") {
			Transfer(check, save); break;
		}
			  else if (account == "save") {
			Transfer(save, check); break;
		}
		case 4: if (account == "check") {
			ShowTheBalance(check); break;
		}
			  else if (account == "save") {
			ShowTheBalance(save); break;
		}
		case 5: cout << "Wait...\n";
			Quit(check, save);
			return 0;
		}
	}
}