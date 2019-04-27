#include <string>
#include <vector>
#include <iostream>

namespace design_patterns
{
    


enum class Color{Green, Red, Blue};
enum class Size{Small, Medium, Large};

class Product{
public:
    Product(const std::string& name, Color color, Size size)
        : name_(name), color_(color), size_(size) { }
    
    Color getColor() const {return color_;}
    std::string getName() const {return name_;}
    Size getSize() const {return size_;}
private:
    std::string name_;
    Color color_;
    Size size_;
};

class ProductFilter{
public:
    using Products_t = std::vector<Product*>;
    static  Products_t filter_by_Color(Products_t products, Color color)
    {
        std::vector<Product*> ret;
        for (auto& p : products)
        {
            if(p->getColor() == color) ret.push_back(p);
        }
        return ret;
    }

    static Products_t filter_by_Size(Products_t products, Size size)
    {
        std::vector<Product*> ret;
        for (auto& p : products)
        {
            if(p->getSize() == size) ret.push_back(p);
        }
        return ret;
    }
};

//OPEN-CLOSE Principle
template <typename T>
class ISpecification{
public:
    virtual bool is_satisfied(T* item) = 0;
};

template <typename T>
class IFilter{
public:
    virtual std::vector<T*> filter(std::vector<T*> products, ISpecification<T>& spec) = 0;
};

class Filter : public IFilter<Product>
{
public:
    using Products_t = std::vector<Product*>;
    Products_t filter(Products_t products, ISpecification<Product>& spec) override
    {
        Products_t result;
        for(auto& p : products)
        {
            if(spec.is_satisfied(p)) result.push_back(p);
        }
        return result;
    }
};

class ColorSpec : public ISpecification<Product>
{
public:
    ColorSpec(Color color) : color_(color){}
    bool is_satisfied(Product* p) override 
    {
        return p->getColor() == color_;
    }
    Color color_;
};

class SizeSpec : public ISpecification<Product>
{
public:
    SizeSpec(Size size) : size_(size){}
    bool is_satisfied(Product* p) override 
    {
        return p->getSize() == size_;
    }

    Size size_;
};


void OC_test()
{
    Product apple{"apple", Color::Red, Size::Small};
    Product phone{"phone", Color::Green, Size::Small};
    Product laptop{"laptop", Color::Blue, Size::Medium};

    std::vector<Product*> products{&apple, &phone, &laptop};

    //use static methods to implement filter function
    //it's very ugly
    auto results = ProductFilter::filter_by_Color(products, Color::Red);
    for(auto& p : results)
    {
        std::cout << p->getName() << " color is red" << std::endl;
    }

    auto results2 = ProductFilter::filter_by_Size(products, Size::Small);
    for(auto& p : results2)
    {
        std::cout << p->getName() << " size is small" << std::endl;
    }


    //using open close 
    Filter f{};
    ColorSpec cspec{Color::Red};

    auto results3 = f.filter(products, cspec);
    for(auto& p : results3)
    {
        std::cout << p->getName() << " color is red" << std::endl;
    }

    SizeSpec sspec{Size::Small};

    auto result4 = f.filter(products, sspec);
    for(auto& p : result4)
    {
        std::cout << p->getName() << " size is small" << std::endl;
    }

}
} // design_patterns