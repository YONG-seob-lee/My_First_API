#pragma once

#define XRES 1024		// ȭ���� x �� ũ��
#define YRES 768		// ȭ���� y �� ũ��

//enemy
#define ENEMYMOVETICK 10		// ���� �̵� ����
#define ENEMYSPEED 5	// ���� �̵� �ӵ�

//hero
#define HEROMOVETICK 10	// �Ʊ��� �̵� ����
#define HEROMOVESIZE 7	// �Ʊ��� �̵� �ӵ�

// bullet
#define BULSPEED 30	// �Ѿ� �ӵ�

struct HANDLES
{
	HBITMAP hbmp, hOldbmp;	// ��Ʈ�� �ڵ�
	HDC dc; //�׸���� �ڵ�
	BITMAP bit; // ���� ��Ʈ�� ����ü
	POINT pt; // ���� ��ġ ����


};

struct UPDATE
{
	// move
	DWORD mStartTick;
	DWORD mEndTick;
	
	// animate
	DWORD aStartTick;
	DWORD aEndTick;
	int MaxFrame;	// 0���� ����
	int Frame;	// ���� ������ �ִ� ������
	int size;	// �� �������� x ������


	int dir;	// ���� direction

	BOOL bTrigger;	// Ʈ���� �ߵ�����, ���� ��� �Ѿ��� �߻����ΰ� Ȯ���ϴ� ������

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