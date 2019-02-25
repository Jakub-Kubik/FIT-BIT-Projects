#!/usr/bin/python3
# -*- coding: utf-8 -*-
# Jakub Kubik (xkubik32)
# Interpret for IPPcode18
# 15.4.2018

from IPPcode18_xml_parser import ParseProgramArgsAndOpenFile, ParseXML
from interpretStructures import Variable, Frame, AllLabels
from interpretStructures import semantic_err, operand_type_err, no_var_err, \
    no_frame_err, no_value_err, zero_div_err, str_operation_err
import sys
import re

# ########## Global variables for interpret
globalFrame = Frame()       # object for global frame
tempFrame = Frame()         # object for temporary frame
locFrame = Frame()          # object for locale frame
frameStack = list()         # stack for local frames
isTempFr = False            # now in temporary frame
isLocFr = False             # now in locale frame

dataStack = list()          # stack for data (2 instruction)
returnIndexStack = []       # stack for indexes of instruction which to use when return is called
allLabels = None            # object for all labels
ActInstr = None             # ActInstr must ge global because i need to change it in CALL and RET and JMPS instr
InstrList = list()          # Instruction list
indexOfActInstr = 0         # index of actual instruction
isInstrCallRetJmp = False


def check_scope_and_find_var(instr_name, var):
    """ Check if variable object is in actual frame """
    frame_name = var[:3]
    global globalFrame
    global locFrame
    global tempFrame   
    global isTempFr
    global isLocFr 

    if frame_name == 'LF@':
        # check if loc frame is available
        if isLocFr:
            # check if variable is in loc frame
            if not locFrame.find_var(var):
                sys.stderr.write(instr_name + ": local frame exist but variable isn't there\n")
                sys.exit(no_var_err)
            else:
                return locFrame.get_var(var)
        # loc frame isnt available
        sys.stderr.write(instr_name + ': bad scope local frame doesn\'t exist\n')
        sys.exit(no_frame_err)

    elif frame_name == 'TF@':
        # check if temp frame is available
        if isTempFr:
            # check if variable is in temp frame
            if not tempFrame.find_var(var):
                sys.stderr.write(instr_name + ': temporary frame exists but variable isn\'t there\n')
                sys.exit(no_var_err)
            # variable is in frame so return it
            return tempFrame.get_var(var)
        # temp frame isn't available
        sys.stderr.write(instr_name + ': bad scope temp frame doesn\'t exist\n')
        sys.exit(no_frame_err)

    elif frame_name == 'GF@':
        # check if variable is in global frame
        if not globalFrame.find_var(var):
            sys.stderr.write(instr_name + ': global frame exists but variable isn\'t there ' + var + '\n')
            sys.exit(no_var_err)
        # variable is in global frame
        return globalFrame.get_var(var)
        

def insert_var_to_right_frame(var):
    """ Insert variable to right frame according to var name """
    global globalFrame
    global tempFrame
    global locFrame
    global frameStack

    if var.name[:3] == 'GF@':
        globalFrame.insert_var(var)

    elif var.name[:3] == 'TF@':
        tempFrame.insert_var(var)
    
    elif var.name[:3] == 'LF@':
        locFrame.insert_var(var)
        frameStack.pop()
        frameStack.append(locFrame)


def check_arithmetic_op(instr_name, position):
    """ Check operand in arithmetic operation in instructions like add, sub, mul, idiv """
    if ActInstr.args_type[position] == 'var':
        ret_var = check_scope_and_find_var(instr_name, ActInstr.args[position])
        if ret_var.data_type == 'nondef':
            sys.stderr.write(instr_name + ': ' + str(position) + ' operand is var and has undef value\n')
            sys.exit(no_value_err)
        elif ret_var.data_type != 'int':
            sys.stderr.write(instr_name + ': ' + str(position) + ' operand is var but isn\'t int\n')
            sys.exit(operand_type_err)
        return int(ret_var.value)

    elif ActInstr.args_type[position] != 'int':
        sys.stderr.write(instr_name + ': ' + str(position) + ' operand isn\'t int\n')
        sys.exit(operand_type_err)
    return int(ActInstr.args[position])


