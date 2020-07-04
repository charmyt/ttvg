#pragma once
#include <cmath>
#include <set>

#include <SFML/Graphics.hpp>

namespace tt
{

struct zone_compare
{

bool operator()(const sf::FloatRect& lhs, const sf::FloatRect& rhs) const
{
    return lhs.left < rhs.left;
}

};

using ZoneSet = std::set<sf::FloatRect, zone_compare>;

class ResourceManager;

template<typename V>
sf::Vector2f getTileFromGlobal(const V& position, const sf::Vector2i& tileSize)
{
    auto x = static_cast<float>(std::floor(position.x / tileSize.x));
    auto y = static_cast<float>(std::floor(position.y / tileSize.y));
    return { x, y };
}

template<typename Int>
sf::Vector2f getTileFromGlobal(Int posx, Int posy, const sf::Vector2i& tileSize)
{
    return getTileFromGlobal(sf::Vector2<Int>{ posx, posy }, tileSize);
}

template<typename V>
sf::Vector2f getGlobalFromTile(const V& tilepos, const sf::Vector2i& tileSize)
{
    auto x = static_cast<float>(tilepos.x * tileSize.x);
    auto y = static_cast<float>(tilepos.y * tileSize.y);
    return { x, y };
}

template<typename Int>
sf::Vector2f getGlobalFromTile(Int posx, Int posy, const sf::Vector2i& tileSize)
{
    return getGlobalFromTile(sf::Vector2<Int>{ posx, posy }, tileSize);
}

class Background;
using BackgroundPtr = std::unique_ptr<Background>;
using BackgroundSharedPtr = std::shared_ptr<Background>;

class Background : public sf::Sprite
{

public:
    using Zone = std::tuple<std::string, sf::FloatRect>;

    Background(std::string_view name, ResourceManager& resmgr, const sf::Vector2i& tilesize);

    sf::FloatRect getWorldRect() const;

    float getLeftBoundary() const { return 0; }
    float getRightBoundary() const;
    float getTopBoundary() const { return 0; }
    float getBottomBoundary() const;

    void setTileSize(const sf::Vector2i& val) { _tilesize = val; }
    sf::Vector2i tilesize() const { return _tilesize; }

    // TODO: there are some interesting optimizations that
    // could be done here, which might be interesting to do
    // but there are other things I want to do right now
    std::string zoneName(const sf::Vector2f& v);

protected:

    std::unique_ptr<sf::Texture>    _texture;
    std::vector<Zone>               _zones;

private:
    sf::Vector2i    _tilesize;

};

} // namespace tt