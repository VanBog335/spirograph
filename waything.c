 /* сделано VanBog'ом.
 * программа, которая рисует крассивые узоры
 * первая реализация была на js, но впоследствии
 * переписана на си и либу raylib.
 * всё для компиляции лежит в этой папке,
 * абсолютно никакие внешние .h файлы не нужны.
 * на линуксе не тестил, но в теории должно работать.
 * на винде компилил с помощью gcc в msys2.
 * при компиляции с помощью tcc что-то происходило,
 * из-за которой ломалось отображение узора.
 * а ведь бинарник мог весить не 23 кб, а 7кб...
 * Up.10.04.25 БЛЯТЬ Я ПОЧИНИЛ СПУСТЯ 2 МЕСЯЦА!!!
 * но tcc сломал сломал работу IsKeyPressed()
 * это не компромис, но зато я теперь знаю что
 * это нечто будет работать корректно вне зависимости
 * от компилятора. ура
 * Up.15.04.25 после ручного билда tcc баг ушёл, но
 * так как tcc расчитан на скорость компиляции и малый
 * размер бинарников, то скорость работы выходной
 * программы по сравнению с gcc так неплохо так проседает
 */

#include "raylib.h"
#include "incs.h"
#include <stdlib.h> // бе

i32 main(i32 argc, char **argv)
{
	const u16 scrWidth = 512;
	const u16 scrHeight = 512;
	u32 pixCount = argc == 1 ? 40000 : atoi(argv[1]);
	i8 p = 0; // кол-во синусов и косинусов в формуле, иначе лепестков
	Color col;
	i16 px, py;
	// k - отношение диаметров шестерёнок
	// d - отверсие карандаша в шестерёнке
	bool drawn[scrWidth*scrHeight];

	f32
	time = 1,
	waveModifier = 0, // k
	angleStep = 0, 
	scale; // d
	
	Vector2 *pixelPositions = (Vector2 *)malloc(sizeof(Vector2) * pixCount);

	InitWindow(scrWidth, scrHeight, "спирограф");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		// привязка работы к 60 фпс, при любом будет нормально вращаться
		time += 0.002*60 * GetFrameTime();
		
		// старый код, отвечающий за изменение pixCount
		/*
		if (tmp1 != pixCount) {
			pixCount = tmp1;
			pixelPositions = (Vector2 *)realloc(pixelPositions, sizeof(Vector2) * pixCount);

			angleStep = M_PI * 2 / pixCount;
			if (waveModifier != (i8)waveModifier)
				angleStep *= Pow(2,p-1);
		}
		
		if (IsKeyDown(KEY_UP)) {
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
				tmp1 += 10*1000*60*GetFrameTime();
			else if (IsKeyPressed(KEY_UP))
				tmp1 += 10;
		}
		
		if (IsKeyDown(KEY_DOWN)) {
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
				tmp1 -= 10*1000*60*GetFrameTime();
			else if (IsKeyPressed(KEY_DOWN))
				tmp1 -= 10;
		}
		
		if (pixCount >= 4e9) {
			pixCount = 0;
			tmp1 = 0;
		}
		*/
		
		// black magic
		if (time > 1) {
			time = 0;

			// Randomize frequency and iteration j count
			waveModifier = -1.5 - GetRandomValue(0,5) / 2;
			p = 2 + GetRandomValue(0,5) / -waveModifier;

			angleStep = M_PI * 2 / pixCount;
			if (waveModifier != (i8)waveModifier)
				angleStep *= Pow(2,p-1);

			col = GetColor((GetRandomValue(9,1<<25)<<8)+255);
		}

		scale = Lsin(time * M_PI) * 0.75;
		
		for (u32 i = 0; i < pixCount; i++) {
			f32 accumX = 0.0f;
			f32 accumY = 0.0f;
			for (i32 j = 0; j < p; j++) {
				accumX += Pow(scale, j) * Lsin(time + Pow(waveModifier, j) * i * angleStep);
				accumY += Pow(scale, j) * Lcos(time + Pow(waveModifier, j) * i * angleStep);
			}
			pixelPositions[i].x = (scrWidth >> 1) + 80 * accumX;
			pixelPositions[i].y = (scrHeight >> 1) + 80 * accumY;
		}

		BeginDrawing();
		// {
		//ClearBackground(GetColor(24));
		// я хз как, но можно заюзать процентно альфу
		DrawRectangle(0, 0, scrWidth, scrHeight, GetColor( (u8)(32*60*GetFrameTime()) ) );
		
		for (u32 i = 0; i < scrWidth*scrHeight; i++) {
			drawn[i] = 0;
		}
		
			for (u32 i = 0; i < pixCount; i++) {
				px = pixelPositions[i].x;
				py = pixelPositions[i].y;
				if (px >= 0 && px < scrWidth && py >= 0 && py < scrHeight) {
					i32 index = py * scrWidth + px;
					if (!drawn[index]) {
						DrawPixel(px, py, col);
						drawn[index] = 1;
					}
				}
			}
		
		DrawRectangle(0, 0, 90, 2+5+2+14+14, BLACK);
		
		DrawText(TextFormat("%u", pixCount), 2, 5+14, 20, RAYWHITE);
		DrawText(TextFormat("%.3f", GetFrameTime()), 2, 0, 20, RAYWHITE);
		//DrawFPS(2,0);
		// }
		EndDrawing();
	}
	free(pixelPositions);
	CloseWindow();
}