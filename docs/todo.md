

### 1) ./modules/collisions.cpp

- dont really like the implementation of near distances, set_abs etc. splitting between two object types. 

###  4) How to deal with Mob Entity right way - I want to avoid enum/bool in entity search - shall I have additional table with SpawnedMobs. It will have entity Id?
Maybe just keep all of it in entities. It cant be that bad? Dont overengineer it 

### 5) Item texture id -> I messed there something, but its not that bad -> to figure out