#include <iostream>
#include <getopt.h>
#include <string>
#include "main.h"

ValidationValues validationValues = { // all values in [ton/ha]
    4.428, // old yield 
    4.92,  // new yield
    0.492, // 10% difference between old and new yield
    121
};

ObservedValues observedValues = {
    3.38, // original yield in [ton/ha]
    360,  // price in euros for 1 ton
    21800 // original net profit in euros 
};

std::vector<Report*> addingNitrogenInWinterSeason(CliParams* cliParams, WinterSeasons* seasons) {
    std::vector<Report*> reports;
    
    int daysOverMin = 0; // has to be 3 days over 2°C and plant will start take nitrogen again
    bool isUnderMin = false;
    bool oneDayUnder = false; // one day under minimum we do not expect consumption of nitrogen

    if (cliParams->isValidation) {
        // only winter season 2015/2016 is used because validation values were made in this season
        std::vector<float>::iterator tempPerDay;
        Report* report = new Report;
        for (tempPerDay = seasons->season3.begin(); tempPerDay < seasons->season3.end(); ++tempPerDay) {
            if (*tempPerDay < MIN_TEMP && oneDayUnder) {
                isUnderMin = true;
                daysOverMin = 0;        
            }
            else if (*tempPerDay >= MIN_TEMP) {
                if (isUnderMin && daysOverMin < 3) daysOverMin++;
                else {
                    isUnderMin = false;
                    oneDayUnder = false;
                }
                report->yieldIncreaseDays++;
                report->seasonYieldIncrease += YIELD_INCREASE;
            }
            else {
                oneDayUnder = true;
            }
        }
        reports.push_back(report);
        report->finalYield = validationValues.oldYield + (validationValues.oldYield * (report->seasonYieldIncrease / 100) );
    }
    else if (cliParams->isSeasonNumber) {

        std::vector<std::vector<float>> tempSeasons;
        tempSeasons.push_back(seasons->season1);
        tempSeasons.push_back(seasons->season2);
        tempSeasons.push_back(seasons->season3);
        tempSeasons.push_back(seasons->season4);

        for (std::vector<float> season: tempSeasons) {
            Report* report = new Report;
            for (float tempPerDay: season) {
                if (tempPerDay < MIN_TEMP && oneDayUnder) {
                    isUnderMin = true;
                    daysOverMin = 0;        
                }
                else if (tempPerDay >= MIN_TEMP) {
                    if (isUnderMin && daysOverMin < 3) daysOverMin++;
                    else {
                        isUnderMin = false;
                        oneDayUnder = false;
                    }
                    report->yieldIncreaseDays++;
                    report->seasonYieldIncrease += YIELD_INCREASE;
                }
                else {
                    oneDayUnder = true;
                }
            }
            report->finalYield = observedValues.originalYield + (observedValues.originalYield * (report->seasonYieldIncrease / 100) );
            float wholeProfit = report->finalYield * observedValues.sellingPrice;
            report->newEarnedMoney = (wholeProfit - COSTS) * GROWING_AREA;
            report->moneyDifference = report->newEarnedMoney - observedValues.orignalNetProfit;
            reports.push_back(report);
        }
    }
    
    return reports;
}
void printResult(CliParams *cliParams, std::vector<Report*> reports) {
    if (cliParams->isValidation) {
        std::cout << "+----------------------------------+" << std::endl;
        std::cout << "|    Summary of validation test    |" << std::endl;
        std::cout << "+----------------------------------+" << std::endl;
        std::cout << "| Valid values:                    |" << std::endl;
        std::cout << "|     original yield = " << validationValues.oldYield << " t/ha  |" << std::endl;
        std::cout << "|     new yield      = " << validationValues.newYield << " t/ha   |" << std::endl;
        std::cout << "|     yield diff     = " << validationValues.yieldDifference << " t/ha  |" << std::endl;
        std::cout << "|     days over 2°C  = " << validationValues.yieldIncreaseDays << " / " << SEASON_DURATION << "   |" << std::endl;
        std::cout << "+----------------------------------+" << std::endl;
        std::cout << "| Values in observed conditions:   |" << std::endl;
        std::cout << "|     yield increase = " << reports[0]->seasonYieldIncrease << " \%   |" << std::endl;
        std::cout << "|     yield increase = " << validationValues.oldYield * (reports[0]->seasonYieldIncrease / 100) << " t/ha |" << std::endl;
        std::cout << "|     new yield      = " << reports[0]->finalYield << " t/ha |" << std::endl;
        std::cout << "|     days over 2°C  = " << reports[0]->yieldIncreaseDays << " / " << SEASON_DURATION << "    |" << std::endl;
        std::cout << "+----------------------------------+" << std::endl;
    }
    else if (cliParams->isSeasonNumber) {
        std::vector<std::string> whichSeason = {
            "Season 2013/2014", "Season 2014/2015",
            "Season 2015/2016", "Season 2016/2017"
        };
        int countSeason = 0;
        for (Report* report: reports) {
            std::cout << "+----------------------------------+" << std::endl;
            std::cout << "|    Summary of observed yield     |" << std::endl;
            std::cout << "|         " << whichSeason[countSeason++] << "         |" << std::endl;
            std::cout << "+----------------------------------+" << std::endl;
            std::cout << "| Results:                         |" << std::endl;
            std::cout << "|     original yield = " << observedValues.originalYield << " t/ha   |" << std::endl;
            std::cout << "|     new yield = " << report->finalYield << " t/ha     |" << std::endl;
            std::cout << "|     yield diff = " << report->finalYield - observedValues.originalYield << " t/ha    |" << std::endl;
            std::cout << "|     yield increase = " << report->seasonYieldIncrease << " \%   |" << std::endl;
            std::cout << "|     days over 2°C  = " << report->yieldIncreaseDays << " / " << SEASON_DURATION << "    |" << std::endl;
            std::cout << "|                                  |" << std::endl;
            std::cout << "|     Original net profit = " << observedValues.orignalNetProfit << " € |" << std::endl;
            std::cout << "|     New net profit = " << report->newEarnedMoney << " € |" << std::endl;
            std::cout << "|     Profit difference = " << report->moneyDifference << " € |" << std::endl;
            std::cout << "+----------------------------------+" << std::endl;
        }
    }
}

