CPD
===

CPD Project

UPDATE 21-10-13 16:00

Três novas correcções foram acordadas:

1.
Em vez the se incrementar uma unidade o starvation period do wolf quando este come um squirrel,
cada vez que isto acontece, o starvation period do wolf toma o valor do starvation period
definido no input. Isto simplifica o problema de ser necessário de contabilidade o número de 
esquilos que o lobo comeu, quando se moveu.

2.
Cada sub-geração deve trabalhar sobre uma matriz com o mesmo estado, ou seja, a segunda sub-geração
não deve depender da primeira sub-geração. Para que isto seja realizado, recomenda-se que seja mantida
uma cópia do estado da matriz, que mantenha o extado da geração anterior.

3.
Wolfs passam apenas a morrer após realizarem um número de movimentos igual ao starvation period (caso
não comam nenhuma esquilo entretanto). Por exemplo, se o starvation period é 1 e o número de geracoes 1
o output deverá conter um lobo na nova posição. Por outro lado caso o número de geraçoes fosse superior
a 1 este morreria.
