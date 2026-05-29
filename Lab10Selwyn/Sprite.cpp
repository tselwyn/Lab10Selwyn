// Tyler Selwyn
// CPSC 440 - Game Programming
// Lab 10 - Sprite Specialty Powers
// Sprite.cpp - Sprite class implementation

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <cstdlib>
#include "Sprite.h"

sprite::sprite()
{
	x = rand() % 400;
	y = rand() % 300;
	width = 0;
	height = 0;
	xspeed = rand() % 4 + 2;
	yspeed = rand() % 4 + 2;
	xdelay = 0;
	ydelay = 0;
	xcount = 0;
	ycount = 0;
	curframe = 0;
	maxframe = 0;
	animdir = 1;
	framecount = 0;
	framedelay = 5;

	// specialty power defaults
	for (int i = 0; i < 4; i++)
		specialtyPower[i] = false;

	CollisionIsTrue = false;
	angle = 0.0f;
	tintR = 1.0f;
	tintG = 1.0f;
	tintB = 1.0f;
	scale = 1.0f;
	scaleTimer = 0;
	dead = false;
	freezeTimer = 0;
}

sprite::~sprite()
{
	for (int i = 0; i < maxframe; i++)
		al_destroy_bitmap(image[i]);
}

void sprite::load_animated_sprite(int size)
{
	char s[80];
	maxframe = size;
	for (int n = 0; n < size; n++)
	{
		sprintf_s(s, "Sprite%d.bmp", n);
		image[n] = al_load_bitmap(s);
		al_convert_mask_to_alpha(image[n], al_map_rgb(255, 255, 255));
	}
	width = al_get_bitmap_width(image[0]);
	height = al_get_bitmap_height(image[0]);
	curframe = 0;
	framedelay = 5;
	framecount = 0;
}

void sprite::updatesprite()
{
	if (dead) return;

	if (++xcount > xdelay)
	{
		xcount = 0;
		x += xspeed;
	}

	if (++ycount > ydelay)
	{
		ycount = 0;
		y += yspeed;
	}

	if (framecount++ > framedelay)
	{
		framecount = 0;
		curframe++;
		if (curframe >= maxframe)
			curframe = 0;
	}
}

void sprite::bouncesprite(int SCREEN_W, int SCREEN_H)
{
	if (dead) return;

	if (x < 0)
	{
		x = 0;
		xspeed = rand() % 2 + 4;
		animdir *= -1;
	}
	else if (x > SCREEN_W - width)
	{
		x = SCREEN_W - width;
		xspeed = rand() % 2 - 6;
		animdir *= -1;
	}

	if (y < 0)
	{
		y = 0;
		yspeed = rand() % 2 + 4;
		animdir *= -1;
	}
	else if (y > SCREEN_H - height)
	{
		y = SCREEN_H - height;
		yspeed = rand() % 2 - 6;
		animdir *= -1;
	}
}

void sprite::drawSprite()
{
	if (dead) return;

	al_draw_bitmap(image[curframe], x, y, 0);
}

void sprite::assignRandomPower()
{
	int power = rand() % 4;
	specialtyPower[power] = true;
}

void sprite::collision(sprite spriteArray[], int arraySize, int myIndex, int screenW, int screenH)
{
	// collision detection added in commit 3
}