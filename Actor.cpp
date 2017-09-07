#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"
#include <cstdlib>
#include <cmath>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

/************************************************/
/**********the code below is for ACTOR***********/
/************************************************/

Actor::Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth)
:GraphObject(imageID, startX, startY, startDir, depth)
{
    //constructor for actor simply passes on the params to GraphObject via init list
    m_stud = world;
}

Actor::~Actor()
{
    //destructor doesnt need to do anything, but base is virtual
}

Actor::Direction Actor::getRandDirect()
{
    int dir = randInt(1, 4);   //generates number 1 - 4
    switch(dir)
    {
        case 1:
            return up;
            break;
        case 2:
            return right;
            break;
        case 3:
            return down;
            break;
        case 4:
            return left;
            break;
        default:
            return none;
    }
}

bool Actor::setDirect(Direction direct, int &prevX, int &prevY, int dist)
{
//    getWorld()->canMoveTo(prevX, prevY);
    if (direct == up)
    {
        if (getWorld()->canMoveTo(prevX, prevY+1))    //if we can move there
        {
        prevY = prevY + 1;      //otherwise it is open and we can move to the new position, returns true for success
        return true;
        }
        else
        {
            return false;
        }
    }
    else if (direct == right)
    {
        if (getWorld()->canMoveTo(prevX+1, prevY))
        {
        prevX = prevX + 1;
        return true;
        }
        else
        {
            return false;
        }
    }
    else if (direct == down)
    {
        if (getWorld()->canMoveTo(prevX, prevY-1))
        {
        prevY = prevY - 1;
        return true;
        }
        else
        {
            return false;
        }
    }
    else if (direct == left)
    {
        if (getWorld()->canMoveTo(prevX-1, prevY))
        {
        prevX = prevX - 1;
        return true;
        }
        {
            return false;
        }
    }
    return false;
}

bool Actor::blocksMovement() const
{
    return false;
}

StudentWorld* Actor::getWorld() const
{
    return m_stud;
}

bool Actor::isEdible() const
{
    return false;   //most things aren't edible unless otherwise set
}

void Actor::getStunned()
{
    //does nothing in most cases unless specialized later for insects
}

void Actor::getPoisoned()
{
    //does nothing in most cases unless specialized later for insects
}

bool Actor::isDead() const
{
    return false;   //things aren't dead until they are actually dead
}

void Actor::updateEnergy(int amt)
{
    return; //does nothing, only to be used by Food class
}

bool Actor::isEnemy(int colony) const
{
    return false;   //returns false by default unless otherwise overridden
}

void Actor::getBitten(int amt)
{
    return; //things dont get bitten unless otherwise overriden
}

bool Actor::isPheromone(int colony) const
{
    return false;
}

bool Actor::isAntHill(int colony) const
{
    return false;
}

bool Actor::isDangerous(int colony) const
{
    return false;
}

int Actor::getEnergy() const
{
    return 0;
}

/************************************************/
/**********the code below is for PEBBLE**********/
/************************************************/

Pebble::Pebble(StudentWorld* sw, int startX, int startY)
:Actor(sw, startX, startY, right, IID_ROCK, 1)
{
    //constructs pebble with image IID_ROCK, facing right, at location, and depth of 1 via initializer list
}

bool Pebble::blocksMovement() const   //overrides the actor blockAnt that returns false since Pebble DOES block
{
    return true;
}

Pebble::~Pebble()
{
    //destructor for pebble, nothing needed here since nothing dynamically created by pebble
}
void Pebble::doSomething()
{
    //pebble does nothing, it's a pebble
}

/************************************************/
/**********the code below is for EHOLDER*********/
/************************************************/

EnergyHolder::EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth)
:Actor(sw, startX, startY, startDir, imageID, depth)
{
    m_hitPts = energy;
}

EnergyHolder::~EnergyHolder()
{
    //nothing to do
}

void EnergyHolder::updateEnergy(int amt)
{
    m_hitPts += amt;
}

int EnergyHolder::getEnergy() const
{
    return m_hitPts;
}

bool EnergyHolder::isDead() const
{
    return (m_hitPts <= 0);
}

