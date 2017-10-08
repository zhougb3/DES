#include <bitset>
#include "dataStruct.h"
using namespace std;


//EÀ©Õ¹º¯Êý
bitset<48> E_extend(bitset<32> source) {
	bitset<48> result;
	for (int i = 0; i < 48; ++i) {
		result[i] = source[E[i] - 1];
	}
	return result;
}

//P-ÖÃ»»
bitset<32> PInverseReplace(bitset<32> source) {
	bitset<32> result;
	for (int i = 0; i < 32; ++i) {
		result[i] = source[P[i] - 1];
	}
	return result;
}

//feistelº¯Êý
bitset<32> feistel(bitset<32> right, bitset<48> k) {
	bitset<48> eRight = E_extend(right);
	bitset<48> xorResult;
	bitset<32> sboxResult;
	xorResult = eRight ^ k;
	int x = 0;
	for (int i = 0; i< 48; i = i + 6)
	{
		int row = xorResult[47 - i] * 2 + xorResult[47 - i - 5];
		int col = xorResult[47 - i - 1] * 8 + xorResult[47 - i - 2] * 4 + xorResult[47 - i - 3] * 2 + xorResult[47 - i - 4];
		int num = S_BOX[i / 6][row][col];
		bitset<4> binaryNum(num);
		sboxResult[31 - x - 3] = binaryNum[0];
		sboxResult[31 - x - 2] = binaryNum[1];
		sboxResult[31 - x - 1] = binaryNum[2];
		sboxResult[31 - x] = binaryNum[3];
		x += 4;
	}
	return PInverseReplace(sboxResult);
}
