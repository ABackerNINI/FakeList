//
//	Created by ABacker on 9/5/2016
//

#include "FakeList.h"

#include <list>
#include <cstring>
#include <iostream>

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

void test_operator() {
	printf("test_operator\n");

	string_builder sb1 = build();
	sb1 += "test_operator";
	sb1.print(true);
	string_builder sb2 = build();
	sb1 += std::move(sb2);
	sb1.print(true);

	//string_builder sb3 = sb1 + sb2;
}

int main() {
	test_operator();

	string_builder sb = "xxx";

	return 0;
}
