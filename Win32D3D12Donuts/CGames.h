#pragma once
class CGraphics;
using namespace DirectX;
#define FrameCount 3
#define DONUTS_BITMAP_WIDTH 1024
#define DONUTS_BITMAP_HEIGHT 1024

#define NearZ 0.1f
#define FarZ  1.0f
#define SubtitleZ   NearZ+0.1f
#define StageZ  FarZ-0.1f

#define BONUSTOLIFE_VALUE 20
#define CIRCLE_SCORE 100
#define TOY_SCORE 50
#define CHOCOLATE_SCORE 20

typedef std::shared_ptr<std::vector<UINT8> > ByteArray;

enum PStatus{
	GAMEMENU=0,
	PLAYGAME=1,
	GAMEOVER=2,
	PASS=3,
	BONUSANIM=4,
	LOSTLIFE=5,
	QUIT
};
enum GameSound {
	BOMB,  //ը
	BOUNCE, //�ζ�
	SCORE,  //�÷�
	STICK,  //ײ��
	COLLIDE,//��ײ
	STAR,   //��
	FIRE,   //����
	HUM,    //����
	LEVEL,  //�ؿ�
	REV,    //ת
	SHIELD, //�����
	SKID   //Ħ��
};
struct GAMEOBJECT
{
	XMFLOAT4X4 modelMatrices;
	XMFLOAT4X4 textureMatrices;
	BYTE status;
	short texid;      //ʹ�������
	BYTE type;        //����
	short animcurrent;//��ǰ����֡
	short animtotal;  //������֡��
	float fxdst;      //xĿ�ĵ�
	float fydst;      //yĿ�ĵ�
	float fxvel;      //x����
	float fyvel;      //y����

};
class CGames
{
	CGraphics*m_sender;
	BYTE keyboard[256];
	DWORD dwPresskeyDelay[256];


	const UINT m_nMaxObjectsCount = 5000;

	float m_fAnimFrame=10;
	float m_fAnimFrameCount=0;

	float m_fMove = 10;
	float m_fMoveCount = 0;

	float m_fBonus = 50;
	float m_fBonusCount = 0;

	UINT m_uLevel;
	UINT m_uScore;
	int m_nLife;
	UINT m_uStars;

	int m_nDelayTimes;
	float m_fDelayCount;

	int m_nProgstatus;

	ByteArray NullFile;
	ByteArray ReadSndFile(const std::wstring& fileName);

	BOOL m_bWantSound;

public:
	inline UINT GetMaxObjectsCount() { return m_nMaxObjectsCount; }
	std::vector<GAMEOBJECT> m_objectCollections[FrameCount];
	std::vector<std::vector<byte>> m_sound;

	CGames(CGraphics*sender);
	~CGames();
	void GetInput(UINT i);
	void OnKeyDown(WPARAM key);
	void OnKeyUp(WPARAM key);
	VOID Update(UINT i, float ElapsedTicks);
	VOID BltLevel(UINT i, int nLevel, float fx, float fy, float fsize);
	VOID BltScore(UINT i, int nScore, float fx, float fy);
	VOID BltSpeaker(UINT, float fx, float fyi);
	VOID BltDonuts(UINT i, float fx, float fy);
	VOID BltStars(UINT i, float fx, float fy, int num);

	VOID BltDelayTimes(UINT i, float fx, float fy, int num);

	VOID DisplaySplash(UINT i, float ElapsedTicks);
	VOID PlayGames(UINT i, float ElapsedTicks);
	VOID GameOver(UINT i, float ElapsedTicks);
	VOID Clearance(UINT i, float ElapsedTicks);
	VOID BonusAnim(UINT i, float ElapsedTicks);
	VOID LostLife(UINT i, float ElapsedTicks);

	VOID UpdateGames(UINT i, float ElapsedTicks);
	VOID DisplayGames(UINT i, float ElapsedTicks);

	VOID Initialize(UINT i);
	VOID AddObject(UINT i, UINT type, float fsx, float fsy, float fdx, float fdy, UINT num);
	VOID AddBomb(UINT i, float fsx, float fsy,float fspeed);
	BOOL HitOnCircle(UINT i,GAMEOBJECT o);
	BOOL HitOnToy(UINT i, GAMEOBJECT o);
	BOOL HitOnChocolate(UINT i, GAMEOBJECT o);

	VOID CheckBulletHit(UINT i);
	BOOL CheckWin(UINT i);
	VOID CheckHitDonuts(UINT i);

	BOOL LoadSoundFile();
	VOID PlayGameSound(int id);

	VOID QuitGame();
};