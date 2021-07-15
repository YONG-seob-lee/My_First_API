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
		// 화면DC, x위치, y위치, 너비, 높이, dc의 x위치, y 위치, 찍는 방법
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
	///////////////////     화면에 호환되는 버퍼 생성     /////////////////////
	m_Base.hbmp = (HBITMAP)LoadImage(NULL, L"image/base.bmp", IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);		// 비트맵 핸들 얻어오는 과정

	m_Base.dc = CreateCompatibleDC(NULL);	// 화면에 호환되는 DC 얻기
	m_Base.hOldbmp = (HBITMAP)SelectObject(m_Base.dc, m_Base.hbmp);	// dc 랑 비트맵과 연결고리를 맺음
	
	GetObject(m_Base.hbmp, sizeof(BITMAP), &m_Base.bit);	// 비트맵 핸들로 비트맵 구조체 필드를 채움

////////////////////////			m_BG part				////////////////////////////////

	m_BG.hbmp = (HBITMAP)LoadImage(NULL, L"image/BG.bmp", IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);		// 비트맵 핸들 얻어오는 과정

	m_BG.dc = CreateCompatibleDC(m_Base.dc);	// 기본버퍼에 호환되는 DC 얻기
	m_BG.hOldbmp = (HBITMAP)SelectObject(m_BG.dc, m_BG.hbmp);	// dc 랑 비트맵과 연결고리를 맺음

	GetObject(m_BG.hbmp, sizeof(BITMAP), &m_BG.bit);	// 비트맵 핸들로 비트맵 구조체 필드를 채움

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
	// base dc에 bg dc 를 찍는 과정
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

	if (m_uEnemy.mEndTick - m_uEnemy.mStartTick > ENEMYMOVETICK)	// 0.1 초의 차이가 처음과 마지막 사이의 차이이상 날때
	{
		m_uEnemy.mStartTick = m_uEnemy.mEndTick;	// 엔드에 있는 위치를 처음부분으로 재설정한다.
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
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)	// 키가 눌린경우
		{
			m_Hero.pt.x -= speed;

		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	// 키가 눌린경우
		{
			m_Hero.pt.x += speed;

		}
		if (GetAsyncKeyState(VK_UP) & 0x8000)	// 키가 눌린경우
		{
			m_Hero.pt.y -= speed;

		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)	// 키가 눌린경우
		{
			m_Hero.pt.y += speed;

		}
	}
	
}

BOOL Game::UpdateCheck(UPDATE & up, DWORD tick)
{
	up.mEndTick = GetTickCount();

	if (up.mEndTick - up.mStartTick > tick)	// tick 의 차이가 처음과 마지막 사이의 차이이상 날때
	{
		up.mStartTick = up.mEndTick;	// 엔드에 있는 위치를 처음부분으로 재설정한다.
		return TRUE;
	}

	return FALSE;
}

void Game::Load(HANDLES & h, LPCWSTR name)
{
	h.hbmp = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);		// 비트맵 핸들 얻어오는 과정
	if (h.hbmp == NULL)
	{
		MessageBox(NULL, name, L"비트맵 로딩 에러", MB_OK);
	}
	h.dc = CreateCompatibleDC(m_Base.dc);	// m_Base.dc 에 호환되는 DC 얻기
	h.hOldbmp = (HBITMAP)SelectObject(h.dc, h.hbmp);	// dc 랑 비트맵과 연결고리를 맺음

	GetObject(h.hbmp, sizeof(BITMAP), &h.bit);	// 비트맵 핸들로 비트맵 구조체 필드를 채움
}

RECT & Game::getRECT()
{
	return m_rtClient;
}
