#ifndef ANIMATEDSPRITE_INCLUDE
#define ANIMATEDSPRITE_INCLUDE

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

#include <iostream>
#include <string>

#include "Animation.h"

using namespace sf;

class AnimatedSprite : public Drawable, public Transformable
{
public:
    std::string name; ///TEMP

    explicit AnimatedSprite(std::string name="sprite", Time frameTime = seconds(0.2f), bool paused = false, bool looped = true);

    virtual void update(Time deltaTime);
    void setAnimation(const Animation& animation);
    void setFrameTime(Time time);
    void play();
    void play(const Animation& animation);
    void pause();
    void stop();
    void setLooped(bool looped);
    void setColor(const Color& color);
    const Animation* getAnimation() const;
    FloatRect getLocalBounds() const;
    FloatRect getGlobalBounds() const;
    bool isLooped() const;
    bool isPlaying() const;
    Time getFrameTime() const;
    void setFrame(std::size_t newFrame, bool resetTime = true);
    void redraw(RenderTarget& target, RenderStates states) const;

protected:
    Time m_currentTime;
    bool m_isPaused;

private:
    const Animation* m_animation;
    Time m_frameTime;
    std::size_t m_currentFrame;
    bool m_isLooped;
    const Texture* m_texture;
    Vertex m_vertices[4];

    virtual void draw(RenderTarget& target, RenderStates states) const;

};

#endif // ANIMATEDSPRITE_INCLUDE
