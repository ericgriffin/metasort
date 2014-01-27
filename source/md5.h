/*  md5.h
 *  Copyright (c) Eric Griffin
 *
 *  For conditions of distribution and use, see the
 *  LICENSE file in the root of the source tree.
 */

#ifndef _MD5_H_
#define _MD5_H_ 1

#include <stdio.h>
#include <string.h>

typedef unsigned char BYTE ;
typedef unsigned char *POINTER;
typedef unsigned short int UINT2;
typedef unsigned int UINT4;

class MD5
{
private:
	struct __context_t
	{
		UINT4 state[4];
		UINT4 count[2];
		unsigned char buffer[64];
	} context;

	#pragma region static helper functions
	static void MD5Transform(UINT4 state[4], unsigned char block[64]);
	static void Encode(unsigned char* output, UINT4* input, unsigned int len);
	static void Decode(UINT4* output, unsigned char* input, unsigned int len);
	#pragma endregion

public:
	BYTE digestRaw[16];
	char digestChars[33];
	
	MD5();
	void Init();
	void Update(unsigned char* input, unsigned int inputLen);
	void Final();
	void writeToString();
	char* digestFile(char* filename);
};

#pragma region MD5 defines

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

static unsigned char PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))
#define FF(a, b, c, d, x, s, ac) {(a) += F((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT((a), (s)); (a) += (b);}
#define GG(a, b, c, d, x, s, ac) {(a) += G((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT((a), (s)); (a) += (b);}
#define HH(a, b, c, d, x, s, ac) {(a) += H((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT((a), (s)); (a) += (b);}
#define II(a, b, c, d, x, s, ac) {(a) += I((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT((a), (s)); (a) += (b);}

#pragma endregion

#endif // _MD5_H_