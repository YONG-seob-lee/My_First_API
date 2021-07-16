#pragma once
#include "framework.h"

class Game
{
private:
	HANDLES m_Base;	// 화면에 호환되는 기본 버퍼
	HANDLES m_BG;	// background 배경 비트맵
	HANDLES m_Hero;	// 주인공 비트맵
	HANDLES m_Enemy;	// 적 비트맵
	HANDLES m_Bullet;	// 총알 비트맵
	HANDLES m_Explode;	// 폭발 이펙트
	HANDLES m_EnBullet;	// 적 총알 비트맵
	HANDLES m_Title;	// 타이틀
	HANDLES m_TitleName;	// 타이틀 네임
	HANDLES m_Press;	// 'press any key'
	HANDLES m_Ready;
	HANDLES m_Start;
	HANDLES m_Pause;

	// util 변수
	RECT m_rtClient;	// 화면 사각형 받아오는 변수
	int m_nGameState;	// 현재 게임 상황(게임 중인지 or 타이틀 화면인지 확인하기 위한 변수

	
	////////////// updatd 함수들	/////////////////
	UPDATE m_uEnemy;
	UPDATE m_uHero;
	UPDATE m_uBullet;
	UPDATE m_uEnBullet;
	UPDATE m_uExplode;
	UPDATE m_uPress;
	UPDATE m_uReady;
	UPDATE m_uStart;
	UPDATE m_uPause;






public:
	Game();
	~Game();


	////////////// basic 함수들 /////////////////////
	void GameLoop(HDC hdc);	// 메인 루프에서 호출되는 함수
	void DrawAll(HDC hdc);		// 화면 찍어내는 함수
	void Update();		// 화면 좌표 갱신 담당 함수

	////////////// init 함수들  /////////////////////
	void InitAll();				// 각종 데이터 초기화 (스테이지 바뀔때 마다 호출)
	void InitGame();			// 게임 실행시 최초로 처음에만 초기화 되는 멤버들의 집합 멤버함수
	void InitResource();
	void InitHero();
	void InitEnemy();
	void InitBullet();
	void InitExplode();

	/////////////  draw 함수들	/////////////////////
	void DrawBG();
	void DrawHero();
	void DrawEnemy();
	void DrawBullet();
	void DrawExplode();
	void DrawTitle();
	void DrawTitleName();
	void DrawPress();
	void DrawReady();
	void DrawStart();
	void DrawPause();

	///////////// update 함수들	/////////////////////
	void UpdateEnemy();
	void UpdateHero(int speed);
	void UpdateBullet();
	void UpdateExplode();
	void UpdatePress();
	void UpdateReady();
	void UpdateStart();
	void UpdatePause();

	void InitTick(UPDATE& up, int mode = 0);	// 0 : all, 1 : move, 2 : animated
	BOOL UpdateCheck(UPDATE& up, DWORD tick);

	/////////////  util 함수들	//////////////////////
	void Load(HANDLES& h, LPCWSTR name);
	RECT& getRECT();
	void AttariCheck();	// 판정요건
	BOOL KeyCheck();	// 키 체크 함수

};

