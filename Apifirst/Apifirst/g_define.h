#pragma once

#define XRES 1024		// ȭ���� x �� ũ��
#define YRES 768		// ȭ���� y �� ũ��

//enemy
#define ENEMYMOVETICK 10		// ���� �̵� ����
#define ENEMYSPEED 10	// ���� �̵� �ӵ�

//hero
#define HEROMOVETICK 10	// �Ʊ��� �̵� ����
#define HEROMOVESIZE 7	// �Ʊ��� �̵� �ӵ�

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

	int dir;	// ���� direction



	// animate
};

enum
{
	DIR_LEFT,
	DIR_RIGHT,
};