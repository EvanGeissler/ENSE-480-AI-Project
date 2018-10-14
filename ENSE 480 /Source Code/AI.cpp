#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include "AI.h"

using namespace std;

/*-----------------------------------------------------------------------------------------*/
/*-------------------------------------- CONSTRUCTOR --------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
AI::AI():player(){

    for(int i = 0; i < NUM_CAT_ROWS; i++){
        for(int j = 0; j < MAX_BIAS_COLS; j++){
            biases[i][j] = -1;
        }
    }
    
    setToNeutral();
    
    askedList[0] = -1;
    askedIndex = 0;
    
    pickedList[0] = -1;
    pickedIndex = 0;
    
    successRateIndex = 0;

    domBias = 0;
    leastBiased = 1;
    
    continueChoice = 0;
    swerveChoice = 0;
    biasPicked = 0;
    
    waitFor = 5;
    
    overRide = false;
    waitOverRide = false;
    pickFirstFlag = true;
}


/*-----------------------------------------------------------------------------------------*/
/*-------------------------------------- INITIALIZER --------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
void AI::setToNeutral(){
    //Sets all bias spots used to neutral (0.5)
    for(int i = 0; i < NUM_BIASES; i++){
        biases[biasInfo[i].code/10][biasInfo[i].code%10] = 0.5;
    }
}


/*-----------------------------------------------------------------------------------------*/
/*---------------------------------- LOAD PREVIOUS BIAS -----------------------------------*/
/*-----------------------------------------------------------------------------------------*/
void AI::loadPreviousBiases(){
    ifstream previousBiases1 ("previousBiases.txt");
    
    if(previousBiases1.is_open()){
        for(int i = 0; i < NUM_Q_POSSIBILITIES; i++){
            for(int j = 0; j < 4; j++){
                previousBiases1 >> previousBiasInfo[i][j];
            }
        }
    }
    
    previousBiases1.close();
}


/*-----------------------------------------------------------------------------------------*/
/*--------------------------------- SAVES BIAS INFORMATION --------------------------------*/
/*-----------------------------------------------------------------------------------------*/
void AI::saveBiasInformation(){
    ofstream previousBiases ("previousBiases.txt", ios::trunc | ios::out);
    
    int temp = 0;
    int tempPicked = 0;
    int counter = 0;
    int loopFlag = 0;
    bool found;
    bool noMoreEntries;
    
    //Go through all values in askedList
    do{
        found = false;
        noMoreEntries = false;
        loopFlag = 0;
        
        //Go through all values in previousBiasInfo if needed
        do{
            if((previousBiasInfo[counter][0] == askedList[temp]) && (previousBiasInfo[counter][1] == askedList[temp+1])){
                found = true;
            }else if(previousBiasInfo[counter][0] == -1){
                noMoreEntries = true;
            }

            loopFlag += 1;
        }while((loopFlag < NUM_Q_POSSIBILITIES) && (found == false) && (noMoreEntries == false));

        //Add to previous
        if(found == false && noMoreEntries == true){
            //Add new combination of questions
            previousBiasInfo[counter][0] = askedList[temp];
            previousBiasInfo[counter][1] = askedList[temp+1];

            //Add new picked and % percentage picked
            previousBiasInfo[counter][2] = 1;
            previousBiasInfo[counter][3] = pickedList[tempPicked];
        }else{
            previousBiasInfo[counter][2] += 1;
            //((% * (people -1)) + answer)/people
            previousBiasInfo[counter][3] = (((previousBiasInfo[counter][3] * (previousBiasInfo[counter][2] - 1)) + pickedList[tempPicked])/previousBiasInfo[counter][2]);
        }
        
        //counter = 0;
        temp += 2;
        tempPicked += 1;
        
        found = false;
        noMoreEntries = false;
        counter += 1;

    }while(temp < askedIndex);
    
    //Sorts biases by most efficient first
    sortBiases();
    
    for(int i = 0; i < NUM_Q_POSSIBILITIES; i++){
        for(int j = 0; j < 4; j++){
            previousBiases << previousBiasInfo[i][j];
           // previousBiases << -1;
            previousBiases << " ";
        }
        previousBiases << "\n";
    }
    
    previousBiases.close();
}


