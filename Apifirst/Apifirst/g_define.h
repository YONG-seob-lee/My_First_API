#pragma once

#define XRES 1024		// 화면의 x 축 크기
#define YRES 768		// 화면의 y 축 크기

//enemy
#define ENEMYMOVETICK 10		// 적의 이동 단위
#define ENEMYSPEED 10	// 적의 이동 속도

//hero
#define HEROMOVETICK 10	// 아군의 이동 단위
#define HEROMOVESIZE 7	// 아군의 이동 속도

struct HANDLES
{
	HBITMAP hbmp, hOldbmp;	// 비트맵 핸들
	HDC dc; //그림찍기 핸들
	BITMAP bit; // 실제 비트맵 구조체
	POINT pt; // 찍을 위치 정보


};

struct UPDATE
{
	// move
	DWORD mStartTick;
	DWORD mEndTick;

	int dir;	// 방향 direction



	// animate
};

enum
{
	DIR_LEFT,
	DIR_RIGHT,
};