int main(int argc, char** argv) {
    std::vector<Report*> reports;
    CliParams cliParams = proccessingCliParams(argc, argv); 
    WinterSeasons* winterSeasons = setValuesToSeasons();

    reports = addingNitrogenInWinterSeason(&cliParams, winterSeasons);

    printResult(&cliParams, reports);

    for (Report *report: reports)
        delete report; 

    delete winterSeasons;

    return 0;
}

CliParams proccessingCliParams(int paramCnt, char** paramArgv) {
    std::string help_text = "./adjustnitrogen <-s | -v >\n-s - use for simulating yield increasing\n-v - use for model validating";

    CliParams cliParams = { false, false };
    int character;
    opterr = 0;
    std::string optionParams = "sv";

    while ( (character = getopt(paramCnt, paramArgv, "sv")) != -1) {
        switch (character) {
            case 's':
                cliParams.isSeasonNumber = true;
                break;
            case 'v':
                cliParams.isValidation = true;
                break;
            case '?':
                if (optionParams.find(character) == std::string::npos) {
                    std::cerr << help_text << std::endl;
                    exit(-1);
                }
                break;
        }
    }
    if ( (paramCnt - optind) == 0 ) {
        if ((cliParams.isValidation && cliParams.isSeasonNumber) || 
            (!cliParams.isValidation && !cliParams.isSeasonNumber)) {
            std::cerr << help_text << std::endl;
            exit(-1);
        }
    }
    else {
        std::cerr << help_text << std::endl;
        exit(-1);
    }
    return cliParams;
}