/*-----------------------------------------------------------------------------------------*/
/*---------------------------------------- SORTING ----------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
void AI::sortBiases(){
    int tempValue;
    bool swapped;
    
    for(int i = 0; i < NUM_Q_POSSIBILITIES - 1; i++){
        swapped = false;
        for(int j = 0; j < NUM_Q_POSSIBILITIES - i - 1; j++){
            if(previousBiasInfo[j+1][3] == -1){
                break;
            }else if(previousBiasInfo[j][3] < previousBiasInfo[j+1][3]){
                
                //Ensure ALL values assosiated are swapped
                for(int k = 0; k < 4; k++){
                    tempValue = previousBiasInfo[j][k];
                    previousBiasInfo[j][k] = previousBiasInfo[j+1][k];
                    previousBiasInfo[j+1][k] = tempValue;
                }
                swapped = true;
            }
        }
        
        if(swapped == false){
            break;
        }
    }
}

           
/*-----------------------------------------------------------------------------------------*/
/*---------------------------------------- GETTERS ----------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
Player AI::getPlayer(){
    return(player);
}


/*-----------------------------------------------------------------------------------------*/
/*---------------------------------------- SETTERS ----------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

/*----------------------------------------SET PLAYER---------------------------------------*/
void AI::setPlayer(Player player){
    this->player = player;
}

/*----------------------------------- SET CONTINUE CHOICE ---------------------------------*/
void AI::setContinueChoice(int continueChoice){
    this->continueChoice = continueChoice;
}

/*------------------------------------ SET SWERVE CHOICE ----------------------------------*/
void AI::setSwerveChoice(int swerveChoice){
    this->swerveChoice = swerveChoice;
}


/*-----------------------------------------------------------------------------------------*/
/*----------------------------------- SETTERS FOR BIASES ----------------------------------*/
/*-----------------------------------------------------------------------------------------*/

/*-------------------------------------- SET AGE BIAS -------------------------------------*/
void AI::setAgeBias(string userIn){
    //Every 10 year similar is 0.1% ex) age = 30 chose age 40 over 50
    //However, age bias may be parabolic
    //Have shifted one way
    biasChosen(userIn);

    int difference;
    bool picked = false;

    if(biasPicked == AGE){
        picked = true;
    }
    
    //CHANGE SUB CAT BIAS (Based on being picked i.e. += 0.1)
    //CHANGE SUB CAT BIAS (Based on NOT being picked i.e. -= 0.1)
    if(biasPicked == OLD){
        difference = abs(100 - player.getAge());
        picked = true;
        biases[OLD/10][OLD%10] += 0.1;
    }else if(notPicked == OLD){
        biases[OLD/10][OLD%10] -= 0.1;
    }
    
    if(biasPicked == MIDDLE){
        difference = abs(50 - player.getAge());
        picked = true;
        biases[MIDDLE/10][MIDDLE%10] += 0.1;
    }else if(notPicked == MIDDLE){
        biases[MIDDLE/10][MIDDLE%10] -= 0.1;
    }
    
    if(biasPicked == YOUNG){
        difference = abs(10 - player.getAge());
        picked = true;
        biases[YOUNG/10][YOUNG%10] += 0.1;
    }else if(notPicked == YOUNG){
        biases[YOUNG/10][YOUNG%10] -= 0.1;
    }

    //CHANGE AGE BIAS
    if(picked == false){
        biases[AGE/10][AGE%10] -= 0.1;
    }else if(picked == true){
        if(difference > 50){
            biases[AGE/10][AGE%10] += 0.2;
        }else if(difference > 40){
            biases[AGE/10][AGE%10] += 0.1;
        }else if(difference > 30){
            biases[AGE/10][AGE%10] += 0.05;
        }else if(difference > 20){
            biases[AGE/10][AGE%10] += 0.025;
        }else{
            biases[AGE/10][AGE%10] -= 0.05;
        }
    }
}

