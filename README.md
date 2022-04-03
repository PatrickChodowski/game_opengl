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

### Adjust animations to use models instead of textures


### Models
- one more, new way of handling textures, frames and animations -> this will be renamed to models. Based on the files provided in ./models folder

Data file will have name format of {model_id}_{model_name}.json

Spritesheet will have name format of {model_id}_{model_name}___spritesheet.png