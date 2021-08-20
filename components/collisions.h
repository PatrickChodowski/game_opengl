
#ifndef COLLISIONS_H
#define COLLISIONS_H


namespace colls
{
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

    void resolve_solid_collisions(std::vector<colls::DistanceBetweenPoints> near_distances)
    {
      struct SolidLimits limits;
      int hero_quad_id = hero::get_hero_quad_id();
      int hid = quads::find_quad_id(hero_quad_id, quads::AllQuads);

      for(int i=0; i<near_distances.size(); i++)
      {
        int qid = quads::find_quad_id(near_distances[i].b_quad_id, quads::AllQuads);
        if(quads::AllQuads[qid].solid)
        { 
          // loop through sensors
          for (auto const& s : quads::AllQuads[hid].sensors)
          {
            // hero entity on the left
            if((s.first == SENSOR_TOP_RIGHT || s.first == SENSOR_RIGHT || s.first == SENSOR_BOTTOM_RIGHT) && 
              (s.second.x >=  quads::AllQuads[qid].abs[AABB_FULL].min_x) && 
              (s.second.y >=  quads::AllQuads[qid].abs[AABB_FULL].min_y && s.second.y <= quads::AllQuads[qid].abs[AABB_FULL].max_y) &&
              (quads::AllQuads[hid].sensors[SENSOR_CENTER].x < quads::AllQuads[qid].abs[AABB_FULL].min_x)
            )
            {
               limits.right_borders.push_back(quads::AllQuads[qid].abs[AABB_FULL].min_x);
            }

            // hero entity on the right
            if((s.first == SENSOR_TOP_LEFT || s.first == SENSOR_LEFT || s.first == SENSOR_BOTTOM_LEFT) && 
              (s.second.x <=  quads::AllQuads[qid].abs[AABB_FULL].max_x) && 
              (s.second.y >=  quads::AllQuads[qid].abs[AABB_FULL].min_y && s.second.y <= quads::AllQuads[qid].abs[AABB_FULL].max_y) &&
              (quads::AllQuads[hid].sensors[SENSOR_CENTER].x > quads::AllQuads[qid].abs[AABB_FULL].max_x)
            )
            {
               limits.left_borders.push_back(quads::AllQuads[qid].abs[AABB_FULL].max_x);
            }

            // hero entity on the top
            if((s.first == SENSOR_BOTTOM_RIGHT || s.first == SENSOR_BOTTOM || s.first == SENSOR_BOTTOM_LEFT) && 
              (s.second.y >=  quads::AllQuads[qid].abs[AABB_FULL].min_y) && 
              (s.second.x >=  quads::AllQuads[qid].abs[AABB_FULL].min_x && s.second.x <= quads::AllQuads[qid].abs[AABB_FULL].max_x) &&
              (quads::AllQuads[hid].sensors[SENSOR_CENTER].y < quads::AllQuads[qid].abs[AABB_FULL].max_y)
            )
            {
               limits.bottom_borders.push_back(quads::AllQuads[qid].abs[AABB_FULL].min_y);
            }

            // hero entity on the bottom
            if((s.first == SENSOR_TOP_LEFT || s.first == SENSOR_TOP || s.first == SENSOR_TOP_RIGHT) && 
              (s.second.y <=  quads::AllQuads[qid].abs[AABB_FULL].max_y) && 
              (s.second.x >=  quads::AllQuads[qid].abs[AABB_FULL].min_x && s.second.x <= quads::AllQuads[qid].abs[AABB_FULL].max_x) &&
              (quads::AllQuads[hid].sensors[SENSOR_CENTER].y > quads::AllQuads[qid].abs[AABB_FULL].min_y)
            )
            {
               limits.top_borders.push_back(quads::AllQuads[qid].abs[AABB_FULL].max_y);
            }
          }
        }
      }

      // hero on the left     x |_|
      if (limits.right_borders.size() > 0)
      { 
        float min_x_border = *std::min_element(limits.right_borders.begin(), limits.right_borders.end());
      } 
      // hero on the right   |_| x 
      if (limits.left_borders.size() > 0)
      { 
        float max_x_border = *std::max_element(limits.left_borders.begin(), limits.left_borders.end());
      } 
      // hero on the top 
      if (limits.bottom_borders.size() > 0)
      { 
        float min_y_border = *std::min_element(limits.bottom_borders.begin(), limits.bottom_borders.end());
      } 
      // hero on the bottom 
      if (limits.top_borders.size() > 0)
      { 
        float max_y_border = *std::min_element(limits.top_borders.begin(), limits.top_borders.end());
      } 
    }

    
    void handle_collisions(std::vector<colls::DistanceBetweenPoints> near_distances)
    {
      // handle near distances
      // easiest example:
      // solid map tile wont let the hero through
      if(near_distances.size() > 0)
      {
        hero::set_hero_sensors();
        quads::set_abs(near_distances);
        resolve_solid_collisions(near_distances);

        // move camera back based on the solid limits


      }
    }
}

#endif