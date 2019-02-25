/**
 * @description  class for parsing feeds
 * @author       Jakub Kubik, xkubik32@stud.fit.vutbr.cz
 * @date         04.11.2018 */

#include <string.h>

#include "include/FeedParser.h"
#include "include/main.h"


int FeedParser::parse() {

    getFeedFormat();

    switch(feed.version) {

        case(RSS1):
            parseRSS1();
            break;
        
        case(RSS2):
            parseRSS2();
            break;
        
        case(Atom):
            parseAtom();
            break;
        
        default:
            printError("Unsuported feed format");
    }

    return OK;
}


void FeedParser::parseRSS1() {

    xmlDoc *doc = NULL;
    xmlNode *rootNode = NULL;

    doc = xmlParseMemory(feedToParse.c_str(), feedToParse.length());
    rootNode = xmlDocGetRootElement(doc);

    /* whole feed */
    for (xmlNode *node = rootNode->children; node != NULL; node = node->next) {

        /* channel */
        if (strcmp((const char *)node->name, "channel") == 0) {

            /* walking through elements inside channel */
            for (xmlNode *nodeInChannel = node->children; nodeInChannel != NULL; nodeInChannel = nodeInChannel->next) {
                if (strcmp((const char *)nodeInChannel->name, "title") == 0) {
                    feed.title = getContent(nodeInChannel);
                }
            }
        }

        /* item */
        else if (strcmp((const char *)node->name, "item") == 0) {

            items item;

            for (xmlNode *nodeInItem = node->children; nodeInItem != NULL; nodeInItem = nodeInItem->next) {
                
                if (strcmp((const char *)nodeInItem->name, "title") == 0) {
                    item.title = getContent(nodeInItem);
                }

                if (strcmp((const char *)nodeInItem->name, "date") == 0) {
                    item.date = getContent(nodeInItem);
                }

                if (strcmp((const char *)nodeInItem->name, "creator") == 0) {
                    item.author = getContent(nodeInItem);
                }

                else if (strcmp((const char *)nodeInItem->name, "link") == 0) {
                    item.URL  = getContent(nodeInItem);
                }
            }

            feed.item.push_back(item);
        }
    }

    xmlFreeDoc(doc);       // free document
    xmlCleanupParser();    // Free globals
}


void FeedParser::parseRSS2() {

    xmlDoc *doc = NULL;
    xmlNode *rootNode = NULL;

    doc = xmlParseMemory(feedToParse.c_str(), feedToParse.length());
    rootNode = xmlDocGetRootElement(doc);

    /* whole feed */
    for (xmlNode *node = rootNode->children; node != NULL; node = node->next) {

        xmlNode *nodeChannels = node;

        /* in RSS2 is only one channel element for whole feed */
        if ( strcmp((const char *)nodeChannels->name, "channel") == 0 ) {

            /* channel */
            for (nodeChannels = nodeChannels->children; nodeChannels != NULL; nodeChannels = nodeChannels->next) {
                
                /* title of feed */
                if (strcmp((const char *)nodeChannels->name, "title") == 0) {
                    feed.title = getContent(nodeChannels);
                }

                /* item */
                else if (strcmp((const char *)nodeChannels->name, "item") == 0) {

                    items item;

                    for (xmlNode *nodeInItem = nodeChannels->children; nodeInItem != NULL; nodeInItem = nodeInItem->next) {
                        
                        if (strcmp((const char *)nodeInItem->name, "title") == 0) {
                            item.title = getContent(nodeInItem);
                        }
                        else if (strcmp((const char *)nodeInItem->name, "author") == 0) { 
                            item.author = getContent(nodeInItem);
                        }
                        else if (strcmp((const char *)nodeInItem->name, "pubDate") == 0) { 
                            item.date = getContent(nodeInItem);
                        }
                        
                        else if (strcmp((const char *)nodeInItem->name, "link") == 0) { 
                            item.URL = getContent(nodeInItem);
                        }
                    }

                    feed.item.push_back(item);
                }
            }
        
        } // end of loop for channels
    
    }
    xmlFreeDoc(doc);       // free document
    xmlCleanupParser();    // Free globals
}


