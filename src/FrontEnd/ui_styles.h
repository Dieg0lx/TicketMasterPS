#ifndef UI_STYLES_H
#define UI_STYLES_H

#include "../../include/raylib.h"

extern Sound g_fxClick;

#define COL_BG          (Color){ 20, 24, 28, 255 }
#define COL_PANEL       (Color){ 35, 40, 45, 255 }
#define COL_PRIMARY     (Color){ 2, 108, 223, 255 }
#define COL_ACCENT      (Color){ 60, 150, 255, 255 }
#define COL_TEXT_WHITE  (Color){ 245, 245, 245, 255 }
#define COL_TEXT_GRAY   (Color){ 160, 160, 160, 255 }
#define COL_INPUT_BG    (Color){ 50, 55, 60, 255 }
#define COL_ERROR       (Color){ 231, 76, 60, 255 }
#define COL_SUCCESS     (Color){ 46, 204, 113, 255 }

#define COL_SEAT_FREE   (Color){ 100, 100, 100, 255 }
#define COL_SEAT_BUSY   (Color){ 231, 76, 60, 255 }
#define COL_SEAT_SEL    (Color){ 241, 196, 15, 255 }

static int DrawStyledButton(Rectangle bounds, const char* text) {
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, bounds);
    int clicked = 0;

  if (hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        PlaySound(g_fxClick); 
        clicked = 1;
    }

    DrawRectangleRounded((Rectangle){bounds.x+2, bounds.y+4, bounds.width, bounds.height}, 0.3f, 8, (Color){0,0,0,80});
    
    DrawRectangleRounded(bounds, 0.3f, 8, hover ? COL_ACCENT : COL_PRIMARY);
    
    int fontSize = 20;
    int textW = MeasureText(text, fontSize);
    DrawText(text, 
        (int)(bounds.x + bounds.width/2 - textW/2), 
        (int)(bounds.y + bounds.height/2 - fontSize/2), 
        fontSize, WHITE);

    return clicked;
}

static void DrawStyledInput(Rectangle bounds, const char* text, bool isActive, bool isPassword) {
    DrawRectangleRounded(bounds, 0.2f, 6, COL_INPUT_BG);
    
    if (isActive) {
        DrawRectangleLinesEx(bounds, 2, COL_PRIMARY);
    }

    int fontSize = 20;
    if (isPassword) {
        char mask[50] = {0};
        int len = TextLength(text);
        for(int i=0; i<len && i<49; i++) mask[i] = '*';
        DrawText(mask, (int)bounds.x + 10, (int)bounds.y + 15, fontSize, COL_TEXT_WHITE);
    } else {
        DrawText(text, (int)bounds.x + 10, (int)bounds.y + 15, fontSize, COL_TEXT_WHITE);
    }

    if (isActive && ((int)(GetTime() * 2) % 2 == 0)) {
        int textW = isPassword ? MeasureText("****", fontSize) : MeasureText(text, fontSize); 
        if (isPassword) textW = TextLength(text) * MeasureText("*", fontSize);
        
        DrawText("|", (int)bounds.x + 10 + textW, (int)bounds.y + 15, fontSize, COL_PRIMARY);
    }
}

static void DrawNotification(const char* msg, float alpha) {
    if (alpha <= 0) return;
    
    int width = MeasureText(msg, 20) + 40;
    Rectangle r = { GetScreenWidth()/2 - width/2, 50, (float)width, 40 };
    
    Color bg = COL_SUCCESS;
    bg.a = (unsigned char)(255 * alpha);
    Color txt = WHITE;
    txt.a = (unsigned char)(255 * alpha);

    DrawRectangleRounded(r, 0.5f, 8, bg);
    DrawText(msg, r.x + 20, r.y + 10, 20, txt);
}

static void DrawCartIcon(float x, float y, int itemCount) {
    Color tint = COL_TEXT_WHITE;
    
    DrawCircle(x + 10, y + 25, 2, tint); 
    DrawCircle(x + 22, y + 25, 2, tint); 
    
    DrawLineEx((Vector2){x + 5, y + 20}, (Vector2){x + 25, y + 20}, 2, tint); 
    DrawLineEx((Vector2){x + 5, y + 20}, (Vector2){x + 2, y + 5}, 2, tint);   
    DrawLineEx((Vector2){x + 25, y + 20}, (Vector2){x + 28, y + 5}, 2, tint); 
    DrawLineEx((Vector2){x + 2, y + 5}, (Vector2){x + 28, y + 5}, 2, tint);   
    
    DrawLineEx((Vector2){x + 28, y + 5}, (Vector2){x + 32, y + 0}, 2, tint); 
    
    if (itemCount >= 0) { 
        DrawCircle(x + 30, y - 5, 8, COL_ERROR);
        const char* num = TextFormat("%d", itemCount);
        int w = MeasureText(num, 10);
        DrawText(num, x + 30 - w/2, y - 10, 10, WHITE);
    }
}

#endif