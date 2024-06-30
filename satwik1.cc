/*
 Original Work by 
 Name - Sai Shiva Satwik Mallajosyula
 ASU ID - 1222168012
*/
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <stack>
#include <unordered_map>

#include "lexer.h"
#include "compiler.h"

using namespace std;


LexicalAnalyzer lexer;
unordered_map<string,int> var;
void parse_body();
void parse_assign_stmt();
void parse_default();

struct InstructionNode* n = new InstructionNode;
   

struct InstructionNode * head = nullptr;
struct InstructionNode * tmp = nullptr;

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




void parse_variables(){
    tok = lexer.GetToken(); 
    while(tok.token_type != LBRACE){
        if(tok.token_type == ID){ //if token is a variable
            mem[location(tok.lexeme)]= 0;

        }
        tok = lexer.GetToken();
    }

    
    

}

void parse_condition(){
    

}


// IF a<0 {}
void parse_if_stmt(){
    struct InstructionNode * newNode = new InstructionNode;
    newNode->next = nullptr;
    // cout<<"i am here before taking if"<<tok.lexeme<<endl;
    tok = lexer.GetToken();// consume IF
    // cout<<"i am here after taking if"<<tok.lexeme<<endl;
    if(tok.token_type == IF){

        newNode->type = CJMP;

        //CONDITION
        tok = lexer.GetToken(); //if conition If a  OPERAND 1=a
            // cout << tok.lexeme<<endl;
            // cout << "I am here take first operand"<<tok.lexeme<<endl;
            newNode->cjmp_inst.operand1_index = location(tok.lexeme);
            if(tok.token_type == NUM){
                // cout << "inside stoi 1 to check stoi error"<<tok.lexeme<<endl;
                mem[location(tok.lexeme)]=stoi(tok.lexeme);
                
            }
            tok = lexer.GetToken(); //take the operator <

            if(tok.token_type == GREATER){ //relop
                //  cout << "operator"<<tok.token_type<<endl;
                newNode->cjmp_inst.condition_op = CONDITION_GREATER;

            }
            else if(tok.token_type == LESS){ //relop
                newNode->cjmp_inst.condition_op = CONDITION_LESS;

            }

            else if(tok.token_type == NOTEQUAL){ //relop
                newNode->cjmp_inst.condition_op = CONDITION_NOTEQUAL;

            }

            tok = lexer.GetToken(); //take second operand 0
            // cout << tok.lexeme<<endl;
            // cout << "I am here take second operand"<<tok.token_type<<endl;
            if(tok.token_type == ID || tok.token_type == NUM){//primary
                newNode->cjmp_inst.operand2_index = location(tok.lexeme);
                if(tok.token_type == NUM){
                    // cout << "inside stoi 2 to check stoi error"<<tok.lexeme<<endl;
                    mem[location(tok.lexeme)]=stoi(tok.lexeme);
                }
                
            }
            if(head == nullptr){
                head = newNode;
                tmp=head;
            }
            else{
                tmp->next = newNode;
                tmp=newNode;
            }

            tok = lexer.GetToken(); //left brace
            
            parse_body();
        
    }
   

    InstructionNode *noopNode = new InstructionNode;
    noopNode->type = NOOP;
    noopNode->next = nullptr;
    newNode->cjmp_inst.target=noopNode;

    if(head == nullptr){
        head = noopNode;
        tmp=noopNode;
    }
    else{
        tmp->next = noopNode;
        tmp=noopNode;
    }


    //if a<0
    //{
    // body    
    //}
    //noopnode












}

void parse_case(Token token){
   

    struct InstructionNode* newNode = new InstructionNode;
    newNode->next = nullptr;
    newNode->type = CJMP;
    newNode->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
    newNode->cjmp_inst.operand1_index = location(token.lexeme);
    


   
    
    tok = lexer.GetToken();//CASE
    

    tok = lexer.GetToken(); //NUM
   

    
   
    newNode->cjmp_inst.operand2_index = location(tok.lexeme);
    mem[location(tok.lexeme)]=stoi(tok.lexeme);
    

    tok = lexer.GetToken(); //COLON
    tok = lexer.GetToken(); //LBRACE

    if(head == nullptr){
        head = newNode;
        tmp=head;
    }
    else{
        tmp->next = newNode;
        tmp=newNode;
    }

    
    parse_body();
    

    newNode->cjmp_inst.target = newNode->next;
     n->next = nullptr;
    n->type = NOOP;
    tmp->next =n;
    tmp = newNode; 
    tmp->next = nullptr;



    
   


    
    
    









    

}

