#include "ast.hpp"

#include <string>
#include <regex>

static int makeNameUnq=0;

static std::string makeName(std::string base)
{
    return "_"+base+"_"+std::to_string(makeNameUnq++);
}

void CompileRec(
    std::string destReg,    // The name of the register to put the result in
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    
    if( regex_match( program->type, reNum ) ){
        std::cout<<"const "<<destReg<<" "<<program->type<<std::endl;
        
    }else if( regex_match( program->type, reId ) ){
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        std::cout<<"add "<<destReg<<" "<<program->type<<" "<<zero<<std::endl;

    }else if(program->type=="Param"){
        std::cout<<"param "<<destReg<<" "<<program->value<<std::endl;

    }else if(program->type=="Seq"){
        for(unsigned i=0; i<program->branches.size(); i++){
            CompileRec(destReg, program->branches[i]);
        }
    }
    else if(program->type== "Output"){  //can i output a deistantion register?
        (CompileRec(destReg, program->branches[0])); //run program, put value of calculation into destReg
        // We need to run the calucaltion and store the value in destReg
        //The program is recusrive, so destReg will be different destinations, and will contain the value of the output
        //so we just need to output it and return its value at the end of the code.
        std::cout << "output " << destReg << std::endl;
        //std::cout << "halt " << destReg << std::endl;   no need as already stored in destReg
    }
    else if(program->type =="Input"){
        std::cout << "input " << destReg << std::endl;
        //maybe return
    }
    else if(program->type == "LessThan"){ //not sure if i can stoe into srcReg and use it in compilation
        std::string val1 = makeName("val1");
        std::string val2 = makeName("val2");
        CompileRec(val1, program->branches[0]);
        CompileRec(val2, program->branches[1]);
        std::cout << "lt " << destReg << " " <<  val1 << " " << val2 << std::endl;
    }
    else if(program->type == "Add"){
        std::string val1 = makeName("val1");
        std::string val2 = makeName("val2");
        CompileRec(val1, program->branches[0]);
        CompileRec(val2, program->branches[1]);
        std::cout << "add " << destReg << " " <<  val1 << " " << val2 << std::endl;
    }
    else if(program->type == "Sub"){
        std::string val1 = makeName("val1");
        std::string val2 = makeName("val2");
        CompileRec(val1, program->branches[0]);
        CompileRec(val2, program->branches[1]);
        std::cout << "sub " << destReg << " " <<  val1 << " " << val2 << std::endl;
    }
    else if(program->type == "Assign"){
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        CompileRec(destReg, program->branches[0]);
        std::cout << "add " << program->value << " " << destReg << " " << zero << std::endl;
    }
    else if(program->type == "If"){
        std::string branchval = makeName("branchval");
        CompileRec(branchval, program->branches[0]);       
        std::string zero = makeName("zero");
        std::cout << "const "<<zero<<" 0"<<std::endl;
        std::string equalzero = makeName("equalzero");
        std::string end = makeName("end");
        std::cout << "beq " << branchval << " " << zero << " " << equalzero << std::endl;
        CompileRec(destReg, program->branches[1]);
        std::cout << "bne " << branchval << " " << zero << " " << end << std::endl;
        std::cout << ":" << equalzero <<std::endl;
        CompileRec(destReg, program->branches[2]);
        std::cout << ":" << end <<std::endl;
        /*
        branchval = compilation 0
        zero = 0
        beq branchval zero equalzero 
        destreg = compilation 1
        bne branchval zero end
        :equalzero
        destReg = compilation 2
        :end
        */
    }
    else if(program->type == "While") { //check might not be correct with jumping to end and doing nothing
        CompileRec(destReg, program->branches[0]);
        std::string zero = makeName("zero");
        std::cout << "const "<< zero <<" 0"<<std::endl;
        std::string iszero = makeName("iszero");
        std::string nonzero = makeName("nonzero");
        std::cout << "beq " << destReg << " " << zero << " " << iszero << std::endl;
        std::cout << ":" << nonzero <<std::endl;
        CompileRec(destReg, program->branches[1]);
        CompileRec(destReg, program->branches[0]);
        std::cout << "bne " << destReg << " " << zero << " " << nonzero << std::endl;
        std::cout << ":" << iszero <<std::endl;

        /*
        destReg = compilation 0
        zero = 0
        bne destReg zero nonzero
        halt zero
        :nonzero 
        destReg = compilation 1;
        destReg = compilation 0
        bne destReg zero nonzero
        halt zero
        */
    }
    else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}

void Compile(
    TreePtr program
){
    // Create a register to hold the final result
    std::string res=makeName("res");
    
    // Compile the whole thing
    CompileRec(res, program);
    
    // Put the result out
    std::cout<<"halt "<<res<<"\n";
}
