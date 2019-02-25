/**
 * @description  main for whole program
 * @author       Jakub Kubik, xkubik32@stud.fit.vutbr.cz
 * @date         07.10.2018 */

#include <iostream>

#include "include/main.h"
#include "include/ArgumentsParser.h"
#include "include/FeedfileParser.h"
#include "include/Client.h"
#include "include/FeedParser.h"

int main(int argc, char **argv) {

    std::vector<std::string> URLs;  // vector for saving URL strings
    ArgumentsParser arg;        

    /* parse program arguments */
    if (arg.parse_arguments(argc, argv) == badInputERR) return badInputERR;


    /* check if -f <filename> or URL was passed to porgram and save URL(s) to vector URLs */
    if (arg.isFeedFileArg()) {
        FeedfileParser f = FeedfileParser(arg.getFeedFileName());

        if (f.saveRequiredURLs(URLs) == fileERR) {
            printError("file (" + arg.getFeedFileName() + ") wasnt oppened successfully");
            return fileERR;
        }
    }
    else { 
        URLs.push_back(arg.getURLAddress());
    }

    /* Cycle for processing all URL(s) inside URL string vector */
    for(unsigned int i = 0; i < URLs.size(); ++i) {

        /* get potentional feed from server, parse and print it */
        Client client = Client(URLs[i]);
        if (client.connect(arg.isCertFile(), arg.getCertFile(), arg.isCertAddr(), arg.getCertAddr()) != clientERR) {
            
            std::string potentionalFeed = client.getFeedContent();
            if (potentionalFeed != serverResponseERR) {
                
                FeedParser feed = FeedParser(potentionalFeed);
                feed.parse();
                feed.printFormated(arg.isReqTime(), arg.isReqAuthor(), arg.isReqURL());
            }
        }
    }
    return 0;
}


void printError(std::string error) {

    std::cerr << "Error: " + error + "\n" << std::endl;
}