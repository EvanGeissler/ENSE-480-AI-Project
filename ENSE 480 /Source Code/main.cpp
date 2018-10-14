/**
 * main.cpp
 *
 * Description:
 *      -Takes in information about a user
 *      -Asks user questions to try and form biases
 *      -Asks new questions based on current & past information
 *      -Tries to get predicte and get a user to answer certain way
 *
 * Author: Evan Geissler
 * Date: April, 2018
 */

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

#include "Player.h"
#include "AI.h"

using namespace std;

//How many questions the AI will ask
static const int MAX_NUM_QUESTIONS = 10;

void getPlayerInformation(Player &playerNew);

bool checkInput(clock_t time_start, string userIn);

void printInitialQuestions();
void printQuestionIntro();
void printQuestionMid();
void printQuestionEnd();

const static int MIN_QUESTION_TIME = 1;
int main(){
    //player is the original player that DOES NOT CHANGE
    Player player;
    
    //AI has a player that DOES change throughout
    AI january;
    
    //User Input
    int numIn;
    string stringIn;

    //Timer
    clock_t time_start;
    
    //Counter for num questions asked loop
    int qLoop = 0;
        
    cout << endl;
    cout << "Welcome! Please answer the following questions: " << endl;

/*---------------------------- STEP 1: Get player information ----------------------------*/
    getPlayerInformation(player);
    january.setPlayer(player);

    cout << endl;
       
/*--------------------------- STEP 2: Ask First Set of Questions -------------------------*/
    /* USE A TEXT FILE INSTEAD */
    /* POSSIBLY BOLD IMPORTANT PARTS. I.E. SEX */
    
    /* IMPORTANT NOTES */
        //Use actual numbers so it's easier to compare for player
        //Use a, b, etc. for options so user isn't possibly swayed by a number
        //List assumptions below (such as people will die)
    cout << "Please answer the following questions with honesty: " << endl << endl;
  
    //1 vs Many
    cout << "Initial 1. ";
    do{
        printQuestionIntro();
        cout << "1 person walks ";
        printQuestionMid();
        cout << "2 people";
        printQuestionEnd();
       
        time_start = clock();
        cin >> stringIn;

    }while(checkInput(time_start, stringIn));
    
    january.setContinueChoice(1);
    january.setSwerveChoice(2);
    january.setMorals(stringIn);
    cout << endl;
    
    //1 vs Weight
    cout << "Initial 2. ";
    do{
        printQuestionIntro();
        cout << "1 person walks ";
        printQuestionMid();
        cout << "a fat person";
        printQuestionEnd();
    
        time_start = clock();
        cin >> stringIn;
    
    }while(checkInput(time_start, stringIn));

    january.setContinueChoice(0);
    january.setSwerveChoice(21);
    january.setWeightBias(stringIn);
    cout << endl;
    
    //1 vs Age
    cout << "Initial 3. ";
    do{
        printQuestionIntro();
        cout << "1 person walks ";
        printQuestionMid();
        cout << "an old person";
        printQuestionEnd();
        
        time_start = clock();
        cin >> stringIn;
    
    }while(checkInput(time_start, stringIn));
   
    january.setContinueChoice(0);
    january.setSwerveChoice(11);
    january.setAgeBias(stringIn);
    cout << endl;
    
    cout << "Initial 4. ";
    do{
        //1 vs Sex
        printQuestionIntro();
        cout << "1 person walks ";
        printQuestionMid();
        cout << "a woman";
        printQuestionEnd();
        
        time_start = clock();
        
        cin >> stringIn;
    }while(checkInput(time_start, stringIn));

    january.setContinueChoice(0);
    january.setSwerveChoice(32);
    january.setSexBias(stringIn);
    cout << endl;
    
    cout << endl;

    //Save initial information
    january.saveBiases();
    
    cout << "INITIAL BIASES: " << endl;
    january.printCurrentResults();
    
/*---------------------- STEP 3: Load Previous Biases Based on Initial ---------------------*/
    //Loads any previous biases that can assist
    january.loadPreviousBiases();

/*---------------------------- STEP 4: Let AI Take Over Questions --------------------------*/
    cout << "STARTING AI QUESTIONING: " << endl;
    cout << endl;
    
    //Calculate morals/biases
    do {
        cout << "Q" << qLoop+1 << ". ";
            
        if(january.pickFirst() == false){
          january.pickChoices();
        }
    
        printQuestionIntro();
        january.printContinue();
        printQuestionMid();
        january.printSwerve();
        printQuestionEnd();
        
        do{
            time_start = clock();
            cin >> stringIn;
        }while(checkInput(time_start, stringIn));
       
        january.updateBiases(stringIn);
        cout << endl;
        
        january.recordAnswer(stringIn);
        january.resetVariables();

        cout << "CURRENT RESULTS: " << endl;
        january.printCurrentResults();
        
        qLoop += 1;
    } while (qLoop < MAX_NUM_QUESTIONS);

    cout << endl;

/*------------------------------ STEP 5: Display final results -----------------------------*/
    january.printResults();
    cout << endl;

/*----------------------------- STEP 6: Save Results in .txt File --------------------------*/
    january.saveBiasInformation();
    
}

