/**
 * Player.h
 *
 * Description:
 *      -Player specific information is stored in this class.
 *      -This includes their age, weight, and sex
 *      -Biases are private and use setters/getters
 *      -Used by AI.cpp and main.cpp to ask questions
 *      -Fields can be added or removed to increase the information about a user
 *
 *      For example, float height could be addded to compare height biases
 *
 * Author: Evan Geissler
 * Date: April, 2018
 */

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
