/**
 * @description  header file for class for parsing feedfile
 * @author       Jakub Kubik, xkubik32@stud.fit.vutbr.cz
 * @date         04.11.2018 */

#ifndef FEEDFILEPARSER_H
#define FEEDFILEPARSER_H

#include <iostream>
#include <fstream>
#include <vector>


// @brief class for parsing feedfile content
class FeedfileParser {

    private:
        std::fstream fs; // file handler

    public:
        FeedfileParser(std::string fileName);
        int saveRequiredURLs(std::vector<std::string> &URLs); // save URLs from feedfile to URLs vector
};

#endif // FEEDFILEPARSER_H