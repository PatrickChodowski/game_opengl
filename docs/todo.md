

### 1) ./modules/collisions.cpp

- dont really like the implementation of near distances, set_abs etc. splitting between two object types. 

### 2) time for entity generation
- need to create entity and save it through the game and it will be the backbone, instead of quads
- preserve and manage entity table
- clean information about quads and entity - there can be better split

### 3) I hate everything about handling game state -> vector and map of game states etc. - nightmares

what quad data needs to have to render, looking at the buffer:
