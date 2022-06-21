#pragma once
#ifndef ENCODER_H
#define ENCODER_H

#include "userconfig.h"

#ifdef ENCODER

void encoderSetup();
void encoderLoop();

bool button_any();
bool button_play();
bool button_stop();
bool button_up();
bool button_down();
bool button_menu();
bool button_root();

#endif // ENCODER
#endif // ENCODER_H