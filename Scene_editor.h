#ifndef SCENE_EDITOR_H_INCLUDED
#define SCENE_EDITOR_H_INCLUDED

#include <SFML/Graphics/Text.hpp>
#include <string>
#include <cassert>
#include "Scene_Field.h"
#include "Field.h"

class Scene_editor : public Scene_Field
{
    public:
        Scene_editor(std::string name, std::map <std::string, Texture*> *field_tex_map);
        virtual void update(Event& event, std::string& command_main);
        virtual void update(Time deltaTime);

    private:
        std::string s_input;
        bool if_input;
        void command(std::string data);
        void save_map();

        virtual void draw(RenderTarget& target, RenderStates states) const override;
};

Scene_editor new_editor_scene(Texture* bg, unsigned int length, unsigned int width, std::map <std::string, Texture*> *field_blocks,
                            Texture* player_texture, Vector2i screen_dimensions, int num);
#endif // SCENE_EDITOR_H_INCLUDED
