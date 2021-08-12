
#ifndef COLLISIONS_H
#define COLLISIONS_H

namespace colls
{

    float get_distance_between_points(float x1, float y1, float x2, float y2)
    {
      // std::cout << "Calculate distance between (" << x1 << "," << y1 <<") and (" << x2 << "," << y2 << ")" << std::endl; 
      float distance = std::sqrt(std::pow(x2-x1, 2) + std::pow(y2-y1, 2));
      // std::cout << "Distance is: " << distance << std::endl;
      return distance;
    }

    void get_hero_distances(float hero_x, float hero_y, std::vector<quads::Quad>)
    {
      // calculate distances between hero and solid map objects/entities
      // 
        
    }

    void find_hero_broad_collisions()
    {
      // For walls and map objects its being run only if the position of player (camera moved) changed
      // if it didnt change, will be only calculated against alive entities

    }




}

#endif