int EnergyHolder::pickupAndEatFood(int amt)
{
    int x = this->getX();
    int y = this->getY();
    
    Actor* temp = this->getWorld()->getEdibleAt(x, y);
    
    if (temp == nullptr)
        return 0;           //no food there!
    
    if (temp->getEnergy() - amt < 0)
    {
        int ret = temp->getEnergy();
        temp->updateEnergy(-1*getEnergy());
        updateEnergy(ret);
        return ret;
    }
    else if(temp->getEnergy() - amt >= 0)
    {
        temp->updateEnergy(-1*amt);
        updateEnergy(amt);
        return amt;
    }

    return 0;
}

int EnergyHolder::pickupFood(int amt)
{
    int x = this->getX();
    int y = this-> getY();
    Actor* temp= this-> getWorld()->getEdibleAt(x, y);
    
    if (temp == nullptr)
        return 0;
    
    if (temp->getEnergy() - amt < 0)
    {
        int ret = temp->getEnergy();
        temp->updateEnergy(-1*getEnergy());
        return ret;
    }
    else if (temp->getEnergy() - amt >= 0)
    {
        temp->updateEnergy(-1*amt);
        return amt;
    }
    
    return 0;
}

void EnergyHolder::addFood(int amt)
{
    int x = this->getX();
    int y = this->getY();
    
    Actor* temp = this->getWorld()->getEdibleAt(x, y);
    
    if (temp == nullptr)    //no food object at that location yet, so we need to make one
    {
        getWorld()->createFood(x, y);
        return;
    }
    
    temp->updateEnergy(amt);    //otherwise there already is food there and we just add the amount (100)
    
}


/************************************************/
/**********the code below is for INSECT**********/
/************************************************/

Insect::Insect(StudentWorld* world, int startX, int startY, int energy, int imageID)
:EnergyHolder(world, startX, startY, none, energy, imageID, 1)
{
    // no direction initally, depth of 1 for all insects
    Direction direct = getRandDirect();
    setDirection(direct);
    m_dist = 0;
    m_sleepTicks = 0;
}

Insect::~Insect()
{
    //does nothing;
}

int Insect::getDist() const
{
    return m_dist;
}

void Insect::setDist(int dist)
{
    m_dist = dist;
}

void Insect::increaseSleepTicks(int amt)
{
    m_sleepTicks = m_sleepTicks + amt;
}

int Insect::getSleepTicks() const
{
    return m_sleepTicks;
}

bool Insect::getStunState() const
{
    return m_stunnedHere;
}

void Insect::setStunState(bool stun)
{
    m_stunnedHere = stun;
}

void Insect::getPoisoned()
{
    this->updateEnergy(-150);
}

void Insect::getStunned()
{                                                       //checks sleep ticks to make sure it moved away previously
    if(this->getStunState() == false && this->getSleepTicks() == 0)  //as long as it hasnt been stunned at this location before
    {
        this->setStunState(true);   //mark as has been stunned
        this->increaseSleepTicks(2);  //increase sleep ticks by 2
    }
}

void Insect::getBitten(int amt)
{
    this->updateEnergy(-1*amt);
}

void Insect::setColony(int col)
{
    m_colony = col;
}

int Insect::getColonyHelp() const
{
    return m_colony;
}

bool Insect::isEnemy(int colony)
{
    return (getColonyHelp() != colony);
}

bool Insect::isDangerous(int colony) const
{
    return (m_colony != colony);
}


/************************************************/
/**********the code below is for ANT*************/
/************************************************/

Ant::Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program, int imageID)
:Insect(sw, startX, startY, 1500, imageID)
{
    m_colony = colony;
    holdFood = 0;
    instructCounter = 0;
    prevBit = false;
    setStunState(false);
    prevBlocked = false;
    randValue = 0;
    m_prog = program;
}

Ant::~Ant()
{
    //does nothing
}

