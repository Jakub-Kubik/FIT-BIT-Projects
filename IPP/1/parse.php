<?php
    # ##########################################################
    #
    #      Jan Jakub Kubik (xkubik32)
    #      VUT FIT 2BIA 
    #      xkubik32@stud.fit.vutbr.cz
    #      date: 13.3.2018
    #      description: Main body of my program it 
    #                   means calling all required function 
    #
    # ##########################################################

    include_once "data.php";
    include_once "functions.php";

    ########## default setting ##########
    setlocale(LC_ALL, 'cs_CZ.UTF-8');
    
    ######### checking program parameters ##########
    checkParameters();
    
    ########## converting stdin to array ##########
    $array = array();
    $array = stdinToArray();

    ########## removing comments, whitespace and tabulators from array ##########
    $arrayNoComments = array();
    $arrayNoComments = arrayRemoveCommentsWhitespaces();

    ########## lexical and syntax checking of array ##########
    lexicalAndSyntaxAnalysis();
    
    ########## generating XML to stdout from array ##########
    generateXML();

    ####### (EXTENSION) opening file for statistic ########
    extension();

    return OK;
?>