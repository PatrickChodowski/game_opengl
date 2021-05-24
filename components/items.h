#ifndef ITEMS_H
#define ITEMS_H


namespace items
{
  struct Item
  {
    int entity_id;
    std::string type;
    int texture_id;

    int x;
    int y;
  };

  std::map<int, Item> Catalog = {};
  std::vector<int> items_on_ground = {};
}

#endif