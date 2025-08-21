// набор типов данных, для удобства
typedef signed char i8;
typedef unsigned char u8;
typedef signed short i16;
typedef unsigned short u16;
typedef signed int i32;
typedef unsigned int u32;
typedef signed long long int i64;
typedef unsigned long long int u64;
typedef void u0;
/* typedef _Float16 f16; */   // а что, gcc на винде имеет поддержку этого
typedef float f32;
typedef double f64;
typedef long double f128;     // я сам удивился что есть float такой длины

#define MY_2PI (QUOTER<<2)             // полный круг
#define MY_PI (QUOTER<<1)              // пол круга
#define QUOTER 256                     // четверть круга
#define AMPLITUDE (0xff +.0)           // макс значения типа данных массива графика
#define M_PI 3.14159265f   // очевидно

#define BITVAL(X,N) (((X)>>(N))&1)
#define BITFLIP(X,N) (X^(1<<(N)))


/* сгенерил таблицу https://ppelikan.github.io/drlut
 * возростающий график 90 градусов.
 * при надобности можно увеличить, но размер обязательно
 * должен быть степенью двойки, иначе в функциях синуса и
 * косинуса всё полетит, т.к используются & вместо
 * медленного модуля.
 */

const u8 sine[QUOTER] = {
0,2,3,5,6,8,9,11,13,14,16,17,19,20,22,23,25,27,28,30,31,33,34,36,37,39,41,42,44,45,47,48,50,51,53,54,56,57,59,60,62,63,65,67,68,70,71,73,74,76,77,79,80,81,83,84,86,87,89,90,92,93,95,96,98,99,100,102,103,105,106,108,109,110,112,113,115,116,117,119,120,122,123,124,126,127,128,130,131,132,134,135,136,138,139,140,142,143,144,146,147,148,149,151,152,153,154,156,157,158,159,161,162,163,164,165,167,168,169,170,171,172,174,175,176,177,178,179,180,181,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,208,209,210,211,212,213,214,215,215,216,217,218,219,220,220,221,222,223,223,224,225,226,226,227,228,228,229,230,231,231,232,232,233,234,234,235,236,236,237,237,238,238,239,240,240,241,241,242,242,243,243,244,244,244,245,245,246,246,247,247,247,248,248,248,249,249,249,250,250,250,251,251,251,252,252,252,252,252,253,253,253,253,253,254,254,254,254,254,254,254,255,255,255,255,255,255,255,255,255,255
};

// нужна для мешьшего перерасчёта в обеих функция и нужна
// для конвертиции радианов в индекс массива с графиком.

const f32 MRadsToMyRads = MY_PI/M_PI;

// я, конечно, сам это написал, но я половины этой функции не понимаю.
f32 calc_sineIndex(u16 step)
{
	u16 index;
	if (step < QUOTER) {
		index = step;
	} else if (step < MY_PI) {
		index = QUOTER - (step & (QUOTER - 1)) - 1;
	} else if (step < QUOTER + MY_PI) {
		index = step & (MY_PI - 1);
		return -sine[index] / AMPLITUDE;
	} else {
		index = QUOTER - (step & (QUOTER - 1)) - 1;
		return -sine[index] / AMPLITUDE;
	}

	return sine[index] / AMPLITUDE;
}

f32 Lsin(f32 step) {
	u16 realStep = (u32)(step * MRadsToMyRads) & (MY_2PI - 1);
	return calc_sineIndex(realStep);
}

f32 Lcos(f32 step) {
	u16 realStep = (u32)(step*MRadsToMyRads + QUOTER)&(MY_2PI-1);
	return calc_sineIndex(realStep);
}



// да, оно оптимизированно, но есть рекурсия. не люблю рекурсии.
// да я даже не знаю, зачем в битовые операции тут полез.
f32 Pow(f32 x, i32 y) {
	f32 tmp;
	if (!y) return 1;
	tmp = Pow(x,y>>1);
	if (!(y&1)) return tmp*tmp;
	else	if (y>0) return x * tmp*tmp;
			else return (tmp*tmp) / x;
}

//-------------------------------------------------------------------

// musl libc
i32 atoi(const char *s)
{
	i32 n=0, neg=0;
	while (*s == ' ' || (u32)*s-'\t' < 5) s++;
	switch (*s) {
		case '-': neg=1;
		case '+': s++;
	}
	// Compute n as a negative number to avoid overflow on INT_MIN
	while ((u32)*s-'0' < 10)
		n = 10*n - (*s++ - '0');
	return neg ? n : -n;
}

//-------------------------------------------------------------------




// бэкап старой версии на случай помломки, уже поидее не нужОн

/*
#include "raylib.h"

#define ISIN_QN 10
#define QA 12
#define ISIN_B 19900
#define ISIN_C 3516
#define rndn GetRandomValue
#define M_PI 3.14159265f
#define M_D2R (M_PI / 180.)
#define M_R2D (180. / M_PI)

int isin_s4(int x) {
	int c,y;

	c = x << (30 - ISIN_QN);
	x -= 1 << ISIN_QN;

	x <<= (31 - ISIN_QN);
	x >>= (31 - ISIN_QN);
	x *= x;
	x >>= (2 * ISIN_QN - 14);

	y = ISIN_B - (x * ISIN_C >> 14);
	y = (1 << QA) - (x * y >> 16);

	return c >= 0 ? y : -y;
}

float Math_SinF(float angle) {
	int raw = (int)(angle * M_R2D * 4096 / 360);
	return isin_s4(raw) / 4096.;
}

float Math_CosF(float angle) {
	int raw = (int)(angle * M_R2D * 4096 / 360);
	raw += (1 << ISIN_QN); 
	return isin_s4(raw) / 4096.;
}

int Math_Floor(float value) {
	int valueI = (int)value;
	return valueI > value ? valueI - 1 : valueI;
}

float Pow(float x, int y) {
	float tmp;
	if (!y) return 1;
	tmp = Pow (x,y/2);
	if (!(y&1)) return tmp*tmp;
	else	if (y > 0) return x * tmp*tmp;
			else return (tmp*tmp) / x;
}

int main() {
	int scrWidth = 600;
	int scrHeight = 600;
	int pixCount = 15000;
	int p;
	Color col;
	float t = 1;
	float k,s,d,x,y;

	InitWindow(scrWidth,scrHeight,"eey");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		t += 0.006;
		if (t >= 1) {
			t = 0;

			k = -1.5 - rndn(0,5) / 2;
			p = 2 + rndn(0,5) / -k;

			s = M_PI * 2 / pixCount;
			if (k != Math_Floor(k))
				s *= Pow(2,p-1);

			col = GetColor((rndn(9,1<<25)<<8)+255);
		}

		d = Math_SinF(t * M_PI) * 0.75;

		BeginDrawing();
			DrawRectangle(0, 0, scrWidth, scrHeight, GetColor(24));

			for (int i = 0; i < pixCount; i++) {
				x = 0;
				y = 0;
				for (int j = 0; j < p; j++) {
					x += Pow(d,j) * Math_SinF(t + Pow(k,j) * i * s);
					y += Pow(d,j) * Math_CosF(t + Pow(k,j) * i * s);
				}
				DrawPixel(scrWidth/2 + 85*x, scrHeight/2 + 85*y, col);
			}

			DrawFPS(5,5);
		EndDrawing();
	}
	CloseWindow();
}
*/
