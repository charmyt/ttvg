#include <fstream>

#include <boost/filesystem.hpp>

#include <fmt/core.h>

#include "ResourceManager.h"
#include "Background.h"

namespace nl = nlohmann;

namespace tt
{

Background::Background(std::string_view name, ResourceManager& resmgr, const sf::Vector2f& tilesize)
    : _tilesize { tilesize },
      _mapname{ name }
{
    const std::string texturename = fmt::format("maps/{}.png", _mapname);
    _texture = resmgr.loadUniquePtr<sf::Texture>(texturename);
    if (_texture) setTexture(*_texture);

    const std::string jsonfile = 
        resmgr.getFilename(fmt::format("maps/{}.json", _mapname));

    std::ifstream file(jsonfile.c_str());
    if (file.is_open())
    {
        _json = std::make_unique<nl::json>();
        file >> *_json;
        file.close();
    }

    initZones();
}

void Background::initZones()
{
    if (!_json) return;
    if (!_json->at("zones").is_array()) return;

    for (const auto& item : (*_json)["zones"].items())
    {
        for (const auto& c : item.value()["rects"].items())
        {
            std::string temp{ c.value().get<std::string>() };
            auto start = temp.begin();
            auto stop = temp.end();

            sf::FloatRect rect;
            bool result =
                phrase_parse(start, stop, FloatRectParser, x3::ascii::space, rect);

            if (result)
            {
                _zones.emplace_back(item.value()["name"].get<std::string>(), rect);
            }
        }
    }
}

sf::FloatRect Background::getWorldTileRect() const
{
    sf::Vector2f bound{ getGlobalBounds().width, getGlobalBounds().height };
    auto [width, height] = getTileFromGlobal(bound);
    return { 0, 0, width, height };
}

std::string Background::zoneName(const sf::Vector2f& v)
{
    for (const auto& [name, rect] : _zones)
    {
        if (RectContains(rect, v))
        {
            return name;
        }
    }

    return std::string();
}

} // namespace tt