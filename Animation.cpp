#include "Animation.h"

Animation::Animation() : m_texture(NULL)
{

}

void Animation::addFrame(IntRect rect)
{
    m_frames.push_back(rect);
}

void Animation::setSpriteSheet(Texture& texture)
{
    m_texture = &texture;
}

Texture* Animation::getSpriteSheet()
{
    return m_texture;
}

std::size_t Animation::getSize() const
{
    return m_frames.size();
}

const IntRect& Animation::getFrame(std::size_t n) const
{
    return m_frames[n];
}
