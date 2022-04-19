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


### Map generation

- Fired up the world machine on linux and it looks to be working!
- lets explore world machine for next couple of weeks


### Rendering
- split quads per type of render (static camera, dynamic camera, clicked, Color, Texture) -> provide multiple draw calls but different shaders to remove If Else from shader code

### Next up
- sound effects


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





### Music scenarios

1) Intro - main theme. Mroczny, zimny, nieprzyjazny klimat swiata
https://www.youtube.com/watch?v=Q2evIg-aYw8

2) Przebywanie w osadzie/bezpiecznym miejscu przed wyruszeniem w trase. 
Spokojny, "nienarzucajacy" sie motyw. 

3) Podroz morska lodzia wikingow. Muzyka ktora by leciala gdy gracz jest na morzu. 
Mozliwe ze warto by bylo miec motyw morski lekki i burzliwy - możliwe będą sztormy. 
Inna muzyka w trakcie sztormów dodałaby dramaturgii.

4) Marsz przez pustkowie, najlepiej w trakcie szalejącej wichury/zamieci.

5) Muzyka w trakcie walki. Zaczynałaby sie na początku potyczki i trwała tylko do końca starcia.
Wiedzmin mial sporo takich kawalkow i korzystali z nich w zaleznosci od sytuacji 
("steel for humans" w trakcie walki z ludzmi, "silver for monsters" w walce z potworami)
https://www.youtube.com/watch?v=lAGm9MTyRJ8


inspiracje:
https://www.youtube.com/watch?v=Qj1RS9vHcsw
https://youtu.be/zLSKgqULxyY?t=446
https://www.youtube.com/watch?v=zYB0BBaU5K8
https://www.youtube.com/watch?v=cNDsx_wUVm8
https://www.youtube.com/watch?v=Qhz67kxqnjI (glowny motyw serialu)

inspiracje ale metal:
https://www.youtube.com/watch?v=x0ktYmedDJE
https://www.youtube.com/watch?v=Mbsc-3T6o4M
https://www.youtube.com/watch?v=83eDWKllyWo
https://www.youtube.com/watch?v=pjwMOzn2Y2A