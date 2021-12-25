# Fonts
Path: ./modules/fonts.cpp   ./modules/fonts.h


## Objects

### CharacterData
Specific character data read from the ttf font
```c++
struct CharacterData 
```

### TextData
Specific letter (full string) data. Used to make quads
```c++
struct TextData 
```

### LabelData
Contains full label (full string) data added to the game
```c++
struct LabelData 
```

### chars
Catalog of the characters from the font data ([CharacterData](fonts.md#CharacterData))
```c++
phmap::flat_hash_map<char, CharacterData> chars;
```

### texts
Catalog of the characters with the text data ([TextData](fonts.md#TextData))
```c++
phmap::flat_hash_map<char, TextData> texts;
```

### labels
Catalog of the labels ([LabelData](fonts.md#LabelData))
```c++
phmap::flat_hash_map<char, LabelData> labels;
```

### TextQuads
Quads made out of the [texts](debug.md#texts). Vector of [quads](quads.md#QuadData)
```c++
std::vector<quads::QuadData> TextQuads;
```

### TextIndex
Index of unique Text IDs
```c++
std::vector<int> TextIndex;
```

### LabelIndex
Index of unique Label IDs
```c++
std::vector<int> LabelIndex;
```

### FontTDD
Font's [texture data](textures.md#TextureData)
```c++
textures2::TextureData FontTDD;
```

### NEW_GAME_LABEL_ID
 ID of the label for new game name input
```c++
int NEW_GAME_LABEL_ID;
```




## Functions

### init
Initialize all fonts data (based on the font name). Creates [FontTDD](fonts.md#FontTDD) and binds the texture. Creates also [chars](fonts.md#chars) 
```c++
void init(std::string font_name);
```

### add
Adds new [LabelData](fonts.md#LabelData) to [labels](fonts.md#labels)
```c++
int add(std::string& text, float x_start, float y_start, float camera_type, float scale, float r, float g, float b);

```

### render_chars
Gets characters information for selected [LabelData](fonts.md#LabelData) from [chars](fonts.md#chars)  and writes it to [texts](fonts.md#texts)
```c++
void render_chars(fonts2::LabelData ldd);

```

### render
Populates [TextQuads](fonts.md#TextQuads) from [texts](fonts.md#texts)
```c++
void render();
```

### clear
Clears [TextQuads](fonts.md#TextQuads), [TextIndex](fonts.md#TextIndex), [LabelIndex](fonts.md#LabelIndex), [texts](fonts.md#litextsnes), [labels](fonts.md#labels)
```c++
void clear();
```

### drop
Deletes selected [LabelData](fonts.md#LabelData) from [labels](fonts.md#labels). Deletes ID from [LabelIndex](fonts.md#LabelIndex)
```c++
void drop(int label_id);
```

## Tests
-