#ifndef _DESIGN_PATTERNS_CHAIN_OF_RESPONSIBILITY_POINTER_CHAIN_H_
#define _DESIGN_PATTERNS_CHAIN_OF_RESPONSIBILITY_POINTER_CHAIN_H_
#include <iostream>
#include <string>
using namespace std;

namespace design_patterns
{
class Creature
{
public:
    Creature(const string& name, int attack, int defence)
        : name(name)
        , attack(attack)
        , defence(defence) {}
    
    string name;
    int attack;
    int defence;

    friend ostream& operator<< (ostream& os, const Creature& creature)
    {
        os << "name: " << creature.name 
           << " attack: " << creature.attack 
           << " defence: " << creature.defence << endl;
    }
};

class CreatureModifier
{
public:
    CreatureModifier(Creature& creature) : targetCreature(creature), next_(0){}
    virtual ~CreatureModifier(){}

    void add(CreatureModifier& modifier)
    {
        if(next_) next_->add(modifier);
        else next_ = &modifier;
    }

    virtual void execute()
    {
        if(next_) next_->execute();
    }

protected:
    Creature& targetCreature;
    CreatureModifier* next_;
};

class DoubleAttackModifier : public CreatureModifier
{
public:
    DoubleAttackModifier(Creature& creature) : CreatureModifier(creature){}
    void execute() override
    {
        targetCreature.attack *= 2;
        CreatureModifier::execute();
    }
};

class DoubleDefenceModifier : public CreatureModifier
{
public:
    DoubleDefenceModifier(Creature& creature) : CreatureModifier(creature){}
    void execute() override
    {
        targetCreature.defence *= 2;
        CreatureModifier::execute();
    }
};

void test_pointer_chain()
{
    Creature goblin{"Globin", 1, 1};
    CreatureModifier rootModifier{goblin};
    DoubleAttackModifier daModifier{goblin};
    DoubleDefenceModifier dfModifier{goblin};
    rootModifier.add(daModifier);
    rootModifier.add(dfModifier);

    rootModifier.execute();
    cout << goblin;

}

}

#endif //_DESIGN_PATTERNS_CHAIN_OF_RESPONSIBILITY_POINTER_CHAIN_H_