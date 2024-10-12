#define MAP_W 8
#define MAP_H 8
#define MAP_SIZE (MAP_W * MAP_H)

int map[MAP_SIZE] =
{
    1,1,1,1,1,1,1,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,1,0,0,0,1,
    1,0,0,0,0,1,0,1,
    1,1,1,1,1,1,1,1
};

typedef struct
{
    float x;
    float y;
} Angle;

typedef struct
{
    float x;
    float y;
} Position;

Position P_EMPTY = {-1, -1};