/*------------------------------------ SET WEIGHT BIAS-------------------------------------*/
void AI::setWeightBias(string userIn){
    //Increase 0.1 for every "fat" pushed
    //Increase 0.2 if player is < 175lbs and fat pushed
    biasChosen(userIn);

    int difference;
    bool picked = false;
    
    if(biasPicked == FAT){
        difference = abs(500 - player.getWeight());
        picked = true;
        biases[FAT/10][FAT%10] += 0.1;
    }else if(notPicked == FAT){
        biases[FAT/10][FAT%10] -= 0.1;
    }
    
    if(biasPicked == FIT){
        difference = abs(250 - player.getWeight());
        picked = true;
        biases[FIT/10][FIT%10] += 0.1;
    }else if(notPicked == FIT){
        biases[FIT/10][FIT%10] -= 0.1;
    }
    
    if(biasPicked == THIN){
        difference = abs(100 - player.getWeight());
        picked = true;
        biases[THIN/10][THIN%10] += 0.1;
    }else if(notPicked == THIN){
        biases[THIN/10][THIN%10] -= 0.1;
    }
    
    if(picked == false){
        biases[WEIGHT/10][WEIGHT%10] -= 0.1;
    }else if(picked == true){
        if(difference > 300){
            biases[WEIGHT/10][WEIGHT%10] += 0.2;
        }else if(difference > 200){
            biases[WEIGHT/10][WEIGHT%10] += 0.1;
        }else if(difference > 100){
            biases[WEIGHT/10][WEIGHT%10] += 0.05;
        }else if(difference > 50){
            biases[WEIGHT/10][WEIGHT%10] += 0.025;
        }else{
            biases[WEIGHT/10][WEIGHT%10] -= 0.05;
        }
    }
}

/*----------------------------------- SET SEX BIAS ----------------------------------------*/
void AI::setSexBias(string userIn){
    biasChosen(userIn);

    if(biasPicked == MALE){
        if(player.getSex() != "m"){
            biases[SEX/10][SEX%10] += 0.1;
            biases[MALE/10][MALE%10] += 0.1;
        }else{
            biases[SEX/10][SEX%10] -= 0.1;
            biases[MALE/10][MALE%10] -= 0.1;
        }
        
    }else if(notPicked == MALE){
        if(player.getSex() != "m"){
            biases[SEX/10][SEX%10] -= 0.1;
            biases[MALE/10][MALE%10] -= 0.1;
        }else{
            biases[SEX/10][SEX%10] += 0.1;
            biases[MALE/10][MALE%10] += 0.1;
        }
    }
    
    if(biasPicked == FEMALE){
        if(player.getSex() != "f"){
            biases[SEX/10][SEX%10] += 0.1;
            biases[FEMALE/10][FEMALE%10] += 0.1;
        }else{
            biases[SEX/10][SEX%10] -= 0.1;
            biases[FEMALE/10][FEMALE%10] -= 0.1;
        }
        
    }else if(notPicked == FEMALE){
        if(player.getSex() != "f"){
            biases[SEX/10][SEX%10] -= 0.1;
            biases[FEMALE/10][FEMALE%10] -= 0.1;
        }else{
            biases[SEX/10][SEX%10] += 0.1;
            biases[FEMALE/10][FEMALE%10] += 0.1;
        }
    }
}

/*----------------------------------- SET LEGAL BIAS --------------------------------------*/
void AI::setLegalBias(string userIn){
    biasChosen(userIn);
    
    //Subcatagories
    if(biasPicked == SAMARITAN){
        biases[LEGAL/10][LEGAL%10] += 0.1;
        biases[SAMARITAN/10][SAMARITAN%10] += 0.1;
    }else if(notPicked == SAMARITAN){
        biases[LEGAL/10][LEGAL%10] -= 0.1;
        biases[SAMARITAN/10][SAMARITAN%10] -= 0.1;
    }
    
    if(biasPicked == MURDERER){
        biases[LEGAL/10][LEGAL%10] += 0.1;
        biases[MURDERER/10][MURDERER%10] += 0.1;
    }else if(notPicked == SAMARITAN){
        biases[LEGAL/10][LEGAL%10] -= 0.1;
        biases[MURDERER/10][MURDERER%10] -= 0.1;
    }
}