void parse_case_list(Token token){
    Token toke = lexer.peek(1);
    if(toke.token_type == CASE){
   
        parse_case(token);
        parse_case_list(token);
    }

    else{
        
        return;
    }


}

void parse_default(){

    
    tok = lexer.GetToken();//Default
    

    tok = lexer.GetToken(); //Colon


    tok = lexer.GetToken(); //Left Brace

    
    parse_body();
    

    



    


    

    






}
    



void parse_switch_stmt(){
    
    
    tok = lexer.GetToken(); //switch
    
    Token t;
    tok = lexer.GetToken(); // ID(Variable)

    t = tok;
    
    tok = lexer.GetToken(); //left brace;
    

    parse_case_list(t);

    
    

    Token t1 = lexer.peek(1); // Either default or rbrace;

    if(t1.token_type == RBRACE){
        tok = lexer.GetToken();

    }
    else if(t1.token_type == DEFAULT){
        
        parse_default();
        tok = lexer.GetToken(); //rbrace

        

        
    }

    

    if(head==nullptr)
    {
        head= n;
    }else{
        tmp->next=n;
    }
    tmp=n;






    

}

void parse_for_stmt(){
    struct InstructionNode * newNode = new InstructionNode;
    struct InstructionNode * newNode1 = new InstructionNode;
    newNode->next = nullptr;
    newNode1->next = nullptr;
    tok = lexer.GetToken(); //for
    
    tok = lexer.GetToken(); //LPARAN
    
    parse_assign_stmt();
    
    

    newNode->type = CJMP;
    tok = lexer.GetToken(); //OPERAND 1
    
    
    newNode->cjmp_inst.operand1_index = location(tok.lexeme);
    if(tok.token_type == NUM){
        // cout << "inside stoi 1 to check stoi error"<<tok.lexeme<<endl;
        mem[location(tok.lexeme)]=stoi(tok.lexeme);
        
    }
    tok = lexer.GetToken(); //take the operator <

    if(tok.token_type == GREATER){ //relop
        //  cout << "operator"<<tok.token_type<<endl;
        newNode->cjmp_inst.condition_op = CONDITION_GREATER;

    }
    else if(tok.token_type == LESS){ //relop
        newNode->cjmp_inst.condition_op = CONDITION_LESS;

    }

    else if(tok.token_type == NOTEQUAL){ //relop
        newNode->cjmp_inst.condition_op = CONDITION_NOTEQUAL;

    }

    tok = lexer.GetToken(); //take second operand 0
    // cout << tok.lexeme<<endl;
    // cout << "I am here take second operand"<<tok.token_type<<endl;
    if(tok.token_type == ID || tok.token_type == NUM){//primary
        newNode->cjmp_inst.operand2_index = location(tok.lexeme);
        if(tok.token_type == NUM){
            // cout << "inside stoi 2 to check stoi error"<<tok.lexeme<<endl;
            mem[location(tok.lexeme)]=stoi(tok.lexeme);
        }
        
    }
    if(head == nullptr){
        head = newNode;
        tmp=head;
    }
    else{
        tmp->next = newNode;
        tmp=newNode;
    }

    tok = lexer.GetToken(); //SEMI COLON


    tok = lexer.GetToken();
    // cout << tok.token_type << endl;
    if(tok.token_type == ID){
        newNode1->type = ASSIGN;
        newNode1->assign_inst.left_hand_side_index = location(tok.lexeme);
        tok = lexer.GetToken();
        
    
        if(tok.token_type == EQUAL){
            tok = lexer.GetToken();
            if(tok.token_type == ID || tok.token_type == NUM){ //parse primary
                newNode1->assign_inst.operand1_index = location(tok.lexeme);
                if(tok.token_type == NUM){
                    
                    mem[location(tok.lexeme)]=stoi(tok.lexeme);
                    
                   
                }
                
                tok = lexer.GetToken();
                
                if(tok.token_type == SEMICOLON) {
                    
                    newNode1->assign_inst.op=OPERATOR_NONE;

                    
                    
                    
                    
                }

                else if(tok.token_type == PLUS){
                    newNode1->assign_inst.op = OPERATOR_PLUS;
                    tok = lexer.GetToken();
                    if(tok.token_type == ID || tok.token_type == NUM){
                        newNode1->assign_inst.operand2_index = location(tok.lexeme);
                        
                        if(tok.token_type == NUM){
                            
                            mem[location(tok.lexeme)]=stoi(tok.lexeme);

                        }
                        
                        tok = lexer.GetToken();
                        
                            

                           
                            
                           
                        
                    }

                }

                else if(tok.token_type == MINUS){
                    newNode1->assign_inst.op = OPERATOR_MINUS;
                    tok = lexer.GetToken();
                    if(tok.token_type == ID || tok.token_type == NUM){
                        newNode1->assign_inst.operand2_index = location(tok.lexeme);
                        if(tok.token_type == NUM){
                            
                            mem[location(tok.lexeme)]=stoi(tok.lexeme);

                        }
                        
                        tok = lexer.GetToken();
                        
                            

                            
                            
                           
                        
                    }

                }

                else if(tok.token_type == DIV){
                    newNode1->assign_inst.op = OPERATOR_DIV;
                    tok = lexer.GetToken();
                    if(tok.token_type == ID || tok.token_type == NUM){
                        newNode1->assign_inst.operand2_index = location(tok.lexeme);
                        if(tok.token_type == NUM){
                            
                            mem[location(tok.lexeme)]=stoi(tok.lexeme);

                        }
                        
                        tok = lexer.GetToken();
                        
                            

                            
                            
                           
                        
                    }

                }

                else if(tok.token_type == MULT){
                    newNode1->assign_inst.op = OPERATOR_MULT;
                    tok = lexer.GetToken(); //op2
                    
                    newNode1->assign_inst.operand2_index = location(tok.lexeme);
                    if(tok.token_type == NUM){
                        
                        mem[location(tok.lexeme)]=stoi(tok.lexeme);

                    }
                    
                    tok = lexer.GetToken();//semicolon
                    
                        

                   
                        
                        
                    
                    

                }


            }
        }
    }

     

    // lexer.UngetToken(1);//removing semicolon

    tok=lexer.GetToken(); // RBRACE
    tok=lexer.GetToken(); //LBRACE
    

    parse_body();

    if(head == nullptr){
        head = newNode1;
        tmp=head;
    }

    else{
        tmp->next = newNode1;
        tmp=newNode1;
    }

    InstructionNode *jmp = new InstructionNode;
    jmp->type = JMP;
    jmp->next = nullptr;
    jmp->jmp_inst.target = newNode;
    tmp->next = jmp;
    tmp=tmp->next;


    InstructionNode *noopNode = new InstructionNode;
    noopNode->type = NOOP;
    noopNode->next = nullptr;
    newNode->cjmp_inst.target = noopNode;

    if(head == nullptr){
        head = noopNode;
        tmp=noopNode;
    }
    else{
        tmp->next = noopNode;
        tmp=noopNode;
    }
    

    



    
    
    

}
void parse_while_stmt(){
    struct InstructionNode * newNode = new InstructionNode;
    newNode->next = nullptr;
    tok = lexer.GetToken();//while
    
    
        newNode->type = CJMP;
        tok = lexer.GetToken(); //op1
            
            newNode->cjmp_inst.operand1_index = location(tok.lexeme);
            if(tok.token_type == NUM){
                
                mem[location(tok.lexeme)]=stoi(tok.lexeme);
                
            }
            tok = lexer.GetToken(); //operator

            if(tok.token_type == GREATER){ 
               
                newNode->cjmp_inst.condition_op = CONDITION_GREATER;

            }
            else if(tok.token_type == LESS){ 
                newNode->cjmp_inst.condition_op = CONDITION_LESS;

            }

            else if(tok.token_type == NOTEQUAL){ 
                newNode->cjmp_inst.condition_op = CONDITION_NOTEQUAL;

            }

            tok = lexer.GetToken(); //op2
            if(tok.token_type == ID || tok.token_type == NUM){//primary
                newNode->cjmp_inst.operand2_index = location(tok.lexeme);
                if(tok.token_type == NUM){
                    
                    mem[location(tok.lexeme)]=stoi(tok.lexeme);
                }
                
            }
            if(head == nullptr){
                head = newNode;
                tmp=head;
            }
            else{
                tmp->next = newNode;
                tmp=newNode;
            }

            tok = lexer.GetToken(); //left brace
            
            parse_body();
        


    
    InstructionNode *jmp = new InstructionNode;
    jmp->type = JMP;
    jmp->next = nullptr;
    jmp->jmp_inst.target = newNode;
    tmp->next = jmp;
    tmp=tmp->next;


    InstructionNode *noopNode = new InstructionNode;
    noopNode->type = NOOP;
    noopNode->next = nullptr;
    newNode->cjmp_inst.target = noopNode;

    if(head == nullptr){
        head = noopNode;
        
    }
    else{
        tmp->next = noopNode;
        
    }

    tmp=noopNode;

    
    

}






