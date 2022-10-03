%option noyywrap
 
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    int numLines = 1; // initialize to 1
    void ret_print(char *token_type);
    void yyerror();
%}
 
%x ML_COMMENT
 
alpha       [a-zA-Z]
digit       [0-9]
alnum       {alpha}|{digit}
print       [ -~]
 
IDENTIFIER          {alpha}+{alnum}*
INTEGER     "0"|[0-9]{digit}*
FLOAT       "0"|{digit}*"."{digit}+
CHAR        (\'{print}\')|(\'\\[nftrbv]\')
STRING      \"{print}*\"
KEYWORD     "auto"|"switch"|"case"|"for"|"do"|"while"|"int"|"char"|"float"|"double"|"if"|"else"|"return"|"break"|"continue"|"const"|"enum"|"extern"|"goto"|"register"|"restrict"|"short"|"signed"|"unsigned"|"sizeof"|"static"|"inline"|"struct"|"class"|"typedef"|"union"|"void"|"volatile"
 
%%
 
"//".*                  { printf("Jumped single line comment at line %d\n", numLines); } 
 
"/*"                    { printf("Jumped multi-line comment from line %d ", numLines); BEGIN(ML_COMMENT); }
<ML_COMMENT>"*/"        { printf("to line %d\n", numLines); BEGIN(INITIAL); }
<ML_COMMENT>[^*\n]+     
<ML_COMMENT>"*"         
<ML_COMMENT>"\n"        { numLines += 1; }
 
 
KEYWORD                 { ret_print("KEYWORD");}
 
 
"+"|"-"|"*"|"/"|"++"|"--"|"+="|"-="|"*="|"/="       { ret_print("arith-op"); }
"|"|"&"|"~"|"|="|"&="|"~="|"<<"|"<<="|">>"|">>="    { ret_print("bitop"); }
"&&"|"||"                                           { ret_print("logic-op"); }
"="                                                 { ret_print("assign-op"); }
"=="|"!="|"!"|">"|"<"|">="|"<="                     { ret_print("RELOP"); }
 
 
"("|")"|"]"|"["|"{"|"}"    { ret_print("brace"); }
","|";"                    { ret_print("delimeter"); }
"."                        { ret_print("postfix operator"); }
 
 
{KEYWORD}       { ret_print("keyword"); }
{IDENTIFIER}    { ret_print("identifier"); }
{INTEGER}       { ret_print("integer"); }
{FLOAT}         { ret_print("float"); }
{CHAR}          { ret_print("char"); }
{STRING}        { ret_print("string"); }
 
 
"\n"            { numLines += 1; }
[ \t\r\f]+          /* jump whitespace */
 
.               { yyerror("Unrecognized character"); }
 
%%
 
void ret_print(char *token_type){
    printf("L%d: \t<%s, %s>\n", numLines, token_type, yytext);
}
 
void yyerror(char *message){
    printf("ERROR: \"%s\" in L%d. Token = %s\n", message, numLines, yytext);
    exit(1);
}
 
int main(int argc, char *argv[]){
    yyin = fopen(argv[1], "r");
    yylex();
    fclose(yyin);
    return 0;
}
