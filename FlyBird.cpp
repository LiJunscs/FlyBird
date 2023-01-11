#include"MyClass.h"
const int Win_Width = 288;
const int Win_Height = 512;
int MouseClick = 0;
int GameStart = 0;
BackGround bk1, bk2;
Bird bird_spirit, bird_shadow;
Base bs1, bs2;
vector<Barrier> barriers;
vector<Barrier> barriers_shadow;
int Time;
int Score;
void InitGame();
void updata();
void GameOver();
void generate_barrier();
void delete_barrier();
void collision_detect();
void draw();
void isMouseClick();
int main() {
	InitGame();
	while (GameStart) {
		Time++;
		isMouseClick();
		collision_detect();
		updata();
		draw();
		Sleep(16);
	}
	closegraph();
	system("pause");
	return 0;
}
void InitGame() {
	srand(time(NULL));
	Time = 0;
	Score = 0;
	initgraph(Win_Width, Win_Height);
	cleardevice();
	settextstyle(15, 15, "ËÎÌו");
	setbkcolor(WHITE);
	setbkmode(TRANSPARENT);
	bk1 = BackGround("images/background-day.png", -1, 0);
	bk2 = BackGround("images/background-day.png", -1, 1);
	bird_spirit = Bird("redbird", 0, 10, 34, 24);
	bird_shadow = Bird("bluebird_shadow", 0, 10, 34, 24, 1);
	bs1 = Base("images/base.png", -1, 82, 0);
	bs2 = Base("images/base.png", -1, 82, 1);
	IMAGE game_start;
	loadimage(&game_start, "images/message.png", Win_Width, Win_Height);
	putimage(0, 0, &game_start);
	ExMessage msg;
	while (GameStart == 0) {
		if (peekmessage(&msg)) {
			if (msg.message == WM_LBUTTONDOWN)
				GameStart = 1;
		}
	}
	/*bk1.draw();
	bs1.draw();
	bird_spirit.draw();
	bird_shadow.draw();*/
}
void updata() {
	bk1.updata();
	bk2.updata();
	bird_spirit.updata();
	bird_shadow.updata();
	bs1.updata();
	bs2.updata();
	if (Time == 60) {
		generate_barrier();
		Time = 0;
	}
	for (int i = 0; i < barriers.size(); i++) {
		barriers[i].updata();
		barriers_shadow[i].updata();
		delete_barrier();
	}
}
void draw() {
	cleardevice();
	BeginBatchDraw();
	bk1.draw();
	bk2.draw();
	bs1.draw();
	bs2.draw();
	char score_str[25] = "\0";
	sprintf(score_str, "%s : %d", "Score", Score);
	outtextxy(60, 25, score_str);
	bird_shadow.draw();
	bird_spirit.draw();
	for (int i = 0; i < barriers.size(); i++) {
		barriers_shadow[i].draw();
		barriers[i].draw();
	}
	EndBatchDraw();
}
void GameOver() {
	IMAGE game_over, game_over_shadow;
	loadimage(&game_over, "images/gameover.png");
	loadimage(&game_over_shadow, "images/gameover_shadow.png");
	putimage(48, Win_Height / 2, &game_over_shadow, SRCAND);
	putimage(48, Win_Height / 2, &game_over, SRCPAINT);
	Sleep(1000);
	exit(0);
}
void generate_barrier() {
	int width = rand() % 10 + 40;
	int height = rand() % 101 + 110;
	Barrier tmp = Barrier("images/pipe.png", width, height, -2);
	Barrier tmp_shadow = Barrier("images/pipe_shadow.png", width, height, -2, 1);
	tmp.setX(Win_Width - tmp.getWidth());
	tmp_shadow.setX(Win_Width - tmp.getWidth());
	if (height & 1) {
		tmp.setY(bs1.getY() - tmp.getHeight());
		tmp_shadow.setY(bs1.getY() - tmp.getHeight());
	}
	barriers.push_back(tmp);
	barriers_shadow.push_back(tmp_shadow);
	Time = 0;
}
void delete_barrier() {
	if (barriers[0].getX() + barriers[0].getWidth() <= 0) {
		barriers.erase(barriers.begin());
		barriers_shadow.erase(barriers_shadow.begin());
		Score++;
	}
}
void collision_detect() {
	int bird_X = bird_spirit.getX(), bird_width = bird_spirit.getWidth();
	int bird_Y = bird_spirit.getY(), bird_height = bird_spirit.getHeight();
	for (auto p = barriers.begin(); p != barriers.end();p++) {
		int barrier_X = p->getX(), barrier_width = p->getWidth();
		int barrier_Y = p->getY(), barrier_height = p->getHeight();
		if (bird_X + bird_width + 1 >= barrier_X && bird_X + bird_width + 1 <= barrier_X + barrier_width && 
			((bird_Y + bird_height + 1 >= barrier_Y && bird_Y + bird_height + 1 <= barrier_Y + barrier_height)||
			(bird_Y - 1 >= barrier_Y&&bird_Y + 1 <= barrier_Y + barrier_height))) {
			GameOver();
			return;
		}
	}
	if (bird_Y + bird_height >= bs1.getY()) {
		GameOver();
		return;
	}
}
void isMouseClick() {
	ExMessage msg;
	if (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {
			//MouseClick = 1;
			bird_spirit.setSpeedY(-25);
			bird_shadow.setSpeedY(-25);
		}
	}
}