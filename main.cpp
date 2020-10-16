#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

struct hash_t
{
	unsigned char values[32] { 0 };
};

unsigned char rtable[256] {
	12, 83, 41, 214, 180, 25, 220, 235, 43, 216, 119, 214, 119, 11, 213, 20, 243,
	53, 191, 90, 79, 211, 16, 42, 216, 143, 23, 165, 249, 182, 61, 160, 2,
	236, 232, 249, 41, 31, 159, 43, 207, 185, 110, 33, 4, 26, 140, 72, 76,
	69, 59, 232, 163, 217, 240, 180, 192, 68, 180, 160, 151, 51, 202, 242, 33,
	76, 146, 156, 95, 32, 173, 201, 157, 91, 98, 182, 244, 62, 64, 195, 18,
	80, 5, 202, 62, 80, 191, 175, 150, 109, 49, 56, 155, 186, 120, 31, 240,
	220, 239, 102, 95, 5, 79, 235, 29, 39, 27, 60, 79, 97, 59, 172, 204,
	190, 230, 167, 37, 30, 198, 66, 227, 116, 87, 22, 15, 50, 145, 79, 248,
	2, 198, 1, 48, 183, 205, 186, 216, 131, 96, 92, 29, 106, 195, 44, 3,
	246, 166, 39, 98, 45, 77, 150, 45, 192, 238, 226, 250, 133, 91, 169, 191,
	39, 224, 17, 218, 29, 111, 93, 85, 216, 250, 190, 103, 65, 33, 107, 62,
	98, 13, 243, 251, 99, 156, 210, 255, 185, 190, 67, 101, 154, 31, 86, 207,
	180, 6, 66, 102, 30, 125, 252, 28, 143, 178, 9, 53, 206, 157, 144, 5,
	105, 227, 179, 251, 170, 250, 31, 222, 199, 142, 226, 87, 89, 36, 126, 176,
	15, 254, 59, 219, 164, 62, 193, 182, 14, 78, 229, 141, 249, 126, 196, 224,
	115, 239, 14, 103, 233, 177, 165, 84, 82, 40, 103, 216, 170, 180, 71
};

hash_t rval(int seed) {
	srand(seed);
	hash_t r;

	for (int i = 0; i < 32; i++) {
		r.values[i] = rand() % 256;
	}

	return r;
}

hash_t hash256(int seed, unsigned char* str) {
	hash_t randh = rval(seed);

	for (int i = 0; i < 32; i++) {
		randh.values[i] = randh.values[i] ^ str[i];
	}

	return randh;
}

hash_t hash(const char* stri, int n) {
	int sum = 0;

	unsigned char* str = new unsigned char[n];
	std::copy(stri, stri + n, str);

	for (int i = 0; i < n; i++) {
		str[i] = rtable[str[i]];

		sum += str[i];
	}

	unsigned char str2[32] { 0 };	

	if (n < 32) {
		std::copy(str, str + n, str2);
		for (int i = 0; i < 32 - n; i++) {
			str2[n + i] = sum % 256;
		}
	} else if (n > 32) {
		int t = n % 32;
		int tt = n / 32;

		for (int i = 0; i < 32 - t; i++) {
			str[tt * 32 + t + i] = sum % 256;
		}

		for (int i = 0; i < 32; i++)
		{
			unsigned char k = 0;

			for (int j = 0; j < tt; j++)
			{
				if (tt == j - 1 && i > t - 1) {
					k ^= sum % 256;
				} else {
					k ^= str[j * 32 + i];
				}			
			}

			str2[i] = k;
		}
	}

	return hash256(sum, str2);
}

hash_t hash(const std::string& str) {
	return hash(str.c_str(), str.size());
}

std::string hashhex(hash_t h) {
	std::string s = "";

	for (int i = 0; i < 32; i++) {
		std::stringstream sstream;
		sstream << std::setfill('0') << std::setw(2) << std::hex << (int)h.values[i];

		s += sstream.str();
	}

	return s;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "no argument!\n";
	}

	hash_t h1 = hash(argv[1]);

	std::cout << hashhex(h1) << '\n';
}
