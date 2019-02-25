/**
 * @description class for client
 * @author Jakub Kubik, xkubik32@stud.fit.vutbr.cz
 * @date 04.11.2018 */

#include <regex>
#include <sstream>

#include "include/Client.h"
#include "include/main.h"


Client::Client(std::string URL ) {

    URL_parse(URL);

   /* Set up the library for Bio sockets */
    ERR_load_BIO_strings();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
}


int Client::connect(bool certFile, std::string certFileName, bool certDir, std::string certDirName) {

    /* check if is used HTTP or HTTPS */
    if (parsedURL.HTTP) {
        if ( !connectHTTP()) 
            return clientERR;
    }
    else {
        if (!connectHTTPS(certFile, certFileName, certDir, certDirName)) 
            return clientERR;
    }
    return OK;
}


std::string Client::getHostNameAndPort() {

    std::string path;
    
    if (parsedURL.Port) {
        path =  parsedURL.hostName + ":" + std::to_string(parsedURL.portNumber);
    }
    else if (parsedURL.HTTP){
        path = parsedURL.hostName + ":80";
    }
    else { 
        path = parsedURL.hostName + ":443";
    }
    
    return path;
}


bool Client::connectHTTP() {
    
    bio = BIO_new_connect(parsedURL.hostName.c_str());
    if(bio == NULL) { 

        printError("BIO object wasnt created"); 
        return false; 
    }
    if (parsedURL.Port) {
        BIO_set_conn_port(bio, std::to_string(parsedURL.portNumber).c_str());
    }
    else {
        BIO_set_conn_port(bio, "80");
    }
    

    if(BIO_do_connect(bio) <= 0) {

        printError("atemptiong to connect to server " + parsedURL.hostName);
        BIO_free_all(bio);
        return false;
    }

    return true;
}


bool Client::connectHTTPS(bool certFile, std::string certFileName, bool certDir, std::string certDirName) {

    /* Set up the SSL context */
    SSL_library_init();
    ctx = SSL_CTX_new(SSLv23_client_method());
    SSL *ssl;

    
    /* Setup certificate location */
    if (certFile && certDir) {
        SSL_CTX_load_verify_locations(ctx, certFileName.c_str(), certDirName.c_str());
    }
    else if (certFile) {
        SSL_CTX_load_verify_locations(ctx, certFileName.c_str(), NULL);
    }
    else if (certDir) {
        SSL_CTX_load_verify_locations(ctx, NULL, certDirName.c_str());
    }
    else {
        SSL_CTX_set_default_verify_paths(ctx); // this is by default

    }

    /* Setup the connection */
    bio = BIO_new_ssl_connect(ctx);

    /* Set the SSL_MODE_AUTO_RETRY flag */
    BIO_get_ssl(bio, & ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);


    /* Create and setup the connection */
    BIO_set_conn_hostname(bio, parsedURL.hostName.c_str());
    if (parsedURL.Port) {
        BIO_set_conn_port(bio, std::to_string(parsedURL.portNumber).c_str());
    }
    else {
        BIO_set_conn_port(bio, "443");
    }

    if(BIO_do_connect(bio) <= 0) {

        printError("atemptiong to connect to server " + parsedURL.hostName);
        BIO_free_all(bio);
        SSL_CTX_free(ctx);
        return false;
    }

    /* check certificates */
    if (SSL_get_peer_certificate(ssl)) {

        if (SSL_get_verify_result(ssl) != X509_V_OK) {
            printError("Certificate verification on server " + parsedURL.hostName);
            return false;
        }
    }
    return true;
}


std::string Client::getFeedContent() {

    std::string request =  "GET " + parsedURL.pathToFeed +  " HTTP/1.0\r\n" 
                            "Host: " + parsedURL.hostName + "\r\n" 
                            "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:62.0) Gecko/20100101 Firefox/62.0\r\n"
                            "Connection: Close\r\n\r\n";

    //std::cout << request << std::endl;

    /* send a get request */
    BIO_write(bio, request.c_str(), request.length());

    /* Read a whole response */ 
    char response[sizeOfResponse];
    std::string feed;
    int p;
    for(;;)
    {
        p = BIO_read(bio, response, sizeOfResponse - 1);    // in p is saved size of response from server
        if(p <= 0) break;
        response[p] = 0;
        feed += response;
    }
    //std::cout << "Response: " << feed << std::endl;

    /* Close the connection and free the context */
    BIO_free_all(bio);
    
    /* remove certificates */
    if (!parsedURL.HTTP) SSL_CTX_free(ctx);


    /* Cutt off the head from http response */
    std::regex rgxOKResponse("200 OK"); // return code is every time first line in http header response
    std::regex rgxXML("\r\n\r\n"); // http header ends every time with \r\n\r\n
    std::smatch match;

    if (std::regex_search(feed, match, rgxOKResponse)) {

        if (std::regex_search(feed, match, rgxXML)) {

            feed = match.suffix();
        }
        return feed;
    }

    /* respose error code */
    else {
        int i = 0;
        char c;
        do {
            c = feed[i++];
            std::cerr << c;
        } while( c != '\n' );

        printError("wrong response from Server with path (" 
                    + parsedURL.hostName + parsedURL.pathToFeed + ")");

        return serverResponseERR;
    }
}


void Client::URL_parse(std::string URL) {

    std::regex rgxHTTP("^http://");
    std::regex rgxHTTPS("^https://");
    std::regex rgxHostName("(.*?((:)|(/)))");
    std::regex rgxPort("^((0|[1-9][0-9]*))");

    std::smatch match;

    /* check if URL is HTTP or HTTPS */ 
    if (std::regex_search(URL, match, rgxHTTP))         parsedURL.HTTP = true;  
    else if (std::regex_search(URL, match, rgxHTTPS))   parsedURL.HTTP = false;
    URL = match.suffix();

    /* save host name */
    if (std::regex_search(URL, match, rgxHostName)) {
        parsedURL.hostName = match[0];
        // remove slash from host for connnecting
        parsedURL.hostName = parsedURL.hostName.substr(0, parsedURL.hostName.size()-1);
        URL = match.suffix();
    }

     /* check port and if is then save */
    if (std::regex_search(URL, match, rgxPort)) {
        std::string p = match[0];

        parsedURL.Port = true;
        parsedURL.portNumber = atoi(p.c_str());
        URL = match.suffix();
    }
    else parsedURL.Port = false;

    /* save path to feed */
    parsedURL.pathToFeed = "/";
    parsedURL.pathToFeed += URL;
}