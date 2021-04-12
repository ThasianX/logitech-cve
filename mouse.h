#pragma once
#ifndef MOUSE_H
#define MOUSE_H

#include <stdbool.h>

typedef struct {
	char button;
	char x;
	char y;
	char wheel;
	char unk1;
} MOUSE_IO;

bool open_mouse();
void close_mouse();

void left_press_in_mouse();
void middle_press_in_mouse();
void right_press_in_mouse();
void press_out_mouse();
void move_mouse(char x, char y);
void move_wheel(char wheel);
void button_press_in(char button);

#endif
