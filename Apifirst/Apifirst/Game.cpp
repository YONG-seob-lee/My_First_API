#include "Game.h"

Game::Game()
{
	InitGame();
}

Game::~Game()
{
}

void Game::GameLoop(HDC hdc)
{
	DrawAll(hdc);
	Update();
}

void Game::DrawAll(HDC hdc)
{
	switch (m_nGameState)
	{
	case GAME_TITLE:
		DrawTitle();
		DrawTitleName();
		DrawPress();
		break;

	case GAME_READY:
		DrawBG();
		DrawHero();
		DrawEnemy();
		DrawReady();
		break;

	case GAME_START:
		DrawBG();
		DrawHero();
		DrawEnemy();
		DrawStart();
		break;

	case GAME_MAIN:
		DrawBG();
		DrawHero();
		DrawEnemy();
		DrawBullet();
		DrawExplode();
		DrawPause();
		break;

	default:
		break;
	}

	

	// base->g_hdc
	BitBlt(hdc, 0, 0, m_Base.bit.bmWidth, m_Base.bit.bmHeight, m_Base.dc, 0, 0, SRCCOPY);
		// ȭ��DC, x��ġ, y��ġ, �ʺ�, ����, dc�� x��ġ, y ��ġ, ��� ���
}

void Game::Update()
{
	switch (m_nGameState)
	{
	case GAME_TITLE:
		UpdatePress();
		break;

	case GAME_READY:
		UpdateReady();
		break;

	case GAME_START:
		UpdateStart();
		break;

	case GAME_MAIN:
		if (m_uPause.bTrigger == FALSE)
		{
			UpdateEnemy();
			UpdateHero(HEROMOVESIZE);
			UpdateBullet();
			UpdateExplode();
		}
		
		break;

	default:
		break;
	}
	
}

void Game::InitAll()
{
	InitHero();
	InitEnemy();
	m_uEnemy.dir = DIR_LEFT;
	m_uEnemy.mStartTick = m_uEnemy.mEndTick = GetTickCount();		// �� ��ġ �� Tick ���� �ʱ�ȭ
	m_uHero.mStartTick = m_uHero.mEndTick = GetTickCount();			// �Ʊ� ��ġ �� Tick ���� �ʱ�ȭ
	m_uHero.bTrigger = FALSE;	// �Ѿ� �߻� ���°� �ƴ����� �ʱ�ȭ (TRUE �� �߻�)

	m_uBullet.mStartTick = m_uBullet.mEndTick = GetTickCount();		// �Ѿ� ��ġ �� Tick ���� �ʱ�ȭ

	m_uExplode.aEndTick = m_uExplode.aStartTick = GetTickCount();	// ani tick �ʱ�ȭ
	m_uExplode.bTrigger = FALSE;	// ���� ����Ʈ�� �ߵ��� �Ǿ����� Ȯ���ϴ� ��
	m_uExplode.Frame = -1;		// 0 ���� ���� �׸� ������ (ù��° �������� ������� -1�� �ʱ�ȭ) ������ ' 0 ' ���� �����ؾ��ϱ� ����
	m_uExplode.MaxFrame = 5;	// 0 ���� �����ؼ� 6���� �׸� ���̱�
	m_uExplode.size = 199;		// �� ������ ���� ������

	m_uPress.bTrigger = TRUE;	// ���̸� ȭ�鿡 ���̰� �����̸� ȭ�鿡 �Ⱥ��̰� �Ϸ��� �Ӽ�

	InitTick(m_uReady, 0);

	m_uPause.bTrigger = FALSE;	// �Ͻ������� �ƴ� ����
}

