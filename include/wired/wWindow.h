#pragma once

typedef struct _wImage wImage;
typedef struct _wString wString;

/* ---------- Window ---------- */
int wWindowOpen(int width, int height);
void wWindowClose();
int wWindowSetIcon(const wImage *img);
int wWindowSetTitle(const wString *title);
int wWindowSetFullscreen();
int wWindowMaximize();
int wWindowMinimize();
int wWindowGetWidth();
int wWindowGetHeight();
int wWindowPollEvents();
int wWindowSwapBuffers();

/* ---------- Screen ---------- */
int wScreenGetCount();
int wScreenGetX(int screen);
int wScreenGetY(int screen);
int wScreenGetWidth(int screen);
int wScreenGetHeight(int screen);
void wScreenGetName(int screen, wString *ret);

/* ---------- Cursor ---------- */
void wCursorShow();
void wCursorHide();
