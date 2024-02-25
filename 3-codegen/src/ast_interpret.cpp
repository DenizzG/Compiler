#include "ast.hpp"

#include <regex>

int32_t Interpret(
    InterpretContext &context, // Contains the parameters and variable bindings
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    
    if( regex_match(program->type, reNum) ){
        return std::atol(program->type.c_str());
        
    
    // TODO : Check for things matching reId
    }else if( regex_match(program->type, reId) ){
        //go to context bindings(dictionary of variable names and their values)
        //search for name of varaible (program->type) and return the value of that variable
        return context.bindings[program->type]; 

    }else if(program->type=="Param"){
        unsigned index=atol(program->value.c_str()); //stores integer
        auto value=context.params.at(index); //context.params stores parameters ,gives the value at that param index
        return value; //returns the value
        
    }else if(program->type=="Output"){
        int32_t val=Interpret(context, program->branches.at(0)); //32 bit integer, interpret to get value and then output
        std::cout << val << std::endl;
        return val;
        
    }else if(program->type == "Input"){
        int32_t val;
        std::cin >> val;
        return val;
    }
    else if (program->type == "LessThan"){
        int32_t a = Interpret(context, program->branches.at(0));
        int32_t b = Interpret(context, program->branches.at(1));
        if (a<b) return 1;
        return 0;
    }
    else if (program->type == "Add"){
        int32_t a = Interpret(context, program->branches.at(0));
        int32_t b = Interpret(context, program->branches.at(1));
        a +=b;
        return a;
    }
    else if (program->type == "Sub"){
        int32_t a = Interpret(context, program->branches.at(0));
        int32_t b = Interpret(context, program->branches.at(1));
        a = a - b;
        return a;
    }
    else if(program->type == "Assign"){
        int32_t n = Interpret(context, program->branches.at(0));
        context.bindings[program->value] = n;
        return n;
    }  

    // TODO: Handle other constructs (Seq, If, While)
    else if(program->type == "Seq"){
        int32_t n;
        for(unsigned i_counter = 0; i_counter < program->branches.size(); i_counter++){
            n = Interpret(context, program->branches.at(i_counter)); //interpret all instrctions until you have last instruction which is returned
        }
        return n;
    }

    else if(program->type == "If"){
        int32_t n = Interpret(context, program->branches.at(0));
        if (n == 0){
            int32_t y = Interpret(context, program->branches.at(2));
            return y;
        }
        else{
            int32_t x = Interpret(context, program->branches.at(1));
            return x;
        }
    }

    else if(program->type == "While"){
        while (Interpret(context, program->branches.at(0)) != 0){
            int32_t n = Interpret(context, program->branches.at(1));
        }
        return 0;
    }

        
    else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}