void Game::InitGame()
{
	m_nGameState = GAME_TITLE;		// ���ۺκ�
	
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

////////////////////////			m_Hero & m_Enemy part		////////////////////////////////
	Load(m_Hero, L"image/pikkachu.bmp");
	Load(m_Enemy, L"image/huny.bmp");
	

////////////////////////			m_Bullet part				////////////////////////////////
	Load(m_Bullet, L"image/thunder.bmp");

////////////////////////			m_Exploid part				////////////////////////////////
	Load(m_Explode, L"image/Explosive_bomb.bmp");

////////////////////////			m_Title part				////////////////////////////////
	Load(m_Title, L"image/title.bmp");
	Load(m_TitleName, L"image/TitleName.bmp");
	Load(m_Press, L"image/Press.bmp");

////////////////////////			util part				////////////////////////////////
	Load(m_Ready, L"image/Ready.bmp");
	Load(m_Start, L"image/Start.bmp");
	Load(m_Pause, L"image/Pause.bmp");
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

void Game::InitBullet()
{
	m_Bullet.pt.x = m_Hero.pt.x + m_Hero.bit.bmWidth / 2 - m_Bullet.bit.bmWidth / 2;
	m_Bullet.pt.y = m_Hero.pt.y;
}

void Game::InitExplode()
{
	m_Explode.pt.x = m_Enemy.pt.x + m_Enemy.bit.bmWidth / 5;
	m_Explode.pt.y = m_Enemy.pt.y ;
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

void Game::DrawBullet()
{
	if (m_uHero.bTrigger == FALSE)	return;

	TransparentBlt(m_Base.dc, m_Bullet.pt.x, m_Bullet.pt.y, m_Bullet.bit.bmWidth, m_Bullet.bit.bmHeight,
		m_Bullet.dc, 0, 0, m_Bullet.bit.bmWidth, m_Bullet.bit.bmHeight, RGB(255, 0, 255));
}

void Game::DrawExplode()
{
	if (m_uExplode.bTrigger == FALSE)	return;		// ���� ���� �ƴϸ� do not draw

	TransparentBlt(m_Base.dc, m_Explode.pt.x, m_Explode.pt.y, m_uExplode.size, m_Explode.bit.bmHeight,
		m_Explode.dc, m_uExplode.size * m_uExplode.Frame, 0, m_uExplode.size, m_Explode.bit.bmHeight, RGB(255, 0, 255));
}

void Game::DrawTitle()
{
	BitBlt(m_Base.dc, 0, 0, m_Title.bit.bmWidth, m_Title.bit.bmHeight, m_Title.dc, 0, 0, SRCCOPY);
}

void Game::DrawTitleName()
{
	TransparentBlt(m_Base.dc, m_TitleName.pt.x, m_TitleName.pt.y, m_TitleName.bit.bmWidth, m_TitleName.bit.bmHeight,
		m_TitleName.dc, 0, 0, m_TitleName.bit.bmWidth, m_TitleName.bit.bmHeight, RGB(255, 0, 255));
}

void Game::DrawPress()
{
	if (m_uPress.bTrigger == FALSE)	return;

	int x = m_rtClient.right / 2 - m_Press.bit.bmWidth / 2;
	int y = 500;

	TransparentBlt(m_Base.dc, x, y, m_Press.bit.bmWidth, m_Press.bit.bmHeight,
		m_Press.dc, 0, 0, m_Press.bit.bmWidth, m_Press.bit.bmHeight, RGB(255, 0, 255));
}

void Game::DrawReady()
{
	int x = m_rtClient.right / 2 - m_Ready.bit.bmWidth / 2;
	int y = m_rtClient.bottom / 2 - m_Ready.bit.bmHeight / 2;

	TransparentBlt(m_Base.dc, x, y, m_Ready.bit.bmWidth, m_Ready.bit.bmHeight,
		m_Ready.dc, 0, 0, m_Ready.bit.bmWidth, m_Ready.bit.bmHeight, RGB(255, 0, 255));
}

void Game::DrawStart()
{
	int x = m_rtClient.right / 2 - m_Start.bit.bmWidth / 2;
	int y = m_rtClient.bottom / 2 - m_Start.bit.bmHeight / 2;

	TransparentBlt(m_Base.dc, x, y, m_Start.bit.bmWidth, m_Start.bit.bmHeight,
		m_Start.dc, 0, 0, m_Start.bit.bmWidth, m_Start.bit.bmHeight, RGB(255, 0, 255));
}

void Game::DrawPause()
{
	if (m_uPause.bTrigger == FALSE)	return;

	int x = m_rtClient.right / 2 - m_Pause.bit.bmWidth / 2;
	int y = m_rtClient.bottom / 2 - m_Pause.bit.bmHeight / 2;

	TransparentBlt(m_Base.dc, x, y, m_Pause.bit.bmWidth, m_Pause.bit.bmHeight,
		m_Pause.dc, 0, 0, m_Pause.bit.bmWidth, m_Pause.bit.bmHeight, RGB(255, 0, 255));
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
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)	// Ű�� �������
		{
			if (m_uHero.bTrigger == FALSE)	// �߻���� �ƴϸ�
			{
				InitBullet();		// �Ѿ� �ʱ�ȭ
				m_uHero.bTrigger = TRUE;	// �߻������� ���¸� �ٲ���
			}

		}
	}
	
}

