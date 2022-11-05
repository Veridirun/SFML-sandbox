#include "Field.h"
#include <iostream>
#include <fstream>
#include <string>

#include "json/json.h"

bool isdrawed = false;


Field::Field(int length, int width, std::string name) : background(NULL), name(name)
{
    x_coord = length/2 * 120 + 60;
    y_coord = width/2 * 120 + 60;
    turn_length = 60;
    cell_length = 120;
    //std::cout << x_coord << " " << y_coord << std::endl;
}

void Field::addTexture(Texture* texture, IntRect rect)
{
    background = texture;

    m_vertices[0].position = Vector2f(0.f, 0.f);
    m_vertices[1].position = Vector2f(0.f, static_cast<float>(rect.height));
    m_vertices[2].position = Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
    m_vertices[3].position = Vector2f(static_cast<float>(rect.width), 0.f);

    float left = static_cast<float>(rect.left) + 0.0001f;
    float right = left + static_cast<float>(rect.width);
    float top = static_cast<float>(rect.top);
    float bottom = top + static_cast<float>(rect.height);

    m_vertices[0].texCoords = Vector2f(left, top);
    m_vertices[1].texCoords = Vector2f(left, bottom);
    m_vertices[2].texCoords = Vector2f(right, bottom);
    m_vertices[3].texCoords = Vector2f(right, top);
}

void Field::addCell(Texture* texture, unsigned int x, unsigned int y)
{
    cells[x][y] = new_cell(texture, "new_cell");
}

void Field::addPlayer(unsigned int length, unsigned int width, Texture* player_texture)
{
    player_0 = new Player("default_player", player_texture);
    player_0->x_coord = x_coord;
    player_0->y_coord = y_coord;
    player_0->addTexCoords(IntRect(0, 0, 120, 120));
}

void Field::field_resize(unsigned int length, unsigned int width)         // CHECK
{
    cells.resize(length);
    for (unsigned int i = 0; i < length; i++)
    {
        cells[i].resize(width);
    }
}

/*Cell* Field::get_cell_by_coord(unsigned int x, unsigned int y)
{
    x /= 120;
    y /= 120;
    return cells[x][y];
}*/

bool Field::can_move_player(int direction, int value)
{
    double xz[2], yz[2];
    xz[0] = player_0->x_coord+1;
    xz[1] = player_0->x_coord-1;
    yz[0] = player_0->y_coord+1;
    yz[1] = player_0->y_coord-1;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            double x = xz[i];
            double y = yz[j];
            int x_new = int((x + direction*value*(turn_length+1))/120);
            int y_new = int((y + ((direction+1)%2)*(-value)*(turn_length+1))/120);
            //std::cout << "new coords: " << x_new << " " << y_new << std::endl;

            Cell* cell_new = cells[x_new][y_new];
            if (cell_new->type_name == "border")
            {
                return false;
            }
        }
    }
    return true;
}

void Field::move_player(int direction, int value)
{
    if (!can_move_player(direction, value)) {
        return;
    }
    if (direction == 0)
    {
        if ((y_coord - cell_length - value*turn_length >= 1080/2) && (y_coord - value*turn_length <= (cells[0].size()-1)*cell_length-1080/2) && player_0->y_coord == y_coord)
        {
            y_coord -= value*turn_length;
        }
        if ((player_0->y_coord - value*turn_length >= cell_length*1.5) && (player_0->y_coord - value*turn_length <= (cells[0].size()-1.5)*cell_length))
        {
            player_0->y_coord -= value*turn_length;
        }
        else return;
    }
    else if (direction == 1)
    {
        if ((x_coord - cell_length + value*turn_length >= 1920/2) && (x_coord + value*turn_length <= (cells.size()-1)*cell_length-1920/2) && player_0->x_coord == x_coord)
        {
            x_coord += value*turn_length;
        }
        if ((player_0->x_coord + value*turn_length >= cell_length*1.5) && (player_0->x_coord + value*turn_length <= (cells[0].size()-1.5)*cell_length))
        {
            player_0->x_coord += value*turn_length;
        }
        else return;
    }
}

std::pair <int, int> Field::get_player_cell_coord()
{
    int x = player_0->x_coord-60;
    int y = player_0->y_coord-60;
    x /= 120;
    y /= 120;
    return std::make_pair(x, y);
}

void Field::action(Texture* texture)
{
    //std::cout << player_0->x_coord << " " << player_0->y_coord << std::endl;
    std::pair <int, int> player_cell_coords = get_player_cell_coord();
    int x = player_cell_coords.first;
    int y = player_cell_coords.second;
    if (cells[x][y]->hasObject("tree"))
    {
        cells[x][y]->action_change("tree", texture);
    }
    else if (cells[x][y]->hasObject("stump"))
    {
        cells[x][y]->action_change("stump", texture);
    }
    else if (cells[x][y]->hasObject("portal"))
    {

    }
}

