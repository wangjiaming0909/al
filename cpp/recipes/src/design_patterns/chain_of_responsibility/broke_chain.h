#ifndef _DESIGN_PATTERNS_CHAIN_OF_RESPONSIBILITY_BROKE_CHAIN_H_
#define _DESIGN_PATTERNS_CHAIN_OF_RESPONSIBILITY_BROKE_CHAIN_H_

#include "boost/signals2.hpp"

#include <iostream>
#include <string>
using namespace boost::signals2;
using namespace std;

namespace design_patterns
{


class Query
{
public:
    string creature_name;
    enum Argument {attack, defence} argument;
    int result;

    Query(const string& name, Argument argument, int result)
        : creature_name(name)
        , argument(argument)
        , result(result){}
};

class Game
{
public:
    signal<void(Query&)> queries;
};

struct NewCreature
{
    NewCreature(Game& game, const string& name, int attack, int defence)
        :game(game)
        , name(name)
        , attack(attack)
        , defence(defence){ }
    
    int getAttack()
    {
        Query q{name, Query::Argument::attack, attack} ;
        game.queries(q);
        return q.result;
    }
    int getDefence()
    {
        return defence;
    }

    friend ostream& operator<< (ostream& os, const NewCreature& creature)
    {
        os << "name: " << creature.name 
           << " attack: " << creature.attack 
           << " defence: " << creature.defence << endl;
        return os;
    }

private:
    Game& game;
    string name;
    int attack;
    int defence;
};

struct NewCreatureModifier
{
    NewCreatureModifier(Game& game, NewCreature& creature) 
        : targetCreature(creature)
        , game_(game){}
    virtual ~NewCreatureModifier(){}

    void add(NewCreatureModifier& modifier)
    {
        if(next_) next_->add(modifier);
        else next_ = &modifier;
    }

    void execute()
    {
        if(next_) next_->execute();
    }
private:
    NewCreature& targetCreature;
    NewCreatureModifier* next_;
    Game& game_;
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