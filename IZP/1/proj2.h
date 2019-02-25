/**
* @decription  Header file for proj2.c 
* @author      Jan Jakub Kubik (xkubik32)
* @date        19.11.2016 */

#ifndef PROJ2_H
#define PROJ2_H

// @decription return codes
#define OK 0 
#define WRONG_PARAM 1
#define WRONG_INPUT  2 


#define MAX_NMB_OF_ARG 5 // @brief max number of arguments

// @description global structure for program
struct Arguments
{
  bool check_log;
  bool check_pow;
  bool bonus;
  double x;
  double y;
  long long int n;
} check = {.check_log = false, .check_pow = false, .bonus = false, .x = 0.0, .y = 0.0, .n = 0};


/**
 * @brief help for program */
void help(void);


/**
 * @brief format output of program */
void format_output();


/**
* @brief       check input params
* @param argc  number of args
* @param argv  arguments
* @return code */
int args_check(int argv, char *argc[]);


/**
* @brief    counting of natural logarithm using taylor polynomial
* @param x  number for counting
* @param n  number of iterations
* @return result */
double taylor_log(double x, unsigned int n);


/**
* @brief    counting of natural logarithm using continued fractions
* @param x  number for counting
* @param n  number of iterations
* @return result */
double cfrac_log(double x, unsigned int n);


/**
* @brief    counting of exponential function using taylor polynomial
* @param x  number for counting
* @param n  number of iterations
* @return result */
double taylor_pow(double x, double y, unsigned int n);


/**
* @brief    counting of exponential function using continued fractions
* @param x  number for counting
* @param n  number of iterations
* @return result */
double taylorcf_pow(double x, double y, unsigned int n);


/**
 * @brief counting of exponential function acording formula
 * @param logar  number for counting (logar^y)
 * @param y  number for counting
 * @param n  number of iterations
 * @return result */
double pow_acording_formula(double logar, double y, int n);


// Bonus tasks
/**
* @brief Counting of natural logarithm with accuracy 10^e-7 */
double mylog(double x);


/**
* @brief Counting of x^y with accuracy 10^e-7 */
double mypow(double x, double y);

#endif