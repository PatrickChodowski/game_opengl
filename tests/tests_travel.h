
#include "../modules/ecs.h" 
#include "../modules/travel.h"

#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

bool test_travel__get_angle()
{
    bool passed = true;

    float angle1 = travel::_get_angle_between_points(0, 0, 1, 1);
    int degrees1 = angle1*180/3.14;

    float angle2 = travel::_get_angle_between_points(0, 0, 0, 2);
    int degrees2 = angle2*180/3.14;

    float angle3 = travel::_get_angle_between_points(0, 0, -1, 1);
    int degrees3 = angle3*180/3.14;

    if(degrees1 != 45){
        std::cout << " [ERROR] Degrees 1 should be 45 instead of " << degrees1 << std::endl;
        return false;
    }

    if(degrees2 != 90){
        std::cout << " [ERROR] Degrees 2 should be 90 instead of " << degrees2 << std::endl;
        return false;
    }

    if(degrees3 != 135){
        std::cout << " [ERROR] Degrees 3 should be 135 instead of " << degrees3 << std::endl;
        return false;
    }

    return passed;
}