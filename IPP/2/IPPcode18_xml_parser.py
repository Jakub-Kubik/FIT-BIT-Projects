# parser for IPPcode18
# Jakub Kubik 
# 15.4.2018


import re
import sys
import getopt
import xml.etree.ElementTree as ElementTree
from interpretStructures import xml_err, lex_syn_err
from interpretStructures import program_param_err, code_in_xml_err


class Instruction:
    """ Simple class for predefined and parsed instructions saved in instruction list from XML """

    def __init__(self, name, length, *args):
        self.args_type = []
        self.name = name
        self.len = length
        self.args = []
        for i in args:
            self.args.append(i)
    
    def args_type(self, argum):
        for i in argum:
            self.args_type.append(i)


def usage():
    print('\n\t====================================================================')
    print('\t\tProgram načte XML reprezentaci programu ze zadaného ')
    print('\t\tsouboru a tento program s využitím standardního vstupu')
    print('\t\tvstupu a výstupu interpretuje.')
    print('\t=====================================================================\n')


class ParseProgramArgsAndOpenFile:
    """ Simple class for parsing program arguments """
    
    def __init__(self):
        self.my_file = None
        if len(sys.argv) != 2:
            sys.stderr.write('Wrong number of program arguemnts\n')
            sys.exit(program_param_err)

        try:
            opts, args = getopt.getopt(sys.argv[1:], "", ["help", "source="])
        except getopt.GetoptError:
            sys.stderr.write("\n\tError in program arguments\n")
            # usage()
            sys.exit(program_param_err)
        for o, a in opts:
            if o == "--help":
                usage()
                sys.exit(0)
            elif o == "--source":
                self.filename = a
            else:
                sys.stderr.write('Wrong program arguments\n')
                sys.exit(program_param_err)
    
    def open_file(self):
        try:
            self.my_file = open(self.filename, "r+")  # or "a+", whatever you need
        except IOError:
            sys.stderr.write("Could not open file!\n")
            sys.exit(code_in_xml_err)    # correct return value
        return self.my_file


