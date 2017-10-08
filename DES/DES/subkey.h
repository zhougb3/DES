#include <bitset>
#include "dataStruct.h"
using namespace std;
bitset<64> key;
bitset<48> subkey[16];

//PC_1置换
bitset<56> PC_1replace(bitset<64> source) {
	bitset<56> result;
	for (int i = 0; i < 56; ++i) {
		result[i] = source[PC_1[i] - 1];
	}
	return result;
}

//PC_2置换
bitset<48> PC_2replace(bitset<56> source) {
	bitset<48> result;
	for (int i = 0; i < 48; ++i) {
		result[i] = source[PC_2[i] - 1];
	}
	return result;
}

//对密钥的前后部分进行左移
bitset<28> leftShift(bitset<28> key, int shift)
{
	bitset<28> temp = key;
	for (int i = 27; i >= 0; --i)
	{
		if (i - shift < 0)
			key[i] = temp[i - shift + 28];
		else
			key[i] = temp[i - shift];
	}
	return key;
}

void generateSubkey() {
	bitset<56> replaceKey = PC_1replace(key);
	bitset<28> C0, D0;
	bitset<56> shiftKey;
	for (int i = 0; i < 28; ++i)
		C0[i] = replaceKey[i];
	for (int i = 0; i < 28; ++i)
		D0[i] = replaceKey[i + 28];
	for (int j = 0; j < 16; ++j) {
		bitset<28> left = leftShift(C0, shiftBits[j]);
		bitset<28> right = leftShift(D0, shiftBits[j]);
		C0 = left;
		D0 = right;
		for (int i = 0; i < 28; ++i)
			shiftKey[i] = left[i];
		for (int i = 0; i < 28; ++i)
			shiftKey[i + 28] = right[i];
		subkey[j] = PC_2replace(shiftKey);
	}
}