WinterSeasons *setValuesToSeasons() {
    WinterSeasons* winterSeasons = new WinterSeasons;

    winterSeasons->season1 = {9.4, 10.3, 9.4, 7.9, 8.7, 6.0, 8.8, 9.3, 10.8, 9.1, 7.2, 6.1, 5.7, 4.3, 6.6, 6.5, 
                            4.1, 7.0, 7.9, 7.1, 4.8, 7.6, 7.0, 7.4, 1.6, 0.4, -1.0, -2.8, 3.2, 1.7, 2.3, 2.9, 0.2, 
                            -1.0, 3.1, 2.8, 0.8, 1.1, 5.6, 2.9, -0.9, 0.6, -1.3, -1.2, 1.4, 0.4, 0.6, -1.0, -1.7, 
                            -0.2, -0.6, 2.6, 2.3, 5.1, 7.9, 8.6, 5.2, 6.2, 6.3, 3.6, 3.7, 3.2, 4.1, 3.9, 5.9, 8.8, 
                            6.9, 6.8, 5.8, 4.3, 4.6, 1.8, 2.4, -1.2, 5.9, 3.6, 2.5, 5.6, 6.2, 10.4, 9.6, 4.3, 0.3, 
                            -1.1, -0.9, -7.8, -8.3, -4.5, -2.5, -1.3, 0.0, 2.2, -0.3, 1.4, 1.8, 0.4, 1.4, 1.5, 3.0, 
                            4.2, 5.4, 3.8, 5.3, 2.3, 1.8, 4.1, 4.8, 5.6, 5.1, 2.7, 6.5, 3.8, 5.9, 6.6, 6.6, 4.6, 7.4, 
                            8.5, 8.2, 7.3, 8.7, 7.9, 5.8, 8.2, 6.6, 7.8, 6.5, 7.9, 6.7, 5.6, 8.9, 8.9, 7.1, 8.0, 4.8};

    winterSeasons->season2 = {4.6, 7.4, 9.3, 13.0, 14.4, 15.1, 14.8, 10.5, 12.4, 13.6, 14.2, 13.4, 10.4, 9.2, 11.6, 
                            9.7, 9.4, 9.5, 6.4, 6.9, 4.3, 2.2, 5.1, 4.7, 3.0, 2.9, 1.7, 1.8, 2.9, 2.3, 1.4, 2.9, 4.7, 
                            4.8, 5.9, 6.1, 5.2, 4.1, 2.3, -0.5, 0.4, 3.6, 3.3, 1.4, 5.9, 6.4, 4.2, 5.7, 9.0, 7.1, 4.7, 
                            6.4, 8.2, 5.2, 5.6, 0.3, -3.2, -3.6, -5.6, -8.6, -7.9, -3.8, -1.1, 2.8, 1.8, 0.5, -0.7, -5.7, 
                            -4.7, 0.3, 7.7, 4.9, 4.1, 2.0, 2.9, 3.7, 4.7, 5.7, 4.2, 2.5, 2.4, 4.7, 6.2, 6.1, 1.6, 0.1, 
                            -0.2, -2.8, -0.9, -0.5, 0.7, -0.9, -0.3, -1.2, -2.1, 0.3, -3.6, -2.6, -5.4, -1.0, -1.4, 2.8, 
                            2.8, 1.3, 1.6, 3.1, 3.9, 2.8, 0.7, -1.1, -1.6, 1.0, 5.4, 6.0, 6.2, 5.7, 6.4, 5.2, 5.4, 6.4, 5.7, 
                            6.1, 4.4, 2.0, 3.6, 4.2, 2.9, 3.2, 3.5, 5.3, 5.4, 5.3, 6.4, 4.9, 7.2};

    winterSeasons->season3 = {5.6, 4.2, 5.3, 5.4, 1.1, 2.6, 6.7, 10.1, 7.6, 14.8, 13.9, 9.3, 7.1, 8.1, 8.3, 10.9, 7.7, 
                            12.3, 8.8, 9.5, 5.4, 3.3, 1.6, -0.9, -0.8, 0.5, 1.1, 1.1, 4.4, 6.6, 8.5, 5.7, 6.9, 4.9, 5.6, 
                            4.5, 4.2, 4.9, 4.1, 4.5, 1.2, 1.3, 2.3, 2.1, 1.1, 0.7, 1.1, 2.4, 2.9, 2.8, 2.9, 3.6, 1.8, 2.7, 
                            3.7, 2.1, -0.1, 0.3, 1.7, 0.6, -3.4, -3.8, -5.2, -7.5, -5.7, -2.3, -2.0, -2.8, -0.3, 1.4, 3.7, 
                            3.9, 3.7, 2.2, 1.9, 0.0, -3.4, -5.1, -5.4, -3.4, -4.9, -9.6, -6.7, -3.0, -1.6, 0.8, 0.1, 2.1, 2.4, 
                            2.8, 4.1, 5.4, 6.1, 5.2, 2.9, 1.6, 3.9, 6.6, 7.1, 8.9, 4.7, 3.4, 2.4, 3.9, 6.1, 6.5, 4.7, 5.5, 8.2, 
                            4.0, 4.3, 7.6, 12.2, 8.6, 2.1, 1.3, 0.0, 2.2, 7.3, 7.7, 3.3, 4.2, 4.4, 3.7, 6.4, 7.3, 4.8, 3.8, 4.7, 
                            6.7, 7.6, 6.4, 6.6, 4.9, 2.9};

    winterSeasons->season4 = {6.9, 6.2, 4.1, 1.3, 7.3, 8.6, 4.9, 1.2, 0.0, 1.0, 2.6, 1.0, -0.1, -1.6, 0.8, 3.0, 8.1, 11.0, 9.9, 
                            10.9, 10.6, 10.8, 9.5, 8.1, 7.6, 5.8, 0.6, -0.8, -1.9, 2.6, 4.1, -1.1, -3.6, -3.3, -1.6, -2.2, -1.9, 
                            -1.1, 3.5, 1.9, 3.6, -3.0, -3.2, -0.8, -2.4, -3.7, -1.2, -1.4, -2.9, -3.4, -3.2, -2.4, -0.8, 2.9, 7.2, 
                            4.2, 1.8, -0.2, -4.1, -4.8, -5.8, -1.3, 1.3, -3.6, -9.6, -14.5, -12.7, -8.8, -11.3, -12.3, -6.2, 1.2, 
                            0.7, -2.1, -8.8, -3.8, -5.8, -10.4, -11.5, -10.3, -8.1, -4.8, -4.8, -7.2, -3.9, -9.5, -7.7, -7.5, -8.1, 
                            -2.5, -0.1, 0.4, 1.7, 1.1, 2.7, 2.7, -0.2, -1.5, 0.0, 1.2, 1.8, 1.4, -1.7, -1.7, 0.2, -2.6, -1.6, 1.4, 
                            2.1, 0.9, 5.4, 5.6, 10.2, 7.6, 1.8, 3.1, 5.3, 9.7, 6.3, 5.1, 7.1, 12.6, 11.6, 7.1, 4.6, 5.1, 4.3, 5.8, 
                            6.3, 4.3, 4.4, 4.3, 5.4};
    
    return winterSeasons;
}
