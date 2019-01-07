/******************************************************************************************************
**                                                                                                   **
**    D3D12 Donuts��Ϸ������                                                                         **
**                                                                                                   **
**              ��������ʩ xx_player   2019.1                                                        **
**                                                                                                   ** 
******************************************************************************************************/
#include "stdafx.h"
#include "CGames.h"
#include "CGraphics.h"
#include <vector>
#include <string>
#include <fstream>
using namespace std;

CGames::CGames(CGraphics*sender) {
	m_sender = sender;
	m_uLevel = 1;
	m_uScore = 0;
	m_nLife = 5;
	m_nProgstatus = PStatus::GAMEMENU;
	m_fDelayCount = 0;
	m_uStars = 0;
	m_bWantSound = TRUE;
	NullFile = std::make_shared< std::vector<byte> >(std::vector<byte>());

	m_nDelayTimes = 10;
	ZeroMemory(dwPresskeyDelay,256);
}
CGames::~CGames()
{


}
VOID CGames::Initialize(UINT i)
{
	//��̥64x64,���� 0 �����100
	for (UINT j = 0;j < 100;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(64.0f, 64.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, StageZ ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(4.0f, 4.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 30;
		m_objectCollections[i][j].type = 0;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//����32x32,���� 1 �����100
	for (UINT j = 100;j < 200;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices,XMMatrixScaling(32.0f,32.0f,0.0f)* XMMatrixTranslation(0.0f,0.0f, StageZ ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(2.0f, 2.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 10;
		m_objectCollections[i][j].type = 1;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//����32x32,���� 2 �����100
	for (UINT j = 200;j < 300;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(32.0f, 32.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, StageZ ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(2.0f, 2.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 10;
		m_objectCollections[i][j].type = 2;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//����32x32,���� 3 �����100
	for (UINT j = 300;j < 400;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(32.0f, 32.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, StageZ ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(2.0f, 2.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 10;
		m_objectCollections[i][j].type = 3;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//����32x32,���� 4 �����100
	for (UINT j = 400;j < 500;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(32.0f, 32.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, StageZ ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(2.0f, 2.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 10;
		m_objectCollections[i][j].type = 4;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//�ɿ���16x16������ 5, �����100
	for (UINT j = 500;j < 600;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, StageZ ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 20;
		m_objectCollections[i][j].type = 5;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//�ɿ���16x16������ 6, �����100
	for (UINT j = 600;j < 700;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, StageZ ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 20;
		m_objectCollections[i][j].type = 6;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//�ɿ���16x16������ 7, �����100
	for (UINT j = 700;j < 800;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, StageZ ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 20;
		m_objectCollections[i][j].type = 7;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//�ɿ���16x16������ 8, �����100
	for (UINT j = 800;j < 900;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, StageZ ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 20;
		m_objectCollections[i][j].type = 8;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//���32x32,���� 9, �����1
	for (UINT j = 900;j < 901;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(32.0f, 32.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, StageZ ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(2.0f, 2.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 40;
		m_objectCollections[i][j].type = 9;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//�����32,���� 10, �����1
	for (UINT j = 901;j < 902;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(32.0f, 32.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, StageZ ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(2.0f, 2.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 40;
		m_objectCollections[i][j].type = 10;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//��
	for (UINT j = 902;j < 999;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixTranslation(0.0f, 0.0f, StageZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//�÷ֱ�ʶ
	for (UINT j = 999;j < 1000;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixTranslation(0.0f, 0.0f, StageZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//��ֵ0 16x16 ,���� 20, �����1
	for (UINT j = 1000;j < 1100;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 20;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//��ֵ1 16x16 ,���� 21, �����1
	for (UINT j = 1100;j < 1200;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 21;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//��ֵ2 16x16 ,���� 22, �����1
	for (UINT j = 1200;j < 1300;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 22;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//��ֵ3 16x16 ,���� 23, �����1
	for (UINT j = 1300;j < 1400;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 23;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//��ֵ4 16x16 ,���� 24, �����1
	for (UINT j = 1400;j < 1500;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 24;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//��ֵ5 16x16 ,���� 25, �����1
	for (UINT j = 1500;j < 1600;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 25;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//��ֵ6 16x16 ,���� 26, �����1
	for (UINT j = 1600;j < 1700;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 26;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//��ֵ7 16x16 ,���� 27, �����1
	for (UINT j = 1700;j < 1800;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 27;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//��ֵ8 16x16 ,���� 28, �����1
	for (UINT j = 1800;j < 1900;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 28;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//��ֵ9 16x16 ,���� 29, �����1
	for (UINT j = 1900;j < 2000;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 29;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}

	//�ؿ���ĸL 16x16 ,���� 30, �����1
	for (UINT j = 2000;j < 2010;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 30;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//�ؿ���ĸE 16x16 ,���� 31, �����1
	for (UINT j = 2010;j < 2020;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 31;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//�ؿ���ĸV 16x16 ,���� 32, �����1
	for (UINT j = 2020;j < 2030;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 32;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//��
	for (UINT j = 2030;j < 3000;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//����V 16x16 ,���� 40, �����1
	for (UINT j = 3000;j < 3001;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 40;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//�ӵ�4x16 ,���� 50, �����1 ,����=51Ϊ��ը
	for (UINT j = 4000;j < 4050;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, StageZ ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f,1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 4;
		m_objectCollections[i][j].type = 50;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//������
	for (UINT j = 4050;j < 4099;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(8.0f, 8.0f, 0.0f)*XMMatrixTranslation(0.0f, 0.0f, StageZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(4.0f, 4.0f, 0.0f)*XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 45;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//�����Ǳ�ʶ
	for (UINT j = 4099;j < 4100;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(8.0f, 8.0f, 0.0f)*XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(4.0f, 4.0f, 0.0f)*XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//����
	for (UINT j = 4100;j < 4105;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}

	//�˵�splash
	for (UINT j = 4105;j < 4106;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 60;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 1;
	}
	//��
	for (UINT j = 4106;j < 4150;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 0;
	}
	//ͨ��ʧ��/�ɹ���ʾ(ע�⣺��ʾ���þ���Ҫ��֤����ʹ����Ψһ��)
	for (UINT j = 4150;j < 4151;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 70;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 2;
	}
	//��
	for (UINT j = 4151;j < 5000;j++) {
		m_objectCollections[i][j].status = 0;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].type = 99;
	}
}

VOID CGames::AddObject(UINT i, UINT type,float fsx,float fsy,float fdx,float fdy, UINT num) {

	for (UINT k = 0;k < num;k++)
	{
		switch (type) {
		case 0:
		{
			for (UINT j = 0;j < 100;j++)
			{
				if (m_objectCollections[i][j].status == 0) {
					m_objectCollections[i][j].status = 1;
					//���Դλ��
					if(fsx==0) fsx = (float)(rand() % (800-64));
					if(fsy==0) fsy = (float)(rand() % (600-64));
					XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(64.0f, 64.0f, 0.0f)* XMMatrixTranslation(fsx, fsy, StageZ ));
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(4.0f, 4.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
					//���Ŀ��λ��
					if(fdx==0) fdx = (float)(rand() % (800-64));
					if(fdy==0) fdy = (float)(rand() % (600-64));
					m_objectCollections[i][j].fxdst = fdx;
					m_objectCollections[i][j].fydst = fdy;
					fsx = fsy = fdx = fdy = 0;
					break;
				}
			}
		}
		break;
		case 1:
		{
			for (UINT j = 100;j < 200;j++)
			{
				if (m_objectCollections[i][j].status == 0) {
					m_objectCollections[i][j].status = 1;
					if (fsx == 0) fsx = (float)(rand() % (800 - 32));
					if (fsy == 0) fsy = (float)(rand() % (600 - 32));
					XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(32.0f, 32.0f, 0.0f)* XMMatrixTranslation(fsx, fsy, StageZ ));
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(2.0f, 2.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
					if (fdx == 0) fdx = (float)(rand() % (800 - 32));
					if (fdy == 0) fdy = (float)(rand() % (600 - 32));
					m_objectCollections[i][j].fxdst = fdx;
					m_objectCollections[i][j].fydst = fdy;
					fdx = fdy = 0;
					break;
				}
			}
		}
		break;
		case 2:
		{
			for (UINT j = 200;j < 300;j++)
			{
				if (m_objectCollections[i][j].status == 0) {
					m_objectCollections[i][j].status = 1;
					if (fsx == 0) fsx = (float)(rand() % (800 - 32));
					if (fsy == 0) fsy = (float)(rand() % (600 - 32));
					XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(32.0f, 32.0f, 0.0f)* XMMatrixTranslation(fsx, fsy, StageZ ));
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(2.0f, 2.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
					if (fdx == 0) fdx = (float)(rand() % (800 - 32));
					if (fdy == 0) fdy = (float)(rand() % (600 - 32));
					m_objectCollections[i][j].fxdst = fdx;
					m_objectCollections[i][j].fydst = fdy;
					fdx = fdy = 0;
					break;
				}
			}
		}
		break;
		case 3:
		{
			for (UINT j = 300;j < 400;j++)
			{
				if (m_objectCollections[i][j].status == 0) {
					m_objectCollections[i][j].status = 1;
					if (fsx == 0) fsx = (float)(rand() % (800 - 32));
					if (fsy == 0) fsy = (float)(rand() % (600 - 32));
					XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(32.0f, 32.0f, 0.0f)* XMMatrixTranslation(fsx, fsy, StageZ ));
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(2.0f, 2.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
					if (fdx == 0) fdx = (float)(rand() % (800 - 32));
					if (fdy == 0) fdy = (float)(rand() % (600 - 32));
					m_objectCollections[i][j].fxdst = fdx;
					m_objectCollections[i][j].fydst = fdy;
					fdx = fdy = 0;
					break;
				}
			}
		}
		break;
		case 4:
		{
			for (UINT j = 400;j < 500;j++)
			{
				if (m_objectCollections[i][j].status == 0) {
					m_objectCollections[i][j].status = 1;
					if (fsx == 0) fsx = (float)(rand() % (800 - 32));
					if (fsy == 0) fsy = (float)(rand() % (600 - 32));
					XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(32.0f, 32.0f, 0.0f)* XMMatrixTranslation(fsx, fsy, StageZ ));
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(2.0f, 2.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
					if (fdx == 0) fdx = (float)(rand() % (800 - 32));
					if (fdy == 0) fdy = (float)(rand() % (600 - 32));
					m_objectCollections[i][j].fxdst = fdx;
					m_objectCollections[i][j].fydst = fdy;
					fdx = fdy = 0;
					break;
				}
			}
		}
		break;	
		case 5:
		{
			for (UINT j = 500;j < 600;j++)
			{
				if (m_objectCollections[i][j].status == 0) {
					m_objectCollections[i][j].status = 1;
					if (fsx == 0) fsx = (float)(rand() % (800 - 16));
					if (fsy == 0) fsy = (float)(rand() % (600 - 16));
					XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(fsx, fsy, StageZ ));
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
					if (fdx == 0) fdx = (float)(rand() % (800 - 16));
					if (fdy == 0) fdy = (float)(rand() % (600 - 16));
					m_objectCollections[i][j].fxdst = fdx;
					m_objectCollections[i][j].fydst = fdy;
					fdx = fdy = 0;
					break;
				}
			}
		}
		break;
		case 6:
		{
			for (UINT j = 600;j < 700;j++)
			{
				if (m_objectCollections[i][j].status == 0) {
					m_objectCollections[i][j].status = 1;
					if (fsx == 0) fsx = (float)(rand() % (800 - 16));
					if (fsy == 0) fsy = (float)(rand() % (600 - 16));
					XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(fsx, fsy, StageZ ));
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
					if (fdx == 0) fdx = (float)(rand() % (800 - 16));
					if (fdy == 0) fdy = (float)(rand() % (600 - 16));
					m_objectCollections[i][j].fxdst = fdx;
					m_objectCollections[i][j].fydst = fdy;
					fdx = fdy = 0;
					break;
				}
			}
		}
		break;
		case 7:
		{
			for (UINT j = 700;j < 800;j++)
			{
				if (m_objectCollections[i][j].status == 0) {
					m_objectCollections[i][j].status = 1;
					if (fsx == 0) fsx = (float)(rand() % (800 - 16));
					if (fsy == 0) fsy = (float)(rand() % (600 - 16));
					XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(fsx, fsy, StageZ ));
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
					if (fdx == 0) fdx = (float)(rand() % (800 - 16));
					if (fdy == 0) fdy = (float)(rand() % (600 - 16));
					m_objectCollections[i][j].fxdst = fdx;
					m_objectCollections[i][j].fydst = fdy;
					fdx = fdy = 0;
					break;
				}
			}
		}
		break;
		case 8:
		{
			for (UINT j = 800;j < 900;j++)
			{
				if (m_objectCollections[i][j].status == 0) {
					m_objectCollections[i][j].status = 1;
					if (fsx == 0) fsx = (float)(rand() % (800 - 16));
					if (fsy == 0) fsy = (float)(rand() % (600 - 16));
					XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(16.0f, 16.0f, 0.0f)* XMMatrixTranslation(fsx, fsy, StageZ ));
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(1.0f, 1.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
					if (fdx == 0) fdx = (float)(rand() % (800 - 16));
					if (fdy == 0) fdy = (float)(rand() % (600 - 16));
					m_objectCollections[i][j].fxdst = fdx;
					m_objectCollections[i][j].fydst = fdy;
					fdx = fdy = 0;
					break;
				}
			}
		}
		break;
		}

	}
}

VOID CGames::AddBomb(UINT i, float fx, float fy,float fspeed)
{
	UINT num = 8 + rand() % 8;
	float fa = XM_2PI / num - XM_PI;
	float fdir = 0;
	float ftx, fty,fsy;
	XMMATRIX matTexture;
	XMMATRIX matWorld;
	for (UINT j = 4030;j < 4030+num;j++) {
		if (m_objectCollections[i][j].status == 0)
		{
			m_objectCollections[i][j].status = 1;
			m_objectCollections[i][j].type = 51;//��ը
			m_objectCollections[i][j].fxvel = fspeed*cosf(fdir);
			m_objectCollections[i][j].fyvel = fspeed*sinf(fdir);
			//��ըʹ���ӵ�����ע��:��������������λ�ã�Y��ƽ��λ���ø�ֵ��,�ӵ�ȡ���1����+16.0f-4.0f
			ftx = (16.0f * 19 + 0 * 16.0f + 12.0f) / DONUTS_BITMAP_WIDTH;
			fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f + 4 * 32.0f + 4 * 32.0f + 0.0f;
			fty = -fsy / DONUTS_BITMAP_HEIGHT;
			matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
			XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, matTexture);
			//ģ����ʾλ��
			matWorld = XMMatrixScaling(32.0f, 32.0f, 0.0f)*XMMatrixTranslation(fx, fy, StageZ );
			XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, matWorld);
			fdir += fa;
			if (j >= 4050) break;
		}
	}
}



void CGames::OnKeyDown(WPARAM key) {
	keyboard[key] = 1;
}
void CGames::OnKeyUp(WPARAM key) {
	keyboard[key] = 0;

}

void CGames::GetInput(UINT i){
	float ftx, fty, fsy;
	float fx, fy;
	XMMATRIX matTexture;
	XMMATRIX matWorld;
	int index = 900;
	DWORD dwThisTick = GetTickCount();
	if (m_objectCollections[i][900].status == 0) index = 901;
	if (keyboard['A']) {
		if (dwThisTick - dwPresskeyDelay['A'] > 30)
		{
			dwPresskeyDelay['A'] = dwThisTick;
			if (m_objectCollections[i][index].status == 1)
			{
				m_objectCollections[i][index].animcurrent--;
				if (m_objectCollections[i][index].animcurrent < 0)
					m_objectCollections[i][index].animcurrent = m_objectCollections[i][index].animtotal - 1;
				//��������ע��:��������������λ�ã�Y��ƽ��λ���ø�ֵ��
				ftx = (float)(m_objectCollections[i][index].animcurrent % 10)*32.0f / DONUTS_BITMAP_WIDTH;
				if (index == 901) fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f + 4 * 32.0f;
				else   			  fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f;
				fty = ((int)m_objectCollections[i][index].animcurrent / 10) * 32.0f;
				fty = -(fsy + fty) / DONUTS_BITMAP_HEIGHT;
				matTexture = XMMatrixScaling(2.0f, 2.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
				XMStoreFloat4x4(&m_objectCollections[i][index].textureMatrices, matTexture);
			}
		}
	}
	if (keyboard['D']) {
		if (dwThisTick - dwPresskeyDelay['D'] > 30)
		{
			dwPresskeyDelay['D'] = dwThisTick;
			if (m_objectCollections[i][index].status == 1)
			{
				m_objectCollections[i][index].animcurrent++;
				if (m_objectCollections[i][index].animcurrent >= m_objectCollections[i][index].animtotal)
					m_objectCollections[i][index].animcurrent = 0;
				//��������ע��:��������������λ�ã�Y��ƽ��λ���ø�ֵ��
				ftx = (float)(m_objectCollections[i][index].animcurrent % 10)*32.0f / DONUTS_BITMAP_WIDTH;
				if (index == 901) fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f + 4 * 32.0f;
				else fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f;
				fty = ((int)m_objectCollections[i][index].animcurrent / 10) * 32.0f;
				fty = -(fsy + fty) / DONUTS_BITMAP_HEIGHT;
				matTexture = XMMatrixScaling(2.0f, 2.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
				XMStoreFloat4x4(&m_objectCollections[i][index].textureMatrices, matTexture);
			}
		}
	}
	//����
	if (keyboard[VK_SPACE]) {
		//ʹ��������ͬ����ʱ���
		if (dwThisTick - dwPresskeyDelay[VK_SPACE] > 30)
		{
			dwPresskeyDelay[VK_SPACE] = dwThisTick;
			//���ܲ�������
			if (m_objectCollections[i][900].status == 1) {
				//�����ӵ�30������
				for (UINT j = 4000;j < 4030;j++) {
					if (m_objectCollections[i][j].status == 0)
					{
						m_objectCollections[i][j].status = 1;
						m_objectCollections[i][j].animcurrent = 0;
						//��������ע��:��������������λ�ã�Y��ƽ��λ���ø�ֵ��
						//�ӵ�����ȡ�ײ�4X4ö���1ö����ftx ��+12.0f
						ftx = (16.0f * 19 + 0 * 16.0f + 12.0f) / DONUTS_BITMAP_WIDTH;
						fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f + 4 * 32.0f + 4 * 32.0f;
						fty = -fsy / DONUTS_BITMAP_HEIGHT;
						matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
						XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, matTexture);
						//��ȡ���λ��(�����ӵ�������ڿ�)
						fx = m_objectCollections[i][index].modelMatrices._41 + 32.0f / 2 - 1.0f;
						fy = m_objectCollections[i][index].modelMatrices._42 + 32.0f / 2 - 1.0f;
						fx += 16 * cosf(XM_2PI*m_objectCollections[i][index].animcurrent / 40 - XM_PI / 2);
						fy += 16 * sinf(XM_2PI*m_objectCollections[i][index].animcurrent / 40 - XM_PI / 2);
						//ģ����ʾλ��,�ӵ���С16.0f
						matWorld = XMMatrixScaling(16.0f, 16.0f, 0.0f)*XMMatrixTranslation(fx, fy, StageZ);
						XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, matWorld);
						// �ӵ�����
						m_objectCollections[i][j].fxvel = 5 * cosf(XM_2PI*m_objectCollections[i][index].animcurrent / 40 - XM_PI / 2);
						m_objectCollections[i][j].fyvel = 5 * sinf(XM_2PI*m_objectCollections[i][index].animcurrent / 40 - XM_PI / 2);
						PlayGameSound(GameSound::FIRE);
						break;
					}
				}
			}
			//���ӵ��������
			keyboard[VK_SPACE] = 0;
		}
	}
	//����
	if (keyboard['W']) {
		if (dwThisTick - dwPresskeyDelay['W'] > 30)
		{
			dwPresskeyDelay['W'] = dwThisTick;
			//����������״��
			if (m_objectCollections[i][900].status == 1)
			{
				//��������
				m_objectCollections[i][901].modelMatrices = m_objectCollections[i][900].modelMatrices;
				m_objectCollections[i][901].animcurrent = m_objectCollections[i][900].animcurrent;
				//��������ע��:��������������λ�ã�Y��ƽ��λ���ø�ֵ��
				ftx = (float)(m_objectCollections[i][901].animcurrent % 10)*32.0f / DONUTS_BITMAP_WIDTH;
				fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f + 4 * 32.0f;
				fty = (m_objectCollections[i][901].animcurrent / 10) * 32.0f;
				fty = -(fsy + fty) / DONUTS_BITMAP_HEIGHT;
				matTexture = XMMatrixScaling(2.0f, 2.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
				XMStoreFloat4x4(&m_objectCollections[i][901].textureMatrices, matTexture);
				//�л�״̬
				m_objectCollections[i][901].status = 1;
				m_objectCollections[i][900].status = 0;
				PlayGameSound(GameSound::SHIELD);
			}
		}
	}
	//�������
	if (keyboard['S']) {
		if (dwThisTick - dwPresskeyDelay['S'] > 30)
		{
			dwPresskeyDelay['S'] = dwThisTick;
			//�����ڿ���״̬
			if (m_objectCollections[i][901].status == 1) {
				//��������
				m_objectCollections[i][900].modelMatrices = m_objectCollections[i][901].modelMatrices;
				m_objectCollections[i][900].animcurrent = m_objectCollections[i][901].animcurrent;
				//��������ע��:��������������λ�ã�Y��ƽ��λ���ø�ֵ��
				ftx = (float)(m_objectCollections[i][900].animcurrent % 10)*32.0f / DONUTS_BITMAP_WIDTH;
				fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f;
				fty = ((int)m_objectCollections[i][900].animcurrent / 10) * 32.0f;
				fty = -(fsy + fty) / DONUTS_BITMAP_HEIGHT;
				matTexture = XMMatrixScaling(2.0f, 2.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
				XMStoreFloat4x4(&m_objectCollections[i][900].textureMatrices, matTexture);
				//�л�״̬
				m_objectCollections[i][900].status = 1;
				m_objectCollections[i][901].status = 0;
				PlayGameSound(GameSound::SKID);
			}
		}
	}
	if (keyboard[VK_LEFT]) {
		if (dwThisTick - dwPresskeyDelay[VK_LEFT] > 30)
		{
			dwPresskeyDelay[VK_LEFT] = dwThisTick;
			if (m_objectCollections[i][index].status == 1)
			{
				m_objectCollections[i][index].modelMatrices._41 -= 5;
				if (m_objectCollections[i][index].modelMatrices._41 < 0)  m_objectCollections[i][index].modelMatrices._41 = 0;
				if (m_objectCollections[i][index].modelMatrices._41 > (800 - 32.0f))  m_objectCollections[i][index].modelMatrices._41 = (800 - 32.0f);
				if (m_objectCollections[i][index].modelMatrices._42 < 0)  m_objectCollections[i][index].modelMatrices._42 = 0;
				if (m_objectCollections[i][index].modelMatrices._42 > (600 - 32.0f))  m_objectCollections[i][index].modelMatrices._42 = (600 - 32.0f);
				//PlayGameSound(GameSound::REV);
			}
		}
	}
	if (keyboard[VK_RIGHT]) {
		if (dwThisTick - dwPresskeyDelay[VK_RIGHT] > 30)
		{
			dwPresskeyDelay[VK_RIGHT] = dwThisTick;
			if (m_objectCollections[i][index].status == 1)
			{
				m_objectCollections[i][index].modelMatrices._41 += 5;
				if (m_objectCollections[i][index].modelMatrices._41 < 0)  m_objectCollections[i][index].modelMatrices._41 = 0;
				if (m_objectCollections[i][index].modelMatrices._41 > (800 - 32.0f))  m_objectCollections[i][index].modelMatrices._41 = (800 - 32.0f);
				if (m_objectCollections[i][index].modelMatrices._42 < 0)  m_objectCollections[i][index].modelMatrices._42 = 0;
				if (m_objectCollections[i][index].modelMatrices._42 > (600 - 32.0f))  m_objectCollections[i][index].modelMatrices._42 = (600 - 32.0f);
				//PlayGameSound(GameSound::REV);
			}
		}
	}
	if (keyboard[VK_UP]) {
		if (dwThisTick - dwPresskeyDelay[VK_UP] > 30)
		{
			dwPresskeyDelay[VK_UP] = dwThisTick;
			if (m_objectCollections[i][index].status == 1)
			{
				m_objectCollections[i][index].modelMatrices._42 -= 5;
				if (m_objectCollections[i][index].modelMatrices._41 < 0)  m_objectCollections[i][index].modelMatrices._41 = 0;
				if (m_objectCollections[i][index].modelMatrices._41 > (800 - 32.0f))  m_objectCollections[i][index].modelMatrices._41 = (800 - 32.0f);
				if (m_objectCollections[i][index].modelMatrices._42 < 0)  m_objectCollections[i][index].modelMatrices._42 = 0;
				if (m_objectCollections[i][index].modelMatrices._42 > (600 - 32.0f))  m_objectCollections[i][index].modelMatrices._42 = (600 - 32.0f);
			}
		}
	}
	if (keyboard[VK_DOWN]) {
		if (dwThisTick - dwPresskeyDelay[VK_DOWN] > 30)
		{
			dwPresskeyDelay[VK_DOWN] = dwThisTick;
			if (m_objectCollections[i][index].status == 1)
			{
				m_objectCollections[i][index].modelMatrices._42 += 5;
				if (m_objectCollections[i][index].modelMatrices._41 < 0)  m_objectCollections[i][index].modelMatrices._41 = 0;
				if (m_objectCollections[i][index].modelMatrices._41 > (800 - 32.0f))  m_objectCollections[i][index].modelMatrices._41 = (800 - 32.0f);
				if (m_objectCollections[i][index].modelMatrices._42 < 0)  m_objectCollections[i][index].modelMatrices._42 = 0;
				if (m_objectCollections[i][index].modelMatrices._42 > (600 - 32.0f))  m_objectCollections[i][index].modelMatrices._42 = (600 - 32.0f);
				//PlayGameSound(GameSound::REV);
			}
		}
	}
	//
	if (keyboard['P']) {
		if (dwThisTick - dwPresskeyDelay['P'] > 30)
		{
			m_bWantSound = !m_bWantSound;
			keyboard['P'] = 0;
		}
	}
}
VOID CGames::DisplaySplash(UINT i, float ElapsedTicks) {

	//��ʾ�˵�
	for (UINT j = 4105;j < 4106;j++) {
		m_objectCollections[i][j].status = 1;
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, XMMatrixScaling(800.0f, 600.0f, 0.0f)* XMMatrixTranslation(0.0f, 0.0f, SubtitleZ));
		//����40x16=640��30x16=480
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, XMMatrixScaling(40.0f, 30.0f, 0.0f)*XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		m_objectCollections[i][j].animcurrent = 0;
		m_objectCollections[i][j].animtotal = 1;
		m_objectCollections[i][j].type = 46;
		m_objectCollections[i][j].fxdst = 0.0f;
		m_objectCollections[i][j].fydst = 0.0f;
		m_objectCollections[i][j].texid = 1;
	}
	//��ʼ��Ϸ
	if (keyboard[VK_RETURN]) {
			//��ʼ�ؿ�
			m_uLevel = 1;
			AddObject(i, 0, 0, 0, 0, 0, 1 + rand() % 2);
			//��ʼ���-����
			float fx = 400 - 16, fy = 600 - 32;
			float ftx, fty, fsy;
			XMMATRIX matTexture;
			XMMATRIX matWorld;
			for (UINT j = 900;j < 901;j++) {
				m_objectCollections[i][j].status = 1;
				m_objectCollections[i][j].animcurrent = 0;
				//��������λ�ã�ע��:��������������λ�ã�Y��ƽ��λ���ø�ֵ��
				ftx = (float)(m_objectCollections[i][j].animcurrent % 10)*32.0f / DONUTS_BITMAP_WIDTH;
				fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f;
				fty = ((int)m_objectCollections[i][j].animcurrent / 10) * 32.0f;
				fty = -(fsy + fty) / DONUTS_BITMAP_HEIGHT;
				matTexture = XMMatrixScaling(2.0f, 2.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
				XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, matTexture);
				//ģ����ʾλ��
				matWorld = XMMatrixScaling(32.0f, 32.0f, 0.0f)*XMMatrixTranslation(fx, fy, StageZ );
				XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, matWorld);
			}
			m_nProgstatus = PStatus::PLAYGAME;
			//�ز˵�
			for (UINT j = 4105;j < 4106;j++) {
				m_objectCollections[i][j].status = 0;
			}
			PlayGameSound(GameSound::LEVEL);
	}

}



VOID CGames::UpdateGames(UINT i, float ElapsedTicks) {
	//֡����
	m_fAnimFrameCount += ( ElapsedTicks / 300000);
	XMMATRIX matTexture;
	float ftx, fty,fsy;
	if (m_fAnimFrameCount > m_fAnimFrame) {
		m_fAnimFrameCount = 0;
		for (UINT j = 0;j < m_nMaxObjectsCount;j++) {
			if (m_objectCollections[i][j].status == 1)
			{
				//����֡������֡
				switch (m_objectCollections[i][j].type)
				{
				case 0: //��̥
				{
					//����֡������ʱ
					m_objectCollections[i][j].animcurrent++;
					if (m_objectCollections[i][j].animcurrent >= m_objectCollections[i][j].animtotal)
						m_objectCollections[i][j].animcurrent = 0;
					ftx = (float)(m_objectCollections[i][j].animcurrent % 5)*(64.0f / DONUTS_BITMAP_WIDTH);
					fty = -(float)(m_objectCollections[i][j].animcurrent / 5)*(64.0f / DONUTS_BITMAP_HEIGHT);
					matTexture = XMMatrixScaling(4.0f, 4.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, matTexture);
				}
				break;
				case 1://���1
				{
					//����֡������ʱ
					m_objectCollections[i][j].animcurrent++;
					if (m_objectCollections[i][j].animcurrent >= m_objectCollections[i][j].animtotal)
						m_objectCollections[i][j].animcurrent = 0;
					ftx = (float)(m_objectCollections[i][j].animcurrent*32.0f / DONUTS_BITMAP_WIDTH);
					fty = -(float)(6 * 64.0f + 32.0f * 0) / DONUTS_BITMAP_HEIGHT;
					matTexture = XMMatrixScaling(2.0f, 2.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, matTexture);
				}
				break;
				case 2://���2
				{
					//����֡������ʱ
					m_objectCollections[i][j].animcurrent++;
					if (m_objectCollections[i][j].animcurrent >= m_objectCollections[i][j].animtotal)
						m_objectCollections[i][j].animcurrent = 0;
					ftx = (float)(m_objectCollections[i][j].animcurrent*32.0f / DONUTS_BITMAP_WIDTH);
					fty = -(float)(6 * 64.0f + 32.0f * 1) / DONUTS_BITMAP_HEIGHT;
					matTexture = XMMatrixScaling(2.0f, 2.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, matTexture);
				}
				break;
				case 3://���3
				{
					//����֡������ʱ
					m_objectCollections[i][j].animcurrent++;
					if (m_objectCollections[i][j].animcurrent >= m_objectCollections[i][j].animtotal)
						m_objectCollections[i][j].animcurrent = 0;
					ftx = (float)(m_objectCollections[i][j].animcurrent*32.0f / DONUTS_BITMAP_WIDTH);
					fty = -(float)(6 * 64.0f + 32.0f * 2) / DONUTS_BITMAP_HEIGHT;
					matTexture = XMMatrixScaling(2.0f, 2.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, matTexture);
				}
				break;
				case 4://���4
				{
					//����֡������ʱ
					m_objectCollections[i][j].animcurrent++;
					if (m_objectCollections[i][j].animcurrent >= m_objectCollections[i][j].animtotal)
						m_objectCollections[i][j].animcurrent = 0;
					ftx = (float)(m_objectCollections[i][j].animcurrent*32.0f / DONUTS_BITMAP_WIDTH);
					fty = -(float)(6 * 64.0f + 32.0f * 3) / DONUTS_BITMAP_HEIGHT;
					matTexture = XMMatrixScaling(2.0f, 2.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, matTexture);
				}
				break;
				case 5://�ɿ���1
				{
					//����֡������ʱ
					m_objectCollections[i][j].animcurrent++;
					if (m_objectCollections[i][j].animcurrent >= m_objectCollections[i][j].animtotal)
						m_objectCollections[i][j].animcurrent = 0;
					ftx = (float)(m_objectCollections[i][j].animcurrent*16.0f / DONUTS_BITMAP_WIDTH);
					fsy = 6 * 64.0f + 4 * 32.0f + 16.0f * 0;
					fty = -fsy / DONUTS_BITMAP_HEIGHT;
					matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, matTexture);
				}
				break;
				case 6://�ɿ���2
				{
					//����֡������ʱ
					m_objectCollections[i][j].animcurrent++;
					if (m_objectCollections[i][j].animcurrent >= m_objectCollections[i][j].animtotal)
						m_objectCollections[i][j].animcurrent = 0;
					ftx = (float)(m_objectCollections[i][j].animcurrent*16.0f / DONUTS_BITMAP_WIDTH);
					fsy = 6 * 64.0f + 4 * 32.0f + 16.0f * 1;
					fty = -fsy / DONUTS_BITMAP_HEIGHT;
					matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, matTexture);
				}
				break;
				case 7://�ɿ���3
				{
					//����֡������ʱ
					m_objectCollections[i][j].animcurrent++;
					if (m_objectCollections[i][j].animcurrent >= m_objectCollections[i][j].animtotal)
						m_objectCollections[i][j].animcurrent = 0;
					ftx = (float)(m_objectCollections[i][j].animcurrent*16.0f / DONUTS_BITMAP_WIDTH);
					fsy = 6 * 64.0f + 4 * 32.0f + 16.0f * 2;
					fty = -fsy / DONUTS_BITMAP_HEIGHT;
					matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, matTexture);
				}
				break;
				case 8://�ɿ���4
				{
					//����֡������ʱ
					m_objectCollections[i][j].animcurrent++;
					if (m_objectCollections[i][j].animcurrent >= m_objectCollections[i][j].animtotal)
						m_objectCollections[i][j].animcurrent = 0;
					ftx = (float)(m_objectCollections[i][j].animcurrent*16.0f / DONUTS_BITMAP_WIDTH);
					fsy = 6 * 64.0f + 4 * 32.0f + 16.0f * 3;
					fty = -fsy / DONUTS_BITMAP_HEIGHT;
					matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, matTexture);
				}
				break;
				case 45://������
				{
					//����֡������ʱ
					m_objectCollections[i][j].animcurrent = 0;
					ftx = 24.0f/ DONUTS_BITMAP_WIDTH;
					fty = -24.0f/ DONUTS_BITMAP_HEIGHT;
					matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
					XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, matTexture);
				}
				break;

				}

			}
		}
	}


	//���߿���
	float fx, fy, fdx, fdy, fdistance;
	m_fMoveCount += (ElapsedTicks / 10000);
	if (m_fMoveCount > m_fMove) {
		m_fMoveCount = 0;
		for (UINT j = 0;j < m_nMaxObjectsCount;j++) {
			if (m_objectCollections[i][j].status == 1)
			{
				//����
				if (m_objectCollections[i][j].type < 9) {
					fx = m_objectCollections[i][j].modelMatrices._41;
					fy = m_objectCollections[i][j].modelMatrices._42;
					fdx = m_objectCollections[i][j].fxdst - fx;
					fdy = m_objectCollections[i][j].fydst - fy;
					fdistance = sqrtf(fdx*fdx + fdy*fdy);
					if ((fdistance >= 0.0f) && (fdistance < 1.0f))
					{
						m_objectCollections[i][j].fxdst = (float)(rand() % (800-64));
						m_objectCollections[i][j].fydst = (float)(rand() % (600-64));
					}
					else
					{
						m_objectCollections[i][j].modelMatrices._41 += fdx / fdistance;
						m_objectCollections[i][j].modelMatrices._42 += fdy / fdistance;
					}
				}
				//�ӵ�
				else 	if (m_objectCollections[i][j].type==50)
				{
						fx = m_objectCollections[i][j].modelMatrices._41;
						fy = m_objectCollections[i][j].modelMatrices._42;
						m_objectCollections[i][j].modelMatrices._41 += m_objectCollections[i][j].fxvel;
						m_objectCollections[i][j].modelMatrices._42 += m_objectCollections[i][j].fyvel;

						if(m_objectCollections[i][j].modelMatrices._41 < 16)	 m_objectCollections[i][j].status = 0;
						if(m_objectCollections[i][j].modelMatrices._42 < 16)	 m_objectCollections[i][j].status = 0;
						if(m_objectCollections[i][j].modelMatrices._41>(800-16)) m_objectCollections[i][j].status = 0;
						if(m_objectCollections[i][j].modelMatrices._42>(600-16)) m_objectCollections[i][j].status = 0;
				}
				//��ը
				else 	if (m_objectCollections[i][j].type == 51)
				{
					m_objectCollections[i][j].modelMatrices._41 += m_objectCollections[i][j].fxvel;
					m_objectCollections[i][j].modelMatrices._42 += m_objectCollections[i][j].fyvel;;
					//��ԭ�ӵ�����
					if (m_objectCollections[i][j].modelMatrices._41 < 16) {
						m_objectCollections[i][j].status = 0;
						m_objectCollections[i][j].type=50;
					}
					if (m_objectCollections[i][j].modelMatrices._42 < 16) {
						m_objectCollections[i][j].status = 0;
						m_objectCollections[i][j].type = 50;
					}
					if (m_objectCollections[i][j].modelMatrices._41 > (800 - 16)) {
						m_objectCollections[i][j].status = 0;
						m_objectCollections[i][j].type = 50;

					}
					if (m_objectCollections[i][j].modelMatrices._42 > (600 - 16)) {
						m_objectCollections[i][j].status = 0;
						m_objectCollections[i][j].type = 50;
					}
				}
				//������
				else 	if (m_objectCollections[i][j].type == 45)
				{
					fx = m_objectCollections[i][j].modelMatrices._41;
					fy = m_objectCollections[i][j].modelMatrices._42;
					fdx = m_objectCollections[i][j].fxdst - fx;
					fdy = m_objectCollections[i][j].fydst - fy;
					fdistance = sqrtf(fdx*fdx + fdy*fdy);
					if ((fdistance >= 0.0f) && (fdistance < 1.0f))
						m_objectCollections[i][j].status = 0;
					m_objectCollections[i][j].modelMatrices._41 += fdx / fdistance;
					m_objectCollections[i][j].modelMatrices._42 += fdy / fdistance;
				}

			}
		}
	}

	//����������
	m_fBonusCount += (ElapsedTicks / 300000);
	if (m_fBonusCount > m_fBonus) {
		m_fBonusCount = 0;
		for (UINT j = 0;j < 100;j++) {
			if (m_objectCollections[i][j].status == 1)
			{
				//���λ��
				fdx = (float)(rand() % (800 - 32));
				fdy = (float)(rand() % (600 - 32));
				//���ӽ����� 
				fx = m_objectCollections[i][j].modelMatrices._41;
				fy = m_objectCollections[i][j].modelMatrices._42;
				for (UINT k = 4050;k < 4099;k++) {
					if (m_objectCollections[i][k].status == 0) {
						m_objectCollections[i][k].status = 1;
						XMStoreFloat4x4(&m_objectCollections[i][k].modelMatrices,XMMatrixScaling(8.0f,8.0f,0.0f)* XMMatrixTranslation(fx, fy, StageZ ));
						XMStoreFloat4x4(&m_objectCollections[i][k].textureMatrices, XMMatrixScaling(4.0f, 4.0f, 0.0f)* XMMatrixTranslation(0,0, 0));
						m_objectCollections[i][k].fxdst = fdx;
						m_objectCollections[i][k].fydst = fdy;
						m_objectCollections[i][k].type = 45;
						break;
					}
				}
			}
			break;
		}
	}

}
VOID CGames::DisplayGames(UINT i, float ElapsedTicks) {
	//�̶���ʾ����
	BltSpeaker(i,800-16.0f,0.0f);
	//��������
	BltDonuts(i,0.0f,0.0f);
    //�����֣���ĸ��
	for (UINT j = 1000;j < 3000;j++) m_objectCollections[i][j].status = 0;
	//
	//�ؿ�
	BltLevel(i, m_uLevel,250.0f,0.0f,16.0f);
	//�÷�
	BltScore(i, m_uScore,600.0f,0.0f);
	//������
	BltStars(i, 400.0f, 0.0f, m_uStars);
}

VOID CGames::PlayGames(UINT i, float ElapsedTicks) {

	UpdateGames(i, ElapsedTicks);
	DisplayGames(i, ElapsedTicks);
    CheckBulletHit(i);
	CheckHitDonuts(i);
	if (CheckWin(i))
	{
		PlayGameSound(GameSound::SCORE);
		m_nProgstatus = PStatus::PASS;
	}
    //��Ϸʧ��
	if (m_nLife < 1) {
		m_nProgstatus = PStatus::GAMEOVER;
		PlayGameSound(GameSound::BOUNCE);
		return;
	}
}
VOID CGames::GameOver(UINT i, float ElapsedTicks) {

	m_fDelayCount += ElapsedTicks / 300000;
	if (m_fDelayCount > 30)
	{
		m_fDelayCount = 0;
		m_nDelayTimes--;
		if (m_nDelayTimes < 0) {
			m_uLevel = 1;
			m_uScore = 0;
			m_nLife = 5;
			m_nProgstatus = PStatus::GAMEMENU;
			m_fDelayCount = 0;
			m_uStars = 0;
			for (UINT j = 0;j < m_nMaxObjectsCount;j++) {
				if (j == 900 || j == 901) continue;
				m_objectCollections[i][j].status = 0;
			}
			m_objectCollections[i][4150].status = 0;
			m_nDelayTimes = 10;
			return;
		}
	}
	//�˳�
	if (keyboard[VK_ESCAPE])
	{
		QuitGame();
		return;
	}
	//��̨�ֳ�
	DisplayGames(i, ElapsedTicks);
	//��ʾʧ����ʾ
	m_objectCollections[i][4150].status = 1;
	XMStoreFloat4x4(&m_objectCollections[i][4150].modelMatrices, XMMatrixScaling(640.0f, 208.0f, 0.0f)*XMMatrixTranslation(100.0f, 100.0f, SubtitleZ));
    //����u=40*16=640,v=13*16=208
	float fy = 0;
	XMStoreFloat4x4(&m_objectCollections[i][4150].textureMatrices, XMMatrixScaling(40.0f, 13.0f, 0.0f)*XMMatrixTranslation(0.0f, fy, 0.0f));
	m_objectCollections[i][4150].texid = 2;
    //��ʾ��ʱ����
	BltDelayTimes(i, 600, 230, m_nDelayTimes);

}
//����
VOID CGames::Clearance(UINT i, float ElapsedTicks) {

	m_fDelayCount += ElapsedTicks / 300000;
	if (m_fDelayCount >30)
	{
		m_fDelayCount = 0;
		m_nDelayTimes--;
		if (m_nDelayTimes < 0) {
			m_uLevel += 1;
			for (UINT j = 0;j < m_nMaxObjectsCount;j++) {
				if (j == 900 || j == 901) continue;
				m_objectCollections[i][j].status = 0;
			}
			AddObject(i, 0, 0, 0, 0, 0, m_uLevel + rand() % 2);
			m_nProgstatus = PStatus::PLAYGAME;
			m_objectCollections[i][4150].status = 0;

			PlayGameSound(LEVEL);
			keyboard[VK_RETURN] = 0;
			//�ָ�����ʱֵ
			m_nDelayTimes = 10;
			return;
		}
	}
	//��̨�ֳ�
	DisplayGames(i, ElapsedTicks);

 	BltLevel(i, m_uLevel+1, 210.0f, 350.0f,64.0f);
	//��ʾͨ����ʾ
	m_objectCollections[i][4150].status = 1;
	XMStoreFloat4x4(&m_objectCollections[i][4150].modelMatrices, XMMatrixScaling(640.0f, 224.0f, 0.0f)*XMMatrixTranslation(100.0f,100.0f, SubtitleZ));
	//����u=40*16=640,v=7*16=112��ע��:��������������λ�ã�Y��ƽ��λ���ø�ֵ��
	float fy = -210.0f / DONUTS_BITMAP_HEIGHT;
	XMStoreFloat4x4(&m_objectCollections[i][4150].textureMatrices, XMMatrixScaling(40.0f,7.0f, 0.0f)*XMMatrixTranslation(0.0f, fy, 0.0f));
	m_objectCollections[i][4150].texid = 2;
	//��ʾ��ʱ����
	BltDelayTimes(i, 600, 250, m_nDelayTimes);
}
//��ʧ���� 
VOID CGames::LostLife(UINT i, float ElapsedTicks) {

	m_fDelayCount += ElapsedTicks / 300000;
	if (m_fDelayCount >30)
	{
		m_fDelayCount = 0;
		m_nDelayTimes--;
		if (m_nDelayTimes < 0) {
			m_nProgstatus = PStatus::PLAYGAME;
			m_objectCollections[i][4150].status = 0;
			//�ָ�����ʱֵ
			m_nDelayTimes = 10;
			return;
		}
	}
	//��̨�ֳ�
	DisplayGames(i, ElapsedTicks);
	//��ʾ��ʧ������ʾ
	m_objectCollections[i][4150].status = 1;
	XMStoreFloat4x4(&m_objectCollections[i][4150].modelMatrices, XMMatrixScaling(640.0f, 224.0f, 0.0f)*XMMatrixTranslation(100.0f, 100.0f, SubtitleZ));
	//����u=40*16=640,v=6*16=96��ע��:��������������λ�ã�Y��ƽ��λ���ø�ֵ��
	float fy = -384.0f / DONUTS_BITMAP_HEIGHT;
	XMStoreFloat4x4(&m_objectCollections[i][4150].textureMatrices, XMMatrixScaling(40.0f,6.0f, 0.0f)*XMMatrixTranslation(0.0f,fy, 0.0f));
	m_objectCollections[i][4150].texid = 2;
	//��ʾ��ʱ����
	BltDelayTimes(i, 600, 250, m_nDelayTimes);

}
VOID CGames::BonusAnim(UINT i, float ElapsedTicks) {
	m_fDelayCount += ElapsedTicks / 30000;
	if (m_fDelayCount >10)
	{
		m_fDelayCount = 0;
		int index = 900;
		if (m_objectCollections[i][900].status == 0) index = 901;
		m_objectCollections[i][index].animcurrent++;
		if (m_objectCollections[i][index].animcurrent >= m_objectCollections[i][index].animtotal) {
			m_objectCollections[i][index].animcurrent = 0;
			m_nProgstatus = PStatus::PLAYGAME;
		}
		//�����ת360
		float ftx, fsy, fty;
		XMMATRIX matTexture;
		//��������
		ftx = (float)(m_objectCollections[i][index].animcurrent % 10)*32.0f / DONUTS_BITMAP_WIDTH;
		if (index == 901) fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f + 4 * 32.0f;
		else   			  fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f;
		fty = ((int)m_objectCollections[i][index].animcurrent / 10) * 32.0f;
		fty = -(fsy + fty) / DONUTS_BITMAP_HEIGHT;
		matTexture = XMMatrixScaling(2.0f, 2.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
		XMStoreFloat4x4(&m_objectCollections[i][index].textureMatrices, matTexture);
		PlayGameSound(STAR);
	}
}
VOID CGames::Update(UINT i, float ElapsedTicks) {
	switch (m_nProgstatus) {
	case PStatus::GAMEMENU:DisplaySplash(i, ElapsedTicks);break;
	case PStatus::PLAYGAME:PlayGames(i, ElapsedTicks);break;
	case PStatus::GAMEOVER:GameOver(i, ElapsedTicks);break;
	case PStatus::PASS:Clearance(i, ElapsedTicks);break;
	case PStatus::BONUSANIM:BonusAnim(i, ElapsedTicks);break;
	case PStatus::LOSTLIFE:LostLife(i, ElapsedTicks);break;
	case PStatus::QUIT:break;
	}
	//
	//ͬ��֡
	UINT nTotalObjectsCount = m_nMaxObjectsCount;
	for (UINT k = 0;k < FrameCount;k++) {
		if (k != i) {
			for (UINT j = 0;j<nTotalObjectsCount;j++)
				m_objectCollections[k][j] = m_objectCollections[i][j];
		}
	}
}

VOID CGames::BltLevel(UINT i, int nLevel,float fx,float fy,float fsize)
{
	int base = 2000;
	float ftx, fsy, fty;
	XMMATRIX matTexture;
	XMMATRIX matWorld;
	float fpos = 0.0f;
	//��ĸLEVEL 
	//L
	for (int k = 0;k < 10;k++)
	{
		if (m_objectCollections[i][base+k].status == 0) {
			m_objectCollections[i][base+k].status = 1;//
			//��������ƫ��λ��
			ftx = (16.0f*10 +0*16.0f) / DONUTS_BITMAP_WIDTH;
			fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f + 4 * 32.0f + 4 * 32.0f;
			fty = -fsy / DONUTS_BITMAP_HEIGHT;
			matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
			XMStoreFloat4x4(&m_objectCollections[i][base + k].textureMatrices, matTexture);
			//ģ����ʾλ��(���룩
			matWorld = XMMatrixScaling(fsize, fsize, 0.0f)*XMMatrixTranslation(fx+fpos, fy, SubtitleZ);
			XMStoreFloat4x4(&m_objectCollections[i][base+k].modelMatrices, matWorld);
			break;
		}
	}
	fpos += fsize;
	//E
	base = 2010;
	for (int k = 0;k < 10;k++)
	{
		if (m_objectCollections[i][base + k].status == 0) {
			m_objectCollections[i][base + k].status = 1;//
			//��������ƫ��λ��
			ftx = (16.0f * 10 + 1 * 16.0f) / DONUTS_BITMAP_WIDTH;
			fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f + 4 * 32.0f + 4 * 32.0f;
			fty = -fsy / DONUTS_BITMAP_HEIGHT;
			matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
			XMStoreFloat4x4(&m_objectCollections[i][base + k].textureMatrices, matTexture);
			//ģ����ʾλ��(���룩
			matWorld = XMMatrixScaling(fsize, fsize, 0.0f)*XMMatrixTranslation(fx+fpos, fy, SubtitleZ);
			XMStoreFloat4x4(&m_objectCollections[i][base + k].modelMatrices, matWorld);
			break;
		}
	}
	fpos += fsize;
	//V
	base = 2020;
	for (int k = 0;k < 10;k++)
	{
		if (m_objectCollections[i][base + k].status == 0) {
			m_objectCollections[i][base + k].status = 1;//
			//��������ƫ��λ��
			ftx = (16.0f * 10 + 2 * 16.0f) / DONUTS_BITMAP_WIDTH;
			fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f + 4 * 32.0f + 4 * 32.0f;
			fty = -fsy / DONUTS_BITMAP_HEIGHT;
			matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
			XMStoreFloat4x4(&m_objectCollections[i][base + k].textureMatrices, matTexture);
			//ģ����ʾλ��
			matWorld = XMMatrixScaling(fsize, fsize, 0.0f)*XMMatrixTranslation(fx+fpos, fy, SubtitleZ);
			XMStoreFloat4x4(&m_objectCollections[i][base + k].modelMatrices, matWorld);
			break;
		}
	}
	fpos += fsize;
	//E
	base = 2010;
	for (int k = 0;k < 10;k++)
	{
		if (m_objectCollections[i][base + k].status == 0) {
			m_objectCollections[i][base + k].status = 1;//
			//��������ƫ��λ��
			ftx = (16.0f * 10 + 1 * 16.0f) / DONUTS_BITMAP_WIDTH;
			fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f + 4 * 32.0f + 4 * 32.0f;
			fty = -fsy / DONUTS_BITMAP_HEIGHT;
			matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
			XMStoreFloat4x4(&m_objectCollections[i][base + k].textureMatrices, matTexture);
			//ģ����ʾλ��
			matWorld = XMMatrixScaling(fsize, fsize, 0.0f)*XMMatrixTranslation(fx+fpos, fy, SubtitleZ);
			XMStoreFloat4x4(&m_objectCollections[i][base + k].modelMatrices, matWorld);
			break;
		}
	}
	fpos += fsize;
	//L
	base = 2000;
	for (int k = 0;k < 10;k++)
	{
		if (m_objectCollections[i][base + k].status == 0) {
			m_objectCollections[i][base+k].status = 1;//
			//��������ƫ��λ��
			ftx = (16.0f * 10 + 0 * 16.0f) / DONUTS_BITMAP_WIDTH;
			fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f + 4 * 32.0f + 4 * 32.0f;
			fty = -fsy / DONUTS_BITMAP_HEIGHT;
			matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
			XMStoreFloat4x4(&m_objectCollections[i][base+k].textureMatrices, matTexture);
			//ģ����ʾλ��(���룩
			matWorld = XMMatrixScaling(fsize, fsize, 0.0f)*XMMatrixTranslation(fx+ fpos, fy, SubtitleZ);
			XMStoreFloat4x4(&m_objectCollections[i][base+k].modelMatrices, matWorld);
			break;
		}
	}
	fpos += 2*fsize;
	//��������
	TCHAR szLevel[MAX_PATH];
	StringCbPrintf(szLevel, MAX_PATH,L"%d", nLevel);
	int j = 0;
	base = 1000;
	int n = 0;
	while (szLevel[j] != L'\0') {
		n = szLevel[j] - L'0';
   		for (int k = base + n*100;k <base + (n+1)*100;k++)
		{
			if (m_objectCollections[i][k].status == 0) {
				m_objectCollections[i][k].status = 1;//
				//��������ƫ��λ��
				ftx = (n * 16.0f) / DONUTS_BITMAP_WIDTH;
				fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f + 4 * 32.0f + 4 * 32.0f;
				fty = -fsy / DONUTS_BITMAP_HEIGHT;
				matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
				XMStoreFloat4x4(&m_objectCollections[i][k].textureMatrices, matTexture);
				//ģ����ʾλ��(���룩
				matWorld = XMMatrixScaling(fsize, fsize, 0.0f)*XMMatrixTranslation(fx+fpos, fy, SubtitleZ);
				XMStoreFloat4x4(&m_objectCollections[i][k].modelMatrices, matWorld);
				break;
			}
		}
		fpos += fsize;
		j++;
	}
}
VOID CGames::BltScore(UINT i, int nScore, float fx, float fy) {

	//��������
	TCHAR szScore[MAX_PATH];
	StringCbPrintf(szScore, MAX_PATH, L"%.8d", nScore);
	int j = 0;
	int base = 1000;
	int n = 0;
	float ftx, fsy, fty;
	XMMATRIX matTexture;
	XMMATRIX matWorld;
	float fpos = 0.0f;
	//��ʾ�÷ֱ�ʶ
	for (UINT j = 999;j < 1000;j++){
		m_objectCollections[i][j].status = 1;
		m_objectCollections[i][j].animcurrent = 0;
		ftx = 0.0f / DONUTS_BITMAP_WIDTH;
		fty = -0.0f / DONUTS_BITMAP_HEIGHT;
		matTexture = XMMatrixScaling(4.0f, 4.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, matTexture);
		matWorld = XMMatrixScaling(16.0f, 16.0f, 0.0f)*XMMatrixTranslation(fx + fpos, fy, SubtitleZ);
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, matWorld);
	}
	fpos += 32.0f;
	//��ʾ�÷�
	while (szScore[j] != L'\0') {
		n = szScore[j] - L'0';
		for (int k = base + n * 100;k <base + (n + 1) * 100;k++)
		{
			if (m_objectCollections[i][k].status == 0) {
				m_objectCollections[i][k].status = 1;//
				//��������ƫ��λ�ã�ע��:��������������λ�ã�Y��ƽ��λ���ø�ֵ��
				ftx =  (n * 16.0f) / DONUTS_BITMAP_WIDTH;
				fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f + 4 * 32.0f + 4 * 32.0f;
				fty = -fsy / DONUTS_BITMAP_HEIGHT;
				matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
				XMStoreFloat4x4(&m_objectCollections[i][k].textureMatrices, matTexture);
				//ģ����ʾλ��(���룩
				matWorld = XMMatrixScaling(16.0f, 16.0f, 0.0f)*XMMatrixTranslation(fx+fpos, fy, SubtitleZ);
				XMStoreFloat4x4(&m_objectCollections[i][k].modelMatrices, matWorld);
				break;
			}
		}
		fpos += 16.0f;
		j++;
	}
}
VOID CGames::BltSpeaker(UINT i, float fx, float fy) {
	if (m_bWantSound) {
		float ftx, fty, fsy;
		XMMATRIX matTexture;
		XMMATRIX matWorld;
		for (UINT j = 3000;j < 3001;j++) {
			m_objectCollections[i][j].status = 1;
			//��������
			ftx = (16.0f * 13 + 0 * 16.0f) / DONUTS_BITMAP_WIDTH;
			fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f + 4 * 32.0f + 4 * 32.0f;
			fty = -fsy / DONUTS_BITMAP_HEIGHT;
			matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
			XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, matTexture);
			//ģ����ʾλ��
			matWorld = XMMatrixScaling(16.0f, 16.0f, 0.0f)*XMMatrixTranslation(fx, fy, SubtitleZ);
			XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, matWorld);
		}
		return;
	}
	for (UINT j = 3000;j < 3001;j++) {
		m_objectCollections[i][j].status = 0;
	}
}
VOID CGames::BltDonuts(UINT i, float fx, float fy) {
	float ftx, fty, fsy;
	XMMATRIX matTexture;
	XMMATRIX matWorld;
	//��
	for (UINT j = 4100;j < 4105;j++) m_objectCollections[i][j].status = 0;
	//��1�����ڹ������ٻ�1��
	for (UINT j = 4100;j < (UINT)(4100+m_nLife-1);j++)
	{
			m_objectCollections[i][j].status = 1;
			//��������
			ftx = 0.0f;
			fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f;
			fty = 0.0f;
			fty = -(fsy + fty) / DONUTS_BITMAP_HEIGHT;
			matTexture = XMMatrixScaling(2.0f, 2.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
			XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, matTexture);
			//ģ����ʾλ��
			matWorld = XMMatrixScaling(16.0f, 16.0f, 0.0f)*XMMatrixTranslation(fx, fy, SubtitleZ);
			XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, matWorld);
			fx += 16.0f;
	}
}
VOID CGames::BltDelayTimes(UINT i, float fx, float fy, int num)
{
	float ftx, fty, fsy;
	XMMATRIX matTexture;
	XMMATRIX matWorld;
	//��������
	TCHAR szDelayTimes[MAX_PATH];
	StringCbPrintf(szDelayTimes, MAX_PATH, L"%d", num);
	int j = 0;
	int base = 1000;
	int n = 0;
	float fpos = 0.0f;
	while (szDelayTimes[j] != L'\0') {
		n = szDelayTimes[j] - L'0';
		for (int k = base + n * 100;k <base + (n + 1) * 100;k++)
		{
			if (m_objectCollections[i][k].status == 0) {
				m_objectCollections[i][k].status = 1;//
				//��������ƫ��λ��
				ftx = (n * 16.0f) / DONUTS_BITMAP_WIDTH;
				fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f + 4 * 32.0f + 4 * 32.0f;
				fty = -fsy / DONUTS_BITMAP_HEIGHT;
				matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
				XMStoreFloat4x4(&m_objectCollections[i][k].textureMatrices, matTexture);
				//ģ����ʾλ��(���룩
				matWorld = XMMatrixScaling(72.0f, 72.0f, 0.0f)*XMMatrixTranslation(fx + fpos,fy, SubtitleZ);
				XMStoreFloat4x4(&m_objectCollections[i][k].modelMatrices, matWorld);
				break;
			}
		}
		fpos += 72.0f;
		j++;
	}
}

VOID CGames::BltStars(UINT i,float fx,float fy,int nStars) {
	float ftx, fty, fsy;
	XMMATRIX matTexture;
	XMMATRIX matWorld;
	float fpos = 0.0f;
	//��ʾ������ʶ
	for (UINT j = 4099;j < 4100;j++) {
		m_objectCollections[i][j].status = 1;
		m_objectCollections[i][j].animcurrent = 0;
		ftx = 24.0f / DONUTS_BITMAP_WIDTH;
		fty = -24.0f / DONUTS_BITMAP_HEIGHT;
		matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
		XMStoreFloat4x4(&m_objectCollections[i][j].textureMatrices, matTexture);
		matWorld = XMMatrixScaling(16.0f, 16.0f, 0.0f)*XMMatrixTranslation(fx+fpos, fy, SubtitleZ);
		XMStoreFloat4x4(&m_objectCollections[i][j].modelMatrices, matWorld);
		//��ʾ�������
	}
	fpos += 32.0f;
	//��������
	TCHAR szStars[MAX_PATH];
	StringCbPrintf(szStars, MAX_PATH, L"%.6d", nStars);
	int j = 0;
	int base = 1000;
	int n = 0;
	while (szStars[j] != L'\0') {
		n = szStars[j] - L'0';
		for (int k = base + n * 100;k <base + (n + 1) * 100;k++)
		{
			if (m_objectCollections[i][k].status == 0) {
				m_objectCollections[i][k].status = 1;//
													 //��������ƫ��λ��
				ftx = (n * 16.0f) / DONUTS_BITMAP_WIDTH;
				fsy = 6 * 64.0f + 4 * 32.0f + 4 * 16.0f + 4 * 32.0f + 4 * 32.0f;
				fty = -fsy / DONUTS_BITMAP_HEIGHT;
				matTexture = XMMatrixScaling(1.0f, 1.0f, 0.0f)*XMMatrixTranslation(ftx, fty, 0.0f);
				XMStoreFloat4x4(&m_objectCollections[i][k].textureMatrices, matTexture);
				//ģ����ʾλ��(���룩
				matWorld = XMMatrixScaling(16.0f, 16.0f, 0.0f)*XMMatrixTranslation(fx+fpos, 0.0f, SubtitleZ);
				XMStoreFloat4x4(&m_objectCollections[i][k].modelMatrices, matWorld);
				break;
			}
		}
		fpos += 16.0f;
		j++;
	}

}

VOID CGames::CheckBulletHit(UINT i) {
	for (UINT j = 4000;j < 4050;j++) {
		if (m_objectCollections[i][j].type != 50) continue;
		if (m_objectCollections[i][j].status == 1)
		{
			//������̥��
			if (HitOnCircle(i, m_objectCollections[i][j]))
			{
				//�����ӵ�
				m_objectCollections[i][j].status = 0;
			}
			//���������
			else if (HitOnToy(i, m_objectCollections[i][j]))
			{
				//�����ӵ�
				m_objectCollections[i][j].status = 0;
			}
			//�����ɿ�����
			else if (HitOnChocolate(i, m_objectCollections[i][j]))
			{
				//�����ӵ�
				m_objectCollections[i][j].status = 0;
			}
		}
	}
}
BOOL CGames::CheckWin(UINT i) {
	for (UINT j = 0;j < 900;j++) {
		if (m_objectCollections[i][j].status == 1) {
			return FALSE;
		}
	}
	return TRUE;
}

//�����ײ�ڼ����
VOID CGames::CheckHitDonuts(UINT i) {
	if (m_objectCollections[i][900].status == 0) return;
	float fdx = 0.0f;
	float fdy = 0.0f;
	float fr1, fr2, fx1, fx2, fy1, fy2,fr;
	for (int j = 0;j < 900;j++) {
		if (m_objectCollections[i][j].status == 1) {
			//����뾶
			fr2 = sqrtf(16 * 16 + 16 * 16) / 2;
			//����
			fx2 = m_objectCollections[i][900].modelMatrices._41 + 32.0f / 2;
			fy2 = m_objectCollections[i][900].modelMatrices._42 + 32.0f / 2;
			switch (m_objectCollections[i][j].type)
			{
			case 0:	fr1 = sqrtf(32 * 32 + 32 * 32) / 2;
					fx1 = m_objectCollections[i][j].modelMatrices._41 + 64.0f / 2;
					fy1 = m_objectCollections[i][j].modelMatrices._42 + 64.0f / 2;
					break;
			case 1:
			case 2:
			case 3:
			case 4:
				fr1 = sqrtf(16 * 16 + 16 * 16) / 2;
				fx1 = m_objectCollections[i][j].modelMatrices._41 + 32.0f / 2;
				fy1 = m_objectCollections[i][j].modelMatrices._42 + 32.0f / 2;
				break;
			case 5:
			case 6:
			case 7:
			case 8:
				fr1 = sqrtf(8 * 8 + 8 * 8) / 2;
				fx1 = m_objectCollections[i][j].modelMatrices._41 + 16.0f / 2;
				fy1 = m_objectCollections[i][j].modelMatrices._42 + 16.0f / 2;
				break;

			}
			//��������
			fr = sqrtf((fx1 - fx2)*(fx1 - fx2) + (fy1 - fy2)*(fy1 - fy2));
			if (fr < (fr1 + fr2)) {
				//����
				m_objectCollections[i][j].status = 0;
				AddBomb(i, m_objectCollections[i][j].modelMatrices._41, m_objectCollections[i][j].modelMatrices._42, 10.0f);
				//��ʧ��
				m_nLife--;
				if (m_nLife < 1) {
					m_nProgstatus = PStatus::GAMEOVER;
					m_nDelayTimes = 10;
					PlayGameSound(HUM);
				}
				else
				{
					m_nProgstatus = PStatus::LOSTLIFE;
					m_nDelayTimes = 10;
					PlayGameSound(BOMB);
				}
				return;
			}
		}
	}
	//��齱����
	for (int j = 4050;j < 4099;j++) {
		if (m_objectCollections[i][j].status == 1) {
			float fr1 = sqrtf(4 * 4 + 4 * 4) / 2;
			float fr2 = sqrtf(32 * 32 + 32 * 32) / 2;
			//�������
			float fx1 = m_objectCollections[i][900].modelMatrices._41 + 32.0f / 2;
			float fy1 = m_objectCollections[i][900].modelMatrices._42 + 32.0f / 2;
			//ը������
			float fx2 = m_objectCollections[i][j].modelMatrices._41 + 8.0f / 2;
			float fy2 = m_objectCollections[i][j].modelMatrices._42 + 8.0f / 2;
			//��������
			float fr = sqrtf((fx1 - fx2)*(fx1 - fx2) + (fy1 - fy2)*(fy1 - fy2));
			if (fr < (fr1 + fr2)) {
				//���ؽ�����
				m_objectCollections[i][j].status = 0;
				//����
			 	m_uStars++;
				if (m_uStars > BONUSTOLIFE_VALUE) {
					m_uStars -= BONUSTOLIFE_VALUE;
					m_nLife++;
					//��������
					m_objectCollections[i][900].animcurrent = 0;
					m_nProgstatus =PStatus::BONUSANIM;
				}
				return;
			}
		}
	}
}

//����Ƿ������̥��
BOOL CGames::HitOnCircle(UINT i,GAMEOBJECT o) {
	for (UINT j = 0;j < 100;j++) {
		if (m_objectCollections[i][j].status == 1)
		{
			float fr1 = sqrtf(4 * 4 + 4 * 4) / 2;
			float fr2 = sqrtf(32 * 32 + 32 * 32) / 2;
			//�ӵ�����
			float fx1 = o.modelMatrices._41 + 4.0f;
			float fy1 = o.modelMatrices._42 + 4.0f;
			//��̥����
			float fx2 = m_objectCollections[i][j].modelMatrices._41 + 64.0f / 2;
			float fy2 = m_objectCollections[i][j].modelMatrices._42 + 64.0f / 2;
			//��������
			float fr = sqrtf((fx1 - fx2)*(fx1 - fx2) + (fy1 - fy2)*(fy1 - fy2));
			if (fr < (fr1 + fr2)) {
				AddObject(i, 1, m_objectCollections[i][j].modelMatrices._41, m_objectCollections[i][j].modelMatrices._42, 0, 0, 1);
				AddObject(i, 2, m_objectCollections[i][j].modelMatrices._41, m_objectCollections[i][j].modelMatrices._42, 0, 0, 1);
				AddObject(i, 3, m_objectCollections[i][j].modelMatrices._41, m_objectCollections[i][j].modelMatrices._42, 0, 0, 1);
				AddObject(i, 4, m_objectCollections[i][j].modelMatrices._41, m_objectCollections[i][j].modelMatrices._42, 0, 0, 1);
				//������̥
				m_objectCollections[i][j].status = 0;
				//����
				m_uScore += CIRCLE_SCORE;
				PlayGameSound(STICK);
				return TRUE;
			}
		}
	}
	return FALSE;
}
//����Ƿ���������
BOOL CGames::HitOnToy(UINT i, GAMEOBJECT o) {
	for (UINT j = 100;j < 500;j++) {
		if (m_objectCollections[i][j].status == 1)
		{
			float fr1 = sqrtf(4 * 4 + 4 * 4) / 2;
			float fr2 = sqrtf(16 * 16 + 16 * 16) / 2;
			//�ӵ�����
			float fx1 = o.modelMatrices._41 + 4.0f;
			float fy1 = o.modelMatrices._42 + 4.0f;
			//�������
			float fx2 = m_objectCollections[i][j].modelMatrices._41 + 32.0f / 2;
			float fy2 = m_objectCollections[i][j].modelMatrices._42 + 32.0f / 2;
			//��������
			float fr = sqrtf((fx1 - fx2)*(fx1 - fx2) + (fy1 - fy2)*(fy1 - fy2));
			if (fr < (fr1 + fr2)) {
                //�����ɿ���
				switch (m_objectCollections[i][j].type)
				{
				case 1:	AddObject(i, 5, m_objectCollections[i][j].modelMatrices._41, m_objectCollections[i][j].modelMatrices._42, 0, 0, 4);
				case 2:	AddObject(i, 6, m_objectCollections[i][j].modelMatrices._41, m_objectCollections[i][j].modelMatrices._42, 0, 0, 4);
				case 3:	AddObject(i, 7, m_objectCollections[i][j].modelMatrices._41, m_objectCollections[i][j].modelMatrices._42, 0, 0, 4);
				case 4:	AddObject(i, 8, m_objectCollections[i][j].modelMatrices._41, m_objectCollections[i][j].modelMatrices._42, 0, 0, 4);
				}
				//�������
				m_objectCollections[i][j].status = 0;
				//����
				m_uScore += TOY_SCORE;
				PlayGameSound(COLLIDE);
				return TRUE;
			}
		}
	}
	return FALSE;
}
//����Ƿ�����ɿ���
BOOL CGames::HitOnChocolate(UINT i, GAMEOBJECT o) {
	for (UINT j = 500;j < 900;j++) {
		if (m_objectCollections[i][j].status == 1)
		{
			float fr1 = sqrtf(4 * 4 + 4 * 4) / 2;
			float fr2 = sqrtf(8 * 8 + 8 * 8) / 2;
			//�ӵ�����
			float fx1 = o.modelMatrices._41 + 4.0f;
			float fy1 = o.modelMatrices._42 + 4.0f;
			//�ɿ�������
			float fx2 = m_objectCollections[i][j].modelMatrices._41 + 16.0f / 2;
			float fy2 = m_objectCollections[i][j].modelMatrices._42 + 16.0f / 2;
			//��������
			float fr = sqrtf((fx1 - fx2)*(fx1 - fx2) + (fy1 - fy2)*(fy1 - fy2));
			if (fr < (fr1 + fr2)) {
				//�����ɿ���
				m_objectCollections[i][j].status = 0;
				AddBomb(i, m_objectCollections[i][j].modelMatrices._41, m_objectCollections[i][j].modelMatrices._42,10.0f);
				//����
				m_uScore += CHOCOLATE_SCORE;
				PlayGameSound(BOMB);
				return TRUE;
			}
		}
	}
	return FALSE;
}



BOOL CGames::LoadSoundFile() {
	TCHAR*sndfile[]{
		L"bangbang.wav",
		L"bounce.wav",
		L"c_bang.wav",
		L"d_bang.wav",
		L"p_bang.wav",
		L"s_bang.wav",
		L"gunfire.wav",
		L"hum.wav",
		L"level.wav",
		L"rev.wav",
		L"shield.wav",
		L"skid.wav",
	};
	int len = sizeof(sndfile) / sizeof(sndfile[0]);
	m_sound.resize(len);
	for (int i = 0;i < len;i++) {
	   TCHAR szMsg[MAX_PATH];
	   wsprintf(szMsg, L"%s", m_sender->GetAssetFullPath(sndfile[i]).c_str());
	   ByteArray ba = ReadSndFile(m_sender->GetAssetFullPath(sndfile[i]));
	   int size = ba->size();
	   m_sound[i].resize(size);
	   BYTE* pData = &m_sound[i][0];
	   memcpy(pData, ba->data(), ba->size());
	}
	return TRUE;
}
ByteArray CGames::ReadSndFile(const std::wstring& fileName)
{

	struct _stat64 fileStat;
	int fileExists = _wstat64(fileName.c_str(), &fileStat);
	if (fileExists == -1)	return NullFile;
	ifstream file(fileName, ios::in | ios::binary);
	if (!file)	return NullFile;
	//(unsigned int)��32λ������C4244 ������ʾ��64λ����Ӧ(unsigned int)�������ݽض�
	ByteArray byteArray = make_shared<vector<byte> >((unsigned int)file.seekg(0, ios::end).tellg());
	file.seekg(0, ios::beg).read((char*)byteArray->data(), byteArray->size());
	file.close();
	return byteArray;
}
VOID CGames::PlayGameSound(int id)
{
	if (m_bWantSound)
	{
		BYTE* pData = &m_sound[id][0];
		PlaySound((LPCWSTR)(LPVOID)pData, NULL, SND_MEMORY | SND_ASYNC);
	}
}

VOID CGames::QuitGame() {

	PostQuitMessage(0);
	m_nProgstatus = PStatus::QUIT;
}