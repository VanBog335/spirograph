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
 * Up.10.04.25 БЛЯТЬ Я ПОЧИНИЛ СПУСТЯ 1.5 МЕСЯЦА!!!
 * но tcc сломал сломал работу IsKeyPressed()
 * это не компромис, но зато я теперь знаю что
 * это нечто будет работать корректно вне зависимости
 * от компилятора. ура
 */

#include "raylib.h"
#include "incs.h"

i32 main()
{
	u16 scrWidth = 512;
	u16 scrHeight = 512;
	u32 pixCount = 40000;
	i8 p = 0; // кол-во синусов и косинусов в формуле, иначе лепестков
	Color col;
	u8 tmpbool = 0b00000001;
	u32 tmp1 = pixCount;

	// k - отношение диаметров шестерёнок
	// d - отверсие карандаша в шестерёнке

	f32
	time = 1,
	waveModifier = 0, // k
	angleStep = 0, 
	scale, x, y; // d

	InitWindow(scrWidth, scrHeight, "спирограф");
	SetTargetFPS(0);

	while (!WindowShouldClose()) {
		// привязка работы к 60 фпс, при любом будет нормально вращаться
		time += 0.003*60 * GetFrameTime();
		
		if (tmp1 != pixCount) {
			pixCount = tmp1;

			angleStep = M_PI * 2 / pixCount;
			if (waveModifier != (i8)waveModifier)
				angleStep *= Pow(2,p-1);
		}
		
		if (IsKeyDown(KEY_UP)) {
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
				tmp1 += 10*10*60*GetFrameTime();
			else if (IsKeyPressed(KEY_UP))
				tmp1 += 10;
		}
		
		if (IsKeyDown(KEY_DOWN)) {
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
				tmp1 -= 10*10*60*GetFrameTime();
			else if (IsKeyPressed(KEY_DOWN))
				tmp1 -= 10;
		}
		
		if (pixCount >= 4e9) {
			pixCount = 0;
			tmp1 = 0;
		}

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

		BeginDrawing();
		// {
		//ClearBackground(GetColor(24));
		DrawRectangle(0, 0, scrWidth, scrHeight, GetColor( (u8)(32*60*GetFrameTime()) ) );

		for (int i = 0; i < pixCount; i++) {
			x = 0;
			y = 0;
			for (int j = 0; j < p; j++) {
				x += Pow(scale,j) * Lsin(time + Pow(waveModifier,j) * i * angleStep);
				y += Pow(scale,j) * Lcos(time + Pow(waveModifier,j) * i * angleStep);
			}
			DrawPixel((scrWidth>>1) + 80*x, (scrHeight>>1) + 80*y, col);
		}
		
		DrawRectangle(0, 0, 90, 2+5+2+14+14, BLACK);

		//DrawText(TextFormat("%08d", BITVAL(tmpbool,0)*1 + BITVAL(tmpbool,1)*10 + BITVAL(tmpbool,2)*100 + BITVAL(tmpbool,3)*1000 + BITVAL(tmpbool,4)*10000 + BITVAL(tmpbool,5)*100000 + BITVAL(tmpbool,6)*1000000 + BITVAL(tmpbool,7)*10000000), 5, 5+14+5, 20, RAYWHITE);
		
		DrawText(TextFormat("%u", pixCount), 2, 5+14, 20, RAYWHITE);
		
		DrawFPS(2,0);
		// }
		EndDrawing();
	}
	CloseWindow();
}