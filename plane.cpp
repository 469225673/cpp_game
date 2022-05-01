#include <stdio.h>
#include <graphics.h>
#include <mmsyscom.h>//win32多媒体接口
#pragma comment(lib,"winmm.lib")
#define WIN_WIDTH 800
#define WIN_HEIGHT 810
#define BULLET_NUM 15//最大子弹数量
#define ENEMY_NUM 10
struct Image
{
	IMAGE background;//背景图片
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
	bool flag;//是否存活
	union 
	{
		int type;//敌机类型
		int score;//玩家特有属性

	}un;
	int hp;//血量
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
//加载图片
void GameInit()
{
	//设置随机数种子
	srand(GetTickCount());
	t1 = t2 = t3 = t4 = GetTickCount();
	//播放音乐
	mciSendString("open ./image/hktk.mp3 alias BGM",0,0,0);//向多媒体设备接口发送字符串 media device interface
	mciSendString("play BGM repeat",0, 0, 0);//播放音乐
	//加载背景图片
	loadimage(&image.background, "./image/background.jpg");//把图片加载并保存
	loadimage(&image.player[0],"./image/tuyan.png");
	loadimage(&image.player[1], "./image/xiyan.png");
	loadimage(&image.bullet[0], "./image/bullet.png");
	loadimage(&image.bullet[1], "./image/bullet1.png");

	loadimage(&image.enemy[0], "./image/bullet1.png");
	loadimage(&image.enemy[1], "./image/bullet1.png");
	loadimage(&image.enemy[2], "./image/bullet1.png");
	loadimage(&image.enemy[3], "./image/bullet1.png");

	//初始化玩家数据
	player.x = WIN_WIDTH/2;
	player.y = WIN_HEIGHT -156;
	player.flag = true;
	player.hp = 99;
	player.un.score = 0;
	//初始化子弹数据
	for (int i = 0; i < BULLET_NUM; i++)
	{
		bullet[i].flag = false;

	}
	//初始化敌机
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].flag = false;
		EnemyHp(i);
	}


}
void Gamedraw() 
{
	putimage(0, 0, &image.background);//画出背景图片
	putimage(player.x, player.y, &image.player[0], NOTSRCERASE);//画出飞机
	putimage(player.x, player.y, &image.player[1], SRCINVERT);
	//绘制子弹
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].flag == true)
		{
			putimage(bullet[i].x, bullet[i].y, &image.bullet[0], NOTSRCERASE);//画出子弹
			putimage(bullet[i].x, bullet[i].y, &image.bullet[1], SRCINVERT);

		}
	}
	//绘制敌机
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
			EnemyHp(i);//产生敌机
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
	//发射子弹
	if (GetAsyncKeyState(VK_SPACE)&&t2-t1>200)
	{
		CreateBullet();
		//mciSendString("close gun"，0，0，0)子弹声音开启
		//mciSendString("open ./image/文件  alias gun"，0，0，0)子弹声音
		//mciSendString("play gun"，0，0，0)子弹声音关闭

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
	//遍历敌机数组，判断是否存活
	//遍历子弹数组
	//判断子弹是否击中敌机。坐标
	//打到了 属性改变血量-- 存活状态  玩家分数--
	//如果血量为0 敌机死亡
}
int main()
{
	initgraph(WIN_WIDTH, WIN_HEIGHT/*,SHOWCONSOLE*/);//创建一个图形窗口,
	GameInit();
	BeginBatchDraw();//开启双款重绘图，先在内存里画好，再显示
	while (1)
	{
		Gamedraw();
		FlushBatchDraw();//想看了显示出来
		GameControl(2);
	}
	EndBatchDraw();//结束
	return 0;

}