def check_relational_op(instr, position):
    """ Check operand in relational operation in instructions like lt, gt, eq """
    if ActInstr.argsType[position] == 'var':
        op = check_scope_and_find_var(instr, ActInstr.args[position])
    else:
        value = ippcode_type_to_python_type(ActInstr.argsType[position], ActInstr.args[position])
        op = Variable('op' + str(position), ActInstr.argsType[position], value)
    return op
    

def check_logical_op(instr, position):
    """ Check operand in logical operation in instructions like and, or, not """
    if ActInstr.argsType[position] == 'var':
        op = check_scope_and_find_var(instr, ActInstr.args[position])
        if op.dataType != 'bool':
            sys.stderr.write(instr + ' symb' + str(position) + ' is var but value ins\'t bool\n')
            sys.exit(operand_type_err)
    else:
        value = ippcode_type_to_python_type(ActInstr.argsType[position], ActInstr.args[position])
        op = Variable('op' + str(position), ActInstr.argsType[position], value)
        if op.data_type != 'bool':
            sys.stderr.write(instr + 'op' + str(position) + ' is literal but value ins\'t bool')
            sys.exit(operand_type_err)
    return op


def check_string_op(instr, position):
    """ Check operand in string operation in instructions like int2char, stri2int, concat, strlen, getchar, setchar """
    if ActInstr.argsType[position] == 'var':
        op = check_scope_and_find_var(instr, ActInstr.args[position])
    else:
        value = ippcode_type_to_python_type(ActInstr.argsType[position], ActInstr.args[position])
        op = Variable('op' + str(position), ActInstr.argsType[position], value)
    return op


def check_jump_op(instr, position):
    """ Check operand in jump operation in instructions like jump, jumpifeq, jumpifneq """
    if ActInstr.argsType[position] == 'var':
        op = check_scope_and_find_var(instr, ActInstr.args[position])
    else:
        value = ippcode_type_to_python_type(ActInstr.argsType[position], ActInstr.args[position])
        op = Variable('op' + str(position), ActInstr.argsType[position], value)
    return op


def replace(match):
    return chr(int(match.group(1)))


def ippcode_type_to_python_type(dat_type, val):
    """ Convert IPPcode18 data type to python data type """
    if dat_type == 'bool':
        if val == 'true':
            return True
        elif val == 'false':
            return False
    
    elif dat_type == 'int':
        return int(val)
    
    elif dat_type == 'string':
        
        if (' ' in val) is True or ('#' in val) is True:
            sys.stderr.write('in string is space or #\n')
            sys.exit(32)
        regex = re.compile(r"\\(\d{1,3})")
        new = regex.sub(replace, val)
        
        return new


# Functions for interpreting instructions
def move():
    """ MOVE <var> <symb> """
    check_scope_and_find_var('MOVE', ActInstr.args[0])

    if ActInstr.argsType[1] == 'var':
        # <symb> is variable
        ret_var = check_scope_and_find_var('MOVE', ActInstr.args[1])
        var = Variable(ActInstr.args[0], ret_var.dataType, ret_var.value)
    
    else:
        # <symb> is literal
        value = ippcode_type_to_python_type(ActInstr.argsType[1], ActInstr.args[1])
        var = Variable(ActInstr.args[0], ActInstr.argsType[1], value)

    insert_var_to_right_frame(var)


def createframe():
    """ CREATEFRAME """
    global tempFrame
    global isTempFr
    tempFrame = Frame() 
    isTempFr = True


def pushframe():
    """ PUSHFRAME """
    global locFrame
    global frameStack
    global isLocFr
    global isTempFr
    # check if temp frame is available
    if not isTempFr:
        sys.stderr.write('PUSHFRAME: access to undefined temp frame\n')
        sys.exit(no_frame_err)
    
    # create local frame
    locFrame = Frame()
    
    # copy all varaibles from temp frame to loc frame
    for key, var in tempFrame.vars_dict.items():
        name = 'LF@' + key[3:]
        var.name = name
        locFrame.insert_var(var)
    
    # add loc frame to stack frame
    frameStack.append(locFrame)
    isLocFr = True             
    isTempFr = False


