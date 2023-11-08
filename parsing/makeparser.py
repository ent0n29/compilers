#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""1
Created on Mon Oct 31 10:38:01 2022

@author: Mauro Leoncini, for teaching purposes only
"""

import sys

INCLUDE_HPP = 'pparser.hpp'
TOKENS_H = 'tokens.h'

global tokens, variables, EPS
tokens = {}
variables = {}
EPS = 0

def makehpp(picklefile):
    from pickle import load
    with open(picklefile, "rb") as f:
        Object = load(f)
    
    rules = Object['rewriting_rules']
    variables = Object['variables']
    tokens = Object['tokens']
    productions = Object['productions']
    first = Object['first']
    follow = Object['follow']
    BASE_VARIABLES = Object['base']
    axiom = Object['axiom']
    
    def firstseq(prod):
        #pdb.set_trace()
        res = set()
        for e in prod:
            if e in tokens.values() or e==EPS:
                res.add(e)
                return res
            else:
                if EPS not in first[e]:
                    res = res | first[e]
                    return res
                else:
                    res = res | (first[e]-{EPS})
        res.add(EPS)
        return res
        
    def symb(v):
        nonlocal variables, tokens
        for name,value in {**tokens,**variables}.items():
            if value == v:
               return name
        return str(v)
    
    def conflict(head,e,p1,p2):
        print(f"Conflitto per la variabile {symb(head)}",end=' ')
        print(f"e il token {symb(e)}.\nSi possono applicare le",end=' ')
        print(f"produzioni {p1} e {p2}")

    with open(TOKENS_H,'w') as f:
        f.write('enum TokenType {\n')
        for name,val in tokens.items():
            if name[:3]=='tok':
                f.write('\t'+name+' = '+str(val)+',\n')
        f.write('};\n\n')
        
    with open(INCLUDE_HPP,'w') as f:
        f.write(f"#define EPS {EPS}\n")
        f.write(f"#define BASE_VARIABLES {BASE_VARIABLES}\n\n")
        #f.write("#define BASE_TOKENS -1\n\n")
        for name,val in variables.items():
            f.write('const int '+name+' = '+str(val)+';\n')
        f.write('const int AXIOM'+' = '+str(axiom)+';\n')
        f.write('\n')
        parsing_table = {}
        cnt = 0
        for head,tail in productions.items():
            parsing_table[head] = {}
            for i,rhs in enumerate(tail):
                cnt += 1
                prodname='prod'+str(cnt)
                f.write('std::vector<int> '+prodname+'{')
                for j,v in enumerate(rhs):
                    if j==0:
                        f.write(str(v))
                    else:
                        f.write(','+str(v))
                f.write('};\t// '+rules[cnt-1])
                fs = firstseq(rhs)
                for e in fs:
                    if e != EPS:
                        if not parsing_table[head].get(e,False):
                            parsing_table[head][e] = cnt
                        else:
                            conflict(head,e,cnt,parsing_table[head][e])
                            sys.exit(1)
                    else:
                        for s in follow[head]:
                            if not parsing_table[head].get(s,False):
                                parsing_table[head][s] = cnt
                            else:
                                conflict(head,s,cnt,parsing_table[head][s])
                                sys.exit()
        f.write("\nstd::map<int,std::vector<int>> prods = {")
        for j in range(1,cnt+1):
            if j==1:
                f.write('\n\t{'+str(j)+',{prod'+str(j)+'}}')
            else:
                f.write(',\n\t{'+str(j)+',{prod'+str(j)+'}}')
        f.write('\n};\n')
        f.write('std::map<int,std::map<int,int>> parsing_table;\n')
        f.write('void set_parsing_table() {\n')
        for var,col in parsing_table.items():
            f.write(f'\tparsing_table[{str(var)}]='+'{')
            start = True
            for tok,prod in col.items():
                if start:
                    f.write('{'+str(tok)+','+str(prod)+'}')
                    start = False
                else:
                    f.write(', {'+str(tok)+','+str(prod)+'}')
            f.write('};\n')
        f.write('};\n')
            
        f.write("\nstd::map<int,std::string> display = {")
        for j,rewrule in enumerate(rules):
            if j==0:
                f.write('\n\t{'+str(j+1)+',"'+rewrule.strip()+'"}')
            else:
                f.write(',\n\t{'+str(j+1)+',"'+rewrule.strip()+'"}')
        f.write('\n};\n')
        
if __name__=='__main__':
    nargs = len(sys.argv)
    if nargs==1:
        print("Errore: manca l'indicazione del pickle file con i dati della grammatica")
        sys.exit()
    else:
        makehpp(sys.argv[1])
    
    
