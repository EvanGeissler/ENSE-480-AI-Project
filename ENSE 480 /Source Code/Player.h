#include <iostream> 
#include <string>

#ifndef __PLAYER_H__
#define __PLAYER_H__

using namespace std;

class Player{
private:
    
    int age;
    int weight;
    string sex;
    
public:

    //Constructors
    Player();
    Player(const Player& original);
    
    //Override Functions
    Player& operator= (const Player& original);
    
    //Setters and getters
    int getAge();
    int getWeight();
    string getSex();

    void setAge(int age);
    void setWeight(int weight);
    void setSex(string sex);

    
    /*checkAge
     /Purpose: Ensure player is between 3 and 125
     /Parameters:
     /Returns: True if met
     */
    bool checkAge();
    
    /*checkWeight
     /Purpose: Ensure player is between 25lbs and 500lbs
     /Parameters:
     /Returns: True if met
     */
    bool checkWeight();
    
    /*checkSex
     /Purpose: Ensure player is male or female (for simplicity)
     /Parameters:
     /Returns: True if met
     */
    bool checkSex();
};

#endif
