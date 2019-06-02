#ifndef _DESIGN_PATTERNS_GROOVY_BUILDER_H_
#define _DESIGN_PATTERNS_GROOVY_BUILDER_H_
#include <string>
#include <vector>

using namespace std;

namespace design_patterns
{
struct Tag
{
    Tag(const string& name, const string& text) : name(name), text(text){}
    Tag(const string &name, const string &text, const vector<Tag> &children) : name(name), text(text), children(children){}
    string name;
    string text;
    vector<Tag> children;
    vector<pair<string, string>> attributes;
};

struct P : public Tag
{
    P(initializer_list<Tag> children) : Tag("p", "", children)
    {
    }
};

struct IMG : public Tag
{
    IMG(string url) : Tag("IMG", "")
    {
        attributes.emplace_back("url", url);
    }
};



void test_groovy_builder()
{
    P p{
        IMG{"https://a.com/a.png"},
        IMG{"https://a.com/b.png"}
    };
}

}
#endif // _DESIGN_PATTERNS_GROOVY_BUILDER_H_