/*----------------------------------- SET HEALTH BIAS --------------------------------------*/
void AI::setHealthBias(string userIn){
    biasChosen(userIn);
    
    //Subcatagories
    if(biasPicked == HEALTHY){
        biases[HEALTH/10][HEALTH%10] += 0.1;
        biases[HEALTHY/10][HEALTHY%10] += 0.1;
    }else if(notPicked == HEALTHY){
        biases[HEALTH/10][HEALTH%10] -= 0.1;
        biases[HEALTHY/10][HEALTHY%10] -= 0.1;
    }
    
    if(biasPicked == SICK){
        biases[HEALTH/10][HEALTH%10] += 0.1;
        biases[SICK/10][SICK%10] += 0.1;
    }else if(notPicked == SAMARITAN){
        biases[HEALTH/10][HEALTH%10] -= 0.1;
        biases[SICK/10][SICK%10] -= 0.1;
    }
}

/*----------------------------------- SET MORAL BIAS --------------------------------------*/
void AI::setMorals(string userIn){
    biasChosen(userIn);

    //Used for initial quesiton
    if(swerveChoice == 2 && continueChoice == 1){
        if(biasPicked == 1){
            biases[MORAL/10][MORAL%10] -= 0.1;
        }else{
            biases[MORAL/10][MORAL%10] += 0.1;
        }
    }
    
    if((biasPicked < 100) && (notPicked > 100)){
        biases[MORAL/10][MORAL%10] -= 0.1;
    }else if((biasPicked > 100) && (notPicked < 100)){
        biases[MORAL/10][MORAL%10] += 0.1;
    }
}

/*-----------------------------------------------------------------------------------------*/
/*------------------------------------- UPDATES BIASES ------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

void AI::updateBiases(string userIn){
    tempSwerve = swerveChoice;
    tempContinue = continueChoice;
    
    if(tempSwerve >= 100){
        tempSwerve = (tempSwerve/10) + (tempSwerve % 100);
    }
    
    if(tempContinue >= 100){
        tempContinue = (tempContinue/10) + (tempContinue % 100);
    }
    
    //Changes user moral information
    if((waitFor <= 0) || (waitOverRide == true)){
        setMorals(userIn);
    }
    
    if(biasPicked > 100){
        biasPicked = biasPicked/10 + biasPicked%10;
    }
    
    if(notPicked > 100){
        notPicked = notPicked/10 + notPicked%10;
    }
    
    if(tempSwerve != tempContinue){
        //Change bias for any other stuff
        if((tempSwerve/10 == 1)||(tempContinue/10 == 1)){
            setAgeBias(userIn);
        }
    
        if((tempSwerve/10 == 2)||(tempContinue/10 == 2)){
            setWeightBias(userIn);
        }
    
        if((tempSwerve/10 == 3)||(tempContinue/10 == 3)){
            setSexBias(userIn);
        }
        
        if((tempSwerve/10 == 4)||(tempContinue/10 == 4)){
            setLegalBias(userIn);
        }
        
        if((tempSwerve/10 == 5)||(tempContinue/10 == 5)){
            setHealthBias(userIn);
        }
    }
    
    checkBiasBounds();
}


/*-----------------------------------------------------------------------------------------*/
/*------------------------------------ CHECK FUNCTIONS ------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

/*------------------------------------- Check Biases --------------------------------------*/
void AI::checkBiasBounds(){
    int ti;
    int tj;
    
    for(int i = 0; i < NUM_BIASES; i++){
        ti = biasInfo[i].code/10;
        tj = biasInfo[i].code%10;
        if(biases[ti][tj] <= 0.0001){
            biases[ti][tj] = 0.0;
        }else if(biases[ti][tj] >= 1.0){
            biases[ti][tj] = 1.0;
        }
    }
    
}

/*---------------------------- Checks for repeating questions -----------------------------*/
bool AI::repeatCheck(){
    int tempContinue;
    int tempSwerve;

    //Check if override is in place or not
    if(overRide == true){
        return(false);
    }
    
    //No questions have been asked yet
    if(askedList[0] == -1){
        return(false);
    }

    //Values are the same
    if(continueChoice > 100){
        tempContinue = continueChoice/10 + continueChoice%10;
    }else{
        tempContinue = continueChoice;
    }
    
    if(swerveChoice > 100){
        tempSwerve = swerveChoice/10 + swerveChoice%10;
    }else{
        tempSwerve = swerveChoice;
    }
    
    if(tempSwerve == tempContinue){
        return(true);
    }

    //Repeated return true
    for(int i = 0; i < askedIndex - 1; i+=2){
        if((continueChoice == askedList[i])&&(swerveChoice == askedList[i+1])){
            return(true);
        }
    }
    
    //No repeated values, continue
    return(false);
}


