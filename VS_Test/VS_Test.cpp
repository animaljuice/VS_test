#include "tests.h"

int main()
{
	FILE *inFile = nullptr;
	FILE *outFile = nullptr;

	for (size_t argNum = 0; argNum < __argc; argNum++){
		if (strcmp(__argv[argNum], "-h") == 0) {
			std::cout << "Create file signature -i input -o output [-bs block size]" << std::endl;
			std::cout << "-i - path to input file" << std::endl;
			std::cout << "-o - path to output file" << std::endl;
			std::cout << "-bs - hashed block size (1 Mb - default)" << std::endl;

			return 0;
		}
		else if (strcmp(__argv[argNum], "-i") == 0) {
			if (fopen_s(&inFile, __argv[++argNum], "rb") != 0) {
				std::cout << "There is a problem with input file. Probably it is unexisted or may be you have not some permission rights. Programm will be finished... Sorry... I tried";
				return -1;
			}
			else
			{
				std::cout << "in збс";
			}
		}
		else if (strcmp(__argv[argNum], "-o") == 0) {
			if (fopen_s(&outFile, __argv[++argNum], "wb") == 0) {
				std::cout << "out збс";
			}
			else
			{

			}
		}
	}
	
	MD5Test();
}