void Field::add_object_to_cell(std::string type_name, int x, int y, Texture* texture)
{
    cells[x][y]->addObject(texture, type_name, 1);
}

void Field::change_cell_texture(std::string name, int x, int y, Texture* texture)
{
    cells[x][y]->change_texture(name, texture);
}

void Field::someTextures(std::map <std::string, Texture*> *field_block, int num)
{
    std::string path = "Locations/loc_";
    path += std::to_string(num);
    path += ".json";
    std::cout << path << std::endl;
    Json::Value Locations;
    std::ifstream ifs(path);
    ifs >> Locations;


    for (unsigned int x = 0; x < cells.size(); x++)
    {
        for (unsigned int y = 0; y < cells[x].size(); y++)
        {
            std::string cell_type = Locations["map"][x][y]["type"].asString();
            cells[x][y] = new_cell((*field_block)[cell_type], cell_type);

            std::string object_type = "";
            if (Locations["big_objects"][x][y].isArray())
            {
                unsigned int cell_object_size = Locations["big_objects"][x][y].size();
                for (unsigned int i = 0; i < cell_object_size; i++)
                {
                    if (Locations["big_objects"][x][y][i].isObject())
                    {
                        std::string object_type = Locations["big_objects"][x][y][0]["type"].asString();
                        std::string object_depth_level = Locations["big_objects"][x][y][0]["depth_level"].asString();
                        cells[x][y]->addObject((*field_block)[object_type], object_type, 1);
                    }
                }
            }
        }
    }
    std::cout << std::endl;
}

void Field::save_field(int num)
{
    std::string path = "Locations/loc_";
    path += std::to_string(num);
    path += ".json";

    Json::Value Location;
    for (unsigned int x = 0; x < cells.size(); x++)
    {
        for (unsigned int y = 0; y < cells[x].size(); y++)
        {
            cells[x][y]->save_cell(x, y, Location);
        }
    }
    std::ofstream ofs(path);
    ofs << Location;
    ofs.close();
}

void Field::draw(RenderTarget& target, RenderStates states) const
{
//    if (isdrawed)
//        return;
//    isdrawed = true;

    //std::cout << "Who asked " << name << " to draw?\n";
    //std::cout << name << " draw pos " << getPosition().x << " " << getPosition().y << std::endl;

    /*if (background)
    {
        states.transform *= getTransform();
        states.texture = background;
        target.draw(m_vertices, 4, Quads, states);
    }*/

    //std::cout << "field draw pre\n";
//    for (unsigned int i = 0; i < 17; i++)
//    {
//        for (unsigned int j = 0; j < 10; j++)
//        {
//            int x = (x_coord-960)/cell_length;
//            int y = (y_coord-540)/cell_length;
//            //std::cout << cells[x+i][y+j]->mapsize() << " ";
//        }
//        //std::cout << std::endl;
//    }

    //std::cout << "field draw begin\n";
    for (unsigned int i = 0; i < 17; i++)
    {
        for (unsigned int j = 0; j < 10; j++)
        {
            int x = (x_coord-960)/cell_length;
            int y = (y_coord-540)/cell_length;
            //std::cout << cells[x+i][y+j]->mapsize() << " ";
            cells[x+i][y+j]->addPosition(x*cell_length-x_coord+960+cell_length*i, y*cell_length-y_coord+540+cell_length*j);
            cells[x+i][y+j]->draw(target, states);
            //std::cout << x*120-x_coord+960+120*i << " " << y*120-y_coord+540+120*j << std::endl;
        }
        //std::cout << std::endl;
    }
    //std::cout << "field draw end\n";
    if (true)
    {
        player_0->setPosition(900-(x_coord-player_0->x_coord), 480-(y_coord-player_0->y_coord));
        //std::cout << player_0->getPosition().x << " " << player_0->getPosition().y << std::endl;
        player_0->draw(target, states);
    }
}

Field* new_field(Texture* bg, unsigned int length, unsigned int width, Texture* cell_texture, Texture* player_texture, Vector2i screen_dimensions)
{
    Field* field = new Field(length, width, "Main field");
    field->addTexture(bg, IntRect(0, 0, 1920, 1080));
    field->setScale((float)screen_dimensions.x / 1920, (float)screen_dimensions.y / 1080);

    field->field_resize(length, width);

    for (unsigned int x = 0; x < length; x++)
    {
        for (unsigned int y = 0; y < width; y++)
        {

            field->addCell(cell_texture, x, y);
        }
    }

    field->addPlayer(length, width, player_texture);

    return field;
}
