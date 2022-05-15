# Vikings: Odin's wrath

Build the game:

```{commandline}

make
```

Run the game:


```{commandline}

./game
```


Build the tests:

```{commandline}

make test
```

run the tests:

```{commandline}

./test
```


Check opengl version
```{commandline}
glxinfo | grep "OpenGL version"
```

Ubuntu:
OpenGL version string: 4.6.0 NVIDIA 470.103.01

### Scripting
Using python script (totally forgot how)

#### Open commandline and go to directory
```{commandline}
cd ./Documents/projects/game_opengl/scripts
```
 
#### Open game terminal
```{commandline}
python3 app.py
```

#### Add and run function

1) Edit game_api.py -> add another function
2) Edit py.cpp/py.h -> add another function
3) Creates console.py with new arguments. This will be read by game engine when clicking 0


## todo list:
### Animations -> make sure the looped animations are looped (checking if next anim id is the same as current one? skip first frame if so?)
### Zooming in -> analyse how to zoom in on specific point 

### Unit tests
- Done: anims, camera, collisions, ecs, items, utils, scenes, models
- Dependency: Travel, map, navmesh, pathfinding, door collisions tests will wait till I have new maps pipeline as there is no point now

### Map generation
- Fired up the world machine on linux and it looks to be working!
- lets explore world machine for next couple of weeks

### Interactions history:
- encounters: https://www.youtube.com/watch?v=HZft_U4Fc-U&t=1s&ab_channel=GDC
- bitset of encounter and events flags. Encounters instead of quests
- quest => meet someone, go somwhere, get something, go back, get reward
(witcher: there is a wolg in the woods, you go there, there is a wolf, get the wolf heads, get it back and reward)

game design questions: (narrative)
- why is this peasant not doing anything about the wolf?
- if I do nothing why doesnt the peasant care
- why are woods empty if I go there first before meeting the peasant (activating the quest)
- why does no one else in the worlds care about the wolf in the woods?

- encounter -> You can know about the wolf or dont know about the wolf, but you can meet the wolf anyway

encounter structure:
- go to peasant house -> he tells you about the wolf ( you could see it before, already killed it, give the head)
OR 
- go to woods, meet the wolf and kill it or run away
OR revisit the peasant saying I still havent killed it
ignored the wolf again, came back and pesant is gone, wolf is gone, there is peasant hat in the woods

-> add peasant impatience level (if gets high then he goes and tries himself)

"ready myself" has to stay as an option always as we have to give player a way to pause the discussion -> defensive logic

Every scene written to cope with the game, regardless of state
Preconditions -> evry line/action gated by its preconditions
fallback: ready myself


2) state modelling
- state machines of everything 
- wolf: know about the wolf, wolf seen, wolf killed, wolf head cut off
- peasant: met, , told of wolf, grateful
- peasant impatient: impatient, gone, dead 

each state implies previous ones

How to implement state machines

StateMachine
{
    int last step
    enum?
}


State machines allow us to have emerging stories:
peasant told me about the wolf, I did nothing, pesant got impatient, went to kill a wolf, I later found his body, killed the wolf in revenge

Unexpacted benefits:





### Heightmaps/terrain:
- blender gis has broken api and cant get heightmap
- maybe this? https://tangrams.github.io/heightmapper/#3.10417/33.10/59.74
- https://blendermarket.com/products/nodescapes


### Rendering
- split quads per type of render (static camera, dynamic camera, clicked, Color, Texture) -> provide multiple draw calls but different shaders to remove If Else from shader code

### Sounds/Music
When I have longer soundtrack, will improve the functionality of sounds module. Things to do:
- volume setting
- stopping the sound
- starting the sound
- learn more about thread management



## Credits:
### Fonts
Joel Carrouche: https://www.fontspace.com/norse-font-f21080 www.joelcarrouche.com. Donate if used

font string:
ABCDEFGHIJKLMNOPRSTUWVXYZ0123456789!@#$%&*(){}[]^:;”’<>,./?|\-_=+

### software
https://github.com/adamstark/AudioFile/blob/master/LICENSE



### logo colors:
// background
#191717 - rgb(25, 23, 23)  - 0.098, 0,09, 0.09
// color
#B3ACAC - rgb(179, 172, 172)
// font
Medieval Sharp



### Betatesterzy
- Michal Klos "mogę być betatesterem" "ale potem chcę żeby gdzieś na jakimś kamieniu znalazł się mój nick 😃"



### nazwy 
Fiords of blood
Axe and oar

### Memory check

https://valgrind.org/docs/manual/quick-start.html


### Music scenarios
https://docs.google.com/document/d/1AhmMyDQHfmy1Cq3JuBcPSzUPpqGaNg9okGu3JyKvxtY/edit
