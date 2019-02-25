<?php
    # ##########################################################
    #
    #      Jan Jakub Kubik (xkubik32)
    #      VUT FIT 2BIA 
    #      xkubik32@stud.fit.vutbr.cz
    #      date: 13.3.2018
    #      description: data structure and
    #                   regular expressions
    #
    # ##########################################################

    // constants for error values
    define("LEXSYNERR", 21);
    define("FILEERR", 12);
    define("PARAMERR", 10);
    define("OK", 0);

    // regular expression for variable
    $regVar = '/^(GF@|LF@|TF@)[a-zA-Z\_\-\$\&\%\*][a-zA-Z0-9\_\-\$\&\%\*]*$/'; # OK FUNGUJE
    
    // regular expression for variable, int, bool, string
    $regSymb = '/^((GF@|LF@|TF@)[a-zA-Z\_\-\$\&\%\*][a-zA-Z0-9\_\-\$\&\%\*]*)$|(^int@[-+]?\d+$)|^bool@(true|false)$/';
    $regString = '/string@(?!.*\\\\[^\d])(?:\p{L}|\d|[[:print:]])*(?<!(\\\\\d{0})|(\\\\\d{1})|(\\\\\d{2})|(\\\\))$/u'; 

    // regular expression for label (the same as for variable but without frame)
    $regLabel = '/^[a-zA-Z\_\-\$\&\%\*][a-zA-Z0-9\_\-\$\&\%\*]*$/'; # OK FUNGUJE

    // regular expression for name of int, string, bool
    $regType = "/^(int|string|bool)$/";

    // two dimesional array for key word of instruction, number of arguents and types
    $keyWordsWidthAttrib = array
    (
        array("DEFVAR", 1, $regVar),                          
        array("MOVE", 2, $regVar, "REGSYMB"),                   
        array("PUSHS", 1, "REGSYMB"),
        array("POPS", 1, $regVar),
        array("ADD", 3, $regVar, "REGSYMB", "REGSYMB"),
        array("SUB", 3, $regVar, "REGSYMB", "REGSYMB"),
        array("MUL", 3, $regVar, "REGSYMB", "REGSYMB"),
        array("IDIV", 3, $regVar, "REGSYMB", "REGSYMB"),
        array("LT", 3, $regVar, "REGSYMB", "REGSYMB"), 
        array("GT", 3, $regVar, "REGSYMB", "REGSYMB"), 
        array("EQ", 3, $regVar, "REGSYMB", "REGSYMB"),
        array("AND", 3, $regVar, "REGSYMB", "REGSYMB"),
        array("OR", 3, $regVar, "REGSYMB", "REGSYMB"),
        array("NOT", 2, "REGSYMB", "REGSYMB"),
        array("INT2CHAR", 2, $regVar, "REGSYMB"),
        array("STRI2INT", 3, $regVar, "REGSYMB", "REGSYMB"),
        array("READ", 2, $regVar, $regType),
        array("WRITE", 1, "REGSYMB"),
        array("CONCAT", 3, $regVar, "REGSYMB", "REGSYMB"),
        array("STRLEN", 2, $regVar, "REGSYMB"),
        array("GETCHAR", 3, $regVar, "REGSYMB", "REGSYMB"),
        array("SETCHAR", 3, $regVar, "REGSYMB", "REGSYMB"),
        array("TYPE", 2, $regVar, "REGSYMB"),
        array("DPRINT", 1, "REGSYMB"),
        array("LABEL", 1, $regLabel),                          
        array("CALL", 1, $regLabel),
        array("JUMP", 1, $regLabel),
        array("JUMPIFEQ", 3, $regLabel, "REGSYMB", "REGSYMB"),    
        array("JUMPIFNEQ", 3, $regLabel, "REGSYMB", "REGSYMB"),
        array("BREAK", 0),
        array("CREATEFRAME", 0),
        array("PUSHFRAME", 0),
        array("POPFRAME", 0),
        array("RETURN", 0),
    );

    ######### variables for extension STATP for writing aditional info to required file #######
    $isExtension = false;
    $isLocFirst = false; # poradie parametrov
    $isLoc = false;
    $nmbOfInstr = 0;
    $isComment = false;
    $nmbOfComments = 0;
    $fileName = "nothing";

?>