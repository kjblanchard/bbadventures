#pragma once
#include <string>
#include <memory>
#include <GoonEngine/content/text.h>
namespace Bba {
struct TextInteractionComponent {
	std::string Text;
    geText* TextImage;
};
}  // namespace Bba