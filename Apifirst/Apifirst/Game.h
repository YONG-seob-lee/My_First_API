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

	// util 변수
	RECT m_rtClient;	// 화면 사각형 받아오는 변수
	
	////////////// updatd 함수들	/////////////////
	UPDATE m_uEnemy;
	UPDATE m_uHero;
	UPDATE m_uBullet;

public:
	////////////// basic 함수들 /////////////////////
	void GameLoop(HDC hdc);	// 메인 루프에서 호출되는 함수
	void DrawAll(HDC hdc);		// 화면 찍어내는 함수
	void Update();		// 화면 좌표 갱신 담당 함수

	////////////// init 함수들  /////////////////////
	void InitAll();				// 각종 데이터 초기화 (스테이지 바뀔때 마다 호출)
	void InitResource();
	void InitHero();
	void InitEnemy();
	void InitBullet();

	/////////////  draw 함수들	/////////////////////
	void DrawBG();
	void DrawHero();
	void DrawEnemy();
	void DrawBullet();

	///////////// update 함수들	/////////////////////
	void UpdateEnemy();
	void UpdateHero(int speed);
	void UpdateBullet();
	BOOL UpdateCheck(UPDATE& up, DWORD tick);

	/////////////  util 함수들	//////////////////////
	void Load(HANDLES& h, LPCWSTR name);
	RECT& getRECT();

};

