/**
 * @description  header file for class for parsing feeds
 * @author       Jakub Kubik, xkubik32@stud.fit.vutbr.cz
 * @date         04.11.2018 */

#ifndef FEEDPARSER_H
#define FEEDPARSER_H

#include <iostream>
#include <vector>
#include <libxml/parser.h>
#include <libxml/tree.h>


// @brief enum for feed formats
enum versions {

    RSS1,
    RSS2,
    Atom,
    UNKNOWN
};


// @brief struct for one item in feed
typedef struct items {

    std::string title;
    std::string date;
    std::string author;
    std::string URL;
} items;


// @brief struct for whole feed
typedef struct feedContent {

    versions version;
    std::string title;
    std::vector <items> item;    
} feedContent;


// @brief class for parsing feeds and printing formated output
class FeedParser {

    private:
        std::string feedToParse;
        feedContent feed; 

        void getFeedFormat();
        void parseRSS1();
        void parseRSS2();
        void parseAtom();
        std::string getContent(xmlNode * node);
        std::string getProp(xmlNode * node, const char * prop);

    public:
        FeedParser(std::string feed) { feedToParse = feed; };

        int parse();
        int printFormated(bool time, bool author, bool path);
};

#endif // FEEDPARSER_H