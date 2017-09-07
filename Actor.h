
#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class Compiler;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth);
    
    virtual ~Actor();
    
    // Action to perform each tick.
    virtual void doSomething() = 0;
    
    // Is this actor dead?
    virtual bool isDead() const;
    
    // Does this actor block movement?
    virtual bool blocksMovement() const;
    
    // Cause this actor to be be bitten, suffering an amount of damage.
    virtual void getBitten(int amt);
    
    // Cause this actor to be be poisoned.
    virtual void getPoisoned();
    
    // Cause this actor to be be stunned.
    virtual void getStunned();
    
    // Can this actor be picked up to be eaten?
    virtual bool isEdible() const;
    
    virtual void updateEnergy(int amt);
    
    virtual int getEnergy() const;
    
    // Is this actor detected by an ant as a pheromone?
    virtual bool isPheromone(int colony) const;
    
    // Is this actor an enemy of an ant of the indicated colony?
    virtual bool isEnemy(int colony) const;
    
    // Is this actor detected as dangerous by an ant of the indicated colony?
    virtual bool isDangerous(int colony) const;
    
    // Is this actor the anthill of the indicated colony?
    virtual bool isAntHill(int colony) const;
    
    // Get this actor's world.
    StudentWorld* getWorld() const;
    
    Direction getRandDirect();
    bool setDirect(Direction direct, int &prevX, int &prevY, int dist);
    
private:
    StudentWorld* m_stud;
};

class Pebble : public Actor
{
public:
    Pebble(StudentWorld* sw, int startX, int startY);
    virtual ~Pebble();
    virtual void doSomething();
    virtual bool blocksMovement() const;
};

class EnergyHolder : public Actor
{
public:
    EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth);
 
    virtual bool isDead() const;
    
    virtual ~EnergyHolder();
    
    //virtual void doSomething();
    //no doSomething because we want it pure virtual 
    
    // Get this actor's amount of energy (for a Pheromone, same as strength).
    virtual int getEnergy() const;
    
    // Adjust this actor's amount of energy upward or downward.
    virtual void updateEnergy(int amt);
    
    // Add an amount of food to this actor's location.
    void addFood(int amt);
    
    // Have this actor pick up an amount of food.
    int pickupFood(int amt);
    
    // Have this actor pick up an amount of food and eat it.
    int pickupAndEatFood(int amt);
    
    // Does this actor become food when it dies?
 //   virtual bool becomesFoodUponDeath() const;
    
private:
    int m_hitPts;
};

class Food : public EnergyHolder
{
public:
    Food(StudentWorld* sw, int startX, int startY, int energy);
    virtual ~Food();
    virtual void doSomething();
    virtual bool isEdible() const;
};

class AntHill : public EnergyHolder
{
public:
    AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program);
    virtual ~AntHill();
    virtual void doSomething();
    virtual bool isAntHill(int colony) const;

private:
    Compiler* m_prog;
    int m_colony;
};

class Pheromone : public EnergyHolder
{
public:
    Pheromone(StudentWorld* sw, int startX, int startY, int colony, int imageID);
    virtual ~Pheromone();
    virtual void doSomething();
    virtual bool isPheromone(int colony) const;
    
    virtual void updateEnergy(int amt);
    // Increase the strength (i.e., energy) of this pheromone.
    void increaseStrength();
    
private:
    int m_colony;
};

class TriggerableActor : public Actor
{
public:
    TriggerableActor(StudentWorld* sw, int x, int y, int imageID);
    virtual ~TriggerableActor();
    virtual bool isDangerous(int colony) const;
};

class WaterPool : public TriggerableActor
{
public:
    WaterPool(StudentWorld* sw, int x, int y);
    ~WaterPool();
    virtual void doSomething();
};

class Poison : public TriggerableActor
{
public:
    Poison(StudentWorld* sw, int x, int y);
    virtual ~Poison();
    virtual void doSomething();
};

class Insect : public EnergyHolder
{
public:
    Insect(StudentWorld* world, int startX, int startY, int energy, int imageID);
    
    virtual ~Insect();
    
    virtual void doSomething() = 0;
    
    int getDist() const;

    void setDist(int dist);
    
    bool getStunState() const;  //tells us if stunned or not
    
    void setStunState(bool stun);   //sets the stun state to true or false
    
    void setColony(int col);

    int getColonyHelp() const;
    
    virtual void getBitten(int amt);
    virtual void getPoisoned();
    virtual void getStunned();
    virtual bool isEnemy(int colony);
    
    virtual bool isDangerous(int colony) const;
    
//    virtual bool becomesFoodUponDeath() const;
    
    // Set x,y to the coordinates of the spot one step in front of this insect.
//   void getXYInFrontOfMe(int x, int y) const;
    
    // Move this insect one step forward if possible, and return true;
    // otherwise, return false without moving.
//    virtual bool moveForwardIfPossible();
    
    // Increase the number of ticks this insect will sleep by the indicated amount.
    void increaseSleepTicks(int amt);
    
    int getSleepTicks() const;
    
private:
    int m_dist;
    int m_sleepTicks;
    bool m_stunnedHere;
    int m_colony;
};

class Ant : public Insect
{
public:
    Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program, int imageID);
    virtual ~Ant();
    virtual void doSomething();
    virtual void getBitten(int amt);
    virtual bool isEnemy(int colony) const;
   // virtual bool moveForwardIfPossible();
private:
    int m_colony;
    int holdFood;
    int instructCounter;
    bool prevBit;
    bool prevBlocked;
    int randValue;
    Compiler* m_prog;
};

class Grasshopper : public Insect
{
public:
    Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID);
    
    virtual ~Grasshopper();
   // virtual bool isEnemy(int colony) const = 0;
  //  virtual void doSomething() const; this isnt needed
};

class BabyGrasshopper : public Grasshopper
{
public:
    BabyGrasshopper(StudentWorld* sw, int startX, int startY);
    virtual ~BabyGrasshopper();
    virtual void doSomething();
    virtual bool isEnemy(int colony) const;
private:
    int m_colony;
};

class AdultGrasshopper : public Grasshopper
{
public:
    AdultGrasshopper(StudentWorld* sw, int startX, int startY);
    ~AdultGrasshopper();
    virtual void doSomething();
    virtual void getPoisoned();
    virtual void getStunned(); 
    virtual void getBitten(int amt);
    virtual bool isEnemy(int colony) const;
private:
    int m_colony;
};

#endif // ACTOR_H_
