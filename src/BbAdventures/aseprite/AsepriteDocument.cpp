#include <BbAdventures/aseprite/AsepriteDocument.hpp>
#include <GoonEngine/debug.h>
#include <GoonEngine/utils.h>
#include <fstream>
#include <istream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace Bba;
// #ifdef GN_PLATFORM_MACOS
// static const char *_asepritePrefix = "../Resources/assets/aseprite/";
// #else
static const char *_asepritePrefix = "assets/aseprite/";
// #endif
AsepriteDocument::AsepriteDocument(std::string p) {
	auto fullPathNoType = _asepritePrefix + p;
	auto fullPathJson = fullPathNoType + ".json";
	char buf[1000];
	geGetLoadFilename(buf, sizeof(buf), fullPathJson.c_str());

	std::ifstream fileStream(buf);
	auto j = json::parse(fileStream);
	for (const auto &frameJson : j["frames"]) {
		Frame frame;
		frame.filename = frameJson["filename"];
		frame.frame = {
			frameJson["frame"]["x"],
			frameJson["frame"]["y"],
			frameJson["frame"]["w"],
			frameJson["frame"]["h"]};
		frame.rotated = frameJson["rotated"];
		frame.trimmed = frameJson["trimmed"];
		frame.spriteSourceSize = {
			frameJson["spriteSourceSize"]["x"],
			frameJson["spriteSourceSize"]["y"],
			frameJson["spriteSourceSize"]["w"],
			frameJson["spriteSourceSize"]["h"]};
		frame.sourceSize = {
			frameJson["sourceSize"]["w"],
			frameJson["sourceSize"]["h"]};
		frame.duration = frameJson["duration"];
		frames.push_back(frame);
	}
	meta.app = j["meta"]["app"];
	meta.version = j["meta"]["version"];
	meta.image = j["meta"]["image"];
	meta.format = j["meta"]["format"];
	meta.scale = j["meta"]["scale"];
	meta.size = {
		j["meta"]["size"]["w"],
		j["meta"]["size"]["h"]};
	for (const auto &tagJson : j["meta"]["frameTags"]) {
		FrameTag tag;
		tag.name = tagJson["name"];
		tag.direction = tagJson["direction"];
		tag.color = tagJson["color"];
		tag.from = tagJson["from"];
		tag.to = tagJson["to"];
		meta.frameTags.push_back(tag);
	}
	for (const auto &layerJson : j["meta"]["layers"]) {
		Layer layer;
		layer.name = layerJson["name"];
		layer.opacity = layerJson["opacity"];
		layer.blendMode = layerJson["blendMode"];
		meta.layers.push_back(layer);
	}
}