/*-----------------------------------------------------------------------------------------*/
/*----------------------------------- FIND BIAS CHOSEN ------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

/*------------------------------------ What Was Picked ------------------------------------*/
void AI::biasChosen(string userIn){
    if(userIn[0] == 'b'){
        biasPicked = swerveChoice;
        notPicked = continueChoice;
    }else{
        biasPicked = continueChoice;
        notPicked = swerveChoice;
    }
}

/*---------------------------------- Record User Answer -----------------------------------*/
void AI::recordAnswer(string userIn){
    //Want b so b = 1, a = 0
    if(userIn[0] == 'b'){
        pickedList[pickedIndex] = 1;
        pickedIndex += 1;
    }else if(userIn[0] == 'a'){
        pickedList[pickedIndex] = 0;
        pickedIndex += 1;
    }
}


/*-----------------------------------------------------------------------------------------*/
/*------------------------------------ RESET VARIABLES ------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
void AI::resetVariables(){
    swerveChoice = 0;
    continueChoice = 0;
    domBias = 0;
    leastBiased = 1;
}


/*-----------------------------------------------------------------------------------------*/
/*--------------------------------- PICK PEDESTRIAN INFO ----------------------------------*/
/*-----------------------------------------------------------------------------------------*/

/*-------------------------------- PREVIOUS QUESTIONS FIRST -------------------------------*/
bool AI::pickFirst(){
    //We can assume that these questions have NOT been asked yet
    //Must have at least x% success rate
    
    bool loopFlag = true;
    
    if(pickFirstFlag == true){
        if(previousBiasInfo[successRateIndex][3] >= NEEDED_SUCCESS_RATE){

            //Loop until we no longer have reliable success rates
            do{
                //Only accept success rates with so many people
                if(previousBiasInfo[successRateIndex][2] >= MIN_PEOPLE_NEEDED){
                    continueChoice = previousBiasInfo[successRateIndex][0];
                    swerveChoice = previousBiasInfo[successRateIndex][1];
                    
                    askedList[askedIndex] = continueChoice;
                    askedIndex += 1;
                    askedList[askedIndex] = swerveChoice;
                    askedIndex += 1;
                    
                    successRateIndex += 1;
                    return(true);
                }
            
                successRateIndex += 1;
            }while(previousBiasInfo[successRateIndex][3] >= NEEDED_SUCCESS_RATE);
        }
    }
    
        pickFirstFlag = false;
        return(false);
}

/*--------------------------------- NEW QUESTIONS SECOND ----------------------------------*/
void AI::pickChoices(){
    /* WE WANT USER TO PICK SWERVE */
    
    if(pickFirstFlag == false){
        bool needToChange;
    
        int continueTried = 1;
        int swerveTried = 1;
        
        //Default to the first one
        int multiplierSwerve = 10;
        int multiplierContinue = 10;
    
        domBias = 0;        //For swerve
        leastBiased = 1;    //For Continue

        //Check for 1 or 2 people after x num questions asked
        if(waitFor <= 0){
            if(initialBiases[MORAL/10][MORAL%10] < 0.5){
                multiplierSwerve = 100;
            }else{
                multiplierContinue = 100;
            }
        }else{
            waitFor -= 1;
        }
 
        //Already dealt with moral bias (which picks 1 or 2 people)
        //Check on next bias i.e. age
        for(int i = 1; i < NUM_CAT_ROWS; i++){
            for(int j = 1; j < MAX_BIAS_COLS; j++) {
            
                //Pick for swerve option
                if(biases[i][j] >= domBias){
                    domBias = biases[i][j];
                    swerveChoice = (i * multiplierSwerve) + j;
                }
            
                //Pick for continue/walk action
                if((biases[i][j] <= leastBiased) && (biases[i][j] >= 0)){                    leastBiased = biases[i][j];
                    continueChoice = (i * multiplierContinue) + j;
                }
            }
        }
    
        //See if we have asked question or not
        needToChange = repeatCheck();
    
        //Repick question
        while(needToChange == true){
            leastBiased = 1;
            domBias = 0;
        
            for(int k = askedIndex; k > 0; k--){
                for(int i = 1; i < NUM_CAT_ROWS; i++){
                    for(int j = 1; j < MAX_BIAS_COLS; j++) {
                
                        //Pick for swerve option if we've tried all combinations of continue
                        if((continueTried % (NUM_BIASES - NUM_CAT_ROWS)) == 0){
                            if((biases[i][j] >= domBias) && (askedList[k] != biases[i][j])){
                                domBias = biases[i][j];
                                swerveChoice = (i * multiplierSwerve) + j;
                            }
                            swerveTried += 1;
                        }
                
                        //Pick for continue/walk action
                        if((biases[i][j] <= leastBiased) && (biases[i][j] >= 0)){
                            if(biases[i][j] != askedList[k-1]){
                                leastBiased = biases[i][j];
                                continueChoice = (i * multiplierContinue) + j;
                            }
                        }
                        continueTried += 1;
                    }
                }
            }
        
            if((continueTried == pow(NUM_SUB_CAT, 2)) && (swerveTried == NUM_SUB_CAT)){
                overRide = true;
            }

            needToChange = repeatCheck();
        };
    
        askedList[askedIndex] = continueChoice;
        askedIndex += 1;
        askedList[askedIndex] = swerveChoice;
        askedIndex += 1;
    }
}