def popframe():
    """ POPFRAME """
    global tempFrame
    global isTempFr
    global isLocFr
    global frameStack
    # check if frame stack isn't empty
    if len(frameStack) == 0:
        sys.stderr.write('POPFRAME: stack for frames is empty\n')
        exit(no_frame_err)

    isTempFr = True   
    isLocFr = False   
    tempFrame = Frame() 

    # copy all variables from loc frame to temp frame
    local_frame = frameStack.pop()
    for key, var in local_frame.varsDict.items():
        name = 'TF@' + key[3:]
        var.name = name
        tempFrame.insert_var(var)


def defvar():
    """ DEFVAR <var> """ 
    global globalFrame
    global locFrame
    global tempFrame
    global frameStack
    
    # check global frame
    if ActInstr.args[0][:3] == 'GF@':
        if globalFrame.find_var(ActInstr.args[0][:3]):
            sys.stderr.write('DEFVAR: redefinition of variable in global frame\n')
            sys.exit(semantic_err)

        # inset to global frame nondef <var>
        var = Variable(ActInstr.args[0], 'nondef', '42')
        globalFrame.insert_var(var)
    
    # check local frame
    elif ActInstr.args[0][:3] == 'LF@':
        if not isLocFr:
            sys.stderr.write('DEFVAR: access to non existing local frame\n')
            sys.exit(no_frame_err)

        elif locFrame.find_var(ActInstr.args[0][:3]):
            sys.stderr.write('DEFVAR: redefinition of variable in local frame\n')
            sys.exit(semantic_err)

        # insert to local frame nondef <var>
        var = Variable(ActInstr.args[0], 'nondef', '42')
        locFrame.insert_var(var)
        frameStack.pop()
        frameStack.append(locFrame)

    # check temporary frame
    elif ActInstr.args[0][:3] == 'TF@':
        if not isTempFr:
            sys.stderr.write('DEFVAR: access to nonexisting temporary frame\n')
            sys.exit(no_frame_err)
        
        elif tempFrame.find_var(ActInstr.args[0][:3]):
            sys.stderr.write('DEFVAR: redefinition of variable in temp frame\n')
            sys.exit(semantic_err)

        # insert to temporary frame nondef <var>
        var = Variable(ActInstr.args[0], 'nondef', '42')
        tempFrame.insert_var(var)


def call():
    """ CALL <label> """
    global ActInstr
    global isInstrCallRetJmp
    global returnIndexStack
    global InstrList
    global indexOfActInstr
    # check if label exist
    if not allLabels.findLabel(ActInstr.args[0]):
        sys.stderr.write('CALL: label doesnt exist\n')
        sys.exit(semantic_err)

    # set next instr index to return index stack 
    label_index = allLabels.getLabel(ActInstr.args[0])  # get index of label in instr list
    ActInstr = InstrList[label_index]                   # set act instr
    returnIndexStack.append(indexOfActInstr+1)          # save next instr index after this instr call
    isInstrCallRetJmp = True
    indexOfActInstr = label_index


def return_instr():
    """ RETURN """ 
    global ActInstr
    global isInstrCallRetJmp
    global returnIndexStack
    global indexOfActInstr
    # check where to return 
    if len(returnIndexStack) == 0:
        sys.stderr.write('RETURN: Nowhere to return\n')
        sys.exit(no_value_err)

    # set ActInstr to return index instr from instr list
    indexOfActInstr = returnIndexStack.pop()
    ActInstr = InstrList[indexOfActInstr]
    isInstrCallRetJmp = True


def pushs():
    """ PUSHS <symb> """
    global dataStack
    if ActInstr.argsType[0] == 'var':
        # <symb> is variable
        var = check_scope_and_find_var('PUSHS', ActInstr.args[0])
        if var.dataType == 'nondef':
            sys.stderr.write('PUSHS: variable has undefined value\n')
            sys.exit(no_value_err)

    else:
        # <symb> is literal
        value = ippcode_type_to_python_type(ActInstr.argsType[0], ActInstr.args[0])
        var = Variable('literal', ActInstr.argsType[0], value)
    
    # add value and data type to data stack
    dataStack.append(var)


