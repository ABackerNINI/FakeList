
#include "fakelist.h"
#include <iostream>

using namespace std;

int main(){
	string_builder sb;
	char *s = new char[15];
	strcpy(s, "1234567890");
	sb.assign(std::move(s),strlen(s));
	printf("%c\n", sb[1]);
	sb.append("1234567890");
	printf("%c\n", sb[5]);
	cout << sb.toString() << endl;

	char *s1 = new char[30];
	strcpy(s1, "abcdefghijklmnopqrstuvwxyz");
	string_builder sb1(std::move(s1));
	cout << sb1.toString() << endl;
	string_builder::iterator it;
	it = sb.begin();
	for (; it != sb.end(); ++it)printf("%c\n", *it);
	sb1.print();

	return 0;
}