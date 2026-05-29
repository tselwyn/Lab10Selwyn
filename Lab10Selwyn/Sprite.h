// Tyler Selwyn
// CPSC 440 - Game Programming
// Lab 10 - Sprite Specialty Powers
// Sprite.h - Sprite class header with specialty power support

#ifndef SPRITE_H
#define SPRITE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class sprite
{
public:
	sprite();
	~sprite();
	void updatesprite();
	void bouncesprite(int SCREEN_W, int SCREEN_H);
	void load_animated_sprite(int size);
	void drawSprite();
	void assignRandomPower();
	void collision(sprite spriteArray[], int arraySize, int myIndex, int screenW, int screenH);

	int getX() { return x; }
	int getY() { return y; }
	int getWidth() { return width; }
	int getHeight() { return height; }

private:
	int x, y;
	int width, height;
	int xspeed, yspeed;
	int xdelay, ydelay;
	int xcount, ycount;
	int curframe, maxframe, animdir;
	int framecount, framedelay;
	ALLEGRO_BITMAP* image[9];

	// specialty powers - only one is true per sprite
	bool specialtyPower[4]; // 0=Spinning, 1=Scared, 2=Baby, 3=Freeze
	bool CollisionIsTrue;

	// SpinningSprite
	float angle;

	// ScaredSprite
	float tintR, tintG, tintB;

	// BabySprite
	float scale;
	int scaleTimer;
	bool dead;

	// FreezeSprite
	int freezeTimer;
};

#endif