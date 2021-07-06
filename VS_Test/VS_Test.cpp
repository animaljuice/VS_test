#include <thread>
#include <iostream>
#include "MD5.h"

FILE* inFile = nullptr;
FILE* outFile = nullptr;
uint64_t blockSize = 1048576;
uint64_t inFileSize;

bool parsCommandLine() {
	for (size_t argNum = 0; argNum < __argc; argNum++) {
		if (strcmp(__argv[argNum], "-h") == 0) {
			std::cout << "Create file signature -i input -o output [-bs block size]" << std::endl;
			std::cout << "-i - path to input file" << std::endl;
			std::cout << "-o - path to output file" << std::endl;
			std::cout << "-bs - hashed block size in bytes (1 Mb - default)" << std::endl;

			return true;
		}
		else if (strcmp(__argv[argNum], "-i") == 0) {
			struct _stat64 statistic;
			if (_stat64(__argv[++argNum], &statistic) != 0) {
				throw std::exception("There is a problem with input file. Probably it is unexisted or may be you have not some permission rights. Programm will be finished... Sorry... I tried");
			}
			else
			{
				inFileSize = statistic.st_size;
				if (fopen_s(&inFile, __argv[argNum], "rb") != 0) {
					throw std::exception("I can't open input file");
				}
			}
		}
		else if (strcmp(__argv[argNum], "-o") == 0) {
			if (fopen_s(&outFile, __argv[++argNum], "wb") != 0)
			{
				throw std::exception("There is a problem with output file. Probably you have no rights for writing to this directory.");
			}
		}
		else if (strcmp(__argv[argNum], "-bs") == 0) {
			auto bs = _atoi64(__argv[++argNum]);
			blockSize = bs == 0 ? blockSize : bs;
		}
	}
}

int main()
{
	try {
		parsCommandLine();
	}
	catch (std::exception ex) {
		std::cout << ex.what() << std::endl;
		int errCode = 0;
		_get_errno(&errCode);
		return errCode;
	}

	std::atomic<long> blockCounter = 0;
	auto fullBlockCount = inFileSize / blockSize;
	auto lastBlockSize = inFileSize % blockSize;
	auto blockProcess = [&blockCounter, &fullBlockCount]() {
		while (true)
		{
			auto blockIndex = blockCounter.fetch_add(1);
			if (blockIndex >= fullBlockCount) {
				break;
			}
			std::cout << blockIndex << std::endl;
		}
	};

	std::vector<std::thread> threads(std::thread::hardware_concurrency());
	for (size_t threadIndex = 0; threadIndex < threads.size(); threadIndex++)
	{
		threads[threadIndex] = std::thread(blockProcess);
	}

	while (true)
	{

	}
	
	//fread(&curStrSize, sizeof(curStrSize), 1, file

	return 0;
}