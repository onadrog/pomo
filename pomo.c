#include <raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FOCUS_TIMER 25.0f
#define BREAK_TIMER 5.0f

#define SET_TIMER(timer) timer * 60.0f

typedef enum {
  FOCUS = 1,
  BREAK = 2,
  PAUSE = 3

} FLAGS;

void convert_seconds_to_time(float sec, int timer_size) {
  int hours = (int)(sec / 3600);
  int minutes = (int)((sec - (hours * 3600)) / 60);
  int seconds = (int)(sec - (hours * 3600) - (minutes * 60));

  const char *timer = TextFormat("%02d:%02d:%02d\n", hours, minutes, seconds);
  DrawText(timer, (SCREEN_WIDTH - timer_size) * 0.5 - 20, 300, 20, WHITE);
}

void manage_states() {}

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "pomo");
  const Color focus_clr = {.r = 59, .g = 66, .b = 82, .a = 1};
  const Color pause_clr = {.r = 235, .g = 203, .b = 139, .a = 1};
  const char *msg = "Focus";
  const float font_size = (float)(SCREEN_WIDTH * (1.0 / 4.0));
  float timer = SET_TIMER(FOCUS_TIMER);
  int msg_size = MeasureText(msg, font_size);
  int timer_size = MeasureText("00:00:00", 20);
  FLAGS flag = FOCUS;
  FLAGS last_flag = flag;

  InitAudioDevice();

  Sound timer_sfx = LoadSound("./sfx/timer_sfx.wav");
  Sound pause_sfx = LoadSound("./sfx/pause_sfx.wav");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();

    if (IsKeyPressed(KEY_SPACE)) {
      if (flag ^ PAUSE) {
        PlaySound(pause_sfx);
        msg = "PAUSE";
        msg_size = MeasureText(msg, font_size);
        last_flag = flag;
        flag = PAUSE;
      } else {
        PlaySound(pause_sfx);
        msg = "FOCUS";
        msg_size = MeasureText(msg, font_size);
        flag = last_flag;
      }
    }

    if (flag ^ PAUSE)
      timer -= GetFrameTime();

    if (flag & BREAK)
      ClearBackground(pause_clr);
    else
      ClearBackground(focus_clr);

    if ((timer <= 0.0) && flag & FOCUS) {
      PlaySound(timer_sfx);
      flag = BREAK;
      msg = "BREAK";
      msg_size = MeasureText(msg, font_size);
      timer = SET_TIMER(BREAK_TIMER);
    } else if ((timer <= 0.0) && flag & BREAK) {
      PlaySound(timer_sfx);
      flag = FOCUS;
      msg = "FOCUS";
      msg_size = MeasureText(msg, font_size);
      timer = SET_TIMER(FOCUS_TIMER);
    }
    DrawText(msg, (SCREEN_WIDTH - msg_size) * 0.5, 20, 200, WHITE);
    convert_seconds_to_time(timer, timer_size);
    EndDrawing();
  }

  UnloadSound(timer_sfx);
  UnloadSound(pause_sfx);

  CloseAudioDevice();
  CloseWindow();

  return 0;
}
