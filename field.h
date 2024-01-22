#ifndef FIELD_H_INCLUDE
#define FIELD_H_INCLUDE
#include "grid.h"
enum class Terrain
{
    Earth = 0,
    Fire = 1,
    Water = 2,
    Wind_X = 3,
    Wind_Y = 4,
};

class field{
    public:
        field();
        field(int field_width, int field_height);
        bool creat_terrain(int id_x, int id_y, Terrain xy_terrain);
        Terrain get(int id_x, int id_y) const;
    private:
        Grid <Terrain> terrains_map;
        int field_height;
        int field_width;
};

field creatfield(int x, int y, std::string Map_csv);
#endif // FILED_H_INCLUDE

