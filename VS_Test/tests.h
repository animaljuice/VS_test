#pragma once

#include <iostream>
#include <string>
#include "MD5.h"

void MD5Test() {

	auto hexOut = [](const std::vector<md5byte> &res) {
		for (size_t byteIndex = 0; byteIndex < res.size(); byteIndex++) {
			std::cout << std::hex << int(res[byteIndex]);
		}
		std::cout << std::endl;
	};

	MD5 tmd5;

	auto strTest = [&tmd5, hexOut](const std::string& testStr) {
		hexOut(tmd5.hash(reinterpret_cast<const md5byte*>(testStr.data()), testStr.size()));
	};

	strTest("qwe");
	strTest("1");
	strTest("Говорят, что готовить парад зимой так же нелепо, как выносить эксковатор прямо в жвачку. Незнаю, по мне, так негатив, получаемый от последующей концентрации извенений, почти полностью невелируется выравниванием летальности");
}