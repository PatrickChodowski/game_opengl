ideas:
 - test with static array
 - add function to render one piece of data -> will return quad_id that can be later stored
 - work more on the reset controller
 - avoid resetting whole AllQuads


### how to load really big map -> impossible now, question is why exactly  -> quads optimization, maybe chunks?

- create one big float array for all the quads data instead of AllQuads -> define the ranges for each module
0-99 menu quads
100-999 entity quads
1000-10000 map quads
 etc.

 have Quads controller keeping track of ranges and flags if given range need an update 
 update -> wipe out the range and assign new values from module's catalog

 One array,all quads, no more module quads, just accessing this chunk of memory from all other modules

 checks if some modules exceeds the range, then just throw error

 ### optimazatioon :: quads controller, arrays instead of vectors




### npc interaction system -> heart of the game! heart of the story basically -> introduce npcs -> earls, jarls, kings, gods etc.

### personality traits of the leaders -> random
- random personality traits
- random relations with other kingdoms
- personality traits deciding if they start a conflict

### 3d models instead of spritesheets:
 - photogrammetry
 - meshroom 
 - https://www.mixamo.com/
 - blender
 - blender python api
 - PIFuh by facebook
 - gITF file format 






### animate mobs

### shaders management -> remove if elses from shaders -> one shader per draw call

### animations of course not perfect- need to breakable, more logic, etc. but looks good!

### OpenGL book

### Shaders book

#


 ### how to make it isometric! just for test -> isometric projection

We angle our camera along two axes (swing the camera 45 degrees to one side, then 30 degrees down). This creates a diamond (rhombus) shaped grid where the grid spaces are twice as wide as they are tall. This style was popularized by strategy games and action RPGs. If we look at a cube in this view, three sides are visible (top and two facing sides).

https://gamedevelopment.tutsplus.com/tutorials/creating-isometric-worlds-a-primer-for-game-developers--gamedev-6511

 https://www.youtube.com/watch?v=RmMDFydHeso&ab_channel=Crompwell  

https://www.youtube.com/watch?v=04oQ2jOUjkU&ab_channel=JordanWest -> very good instructions


 ### when zoom out, middle mouse button could move the view -> but how? :o

 ### menu highliting -> nice feature to have :)

 ### saves scrolling

 ### Position, color, normalized vector structs in utils

 ### fonts size options? :( :( :(:( maybe another day...

 ### travels code is messy and probably will enjoy another iteration later + some tests eventually if needed

 ### game should be 2D topdown

# Notes:

ZARRTY:
--- - --- 
easter egg:
zielony ssak
ksiazka o przeuzywanych cytatach - droga nna ktorej spotykasz wrogow to ta dobras
fishing mechanic that doesnt do anything
drugi akt to oczywiscie pustynia
introdce thor by being part of the ogre (Thrym)

TODO:
  - anims:
    - Animations from json file (hero walking)
    - Animations manager being worked on 


  - debug (branch):
    - render shadow quads
    - set_labels() definately not set in the right moment (on quads in handle_state, but quads are summarized later :o)
    
 - mobs
  - close travel if new started for given mob
  - travel nie dziala dobrze jak jest zoom i move, ale to dobrze, bo zoom to tylko debug


IDEAS:
- reinforcement learning for monsters play
- animations from blender
- debug shader do wyswietlania collisions etc.
- https://www.youtube.com/watch?v=Lm_AzK27mZY&ab_channel=GameMaker%27sToolkit - Nemesis system - npc having knowledge of what happened before
- https://www.youtube.com/watch?v=p40p0AVUH70&ab_channel=GDC - frakcje i system zero sum game  - wygraj szacunek jednej frakcji i reszta sie zbierze by zaatakowac zamek
 mozna wygrac lub przegrac ta bitwe

- to bedzie o wikingach nie?
- statki, dziki, bogowie, topory

- try to understand how to implement animation using json (map of structs instead of IF ELSE tree)?
- read json into this
- similar with behavior trees for AI

https://www.youtube.com/watch?v=6VBCXvfNlCM&ab_channel=AIandGames
https://www.youtube.com/watch?v=JyF0oyarz4U&ab_channel=AIandGames

game:
-- cactuses are strong



vikingowie:
budowa lodzi
napadznie miast
wygnanie
status w kazdym miescie
banda skogermor i napad na miasto
mozliwosc zostania earlem/jarlem/krolem






## assets:

https://opengameart.org/content/grassland-tileset
https://opengameart.org/content/10-basic-rpg-enemies
https://opengameart.org/content/four-characters-my-lpc-entries
https://opengameart.org/content/desert-tileset
https://opengameart.org/content/stunning-pixel-art-rpg-tileset