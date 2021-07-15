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

	// util ����
	RECT m_rtClient;	// ȭ�� �簢�� �޾ƿ��� ����
	
	////////////// updatd �Լ���	/////////////////
	UPDATE m_uEnemy;
	UPDATE m_uHero;
	UPDATE m_uBullet;

public:
	////////////// basic �Լ��� /////////////////////
	void GameLoop(HDC hdc);	// ���� �������� ȣ��Ǵ� �Լ�
	void DrawAll(HDC hdc);		// ȭ�� ���� �Լ�
	void Update();		// ȭ�� ��ǥ ���� ��� �Լ�

	////////////// init �Լ���  /////////////////////
	void InitAll();				// ���� ������ �ʱ�ȭ (�������� �ٲ� ���� ȣ��)
	void InitResource();
	void InitHero();
	void InitEnemy();
	void InitBullet();

	/////////////  draw �Լ���	/////////////////////
	void DrawBG();
	void DrawHero();
	void DrawEnemy();
	void DrawBullet();

	///////////// update �Լ���	/////////////////////
	void UpdateEnemy();
	void UpdateHero(int speed);
	void UpdateBullet();
	BOOL UpdateCheck(UPDATE& up, DWORD tick);

	/////////////  util �Լ���	//////////////////////
	void Load(HANDLES& h, LPCWSTR name);
	RECT& getRECT();

};

