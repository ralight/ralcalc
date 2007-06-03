ralcalc
=======

ralcalc is a simple command line calculator designed to take a single line
input and produce a result. It is also installed as the "=" command.

For example:

= 2*9
2*9 = 18

Both the input and output make use of SI prefixes (see 
http://en.wikipedia.org/wiki/SI_prefix ) for easier input and reading of
results. Only the common engineering prefixes that are powers a multiple of
three are used, so G, M, k, m, u, n, etc.


Operators
---------

+		- Addition
-		- Subtraction
* x		- Multiplication (the 'x' is for use when shell expansion may be a
		  problem)
/		- Division
^		- Power
[]		- Brackets - increase the precedence of anything within the brackets.
_		- The result of the last ralcalc run (stored in $HOME/.ralcalc_result)


= 2^.5
2^.5 = 1.41421

= 1/_
1/_ = 707.107m


Precedence
----------

The equations are processed left to right, which breaks traditional operator
precedence. This is regarded as a bug and will be fixed! Anything within
brackets [] is processed first, so this can be used to work around the bad
behaviour.

= 1+2^.5
1+2^.5 = 1.73205 (incorrect)

= 1+[2^.5]
1+[2^.5] = 2.41421 (correct)


Problems
--------

Negation of values is only carried out on numbers, not on brackets.

= -3^3
-3^3 = -27 (correct)

= -[3^3]
-[3^3] = 27 (incorrect)


Licence
-------

This code is licensed under a BSD-style licence. See LICENCE.txt for more
information.


Author
------

ralcalc was written by Roger Light to fulfill his own needs. He knows that
there are other languages that are better suited to this kind of thing, but he
isn't really bothered about that. 

Updates should be available at http://atchoo.org/tools/ralcalc/
Questions/suggestions should be directed to roger@atchoo.org

