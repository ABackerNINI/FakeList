//
//	Created by ABacker on 10/13/2016
//

#include <iostream>
#include <crtdbg.h>
#include <list>

#include "../FakeList/FakeList.h"

#define CRTDBG_MAP_ALLOC

using namespace std;
using namespace nini;

string_builder build() {
	string_builder sb;
	char *s = new char[15];
	strcpy_s(s, sizeof(char)* 15, "1234567890");
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

void test_const_iterator() {
	printf("test_const_iterator\n");

	string_builder sb = build();

	for (string_builder::const_iterator it = sb.begin(); it != sb.end(); ++it) {
		printf("%c", *it);
	}
	printf("\n");
}

void test_iterator() {
	printf("test_iterator\n");

	string_builder sb = build();

	for (string_builder::iterator it = sb.begin(); it != sb.end(); ++it) {
		*it = 'c';
	}
	sb.print();
	printf("\n");
}

int main() {
	//_CrtSetBreakAlloc(168);

	test_const_iterator();
	test_iterator();

	_CrtDumpMemoryLeaks();

	return 0;
}