#include <vector>
#include "collisions.h"

namespace collisions
{
  int SENSOR_COUNT = 9;
  int SENSOR_OFFSET = 1;
  int ABS_COUNT = 1;

// TODO
 std::vector<collisions::DistanceBetweenPoints> get_entity_distances(int entity_quad_id)
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


}