#include "Scene_Field.h"

Scene_Field::Scene_Field(std::string name, std::map <std::string, Texture*> *field_blocks) : Scene::Scene(name), field_tex_map(field_blocks)
{
    current_field = -1;

    for (int i = 0; i < field_N; ++i)
    {
        field[i] = nullptr;
    }
}

void Scene_Field::add_field(Field* field_to_add, int num)
{
    field[num] = field_to_add;
    if (current_field == -1)
    {
        current_field = num;
    }
}

void Scene_Field::add_Field(Texture* bg, unsigned int length, unsigned int width, std::map <std::string, Texture*> *field_blocks,
                            Texture* player_texture, Vector2i screen_dimensions, int num)
{
    Field* field_0 = new_field(bg, length, width, (*field_blocks)["null"], player_texture, screen_dimensions);
    field[num] = field_0;
    if (current_field == -1)
    {
        current_field = num;
    }
}

void Scene_Field::change_current_field(int num)
{
    current_field = num;

    if (field[num] == nullptr) {
        cout << "FATAL ERROR: Trying to load null field\n";
        throw;
    }

    field[num]->load_field(*field_tex_map, num);
}

void Scene_Field::update(Event& event, std::string& command_main)
{
    //std::cout << "Scene_Field update event\n";
    if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
        {
        case sf::Keyboard::W:
            field[current_field]->set_player_movement_direction(3);
            //field[current_field]->move_player(3);
            break;
        case sf::Keyboard::D:
            field[current_field]->set_player_movement_direction(0);
            //field[current_field]->move_player(0);
            break;
        case sf::Keyboard::S:
            field[current_field]->set_player_movement_direction(1);
            //field[current_field]->move_player(1);
            break;
        case sf::Keyboard::A:
            field[current_field]->set_player_movement_direction(2);
            //field[current_field]->move_player(2);
            break;
        case sf::Keyboard::Space:
            field[current_field]->action((*field_tex_map)["stump"]);
            break;
        case sf::Keyboard::Tab:
            change_current_field((current_field+1)%2);
            break;
        default:
            break;
        }
    }
    if (event.type == sf::Event::KeyReleased)
    {
        switch (event.key.code)
        {
        case sf::Keyboard::W:
            field[current_field]->release_player_movement_direction(3);
            break;
        case sf::Keyboard::D:
            field[current_field]->release_player_movement_direction(0);
            break;
        case sf::Keyboard::S:
            field[current_field]->release_player_movement_direction(1);
            break;
        case sf::Keyboard::A:
            field[current_field]->release_player_movement_direction(2);
            break;
        default:
            break;
        }
    }
    /*if (event.type == Event::MouseButtonReleased) // ?
    {
        switch (event.mouseButton.button)
        {
        case Mouse::Left:
            break;

        default:
            break;
        }
    }*/
}

void Scene_Field::set_player_movement_direction(int direction)
{
    field[current_field]->set_player_movement_direction(direction);
}

void Scene_Field::release_player_movement_direction(int direction)
{
    field[current_field]->release_player_movement_direction(direction);
}

void Scene_Field::update(Time deltaTime)
{
    field[current_field]->update(deltaTime);
}

void Scene_Field::load_field(int num, std::string who_call)
{
    //std::cout << "who_call: " << who_call << std::endl;

    field[num]->load_field(*field_tex_map, num);
}

void Scene_Field::draw(RenderTarget& target, RenderStates states) const
{
    /*if (background)
    {
        states.transform *= getTransform();
        states.texture = background;
        target.draw(m_vertices, 4, Quads, states);
    }*/
    if (current_field != -1)
    {
        field[current_field]->draw(target, states);
    }
    draw_scene_Interface(target, states);
}

Scene_Field new_field_scene(Texture* bg, unsigned int length, unsigned int width, std::map <std::string, Texture*> *field_blocks,
                            Texture* player_texture, Vector2i screen_dimensions, int num)
{
    Scene_Field field_scene(std::string("field_scene"), field_blocks); // FIX (pointer)
    field_scene.add_Field(bg, length, width, field_blocks, player_texture, screen_dimensions, num);
    return field_scene;
}
