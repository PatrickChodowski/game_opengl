
#ifndef COLLISIONS_H
#define COLLISIONS_H

#define QUAD_TYPE_MAP 0.0f
#define QUAD_TYPE_MENU 1.0f
#define QUAD_TYPE_TEXT 2.0f
#define QUAD_TYPE_ENTITY 3.0f


// aaarghhhh it needs MVP camera input noooo


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

    float get_distance_between_points_scaled(float x1, float y1, float x2, float y2, int type_1, int type_2)
    {
      // Something tell me it wont work long term, maybe I shall just use ScaledQuads for finding distance?
      float scale_factor = (1.0f/float(camera::zoom));
      float x1_s;
      float x2_s;
      float y1_s;
      float y2_s;
      if(type_1 == QUAD_TYPE_MAP){
        x1_s = ((float)x1 + (float)(-camera::x))*scale_factor;
        y1_s = ((float)y1 + (float)camera::y)*scale_factor;
      } else {
        x1_s = (float)x1*scale_factor;
        y1_s = (float)y1*scale_factor;
      }
      if(type_2 == QUAD_TYPE_MAP){
        x2_s = ((float)x2 + (float)(-camera::x))*scale_factor;
        y2_s = ((float)y2 + (float)camera::y)*scale_factor;
      } else {
        x2_s = (float)x1*scale_factor;
        y2_s = (float)y1*scale_factor;
      }

      std::cout << "Calculate distance between (" << x1_s << "," << y1_s <<") and (" << x2_s << "," << y2_s << ")" << std::endl;
      float distance = std::sqrt(std::pow(x2_s-x1_s, 2) + std::pow(y2_s-y1_s, 2));
      std::cout << "Distance is: " << distance << std::endl;
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
      std::vector<colls::DistanceBetweenPoints> distances = {};
      if (quads.size() > 0)
      {
        for(int q = 0; q < quads.size(); q++)
        {
          if(entity_quad_id != quads[q].id){
            // float dist = get_distance_between_points(entity_quad.x, entity_quad.y, quads[q].x, quads[q].y);

            float dist = get_distance_between_points_scaled(entity_quad.x, 
                                                            entity_quad.y, 
                                                            quads[q].x, 
                                                            quads[q].y, 
                                                            entity_quad.type_id,
                                                            quads[q].type_id);
            DistanceBetweenPoints dbp;
            dbp.a_quad_id = entity_quad_id;
            dbp.b_quad_id = quads[q].id;
            dbp.distance = dist;
            distances.push_back(dbp);
          }
        }
      }
      return distances;
    }

    std::vector<colls::DistanceBetweenPoints>  find_entity_broad_collisions(int entity_quad_id, bool moved = true)
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
        // std::cout << " map distance size:" << map_distances.size() << std::endl;
        if(map_distances.size() > 0){
          distances.insert(distances.end(), map_distances.begin(), map_distances.end());
        }
      }

      std::vector<colls::DistanceBetweenPoints> ent_distances = get_entity_distances(entity_quad_id, ent::EntityQuads);
      if(ent_distances.size() > 0){
        distances.insert(distances.end(), ent_distances.begin(), ent_distances.end());
      }

      return distances;
    }

    void find_broad_collisions()
    {
      // running logic for all alive entities
      std::vector<colls::DistanceBetweenPoints> all_distances;
      for(int e=0; e < ent::EntityQuads.size(); e++)
      {
        if(ent::EntityQuads[e].alive)
        {
          std::vector<colls::DistanceBetweenPoints> ent_distances = find_entity_broad_collisions(ent::EntityQuads[e].id);
          if(ent_distances.size() > 0){
            all_distances.insert(all_distances.end(), ent_distances.begin(), ent_distances.end());
          }
        }
      }

      // printing colllisions
      for(int c=0; c<all_distances.size(); c++)
      {
        colls::DistanceBetweenPoints dbp = all_distances[c];
        std::cout << "Main quad id: " << dbp.a_quad_id << " other quad: " << dbp.b_quad_id << " distance:" << dbp.distance << std::endl;

      }
    }

}

#endif