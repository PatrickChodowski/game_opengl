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
### Collision box currently has the size of the texture which is not good as its too big -> invisible walls. Use hook position of the frame?


### Map generation

- Fired up the world machine on linux and it looks to be working!
- lets explore world machine for next couple of weeks


### Rendering
- split quads per type of render (static camera, dynamic camera, clicked, Color, Texture) -> provide multiple draw calls but different shaders to remove If Else from shader code


### Next up
- sound effects


### Entities
ECS:
 - collisions with new objects -> items, mobs, entities, doors

 AND THEN:
 - picking up items
 - going through the door
 - interacting with npc
 - displaying menu

AND THEEN:
 - rewrite tests
 - rewrite Python api

## Credits:


### Fonts
Joel Carrouche: https://www.fontspace.com/norse-font-f21080 www.joelcarrouche.com. Donate if used


font string:
ABCDEFGHIJKLMNOPRSTUWVXYZ0123456789!@#$%&*(){}[]^:;”’<>,./?|\-_=+



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