void parse_output_stmt(){
    struct InstructionNode * newNode = new InstructionNode;
    newNode->next = nullptr;
    tok = lexer.GetToken();//take output 
    if(tok.token_type == OUTPUT){
        newNode->type = OUT;
        tok = lexer.GetToken();
        if(tok.token_type == ID)
        {
            newNode->output_inst.var_index = location(tok.lexeme);
            tok = lexer.GetToken();
            if(tok.token_type == SEMICOLON) {
                

                 if(head == nullptr){
                        head = newNode;
                    }

                    else{
                        tmp->next = newNode;
                        
                    }
                    tmp=newNode;
                
            }
        }

    }


    

}

void parse_input_stmt(){
    struct InstructionNode * newNode = new InstructionNode;
    newNode->next = nullptr;
    tok = lexer.GetToken();// input
    if(tok.token_type == INPUT){
        newNode->type = IN;
        tok = lexer.GetToken(); // input var
        if(tok.token_type == ID)
        {
            newNode->input_inst.var_index = location(tok.lexeme);
            tok = lexer.GetToken();//semicolon
            if(tok.token_type == SEMICOLON) {
                

                 if(head == nullptr){
                        head = newNode;
                        
                    }

                    else{
                        tmp->next = newNode;
                        
                    }
                    tmp=newNode;
                
            }
        }

    }
    

}

