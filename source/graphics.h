typedef struct //0 - 1
{
    float red;
    float green;
    float blue;
} Color;

Color red = {1.0f, 0.0f, 0.0f};  
Color green = {0.0f, 1.0f, 0.0f};  
Color blue = {0.0f, 0.0f, 1.0f}; 

#define WINDOW_W 1280
#define WINDOW_H 720

#define VIEWPORT_WIDTH (WINDOW_W)
#define VIEWPORT_HEIGHT (WINDOW_H)