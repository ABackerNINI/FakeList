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
	strcpy_s(s, 15, "1234567890");
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

void test_pop_front() {
	printf("test_pop_front\n");

	string_builder sb = build();
	size_type _Size = sb.size();

	while (!sb.empty()) {
		sb.pop_front();
		sb.print(true);

		//check size
		if(--_Size!=sb.size()) {
			throw 1;
		}
	}
}

void test_pop_back() {
	printf("test_pop_back\n");

	string_builder sb = build();
	size_type _Size = sb.size();

	printf("front:%c back:%c\n", sb.front(), sb.back());

	while (!sb.empty()) {
		sb.pop_back();
		sb.print(true);
		
		//check size
		if (--_Size != sb.size()) {
			throw 1;
		}
	}
}

int main() {
	//_CrtSetBreakAlloc(168);

	test_pop_front();
	test_pop_back();

	_CrtDumpMemoryLeaks();

	return 0;
}