#include <iostream>
#include <sstream>
#include <string>

#include <iostream>

using namespace std;

class Node
{
public:
    friend class LinkedList;

    // constructor
    Node(string data, int address);

private:
    string data;
    int address;
    Node *next;
};

Node::Node(string data, int address)
{
    this->data = data;
    this->address = address;
}

class LinkedList
{
public:
    // constructor
    LinkedList();

    // member function
    void insert(string data, int address);

    int search(string data);

private:
    Node *sentinel;
};

LinkedList::LinkedList()
{
    this->sentinel = new Node("$", -1);
}

void LinkedList::insert(string data, int address)
{
    // start from the sentinel node,
    // traverse to the right spot
    Node *p = this->sentinel;
    Node *tmp = new Node(data, address);
    // loop through everything;
    // if meet the same node and fence is 1, override anyway;
    // find the right spot and insert
    while (p->next != NULL)
    {

        // if p->next->address is bingo, override
        if (p->next->address == address)
        {
            p->next->data = data;
            return;
        }

        // else if p->next->address is larger, insert before it
        else if (p->next->address > address)
        {
            tmp->next = p->next;
            p->next = tmp;
            return;

            // else continue
        }
        else
        {
            p = p->next;
        }
    }

    // if meet null, insert to the last
    p->next = tmp;
}

int LinkedList::search(string data)
{
    Node *p = this->sentinel;

    while (p->next != NULL)
    {
        if (p->next->data == data)
        {
            return p->next->address;
        }
        else
        {
            p = p->next;
        }
    }

    return -1;
}


int symbolTable()
{
	string data;
	int address;
	string data_to_find;
	LinkedList *storage = new LinkedList();

	// get input
	cout << "Start Input" << endl;
	string input;
	getline(cin, input);
	stringstream s(input);
	while (s >> data >> hex >> address)
	{
		storage->insert(data, address);
	}
	cout << "End Input" << endl;

	cout << "Search" << endl;

	cin >> data_to_find;
	int result = storage->search(data_to_find);
	if (result > 0)
	{
		cout << hex << result << endl;
	}
	else
	{
		cout << "Not found" << endl;
	}

	return 0;
}
