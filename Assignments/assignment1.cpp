#include<iostream>
#include<fstream>
#include<string>
#include<cctype>

using namespace std;

enum relationType { LESS, GREATER, EQUAL };

bool checkName(const string& name);
bool checkPhone(const string& phone);
bool checkAddress(const string& address);

class itemType
{
private:
	string name;
	string phone;
	string address;
public:
	itemType()
	{
		name = "";
		phone = "";
		address = "";
	}
	itemType(string n, string p, string a)
	{
		name = n;
		phone = p;
		address = a;
	}
	string getName() const
	{
		return name;
	}
	string getPhone() const
	{
		return phone;
	}
	string getAddress() const
	{
		return address;
	}
	void setName(string n)
	{
		name = n;
	}
	void setPhone(string p)
	{
		phone = p;
	}
	void setAddress(string a)
	{
		address = a;
	}
	relationType compareTo(const itemType& other) const
	{
		if (name < other.name)
		{
			return LESS;
		}
		else if (name > other.name)
		{
			return GREATER;
		}
		else
		{
			return EQUAL;
		}
	}
	void display() const
	{
		cout << "Name: " << name << endl;
		cout << "Phone: " << phone << endl;
		cout << "Address: " << address << endl;
	}
};

class sortedType
{
private:
	static const int MAX = 100;
	itemType items[MAX];
	int count;
public:
	sortedType()
	{
		count = 0;
	}
	bool isFull() const
	{
		return count == MAX;
	}
	bool isEmpty() const
	{
		return count == 0;
	}
	int getCount() const
	{
		return count;
	}
	bool addContact(const itemType& item)
	{
		if (isFull())
		{
			return false;
		}
		for (int i = 0; i < count; i++)
		{
			if (items[i].compareTo(item) == EQUAL)
			{
				return false;
			}
		}
		int pos = count;
		while (pos > 0 && items[pos - 1].compareTo(item) == GREATER)
		{
			items[pos] = items[pos - 1];
			pos--;
		}
		items[pos] = item;
		count++;
		return true;
	}
	bool deleteContact(const string& name)
	{
		for (int i = 0; i < count; i++)
		{
			if (items[i].getName() == name)
			{
				for (int j = i; j < count - 1; j++)
				{
					items[j] = items[j + 1];
				}
				count--;
				return true;
			}
		}
		return false;
	}
	itemType searchContact(const string& name) const
	{
		itemType empty;
		for (int i = 0; i < count; i++)
		{
			if (items[i].getName() == name)
			{
				return items[i];
			}
		}
		return empty;
	}
	void displayAllContacts() const
	{
		if (isEmpty())
		{
			cout << "No contacts found" << endl;
			return;
		}
		cout << "----ALL CONTACTS----" << endl;
		for (int i = 0; i < count; i++)
		{
			cout << "Contact " << (i + 1) << " : " << endl;
			items[i].display();
		}
	}
	bool saveToFile(const string& fileName)
	{
		ofstream file;
		file.open(fileName, ios::out);
		if (!file)
		{
			cout << "Could not open a file" << endl;
			return false;
		}
		for (int i = 0; i < count; i++)
		{
			file << items[i].getName() << endl;
			file << items[i].getPhone() << endl;
			file << items[i].getAddress() << endl;
		}
		file.close();
		return true;
	}
	bool loadFromFile(const string& fileName)
	{
		ifstream file;
		file.open(fileName, ios::in);
		if (!file)
		{
			cout << "Could not open a file" << endl;
			return false;
		}
		count = 0;
		string name, phone, address;
		while (file.good())
		{
			if (!getline(file, name) || !getline(file, phone) || !getline(file, address))
			{
				break;
			}
			if (!isFull() && checkPhone(phone))
			{
				itemType item(name, phone, address);
				addContact(item);
			}
		}
		file.close();
		return true;
	}
};
bool checkName(const string& name)
{
	return !name.empty();
}
bool checkPhone(const string& phone)
{
	if (phone.length() != 11)
	{
		return false;
	}
	for (char c : phone)
	{
		if (!isdigit(c))
		{
			return false;
		}
	}
	return true;
}
bool checkAddress(const string& address)
{
	return !address.empty();
}
string getNameInput()
{
	string input;
	while (true)
	{
		cout << "Enter name: " << endl;
		getline(cin, input);
		if (checkName(input))
		{
			return input;
		}
		cout << "Name can not be empty" << endl;
	}
}
string getPhoneInput()
{
	string input;
	while (true)
	{
		cout << "Enter phone: " << endl;
		getline(cin, input);
		if (checkPhone(input))
		{
			return input;
		}
		cout << "Phone must contain 11 digits" << endl;
	}
}
string getAddressInput()
{
	string input;
	while (true)
	{
		cout << "Enter address: " << endl;
		getline(cin, input);
		if (checkAddress(input))
		{
			return input;
		}
		cout << "Address can not be empty" << endl;
	}
}
itemType getItemInput()
{
	cout << "----ADD NEW CONTACT----" << endl;
	string name = getNameInput();
	string phone = getPhoneInput();
	string address = getAddressInput();
	return itemType(name, phone, address);
}
string getFileNameInput()
{
	string input;
	cout << "Enter file name: " << endl;
	getline(cin, input);
	return input;
}
void displayMenu()
{
	cout << "----CONTACT MANAGER----" << endl;
	cout << "1. Add New Contact" << endl;
	cout << "2. Search Contact" << endl;
	cout << "3. Delete Contact" << endl;
	cout << "4. Display All Contacts" << endl;
	cout << "5. Save Contacts to File" << endl;
	cout << "6. Load Contacts from File" << endl;
	cout << "7. Exit" << endl;
	cout << "Enter your choice: ";
}
int main()
{
	sortedType contacts;
	int choice;
	const string filename = "contacts.txt";
	do
	{
		displayMenu();
		while (!(cin >> choice))
		{
			cout << "Invalid input. Enter a number: ";
			cin.clear();
			cin.ignore(1000, '\n');
		}
		cin.ignore();
		switch (choice)
		{
		case 1:
		{
			if (contacts.isFull())
			{
				cout << "Contact list is full" << endl;
			}
			else
			{
				itemType newContact = getItemInput();
				if (contacts.addContact(newContact))
				{
					cout << "Contact added successfully" << endl;
				}
				else
				{
					cout << "Contact with this name already exist" << endl;
				}
			}
			break;
		}
		case 2:
		{
			cout << "----SEARCH CONTACT----" << endl;
			string searchContact = getNameInput();
			itemType foundContact = contacts.searchContact(searchContact);
			if (!foundContact.getName().empty())
			{
				cout << "Contact found" << endl;
				foundContact.display();
			}
			else
			{
				cout << "Contact not found" << endl;
			}
			break;
		}
		case 3:
		{
			cout << "----DELETE CONTACT----" << endl;
			string deleteContact = getNameInput();
			if (contacts.deleteContact(deleteContact))
			{
				cout << "Contact deleted successfully" << endl;
			}
			else
			{
				cout << "Contact not found" << endl;
			}
			break;
		}
		case 4:
		{
			contacts.displayAllContacts();
			break;
		}
		case 5:
		{
			cout << "----SAVE CONTACTS----" << endl;
			if (contacts.saveToFile(filename))
			{
				cout << "Contacts saved successfully to " << filename << endl;
			}
			break;
		}
		case 6:
		{
			cout << "----LOAD CONTACTS----" << endl;
			if (contacts.loadFromFile(filename))
			{
				cout << "Contacts loaded successfully from " << filename << endl;
			}
			break;
		}
		case 7:
		{
			cout << "Thankyou for using CONTACT MANAGER" << endl;
			break;
		}
		default:
		{
			cout << "Invalid choice. Try again" << endl;
			break;
		}
		}
		if (choice != 7)
		{
			cout << "Press enter to continue..." << endl;
			cin.get();
		}
	} while (choice != 7);
	return 0;
}