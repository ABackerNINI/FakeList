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

void test_erase1(int _L, int _R) {
	printf("test_erase [%d,%d)\n", _L, _R);

	string_builder sb = build();
	size_type _Oldsize = sb.size();

	string_builder::const_iterator _IterL = sb.begin(), _IterR = sb.begin();

	for (int i = 0; i < _L; ++_IterL, ++i) {
	}
	for (int i = 0; i < _R; ++_IterR, ++i) {
	}

	sb.erase(_IterL, _IterR);
	sb.print(true);

	//check size
	if (sb.size() != _Oldsize - (_R - _L)) {
		throw 1;
	}
}

void test_erase2(int _L, int _R) {
	printf("test_erase [%d,%d)\n", _L, _R);

	string_builder sb = build();
	size_type _Oldsize = sb.size();

	sb.erase(_L, _R - _L);
	sb.print(true);

	//check size
	if (sb.size() != _Oldsize - (_R - _L)) {
		throw 1;
	}
}

int main() {
	//_CrtSetBreakAlloc(168);

	for (int i = 0; i < 37; ++i) {
		for (int j = i; j < 38; ++j) {
			test_erase1(i, j);
		}
	}

	for (int i = 0; i < 37; ++i) {
		for (int j = i; j < 38; ++j) {
			test_erase2(i, j);
		}
	}

	_CrtDumpMemoryLeaks();

	return 0;
}