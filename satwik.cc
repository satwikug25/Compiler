#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <stack>

#include "lexer.h"
#include "compiler.h"

using namespace std;


LexicalAnalyzer lexer;
unordered_map<string,int> var;
stack<string> st;
InstructionNode * head = nullptr;
InstructionNode * tmp = nullptr;

Token tok;

int location(string variable){
    if(var.find(variable)!=var.end()){//variable found in the hashmap
        
        return var[variable];
    }
    else{ //variable not found in the hashmap
        
        var[variable] = next_available;
        next_available++;

        return var[variable];


    }



}

struct InstructionNode * parse_generate_intermediate_representation()
{
    tok = lexer.GetToken();
    while(tok.token_type != LBRACE){
        if(tok.token_type == ID){
            mem[location(tok.lexeme)]=0;

        }
        tok = lexer.GetToken();
    }

    st.push("}");

    

    while(!st.empty()){
        tok = lexer.GetToken();
        switch (tok.token_type)
        {
            case RBRACE:
                st.pop();
                
                break;
            case LBRACE:
                st.push("}");
                break;
  
            case INPUT:
                tmp->type = IN;
                tok = lexer.GetToken();
                while(tok.token_type != SEMICOLON){
                    if(tok.token_type == ID){
                        tmp->input_inst.var_index = location(tok.lexeme);
                        tok = lexer.GetToken();
                    }
                }
                tmp->next = new InstructionNode();
                tmp = tmp->next;



                break;

            case OUTPUT:
                tmp->type = OUT;
                tok = lexer.GetToken();
                while(tok.token_type != SEMICOLON){
                    if(tok.token_type == ID){
                        tmp->output_inst.var_index = location(tok.lexeme);
                        tok = lexer.GetToken();
                    }
                }
                tmp->next = new InstructionNode();
                tmp = tmp->next;

                break;
            
            case ID:
                tmp->type = ASSIGN;
                tmp->assign_inst.left_hand_side_index = location(tok.lexeme);
                tok = lexer.GetToken();//=
                tok = lexer.GetToken();//ID||NUM
                while(tok.token_type != SEMICOLON){
                    if(tok.token_type == ID || tok.token_type == NUM){
                        Token t=lexer.peek(1);
                        if(t.token_type == PLUS || t.token_type == MINUS || t.token_type == DIV || t.token_type == MULT){
                            
                        }
                        else{
                            tmp->assign_inst.op = OPERATOR_NONE;
                        }

                        tmp->assign_inst.operand1_index = location(tok.lexeme);

                        
                    }
                    else if(tok.token_type == PLUS){
                        tmp->assign_inst.op = OPERATOR_PLUS;
                        tok = lexer.GetToken();
                        tmp->assign_inst.operand2_index = location(tok.lexeme);


                    }
                    else if(tok.token_type == MINUS){
                        tmp->assign_inst.op = OPERATOR_MINUS;
                        tok = lexer.GetToken();
                        tmp->assign_inst.operand2_index = location(tok.lexeme);

                    }
                    else if(tok.token_type == MULT){
                        tmp->assign_inst.op = OPERATOR_MULT;
                        tok = lexer.GetToken();
                        tmp->assign_inst.operand2_index = location(tok.lexeme);

                    }
                    else if(tok.token_type == DIV){
                        tmp->assign_inst.op = OPERATOR_DIV;
                        tok = lexer.GetToken();
                        tmp->assign_inst.operand2_index = location(tok.lexeme);

                    }

                    

                    tok=lexer.GetToken();
                }
                tmp->next = new InstructionNode();
                tmp = tmp->next;

                break;
            default:
                break;
        }
    
    }

    tok = lexer.GetToken();
    while(tok.token_type != END_OF_FILE) {
        int num = stoi(tok.lexeme);
        inputs.push_back(num);
        tok = lexer.GetToken();

    }

    tmp->next = NULL;

    


    
    return head;


}
