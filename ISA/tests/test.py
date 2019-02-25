

# Referential output
# --------------------------------
feedOutput = dict()

# test 1
feedOutput.update({'atom/1/noParam' : '''***Example Feed***
Atom-Powered Robots Run Amok

''' })

# test 2
feedOutput.update({'atom/1/allParam' : '''***Example Feed***
Atom-Powered Robots Run Amok
Aktualizace: 2003-12-13T18:30:02Z
Autor: John Doe (johndoe@example.com), 
URL: http://example.org/2003/12/13/atom03/edit

''' })

# test 3
feedOutput.update({'atom/2/noParam' : '''***MY title***
Atom-Powered Robots Run Amok

'''})

# test 4
feedOutput.update({'atom/2/allParam' : '''***MY title***
Atom-Powered Robots Run Amok
Aktualizace: 2003-12-13T18:30:02Z
Autor: John Doe (johndoe@example.com), 
URL: http://example.org/2003/12/13/atom03/edit

''' }) 

# test 5
feedOutput.update({'rss1/1/noParam' : '''***XML.com***
Processing Inclusions with XSLT
Putting RDF to Work

''' }) 

# test 6
feedOutput.update({'rss1/1/allParam' : '''***XML.com***
Processing Inclusions with XSLT
Aktualizace: 13.7.1996
Autor: Simon St.Laurent (mailto:simonstl@simonstl.com)
URL: http://xml.com/pub/2000/08/09/xslt/xslt.html

Putting RDF to Work
Aktualizace: 13.7.1996
Autor: Simon St.Laurent (mailto:simonstl@simonstl.com)
URL: http://xml.com/pub/2000/08/09/rdfdb/index.html

''' }) 

# test 7
feedOutput.update({'rss1/2/noParam' : '''***XML.com***
Processing Inclusions with XSLT
Putting RDF to Work

''' }) 

# test 8
feedOutput.update({'rss1/2/allParam' : '''***XML.com***
Processing Inclusions with XSLT
Aktualizace: 
Autor: 
URL: http://xml.com/pub/2000/08/09/xslt/xslt.html

Putting RDF to Work
Aktualizace: 13.7.1996
Autor: Simon St.Laurent (mailto:simonstl@simonstl.com)
URL: http://xml.com/pub/2000/08/09/rdfdb/index.html

''' }) 

# test 9
feedOutput.update({'rss2/1/noParam' : '''***RSS Title***
Example entry
Next header

''' }) 

# test 10
feedOutput.update({'rss2/1/allParam' : '''***RSS Title***
Example entry
Aktualizace: Sun, 06 Sep 2009 16:20:00 +0000
Autor: Johny Mrkvicka
URL: http://www.example.com/blog/post/1

Next header
Aktualizace: 
Autor: 
URL: http://www.example.com/blog/post/1

''' }) 

# test 11
feedOutput.update({'rss2/2/noParam' : '''***RSS Title***
Example entry
Next header

''' }) 

# test 12
feedOutput.update({'rss2/2/allParam' : '''***RSS Title***
Example entry
Aktualizace: Sun, 06 Sep 2009 16:20:00 +0000
Autor: Johny Mrkvicka
URL: http://www.example.com/blog/post/1

Next header
Aktualizace: 
Autor: 
URL: http://www.example.com/blog/post/1

''' }) 

import sys
import os
from subprocess import Popen, PIPE, DEVNULL

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


class Test:

    def __init__(self):
        self.number = 0
        self.succ = 0
        print(bcolors.BOLD + bcolors.WARNING + 
        """
        ===============================================
            Automated testing of program feedreader
            with using local server
        ===============================================
        """ + bcolors.ENDC + bcolors.BOLD
        )


    def check(self, args, referential):
        if "allParam" in referential:
            result = Popen(['./feedreader', args, "-Tua"], stdout=PIPE)
        else:
            result = Popen(['./feedreader', args], stdout=PIPE)


        out = result.communicate()[0].decode('utf-8')
        feedreaderOutput = out.replace(args + '\n\n' , "")
        self.number = self.number + 1
        print("=============================================================")
        print(bcolors.BOLD + "test" + str(self.number) + ": " + bcolors.OKGREEN, end="")
        
        if (feedreaderOutput == feedOutput[referential]): 
            print(bcolors.OKGREEN +  "[OK]" + bcolors.ENDC)
            self.succ = self.succ + 1
            print("\n" + bcolors.BOLD + "URL to feedreader   <--->   type of testing:" + bcolors.ENDC)
            print(args + "   <--->   " + referential + "\n")
        else:
            print(bcolors.FAIL + "[FAIL]" + bcolors.ENDC)
            print("\n" + bcolors.BOLD + "URL to feedreader   <--->   type of testing:" + bcolors.ENDC)
            print(args + "   <--->   " + referential + "\n")
            print(bcolors.BOLD + "Parsed from feedreader:\n" + bcolors.ENDC + feedreaderOutput)
            print( bcolors.BOLD + "Referential output:\n" + bcolors.ENDC + feedOutput[referential])

        
        #print(str(len(feedreaderOutput)) + " " + str(len(feedOutput[referential])))


    def result(self):
        print( bcolors.BOLD + 
        """ 
                Overall tests overview 
        =================================== 
        """ +
                bcolors.OKGREEN + "\tpassed: " + str(self.succ) + "\n" +
                bcolors.FAIL +  "\t\tnot passed: " + str(self.number - self.succ) + "\n" +
                bcolors.OKBLUE + "\t\tsuccess: {:.2f}".format((self.succ / self.number) * 100)  + "%" +
                bcolors.ENDC +  
        """
        ===================================
        """
        )


def main():

    port = 5300
    test = Test()

    # process all tests
    test.check('http://localhost:' + str(port) +'/atom/1', 'atom/1/noParam' )  # test1
    test.check('http://localhost:' + str(port) +'/atom/1', 'atom/1/allParam' ) # test2
    test.check('http://localhost:' + str(port) +'/atom/2', 'atom/2/noParam' )  # test3
    test.check('http://localhost:' + str(port) +'/atom/2', 'atom/2/allParam' ) # test4

    test.check('http://localhost:' + str(port) +'/rss1/1', 'rss1/1/noParam' )  # test5
    test.check('http://localhost:' + str(port) +'/rss1/1', 'rss1/1/allParam' ) # test6
    test.check('http://localhost:' + str(port) +'/rss1/2', 'rss1/2/noParam' )  # test7
    test.check('http://localhost:' + str(port) +'/rss1/2', 'rss1/2/allParam' ) # test8
    
    test.check('http://localhost:' + str(port) +'/rss2/1', 'rss2/1/noParam' )  # test9
    test.check('http://localhost:' + str(port) +'/rss2/1', 'rss2/1/allParam' ) # test10
    test.check('http://localhost:' + str(port) +'/rss2/2', 'rss2/2/noParam' ) # test11
    test.check('http://localhost:' + str(port) +'/rss2/2', 'rss2/2/allParam' ) # test12

    test.result()



if __name__ == '__main__':
    main()