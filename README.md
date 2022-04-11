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

### Entities -> rework. Need to able to easily put a Logo inside the game. Currently a massive amount of work just to put a single entity in new place
# lets do entity 2.0

### Animations -> make sure the looped animations are looped (checking if next anim id is the same as current one? skip first frame if so?)
### Zooming in -> analyse how to zoom in on specific point 
### Active/Bind textures should have the same slot as in the sampler (it can come back later)
### Collision box currently has the size of the texture which is not good as its too big -> invisible walls. Use hook position of the frame?


### Models
- one more, new way of handling textures, frames and animations -> this will be renamed to models. Based on the files provided in ./models folder

Data file will have name format of {model_id}_{model_name}.json

Spritesheet will have name format of {model_id}_{model_name}___spritesheet.png


### Map generation

- Fired up the world machine on linux and it looks to be working!
- lets explore world machine for next couple of weeks


### Rendering
- split quads per type of render (static camera, dynamic camera, clicked, Color, Texture) -> provide multiple draw calls but different shaders to remove If Else from shader code


### Next up
- sound effects


### Entities

Eventually entities will be defined in the /data/entities/
Basic information -> id, name, model_id, start_frame_id, dims(w, h)

And list of components?

- NPCComponent
- HeroComponent
- MobComponent
- ButtonComponent?
- MenuComponent?
- LabelComponent?
- ItemComponent?
- DoorComponent?
- CollisionComponent?
- MouseClickComponent?
- PositionComponent
- ColorComponent
- ModelComponent
- DimComp




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