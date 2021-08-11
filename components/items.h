#ifndef ITEMS_H
#define ITEMS_H

// frame ids per item in items.json

namespace items
{
  std::map<std::string, int> Catalog = {};
  std::vector<int> items_on_ground = {};

  void init()
  {
    Catalog.insert(std::pair<std::string, int>{"stick", 0});
  }
}

#endif