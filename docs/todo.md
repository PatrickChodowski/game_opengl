

### 1) ./modules/collisions.cpp

- dont really like the implementation of near distances, set_abs etc. splitting between two object types. 

### 2) time for entity generation
- need to create entity and save it through the game and it will be the backbone, instead of quads
- preserve and manage entity table
- clean information about quads and entity - there can be better split


what quad data needs to have to render, looking at the buffer:

1,2,3: position: x,y,z
4,5,6,7: color:  r,g,b,a
8: frame_id
9,10: texture_coordinates
11: texture_id
12: is_clicked
13: quad_type_id
14: is_static

struct 
