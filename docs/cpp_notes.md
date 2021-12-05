
### looping through the map


```c++
// c11
for (auto const& x : symbolTable)
{
    std::cout << x.first  // string (key)
              << ':' 
              << x.second // string's value 
              << std::endl;
}

```

```c++
// c17
for (auto const& [key, val] : symbolTable)
{
    std::cout << key        // string (key)
              << ':'  
              << val        // string's value
              << std::endl;
}

```


### template definition

```c++

 template std::vector<quads2::QuadData> quads2::make_quads<debug2::PointData>(std::map<int, debug2::PointData>, int);
 template std::vector<quads2::QuadData> quads2::make_quads<entity::EntityData>(std::map<int, entity::EntityData>, int);
 template std::vector<quads2::QuadData> quads2::make_quads<fonts2::TextData>(std::map<int, fonts2::TextData>, int);
 
```

### function pointers
```c++
  // .h:
  typedef void (*sig_ptr)(SDL_Event event);
  // Catalog of functions to be chosen based on the scene id
  extern std::map <int , sig_ptr> EventsHandler;

  // .cpp:
  std::map <int , sig_ptr> EventsHandler = {};
```

### find if key in map
```c++
m.count(key)


```


### good practice:
 - avoid bools in structs and if(isActive) stuff
 bool is one bit, but uses 8 bytes of memory inside struct as it needs whole block