#include <stdio.h>
#include <graphics.h>
#include <mmsyscom.h>//win32��ý��ӿ�
#pragma comment(lib,"winmm.lib")
#define WIN_WIDTH 800
#define WIN_HEIGHT 810
#define BULLET_NUM 15//����ӵ�����
#define ENEMY_NUM 10
struct Image
{
	IMAGE background;//����ͼƬ
	IMAGE player[2];
	IMAGE bullet[2];
	IMAGE enemy[4];
}image;
enum TYPE
{
	SMALL,
	BIG,
};
struct Plane
{
	int x;
	int y;
	bool flag;//�Ƿ���
	union 
	{
		int type;//�л�����
		int score;//�����������

	}un;
	int hp;//Ѫ��
}player, bullet[BULLET_NUM],enemy[ENEMY_NUM];
DWORD t1, t2 ,t3,t4;
void EnemyHp(int i)
{
	if (rand() % 10 == 0)
	{
		enemy[i].un.type = BIG;
		enemy[i].hp = 3;
	}
	else
	{
		enemy[i].un.type = SMALL;
		enemy[i].hp = 1;
	}
}
//����ͼƬ
void GameInit()
{
	//�������������
	srand(GetTickCount());
	t1 = t2 = t3 = t4 = GetTickCount();
	//��������
	mciSendString("open ./image/hktk.mp3 alias BGM",0,0,0);//���ý���豸�ӿڷ����ַ��� media device interface
	mciSendString("play BGM repeat",0, 0, 0);//��������
	//���ر���ͼƬ
	loadimage(&image.background, "./image/background.jpg");//��ͼƬ���ز�����
	loadimage(&image.player[0],"./image/tuyan.png");
	loadimage(&image.player[1], "./image/xiyan.png");
	loadimage(&image.bullet[0], "./image/bullet.png");
	loadimage(&image.bullet[1], "./image/bullet1.png");

	loadimage(&image.enemy[0], "./image/bullet1.png");
	loadimage(&image.enemy[1], "./image/bullet1.png");
	loadimage(&image.enemy[2], "./image/bullet1.png");
	loadimage(&image.enemy[3], "./image/bullet1.png");

	//��ʼ���������
	player.x = WIN_WIDTH/2;
	player.y = WIN_HEIGHT -156;
	player.flag = true;
	player.hp = 99;
	player.un.score = 0;
	//��ʼ���ӵ�����
	for (int i = 0; i < BULLET_NUM; i++)
	{
		bullet[i].flag = false;

	}
	//��ʼ���л�
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].flag = false;
		EnemyHp(i);
	}


}
void Gamedraw() 
{
	putimage(0, 0, &image.background);//��������ͼƬ
	putimage(player.x, player.y, &image.player[0], NOTSRCERASE);//�����ɻ�
	putimage(player.x, player.y, &image.player[1], SRCINVERT);
	//�����ӵ�
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].flag == true)
		{
			putimage(bullet[i].x, bullet[i].y, &image.bullet[0], NOTSRCERASE);//�����ӵ�
			putimage(bullet[i].x, bullet[i].y, &image.bullet[1], SRCINVERT);

		}
	}
	//���Ƶл�
	for (int  i = 0; i < ENEMY_NUM ;i++)
	{
		if (enemy[i].flag)
		{
			if (enemy[i].un.type == TYPE::SMALL)
			{
				putimage(enemy[i].x, enemy[i].y, &image.bullet[0], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &image.bullet[0], SRCINVERT);
			}
			else
			{
				putimage(enemy[i].x, enemy[i].y, &image.bullet[1], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &image.bullet[1], SRCINVERT);
			}
		}
	}
}
void CreateBullet()
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].flag == false)
		{
			bullet[i].flag = true;
			bullet[i].x = player.x +50;
			bullet[i].y = player.y;
			break;
		}
	}
}
void BulletMove(int speed)
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].flag == true)
		{
			bullet[i].y -=speed;
			if (bullet[i].y <= 0)
			{
				bullet[i].flag = false;
			}
		}
	}


}
void CreateEnemy() 
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].flag == 0)
		{
			enemy[i].flag = true;
			enemy[i].x = rand() % (WIN_WIDTH - 100);
			enemy[i].y = 0;
			EnemyHp(i);//�����л�
			break;

		}
	}
}
void EnemyMove(int speed)
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].flag )
		{
			enemy[i].y +=speed;
			if (enemy[i].y >=WIN_HEIGHT)
			{
				enemy[i].flag = false;
			}
		}
	}

}
void GameControl(int speed)
{
	if (GetAsyncKeyState(VK_UP)&&player.y>=0)
	{
		player.y -= speed;
	}
	if (GetAsyncKeyState(VK_DOWN) && player.y +120<= WIN_HEIGHT)
	{
		player.y += speed;
	}
	if (GetAsyncKeyState(VK_LEFT)&&player.x+75>=0)
	{
		player.x -= speed;
	}
	if (GetAsyncKeyState(VK_RIGHT)&&player.x+75<=WIN_WIDTH)
	{
		player.x += speed;
	}
	//�����ӵ�
	if (GetAsyncKeyState(VK_SPACE)&&t2-t1>200)
	{
		CreateBullet();
		//mciSendString("close gun"��0��0��0)�ӵ���������
		//mciSendString("open ./image/�ļ�  alias gun"��0��0��0)�ӵ�����
		//mciSendString("play gun"��0��0��0)�ӵ������ر�

		t1 = t2;
	}
	t2 = GetTickCount();
	if (t4 - t3 > rand() % 500 + 500)
	{
		CreateEnemy();

		t3 = t4;
	}
	t4 = GetTickCount();
	EnemyMove(1);
	BulletMove(2);

}
void PlayEnemy()
{
	//�����л����飬�ж��Ƿ���
	//�����ӵ�����
	//�ж��ӵ��Ƿ���ел�������
	//���� ���Ըı�Ѫ��-- ���״̬  ��ҷ���--
	//���Ѫ��Ϊ0 �л�����
}
int main()
{
	initgraph(WIN_WIDTH, WIN_HEIGHT/*,SHOWCONSOLE*/);//����һ��ͼ�δ���,
	GameInit();
	BeginBatchDraw();//����˫���ػ�ͼ�������ڴ��ﻭ�ã�����ʾ
	while (1)
	{
		Gamedraw();
		FlushBatchDraw();//�뿴����ʾ����
		GameControl(2);
	}
	EndBatchDraw();//����
	return 0;

}