void Game::UpdateBullet()
{
	if (UpdateCheck(m_uBullet, 50) == TRUE)
	{
		if (m_uHero.bTrigger == TRUE)
		{
			m_Bullet.pt.y -= BULSPEED;
		}
		// �ǰ� ���� üũ�ϴ� �Լ�
		AttariCheck();
	}
	// ȭ�鿡�� ����� �ʱ�ȭ
	if (m_Bullet.pt.y + m_Bullet.bit.bmHeight < 0)
	{
		InitBullet();
		m_uHero.bTrigger = FALSE;	// ���ΰ� ���� ���� off
	}
}

void Game::UpdateExplode()
{
	if (m_uExplode.bTrigger == FALSE)	return;

	m_uExplode.aEndTick = GetTickCount();		// ���� Tick ���ٰ� ī��Ʈ Ȯ���ϴ°��� ����
	if (m_uExplode.aEndTick - m_uExplode.aStartTick > 100)		// 0.1 �� �̻� ���̰� ����
	{
		m_uExplode.aStartTick = m_uExplode.aEndTick;
		m_uExplode.Frame++;
		InitExplode();		// ���� �ʱ�ȭ

		if (m_uExplode.Frame > m_uExplode.MaxFrame)	// �������� �ƽ������ӱ��� ����������
		{
			m_uExplode.Frame = -1;	// ������ �ʱ�ȭ
			m_uExplode.bTrigger = FALSE;
		}
	}
}

void Game::UpdatePress()
{
	if (UpdateCheck(m_uPress, 500) == TRUE)
	{
		m_uPress.bTrigger = !m_uPress.bTrigger;
	}
}

void Game::UpdateReady()
{
	if (UpdateCheck(m_uReady, 2000) == TRUE)	// 2000 �и� ������ ��ŭ ready �׸� ����
	{
		m_nGameState = GAME_START;
		InitTick(m_uStart);		// ��ŸƮ ������Ʈ ���� �ʱ�ȭ

	}
}

void Game::UpdateStart()
{
	if (UpdateCheck(m_uStart, 1500) == TRUE)	// 2000 �и� ������ ��ŭ ready �׸� ����
	{
		m_nGameState = GAME_MAIN;
	}
}

void Game::UpdatePause()
{
	if (m_nGameState == GAME_MAIN)
	{
		m_uPause.bTrigger = !m_uPause.bTrigger;
	}
}

void Game::InitTick(UPDATE & up, int mode)
{
	switch (mode)
	{
	case 0:
		up.mStartTick = up.mEndTick = up.aEndTick = up.aStartTick = GetTickCount();

		break;
	default:
		break;
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

void Game::AttariCheck()
{
	POINT pt;
	pt.x = m_Bullet.pt.x + m_Bullet.bit.bmWidth / 2;
	pt.y = m_Bullet.pt.y;	// �Ѿ� �� ���� ������ ������ �����ϱ⶧���� �Ѿ��� �� �������� üũ

	// PtlnRect �� ȣ�� �� �� ���ǻ���
	// ù ��° ���ڷ� �簢�� ������ �� �Լ� ������ ��ü ��� �� �ֱ�
	RECT rt;
	rt.left = m_Enemy.pt.x;
	rt.right = rt.left + m_Enemy.bit.bmWidth;
	rt.top = m_Enemy.pt.y;
	rt.bottom = rt.top + m_Enemy.bit.bmHeight;

	if (PtInRect(&rt, pt) == TRUE)		// ���� ���߸� TRUE
	{
		InitBullet();	// �Ѿ� �ʱ�ȭ
		m_uHero.bTrigger = FALSE;	// �߻� ���� �ʱ�ȭ
		// ���� ����Ʈ Ȱ��ȭ
		m_uExplode.bTrigger = TRUE;
		// ���� ��ġ �ʱ�ȭ
		InitExplode();
		// ���ھ� ����
	}
}

BOOL Game::KeyCheck()
{
	if (m_nGameState == GAME_TITLE)
	{
		m_nGameState = GAME_READY;
		InitTick(m_uReady);
		return TRUE;
	}

	return FALSE;
}
