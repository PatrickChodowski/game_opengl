
#ifndef COLLISIONS_H
#define COLLISIONS_H

namespace colls
{
    // Distance Between Points is definately too long of a name
    struct DistanceBetweenPoints
    {
      int a_quad_id;
      int b_quad_id;
      float distance;
    };

    struct EntityDistanceTable
    {
      int entity_quad_id;
      DistanceBetweenPoints dbp;
    };

    float get_distance_between_points(float x1, float y1, float x2, float y2)
    {
      // std::cout << "Calculate distance between (" << x1 << "," << y1 <<") and (" << x2 << "," << y2 << ")" << std::endl; 
      float distance = std::sqrt(std::pow(x2-x1, 2) + std::pow(y2-y1, 2));
      // std::cout << "Distance is: " << distance << std::endl;
      return distance;
    }

    std::vector<colls::DistanceBetweenPoints> get_entity_distances(int entity_quad_id, std::vector<quads::Quad> quads)
    {
      // calculate distances between alive entity and solid map objects or other entities (basically other quads)

      // find quad id entity index
      int entity_index = quads::find_quad_id(entity_quad_id, ent::EntityQuads);

      // get entity quad data
      quads::Quad entity_quad = ent::EntityQuads[entity_index];

      // initialize instance of distances table
      std::vector<colls::DistanceBetweenPoints> distances;
      if (quads.size() > 0)
      {
        for(int q = 0; q < quads.size(); q++)
        {
          if(entity_quad_id != quads[q].id){
            float dist = get_distance_between_points(entity_quad.x, entity_quad.y, quads[q].x, quads[q].y);
            DistanceBetweenPoints dbp;
            dbp.a_quad_id = entity_quad_id;
            dbp.b_quad_id = quads[q].id;
            dbp.distance = dist;
          }
        }
      }
      return distances;
    }

    void find_entity_broad_collisions(int entity_quad_id, bool moved = true)
    {
      // function to find broad collisions for each (alive(moving)) entity. 
      // Should work for both hero and other alive(moving) entity

      // For walls and map objects its being run only if the position of player (camera moved) changed
      // if it didnt change, will be only calculated against alive entities
      // temporaily default is true
      // 

      // int hero_quad_id = hero::get_hero_quad_id();

      std::vector<colls::DistanceBetweenPoints> distances;

      if(moved){
        // if there was move, calculate distances between hero and solid map objects
        std::vector<colls::DistanceBetweenPoints> map_distances = get_entity_distances(entity_quad_id, maps::SolidMapQuads);
        if(map_distances.size() > 0){
          distances.insert(distances.end(), map_distances.begin(), map_distances.end());
        }
      }

      std::vector<colls::DistanceBetweenPoints> ent_distances = get_entity_distances(entity_quad_id, ent::EntityQuads);
      if(ent_distances.size() > 0){
        distances.insert(distances.end(), ent_distances.begin(), ent_distances.end());
      }

    }

    void find_broad_collisions()
    {



    }




}

#endif