void Ant::doSomething()
{
    updateEnergy(-1);
    
    int curX = getX();
    int curY = getY();
    int commandCount = 0;
    Direction direct;
    Actor* temp;
    
    if (isDead())
    {
        addFood(100);
        return;
    }
    
    if (getSleepTicks() > 0)   //when sleep ticks is 0 that means it can move
    {
        increaseSleepTicks(-1); //decrease sleep Ticks by 1
        return;
    }

while(commandCount < 10)
{
    Compiler::Command cmd;
    if (! m_prog->getCommand(instructCounter, cmd))
    {
        updateEnergy(-1*getEnergy());
        return;
    }
    
    switch (cmd.opcode)
    {
        case Compiler::moveForward:
            curX = getX();
            curY = getY();
            if (setDirect(getDirection(), curX, curY, 1))
            {
                moveTo(curX, curY);
                prevBlocked = false;
                prevBit = false;
            }
            else
            {
                prevBlocked = true;
            }
            break;
        case Compiler::eatFood:
            if (holdFood - 100 >= 0)
            {
                holdFood -= 100;
                updateEnergy(100);
            }
            if (holdFood - 100 < 0)
            {
                updateEnergy(holdFood);
                holdFood = 0;
            }
            break;
        case Compiler::dropFood:
            addFood(100);
            break;
        case Compiler::bite:
            getWorld()->biteEnemyAt(this, m_colony, 15);
            break;
        case Compiler::pickupFood:
            if (holdFood + 400 <= 1800)
            {
                holdFood = pickupFood(400);
            }
            if (holdFood + 400 > 1800)
            {
                int amt = 1800-holdFood;
                holdFood = pickupFood(amt);
            }
            break;
        case Compiler::emitPheromone:
            curX = getX();
            curY = getY();
            temp = getWorld()->getPheromoneAt(curX, curY, m_colony);
            if (temp == nullptr)
            {
                getWorld()->makePheromone(curX, curY, m_colony);
            }
            updateEnergy(256);
            break;
        case Compiler::faceRandomDirection:
            direct = getRandDirect();
            setDirection(direct);
            break;
        case Compiler::rotateClockwise:
            switch (getDirection())
            {
                case up:
                    setDirection(right);
                    break;
                case right:
                    setDirection(down);
                    break;
                case down:
                    setDirection(left);
                    break;
                case left:
                    setDirection(up);
                    break;
                default:
                    break;
            }
            break;
        case Compiler::rotateCounterClockwise:
            switch(getDirection())
            {
                case up:
                    setDirection(left);
                    break;
                case left:
                    setDirection(down);
                    break;
                case down:
                    setDirection(right);
                    break;
                case right:
                    setDirection(up);
                    break;
                default:
                    break;
            }
            break;
        case Compiler::generateRandomNumber:
            if (stoi(cmd.operand1) == 0)
                randValue = 0;
            else
                randValue = randInt(0, stoi(cmd.operand1)-1);
            break;
        case Compiler::goto_command:
            instructCounter = stoi(cmd.operand1);
            break;
        case Compiler::if_command:
            switch(stoi(cmd.operand1))
            {
                case Compiler::Condition::last_random_number_was_zero:
                    if(randValue == 0)
                        instructCounter = stoi(cmd.operand2);
                    else
                        instructCounter++;
                    break;
                case Compiler::Condition::i_am_carrying_food:
                    if (holdFood > 0)
                        instructCounter = stoi(cmd.operand2);
                    else
                        instructCounter++;
                    break;
                case Compiler::Condition::i_am_hungry:
                    if (getEnergy() <= 25)
                        instructCounter = stoi(cmd.operand2);
                    else
                        instructCounter++;
                    break;
                case Compiler::Condition::i_am_standing_with_an_enemy:
                    curX = getX();
                    curY = getY();
                    if (getWorld()->isEnemyAt(curX, curY, m_colony))
                        instructCounter = stoi(cmd.operand2);
                    else
                        instructCounter++;
                    break;
                case Compiler::Condition::i_am_standing_on_food:
                    curX = getX();
                    curY = getY();
                    if(getWorld()->getEdibleAt(curX, curY) != nullptr)
                    {
                        if(!getWorld()->getEdibleAt(curX, curY)->isDead())  //if it is not dead
                            instructCounter = stoi(cmd.operand2);
                    }
                    else
                        instructCounter++;
                    break;
                case Compiler::Condition::i_am_standing_on_my_anthill:
                    curX = getX();
                    curY = getY();
                    if(getWorld()->isAntHillAt(curX, curY, m_colony))
                        instructCounter = stoi(cmd.operand2);
                    else
                        instructCounter++;
                    break;
                case Compiler::Condition::i_smell_pheromone_in_front_of_me:
                    curX = getX();
                    curY = getY();
                    direct = getDirection();
                    setDirect(direct, curX, curY, 1);
                    if(getWorld()->getPheromoneAt(curX, curY, m_colony) != nullptr)
                        instructCounter = stoi(cmd.operand2);
                    else
                        instructCounter++;
                    break;
                case Compiler::Condition::i_smell_danger_in_front_of_me:
                    curX = getX();
                    curY = getY();
                    direct = getDirection();
                    setDirect(direct, curX, curY, 1);
                    if(getWorld()->isDangerAt(curX, curY, m_colony))
                        instructCounter = stoi(cmd.operand2);
                    else
                        instructCounter++;
                    break;
                case Compiler::Condition::i_was_bit:
                    if(prevBit == true)
                        instructCounter = stoi(cmd.operand2);
                    else
                        instructCounter++;
                    break;
                case Compiler::Condition::i_was_blocked_from_moving:
                    if(prevBlocked == true)
                        instructCounter = stoi(cmd.operand2);
                    else
                        instructCounter++;
                default:
                    break;
                }
            break;
        default:
            break;
    }
    if((cmd.opcode != Compiler::goto_command)&&(cmd.opcode != Compiler::if_command))
        instructCounter++;
    
    if((cmd.opcode != Compiler::goto_command)&&(cmd.opcode != Compiler::if_command) && (cmd.opcode != Compiler::generateRandomNumber))
        return;
    else
        commandCount++;
    
    
//end of while loop!
}
//end of while loop!
    
}

