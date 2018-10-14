/**
 * questionTable.h
 *
 * Description:
 *      -Struct made of int, string to store question grammer and information
 *         -int code:
 *          -Integer to find value in table
 *          -This value relates to biasInformation
 *          -Also relates to the other tables
 *
 *         -string type:
 *          -Helps with outputting proper english sentences
 *          -For example: usses walks for singluar and walk for plural
 *
 *      For Example: code = 0, type = "a person"
 *                   code = 01, type = "2 people"
 *
 * Author: Evan Geissler
 * Date: April, 2018
 */

#include <iostream>
#include <string>

#ifndef __QUESTIONTABLE_H__
#define __QUESTIONTABLE_H__

using namespace std;
    
    struct QUESTION_TABLE{
        int code;
        string type;
    };

#endif
