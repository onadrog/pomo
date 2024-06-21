/*
 * Copyright 2024 Sébastien Gordano @onadrog
 *
 *Licensed under the Apache License, Version 2.0 (the "License");
 *you may not use this file except in compliance with the License.
 *You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef POMO_HEADER
#define POMO_HEADER
#include <raylib.h>

typedef enum {
  FOCUS = 1,
  BREAK = 2,
  PAUSE = 4, // must be last state flag
} STATES_FLAGS;

typedef struct {
  STATES_FLAGS state;
  char *msg;
  int msg_len;
  float duration;
  Color fg_color;
  Color bg_color;
} State;

#define FOCUS_MSG "FOCUS"
#define BREAK_MSG "BREAK"
#define PAUSE_MSG "PAUSE"

#define SET_TIMER(timer) timer * 60.f
#define FONT_SIZE (float)(SCREEN_WIDTH * 0.25f)

/**
 * @brief Convert seconds to time format
 * @param sec seconds to convert to time format HH:MM:SS
 * @param timer_size size of the timer text in pixels
 */
void convert_seconds_to_time(float sec, int timer_size);
void display_state_to_screen(State s, int timer, int timer_size);
#endif // POMO_HEADER