void FeedParser::parseAtom() {

    xmlDoc *doc = NULL;
    xmlNode *rootNode = NULL;

    doc = xmlParseMemory(feedToParse.c_str(), feedToParse.length());
    rootNode = xmlDocGetRootElement(doc);

    /* whole feed */
    for (xmlNode *node = rootNode->children; node != NULL; node = node->next) {

        /* title */
        if (strcmp((const char *)node->name, "title") == 0) {
            feed.title = getContent(node);
        }

        /* item */
        else if (strcmp((const char *)node->name, "entry") == 0) {

            items item;
            std::string updated;

            for (xmlNode *nodeInEntry = node->children; nodeInEntry != NULL; nodeInEntry = nodeInEntry->next) {
                
                if (strcmp((const char *)nodeInEntry->name, "title") == 0) {
                    item.title = getContent(nodeInEntry);
                }

                else if (strcmp((const char *)nodeInEntry->name, "published") == 0) {
                    item.date = getContent(nodeInEntry);
                }
                else if (strcmp((const char *)nodeInEntry->name, "updated") == 0) {
                    updated = getContent(nodeInEntry);
                } 

                else if (strcmp((const char *)nodeInEntry->name, "author") == 0) {
                    
                    for (xmlNode * authorNode = nodeInEntry->children; authorNode != NULL; authorNode = authorNode->next) {
                        if (strcmp((const char *)authorNode->name, "name") == 0) {
                            item.author += getContent(authorNode);
                        }
                        if (strcmp((const char *)authorNode->name, "email") == 0) {
                            item.author += " (" + getContent(authorNode) + "), ";
                        }
                    }
                }
                
                else if (strcmp((const char *)nodeInEntry->name, "link") == 0) { 
                    
                    const char * prop = "href";
                    xmlChar * value;
                    value = xmlGetProp(nodeInEntry, (xmlChar *)prop);
                    item.URL = (const char*)value;
                    xmlFree(value);
                }
            }

            if (item.date == "") {
                item.date = updated;
            }

            feed.item.push_back(item);
        }
    }
    xmlFreeDoc(doc);       // free document
    xmlCleanupParser();    // Free globals
}


std::string FeedParser::getContent(xmlNode * node)
{
    std::string retval;
    if (node) {

        xmlChar * content = xmlNodeGetContent(node);
        if (content) {

            retval = (const char *)content;
            xmlFree(content);
        }
    }
    return retval;
}


std::string FeedParser::getProp(xmlNode * node, const char * prop) {
    std::string retval;
    
    xmlChar * value;
    
    value = xmlGetProp(node, (xmlChar *)prop);
    if (value) {

        retval = (const char*)value;
        xmlFree(value);
    }
    
    return retval;
}


void FeedParser::getFeedFormat() {

    xmlDoc *doc = NULL;
    xmlNode *node = NULL;

    doc = xmlParseMemory(feedToParse.c_str(), feedToParse.length());
    node = xmlDocGetRootElement(doc);

    if (node) {

        if (node->name && node->type == XML_ELEMENT_NODE) {

            if ((strcmp((const char *)node->name, "RDF")) == 0) {
                feed.version = RSS1;
            }

            else if ((strcmp((const char *)node->name, "rss")) == 0) {
                const char * version = (const char *)xmlGetProp(node, (const xmlChar *)"version");
				
                if (!version) { 
					xmlFree((void *)version);
                }
                else if (strcmp(version, "2.0") == 0) {
                    feed.version = RSS2;
                }
            }
            
            else if ((strcmp((const char *)node->name, "feed")) == 0) {
                feed.version = Atom;
            }
            else {
                feed.version = UNKNOWN;
            }
        }
        else {
            feed.version = UNKNOWN;
        }
    }
    else {
        feed.version = UNKNOWN;
    }

    xmlFreeDoc(doc);       // free document
    xmlCleanupParser();    // Free globals
}


int FeedParser::printFormated(bool time, bool author, bool path) {

    std::cout << "***" << feed.title << "***" << std::endl;
    
    for(unsigned int j=0; j < feed.item.size(); j++) {
        
        std::cout << feed.item[j].title << std::endl;

        if (time) std::cout << "Aktualizace: " << feed.item[j].date << std::endl;
        if (author) std::cout << "Autor: " << feed.item[j].author << std::endl;
        if (path) std::cout << "URL: " << feed.item[j].URL << std::endl;

        /* if is optional parameter then make an empty line between feeds in one feed channel */
        if (time || author || path)
            std::cout << std::endl;
    }
    /* one space between feed channels */
    if (!time && !author && !path)
            std::cout << std::endl;

    return OK;
}