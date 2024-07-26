var fs=require("fs");
var maxLines=1000;
var maxMultiple=9;
var b,c;

var txt1=Array.from(new Array(maxLines)).map((o,i)=>`#define FOREACH_${i}(prio,na,FN, E, ...) \\
    case APPLY_SUB(MUL(na,prio),MUL(${i},prio)): \\
        FN; \\
        ip=APPLY_SUB(MUL(na,prio),MUL(${i},prio)); \\
    break; \\
    FOREACH_${i-1}(prio,na,E, __VA_ARGS__)
`).slice(1).join("\n");
var ct1=`
#define PP_CONCAT(arg0, arg1) PP_CONCAT_IMPL(arg0, arg1)
#define PP_CONCAT_IMPL(arg0, arg1) arg0 ## arg1
#define APPLY_SUB(x,y) PP_CONCAT(SUB_,y(x))
// #define PP_IDENTITY(arg0) arg0

//#define PP_COMMA PP_LPAREN() PP_RPAREN()                // -> PP_COMMA ( )
//#define PP_IDENTITY(PP_COMMA PP_LPAREN() PP_RPAREN())   // -> PP_COMMA() -> ,

#define FOREACH_0(prio,na,FN, ...) case MUL(na,prio):  FN;ip=MUL(na,prio); break;
${txt1}
`

var a=Array.from(new Array(maxLines)).map((o,i)=>`_${i}`).slice(1);
a.reverse();
var ct2=`#define N_VA_ARGS_(${a.join(",")}, N, ...) N`;

a=Array.from(new Array(maxLines)).map((o,i)=>`${i}`);
a.reverse();
var ct3=`#define N_VA_ARGS(...) N_VA_ARGS_(__VA_ARGS__ __VA_OPT__(,) ${a.join(",")})`;



a=Array.from(new Array(maxLines*maxMultiple)).map((o,i)=>`#define INC_${i} ${i+1}`).join("\n");
var ct4=`
#define INC(x) INC_ ## x
${a}
`

a=Array.from(new Array(maxLines*maxMultiple)).map((o,i)=>`#define ADD_${i}(x) ADD_${i-1}(INC(x))`).slice(2).join("\n");

var ct5=`
#define ADD(x, y) ADD_ ## x(y)
#define ADD_0(x) x
#define ADD_1(x) INC(x)
${a}
`
a=Array.from(new Array(maxLines*maxMultiple)).map((o,i)=>`#define ___DEC_${i} ${i-1}`).join("\n");
b=Array.from(new Array(maxLines*maxMultiple)).map((o,i)=>`#define SUB_${i}(x) SUB_${i-1}(___DEC(x))`).slice(2).join("\n");
c=Array.from(new Array(maxLines*maxMultiple)).map((o,i)=>`#define MUL_${i}(a) ADD_##a(MUL_${i-1}(a))`).slice(2).join("\n");
var ct6=`
#define ___DEC(x) ___DEC_ ## x
${a}
#define SUB(x, y) SUB_ ## y(x)
#define SUB_0(x) x
#define SUB_1(x) ___DEC(x)
${b}



#define MUL(x,y) MUL_##x(y)
#define MUL_0(a) 0
#define MUL_1(a) a
${c}
`


fs.writeFileSync("./macro.h",`
#pragma once

${ct4}
${ct5}
${ct6}
${ct2}
${ct3}
${ct1}
`)
//var a=Array.from(new Array(1000)).map((o,i)=>`_${i}`).slice(1);a.reverse();copy(`#define N_VA_ARGS_(${a.join(",")}, N, ...) N`)
//var a=Array.from(new Array(1000)).map((o,i)=>`${i}`);a.reverse();copy(`#define N_VA_ARGS(...) N_VA_ARGS_(__VA_ARGS__ __VA_OPT__(,) ${a.join(",")})`)