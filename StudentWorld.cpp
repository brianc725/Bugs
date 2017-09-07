#include "StudentWorld.h"
#include "Compiler.h"
#include <string>
#include <list>
#include <algorithm>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::~StudentWorld()
{
    cleanUp();  //destructor calls our cleanup function so no duplicaiton
}

int StudentWorld::init()
{
    m_ticks = 0;
    m_ants = 0;
    score0 = 0;
    score1 = 0;
    score2 = 0;
    score3 = 0;
    winningAntNumber = 5;
    winCount = 0;
    
    compilerForEntrant0 = nullptr;
    compilerForEntrant1 = nullptr;
    compilerForEntrant2 = nullptr;
    compilerForEntrant3 = nullptr;
    
    AntHill *ah0, *ah1, *ah2, *ah3;
    
    m_antNames = getFilenamesOfAntPrograms();
    
    string error;

    int howManyToComp = m_antNames.size();
    numFiles = howManyToComp;
    
    switch(howManyToComp)
    {
        case 1:
            compilerForEntrant0 = new Compiler;
            if (! compilerForEntrant0->compile(m_antNames[0], error))
            {
                setError(m_antNames[0] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            break;
        case 2:
            compilerForEntrant0 = new Compiler;
            if (! compilerForEntrant0->compile(m_antNames[0], error))
            {
                setError(m_antNames[0] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            compilerForEntrant1 = new Compiler;
            if (! compilerForEntrant1->compile(m_antNames[1], error))
            {
                setError(m_antNames[0] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            break;
        case 3:
            compilerForEntrant0 = new Compiler;
            if (! compilerForEntrant0->compile(m_antNames[0], error))
            {
                setError(m_antNames[0] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            compilerForEntrant1 = new Compiler;
            if (! compilerForEntrant1->compile(m_antNames[1], error))
            {
                setError(m_antNames[0] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            compilerForEntrant2 = new Compiler;
            if (! compilerForEntrant2->compile(m_antNames[2], error))
            {
                setError(m_antNames[2] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            break;
        case 4:
            compilerForEntrant0 = new Compiler;
            if (! compilerForEntrant0->compile(m_antNames[0], error))
            {
                setError(m_antNames[0] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            compilerForEntrant1 = new Compiler;
            if (! compilerForEntrant1->compile(m_antNames[1], error))
            {
                setError(m_antNames[0] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            compilerForEntrant2 = new Compiler;
            if (! compilerForEntrant2->compile(m_antNames[2], error))
            {
                setError(m_antNames[2] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            compilerForEntrant3 = new Compiler;
            if (! compilerForEntrant3->compile(m_antNames[3], error))
            {
                setError(m_antNames[3] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            break;
        default:
            break;
    }
    
    Field f;
    string fieldFile = getFieldFilename();
    string error2;
    
    //checks for an error
    if (f.loadField(fieldFile, error2) != Field::LoadResult::load_success)
    {
        setError(fieldFile + " " + error2);
        return GWSTATUS_LEVEL_ERROR;
    }
    
    //inserts items onto the map
    for (int y = 0; y < VIEW_HEIGHT; y++)
    {
        for (int x = 0; x < VIEW_WIDTH; x++)
        {
            Field::FieldItem pebb = f.getContentsOf(x, y); //x, y NOT y, x
            if (pebb == Field::FieldItem::rock)
            {
                Actor *a = new Pebble(this, x, y);
                map1[y][x].push_back(a);
            }
            
            Field::FieldItem grass = f.getContentsOf(x, y);
            if (grass == Field::FieldItem::grasshopper)
            {
                Actor *a = new BabyGrasshopper(this, x, y);
                map1[y][x].push_back(a);
            }
            
            Field::FieldItem water = f.getContentsOf(x, y);
            if(water == Field::FieldItem::water)
            {
                Actor *a = new WaterPool(this, x, y);
                map1[y][x].push_back(a);
            }
            
            Field::FieldItem pois = f.getContentsOf(x, y);
            if (pois == Field::FieldItem::poison)
            {
                Actor *a = new Poison(this, x, y);
                map1[y][x].push_back(a);
            }
            
            Field::FieldItem food = f.getContentsOf(x, y);
            if (food == Field::FieldItem::food)
            {
                Actor *a = new Food(this, x, y, 6000);
                map1[y][x].push_back(a);
            }
        
            Field::FieldItem hill0 = f.getContentsOf(x, y);
            if(hill0 == Field::FieldItem::anthill0)
            {
                if(compilerForEntrant0 != nullptr)
                {
                    Actor *a = new AntHill(this, x, y, 0, compilerForEntrant0);
                    ah0 = static_cast<AntHill*>(a);
                    map1[y][x].push_back(a);
                }
            }
            
            Field::FieldItem hill1 = f.getContentsOf(x, y);
            if(hill1 == Field::FieldItem::anthill1)
            {
                if(compilerForEntrant1 != nullptr)
                {
                    Actor *a = new AntHill(this, x, y, 1, compilerForEntrant1);
                    ah1 = static_cast<AntHill*>(a);
                    map1[y][x].push_back(a);
                }
            }
            
            Field::FieldItem hill2 = f.getContentsOf(x, y);
            if (hill2 == Field::FieldItem::anthill2)
            {
                if(compilerForEntrant2 != nullptr)
                {
                    Actor *a = new AntHill(this, x, y, 2, compilerForEntrant2);
                    ah2 = static_cast<AntHill*>(a);
                    map1[y][x].push_back(a);
                }
            }
            
            Field::FieldItem hill3 = f.getContentsOf(x, y);
            if (hill3 == Field::FieldItem::anthill3)
            {
                if (compilerForEntrant3 != nullptr)
                {
                    Actor *a = new AntHill(this, x, y, 3, compilerForEntrant3);
                    ah3 = static_cast<AntHill*>(a);
                    map1[y][x].push_back(a);
                }
            }
        }
    }

    m_fieldName = getFieldFilename();
    
    switch (numFiles)
    {
        case 1:
            m_editedNames.push_back(compilerForEntrant0->getColonyName());
            break;
        case 2:
            m_editedNames.push_back(compilerForEntrant0->getColonyName());
            m_editedNames.push_back(compilerForEntrant1->getColonyName());
            break;
        case 3:
            m_editedNames.push_back(compilerForEntrant0->getColonyName());
            m_editedNames.push_back(compilerForEntrant1->getColonyName());
            m_editedNames.push_back(compilerForEntrant2->getColonyName());
            break;
        case 4:
            m_editedNames.push_back(compilerForEntrant0->getColonyName());
            m_editedNames.push_back(compilerForEntrant1->getColonyName());
            m_editedNames.push_back(compilerForEntrant2->getColonyName());
            m_editedNames.push_back(compilerForEntrant3->getColonyName());
    }
    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_ticks++;
   
    for (int y = 0; y < VIEW_HEIGHT; y++)
    {
        for (int x = 0; x < VIEW_WIDTH; x++)
        {
            for(list<Actor*>::iterator it = map1[y][x].begin(); it != map1[y][x].end(); )
            {
                int tempX = (*it)->getX();
                int tempY = (*it)->getY();
                
                //NORMALLY WE WOULD CHECK TO MAKE SURE THE ELEMENT IS STILL ALIVE!!!!!!!!!!!!!!!
                (*it)->doSomething();
                
                if ((x != tempX)||(y != tempY))     //if the item is at a new position
                {
                    map1[tempY][tempX].push_back(*it);  //adds the element at the new position
                    it = map1[y][x].erase(it);          //erases the old one at the old position
                }
                else
                    it++;
            }
        }
    }
    
    for (int y = 0; y < VIEW_HEIGHT; y++)
    {
        for (int x = 0; x < VIEW_WIDTH; x++)
        {
            for(list<Actor*>::iterator it = map1[y][x].begin(); it != map1[y][x].end(); )
            {
                if ((*it)->isDead())
                {
                    delete (*it);
                    it = map1[y][x].erase(it);
                }
                else
                    it++;
            }
        }
    }
    
    updateDisplayText();
    
    
    if(m_ticks == 2000)
    {
        if((winCount != 0)&&(winningAntNumber > 5))
        {
            switch(winCount)
            {
                case 1:
                    setWinner(m_editedNames[0]);
                    break;
                case 2:
                    setWinner(m_editedNames[1]);
                    break;
                case 3:
                    setWinner(m_editedNames[2]);
                    break;
                case 4:
                    setWinner(m_editedNames[3]);
                    break;
            }
            return GWSTATUS_PLAYER_WON;
        }
        else
            return GWSTATUS_NO_WINNER;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (int y = 0; y < VIEW_HEIGHT; y++)
    {
        for (int x = 0; x < VIEW_WIDTH; x++)
        {
            for (list<Actor*>::iterator it = map1[y][x].begin(); it != map1[y][x].end(); )
            {
                delete(*it);
                it = map1[y][x].erase(it);
            }
        }
    }
}

void StudentWorld::updateDisplayText()
{
    int ticks = 2000 - m_ticks;
    int antsAnt0, antsAnt1, antsAnt2, antsAnt3;
    
    antsAnt0 = score0;
    antsAnt1 = score1;
    antsAnt2 = score2;
    antsAnt3 = score3;
    
    for (int i = 0; i < 4; i++)
    {
        if (antsAnt0 > winningAntNumber)    //needs to be bigger than 5 to be winning
        {
            winningAntNumber = antsAnt0;
            winCount = 1;
        }
        if (antsAnt1 > winningAntNumber)
        {
            winningAntNumber = antsAnt1;
            winCount = 2;
        }
        if (antsAnt2 > winningAntNumber)
        {
            winningAntNumber = antsAnt2;
            winCount = 3;
        }
        if (antsAnt3 > winningAntNumber)
        {
            winningAntNumber = antsAnt3;
            winCount = 4;
        }
    }
    
    string lead0 = "";
    string lead1 = "";
    string lead2 = "";
    string lead3 = "";
    
    switch(winCount)
    {
        case 1:
            lead0 = "*";
            break;
        case 2:
            lead1 = "*";
            break;
        case 3:
            lead2 = "*";
            break;
        case 4:
            lead3 = "*";
            break;
    }
    
    //if this is max, lead for that one is "*"
    //need to check for less than 4 files
    ostringstream oss;
    oss << "Ticks:" << setw(5) << ticks << " - ";
    oss.fill('0');
    switch(numFiles)
    {
        case 1:
            oss << m_editedNames[0] << lead0 << ": " << setw(2) << score0;
            break;
        case 2:
            oss << m_editedNames[0] << lead0 << ": " << setw(2) << score0;
            oss << "  ";
            oss << m_editedNames[1] << lead1 << ": " << setw(2) << score1;
            break;
        case 3:
            oss << m_editedNames[0] << lead0 << ": " << setw(2) << score0;
            oss << "  ";
            oss << m_editedNames[1] << lead1 << ": " << setw(2) << score1;
            oss << "  ";
            oss << m_editedNames[2] << lead2 << ": " << setw(2) << score2;
            break;
        case 4:
            oss << m_editedNames[0] << lead0 << ": " << setw(2) << score0;
            oss << "  ";
            oss << m_editedNames[1] << lead1 << ": " << setw(2) << score1;
            oss << "  ";
            oss << m_editedNames[2] << lead2 << ": " << setw(2) << score2;
            oss << "  ";
            oss << m_editedNames[3] << lead3 << ": " << setw(2) << score3;
            break;
    }
    
    string s = oss.str();
    setGameStatText(s);
    
}

bool StudentWorld::canMoveTo(int x, int y) const
{
    if (y < 0 || y > VIEW_HEIGHT || x < 0 || x > VIEW_WIDTH)
        return false;

    for(list<Actor*>::const_iterator it = map1[y][x].begin(); it != map1[y][x].end(); it++)
    {
        if ((*it) != nullptr)
        {
        if ((*it)->blocksMovement())    //if movement is blocked and cannot move due to pebble, return false
            return false;
        }
    }
    return true;
}

bool StudentWorld::stunAllStunnableAt(int x, int y)
{
    if (x < 0 || x > VIEW_WIDTH)
        return false;
    
    if (y < 0 || y > VIEW_HEIGHT)
        return false;

    
    for(list<Actor*>::iterator it = map1[y][x].begin(); it != map1[y][x].end(); it++)
    {
        (*it)->getStunned();
    }
    return true;
}

bool StudentWorld::poisonAllPoisonableAt(int x, int y)
{
    if (x < 0 || x > VIEW_WIDTH)
        return false;
    
    if (y < 0 || y > VIEW_HEIGHT)
        return false;

    
    for(list<Actor*>::iterator it = map1[y][x].begin(); it != map1[y][x].end(); it++)
    {
        (*it)->getPoisoned();
    }
    return true;
}

Actor* StudentWorld::getEdibleAt(int x, int y) const
{
    if (x < 0 || x > VIEW_WIDTH)
        return nullptr;
    
    if (y < 0 || y > VIEW_HEIGHT)
        return nullptr;

    
    for(list<Actor*>::const_iterator it = map1[y][x].begin(); it != map1[y][x].end(); it++)
    {
        if ((*it)->isEdible() && !(*it)->isDead())
        {
            return (*it);
        }
    }
    return nullptr;
}

Actor* StudentWorld::getPheromoneAt(int x, int y, int colony) const
{
    if (x < 0 || x > VIEW_WIDTH)
        return nullptr;
    
    if (y < 0 || y > VIEW_HEIGHT)
        return nullptr;

    
    for(list<Actor*>::const_iterator it = map1[y][x].begin(); it != map1[y][x].end(); it++)
    {
        if((*it)->isPheromone(colony))
        {
            return (*it);
        }
    }
    return nullptr;
}

void StudentWorld::makeAdultGrass(int x, int y)
{
    if (x < 0 || x > VIEW_WIDTH)
        return;
    
    if (y < 0 || y > VIEW_HEIGHT)
        return;

    
    Actor *a = new AdultGrasshopper(this, x, y);
    map1[y][x].push_back(a);
}

void StudentWorld::createFood(int x, int y)
{
    if (x < 0 || x > VIEW_WIDTH)
        return;
    
    if (y < 0 || y > VIEW_HEIGHT)
        return;

    Actor* a = new Food(this, x, y, 100);
    map1[y][x].push_back(a);
}

void StudentWorld::increaseScore(int colony)
{
    switch(colony)
    {
        case 0:
            score0++;
            break;
        case 1:
            score1++;
            break;
        case 2:
            score2++;
            break;
        case 3:
            score3++;
            break;
    }
}

// Bite an enemy of an ant of the indicated colony at me's location
// (other than me; insects don't bite themselves).  Return true if an
// enemy was bitten.
bool StudentWorld::biteEnemyAt(Actor* me, int colony, int biteDamage)
{
    int curX = me->getX();
    int curY = me->getY();
    
    vector<Actor*> biteable;
    
    for(list<Actor*>::const_iterator it = map1[curY][curX].begin(); it != map1[curY][curX].end(); it++)
    {
        if (isEnemyAt(curX, curY, colony) && (*it) != me)
        {
            
            biteable.push_back(*it);    //push back things that can be bitten
        }
    }
    
    if (biteable.size() == 0)   //if nothing can be bitten on that space
        return false;
    
    int randomBite = randInt(0, biteable.size()-1); //get random int from 0 to size-1 as our position of the thing
    biteable[randomBite]->getBitten(biteDamage);
    
    for(vector<Actor*>::iterator ite = biteable.begin(); ite != biteable.end(); )
    {
        ite = biteable.erase(ite);
    }
    
    return true;
}

bool StudentWorld::isEnemyAt(int x, int y, int colony) const
{
    if (x < 0 || x > VIEW_WIDTH)
        return false;
    
    if (y < 0 || y > VIEW_HEIGHT)
        return false;

    
    for(list<Actor*>::const_iterator it = map1[y][x].begin(); it != map1[y][x].end(); it++)
    {
        if((*it)->isEnemy(colony))
            return true;
    }
    return false;
}

void StudentWorld::addActor(Actor* a)
{
    map1[a->getY()][a->getX()].push_back(a);
}

void StudentWorld::makePheromone(int x, int y, int colony)
{
    if (x < 0 || x > VIEW_WIDTH)
        return;
    
    if (y < 0 || y > VIEW_HEIGHT)
        return;

    
    //Pheromone(StudentWorld* sw, int startX, int startY, int colony, int imageID)
    Actor *a;
    switch (colony)
    {
        case 0:
            a = new Pheromone(this, x, y, colony, IID_PHEROMONE_TYPE0);
            map1[y][x].push_back(a);
            break;
        case 1:
            a = new Pheromone(this, x, y, colony, IID_PHEROMONE_TYPE1);
            map1[y][x].push_back(a);
            break;
        case 2:
            a = new Pheromone(this, x, y, colony, IID_PHEROMONE_TYPE2);
            map1[y][x].push_back(a);
            break;
        case 3:
            a = new Pheromone(this, x, y, colony, IID_PHEROMONE_TYPE3);
            map1[y][x].push_back(a);
            break;
    }
}

bool StudentWorld::isAntHillAt(int x, int y, int colony) const
{
    if (x < 0 || x > VIEW_WIDTH)
        return false;
    
    if (y < 0 || y > VIEW_HEIGHT)
        return false;
    
    for(list<Actor*>::const_iterator it = map1[y][x].begin(); it != map1[y][x].end(); it++)
    {
        if ((*it)->isAntHill(colony) &&!(*it)->isDead())
            return true;
    }
    return false;
}

bool StudentWorld::isDangerAt(int x, int y, int colony) const
{
    if (x < 0 || x > VIEW_WIDTH)
        return false;
    
    if (y < 0 || y > VIEW_HEIGHT)
        return false;
    
    for(list<Actor*>::const_iterator it = map1[y][x].begin(); it != map1[y][x].end(); it++)
    {
        if ((*it)->isDangerous(colony))
            return true;
    }
    return false;
}
