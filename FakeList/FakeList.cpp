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
	printf("test_pop_back\n");

	string_builder sb = build();

	printf("front:%c back:%c\n", sb.front(), sb.back());

	while (!sb.empty()) {
		sb.pop_back();
		sb.print(true);
	}
}

void test_pop_front() {
	printf("test_pop_front\n");

	string_builder sb = build();

	while (!sb.empty()) {
		sb.pop_front();
		sb.print(true);
	}
}

void test_const_iterator() {
	printf("test_const_iterator\n");

	string_builder sb = build();

	for (string_builder::const_iterator it = sb.begin(); it != sb.end();++it) {
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

void test_erase() {
	printf("test_erase\n");

	string_builder sb = build();
	sb.erase(5, sb.size()-5);
	sb.print(true);
}

void test_format_clone() {
	printf("test_format_clone\n");

	string_builder sb1 = build();
	string_builder sb2 = sb1.clone();

	sb2.print(true);
	sb1.format();
	sb1.print(true);
} 

void test() {
	char *s1 = new char[30];
	strcpy(s1, "abcdefghijklmnopqrstuvwxyz");
	string_builder sb1(std::move(s1));
	cout << sb1.to_string() << endl;

	sb1.print(true);
	string_builder sb2 = sb1.clone();
	sb2.print(true);

	sb1.format();
	sb1.print(true);
}

int main(){
	//_CrtSetBreakAlloc(168);

	test_pop_front();
	test_pop_back();
	test_const_iterator();
	test_iterator();
	test_erase();
	test_format_clone();

	_CrtDumpMemoryLeaks();

	return 0;
}