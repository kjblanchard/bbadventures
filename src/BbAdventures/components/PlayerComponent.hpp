#pragma once
#include <BbAdventures/shared/constants.hpp>
namespace Bba {
struct PlayerComponent {
    Directions Direction;
    unsigned int PlayerNum;
    unsigned int ControllerNum;
};

}  // namespace Bba