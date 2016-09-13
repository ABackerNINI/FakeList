
#include "fakelist.h"
#include <iostream>

using namespace std;
using namespace nini;

int main(){
	string_builder sb;
	char *s = new char[15];
	strcpy(s, "1234567890");
	sb.assign(std::move(s),strlen(s));
	printf("%c\n", sb[1]);
	sb.append("1234567890");
	printf("%c\n", sb[5]);
	cout << sb.to_string() << endl;
	sb.insert("aacc", 4, 10);
	sb.insert("ddeeff", 6, 4);
	sb.insert("cccc", 4, 6);
	cout << sb.to_string() << endl;
	sb.print(true);
	auto it_insert = sb.begin();
	it_insert += 4;
	sb.insert("bbb", 3, it_insert);
	//for (auto it = sb.begin(); it != sb.end(); ++it)printf("%c\n", *it);
	sb.print(true);

	printf("%c %c\n", sb.front(), sb.back());

	while(!sb.empty()) {
		sb.pop_front();
		sb.print(true);
	}

	char *s1 = new char[30];
	strcpy(s1, "abcdefghijklmnopqrstuvwxyz");
	string_builder sb1(std::move(s1));
	cout << sb1.to_string() << endl;

	sb1.print(true);


	return 0;
}