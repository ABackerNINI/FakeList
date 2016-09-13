
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
	cout << sb.toString() << endl;
	sb.insert("aacc", 4, 10);
	sb.insert("ddeeff", 6, 4);
	sb.insert("cccc", 4, 6);
	cout << sb.toString() << endl;
	sb.print(true);
	auto it_insert = sb.begin();
	it_insert += 4;
	sb.insert("bbb", 3, it_insert);
	//for (auto it = sb.begin(); it != sb.end(); ++it)printf("%c\n", *it);
	sb.print(true);

	char *s1 = new char[30];
	strcpy(s1, "abcdefghijklmnopqrstuvwxyz");
	string_builder sb1(std::move(s1));
	cout << sb1.toString() << endl;

	sb1.print(true);

	printf("%c %c, %c %c\n", sb.front(), sb.back(), sb1.front(), sb1.back());

	return 0;
}
//
//long long fact[150];
//
//long long factorail() {
//	fact[0] = 1;
//	for (long long i = 1; i < 130;++i) {
//		fact[i] = (fact[i - 1] * i)%1000000000000000;
//	}
//
//	return fact[125];
//}
//
//int main() {
//	cout << factorail() << endl;
//
//
//	return 0;
//}