#include "Game.h"

void Game::GameLoop(HDC hdc)
{
	DrawAll(hdc);
	Update();
}

void Game::DrawAll(HDC hdc)
{
	DrawBG();
	DrawHero();
	DrawEnemy();


	// base->g_hdc
	BitBlt(hdc, 0, 0, m_Base.bit.bmWidth, m_Base.bit.bmHeight, m_Base.dc, 0, 0, SRCCOPY);
		// ȭ��DC, x��ġ, y��ġ, �ʺ�, ����, dc�� x��ġ, y ��ġ, ��� ���
}

void Game::Update()
{
	UpdateEnemy();
	UpdateHero(HEROMOVESIZE);
}

void Game::InitAll()
{
	InitHero();
	InitEnemy();
	m_uEnemy.dir = DIR_LEFT;
	m_uEnemy.mStartTick = m_uEnemy.mEndTick = GetTickCount();
	m_uHero.mStartTick = m_uHero.mEndTick = GetTickCount();
}

void Game::InitResource()
{
	///////////////////     ȭ�鿡 ȣȯ�Ǵ� ���� ����     /////////////////////
	m_Base.hbmp = (HBITMAP)LoadImage(NULL, L"image/base.bmp", IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);		// ��Ʈ�� �ڵ� ������ ����

	m_Base.dc = CreateCompatibleDC(NULL);	// ȭ�鿡 ȣȯ�Ǵ� DC ���
	m_Base.hOldbmp = (HBITMAP)SelectObject(m_Base.dc, m_Base.hbmp);	// dc �� ��Ʈ�ʰ� ������� ����
	
	GetObject(m_Base.hbmp, sizeof(BITMAP), &m_Base.bit);	// ��Ʈ�� �ڵ�� ��Ʈ�� ����ü �ʵ带 ä��

////////////////////////			m_BG part				////////////////////////////////

	m_BG.hbmp = (HBITMAP)LoadImage(NULL, L"image/BG.bmp", IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);		// ��Ʈ�� �ڵ� ������ ����

	m_BG.dc = CreateCompatibleDC(m_Base.dc);	// �⺻���ۿ� ȣȯ�Ǵ� DC ���
	m_BG.hOldbmp = (HBITMAP)SelectObject(m_BG.dc, m_BG.hbmp);	// dc �� ��Ʈ�ʰ� ������� ����

	GetObject(m_BG.hbmp, sizeof(BITMAP), &m_BG.bit);	// ��Ʈ�� �ڵ�� ��Ʈ�� ����ü �ʵ带 ä��

////////////////////////			m_Hero part				////////////////////////////////
	Load(m_Hero, L"image/pikkachu.bmp");
	Load(m_Enemy, L"image/huny.bmp");
	DrawHero();
	DrawEnemy();
}

void Game::InitHero()
{
	m_Hero.pt.x = m_rtClient.right / 2 - m_Hero.bit.bmWidth / 2;
	m_Hero.pt.y = m_rtClient.bottom -40 - m_Hero.bit.bmHeight / 2;
}

void Game::InitEnemy()
{
	m_Enemy.pt.x = m_rtClient.right / 2 - m_Enemy.bit.bmWidth / 2;
	m_Enemy.pt.y = 30;
}

void Game::DrawBG()
{
	// base dc�� bg dc �� ��� ����
	BitBlt(m_Base.dc, 0, 0, m_BG.bit.bmWidth, m_BG.bit.bmHeight, m_BG.dc, 0, 0, SRCCOPY);
}

void Game::DrawHero()
{
	// base <- hero
	TransparentBlt(m_Base.dc , m_Hero.pt.x , m_Hero.pt.y , m_Hero.bit.bmWidth, m_Hero.bit.bmHeight,
		m_Hero.dc, 0, 0, m_Hero.bit.bmWidth, m_Hero.bit.bmHeight, RGB(255, 0, 255));
}

void Game::DrawEnemy()
{
	// base <- enemy
	TransparentBlt(m_Base.dc, m_Enemy.pt.x, m_Enemy.pt.y, m_Enemy.bit.bmWidth, m_Enemy.bit.bmHeight,
		m_Enemy.dc, 0, 0, m_Enemy.bit.bmWidth, m_Enemy.bit.bmHeight, RGB(255, 0, 255));
}

void Game::UpdateEnemy()
{
	m_uEnemy.mEndTick = GetTickCount();

	if (m_uEnemy.mEndTick - m_uEnemy.mStartTick > ENEMYMOVETICK)	// 0.1 ���� ���̰� ó���� ������ ������ �����̻� ����
	{
		m_uEnemy.mStartTick = m_uEnemy.mEndTick;	// ���忡 �ִ� ��ġ�� ó���κ����� �缳���Ѵ�.
		if (m_uEnemy.dir == DIR_LEFT)
		{
			m_Enemy.pt.x -= ENEMYSPEED;
			if (m_Enemy.pt.x < 0)
				m_uEnemy.dir = DIR_RIGHT;
		}
		else
		{
			m_Enemy.pt.x += ENEMYSPEED;
			if (m_Enemy.pt.x + m_Enemy.bit.bmWidth > m_rtClient.right)
				m_uEnemy.dir = DIR_LEFT;
		}
	}
}

void Game::UpdateHero(int speed)
{
	if (UpdateCheck(m_uHero, HEROMOVETICK) == TRUE)
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)	// Ű�� �������
		{
			m_Hero.pt.x -= speed;

		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	// Ű�� �������
		{
			m_Hero.pt.x += speed;

		}
		if (GetAsyncKeyState(VK_UP) & 0x8000)	// Ű�� �������
		{
			m_Hero.pt.y -= speed;

		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)	// Ű�� �������
		{
			m_Hero.pt.y += speed;

		}
	}
	
}

BOOL Game::UpdateCheck(UPDATE & up, DWORD tick)
{
	up.mEndTick = GetTickCount();

	if (up.mEndTick - up.mStartTick > tick)	// tick �� ���̰� ó���� ������ ������ �����̻� ����
	{
		up.mStartTick = up.mEndTick;	// ���忡 �ִ� ��ġ�� ó���κ����� �缳���Ѵ�.
		return TRUE;
	}

	return FALSE;
}

void Game::Load(HANDLES & h, LPCWSTR name)
{
	h.hbmp = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);		// ��Ʈ�� �ڵ� ������ ����
	if (h.hbmp == NULL)
	{
		MessageBox(NULL, name, L"��Ʈ�� �ε� ����", MB_OK);
	}
	h.dc = CreateCompatibleDC(m_Base.dc);	// m_Base.dc �� ȣȯ�Ǵ� DC ���
	h.hOldbmp = (HBITMAP)SelectObject(h.dc, h.hbmp);	// dc �� ��Ʈ�ʰ� ������� ����

	GetObject(h.hbmp, sizeof(BITMAP), &h.bit);	// ��Ʈ�� �ڵ�� ��Ʈ�� ����ü �ʵ带 ä��
}

RECT & Game::getRECT()
{
	return m_rtClient;
}
