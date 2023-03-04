#pragma once
#include <functional>
#include <iostream>
#include <map>
#include <memory>
using namespace std;

class HotDrink {
public:
  virtual void prepare(int volume) = 0;
};

struct Tea : HotDrink {
  void prepare(int volume) override {
    cout << "make tea volume: " << volume << endl;
  }
};

struct Coffee : HotDrink {
  void prepare(int volume) override {
    cout << "make coffee volume: " << volume << endl;
  }
};

struct HotDrinkFactory {
  // we could provide some default implemetation in this abstract_factory
  virtual unique_ptr<HotDrink> make() = 0 ;
};

struct CoffeeFactory : HotDrinkFactory{
  unique_ptr<HotDrink> make() override {
    return std::make_unique<Coffee>();
  }
};

struct TeaFactory : HotDrinkFactory{
  unique_ptr<HotDrink> make() override {
    return std::make_unique<Tea>();
  }
};

class DrinkFactory {
  std::map<string, unique_ptr<HotDrinkFactory>> factories;
  public:
  DrinkFactory() {
    factories["coffee"] = make_unique<CoffeeFactory>();
    factories["tea"] = make_unique<TeaFactory>();
  }

  unique_ptr<HotDrink> make_drink(const string& name) {
    auto drink = factories[name]->make();
    drink->prepare(200); // oops!
    return drink;
  }
};

// functional factory
class DrinkWithVolumeFactory {
  map<string, function<unique_ptr<HotDrink>()>> factories;
  public:
  DrinkWithVolumeFactory() {
    factories["tea"] = []{
      auto tea = make_unique<Tea>();
      tea->prepare(100);
      return tea;
    };
  }

  unique_ptr<HotDrink> make_drink(const string& name) {
    return factories[name]();
  }
};


inline void test_abstract_factory() {
  DrinkFactory df;
  df.make_drink("tea");

  DrinkWithVolumeFactory dvf;
  dvf.make_drink("tea");
}
