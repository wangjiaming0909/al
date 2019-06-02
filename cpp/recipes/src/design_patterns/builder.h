#ifndef _DESIGN_PATTERNS_BUILDER_H_
#define _DESIGN_PATTERNS_BUILDER_H_
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <memory>

namespace design_patterns
{

class HtmlBuilder;
struct HtmlElement
{
    std::string name;
    std::string text;
    std::vector<HtmlElement> elements;
    const size_t indent_size = 2;

    HtmlElement(){}
    HtmlElement(const std::string& name, const std::string& text)
        : name(name), text(text){}

    std::string str(int indent = 0) const
    {
        std::ostringstream oss;
        std::string i(indent_size * indent, ' ');
        oss << i << "<" << name << ">" << std::endl;

        if(text.size() > 0)
            oss << std::string(indent_size * (indent + 1), ' ') << text << std::endl;

        for(const auto& e: elements)
            oss << e.str(indent + 1);

        oss << i << "</" << name << ">" << std::endl;
        return oss.str();
    }

    static HtmlBuilder builder(const std::string &root_name);
    static std::unique_ptr<HtmlBuilder> builder_ptr(const std::string &root_name);
};

struct HtmlBuilder
{
    HtmlBuilder(const std::string& root_name)
    {
        root.name = root_name;
    }

    HtmlBuilder &add_child(const std::string& child_name, const std::string& child_text)
    {
        HtmlElement e{child_name, child_text};
        root.elements.emplace_back(e);
        return *this;
    }

    HtmlBuilder* add_child2(const std::string &child_name, const std::string &child_text)
    {
        HtmlElement e{child_name, child_text};
        root.elements.emplace_back(e);
        return this;
    }

    std::string str() const 
    {
        return root.str();
    }

    operator HtmlElement() { return root; }

    HtmlElement root;
};

HtmlBuilder HtmlElement::builder(const std::string &root_name)
{
    return HtmlBuilder{root_name};
}

std::unique_ptr<HtmlBuilder> HtmlElement::builder_ptr(const std::string &root_name)
{
    return std::make_unique<HtmlBuilder>(root_name);
}

void test_build()
{
    HtmlBuilder builder{"root"};
    builder.add_child("li", "hello").add_child("li", "world");
    std::cout << builder.str() << std::endl;

    HtmlElement e = HtmlBuilder{"root"};

    HtmlElement ee = HtmlElement::builder("root").add_child("li", "hello").add_child("li", "world");
    std::cout << ee.str() << std::endl;

    HtmlElement eee = *(HtmlElement::builder_ptr("root")->add_child2("li", "hello")->add_child2("li", "world"));
    std::cout << eee.str() << std::endl;
}
}
#endif //_DESIGN_PATTERNS_BUILDER_H_