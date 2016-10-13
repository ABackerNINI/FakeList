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

void test_replace(int _L, int _R, const char *_Str) {
	printf("test_replace [%d,%d) to [%s]\n", _L, _R, _Str);

	string_builder sb = build();
	size_type _Size = sb.size();
	size_type _Count = strlen(_Str);

	string_builder::const_iterator _IterL = sb.begin(), _IterR = sb.begin();

	for (int i = 0; i < _L; ++i, ++_IterL) {}
	for (int i = 0; i < _R; ++i, ++_IterR) {}

	sb.replace(_IterL, _IterR, _Str, _Count);
	sb.print(true);

	//check size
	if (_Size - (_R - _L) + _Count != sb.size()) {
		throw 1;
	}
}

int main() {
	//_CrtSetBreakAlloc(168);

	for (int i = 0; i < 37; ++i) {
		for (int j = i; j < 38; ++j) {
			test_replace(i, j, "c");
		}
	}

	_CrtDumpMemoryLeaks();

	return 0;
}