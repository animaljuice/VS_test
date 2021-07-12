#pragma once

#include <vector>

using md5byte = unsigned char;

class MD5
{
	uint32_t buf[4];
	uint32_t bytes[2];
	uint32_t in[16];

	void init();
	void update(md5byte const* buf, unsigned len);
	void final(unsigned char digest[16]);
	static void transform(uint32_t buf[4], uint32_t const in[16]);

public:
	MD5();

	std::vector<md5byte> hash(const md5byte *data, int len);

	static uint32_t hashLen();
};

