/**
 * @description  header file for client class
 * @author       Jakub Kubik, xkubik32@stud.fit.vutbr.cz
 * @date         04.11.2018 */

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>


// @brief size of response from server
const int sizeOfResponse = 1024;


// @brief struct for saving parsed URL items
typedef struct {
    bool HTTP;
    
    std::string hostName;
    std::string pathToFeed;

    bool Port;
    int portNumber;
}ParsedUrl;


// @brief class for getting feed content from server
class Client {

    private:
        ParsedUrl parsedURL;    // struct for saving parsed parts of URL
        BIO * bio;
        SSL * ssl;
        SSL_CTX * ctx;

        bool connectHTTP();
        bool connectHTTPS(bool certFile, std::string certFileName, bool certDir, std::string certDirName);
        std::string getHostNameAndPort();

    public:
        Client(std::string URL);
        void URL_parse(std::string);

        int connect(bool certFile, std::string certFileName, bool certDir, std::string certDirName);
        std::string getFeedContent();   // return feed or err
};

#endif // CLIENT_H
