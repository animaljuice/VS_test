#include <thread>
#include <condition_variable>
#include <mutex>
#include <map>
#include <memory>
#include <iostream>
#include "MD5.h"

constexpr uint64_t desiredReadBlockSize = 1ui64 << 32;
uint64_t blockSize = 1048576;
uint64_t inFileSize;

/// <summary>
/// parsing command line
/// </summary>
/// <param name="in"></param>
/// <param name="out"></param>
/// <returns>true if files were inited, false otherwise</returns>
bool parsCommandLine(std::shared_ptr<FILE> &in, std::shared_ptr<FILE> &out) {
	bool res = false;
	FILE* tempFile;

	auto setFilePtr = [&res, &in, &out](FILE *file, std::shared_ptr<FILE> &sharedFile) {
		sharedFile.reset(file, [](FILE* f) {
			if (f) {
				fclose(f);
			}
			});

		if (in.get() != nullptr && out.get() != nullptr) {
			res = true;
		}
	};

	for (size_t argNum = 0; argNum < __argc; argNum++) {
		if (strcmp(__argv[argNum], "-h") == 0) {
			std::cout << "Create file signature -i input -o output [-bs block size]" << std::endl;
			std::cout << "-i - path to input file" << std::endl;
			std::cout << "-o - path to output file" << std::endl;
			std::cout << "-bs - hashed block size in bytes (1 Mb - default)" << std::endl;

			break;
		}
		else if (strcmp(__argv[argNum], "-i") == 0) {
			struct _stat64 statistic;
			if (_stat64(__argv[++argNum], &statistic) != 0) {
				throw std::exception("There is a problem with input file. Probably it is unexisted or may be you have not some permission rights. Programm will be finished... Sorry... I tried");
			}
			else
			{
				inFileSize = statistic.st_size;
				if (fopen_s(&tempFile, __argv[argNum], "rb") != 0) {
					throw std::exception("I can't open input file");
				}

				setFilePtr(tempFile, in);
			}
		}
		else if (strcmp(__argv[argNum], "-o") == 0) {
			if (fopen_s(&tempFile, __argv[++argNum], "wb") != 0)
			{
				throw std::exception("There is a problem with output file. Probably you have no rights for writing to this directory.");
			}

			setFilePtr(tempFile, out);
		}
		else if (strcmp(__argv[argNum], "-bs") == 0) {
			auto bs = _atoi64(__argv[++argNum]);
			blockSize = bs == 0 ? blockSize : bs;
		}
	}

	return res;
}

int main()
{
	std::shared_ptr<FILE> inFile;
	std::shared_ptr<FILE> outFile;

	try {
		if (!parsCommandLine(inFile, outFile)) {
			return 0;
		}
	}
	catch (std::exception ex) {
		std::cout << ex.what() << std::endl;
		int errCode = 0;
		_get_errno(&errCode);
		return errCode;
	}

	std::atomic<long> blockCounter = 0;
	std::map<int, std::vector<md5byte>> checksummBlocks;
	std::mutex checksummBlocksMtx;
	std::condition_variable readBlockWaiter;

	const auto fullBlockCount = inFileSize / blockSize;
	const auto lastBlockSize = inFileSize % blockSize;
	const uint64_t readBlockSize = desiredReadBlockSize - (desiredReadBlockSize % blockSize);
	std::vector<unsigned char> buffer(readBlockSize);

	auto blockProcess = [&blockCounter, &fullBlockCount, inFile, outFile](std::mutex &mtx) {
		MD5 sumGen;
		while (true)
		{
			auto blockIndex = blockCounter.fetch_add(1);
			if (blockIndex >= fullBlockCount) {
				break;
			}
			else {
				fread(buffer.data(), 1, blockSize, inFile.get());
				sumGen.hash(buffer.data(), buffer.size());
			}
		}
	};

	auto writeSumm = [&checksummBlocks, &checksummBlocksMtx]() {

	};

	std::vector<std::thread> threads(std::thread::hardware_concurrency());
	std::vector<std::mutex> mutexes(threads.size());
	for (size_t threadIndex = 0; threadIndex < threads.size(); threadIndex++)
	{
		mutexes[threadIndex].lock();
		threads[threadIndex] = std::thread(blockProcess, mutexes[threadIndex]);
	}

	while (true)
	{
		fread(buffer.data(), 1, buffer.size(), inFile.get());
	}

	return 0;
}