def pops():
    """ POPS <var> """
    global dataStack
    # check data stack size
    if len(dataStack) == 0:
        sys.stderr.write('POPS: data stack is empty\n')
        sys.exit(no_value_err)
    # check scope of <var>
    check_scope_and_find_var('POPS', ActInstr.args[0])

    # remove value from data stack
    var = dataStack.pop()
    new_var = Variable(ActInstr.args[0], var.dataType, var.value)

    # add new_var to <var> frame
    insert_var_to_right_frame(new_var)


def add():
    """ ADD <var> <symb1> <symb2> """
    check_scope_and_find_var('ADD', ActInstr.args[0])
    op1 = check_arithmetic_op('ADD', 1)
    op2 = check_arithmetic_op('ADD', 2)

    value = op1 + op2
    var = Variable(ActInstr.args[0], 'int', value)
    insert_var_to_right_frame(var)


def sub():
    """ SUB <var> <symb1> <symb2> """ 
    check_scope_and_find_var('SUB', ActInstr.args[0])
    op1 = check_arithmetic_op('SUB', 1)
    op2 = check_arithmetic_op('SUB', 2)

    value = op1 - op2
    var = Variable(ActInstr.args[0], 'int', value)
    insert_var_to_right_frame(var)


def mul():
    """ MUL <var> <symb1> <symb2> """ 
    check_scope_and_find_var('MUL', ActInstr.args[0])
    op1 = check_arithmetic_op('MUL', 1)
    op2 = check_arithmetic_op('MUL', 2)

    value = op1 * op2
    var = Variable(ActInstr.args[0], 'int', value)
    insert_var_to_right_frame(var)


def idiv():
    """ IDIV <var> <symb1> <symb2> """ 
    check_scope_and_find_var('IDIV', ActInstr.args[0])
    op1 = check_arithmetic_op('IDIV', 1)
    op2 = check_arithmetic_op('IDIV', 2)

    # check division by 0 
    if op2 == 0:
        sys.stderr.write('IDIV: division by 0\n')
        sys.exit(zero_div_err)

    value = op1 // op2
    var = Variable(ActInstr.args[0], 'int', value)
    insert_var_to_right_frame(var)


def lt():
    """ LT <var> <symb1> <symb2> """
    check_scope_and_find_var('LT', ActInstr.args[0])
    op1 = check_relational_op('LT', 1)
    op2 = check_relational_op('LT', 2)

    # check allowed data types
    if op1.dataType != op2.dataType:
        sys.stderr.write('LT: symb1 a symb2 doesn\'t have the same data type')
        sys.exit(operand_type_err)

    value = op1.value < op2.value
    var = Variable(ActInstr.args[0], 'bool', value)
    insert_var_to_right_frame(var)


def gt():
    """ GT <var> <symb1> <symb2> """
    check_scope_and_find_var('GT', ActInstr.args[0])
    op1 = check_relational_op('GT', 1)
    op2 = check_relational_op('GT', 2)

    # check allowed data types
    if op1.dataType != op2.dataType:
        sys.stderr.write('GT: symb1 a symb2 doesn\'t have the same data type')
        sys.exit(operand_type_err)

    value = op1.value > op2.value
    var = Variable(ActInstr.args[0], 'bool', value)
    insert_var_to_right_frame(var)


def eq():
    """ EQ <var> <symb1> <symb2> """
    check_scope_and_find_var('EQ', ActInstr.args[0])
    op1 = check_relational_op('EQ', 1)
    op2 = check_relational_op('EQ', 2)

    # check allowed data types
    if op1.dataType != op2.dataType:
        sys.stderr.write('EQ: symb1 and symb2 doesn\'t have the same data type')
        sys.exit(operand_type_err)

    value = op1.value == op2.value
    var = Variable(ActInstr.args[0], 'bool', value)
    insert_var_to_right_frame(var)


def and_instr():
    """ AND <var> <symb1> <symb2> """
    check_scope_and_find_var('AND', ActInstr.args[0])
    op1 = check_logical_op('AND', 1)
    op2 = check_logical_op('AND', 2)

    value = op1.value and op2.value
    var = Variable(ActInstr.args[0], 'bool', value)
    insert_var_to_right_frame(var)


