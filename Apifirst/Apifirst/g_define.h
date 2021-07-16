#pragma once

#define XRES 1024		// 화면의 x 축 크기
#define YRES 768		// 화면의 y 축 크기

//enemy
#define ENEMYMOVETICK 10		// 적의 이동 단위
#define ENEMYSPEED 5	// 적의 이동 속도

//hero
#define HEROMOVETICK 10	// 아군의 이동 단위
#define HEROMOVESIZE 7	// 아군의 이동 속도

// bullet
#define BULSPEED 30	// 총알 속도

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
	
	// animate
	DWORD aStartTick;
	DWORD aEndTick;
	int MaxFrame;	// 0부터 시작
	int Frame;	// 현재 찍히고 있는 프레임
	int size;	// 한 프레임의 x 사이즈


	int dir;	// 방향 direction

	BOOL bTrigger;	// 트리거 발동여부, 예를 들어 총알이 발사중인가 확인하는 논리변수

};

enum
{
	DIR_LEFT,
	DIR_RIGHT,
};

enum GAME_STAGE
{
	GAME_TITLE,
	GAME_READY,
	GAME_START,
	GAME_MAIN,
	STAGE_CLEAR,
	GAME_OVER
};