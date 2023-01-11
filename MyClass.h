#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<easyx.h>
#include<string>
#include<vector>
#include<time.h>
#include<Windows.h>
#include<queue>
using namespace std;
extern const int Win_Width;
extern const int Win_Height;
extern int MouseClick;
class GameSpirit
{
public:
	GameSpirit();
	GameSpirit(const char*,int,int);
	void updata();
	void draw();
	void setX(int x) {
		this->x = x;
	}
	void setY(int y) {
		this->y = y;
	}
	void setWidth(int width) {
		this->width = width;
	}
	void setHeight(int height) {
		this->height = height;
	}
	void setSpeedX(int speed_X) {
		this->speed_X = speed_X;
	}
	void setSpeedY(int speed_Y) {
		this->speed_Y = speed_Y;
	}
	int getX() {
		return this->x;
	}
	int getY() {
		return this->y;
	}
	int getWidth() {
		return this->width;
	}
	int getHeight() {
		return this->height;
	}
	IMAGE* getImage() {
		return &(this->image);
	}
	~GameSpirit();

private:
	IMAGE image;
	int x, y, width, height, speed_X, speed_Y;
};

GameSpirit::GameSpirit(){}
GameSpirit::GameSpirit(const char* image,int width,int height) {
	this->x = 0;
	this->y = 0;
	this->speed_X = 0;
	this->speed_Y = 0;
	loadimage(&(this->image), image, width, height);
	this->width = this->image.getwidth();
	this->height = this->image.getheight();
}
void GameSpirit::updata() {
	this->x += this->speed_X;
	this->y += this->speed_Y;
}
void GameSpirit::draw() {
	putimage(this->x, this->y, &(this->image));
}
GameSpirit::~GameSpirit()
{
}
class BackGround : public GameSpirit
{
public:
	BackGround();
	BackGround(const char*, int,int);
	void updata();
	~BackGround();

private:
};

BackGround::BackGround()
{
}
BackGround::BackGround(const char* image, int speed_X,int alt=0) :GameSpirit(image, Win_Width, Win_Height) {
	if (alt)
		this->setX(Win_Width);
	this->setSpeedX(speed_X);
}
void BackGround::updata() {
	GameSpirit::updata();
	if (this->getX() + this->getWidth() <= 0)
		this->setX(Win_Width);
}
BackGround::~BackGround()
{
}
class Bird:public GameSpirit
{
public:
	Bird();
	Bird(const char*,int,int,int,int,int);
	void updata();
	void draw();
	~Bird();

private:
	IMAGE images[3];
	double degree;
	int shadow;
};

Bird::Bird()
{
}
Bird::Bird(const char* images, int speed_X,int speed_Y,int width,int height,int shadow=0) {
	this->degree = 0.0;
	this->shadow = shadow;
	char tmp[30] = "\0";
	for (int i = 0; i < 3; i++) {
		sprintf(tmp, "images/%s-%d.png", images, i);
		loadimage(&(this->images[i]), tmp, width, height);
	}
	this->setSpeedY(speed_Y);
	this->setSpeedX(speed_X);
	this->setX(0);
	this->setY(Win_Height / 2);
	this->setWidth(width);
	this->setHeight(height);
}
void Bird :: updata() {
	GameSpirit::updata();
	if (this->getX() <= 0)
		this->setX(0);
	if (this->getX() + this->getWidth() >= Win_Width)
		this->setX(Win_Width - this->getWidth());
	if (this->getY() <= 0)
		this->setY(0);
	if (this->getY() + this->getHeight() >= Win_Height)
		this->setY(Win_Height - this->getHeight());
}
void Bird::draw() {
	if (MouseClick) {
		for (int i = 0; i < 3; i++) {
			if (this->shadow)
				putimage(this->getX(), this->getY(), &(this->images[i]), SRCAND);
			else putimage(this->getX(), this->getY(), &(this->images[i]), SRCPAINT);
		}
		MouseClick = 0;
	}
	else {
		this->setSpeedY(1);
		if (this->shadow)
			putimage(this->getX(), this->getY(), &(this->images[0]), SRCAND);
		else putimage(this->getX(), this->getY(), &(this->images[0]), SRCPAINT);
	}
}
Bird::~Bird()
{
}
class Base:public GameSpirit
{
public:
	Base();
	Base(const char*, int,int,int);
	void updata();
	~Base();

private:

};

Base::Base()
{
}
Base::Base(const char* image, int speed_X, int height, int alt = 0):GameSpirit(image,Win_Width,height) {
	if (alt)
		this->setX(Win_Width);
	this->setY(Win_Height - height);
	this->setSpeedX(speed_X);
}
void Base::updata() {
	GameSpirit::updata();
	if (this->getX() + this->getWidth() <= 0)
		this->setX(Win_Width);
}
Base::~Base()
{
}
class Barrier : public GameSpirit
{
public:
	Barrier();
	Barrier(const char*, int, int, int,int);
	void draw();
	~Barrier();

private:
	int shadow;
};

Barrier::Barrier()
{
}
Barrier::Barrier(const char* image, int width, int height, int speed_X,int shadow=0):GameSpirit(image,width,height) {
	this->shadow = shadow;
	this->setSpeedX(speed_X);
}
void Barrier::draw() {
	if (this->shadow)
		putimage(this->getX(), this->getY(), this->getImage(), SRCAND);
	else putimage(this->getX(), this->getY(), this->getImage(), SRCPAINT);
}
Barrier::~Barrier()
{
}