def or_instr():
    """ OR <var> <symb1> <symb2> """
    check_scope_and_find_var('OR', ActInstr.args[0])
    op1 = check_logical_op('OR', 1)
    op2 = check_logical_op('OR', 2)

    value = op1.value or op2.value
    var = Variable(ActInstr.args[0], 'bool', value)
    insert_var_to_right_frame(var)


def not_instr():
    """ NOT <var> <symb> """
    check_scope_and_find_var('NOT', ActInstr.args[0])
    op = check_logical_op('NOT', 1)

    value = not op.value
    var = Variable(ActInstr.args[0], 'bool', value)
    insert_var_to_right_frame(var)


def int2char():
    """ INT2CHAR <var> <symb> """
    check_scope_and_find_var('INT2CHAR', ActInstr.args[0])
    op = check_string_op('INT2CHAR', 1)

    # check data type
    if op.dataType != 'int':
        sys.stderr.write('INT2CHAR: symb data type isn\'t int')
        sys.exit(operand_type_err)

    try:
        value = chr(op.value)
    except TypeError:
        sys.stderr.write('INT2CHAR: not unciode value')
        sys.exit(str_operation_err)
    var = Variable(ActInstr.args[0], 'str', value)
    insert_var_to_right_frame(var)


def stri2int():
    """ STRI2INT <var> <symb1> <symb2> """
    check_scope_and_find_var('STRI2INT', ActInstr.args[0])
    op1 = check_string_op('STRI2INT', 1)
    op2 = check_string_op('STRI2INT', 2)

    # check data types and indexing in string
    if op1.dataType != 'string' or op2.dataType != 'int':
        sys.stderr.write('STRI2INT: symb1 or symb2 data type isn\'t ok')
        sys.exit(operand_type_err)
    
    if len(op1.value) - 1 < op2.value or op2.value < 0:
        sys.stderr.write('STRI2INT: indexing out of the string\n')
        sys.exit(str_operation_err)

    value = ord(op1.value[op2.value])
    var = Variable(ActInstr.args[0], 'int', value)
    insert_var_to_right_frame(var)


def read():
    """ READ <var> <type> """
    check_scope_and_find_var('READ', ActInstr.args[0])

    # get value acording <type> and save it to right frame
    value = input()

    if ActInstr.args[1] == 'int':
        try:
            value = int(value)
        except TypeError:
            value = 0
    elif ActInstr.args[1] == 'string':
        try:
            value = str(value)
        except TypeError:
            value = '\n'
    elif ActInstr.args[1] == 'bool':
        if value.lower() == 'true':
            value = True
        else:
            value = False

    var = Variable(ActInstr.args[0], ActInstr.args[1], value)
    insert_var_to_right_frame(var)


def write():
    """ WRITE <symb> """
    if ActInstr.argsType[0] == 'var':
        var = check_scope_and_find_var('WRITE', ActInstr.args[0])
    else:
        value = ippcode_type_to_python_type(ActInstr.argsType[0], ActInstr.args[0])
        var = Variable('literal', ActInstr.argsType[0], value)
    
    if var.dataType == 'bool':
        if var.value is True:
            print('true')
        else:
            print('false')
    else:
        print(str(var.value))


def concat():
    """" CONCAT <var> <symb1> <symb2> """
    check_scope_and_find_var('CONCAT', ActInstr.args[0])
    op1 = check_string_op('CONCAT', 1)
    op2 = check_string_op('CONCAT', 2)

    # check data type
    if op1.dataType != 'string' or op2.dataType != 'string':
        sys.stderr.write('CONCAT: symb1 or symb2 data type isnt string')
        sys.exit(operand_type_err)

    value = op1.value + op2.value
    var = Variable(ActInstr.args[0], 'string', value)
    insert_var_to_right_frame(var)