class ParseXML:
    """ Class for parsing whole XML file with transoformed source code """

    def __init__(self):
        # checking data types 
        self.InstrList = []
        self.varRegex = r'^var$'
        self.labelRegex = r'^label$'
        self.symbRegex = r'(^var$)|(^int$)|(^bool$)|(^string$)'
        self.typeRegex = r'^type$'

        # checking conrete values of data type
        self.var = r'^(GF@|LF@|TF@)[a-zA-Z\_\-\$\&\%\*][a-zA-Z0-9\_\-\$\&\%\*]*$'
        self.label = r'^[a-zA-Z\_\-\$\&\%\*][a-zA-Z0-9\_\-\$\&\%\*]*$'
        self.integer = r'^[-+]?\d+$'
        self.string = r'.+'
        self.boolean = r'^true|false$'
        self.type = r'^(int|string|bool)$'

        # associative array for second types of checking
        self.regExp = {'var': self.var, 'label': self.label, 'int': self.integer, 'string': self.string,
                       'bool': self.boolean, 'type': self.type}

        # Creating predefined instructions
        # work with frames and calling funcitons
        self.move = Instruction('MOVE', 2, self.varRegex, self.symbRegex)
        self.createframe = Instruction('CREATEFRAME', 0)
        self.pushframe = Instruction('PUSHFRAME', 0)
        self.popframe = Instruction('POPFRAME', 0)
        self.defvar = Instruction('DEFVAR', 1, self.varRegex)
        self.call = Instruction('CALL', 1, self.labelRegex)
        self.retI = Instruction('RETURN', 0)
        # work with stack
        self.pushs = Instruction('PUSHS', 1, self.symbRegex)
        self.pops = Instruction('POPS', 1, self.varRegex)
        # arithmetic, relatioinal, bool and conversion instuctions
        self.add = Instruction('ADD', 3, self.varRegex, self.symbRegex, self.symbRegex)
        self.sub = Instruction('SUB', 3, self.varRegex, self.symbRegex, self.symbRegex)
        self.mul = Instruction('MUL', 3, self.varRegex, self.symbRegex, self.symbRegex)
        self.idiv = Instruction('IDIV', 3, self.varRegex, self.symbRegex, self.symbRegex)
        self.lt = Instruction('LT', 3, self.varRegex, self.symbRegex, self.symbRegex)
        self.gt = Instruction('GT', 3, self.varRegex, self.symbRegex, self.symbRegex)
        self.eq = Instruction('EQ', 3, self.varRegex, self.symbRegex, self.symbRegex)
        self.andI = Instruction('AND', 3, self.varRegex, self.symbRegex, self.symbRegex)
        self.orI = Instruction('OR', 3, self.varRegex, self.symbRegex, self.symbRegex)
        self.notI = Instruction('NOT', 2, self.varRegex, self.symbRegex)
        self.int2char = Instruction('INT2CHAR', 2, self.varRegex, self.symbRegex)
        self.stri2int = Instruction('STR2INT', 3, self.varRegex, self.symbRegex, self.symbRegex)
        # IO instructions
        self.read = Instruction('READ', 2, self.varRegex, self.typeRegex)
        self.write = Instruction('WRITE', 1, self.symbRegex)
        # string operations
        self.concat = Instruction('CONCAT', 3, self.varRegex, self.symbRegex, self.symbRegex)
        self.strlen = Instruction('STRLEN', 2, self.varRegex, self.symbRegex)
        self.getchar = Instruction('GETCHAR', 3, self.varRegex, self.symbRegex, self.symbRegex)
        self.setchar = Instruction('SETCHAR', 3, self.varRegex, self.symbRegex, self.symbRegex)
        # data types
        self.typeI = Instruction('TYPE', 2, self.varRegex, self.symbRegex)
        # flow of program
        self.label = Instruction('LABEL', 1, self.labelRegex)
        self.jump = Instruction('JUMP', 1, self.labelRegex)
        self.jumpifeq = Instruction('JUMPIFEQ', 3, self.labelRegex, self.symbRegex, self.symbRegex)
        self.jumpifneq = Instruction('JUMPIFNEQ', 3, self.labelRegex, self.symbRegex, self.symbRegex)
        # debug instruction
        self.dprint = Instruction('DPRINT', 1, self.symbRegex)
        self.breakI = Instruction('BREAK', 0)

        # associative array of instruction objects
        self.all_instructions = {
                            'MOVE': self.move, 'CREATEFRAME': self.createframe, 'PUSHFRAME': self.pushframe,
                            'POPFRAME': self.popframe, 'DEFVAR': self.defvar, 'CALL': self.call, 'RETURN': self.retI,
                            'PUSHS': self.pushs, 'POPS': self.pops, 'ADD': self.add, 'SUB': self.sub, 'MUL': self.mul,
                            'IDIV': self.idiv, 'LT': self.lt, 'GT': self.gt, 'EQ': self.eq, 'AND': self.andI,
                            'OR': self.orI, 'NOT': self.notI, 'INT2CHAR': self.int2char, 'STRI2INT': self.stri2int,
                            'READ': self.read, 'WRITE': self.write, 'CONCAT': self.concat, 'STRLEN': self.strlen,
                            'GETCHAR': self.getchar, 'SETCHAR': self.setchar, 'TYPE': self.typeI, 'LABEL': self.label,
                            'JUMP': self.jump, 'JUMPIFEQ': self.jumpifeq, 'JUMPIFNEQ': self.jumpifneq,
                            'DPRINT': self.dprint, 'BREAK': self.breakI
                        }
        self.my_doc = None
        self.root = None
        self.valid_instr_order = None

    def parse_xml_file(self, fd):
        try:
            self.my_doc = ElementTree.parse(fd)
        except ElementTree:
            sys.stderr.write('Could not open file or bad XML format!\n')
            exit(code_in_xml_err)

        # for checking order atrib by instruction
        self.valid_instr_order = 0

        self.root = self.my_doc.getroot()
        # ### checking root tag
        if self.root.tag != 'program':   
            sys.stderr.write('root tag isnt valid\n')
            sys.exit(xml_err)
        else:
            if len(self.root.attrib.keys()) > 3:
                sys.stderr.write('Wrong number of attributes by root tag\n')
                sys.exit(xml_err)
            
            # checking optional atributes in root tag
            if len(self.root.attrib.keys()) == 3:
                if 'description' not in self.root.attrib:
                    sys.stderr.write('Non existing optional attribute description for root tag\n')
                    sys.exit(xml_err)
                if 'name' not in self.root.attrib:
                    sys.stderr.write('Non existing optional attribute name for root tag\n')
                    sys.exit(xml_err)
            elif len(self.root.attrib.keys()) == 2:
                if 'description' not in self.root.attrib and 'name' not in self.root.attrib:
                    sys.stderr.write('Non existing optional attribute description or name for root tag\n')
                    sys.exit(xml_err)
            # checking allowed atributes in root tag and its value
            if 'language' not in self.root.attrib:
                sys.stderr.write('Non existing required attribute for root tag\n')
                sys.exit(xml_err)
            elif self.root.attrib['language'] != 'IPPcode18':
                sys.stderr.write('Wrong value of root tag attribute\n')
                sys.exit(xml_err)  

            # ### checking instruction tags
            for instruction in self.root:
            
                if instruction.tag != 'instruction':
                    sys.stderr.write('Wrong tag for instruction tag\n')
                    sys.exit(xml_err)
                else:
                    # checking  number of attributes in instruction tag
                    self.valid_instr_order = self.valid_instr_order + 1
                    if len(instruction.attrib.keys()) != 2: 
                        sys.stderr.write('Wrong number of attributes for instruction tag\n')
                        sys.exit(xml_err)
                    else:
                        # checking allowed attributes in instruction tag
                        if 'order' not in instruction.attrib or 'opcode' not in instruction.attrib:
                            sys.stderr.write('Non existig required attributes for instrucion tag\n')
                            sys.exit(xml_err)
                        else:
                            # checking instruction atribute order value
                            if instruction.attrib['order'] == '' or \
                                    int(instruction.attrib['order']) != self.valid_instr_order:
                                sys.stderr.write('Wrong order of attributes in instruction tag' +
                                                 instruction.attrib['order'] + ' ' + str(self.valid_instr_order) + '\n')
                                sys.exit(xml_err)
                    
                    # ### checking instruction arguments tags 
                    arg1 = False
                    arg2 = False
                    arg3 = False
                    for arg in instruction:
                        # checing duplicity instr arg tag name
                        if (arg.tag == 'arg1' and arg1 is True) or (arg.tag == 'arg2' and arg2 is True) or \
                                (arg.tag == 'arg3' and arg3 is True):
                            sys.stderr.write('Duplicity in intruction argument tag name\n')
                            sys.exit(xml_err) 
                        else:
                            if arg.tag == 'arg1':
                                arg1 = True
                            elif arg.tag == 'arg2':
                                arg2 = True
                            elif arg.tag == 'arg3':
                                arg3 = True
                            else:
                                sys.stderr.write('Wrong instruction argument tag name\n')
                                sys.exit(xml_err)

                            # checking arg attributes number
                            if len(arg.attrib.keys()) != 1:    
                                sys.stderr.write('Wrong number of attributes in first argument tag\n')
                                sys.exit(xml_err)
                            # checking arg attributes name
                            elif 'type' not in arg.attrib:
                                sys.stderr.write('Wrong attribute name in first argument tag\n')
                                sys.exit(xml_err)

        # ##### save all instruction to instruction list

        for instruction in self.root.findall('instruction'):
            instr_name = instruction.attrib['opcode'].upper()
            i = 0
            temp_args_type = ['1', '1', '1']
            temp_args = ['1', '1', '1']
            for args in instruction:
                if args.tag == 'arg1':
                    temp_args[0] = args.text
                    temp_args_type[0] = args.attrib['type']
                if args.tag == 'arg2':
                    temp_args[1] = args.text
                    temp_args_type[1] = args.attrib['type']
                if args.tag == 'arg3':
                    temp_args[2] = args.text
                    temp_args_type[2] = args.attrib['type']
                i = i + 1

            temp_instr = Instruction(instr_name, i, temp_args[0], temp_args[1], temp_args[2])
            temp_instr.args_type(temp_args_type)
            self.InstrList.append(temp_instr)

        # ##### checking all instructions from valid instruction associative array
        i = 0
        while i < len(self.InstrList):
            
            if self.InstrList[i].name.upper() not in self.all_instructions:
                sys.stderr.write('\n\nInstruction ' + self.InstrList[i].name + ' isnt valid\n')
                sys.exit(lex_syn_err)
            else:
                if self.InstrList[i].len != self.all_instructions[self.InstrList[i].name].len:
                    sys.stderr.write('Instruciton has wrong number of arguemnts\n')
                    sys.exit(lex_syn_err)
                    
                else:
                    j = 0
                    while j < self.InstrList[i].len:
                        # '1' because of my stupid solution for different but right order for instruction arguemnts
                        if self.InstrList[i].args_type[j] == '1':
                            sys.stderr.write('Nespravne poradie arguemntov')
                            sys.exit(xml_err)

                        if re.search(self.all_instructions[self.InstrList[i].name].args[j],
                                     self.InstrList[i].args_type[j]):
                            # checking for empty values in literals to set default
                            if not self.InstrList[i].args[j]:
                                if self.InstrList[i].args_type[j] == 'int':
                                    self.InstrList[i].args[j] = '0' 
                        
                                elif self.InstrList[i].args_type[j] == 'bool':
                                    self.InstrList[i].args[j] = 'true'   
                    
                                elif self.InstrList[i].args_type[j] == 'string':
                                    self.InstrList[i].args[j] = '' 
                                # empty value is of type var or label
                                else:
                                    sys.stderr.write("Empty argument for var or label: " +
                                                     str(self.InstrList[i].args[j]) + self.InstrList[i].args_type[j])
                                    sys.exit(lex_syn_err)
                        
                            elif re.search(self.regExp[self.InstrList[i].args_type[j]],
                                           self.InstrList[i].args[j]) is None:
                                sys.stderr.write("Wrong argument " + str(self.InstrList[i].args[j]))
                                sys.exit(lex_syn_err)  # SYNTAX ERROR wrong data type of argument value

                        else:
                            sys.stderr.write("Wrong type for argument " +
                                             self.all_instructions[self.InstrList[i].name].args[j] + ' ' +
                                             str(self.InstrList[i].args_type[j]))
                            sys.exit(lex_syn_err)  # SYNTAX ERROR wrong type attribute value
                        j = j + 1
            i = i + 1

    def get_instr_list(self):
        """ return fulfilled instruction list """
        return self.InstrList
