#include <iostream>
#include <fstream>
#include <string>
#include "subkey.h"
#include "feistel.h"
using namespace std;

//将char字符数组转为二进制
bitset<64> charToBitset(const char * s)
{
	bitset<64> bits;
	for (int i = 0; i<8; ++i)
		for (int j = 0; j<8; ++j)
			bits[i * 8 + j] = ((s[i] >> j) & 1);
	return bits;
}

//IP置换
bitset<64> IPreplace(bitset<64> source) {
	bitset<64> result;
	for (int i = 0; i < 64; ++i) {
		result[i] = source[IP[i] - 1];
	}
	return result;
}

//IP逆置换
bitset<64> IPInverseReplace(bitset<64> source) {
	bitset<64> result;
	for (int i = 0; i < 64; ++i) {
		result[i] = source[IP_1[i] - 1];
	}
	return result;
}

//16次迭代
bitset<64> itera(bitset<32> left, bitset<32> right,bool order) {
	bitset<32> laterLeft, laterRight;
	for (int i = 0; i < 16; ++i) {
		laterLeft = right;
		if (order)
			laterRight = left ^ feistel(right, subkey[i]);
		else
			laterRight = left ^ feistel(right, subkey[15 - i]);
		left = laterLeft;
		right = laterRight;
	}
	bitset<64> result;
	for (int i = 0; i < 32; ++i)
		result[i] = laterRight[i];
	for (int i = 0; i < 32; ++i)
		result[i + 32] = laterLeft[i];
	return result;
}

//加密函数
bitset<64> encrypt(bitset<64> plainText, string keyString) {
	key = charToBitset(keyString.c_str());
	bitset<64> initialIPreplace = IPreplace(plainText);
	bitset<32> left, right;
	for (int i = 0; i < 32; ++i)
		left[i] = initialIPreplace[i];
	for (int i = 0; i < 32; ++i)
		right[i] = initialIPreplace[i + 32];
	generateSubkey();
	bitset<64> iteraText = itera(left, right, true);
	bitset<64> cipherText = IPInverseReplace(iteraText);
	return cipherText;
}

//解密函数
bitset<64> decrypt(bitset<64> cipherText) {
	bitset<64> initialCipher = IPreplace(cipherText);
	bitset<32> left, right;
	for (int i = 0; i < 32; ++i)
		left[i] = initialCipher[i];
	for (int i = 0; i < 32; ++i)
		right[i] = initialCipher[i + 32];
	bitset<64> iteraText = itera(left, right, false);
	bitset<64> plainText = IPInverseReplace(iteraText);
	return plainText;
}

int main() {
	//读取明文写入密文
	string keyString = "12345678";
	ifstream in;
	ofstream out;
	in.open("initial.txt", ios::binary);
	out.open("cipher.txt", ios::binary);
	bitset<64> plain;
	while (in.read((char*)&plain, sizeof(plain)))
	{
		bitset<64> cipher = encrypt(plain, keyString);
		out.write((char*)&cipher, sizeof(cipher));
		plain.reset(); 
	}
	in.close();
	out.close();

	// 解密 cipher.txt，并写入图片 plain.txt
	in.open("cipher.txt", ios::binary);
	out.open("plain.txt", ios::binary);
	while (in.read((char*)&plain, sizeof(plain)))
	{
		bitset<64> temp = decrypt(plain);
		out.write((char*)&temp, sizeof(temp));
		plain.reset(); 
	}
	in.close();
	out.close();

	return 0;
}
