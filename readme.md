ralcalc
=======

ralcalc is a simple command line calculator designed to take a single line
input and produce a result. It is also installed as the "=" command.

For example:

```
= 2*9
2*9 = 18
```

Both the input and output make use of SI prefixes (see 
http://en.wikipedia.org/wiki/SI_prefix ) for easier input and reading of
results. Only the common engineering prefixes that are powers a multiple of
three are used, so G, M, k, m, u, n, etc.

Numbers can be input using an "e" to indicate a power of ten, so

```
= 1e-3*1000
1e-3*1000 = 1
```

ralcalc expects the SI prefixes to be at the end of a number, so 1e-3n is
valid but 1ne-3 is not. Basically, I'd avoid mixing the two.

Version information can be shown with:

```
= -h
ralcalc -h
```

The -q option causes the input and any error messages to be not printed to the
screen.

The -e option causes the result to be displayed in the "1e-3" form rather than
with SI prefixes.

The -r option causes the result to be displayed with neither SI prefixes nor
exponents.


Operators
---------

```
+		- Addition
-		- Subtraction
* x		- Multiplication (the 'x' is for use when shell expansion may be a
		  problem)
/		- Division
^		- Power
[]		- Brackets - increase the precedence of anything within the brackets.
()		  Both sets of brackets are directly equivalent, so = (3+2]/2 is
          valid.
_		- The result of the last ralcalc run (stored in $HOME/.ralcalc_result)
```


```
= 2^.5
2^.5 = 1.41421

= 1/_
1/_ = 707.107m
```


Precedence
----------

The equations are processed in order of ^, * /, + -. Anything within
brackets [] is processed first.


Licence
-------

This code is licensed under GPL licence. See the COPYING file for more
information.


Author
------

ralcalc was written by Roger Light to fulfill his own needs. He knows that
there are other languages that are better suited to this kind of thing, but he
isn't really bothered about that. 

Updates should be available at http://atchoo.org/tools/ralcalc/
Questions/suggestions should be directed to roger@atchoo.org

