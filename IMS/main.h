#ifndef MAIN_H
#define MAIN_H

#include <vector>

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *          YIELD IS CALCULATED IN TONS FOR 1 hectare 
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

// value of yield increase per day in % calculated from whole yield
#define YIELD_INCREASE 0.082645
// duration in days
#define SEASON_DURATION 135
// minimum temperature when plant take nitrogen
#define MIN_TEMP 2.0 
#define GROWING_AREA 161 // hectars
#define COSTS 1120.5 // costs per hectare

/**
 * Values from research where was added nitrogen in late autumn
 */
struct ValidationValues {
    float oldYield;
    float newYield;
    float yieldDifference;
    uint32_t yieldIncreaseDays;
};

/**
 * Values from the farm we want to show
 * advanteges of adding nitrogen in late autumn 
 */
struct ObservedValues {
    float originalYield;
    float sellingPrice; 
    float orignalNetProfit;

};

/**
 * Average temperatures [degree Celsius] per day in winter seasons
 * Winter season is set from 1.November to 15.March 
 * It is interval of 135 days ()
 */
struct WinterSeasons {
    std::vector<float> season1; // winter season 2013/2014
    std::vector<float> season2; // winter season 2014/2015
    std::vector<float> season3; // winter season 2015/2016
    std::vector<float> season4; // winter season 2016/2017
};

struct Report {
    uint32_t yieldIncreaseDays;
    float seasonYieldIncrease; // maximum increase in % from whole yield
    float finalYield; // yield after add increase in season
    float newEarnedMoney; // amount of money after add yield increase
    float moneyDifference;
};

struct CliParams {
    bool isSeasonNumber;
    bool isValidation;
};

CliParams proccessingCliParams(int paramCnt, char** paramArgv);
WinterSeasons *setValuesToSeasons();
std::vector<Report*> addingNitrogenInWinterSeason(CliParams*, WinterSeasons*);
void printResult(CliParams*, std::vector<Report*>);

#endif