void parse_assign_stmt(){
    struct InstructionNode * newNode = new InstructionNode;
    newNode->next = nullptr;
    tok = lexer.GetToken();
    // cout << tok.token_type << endl;
    if(tok.token_type == ID){
        newNode->type = ASSIGN;
        newNode->assign_inst.left_hand_side_index = location(tok.lexeme);
        tok = lexer.GetToken();
        
    
        if(tok.token_type == EQUAL){
            tok = lexer.GetToken();
            if(tok.token_type == ID || tok.token_type == NUM){ //parse primary
                newNode->assign_inst.operand1_index = location(tok.lexeme);
                if(tok.token_type == NUM){
                    
                    mem[location(tok.lexeme)]=stoi(tok.lexeme);
                    
                   
                }
                
                tok = lexer.GetToken();
                
                if(tok.token_type == SEMICOLON) {
                    
                    newNode->assign_inst.op=OPERATOR_NONE;

                    if(head == nullptr){
                        head = newNode;
                        tmp=head;
                    }

                    else{
                        tmp->next = newNode;
                        tmp=newNode;
                    }
                    
                    
                    
                }

                else if(tok.token_type == PLUS){
                    newNode->assign_inst.op = OPERATOR_PLUS;
                    tok = lexer.GetToken();
                    if(tok.token_type == ID || tok.token_type == NUM){
                        newNode->assign_inst.operand2_index = location(tok.lexeme);
                        
                        if(tok.token_type == NUM){
                            
                            mem[location(tok.lexeme)]=stoi(tok.lexeme);

                        }
                        
                        tok = lexer.GetToken();
                        
                            

                            if(head == nullptr){
                                head = newNode;
                                tmp=head;
                            }

                            else{
                                tmp->next = newNode;
                                tmp=newNode;
                            }
                            
                           
                        
                    }

                }

                else if(tok.token_type == MINUS){
                    newNode->assign_inst.op = OPERATOR_MINUS;
                    tok = lexer.GetToken();
                    if(tok.token_type == ID || tok.token_type == NUM){
                        newNode->assign_inst.operand2_index = location(tok.lexeme);
                        if(tok.token_type == NUM){
                            
                            mem[location(tok.lexeme)]=stoi(tok.lexeme);

                        }
                        
                        tok = lexer.GetToken();
                        
                            

                            if(head == nullptr){
                                head = newNode;
                                tmp=head;
                            }

                            else{
                                tmp->next = newNode;
                                tmp=newNode;
                            }
                            
                           
                        
                    }

                }

                else if(tok.token_type == DIV){
                    newNode->assign_inst.op = OPERATOR_DIV;
                    tok = lexer.GetToken();
                    if(tok.token_type == ID || tok.token_type == NUM){
                        newNode->assign_inst.operand2_index = location(tok.lexeme);
                        if(tok.token_type == NUM){
                            
                            mem[location(tok.lexeme)]=stoi(tok.lexeme);

                        }
                        
                        tok = lexer.GetToken();
                        
                            

                            if(head == nullptr){
                                head = newNode;
                                tmp=head;
                            }

                            else{
                                tmp->next = newNode;
                                tmp=newNode;
                            }
                            
                           
                        
                    }

                }

                else if(tok.token_type == MULT){
                    newNode->assign_inst.op = OPERATOR_MULT;
                    tok = lexer.GetToken(); //op2
                    
                    newNode->assign_inst.operand2_index = location(tok.lexeme);
                    if(tok.token_type == NUM){
                        
                        mem[location(tok.lexeme)]=stoi(tok.lexeme);

                    }
                    
                    tok = lexer.GetToken();//semicolon
                    
                        

                    if(head == nullptr){
                        head = newNode;
                        tmp=head;
                    }

                    else{
                        tmp->next = newNode;
                        tmp=newNode;
                    }
                        
                        
                    
                    

                }


            }
        }
    }
    

}




