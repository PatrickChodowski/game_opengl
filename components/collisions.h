
#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "../dictionary.h"

namespace colls
{
    // Distance Between Points is definately too long of a name
    struct DistanceBetweenPoints
    {
      int a_quad_id;
      int b_quad_id;
      int a_quad_type;
      int b_quad_type;
      float distance;
      float limit; // sum of quants diagonals
      bool is_near;
    };

    float get_distance_between_points(float x1, float y1, float x2, float y2)
    {
      // std::cout << "Calculate distance between (" << x1 << "," << y1 <<") and (" << x2 << "," << y2 << ")" << std::endl; 
      float distance = std::sqrt(std::pow(x2-x1, 2) + std::pow(y2-y1, 2));
      // std::cout << "Distance is: " << distance << std::endl;
      return distance;
    }

    float get_distance_between_quads(quads::Quad q1, quads::Quad q2)
    {
      float distance = std::sqrt(std::pow(q2.s_x - q1.s_x, 2) + std::pow(q2.s_y-q1.s_y, 2));
      return distance;
    }

    std::vector<colls::DistanceBetweenPoints> get_entity_distances(int entity_quad_id)
    {
      // calculate distances between alive entity and quads of certain type

      // find quad id entity index
      int entity_index = quads::find_quad_id(entity_quad_id, ent::EntityQuads);

      // get entity quad data
      quads::Quad entity_quad = ent::EntityQuads[entity_index];

      // initialize instance of distances table
      std::vector<colls::DistanceBetweenPoints> distances = {};
      if (quads::AllQuads.size() > 0)
      {
        for(int q = 0; q < quads::AllQuads.size(); q++)
        {
          if((entity_quad_id != quads::AllQuads[q].id) && (quads::AllQuads[q].coll))
          {
            float dist = get_distance_between_quads(entity_quad, quads::AllQuads[q]);
            DistanceBetweenPoints dbp;
            dbp.a_quad_id = entity_quad_id;
            dbp.b_quad_id = quads::AllQuads[q].id;
            dbp.a_quad_type = entity_quad.entity_type_id;
            dbp.b_quad_type = quads::AllQuads[q].entity_type_id;

            dbp.distance = dist;
            dbp.limit = entity_quad.s_diag + quads::AllQuads[q].s_diag;
            dbp.is_near = false;

            if(dbp.distance <= dbp.limit){
              dbp.is_near = true;
            }
            distances.push_back(dbp);
          }
        }
      }
      return distances;
    }

    std::vector<colls::DistanceBetweenPoints>  find_hero_broad_collisions()
    {
      // function to find broad collisions for hero -> agents should just use navmesh
      // where is navmesh? ha! not implemented yet!
      int entity_quad_id = hero::get_hero_quad_id();
      // std::cout << entity_quad_id << std::endl;
      std::vector<colls::DistanceBetweenPoints> distances;
      if (entity_quad_id != -1)
      {
        distances = get_entity_distances(entity_quad_id);
        int near_count = 0;
        for(int d=0; d<distances.size(); d++)
        {
          if(distances[d].is_near)
          {
            near_count += 1;
            std::cout <<"entity: " << distances[d].a_quad_id  << " type: " <<  distances[d].a_quad_type <<  " Near entity: " << distances[d].b_quad_id 
            << " type: " << distances[d].b_quad_type  << " distance: " << distances[d].distance << " limit: " << distances[d].limit << std::endl;
          }
        }
        if(near_count > 0){
          std::cout << "Near entity count: " << near_count << std::endl;
        }
      }
      return distances;
    }
}

#endif