void Ant::getBitten(int amt)
{
    updateEnergy(-1*amt);
    prevBit = true;
}

bool Ant::isEnemy(int colony) const
{
    return (m_colony != colony);
}

/************************************************/
/**********the code below is for GRASS***********/
/************************************************/

Grasshopper::Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID)
:Insect(sw, startX, startY, energy, imageID)
{
  int dist = randInt(2, 10);
  setDist(dist);
}

Grasshopper::~Grasshopper()
{
    
}

/************************************************/
/*******the code below is for ADULTGRASS*********/
/************************************************/

AdultGrasshopper::AdultGrasshopper(StudentWorld* sw, int startX, int startY)
:Grasshopper(sw, startX, startY, 1600, IID_ADULT_GRASSHOPPER)
{
}

AdultGrasshopper::~AdultGrasshopper()
{
    //nothing
}

void AdultGrasshopper::doSomething()
{
    updateEnergy(-1);
    
    int curX = getX();
    int curY = getY();

    if (isDead())
    {
        addFood(100);
        return;
    }
    
    if (getSleepTicks() > 0)   //when sleep ticks is 0 that means it can move
    {
        increaseSleepTicks(-1); //decrease sleep Ticks by 1
        return;
    }
    
    
    int biteProb = randInt(1, 3);
    if (biteProb == 1)
    {
        getWorld()->biteEnemyAt(this, getColonyHelp(), 50);
        increaseSleepTicks(2);
        return;
    }
    
    //JUMP DOES NOT WORK!!!!
    /*
    int jumpProb = randInt(1, 10);
    if (jumpProb == 1)
    {
        int radians = randInt(1, 360);
        radians *= 3.14159/180;
        int radius = randInt(1, 10);
        int randX = radius*cos(radians);
        int randY = radius*sin(radians);
        int moveX = static_cast<int>(getX() + randX);
        int moveY = static_cast<int>(getY() + randY);
        
        if (getWorld()->canMoveTo(moveX, moveY))
            moveTo(moveX, moveY);
        else
            return;
        
        return;
    }
     */
    
    
    int foodie = pickupAndEatFood(200);
   // updateEnergy(foodie);
    if(foodie > 0)
    {
        int sleepOrNot = randInt(0, 1);
        if (sleepOrNot == 0)
        {
            increaseSleepTicks(2);
            return;
        }
    }
    
    if (getDist() == 0)     //if remaining distance to move is 0
    {
        Direction direct = getRandDirect(); //get new direction
        setDirection(direct);               //set that new direction
        int dist = randInt(2, 10);          //get a new distance between 2-10
        setDist(dist);                      //set that distance
    }
    
    if (setDirect(getDirection(), curX, curY, getDist()))   //if we can move to that position
    {
        moveTo(curX, curY);         //actually make the move
        setStunState(false);          //set stunned here back to false so it doesn't repeat
        int newDist = getDist() - 1;    //distance it wants subtract 1 since it moved
        setDist(newDist);               //set the new distance
    }
    else
    {
        setDist(0);         //if we cant move, we set distance to 0 to get a new direction
    }
    increaseSleepTicks(2); //if it gets to this line, sleepticks is at 0, and we add 2
}

