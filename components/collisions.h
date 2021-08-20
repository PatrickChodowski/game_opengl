
#ifndef COLLISIONS_H
#define COLLISIONS_H


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
      float distance = std::sqrt(std::pow((q2.s_x - q1.s_x), 2) + std::pow((q2.s_y-q1.s_y), 2));
      return distance;
    }

    std::vector<colls::DistanceBetweenPoints> get_entity_distances(int entity_quad_id)
    {
      // calculate distances between alive entity and quads of certain type

      // find quad id entity index
      int entity_index = quads::find_quad_id(entity_quad_id,  quads::AllQuads);

      // get entity quad data
      quads::Quad entity_quad = quads::AllQuads[entity_index];

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
              quads::AllQuads[q].is_clicked = 1.0f;
              quads::AllQuads[q].v_a.is_clicked = 1.0f;
              quads::AllQuads[q].v_b.is_clicked = 1.0f;
              quads::AllQuads[q].v_c.is_clicked = 1.0f;
              quads::AllQuads[q].v_d.is_clicked = 1.0f;
            } else {
              quads::AllQuads[q].is_clicked = 0.0f;
              quads::AllQuads[q].v_a.is_clicked = 0.0f;
              quads::AllQuads[q].v_b.is_clicked = 0.0f;
              quads::AllQuads[q].v_c.is_clicked = 0.0f;
              quads::AllQuads[q].v_d.is_clicked = 0.0f;
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
      std::vector<colls::DistanceBetweenPoints> near_distances;
      if (entity_quad_id != -1)
      {
        distances = get_entity_distances(entity_quad_id);
        for(int d=0; d<distances.size(); d++)
        {
          if(distances[d].is_near)
          {
            near_distances.push_back(distances[d]);
          }
        }
      }
      return near_distances;
    }

    void handle_collisions(std::vector<colls::DistanceBetweenPoints> near_distances)
    {
      // handle near distances
      // easiest example:
      // solid map tile wont let the hero through
      if(near_distances.size() > 0)
      {
        hero::set_hero_sensors();
        quads::set_aabb();
      }
    }
}

#endif