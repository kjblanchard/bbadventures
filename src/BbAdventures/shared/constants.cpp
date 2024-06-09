#include <BbAdventures/shared/constants.hpp>
namespace Bba {
#ifdef GN_PLATFORM_MACOS
std::string ASSET_PREFIX = "../Resources/assets";
#else
std::string ASSET_PREFIX = "assets";
#endif
std::string TILED_PREFIX = "tiled";
std::string IMAGES_PREFIX = "images";
}  // namespace Bba
