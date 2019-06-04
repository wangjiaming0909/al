#ifndef _DESIGN_PATTERNS_CHAIN_OF_RESPONSIBILITY_BROKE_CHAIN_H_
#define _DESIGN_PATTERNS_CHAIN_OF_RESPONSIBILITY_BROKE_CHAIN_H_

#include "boost/signals2.hpp"

#include <iostream>
#include <string>
using namespace boost::signals2;
using namespace std;

namespace design_patterns
{

struct NewCreature
{
    NewCreature(const string& name, int attack, int defence)
        : name(name)
        , attack(attack)
        , defence(defence){ }
    string name;
    int attack;
    int defence;

    int getAttack()
    {

    }
    int getDefence()
    {

    }

    friend ostream& operator<< (ostream& os, const NewCreature& creature)
    {
        os << "name: " << creature.name 
           << " attack: " << creature.attack 
           << " defence: " << creature.defence << endl;
    }
};

struct NewCreatureModifier
{
    NewCreature& targetCreature;
    NewCreatureModifier(NewCreature& creature) : targetCreature(creature){}
    virtual ~NewCreatureModifier(){}

    void add(NewCreatureModifier& modifier)
    {
        if(next_) next_->add(modifier);
        else next_ = &modifier;
    }

    void execute()
    {
        if(next_) 
    }
private:
    NewCreatureModifier* next_;
};


void action1()
{
    cout << "action1" << endl;
}

void test_broke_chain()
{
    using action_t = signal<void()>;
    action_t s{};
    s.connect(action_t::slot_type(action1));

    s();
}

}

#endif //_DESIGN_PATTERNS_CHAIN_OF_RESPONSIBILITY_BROKE_CHAIN_H_