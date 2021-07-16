#pragma once
#include "framework.h"

class Game
{
private:
	HANDLES m_Base;	// ȭ�鿡 ȣȯ�Ǵ� �⺻ ����
	HANDLES m_BG;	// background ��� ��Ʈ��
	HANDLES m_Hero;	// ���ΰ� ��Ʈ��
	HANDLES m_Enemy;	// �� ��Ʈ��
	HANDLES m_Bullet;	// �Ѿ� ��Ʈ��
	HANDLES m_Explode;	// ���� ����Ʈ
	HANDLES m_EnBullet;	// �� �Ѿ� ��Ʈ��
	HANDLES m_Title;	// Ÿ��Ʋ
	HANDLES m_TitleName;	// Ÿ��Ʋ ����
	HANDLES m_Press;	// 'press any key'
	HANDLES m_Ready;
	HANDLES m_Start;
	HANDLES m_Pause;

	// util ����
	RECT m_rtClient;	// ȭ�� �簢�� �޾ƿ��� ����
	int m_nGameState;	// ���� ���� ��Ȳ(���� ������ or Ÿ��Ʋ ȭ������ Ȯ���ϱ� ���� ����

	
	////////////// updatd �Լ���	/////////////////
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


	////////////// basic �Լ��� /////////////////////
	void GameLoop(HDC hdc);	// ���� �������� ȣ��Ǵ� �Լ�
	void DrawAll(HDC hdc);		// ȭ�� ���� �Լ�
	void Update();		// ȭ�� ��ǥ ���� ��� �Լ�

	////////////// init �Լ���  /////////////////////
	void InitAll();				// ���� ������ �ʱ�ȭ (�������� �ٲ� ���� ȣ��)
	void InitGame();			// ���� ����� ���ʷ� ó������ �ʱ�ȭ �Ǵ� ������� ���� ����Լ�
	void InitResource();
	void InitHero();
	void InitEnemy();
	void InitBullet();
	void InitExplode();

	/////////////  draw �Լ���	/////////////////////
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

	///////////// update �Լ���	/////////////////////
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

	/////////////  util �Լ���	//////////////////////
	void Load(HANDLES& h, LPCWSTR name);
	RECT& getRECT();
	void AttariCheck();	// �������
	BOOL KeyCheck();	// Ű üũ �Լ�

};

