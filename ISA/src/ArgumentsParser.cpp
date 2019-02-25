/**
 * @descripion  clas for parsing program argumenets
 * @author      Jakub Kubik, xkubik32@stud.fit.vutbr.cz
 * @date        04.11.2018 */

#include <iostream>
#include <unistd.h>
#include <regex>  
#include <getopt.h>

#include "include/ArgumentsParser.h" 
#include "include/main.h"


ArgumentsParser::ArgumentsParser() {

    arg.URL = false;
    arg.feed_file = false;
    arg.cert_file = false;
    arg.cert_file_name = "";
    arg.cert_addr = false;
    arg.cert_addr_dir = "";
    arg.time = false;
    arg.author = false;
    arg.associatedURL = false;
}


void ArgumentsParser::help() {

    std::string help = "feedreader - program for printing information in feeds\n" +
            std::string("#######################################################\n") +  // because of '+' overloading
                        "USAGE: <URL | -f <feedfile>>[-c <certfile>][-C <certaddr>][-T][-a][-u]\n" +
                        "#######################################################################\n" +
                        "required: URL or -f with filename.\n" +
                        "optional: -c with certfile (file with certificates).\n" +
                        "          -C with certaddr (directory where to check out certifcates).\n" +
                        "          -T (show information about time of record creation).\n" +
                        "          -a (show name of record author).\n" +
                        "          -u (show asociated URL with record).\n" +
                        "          -h (print help to the program arguments).\n\n";

    std::cout << help << std::endl;
}


int ArgumentsParser::parse_arguments(int argc, char *argv[]) {

    const char * const short_opts = "f:c:C:Tauh";
    int opt;

    opterr = 0; // belongs to getopt

    while ( (opt = getopt(argc, argv, short_opts)) != -1  ) {

        switch (opt) {

            case 'f':
                if (arg.feed_file) {
                    printError("Duplicity of -f");
                    this->help();
                    return badInputERR; 
                }
                arg.feed_file = true;
                arg.feed_file_name = optarg;
                break;

            case 'c':
                if (arg.cert_file) {
                    printError("Duplicity of -c");
                    this->help();
                    return badInputERR;
                }
                arg.cert_file = true;
                arg.cert_file_name = optarg;
                break;

            case 'C':
                if (arg.cert_addr) {
                    printError("Duplicity of -C");
                    this->help();
                    return badInputERR;
                }
                arg.cert_addr = true;
                arg.cert_addr_dir = optarg;
                break;

            case 'T':
                if (arg.time) {
                    printError("Duplicity of -T");
                    this->help();
                    return badInputERR;
                } 
                arg.time = true;
                break;

            case 'a':
                if (arg.author) {
                    printError("Duplicity of -a");
                    this->help();
                    return badInputERR;
                } 
                arg.author = true;
                break;

            case 'u':
                if (arg.associatedURL) {
                    printError("Duplicity of -u");
                    this->help();
                    return badInputERR;
                }
                arg.associatedURL = true;
                break;

            case 'h': // -help option for program
                this->help();
                return badInputERR;
                break;
            
            default:    // no allowed option ot word
                printError("Bad options");
                this->help();
                return badInputERR;
        }
    }
    /* check for HTTP or HTTPS program arguments */
    for (int index = optind; index < argc; index++) {

        std::regex rgxURL("^(http://www.|https://www.|http://|https://)[a-z0-9]+([-.]{1}[a-z0-9]+)*(.[a-z]{2,5})?(:[0-9]{1,5})?(/.*)?$");

        if (arg.URL) { // duplicity checking
                printError("Duplicity of -URL");
                this->help();
                return badInputERR;
        }
        /* check URL format with regex */
        if ( !(regex_match(argv[optind], rgxURL)) ) {
            printError("Wrong format for URL address");
            this->help();
            return badInputERR;
        }
        else {
            arg.URL = true;
            arg.URL_address = argv[optind];
        }
    }

    /* handle not allowed combinations of program arguments */
    if ( !(arg.URL || arg.feed_file) ) { // none of req param is used
        printError("Required parameters <URL | -f <feedfile>> werent used");
        this->help();
        return badInputERR;
    }
    if (arg.URL && arg.feed_file) { // both req param are used
        printError("Both parameters <URL | -f <feedfile>> were used, but only one is required");
        this->help();
        return badInputERR;
    }

    return 0;
}


void ArgumentsParser::printStructValues() {

    if ( arg.URL ) std::cout << "URL address is: " << arg.URL_address << std::endl;
    
    else if ( arg.feed_file ) std::cout << "-f <feedfile> is: " << arg.feed_file_name << std::endl;

    if ( arg.cert_file ) std::cout << "-c <cert> is: " << arg.cert_file_name << std::endl;
    if ( arg.cert_addr ) std::cout << "-C <certaddr> is: " << arg.cert_addr_dir << std::endl;
    
    if ( arg.time ) std::cout << "Time option" << std::endl;
    if ( arg.author ) std::cout << "Author option" << std::endl;
    if ( arg.associatedURL ) std::cout << "Associated URL option" << std::endl;
}
