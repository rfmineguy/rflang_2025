ident  := [a-zA-Z_][a-zA-Z0-9_]*
intlit := [0-9]+
dbllit := [0-9]+\.
op     := \+|\*|/|-|%

lit := 
  | intlit
  | dbllit

expression       := <logical_disj>

logical_disj     := <logical_disj> "||" <logical_conj>
                  | <logical_conj>

logical_conj     := <logical_conj> "&&" <relational>
                  | <relational>

relational       := <relational> ">=" <math_expression>
                  | <relational> "<=" <math_expression>
                  | <relational> ">"  <math_expression>
                  | <relational> "<"  <math_expression>
                  | <relational> "==" <math_expression>
                  | <math_expression>

math_expression  := <math_expression> "+" <term>
                  | <math_expression> "-" <term>
                  | <term>

term              := <term> "*" <factor>
                  | <term> "/" <factor>
                  | <term> "%" <factor>
                  | <factor>

factor            := "(" <expression> ")"
                  | <func_call>
                  | <deref>
                  | <number>
                  | <strlit>
                  | <id>

statement :=
  | if <expr> <block>

assign :=
  | <id> <eq> <expr>

vardec :=
  | <id> <colon> <id>