void AdultGrasshopper::getPoisoned()
{
    //does nothing, overrides the -150 in insect
}

void AdultGrasshopper::getStunned()
{
    //does nothing, overrides the stun in insect
}

void AdultGrasshopper::getBitten(int amt)
{
    this->updateEnergy(-1*amt);
    if(!isDead())
    {
        int biteProb = randInt(1, 2);
        if (biteProb == 1)
        {
            getWorld()->biteEnemyAt(this, -5, 50);
        }
    }
}

bool AdultGrasshopper::isEnemy(int colony) const
{
    return true;
}

/************************************************/
/************the code below is for BABY**********/
/************************************************/

BabyGrasshopper::BabyGrasshopper(StudentWorld* sw, int startX, int startY)
:Grasshopper(sw, startX, startY, 500, IID_BABY_GRASSHOPPER)
{

}

BabyGrasshopper::~BabyGrasshopper()
{
    //does nothing
}

void BabyGrasshopper::doSomething()
{
    updateEnergy(-1);
    
    int curX = getX();
    int curY = getY();
   
    if (isDead())
    {
        addFood(100);
        return;
    }
    
    if (getSleepTicks() > 0)   //when sleep ticks is 0 that means it can move
    {
        increaseSleepTicks(-1); //decrease sleep Ticks by 1
        return;
    }
    
    if (getEnergy() >= 1600)
    {
        getWorld()->makeAdultGrass(curX, curY);
        updateEnergy(-1*getEnergy());             //basically sets it's energy to 0 so that it is dead
        addFood(100);
        return;
    }
    
    int foodie = pickupAndEatFood(200);
 //   updateEnergy(foodie);
    if(foodie > 0)
    {
        int sleepOrNot = randInt(0, 1);
        if (sleepOrNot == 0)
        {
            increaseSleepTicks(2);
            return;
        }
    }
    
    if (getDist() == 0)     //if remaining distance to move is 0
    {
        Direction direct = getRandDirect(); //get new direction
        setDirection(direct);               //set that new direction
        int dist = randInt(2, 10);          //get a new distance between 2-10
        setDist(dist);                      //set that distance
    }
    
    if (setDirect(getDirection(), curX, curY, getDist()))   //if we can move to that position
    {
        moveTo(curX, curY);         //actually make the move
        setStunState(false);          //set stunned here back to false so it doesn't repeat
        int newDist = getDist() - 1;    //distance it wants subtract 1 since it moved
        setDist(newDist);               //set the new distance
    }
    else
    {
        setDist(0);         //if we cant move, we set distance to 0 to get a new direction
    }
    increaseSleepTicks(2); //if it gets to this line, sleepticks is at 0, and we add 2
}

bool BabyGrasshopper::isEnemy(int colony) const
{
    return true;
}

/************************************************/
/************the code below is for FOOD**********/
/************************************************/

Food::Food(StudentWorld* sw, int startX, int startY, int energy)
:EnergyHolder(sw, startX, startY, right, energy, IID_FOOD, 2)
{
    
}