def strlen():
    """ STRLEN <var> <symb> """
    check_scope_and_find_var('STRLEN', ActInstr.args[0])
    op = check_string_op('STRLEN', 1)

    # check data type
    if op.dataType != 'string':
        sys.stderr.write('STRLEN: symb1 data type isnt str\n')
        sys.exit(operand_type_err)

    value = len(op.value)
    var = Variable(ActInstr.args[0], 'int', value)
    insert_var_to_right_frame(var)


def getchar():
    """ GETCHAR <var> <symb1> <symb2> """
    check_scope_and_find_var('GETCHAR', ActInstr.args[0])
    op1 = check_string_op('GETCHAR', 1)
    op2 = check_string_op('GETCHAR', 2)

    # check data types and indexing
    if op1.dataType != 'string' or op2.dataType != 'int':
        sys.stderr.write('GETCHAR: symb1 or symb2 data type isn\'t ok')
        sys.exit(operand_type_err)
    
    if len(op1.value) - 1 < op2.value or op2.value < 0:
        sys.stderr.write('GETCHAR: len of op1 string is bigger than required index op2')
        sys.exit(str_operation_err)

    value = op1.value[op2.value]
    var = Variable(ActInstr.args[0], 'string', value)
    insert_var_to_right_frame(var)


def setchar():
    """ SETCHAR <var> <symb1> <symb2> """
    var = check_scope_and_find_var('SETCHAR', ActInstr.args[0])
    op1 = check_string_op('SETCHAR', 1)
    op2 = check_string_op('SETCHAR', 2)

    # check data types and indexing
    if op1.dataType != 'int' or op2.dataType != 'string':
        sys.stderr.write('SETCHAR: first or second operand isn\'t OK')
        sys.exit(operand_type_err)

    if var.dataType == 'nondef':
        sys.stderr.write('SETCHAR: var has undefined value')
        sys.exit(str_operation_err)
    if op2.value == '':
        sys.stderr.write('SETCHAR: empty string')
        sys.exit(str_operation_err)
    if var.dataType != "string":
        sys.stderr.write('SETCHAR bad data type\n')
        sys.exit(operand_type_err)
    if len(var.value) - 1 < op1.value or op1.value < 0:
        sys.stderr.write('SETCHAR: indexing out of range of string')
        sys.exit(str_operation_err)

    value = var.value[:op1.value] + op2.value[0] + var.value[(op1.value+1):]
    new_var = Variable(ActInstr.args[0], 'string', value)
    insert_var_to_right_frame(new_var)


def type_instr():
    """ TYPE <var> <symb> """
    check_scope_and_find_var('TYPE', ActInstr.args[0])
    op1 = check_string_op('SETCHAR', 1)

    # save value to right frame acorfing <var>
    if re.search(r'^[-+]?\d+$', str(op1.value)):
        data_type = "int"
    elif re.search(r'^True|False$', str(op1.value)):
        data_type = "bool"
    else:
        data_type = "string"
        
    var = Variable(ActInstr.args[0], 'setBool', data_type)
    insert_var_to_right_frame(var)


def label():
    """ LABEL <label> """
    # I have already implemented


def jump():
    """ JUMP <label> """
    global isInstrCallRetJmp
    global indexOfActInstr
    global ActInstr

    # <label>
    if not allLabels.find_label(ActInstr.args[0]):
        sys.stderr.write('JUMP: nonexisting label\n')
        sys.exit(semantic_err)
    
    # get jmp instruction index and set act instr
    label_tag = allLabels.get_label(ActInstr.args[0])
    ActInstr = InstrList[label_tag]
    isInstrCallRetJmp = True
    indexOfActInstr = label_tag


def jumpifeq():
    """ JUMPIFEQ <label> <symb1> <symb2> """
    global isInstrCallRetJmp
    global indexOfActInstr
    global ActInstr

    # <label>
    if not allLabels.find_label(ActInstr.args[0]):
        sys.stderr.write('JUMPIFEQ: na neexistijuce navestie\n')
        sys.exit(semantic_err)
    label_tag = allLabels.get_label(ActInstr.args[0])
    op1 = check_jump_op('JUMPIFEQ', 1)                 # <symb1>
    op2 = check_jump_op('JUMPIFEQ', 2)                 # <symb2>

    # chcek data types
    if op1.dataType != op2.dataType:
        sys.stderr.write('JUMPIFEQ op1 data type isnt same as op2 data type')
        sys.exit(operand_type_err)

    # check if i have to jump
    if op1.value == op2.value:
        ActInstr = InstrList[label_tag]
        isInstrCallRetJmp = True
        indexOfActInstr = label_tag


