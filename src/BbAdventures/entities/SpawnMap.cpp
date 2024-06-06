#pragma once
#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/entities/PlayerStart.hpp>
#include <BbAdventures/entities/PlayerExit.hpp>
#include <BbAdventures/tiled/TiledMap.hpp>
#include <functional>
#include <unordered_map>
namespace Bba {
std::unordered_map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap = {
	{"Start", [](TiledMap::TiledObject &object) {
		 return NewPlayerSpawn(object);
	 }},
	{"Exit", [](TiledMap::TiledObject &object) {
		 return NewPlayerExit(object);
	 }}
};

}
