# data structures for interpret
# Jakub Kubik (xkubik32)
# 15.4.2018

import sys

# Error codes for interpret
semantic_err = 52
operand_type_err = 53
no_var_err = 54
no_frame_err = 55
no_value_err = 56
zero_div_err = 57
str_operation_err = 58

# Error codes for lex and syn analysis of xml file
xml_err = 31
lex_syn_err = 32

# Error codes for param and xml file
program_param_err = 10
code_in_xml_err = 11


class DataStructForInterpret:
    # TODO change crate from global variables structure here
    pass


class Variable:
    """ Class for variables saved  in frames """

    def __init__(self, name, data_type, value):
        self.name = name
        self.data_type = data_type
        self.value = value
    

class Frame:
    """ class for all types of frame """

    def __init__(self):
        self.vars_dict = dict()

    def insert_var(self, variable):
        self.vars_dict.update([(variable.name, variable)])
    
    def find_var(self, var_name):
        if var_name in self.vars_dict:
            return True
        else:
            return False

    def get_var(self, var_name):
        return self.vars_dict[var_name]

    def get_var_type(self, var_name):
        return self.vars_dict[var_name].dataType
    
    def get_var_value(self, var_name):
        return self.vars_dict[var_name].value

    # debugging
    def print_var_label_dict(self):
        print('=== Var dict ===')
        for key, value in self.vars_dict.items():
            print('key: ' + key)
            print('name: ' + value.name + 'dat type: ' + value.data_type + ', real type:' +
                  str(type(value.value)) + ', Value: ' + str(value.value))
    

class AllLabels:
    """ Class for saving and checking all labels """

    def __init__(self, instr_list):
        self.label_index = dict()
        i = 0
        for label in instr_list:
            if label.name.lower() == 'label':
                self.label_index.update({label.args[0]: i})
            i = i + 1    

    def find_label(self, label_name):
        if label_name in self.label_index:
            return True
        else:
            return False

    def get_label(self, label_name):
        return self.label_index[label_name]
    
    def get_all_dict(self):
        return self.label_index
    
    def check_labels_redefinition(self, instr_list):
        for key, value in self.label_index.items():
            i = 0
            for label in instr_list:
                if len(label.args) > 0:
                    if label.name.upper() == 'LABEL':
                        if label.args[0] == key:
                            i = i + 1
                        if i == 2:
                            sys.stderr.write('Redefinition of label\n')
                            sys.exit(52)

    # later i wont need
    def print_all_labels(self):
        for k, v in self.label_index.items():
            print('Label key: ' + k + ', value: ' + str(v))
