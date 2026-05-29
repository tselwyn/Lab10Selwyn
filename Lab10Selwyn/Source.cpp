// Tyler Selwyn
// CPSC 440 - Game Programming
// Lab 10 - Sprite Specialty Powers
// Source.cpp - Main game loop

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "Sprite.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int NUM_SPRITES = 5;

int main(void)
{
	// screen dimensions
	int width = 640;
	int height = 480;
	bool done = false;
	bool redraw = true;
	const int FPS = 60;

	// allegro variables
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer = NULL;

	// initialize allegro
	if (!al_init())
		return -1;

	display = al_create_display(width, height);
	if (!display)
		return -1;

	// install addons
	al_install_keyboard();
	al_init_image_addon();

	timer = al_create_timer(1.0 / FPS);

	// set up event queue with keyboard, timer, and display
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	srand((unsigned)time(NULL));

	// create array of 5 sprites
	sprite sprites[NUM_SPRITES];

	// load animation frames and assign random specialty power to each
	for (int i = 0; i < NUM_SPRITES; i++)
	{
		sprites[i].load_animated_sprite(9);
		sprites[i].assignRandomPower();
	}

	al_set_target_bitmap(al_get_backbuffer(display));
	al_start_timer(timer);

	// main game loop
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			// update movement and animation for each sprite
			for (int i = 0; i < NUM_SPRITES; i++)
			{
				sprites[i].bouncesprite(width, height);
				sprites[i].updatesprite();
			}
			// check collisions between all sprites
			for (int i = 0; i < NUM_SPRITES; i++)
			{
				sprites[i].collision(sprites, NUM_SPRITES, i, width, height);
			}
			redraw = true;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				done = true;
		}

		// draw all sprites
		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));

			for (int i = 0; i < NUM_SPRITES; i++)
			{
				sprites[i].drawSprite();
			}

			al_flip_display();
		}
	}

	// cleanup
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);

	return 0;
}