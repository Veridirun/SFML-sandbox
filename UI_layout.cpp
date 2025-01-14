#include "UI_layout.h"

UI_layout::UI_layout()
{
    //ctor
}

void UI_layout::addElement(UI_element* new_element)
{
    elements.push_back(new_element);
}

bool UI_layout::contains(sf::Vector2f curPos)
{
    bool ans = false;
    for (auto g : elements)
    {
        bool tek = g->contains(curPos);
        //if (g->type_name == "main_menu") {ans = true; break;}
        if (tek) {ans = true; break;}
    }
    //std::cout << ans << std::endl;
    return ans;
}

bool UI_layout::get_isClicked()
{
    return isClicked;
}

void UI_layout::push_click(sf::Vector2f curPos)
{
    for (int i = elements.size()-1; i >= 0; i--)
    {
        if (elements[i]->contains(curPos) && elements[i]->isClickable)
        {
            elements[i]->push();
            isClicked = true;
            last_clicked_index = i;
            break;
        }
    }
}

std::string UI_layout::release_click()
{
    isClicked = false;
    return elements[last_clicked_index]->release();
}

int UI_layout::get_elements_size()
{
    return elements.size();
}

void UI_layout::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto g : elements)
    {
        //std::cout << elements.size() << std::endl;
        target.draw(*g);
    }
}
