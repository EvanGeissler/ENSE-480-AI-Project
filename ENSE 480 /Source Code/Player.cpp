#include <iostream>
#include <string>

#include "Player.h"

Player::Player(){
    age = 0;
    weight = 0;
    sex = "";
}

Player::Player(const Player& original){
    age = original.age;
    weight = original.weight;
    sex = original.sex;
    
}

Player& Player::operator= (const Player& original){
    if(this != &original){
        age = original.age;
        weight = original.weight;
        sex = original.sex;
        
        return(*this);
    }
    
    return(*this);
}

int Player::getAge(){
    return(age);
}

int Player::getWeight(){
    return(weight);
}

string Player::getSex(){
    return(sex);
}

void Player::setAge(int age){
    this->age = age;
}

void Player::setWeight(int weight){
    this->weight = weight;
}

void Player::setSex(string sex){
    this->sex[0] = sex[0];
}

bool Player::checkAge(){
    if(age < 3 || age > 100){
        return false;
    }
    
    return true;
}

bool Player::checkWeight(){
    if(weight < 25 || weight > 500){
        return false;
    }
    
    return true;
}

bool Player::checkSex(){
    if((sex[0] != 'f' && sex[0] != 'F') && (sex[0] != 'm' && sex[0] != 'M')){
        return false;
    }
    
    return true;
}
