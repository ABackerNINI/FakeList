//
//	Created by ABacker on 10/14/2016
//

#include <iostream>
#include <crtdbg.h>
#include <list>
#include <xstring>
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

void test_format_clone() {
	printf("test_format_clone\n");

	string_builder sb1 = build();
	string_builder sb2 = sb1.clone();

	sb2.print(true);
	sb1.format();
	sb1.print(true);
}

void test_clone() {
	printf("test_clone\n");

	char *s1 = new char[30];
	strcpy_s(s1, sizeof(char)* 30, "1234567890");
	string_builder sb1(std::move(s1));
	cout << sb1.to_string() << endl;

	sb1.print(true);
	string_builder sb2 = sb1.clone(3);
	sb2.print(true);

	sb1.format(4);
	sb1.print(true);

	list<int>l;
}

int main() {
	//_CrtSetBreakAlloc(168);

	test_format_clone();
	test_clone();

	_CrtDumpMemoryLeaks();

	return 0;
}