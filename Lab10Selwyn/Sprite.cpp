// Tyler Selwyn
// CPSC 440 - Game Programming
// Lab 10 - Sprite Specialty Powers
// Sprite.cpp - Sprite class implementation

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include "Sprite.h"
using namespace std;

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

	// SpinningSprite always rotates
	if (specialtyPower[0])
	{
		angle += 0.05f;
	}

	// FreezeSprite - if frozen, count down and skip movement
	if (specialtyPower[3] && freezeTimer > 0)
	{
		freezeTimer--;
		if (freezeTimer <= 0)
			CollisionIsTrue = false;

		// still animate frames but dont move
		if (framecount++ > framedelay)
		{
			framecount = 0;
			curframe++;
			if (curframe >= maxframe)
				curframe = 0;
		}
		return;
	}

	// BabySprite - count down scale timer
	if (specialtyPower[2] && scaleTimer > 0)
	{
		scaleTimer--;
		if (scaleTimer <= 0)
		{
			scale = 1.0f;
			CollisionIsTrue = false;
		}
	}

	// normal movement
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
	if (specialtyPower[3] && freezeTimer > 0) return;

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

	float w = (float)al_get_bitmap_width(image[curframe]);
	float h = (float)al_get_bitmap_height(image[curframe]);

	// SpinningSprite - rotates around center
	if (specialtyPower[0])
	{
		al_draw_rotated_bitmap(image[curframe], w / 2, h / 2,
			(float)x + w / 2, (float)y + h / 2, angle, 0);
	}
	// ScaredSprite - tinted color after collision
	else if (specialtyPower[1] && CollisionIsTrue)
	{
		al_draw_tinted_bitmap(image[curframe],
			al_map_rgba_f(tintR, tintG, tintB, 1.0f), (float)x, (float)y, 0);
	}
	// BabySprite - scaled down after collision
	else if (specialtyPower[2] && CollisionIsTrue && scaleTimer > 0)
	{
		float scaledW = w * scale;
		float scaledH = h * scale;
		float drawX = (float)x + (w - scaledW) / 2;
		float drawY = (float)y + (h - scaledH) / 2;
		al_draw_scaled_bitmap(image[curframe], 0, 0, w, h,
			drawX, drawY, scaledW, scaledH, 0);
	}
	// FreezeSprite - draws normal but frozen in place
	else if (specialtyPower[3] && CollisionIsTrue && freezeTimer > 0)
	{
		al_draw_bitmap(image[curframe], (float)x, (float)y, 0);
	}
	// normal draw
	else
	{
		al_draw_bitmap(image[curframe], (float)x, (float)y, 0);
	}
}

void sprite::assignRandomPower()
{
	int power = rand() % 4;
	specialtyPower[power] = true;
}

void sprite::collision(sprite spriteArray[], int arraySize, int myIndex, int screenW, int screenH)
{
	if (dead) return;

	for (int i = 0; i < arraySize; i++)
	{
		if (i == myIndex) continue;
		if (spriteArray[i].dead) continue;

		if (x < spriteArray[i].getX() + spriteArray[i].getWidth() &&
			x + width > spriteArray[i].getX())
		{
			if (y < spriteArray[i].getY() + spriteArray[i].getHeight() &&
				y + height > spriteArray[i].getY())
			{
				// ScaredSprite - random color and teleport
				if (specialtyPower[1])
				{
					tintR = (float)(rand() % 100) / 100.0f;
					tintG = (float)(rand() % 100) / 100.0f;
					tintB = (float)(rand() % 100) / 100.0f;
					CollisionIsTrue = true;
					x = rand() % (screenW - width);
					y = rand() % (screenH - height);
				}
				// BabySprite - scale in half and teleport
				else if (specialtyPower[2])
				{
					scale *= 0.5f;
					scaleTimer = 600; // 10 seconds at 60 FPS
					CollisionIsTrue = true;
					x = rand() % (screenW - width);
					y = rand() % (screenH - height);

					if (scale < 0.05f)
					{
						dead = true;
						cout << "Sprite died!" << endl;
					}
				}
				// FreezeSprite - stop movement for 5 seconds
				else if (specialtyPower[3])
				{
					freezeTimer = 300; // 5 seconds at 60 FPS
					CollisionIsTrue = true;
				}
				// SpinningSprite or default - just teleport
				else
				{
					CollisionIsTrue = true;
					x = rand() % (screenW - width);
					y = rand() % (screenH - height);
				}
			}
		}
	}
}