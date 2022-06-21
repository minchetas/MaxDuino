#pragma once

#ifndef DISPLAY_H
#define DISPLAY_H

static void sendStrXY(const char *string, int X, int Y);
static void setXY(unsigned char col,unsigned char row);
static void sendChar(unsigned char data);

#endif