/**
 * @description  class for parsing feedfile
 * @author       Jakub Kubik, xkubik32@stud.fit.vutbr.cz
 * @date         04.11.2018 */

#include <iostream>
#include <fstream> 
#include <regex> 

#include "include/FeedfileParser.h" 
#include "include/main.h" 


FeedfileParser::FeedfileParser(std::string fileName) {

    fs.open(fileName, std::fstream::in);
}


int FeedfileParser:: saveRequiredURLs(std::vector<std::string> &URLs) {

    if (!fs.is_open()) {
        return fileERR;
    }
    
    std::string line;
    int nmbOfLine = 0;

    /* get whole line from file */
    while(std::getline(fs, line)){

        nmbOfLine++;

        /* jump over empty lines */
        if (!line.empty()) {

            std::regex rgxURL("(^(?:http(s)?://)[\\w.-]+(?:\\.[\\w\\.-]+)+[\\w\\-\\._~:/?#[\\]@!\\$&'\\(\\)\\*\\+,;=.]+)");
            std::regex rgxComment("^#.*");
            std::regex rgxSpacesAndEol("^\\s+");

            std::smatch match;

            while ( !(line.empty()) ) {

                /* check for empty space and eol */
                if (std::regex_search(line, match, rgxSpacesAndEol)) {
                    line = match.suffix();
                }
                /* check for comments */
                else if (std::regex_search(line, match, rgxComment)) {
                    line = match.suffix();
                }
                /* check for URLs */
                else if (std::regex_search(line, match, rgxURL)) {
                    URLs.push_back(match[0]);
                    line = match.suffix();
                }
                /* get only to the first error on line */
                else {
                    printError("Feedfile -> wrong format on line " + nmbOfLine);
                    line = "";
                }
            }
        }
    }
    fs.close();

    return OK;    
}