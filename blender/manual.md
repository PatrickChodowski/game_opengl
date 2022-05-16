# Blender Manual


## User interface
3 main parts:
 - Topbar
 - Areas
 - Status Bar


 ## Keys:
 - F3: menu search
 - F11: View Render
 - F12: Render Image

## Workspaces:
Workspaces (Tab)s are basically window layouts to do specific work. Areas can be customized to create workspaces within the same file.


### Layout

Default startup workspace. General one to preview scene and objects and following editors:
 - 3D viewport
 - Outliner (Scene collection, object list) (top right)
 - Properties (bottom right)
 - Timeline (bottom left)


## Data API 
In the outliner the view layer can be replaced with Data API that would list all data objects and their properties in the UI.


## Editors
Editors are used to display and modify different aspects of data

### 3d viewport
Used to interact with 3d scene for modelling, animating, texture painting etc.


#### 3D Cursor
point in 3D space

#### Navigating

Zoom on Area: Shift B  + LMB select and it zooooooooms!

Home key: Shows all objects in the viewport

Dolly zoom : shift + crtl and MMB


Fly/walk navigation -> when you want to move as first person controls
Setup from preferences editor -> navigation methods

or

View -> navigation menu


##### Aligning navigation:
- align active camera to view
- align active camera to selected (object)
- center cursor and frame all
- center view to cursor
- view lock to active

#### Camera View
- rendered image will show everything within the dashed line

#### Viewport shading
different shaders showing for different tasks
- wireframe (only edges)
- solid 
- material preview (Eevee engine) - useful for previewing materials and texture painting
- rendered

## Scene
Scene properties
- camera: Used to select which camera is useda s the active camera.
- background scene: Background scene useful when you only want to animate the foreground
- units
- gravity

## Mesh

Modelling meshes. You can use 3 modes: object, edit, sculpt
Everything is built from 3 basic structures: vertices, edges and faces.

### Primitives
A common object type used in 3D scene. Can be cube, plane, sphere, circle, UV sphere, icosphere, cylinder, cone, torus, grid

### Object origin
Each object has an origin point.  Location of this point determines where the object is located in 3D space. Important when translating, rotating or scaling the object.

### Vertices (Vertex)
Vertex is a single point or position in 3D space. Represented in 3D viewport in Edit Mode as dots
The vertices of an object are stored as an array of coordinates.


### Edges 
Edge always connects two vertices by straight line.

### Faces
Actual surface of the object. Always flat. They are built between 3,4, or more vertices.

### Normals
Normal is a direction or line that is perpendicular (prostopadle) to the surface.



## Rendering
Blender has 3 render engines:
- eevee: Realtime renderer
- Cycles: path tracer
- Workbench: layout, modeling and previews


### Eevee
Realtime render engine using OpenGl, focused on speed and interactivity. Main goal is to render materials. Uses rasterization instead of rays of light.

Limitations of eevee: 
https://docs.blender.org/manual/en/latest/render/eevee/limitations.html

#### Settings:
- sampling: The more samples, the more aliasing is reduced.
- grease pencil
- ambient occlusion
- bloom - diffuses very bright pixels
- depth of field - further objects are more blurred
- subsurface scattering
- screen space reflections
- motion blur
- volumetrics - adding lightting effects to rendered scene (god rays)
- performance
- hair
- shadows
- indirect lighting
  All lighting that is not coming out from a light object is considered as indirect lighting in Eevee. In eevee it is diffuse and specular.

#### Eevee World:
World environment can emit light.



#### Light settings:







### Displacement maps
Displacement mappign alows a texture input to manipulate the position of vertices on rendered geometry. Displacement maps create real bumps, creaces, ridges in the actual mesh.





















# Glossary

https://docs.blender.org/manual/en/latest/glossary/index.html#term-Aliasing

- Aliasing: rendering artifacts in the form of jagged lines
- Alpha channel - channel of image for transparency
- Ambient light - light coming from the surrounding environment as a whole
- Ambient Occlusion: ratio of how much  ambient light a surface point will receive. Makes points that are under something else darker.
- Anti-Aliasing - technique for minimizing aliasing
- Armature - object consisting of bones. Used to rig characters
- Bounding box - box that encloses the shape of an object
- Color model - RGB, HSV
  - hue (odcien) (what color basically)(between 0 and 360, where read is between 0 and 60 etc.)
  - saturation (nasycenie) (how intense the color is - no saturation = grey). The amount of gray in color, from 0 to 100 percent
  - value (brightness) (0 is black, 1 is the brightest of the color)