/*-----------------------------------------------------------------------------------------*/
/*-------------------------------------- Save Biases --------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
void AI::saveBiases(){
    for(int i = 0; i < NUM_CAT_ROWS; i++){
        for(int j = 0; j < MAX_BIAS_COLS; j++){
            initialBiases[i][j] = biases[i][j];
        }
    }
}


/*-----------------------------------------------------------------------------------------*/
/*-------------------------------------- PRINT INFO ---------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

/*--------------------------------- PRINT CONTINUE CHOICE ----------------------------------*/
void AI::printContinue(){
    //To properly change bias values (Given by OP code)
    for(int i = 1; i < NUM_TOTAL_BIASES; i++){
        if(continueChoice == questions[i].code){
            cout << questions[i].type;
            break;
        }
    }
    
    if(continueChoice >= 100){
        cout << " walk ";
    }else{
        cout << " walks ";
    }
    
}

/*---------------------------------- PRINT SWERVE CHOICE -----------------------------------*/
void AI::printSwerve(){
    //To properly change bias values (Given by OP code)
    for(int i = 1; i < NUM_TOTAL_BIASES; i++){
        if((swerveChoice) == questions[i].code){
            cout << questions[i].type;
            break;
        }
    }
}

/*------------------------------------- PRINT CURRENT --------------------------------------*/
void AI::printCurrentResults(){
    cout << "   Values: " << endl;
    
    //Print all other biases
    for(int i = 0; i < NUM_BIASES; i++){
        if(biasInfo[i].code % 10 == 0){
            cout << endl;
        }
        
        cout << "       " << biasInfo[i].outPut << " = ";
        cout << biases[biasInfo[i].code/10][biasInfo[i].code%10] << endl;
    }
    
    cout << endl;
}

/*------------------------------------- PRINT RESULTS --------------------------------------*/
void AI::printResults(){
    cout << "END RESULTS: " << endl;
    cout << "   Start values: " << endl;
    
    for(int k = 0; k < 2; k++){
        //Print all other biases
        for(int i = 0; i < NUM_BIASES; i++){
            if(biasInfo[i].code % 10 == 0){
                cout << endl;
            }
            
            cout << "       " << biasInfo[i].outPut << " = ";

            if(k == 0){
                cout << initialBiases[biasInfo[i].code/10][biasInfo[i].code%10] << endl;
            }else{
                cout << biases[biasInfo[i].code/10][biasInfo[i].code%10] << endl;
            }
        }
        
        cout << endl;
        
        if(k == 0){
            cout << "   End values: " << endl;
        }
    }
    
    for(int i = askedIndex; i > askedIndex; i--){
        cout << "INDEX VALUE (Going backwards): " << askedList[askedIndex] << " ";
    }
    cout << "END GAME" << endl;
}