/*------------------------------------------------------------------------------------------*/
/*---------------------------------------- END ---------------------------------------------*/
/*---------------------------------------- OF ----------------------------------------------*/
/*--------------------------------------- MAIN ---------------------------------------------*/
/*------------------------------------------------------------------------------------------*/





/*------------------------------------------------------------------------------------------*/
/*-------------------------------------- FUNCTIONS: ----------------------------------------*/
/*------------------------------------------------------------------------------------------*/

/*-------------------------------- Get player information ----------------------------------*/
void getPlayerInformation(Player &playerNew){
    int numIn;
    string stringIn;
    
    //Age
    cout << "What is your age? ";
    cin >> numIn;
    playerNew.setAge(numIn);
     
    while (!playerNew.checkAge()){
        //Generate random message
        cout << "What is your age? ";
        cin >> numIn;
        playerNew.setAge(numIn);
    }
     
     //Weight
     cout << "What is your weight (in lbs)? ";
     cin >> numIn;
     playerNew.setWeight(numIn);
     
     while (!playerNew.checkWeight()){
         //Generate random message
         cout << "What is your weight? ";
         cin >> numIn;
         playerNew.setWeight(numIn);
     }
     
     //Sex
     cout << "What is your sex (m or f)? ";
     cin >> stringIn;
     playerNew.setSex(stringIn);
     
     while (!playerNew.checkSex()){
         //Generate random message
         cout << "What is your sex (m or f)? ";
         cin >> stringIn;
         playerNew.setSex(stringIn);
     }
}


/*---------------------------------- Check Player Input ------------------------------------*/
bool checkInput(clock_t time_start, string userIn){
    clock_t end_time;
    end_time = clock() - time_start;

    if(end_time < MIN_QUESTION_TIME){
        return(true);
    }
    
    if((userIn[0] != 'a') && (userIn[0] != 'b')){
        userIn = "";
        return(true);
    }
    
    return(false);
}

/*------------------------------- Print Questions from Game --------------------------------*/
void printInitialQuestions(){
    
}

void printQuestionIntro(){
    cout << "A Self-Driving Car is heading down a road when ";
}

void printQuestionMid(){
    cout << "infront of the car." << endl;
    cout << "The car will either continue its path OR the car can swerve off the road hitting ";
}

void printQuestionEnd(){
    cout << ". Should the car: " << endl;
    cout << "   a. Continue its course OR" << endl;
    cout << "   b. Swerve off the road?" << endl;
    cout << "ANSWER: ";
}


/*------------------------------------------------------------------------------------------*/
/*---------------------------------------- END ---------------------------------------------*/
/*---------------------------------------- OF ----------------------------------------------*/
/*------------------------------------- FUNCTIONS ------------------------------------------*/
/*------------------------------------------------------------------------------------------*/
