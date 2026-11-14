#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

class Account {
protected:
	string id;
	double balance;
	int pin;
	vector<string> operations;
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

	void addOperation(const string& record) {
		operations.emplace_back(record);
	}

	void operationHistory() {
		cout << "Operations history:" << endl;
		for (const auto& one : operations) {
			cout << one << endl;
		}
	}

	virtual double getComission(double sum) const = 0;

	virtual double getBalance() const = 0;

	virtual bool operator==(const Account&) const = 0;

	virtual int getPin() const = 0;

	virtual string getName() const = 0;

	virtual ~Account() {}
};

class CheckingAccount : public Account {
	string name;
public:
	CheckingAccount(const string& id, double balance, int pin) : Account(id, balance, pin) {}

	string getName() const override {
		return "check";
	}

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
	string name;
public:
	SavingAccount(const string& id, double balance, int pin) : Account(id, balance, pin) {}

	string getName() const override {
		return "save";
	}

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

void ComeBack() {
	cout << "Press 'enter' to go back\n";
	cin.ignore();
	cin.get();
}

void Transfer(Account* from, Account* to) {
	double sum;
	cout << "What the sum you want to transfer: ";

	cin >> sum;

	if ((sum + from->getComission(sum)) > from->getBalance()) {
		cout << "No enough money" << endl;
		ComeBack();
		return;
	}
	else if (sum <= 0) {
		cout << "The amount has to be positive\n";
		ComeBack();
		return;
	}
	from->withdraw(sum + from->getComission(sum));

	to->deposit(sum);

	cout << "The transfer is done: " << sum << " rubles." <<
		" Comission: " << from->getComission(sum) << " rubles." << endl;

	ComeBack();

	ostringstream os;
	os << "Transfer to " << to->getName() << ' ' << sum << " rubles.";
	from->addOperation(os.str());
}

void WithdrawCash(Account* account) {
	double sum;

	if (account->getBalance() == 0) {
		cout << "No money to action" << endl;
		ComeBack();
		return;
	}

	cout << "What the amount you want to get: ";
	cin >> sum;

	while (true) {
		if (sum < 0) {
			cout << "The amount has to be positive" << endl;
			ComeBack();
			return;
		}
		else if (sum == 0) {
			cout << "The amount has not to be empty" << endl;
			ComeBack();
			return;
		}
		else if (sum > account->getBalance()) {
			cout << "No enough money" << endl;
			ComeBack();
			return;
		}
		else {
			account->withdraw(sum);
			cout << "Withdraw completed!" << endl;
			ComeBack();
			ostringstream os;
			os << "Cash withdrawal: " << sum << " rubles.";
			account->addOperation(os.str());
			return;
		}
	}

}

void ShowTheBalance(Account* account) {
	cout << "Balance: " << account->getBalance() << endl;
	ComeBack();
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
				cout << "This account was blocked!" << endl;
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
		ComeBack();
		return;
	}
	else {
		account->deposit(sum);
		cout << "Deposit was successful!\n";
		ComeBack();
	}

	ostringstream os;
	os << "Depositing cash: " << sum << " rubles.";
	account->addOperation(os.str());

	return;
}

int menu() {
	int choice;
	do {
		cout << "1-WITHDRAW CASH    ";
		cout << "2-PUT MONEY    ";
		cout << "3-TRANSFER\t";
		cout << "4-SHOW THE BALANCE\t";
		cout << "5-OPERATIONS HISTORY\t";
		cout << "6-QUIT\n";
		cout << "Your choice: ";
		cin >> choice;
		if (choice < 1 || choice > 6) cout << "Wrong choice! Try again.\n";
	} while (choice < 1 || choice > 6);
	return choice;
}

int main() {
	Account* check = new CheckingAccount("check", 30000, 1234);
	Account* save = new SavingAccount("save", 50000, 1111);

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
		case 5: if (account == "check") {
			check->operationHistory(); break;
		}
			  else if (account == "save") {
			save->operationHistory(); break;
		}
		case 6: cout << "Wait...\n";
			Quit(check, save);
			return 0;
		}
	}
}