#include "field.h"
#include "grid.h"

//nanyuan chnage
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
//end
field::field()
{}

field::field(int field_width, int field_height):field_height(field_height), field_width(field_width)
{
    terrains_map = Grid<Terrain>(field_width, field_height, Terrain::Earth);
}

bool field::creat_terrain(int id_x, int id_y, Terrain xy_terrain)
{
    if (this->terrains_map.get(id_x, id_y) != Terrain::Earth){
        return false;
    }
    else{
        if (xy_terrain == Terrain::Wind_X){
            if (this->terrains_map.get(field_width - 1 - id_x, id_y) == Terrain::Earth){
                this->terrains_map.set(id_x, id_y, xy_terrain);
                this->terrains_map.set(field_width - 1 - id_x, id_y, xy_terrain);
                return true;
            }
            else{
                return false;
            }
        }
        else if (xy_terrain == Terrain::Wind_Y){
            if (this->terrains_map.get(id_x, id_y) == Terrain::Earth){
                this->terrains_map.set(id_x, id_y, xy_terrain);
                this->terrains_map.set(id_x, field_height - 1 - id_y, xy_terrain);
                return true;
            }
            else{
                return false;
            }
        }
        else{
            this->terrains_map.set(id_x, id_y, xy_terrain);
            return true;
        }
    }

}

Terrain field::get(int id_x, int id_y) const
{

    return this->terrains_map.get(id_x, id_y);
}

field creatfield(int x, int y, std::string Map_csv)
{
    field result_map = field(x, y);
    std::ifstream inFile(Map_csv, std::ios::in);
    if (!inFile)
    {
        std::cout << "打开文件失败！" << std::endl;
        exit(1);
    }
    int i = 0, j = 0;
    std::string line;
    std::string field;
    while (getline(inFile, line))
    {
        std::string field;
        std::istringstream sin(line);
        getline(sin, field, ',');
        while(getline(sin, field, ','))
        {
            if (field == "~")
                result_map.creat_terrain(i, j, Terrain::Water);
            if (field == "Y")
                result_map.creat_terrain(i, j, Terrain::Wind_Y);
            if (field == "X")
                result_map.creat_terrain(i, j, Terrain::Wind_X);
            if (field == "|")
                result_map.creat_terrain(i, j, Terrain::Fire);
            i++;
        }
        i = 0;
        j++;
    }
    inFile.close();
    return result_map;
}
