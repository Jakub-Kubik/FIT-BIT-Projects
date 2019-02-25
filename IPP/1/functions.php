<?php
    # ##########################################################
    #
    #      Jan Jakub Kubik (xkubik32)
    #      VUT FIT 2BIA 
    #      xkubik32@stud.fit.vutbr.cz
    #      date: 13.3.2018
    #      description: All my function
    #            
    # ##########################################################


     ############### function for parsing program parameters ###############
     function checkParameters() {

        global $argc, $argv;
        global $isLoc, $isComment;
        global $isLocFirst;
        global $fileName;
        global $isExtension;

        $regV = "/^--stats=.*?$/"; # regular expression for filename

        $isLocFirst = false;

        $i = 2;
        $j = 3;
        $fNameOK = false;


        if ( $argc == 1 ) return OK; # no arguemnts
        if ( $argc <= 4 ) {
            if ( ((strcmp($argv[1], "--help")) == 0) && $argc == 2 ) {
               echo "\n\n" .
                    "\t==============================================================\n" .
                    "\t    This script reads from stdin source code of IPPcode18.\n" .
                    "\t    Check lexical and syntax correctness of source code\n" .
                    "\t    and write on stdout XML representation of source code\n" .
                    "\t==============================================================\n\n\n";
               exit(OK);
            }

            // extension STATP checking arguemnts ##########################################
            else if ( (preg_match($regV, $argv[1]) == 1) && ($argc > 2) ) {
                if ( strlen($argv[1]) < 9 ) exit(PARAMERR); # check if filename isnt empty
                $fileName = substr($argv[1], 8);
                $fNameOK = true;
            }
            else if ( (preg_match($regV, $argv[2]) == 1) && ($argc > 2) ) {
                if ( strlen($argv[2]) < 9 ) exit(PARAMERR); # check if filename isnt empty
                $fileName = substr($argv[2], 8);
                $fNameOK = true;
                $i = 1;
                $j = 3;
            }
            else if ( $argc == 4 )
            if ( (preg_match($regV, $argv[3]) == 1) && ($argc > 2) ) {
                if ( strlen($argv[3]) < 9 ) exit(PARAMERR); # check if filename isnt empty
                $fileName = substr($argv[3], 8);
                $fNameOK = true;
                $i = 1;
                $j = 2;
            }
                
            if ( $fNameOK ) {
                if ( $argc == 3 ) {
                    if ((strcmp($argv[$i], "--loc")) == 0) {
                        $isLoc = true;
                        $isLocFirst = $isLoc;
                        return OK;   # OK
                    }
                    else if ((strcmp($argv[$i], "--comments")) == 0) {
                        $isComment = true;
                        return OK;   # OK
                    }
                    else exit(PARAMERR);
                    
                }
                else if ( $argc == 4 ) {
                    if ( ((strcmp($argv[$i], "--loc")) == 0) && ((strcmp($argv[$j], "--comments")) == 0) ) {
                        $isLoc = true;
                        $isLocFirst = true;
                    }    
                    else if ( ((strcmp($argv[$i], "--comments")) == 0) && ((strcmp($argv[$j], "--loc")) == 0) ) {
                        $isComment = true;
                        $isLoc = false;
                    }
                    else { # not allowed arguments
                        error_log("Error: not allowed in 3 arg  program arguments\n");
                        exit(PARAMERR);
                    }
                }
            }
            else { # not allowed arguments
                error_log("Error: not allowed not matching filename program arguments\n");
                exit(PARAMERR);
            }
        }
        else { # not allowed number of arguments
            error_log("Error: not allowed number of program arguments\n");
            exit(PARAMERR);
        }
    }

    ############### function for saving STDIN to array ###############
    function stdinToArray() {

        $arrayStdinNotComplet = array();
        $arrayStdinComplet = array();

        while ($in = fgets(STDIN) ) {
            // function explode breaks string into an array
            $arrayStdinNotComplet = array_merge($arrayStdinNotComplet, explode("\n", $in)); 
        }

        $i = 0;
        // remove empty lines
        foreach($arrayStdinNotComplet as $value) {
            if ( $value != "" ) {
                $arrayStdinComplet[$i] = $value;
                $i = $i + 1;
            }
        }
        return $arrayStdinComplet;
    }

    ############### Remeove coments and tabs and whitespaces from array ###############
    function arrayRemoveCommentsWhitespaces() {

        global $array;
        global $isComment, $nmbOfComments;
        global $isLoc, $nmbOfInstr;

        $newArray = array();
        
        $i = 0;
        foreach ( $array as $value ) {

            $position = strpos($value, "#");

            // remove comments if is on the line
            if ( $position !== false ) {
                $value = substr($value, 0, $position);
                if ( $isComment ) $nmbOfComments++;
            }                             
                                
            $value = trim(preg_replace('/\t+/', '', $value));  # remove tabulators
            $value = preg_replace("/^\s+|\s+$|\s+(?=\s)/", "", $value); # remove whitespaces except first

            // convert instruction to upper case because instruction are case insesnsitive
            $value = explode(" ", $value);  # make array from string
            $value[0] = strtoupper($value[0]);
            $value = implode(" ",$value);   # make string from array

            if ( ($position !== 0) && ((strcmp($value, "")) != 0) ) {   
                $newArray[$i] = $value;                                 
                $i = $i + 1;
            }
        }
        // it is used only for extension
        $nmbOfInstr = $i - 1;  

        return $newArray;
    }

    ############### function for lexical analysis ###############
    function lexicalAndSyntaxAnalysis() {
        
        global $arrayNoComments;
        global $keyWordsWidthAttrib;
        global $regSymb;
        global $regString;
        

        $codeType = true;

        if (( count($arrayNoComments)) == 0 ) {
            error_log("ERROR: header .ippcode18 wasnt written\n");
            exit(LEXSYNERR);
        }
        foreach ( $arrayNoComments as $value ) {
            
            $value = explode(" ", $value);  # make array from string
            
            if ( $codeType == true ) {  # check header of source code
        
                if ( strcmp($value[0], ".IPPCODE18") != 0 ) {
                    error_log("ERROR: in header of file\n");
                    exit(LEXSYNERR);
                }
                $codeType = false; # variable for checking validity of .ippcode18
            }
            else {
                $validInstruction = false; # var for checking validity of instruction
                foreach ( $keyWordsWidthAttrib as $instruction ) {  
        
                    // check instruction name
                    if ( (strcmp($value[0], $instruction[0])) == 0 ) {
                        $validInstruction = true;
                        
                        // check number of instruciton arguments
                        $number = count($value);
                        if ( $number !=  $instruction[1]+1 ) {
                            error_log("ERROR: wring number oof arguemnts " . $instruction[0] . "\n");
                            exit(LEXSYNERR); # Lexical error
                        }

                        // check appropiate format of instruction arguments by regular expression
                        for ( $i = 0; $i < $instruction[1]; $i++ ) {
                            if ( strcmp($instruction[$i+2], "REGSYMB") == 0 ) {
                                if (( (preg_match($regSymb, $value[$i+1])) == 0 ) && ((preg_match($regString, $value[$i+1])) == 0 )) {
                                    error_log("ERROR: Instruction argument doesnt match" . $value[$i+1] . "\n");
                                    exit(LEXSYNERR); # Lexical error
                                }
                            }
                            else if ( (preg_match($instruction[$i+2], $value[$i+1])) == 0 ) {
                                error_log("ERROR: Instruction argument doesnt match" . $value[$i+1] . "\n");
                                exit(LEXSYNERR); # Lexical error
                            } 
                        }
                        break; # it is associated with foreach
                    }
                }
                if ( !$validInstruction ) {
                    error_log("ERROR: instruction name is invalid ". $value[0] . "\n");
                    exit(LEXSYNERR);
                }
            }  
        }
    }

    ############### function for generating XML file ###############
    function generateXML() {

        global $arrayNoComments;

        $objDom = new DomDocument('1.0');
        $objDom->encoding = 'UTF-8';
        $objDom->formatOutput = true;
        $objDom->preserveWhiteSpace = false;
    
        $root = $objDom->createElement("program");
        $root->setAttribute('language', 'IPPcode18');
        $objDom->appendChild($root);
    
        $counter = 1;
        // i need to jump over .IPPcode18
        unset($arrayNoComments[0]); 
        foreach ( $arrayNoComments as $instruction ) {
    
            $instruction = explode(" ", $instruction);  # make array from string
            
            $instrElem = $objDom->createElement("instruction");
            $instrElem->setAttribute('order', $counter);
            $instrElem->setAttribute('opcode', $instruction[0]);
            $root->appendChild($instrElem);
            $counter++;
    
            $nmbOfInstr = count($instruction) - 1;
            $argCounter = 1;
            $setType = false;
            if ( strcmp($instruction[0], "READ") == 0 ) $setType = true;

            // remove instruction name because i need only instruction arguments
            unset($instruction[0]);
            foreach($instruction as $value) {
    
                $argElem = $objDom->createElement("arg".$argCounter);
                
                if ( $setType && $argCounter == 2 ) {
                    $argElem->setAttribute('type', 'type');
                }
                else {
                    if ( (strncmp("int", $value, 3)) == 0 ) {
    
                        $argElem->setAttribute('type', 'int');
                        $value = substr($value, 4);
                    }
                    else if ( (strncmp("bool", $value, 4)) == 0 ) {
                    
                        $argElem->setAttribute('type', 'bool');
                        $value = substr($value, 5);
                    }
                    else if ( (strncmp("string", $value, 6)) == 0 ) {
                
                        $argElem->setAttribute('type', 'string');
                        $value = substr($value, 7);
                    }
                    else if ( ((strncmp("GF@", $value, 3)) == 0) || ((strncmp("LF@", $value, 3)) == 0) || ((strncmp("TF@", $value, 3)) == 0) ) {
                        $argElem->setAttribute('type', 'var');
                    }
                    else { # it is label
                        $argElem->setAttribute('type', 'label');
                    }
                }
                
                $argText = $objDom->createTextNode($value);
                $argElem->appendChild($argText);
                $instrElem->appendChild($argElem);
    
                $argCounter++;
            }
        }
        
        // print data formated inside XML to stdout
        echo $objDom->saveXML();
    }

    ############### function for bonus task STATP ###############
    function extension() {
        
        global $fileName;
        global $isLoc, $nmbOfInstr;
        global $isLocFirst;
        global $isComment, $nmbOfComments;

        if ( $isLocFirst ) {    # nuber of lines is first
            if ( file_put_contents($fileName, $nmbOfInstr . "\n") == false ) {
                error_log("Error: Cannot open file " . $fileName . " for reading\n");
                exit(FILEERR);
            }
            if ( $isComment ) {
                if ( file_put_contents($fileName, $nmbOfComments . "\n", FILE_APPEND) == false ) {
                    error_log("Error: Cannot open file " . $fileName . "for reading\n");
                    exit(FILEERR);
                }
             }
        }
        else {  # number of comments print first
            if ( $isComment ) {
               if ( file_put_contents($fileName, $nmbOfComments . "\n") == false ) {
                   error_log("Error: Cannot open file " . $fileName . "for reading\n");
                   exit(FILEERR);
               }
            }
            if ( $isLoc ) {
                if ( file_put_contents($fileName,$nmbOfInstr . "\n", FILE_APPEND) == false ) {
                    error_log("Error: Cannot open file " . $fileName . " for reading\n");
                    exit(FILEERR);
                }
            }
        }
    }
?>