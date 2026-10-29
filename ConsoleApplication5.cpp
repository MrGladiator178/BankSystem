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

void transfer(Account* from, Account* to, double sum) {
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

int main() {
	double amount;

	Account* check = new CheckingAccount("check", 30000);
	Account* save = new SavingAccount("save", 50000);

	cout << "Enter the amount: ";
	cin >> amount;

	transfer(check, save, amount);

	cout << "Balance: " << check->getBalance() << '\n';
	cout << "Balance: " << save->getBalance() << '\n';

	delete check;
	delete save;

	return 0;
}