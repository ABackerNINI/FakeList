#define CRTDBG_MAP_ALLOC

#include "fakelist.h"
#include <iostream>
#include <crtdbg.h>

using namespace std;
using namespace nini;

string_builder build() {
	string_builder sb;
	char *s = new char[15];
	strcpy(s, "1234567890");
	sb.assign(std::move(s), strlen(s));

	sb.append("1234567890");

	sb.insert("aacc", 4, 10);
	sb.insert("ddeeff", 6, 4);
	sb.insert("cccc", 4, 6);

	auto it_insert = sb.begin();
	it_insert += 4;
	sb.insert("bbb", 3, it_insert);
	sb.print(true);

	return sb;
}

void test_pop_back() {
	string_builder sb = build();

	printf("%c %c\n", sb.front(), sb.back());

	while (!sb.empty()) {
		sb.pop_back();
		sb.print(true);
	}
}

void test_pop_front() {
	string_builder sb = build();

	printf("%c %c\n", sb.front(), sb.back());

	while (!sb.empty()) {
		sb.pop_front();
		sb.print(true);
	}
}

void test2() {
	char *s1 = new char[30];
	strcpy(s1, "abcdefghijklmnopqrstuvwxyz");
	string_builder sb1(std::move(s1));
	cout << sb1.to_string() << endl;

	sb1.print(true);
}

int main(){
	//_CrtSetBreakAlloc(168);

	test_pop_front();
	test2();

	_CrtDumpMemoryLeaks();

	return 0;
}