void parse_stmt(){
    
    //cout << "peek" << lexer.peek(1).token_type <<endl;
    
    if(lexer.peek(1).token_type == ID){
        
        // cout<<"assign"<<endl;
        parse_assign_stmt();
        
    }
    else if(lexer.peek(1).token_type == INPUT){
        // cout<<"input"<<endl;
        parse_input_stmt();

    }
    else if(lexer.peek(1).token_type == OUTPUT){
        // cout<<"output"<<endl;
        parse_output_stmt();
        
    }
    else if(lexer.peek(1).token_type == IF){
        // cout<<"if"<<endl;
        parse_if_stmt();
    
        
    }
    else if(lexer.peek(1).token_type == FOR){
        parse_for_stmt();
        
    }
    else if(lexer.peek(1).token_type == SWITCH){
        parse_switch_stmt();
        
    }
    else if(lexer.peek(1).token_type == WHILE){
        parse_while_stmt();
    }
    else{
        return;
    }
    


}
void parse_stmt_list(){
    Token token = lexer.peek(1);
    if(token.token_type == ID || token.token_type == INPUT || token.token_type == OUTPUT || token.token_type == IF || token.token_type == FOR || token.token_type == WHILE || token.token_type == SWITCH){
        // cout<<"going to parse statement now"<<endl;
        parse_stmt();
        // cout<<"came from parse statement"<<endl;
        // cout<<"going to parse statement list now"<<endl;
        parse_stmt_list();
        // cout<<"came from parse statement list"<<endl;
    }
    
    else{
        return;
    }

   

}






void parse_body(){
    
    //cout << tok.token_type << endl;
    
    if(tok.token_type ==  LBRACE){
       
        
        parse_stmt_list();
        // cout<<"parsed body not going back to if"<<tok.token_type<<" " <<tok.lexeme<<endl;
        //cout << 6 << endl;

        //to do check for right brace;
        tok = lexer.GetToken();
        // cout<<tok.token_type<<endl;

        

        //cout << 7 << endl;


    }
}

struct InstructionNode * parse_generate_intermediate_representation()
{
    head = nullptr;
    tmp = nullptr;
    struct InstructionNode* node1 = new InstructionNode();
    parse_variables();
   
    parse_body();
    
//parse inputs    
 

    tok = lexer.GetToken();
    // cout << "input section"<<endl;

    while(tok.token_type != END_OF_FILE) {
        // cout << "input section "<<tok.lexeme <<endl;
        int num = stoi(tok.lexeme);
        inputs.push_back(num);
        tok = lexer.GetToken();

    }
   


    
    return head;


}
