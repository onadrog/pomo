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

#include "pomo.h"
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FOCUS_TIMER 25.0f // MINS
#define BREAK_TIMER 5.0f  // MIN

void convert_seconds_to_time(float sec, int timer_size) {
  int hours = (int)(sec / 3600);
  int minutes = (int)((sec - (hours * 3600)) / 60);
  int seconds = (int)(sec - (hours * 3600) - (minutes * 60));

  const char *timer = TextFormat("%02d:%02d:%02d\n", hours, minutes, seconds);
  DrawText(timer, (SCREEN_WIDTH - timer_size) * 0.5 - 20, 300, 20, WHITE);
}

int main(void) {

  if (ChangeDirectory(GetApplicationDirectory()) != 1) {
    printf("Could not load project directory.\n");
    return 1;
  }

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pomo timer");
  const State states[3] = {
      {
          .state = FOCUS,
          .msg = FOCUS_MSG,
          .msg_len = MeasureText(FOCUS_MSG, FONT_SIZE),
          .duration = FOCUS_TIMER,
          .bg_color = {.r = 59, .g = 66, .b = 82, .a = 1},
          .fg_color = WHITE,
      },
      {
          .state = BREAK,
          .msg = BREAK_MSG,
          .msg_len = MeasureText(BREAK_MSG, FONT_SIZE),
          .duration = BREAK_TIMER,
          .bg_color = {.r = 59, .g = 66, .b = 82, .a = 1},
          .fg_color = WHITE,
      },
      {
          .state = PAUSE,
          .msg = PAUSE_MSG,
          .msg_len = MeasureText(PAUSE_MSG, FONT_SIZE),
          .duration = 0.0f,
          .bg_color = {.r = 235, .g = 203, .b = 139, .a = 1},
          .fg_color = WHITE,
      }};

  const int timer_size = MeasureText("00:00:00", 20);
  float timer = SET_TIMER(FOCUS_TIMER);
  STATES_FLAGS flag = FOCUS;
  State current_state = states[0];

  InitAudioDevice();

  Sound timer_sfx = LoadSound("./sfx/timer_sfx.wav");
  Sound pause_sfx = LoadSound("./sfx/pause_sfx.wav");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();

    if (IsKeyPressed(KEY_SPACE)) {
      if (flag > PAUSE) {          // already PAUSE flag
        flag ^= PAUSE;             // remove PAUSE flag
        current_state = states[flag >> 1]; // 6 >> 1 = 3 segfault...
      } else {
        flag |= PAUSE;                     // Adding PAUSE flag
        current_state = states[ARRAY_LEN(states) - 1]; // Temporary fix ...
      }
    }
    switch (flag) {
    case FOCUS:
      if (timer <= 0.0) {
        PlaySound(timer_sfx);
        flag = BREAK;
        current_state = states[flag >> 1];
        timer = SET_TIMER(current_state.duration);
      }
      timer -= GetFrameTime();
      break;
    case BREAK:
      if (timer <= 0.0) {
        PlaySound(timer_sfx);
        flag = FOCUS;
        current_state = states[flag >> 1];
        timer = SET_TIMER(current_state.duration);
      }
      timer -= GetFrameTime();
      break;
    case PAUSE:
      PlaySound(pause_sfx);
      break;
    }
    display_state_to_screen(current_state, timer, timer_size);
    EndDrawing();
  }

  UnloadSound(timer_sfx);
  UnloadSound(pause_sfx);

  CloseAudioDevice();
  CloseWindow();

  return 0;
}

void display_state_to_screen(State s, int timer, int timer_size) {
  ClearBackground(s.bg_color);
  DrawText(s.msg, (SCREEN_WIDTH - s.msg_len) * 0.5f, 20, FONT_SIZE, s.fg_color);
  convert_seconds_to_time(timer, timer_size);
}
