#pragma once
#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/entities/Player.hpp>
#include <BbAdventures/tiled/TiledMap.hpp>
#include <functional>
#include <unordered_map>
namespace Bba {
std::unordered_map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap = {
	{"Start", [](TiledMap::TiledObject &object) {
		 return NewPlayer(object);
	 }},
};

}
