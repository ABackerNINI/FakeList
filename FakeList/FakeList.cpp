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

int main() {
	//_CrtSetBreakAlloc(168);

	test_operator();

	_CrtDumpMemoryLeaks();

	return 0;
}