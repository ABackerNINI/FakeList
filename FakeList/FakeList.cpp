//
//	Created by ABacker on 9/5/2016
//

#include "FakeList.h"
#include <iostream>
#include <crtdbg.h>
#include <list>
#include <xstring>

#define CRTDBG_MAP_ALLOC

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

void test_format_clone() {
	printf("test_format_clone\n");

	string_builder sb1 = build();
	string_builder sb2 = sb1.clone();

	sb2.print(true);
	sb1.format();
	sb1.print(true);
}

void test_operator() {
	printf("test_operator\n");

	string_builder sb1 = build();
	sb1 += "test_opeator";
	sb1.print(true);
	string_builder sb2 = build();
	sb1 += std::move(sb2);
	sb1.print(true);

	//string_builder sb3 = sb1 + sb2;
}

void test_clone() {
	printf("test_clone\n");

	char *s1 = new char[30];
	strcpy(s1, "abcdefghijklmnopqrstuvwxyz");
	string_builder sb1(std::move(s1));
	cout << sb1.to_string() << endl;

	sb1.print(true);
	string_builder sb2 = sb1.clone(3);
	sb2.print(true);

	sb1.format(4);
	sb1.print(true);

	list<int>l;
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

	test_format_clone();
	test_operator();
	test_clone();

	test_replace(1, 2, "c");

	for (int i = 0; i < 37; ++i) {
		for (int j = i; j < 38; ++j) {
			test_replace(i, j, "c");
		}
	}

	_CrtDumpMemoryLeaks();

	return 0;
}