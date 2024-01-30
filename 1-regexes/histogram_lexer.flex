%option noyywrap

%{
/* Now in a section of C that will be embedded
   into the auto-generated code. Flex will not
   try to interpret code surrounded by %{ ... %} */

/* Bring in our declarations for token types and
   the yylval variable. */
#include "histogram.hpp"

// This is to work around an irritating bug in Flex
// https://stackoverflow.com/questions/46213840/get-rid-of-warning-implicit-declaration-of-function-fileno-in-flex
extern "C" int fileno(FILE *stream);

/* End the embedded code section. */
%}


%%

(\[[^\]]*\])          {
      fprintf(stderr, "Word [] : %s\n", yytext);
      yytext++;
      yytext[strlen(yytext)-1] = 0;
      fprintf(stderr, "Word: %s\n", yytext);
      std::string* yytext_address = new std::string;
      *yytext_address = yytext;
      yylval.wordValue = yytext_address;     
      return Word;
      
}

(-?[0-9]+(\.[0-9]+)?)      {        

   fprintf(stderr, "Number (double) : %s\n", yytext);
   yylval.numberValue = strtod(yytext, NULL);
   return Number;
}


(-?[0-9]+(\/[0-9]+))       {   
   
   fprintf(stderr, "Number (fraction) : %s\n", yytext);  
   std::string numerator;
   std::string denominator;
   bool dash = false;
   for (size_t i = 0; i < strlen(yytext); i++){
      if (yytext[i] == '/'){
         dash = true;
      }
      else if (dash == false){
         numerator += yytext[i];
      }
      else {
         denominator += yytext[i];
      }
   }
   yylval.numberValue = (strtod(numerator.c_str(), NULL)/strtod(denominator.c_str(), NULL));
   return Number;

}


(([a-zA-Z])+)          {

   fprintf(stderr, "Word: %s\n", yytext);
   std::string* yytext_address = new std::string;
   *yytext_address = yytext;
   yylval.wordValue = yytext_address;     
   return Word;
}
 
[ ]*     {;}

.        {;}

\n        { fprintf(stderr, "Newline\n"); }


%%

/* Error handler. This will get called if none of the rules match. */
void yyerror (char const *s)
{
  fprintf (stderr, "Flex Error: %s\n", s); /* s is the text that wasn't matched */
  exit(1);
}
