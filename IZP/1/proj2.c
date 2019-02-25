/**
* @decription  Program for counting of natural logarithm and 
*              exponential function with help of taylor polynomial
*              and continued fractions
* @author      Jan Jakub Kubik (xkubik32)
* @date        19.11.2016 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>

#include "proj2.h"


int main(int argc, char *argv[]) {

  int arg = args_check(argc, argv);
  if ( arg != OK) {
    help();
    return arg;
  }

  format_output();

  return OK;
}


void help(void) {

  fprintf(stderr, "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
  fprintf(stderr, "||                                                                                                                    ||\n");
  fprintf(stderr, "||    Run program: --log x n   -> counting of natural logarithm from x in n iterations                                ||\n"
                  "||                 --pow x y n -> counting of exponenitial funciton x check y in n iterations                          ||\n");
  fprintf(stderr, "||                 -h          -> help                                                                                ||\n");
  fprintf(stderr, "||                                                                                                                    ||\n");
  fprintf(stderr, "||     Calculation uses taylor polynomial and continued fractions                                                     ||\n");
  fprintf(stderr, "||                                                                                                                    ||\n");
  fprintf(stderr, "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
  fprintf(stderr, "\n\n");
}


int args_check(int argc, char *argv[]) {

  char *p_error_x;  //pointers for checking x, y, n
  char *p_error_y;
  char *p_error_n;

  // number of arguments
  if ( argc > MAX_NMB_OF_ARG  || argc == 1 ) {     
    fprintf(stderr, "Wrong number of arguments\n");
    help();
    return WRONG_PARAM;
  }

  // log
  if ((strcmp(argv[1], "--log") == 0) && (argc == 4)) {
      check.check_log = true;

      check.x = strtod(argv[2], &p_error_x);
      check.n = strtol(argv[3], &p_error_n, 10);

     if ((*p_error_x != '\0') || (*p_error_n != '\0') || (check.n <= 0)) {
        return WRONG_INPUT;
      }

  }

  // pow
  else if ((strcmp(argv[1], "--pow") == 0) && (argc == 5)) { 
    check.check_pow = true;

    check.x = strtod(argv[2], &p_error_x);
    check.y = strtod(argv[3], &p_error_y);
    check.n = strtol(argv[4], &p_error_n, 10);

    if ((*p_error_x != '\0') ||(*p_error_y != '\0') || (*p_error_n != '\0') || (check.n <= 0)) {
      return WRONG_INPUT;
    }
  }

  // help
  else if ((strcmp(argv[1], "-h") == 0)) {
      help();
  }

  // bonus
  else if (strcmp(argv[1], "--bonus") == 0) {
    check.bonus = true;
    if ((strcmp(argv[2], "--log") == 0)) {
      check.check_log = true;
      check.x = strtod(argv[3], &p_error_x);
      if (*p_error_x != '\0') {
        return WRONG_INPUT;
      }
    }
    else if (strcmp(argv[2], "--pow") == 0) {
      check.check_pow = true;
      check.x = strtod(argv[3], &p_error_x);
      check.y = strtod(argv[4], &p_error_y);
      if (*p_error_x != '\0' || *p_error_y != '\0') {
        return WRONG_INPUT;
      }
    }
  }
  else return WRONG_INPUT;

  // n range
  if (check.n >= UINT_MAX)  {
      fprintf(stderr, "Parameter n is bigger than UNINT_MAX\n");
      return WRONG_INPUT;
  }

  return OK;
}


void format_output() {

  // Normal tasks
  if ( check.check_log ) { // check is global structure
      printf("       log(%g) = %.12g\n",check.x, log(check.x));
      printf(" cfrac_log(%g) = %.12g\n",check.x, cfrac_log(check.x, check.n));
      printf("taylor_log(%g) = %.12g\n",check.x, taylor_log(check.x, check.n));
  }
  else if ( check.check_pow ) {
    printf("         pow(%g,%g) = %.12g\n",check.x, check.y, pow(check.x, check.y));
    printf("  taylor_pow(%g,%g) = %.12g\n",check.x, check.y, taylor_pow(check.x, check.y, check.n));
    printf("taylorcf_pow(%g,%g) = %.12g\n",check.x, check.y, taylorcf_pow(check.x, check.y, check.n));
  }
  
  // Bonus tasks
  else if ( check.bonus ) {
    if ( check.check_log ) {
      printf("mylog(%g) = %.7e\n", check.x, mylog(check.x));
      printf("  log(%g) = %.7e\n",check.x, log(check.x));
    }
    else if ( check.check_pow ) {
      printf("mypow(%g,%g) = %.7e\n", check.x, check.y, mypow(check.x, check.y));
      printf("  pow(%g,%g) = %.7e\n",check.x, check.y, pow(check.x, check.y));
    }
  }
}


double check_log_x(double x) {

  // return values as log from math.h
  if (x == 0) return -INFINITY;
  else if (x <= 0.0) return NAN; 
  else if (isnan(x)) return NAN;
  else if (x == INFINITY) return INFINITY;
  if (x == 1.0) return 0.0; 

  else return OK;
}


double taylor_log(double x, unsigned int n) {

  // checking x value
  double val = check_log_x(x);
  if (val != OK) return val;

  double citatel = 1;
  double sum = 0;

  //--------------------------------------------------------------
  // Formula ->   log(1-x) = -x - x^2/2 - x^3/3 - x^4/4 - ......
  //---------------------------------------------------------------
  if (x > 0 && x < 1) {
      x = 1 - x;  

      for (unsigned int i = 1; i <=n; i++) {
        citatel *= x;
        sum -=  citatel/i;
      }
      return sum;
  }

  //----------------------------------------------------------------------
  // Formula -> log(x) = ((x-1)/x) + ((x-1)/x)^2/2 + ((x-1)/x)^3/3 + .....
  //-----------------------------------------------------------------------
  else if (x > 1){
    for (unsigned int i = 1; i <=n; i++) {
      citatel *= ((x-1) / x);
      sum +=  citatel/i;
    }
    return sum;
  }
  else return WRONG_INPUT;
}


double cfrac_log(double x, unsigned int n) {

  // checking x value
  double val = check_log_x(x);
  if (val != OK) return val;

  double cf = 0.0;
  x = ((x-1) / (1+x));

  for (int i = n; i > 1; i--) {
    cf = (((i-1) * (i-1)) * (x*x)) / (( i*2 - 1) - cf);
  }
  cf = 2*x / (1-cf);
  
  return  cf;
}


double pow_acording_formula(double logar_x, double y, int n) {

  double sum = 1.0;
  double citatel = 1.0;
  double menovatel = 1.0;

  //----------------------------------------------------------------------
  // Formula -> a^x = 1 + (x * ln(a)) + (x^2 * ln^2(a)) +  (x^3 * ln^3(a))
  //-----------------------------------------------------------------------
  for (int i = 1; i<=n; i++) {
     citatel = citatel * y * logar_x;
     menovatel = menovatel * i;
     sum += citatel/menovatel;
    }

     return sum;
}


double check_pow_x_y(double x, double y) {

  // return values as pow from math.h
  if (x == 1) return 1.0;
  else if (x == 0) return 0;
  else if (y == 0) return 1.0;
  else if (x == -1 && fabs(y) == INFINITY ) return 1.0;
  else if (fabs(x) < 1 && y == -INFINITY) return INFINITY;
  else if (fabs(x) < 1 && y == INFINITY) return 0.0;
  else if (fabs(x) > 1 && y == -INFINITY) return 0.0;
  else if (fabs(x) > 1 && y == INFINITY) return INFINITY;
  else if (x == -INFINITY && (int)y%2 == 1 && y < 0) return 0;
  else if (x == -INFINITY && (int)y%2 == 0 && y < 0) return 0;
  else if (x ==  INFINITY && y < 0) return 0;

  else return OK;
}


double taylor_pow(double x, double y, unsigned int n) {

  // checking x value
  double val = check_pow_x_y(x, y);
  if (val != OK) return val;  

  double logar_x = taylor_log(x, n);
  return pow_acording_formula(logar_x, y, n);
}


double taylorcf_pow(double x, double y, unsigned int n) {

  // checking x value
  double val = check_pow_x_y(x, y);
  if (val != OK) return val; 

  double logar_x = cfrac_log(x, n);
  return pow_acording_formula(logar_x, y, n);
}


/****************************************************************************************************************************
 BONUS TASKS
*****************************************************************************************************************************/
double mylog(double x) {

  // checking x value
  double val = check_log_x(x);
  if (val != OK) return val;  

  double pcf = 0.0;
  double cf = 1.0;
  double eps = 1e-8;
  int n = 1;

  while (fabs(pcf-cf) > eps) {
      pcf = cf;
      cf = cfrac_log(x, n);
      n++;
  }
  printf("na %d iteracii\n", n);
  return cf;
}


double mypow(double x, double y) {

  // checking x value
  double val = check_pow_x_y(x, y);
  if (val != OK) return val;  

  double ppw = 0.0;
  double pw = 1.0;
  double eps = 1e-8;
  int n = 1;

  while (fabs(ppw-pw) > eps) {
      ppw = pw;
      pw = taylorcf_pow(x, y, n);
      n++;
  }
  printf("%d iteracii\n", n);
  return pw;
}