Food::~Food()
{
    //does nothing
}

bool Food::isEdible() const
{
    return true; //it is edible
    //so in student world, if isEdible (it is food) then we can add to it, if not we make a new food.
}

void Food::doSomething()
{
    //does nothing
}

/************************************************/
/**********the code below is for TRIGGER*********/
/************************************************/

TriggerableActor::TriggerableActor(StudentWorld* sw, int x, int y, int imageID)
:Actor(sw, x, y, right, imageID, 2)
{
    
}

TriggerableActor::~TriggerableActor()
{
    //does nothing
}

bool TriggerableActor::isDangerous(int colony) const
{
    return true;
}

/************************************************/
/***********the code below is for WATER**********/
/************************************************/

WaterPool::WaterPool(StudentWorld* sw, int x, int y)
:TriggerableActor(sw, x, y, IID_WATER_POOL)
{
    
}

WaterPool::~WaterPool()
{
    //does nothing
}

void WaterPool::doSomething()
{
    getWorld()->stunAllStunnableAt(getX(), getY());
    //must implement stun all stunnable at
    //need to implement getStunned
}

/************************************************/
/**********the code below is for POISON**********/
/************************************************/

Poison::Poison(StudentWorld* sw, int x, int y)
:TriggerableActor(sw, x,y, IID_POISON)
{
    
}

Poison::~Poison()
{
    
}

void Poison::doSomething()
{
    getWorld()->poisonAllPoisonableAt(getX(), getY());
}

/************************************************/
/************the code below is for HILL**********/
/************************************************/

AntHill::AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program)
:EnergyHolder(sw, startX, startY, right, 8999, IID_ANT_HILL, 2)
{
    m_prog = program;
    m_colony = colony;
}

AntHill::~AntHill()
{
    delete m_prog;
}

void AntHill::doSomething()
{
    updateEnergy(-1);
    
    if (isDead())
    {
        return;
    }
    
    Actor* checkFood = getWorld()->getEdibleAt(getX(), getY());
    if (checkFood != nullptr)
    {
        int foodie = pickupAndEatFood(10000);
  //      updateEnergy(foodie);
        return;
    }
    
    if (getEnergy() >= 2000)
    {
        int x = getX();
        int y = getY();
        Actor* a;
        a = nullptr;
        switch(m_colony)
        {
            case 0:
                a = new Ant(getWorld(), x, y, m_colony, m_prog, IID_ANT_TYPE0);
                break;
            case 1:
                a = new Ant(getWorld(), x, y, m_colony, m_prog, IID_ANT_TYPE1);
                break;
            case 2:
                a = new Ant(getWorld(), x, y, m_colony, m_prog, IID_ANT_TYPE2);
                break;
            case 3:
                a = new Ant(getWorld(), x, y, m_colony, m_prog, IID_ANT_TYPE3);
                break;
        }
        if (a != nullptr)
            getWorld()->addActor(a);
        updateEnergy(-1500);
        getWorld()->increaseScore(m_colony);
    }

}

bool AntHill::isAntHill(int colony) const
{
    return (m_colony == colony);
}

/************************************************/
/**********the code below is for PHEROM**********/
/************************************************/

Pheromone::Pheromone(StudentWorld* sw, int startX, int startY, int colony, int imageID)
:EnergyHolder(sw, startX, startY, right, 256, imageID, 2)
{
    m_colony = colony;
}

Pheromone::~Pheromone()
{
    //does nothing
}

void Pheromone::doSomething()
{
    updateEnergy(-1);
}

bool Pheromone::isPheromone(int colony) const
{
    return (m_colony == colony);
}

void Pheromone::increaseStrength()
{
    if (getEnergy() + 256 <= 768)
    {
        updateEnergy(256);
    }
    else if(getEnergy() + 256 > 768)
    {
        int max = 768 - getEnergy();
        updateEnergy(max);
    }
    //NEEDS TO BE IMPLEMENTED STILL!!!!
}

void Pheromone::updateEnergy(int amt)
{
    increaseStrength();
}