def jumpifneq():
    """ JUMPIFNEQ <label> <symb1> <symb2> """
    global isInstrCallRetJmp
    global indexOfActInstr
    global ActInstr

    # <label>
    if not allLabels.find_label(ActInstr.args[0]):
        sys.stderr.write('JUMPIFEQ: na neexistijuce navestie\n')
        sys.exit(semantic_err)
    label_tag = allLabels.get_label(ActInstr.args[0])
    op1 = check_jump_op('JUMPIFNEQ', 1)                 # <symb1>
    op2 = check_jump_op('JUMPIFNEQ', 2)                 # <symb2>

    # chcek data types
    if op1.dataType != op2.dataType:
        sys.stderr.write('JUMPIFNEQ op1 data type isnt same as op2 data type')
        sys.exit(operand_type_err)

    # check if i have to jump
    if op1.value != op2.value:
        ActInstr = InstrList[label_tag]
        isInstrCallRetJmp = True
        indexOfActInstr = label_tag


def dprint():
    """ DPRINT """
    pass


def break_instr():
    """ BREAK """
    pass


dict_with_func_for_instructions = {
    'MOVE': move, 'CREATEFRAME': createframe, 'PUSHFRAME': pushframe, 'POPFRAME': popframe,
    'DEFVAR': defvar, 'CALL': call, 'RETURN': return_instr, 'PUSHS': pushs, 'POPS': pops, 'ADD': add,
    'SUB': sub, 'MUL': mul, 'IDIV': idiv, 'LT': lt, 'GT': gt, 'EQ': eq, 'AND': and_instr,
    'OR': or_instr, 'NOT': not_instr, 'INT2CHAR': int2char, 'STRI2INT': stri2int, 'READ': read,
    'WRITE': write, 'CONCAT': concat, 'STRLEN': strlen, 'GETCHAR': getchar, 'SETCHAR': setchar,
    'TYPE': type_instr, 'LABEL': label, 'JUMP': jump, 'JUMPIFEQ': jumpifeq,
    'JUMPIFNEQ': jumpifneq, 'DPRINT': dprint, 'BREAK': break_instr
}


def main():
    """ Main funciton of interpert """
    global globalFrame
    global tempFrame
    global locFrame
    global frameStack
    global isTempFr
    global isLocFr
    global dataStack
    global returnIndexStack
    global allLabels
    global ActInstr
    global InstrList
    global indexOfActInstr
    global isInstrCallRetJmp

    # Parse XML file with source code and create Instruction List
    prog_args = ParseProgramArgsAndOpenFile()
    fd = prog_args.open_file()
    parse_file = ParseXML()
    parse_file.parse_xml_file(fd)
    InstrList = parse_file.get_instr_list()

    # create associative array of labels for jumping thrught instruction acording labels
    allLabels = AllLabels(InstrList)
    # allLabels.printAllLabels()
    allLabels.check_labels_redefinition(InstrList)

    # Creating frames and all important data structures for interpreting
    ActInstr = InstrList[0]
    InstrList.append(None)

    # interpret instructions
    while ActInstr is not None:
        # print(indexOfActInstr)
        dict_with_func_for_instructions[ActInstr.name]()

        # Move to the next instruction
        if not isInstrCallRetJmp:
            indexOfActInstr = indexOfActInstr + 1
            ActInstr = InstrList[indexOfActInstr]
        isInstrCallRetJmp = False

    # printing debugging info
    # print('\nGlobal frame')
    # globalFrame.printVarLabelDict()

    # if isTempFr:
    #    print('\nTemoporary frame')
    #    tempFrame.printVarLabelDict()

    # if len(frameStack) > 0:
    #    localFrame = frameStack.pop()
    #    print('\nLocal frame')
    #    localFrame.printVarLabelDict()


if __name__ == "__main__":
    main()
