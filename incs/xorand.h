#include "stdtypes.h"

static u64 x;
static u64 s[4];

u64 xoshiroNumMaker() {
	u64 z = (x += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
	return z ^ (z >> 31);
}

u64 rol64(u64 x, i32 k) {
	return (x << k) | (x >> (64 - k));
}

//----------------
// User Functions
//----------------

u64 xorand256() {
	u64 const result = rol64(s[0] + s[3], 23) + s[0];
	u64 const t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;
	s[3] = rol64(s[3], 45);

	return result;
}

u0 xorandInit()
{
	for (u8 i = 0; i < 4; i++) {
		s[i] = xoshiroNumMaker();
	}
}

u0 xorandSeed(u32 seed)
{
	x = seed;
}