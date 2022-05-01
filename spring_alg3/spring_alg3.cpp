#include <iostream>
#include <string>
#include <Windows.h>
#include "Graph.h"
#include "TreeMap.h"

using namespace std;

size_t toint(string str) {
	if (str == "N/A") {
		return INF;
	}
	return stoull(str);
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	cout << "Print flights list in format:\nDepartment1;Destination1;DirectPrice1;ReversePrice1\nDepartment2;Destination2;DirectPrice2;ReversePrice2\n...\nEnd printing with Ctrl+D\n\n";
	struct Flight {
		Flight* next = nullptr;
		string departure, destination;
		size_t direct = INF, reverse = INF;
	};
	TreeMap<string, size_t>* cities = new TreeMap<string, size_t>();
	Flight* flights = new Flight;
	Flight* curr = flights;
	string str = "";
	char c;
	int part = 0; // 0=departure, 1=destination, 2=direct, 3=reverse
	while (c = cin.get()) {
		if (c == ';' || c == '\n' || c == 4) {
			switch (part % 4)
			{
			case 0: // departure
				curr->departure = str;
				try {
					cities->insert(str, cities->get_size());
				}
				catch (runtime_error e) {
					// city added already
				}
				break;
			case 1: // destination
				curr->destination = str;
				try {
					cities->insert(str, cities->get_size());
				}
				catch (runtime_error e) {
					// city added already
				}
				break;
			case 2: // direct
				curr->direct = toint(str);
				break;
			case 3: // reverse
				curr->reverse = toint(str);
				if (c != 4) {
					curr->next = new Flight;
					curr = curr->next;
				}
				break;
			}
			if (part % 4 == 3 && c == 4) { // end of list
				break;
			}
			str = "";
			part++;
		}
		else str += c;
	}

	Graph* graph = new Graph(cities->get_size());
	curr = flights;
	do {
		graph->set_edge(cities->find(curr->departure), cities->find(curr->destination), curr->direct, curr->reverse);
		curr = curr->next;
	} while (curr != nullptr);

	string start, end;
	cin.get();
	cout << "Departure city: ";
	while ((c = cin.get()) != '\n') {
		start += c;
	}
	cout << "Destionation city: ";
	while ((c = cin.get()) != '\n') {
		end += c;
	}

	cout << "\n\nPath:\n";
	Graph::Path* path = graph->find_best(cities->find(start), cities->find(end));
	Graph::Node* current = path->start;
	do {
		cout << cities->find(current->node) << '\n';
		current = current->next;
	} while (current != nullptr);
	printf("\nLength: %I64u\n", path->length);
}