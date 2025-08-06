ident  := [a-zA-Z_][a-zA-Z0-9_]*
intlit := [0-9]+
dbllit := [0-9]+\.
op     := \+|\*|/|-|%

lit := 
  | intlit
  | dbllit

expr :=
  | ( <expr> )
  | <lit> <plus> <expr>
  | <lit> <minus> <expr>
  | <assign>

statement :=
  | if <expr> <block>

assign :=
  | <id> <eq> <expr>

vardec :=
  | <id> <colon> <id>
