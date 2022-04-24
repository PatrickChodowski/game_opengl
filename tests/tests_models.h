
#include "../modules/ecs.h"
#include "../modules/hero.h"
#include "../modules/models.h"
#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

bool test_models__load_model()
{
  bool passed = true;
  game::clear();
  ecs::clear();

  //models::SceneModels.size();

  if(models::SceneModels.size() != 0){
    std::cout << " [ERROR] models::SceneModels are not empty, size:" << models::SceneModels.size() << std::endl;
    return false;
  }

  // Loading warrior2
  models::load(2);

  if(models::SceneModels.size() != 1){
    std::cout << " [ERROR] models::SceneModels should be size 1, but size:" << models::SceneModels.size() << std::endl;
    return false;
  }


  if(!models::ModelTextureMap.count(2)){
    std::cout << " [ERROR] no model 2 in model texture map"  << std::endl;
    return false;
  }

  // trying to load again
  models::load(2);

  if(models::SceneModels.size() != 1){
    std::cout << " [ERROR] models::SceneModels should be size 1, but size:" << models::SceneModels.size() << std::endl;
    return false;
  }

  if(models::ModelTextureMap.at(2) != 1){
    std::cout << " [ERROR] wrong logic in texture map"  << std::endl;
    return false;
  }

  // loading another model
  models::load(3);
  if(models::SceneModels.size() != 2){
    std::cout << " [ERROR] models::SceneModels should be size 2, but size:" << models::SceneModels.size() << std::endl;
    return false;
  }

  if(models::ModelTextureMap.at(2) != 1){
    std::cout << " [ERROR] wrong logic in texture map (2)"  << std::endl;
    return false;
  }

  if(models::ModelTextureMap.at(3) != 2){
    std::cout << " [ERROR] wrong logic in texture map (3)"  << std::endl;
    return false;
  }


  return passed;
}
