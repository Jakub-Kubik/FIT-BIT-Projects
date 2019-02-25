/**
 * @description  header file for class for parsing program arguments
 * @author       Jakub Kubik, xkubik32@stud.fit.vutbr.cz
 * @date         04.11.2018 */

#ifndef ARGUMENTSPARSER_H
#define ARGUMENTSPARSER_H

#include <string>


// @brief data structure for saving checked program arguments
typedef struct {

    bool URL;
    std::string URL_address;
    bool feed_file;
    std::string feed_file_name;

    bool cert_file;
    std::string cert_file_name;
    bool cert_addr;
    std::string cert_addr_dir;

    bool time;
    bool author;
    bool associatedURL;
} Args;


// @brief class for parsing program arguments and showing user how to use program
class ArgumentsParser {

private:
    Args arg;

public:
    ArgumentsParser();

    void help();
    void printStructValues();   // only for testing
    int parse_arguments(int argc, char* argv[]); // parse all program arguments

    bool isFeedFileArg() { return arg.feed_file; };
    std::string getFeedFileName() { return arg.feed_file_name; };

    bool isURLArg() { return arg.URL; };
    std::string getURLAddress() { return arg.URL_address; };

    bool isCertFile() { return arg.cert_file; };
    std::string getCertFile() { return arg.cert_file_name; };

    bool isCertAddr() { return arg.cert_addr; };
    std::string getCertAddr() { return arg.cert_addr_dir; };

    bool isReqTime() {  return arg.time; };
    bool isReqAuthor() { return arg.author; };
    bool isReqURL() { return arg.associatedURL; };
};

#endif // ARGUMENTSPARSER_H
