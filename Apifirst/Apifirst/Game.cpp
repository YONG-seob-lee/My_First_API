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
		// 화면DC, x위치, y위치, 너비, 높이, dc의 x위치, y 위치, 찍는 방법
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
	m_uEnemy.mStartTick = m_uEnemy.mEndTick = GetTickCount();		// 적 위치 매 Tick 마다 초기화
	m_uHero.mStartTick = m_uHero.mEndTick = GetTickCount();			// 아군 위치 매 Tick 마다 초기화
	m_uHero.bTrigger = FALSE;	// 총알 발사 상태가 아님으로 초기화 (TRUE 시 발사)

	m_uBullet.mStartTick = m_uBullet.mEndTick = GetTickCount();		// 총알 위치 매 Tick 마다 초기화

	m_uExplode.aEndTick = m_uExplode.aStartTick = GetTickCount();	// ani tick 초기화
	m_uExplode.bTrigger = FALSE;	// 폭발 이펙트가 발동이 되었는지 확인하는 줄
	m_uExplode.Frame = -1;		// 0 부터 실제 그림 프레임 (첫번째 프레임이 씹힐까봐 -1로 초기화) 시작은 ' 0 ' 으로 시작해야하기 때문
	m_uExplode.MaxFrame = 5;	// 0 부터 시작해서 6까지 그림 붙이기
	m_uExplode.size = 199;		// 한 프레임 가로 사이즈

	m_uPress.bTrigger = TRUE;	// 참이면 화면에 보이고 거짓이면 화면에 안보이게 하려는 속셈

	InitTick(m_uReady, 0);

	m_uPause.bTrigger = FALSE;	// 일시정지가 아닌 상태
}

void Game::InitGame()
{
	m_nGameState = GAME_TITLE;		// 시작부분
	
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

void Game::DrawBullet()
{
	if (m_uHero.bTrigger == FALSE)	return;

	TransparentBlt(m_Base.dc, m_Bullet.pt.x, m_Bullet.pt.y, m_Bullet.bit.bmWidth, m_Bullet.bit.bmHeight,
		m_Bullet.dc, 0, 0, m_Bullet.bit.bmWidth, m_Bullet.bit.bmHeight, RGB(255, 0, 255));
}

void Game::DrawExplode()
{
	if (m_uExplode.bTrigger == FALSE)	return;		// 폭발 중이 아니면 do not draw

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
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)	// 키가 눌린경우
		{
			if (m_uHero.bTrigger == FALSE)	// 발사상태 아니면
			{
				InitBullet();		// 총알 초기화
				m_uHero.bTrigger = TRUE;	// 발사중으로 상태를 바꿔줌
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
		// 피격 판정 체크하는 함수
		AttariCheck();
	}
	// 화면에서 벗어나면 초기화
	if (m_Bullet.pt.y + m_Bullet.bit.bmHeight < 0)
	{
		InitBullet();
		m_uHero.bTrigger = FALSE;	// 주인공 슈팅 상태 off
	}
}

void Game::UpdateExplode()
{
	if (m_uExplode.bTrigger == FALSE)	return;

	m_uExplode.aEndTick = GetTickCount();		// 엔드 Tick 에다가 카운트 확인하는것을 설정
	if (m_uExplode.aEndTick - m_uExplode.aStartTick > 100)		// 0.1 초 이상 차이가 날때
	{
		m_uExplode.aStartTick = m_uExplode.aEndTick;
		m_uExplode.Frame++;
		InitExplode();		// 폭발 초기화

		if (m_uExplode.Frame > m_uExplode.MaxFrame)	// 프레임이 맥스프레임까지 도달했으면
		{
			m_uExplode.Frame = -1;	// 프레임 초기화
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
	if (UpdateCheck(m_uReady, 2000) == TRUE)	// 2000 밀리 세컨드 만큼 ready 그림 띄우기
	{
		m_nGameState = GAME_START;
		InitTick(m_uStart);		// 스타트 업데이트 변수 초기화

	}
}

void Game::UpdateStart()
{
	if (UpdateCheck(m_uStart, 1500) == TRUE)	// 2000 밀리 세컨드 만큼 ready 그림 띄우기
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

void Game::AttariCheck()
{
	POINT pt;
	pt.x = m_Bullet.pt.x + m_Bullet.bit.bmWidth / 2;
	pt.y = m_Bullet.pt.y;	// 총알 맨 위에 닿으면 판정이 나야하기때문에 총알의 맨 위쪽으로 체크

	// PtlnRect 로 호출 할 때 주의사항
	// 첫 번째 인자로 사각형 넣을때 이 함수 내에서 자체 계산 후 넣기
	RECT rt;
	rt.left = m_Enemy.pt.x;
	rt.right = rt.left + m_Enemy.bit.bmWidth;
	rt.top = m_Enemy.pt.y;
	rt.bottom = rt.top + m_Enemy.bit.bmHeight;

	if (PtInRect(&rt, pt) == TRUE)		// 적을 맞추면 TRUE
	{
		InitBullet();	// 총알 초기화
		m_uHero.bTrigger = FALSE;	// 발사 상태 초기화
		// 폭발 이펙트 활성화
		m_uExplode.bTrigger = TRUE;
		// 폭발 위치 초기화
		InitExplode();
		// 스코어 증가
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
