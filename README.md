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





## todo list:

### Models
- one more, new way of handling textures, frames and animations -> this will be renamed to models. Based on the files provided in ./models folder

Data file will have name format of {model_id}_{model_name}.json

Spritesheet will have name format of {model_id}_{model_name}___spritesheet.png