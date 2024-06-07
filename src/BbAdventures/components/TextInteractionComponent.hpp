#pragma once
#include <string>
#include <memory>
#include <GoonEngine/content/image.h>
namespace Bba {
struct InteractorComponent {
	std::string Text;
    geImage* TextImage;
};
}  // namespace Bba