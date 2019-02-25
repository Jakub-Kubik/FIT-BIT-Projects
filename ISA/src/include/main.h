/**
 * @description  header file for main for whole program
 * @author       Jakub Kubik, xkubik32@stud.fit.vutbr.cz
 * @date         04.11.2018 */

#ifndef MAIN_H
#define MAIN_H

#include <string>


// @brief my personal ERROR codes
const int badInputERR = 99;
const int fileERR = 42;
const int certERR = 55;
const int clientERR = 66;

const int feedContentERR = 99;

const std::string serverResponseERR = "ERR";

const int OK = 0;


// @brief general function for printing errors
void printError(std::string error);

#endif //MAIN_H