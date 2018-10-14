/**
 * baisInformation.h
 *
 * Description:
 *      -Struct made of int, string, string to store bias info
 *         -int code:
 *          -Integer to find value in table
 *          -This value relates to questionTable
 *          -Also relates to the other tables
 *
 *         -string name:
 *          -Name of bias, nothing special
 *
 *         -string outPut:
 *          -Helps to display values for output
 *
 *      For Example: code = 21, name = fat, output = Fat Bias
 *
 * Author: Evan Geissler
 * Date: April, 2018
 */

#include <iostream>
#include <string>

#ifndef __BIASINFORMATION_H__
#define __BIASINFORMATION_H__

using namespace std;
    
    struct BIAS_INFORMATION{
        int code;
        string name;
        string outPut;
    };

#endif
