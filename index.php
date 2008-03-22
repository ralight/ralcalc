<?php

require("/home/oojah/includes/main/logging.php");
dolog();
print("<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n");

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "DTD/xhtml1-strict.dtd">
<html><head><title>ralcalc</title>
<link rel="stylesheet" href="/tools/style.css" type="text/css" />
</head><body>

<div>
<h1>ralcalc version 20070603</h1>
<h2>What is ralcalc?</h2>
<p>ralcalc is a simple command line calculator designed to take a single line input and produce a result. I also have it installed as the "=" command for even quicker access.</p>

<p>For example:</p>

<pre>
= 2*9
2*9 = 18
</pre>

<p>Both the input and output make use of SI prefixes (see <a href="http://en.wikipedia.org/wiki/SI_prefix">http://en.wikipedia.org/wiki/SI_prefix</a>) for easier input and reading of results. Only the common engineering prefixes that are powers a multiple of three are used, so G, M, k, m, u, n, etc.</p>

<p>Version information can be shown with:</p>

<pre>
= -h
ralcalc -h
</pre>


<h2>Downloads</h2>
<ul>
<li><a href="ralcalc-20070603.tar.bz2">ralcalc-20070603.tar.bz2</a> (7.4kB) (<a href="ralcalc-20070603.tar.bz2.asc">GPG Signature</a>)</li>
</ul>

<h2>Operators</h2>

<ul>
<li>+&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Addition</li>
<li>-&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Subtraction</li>
<li>* x&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Multiplication (the 'x' is for use when shell expansion may be a problem)</li>
<li>/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Division</li>
<li>^&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Power</li>
<li>[]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Brackets - increase the precedence of anything within the brackets.</li>
<li>_&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The result of the last ralcalc run (stored in $HOME/.ralcalc_result)</li>
</ul>


<pre>
= 2^.5
2^.5 = 1.41421

= 1/_
1/_ = 707.107m
</pre>

<h2>Precedence</h2>

<p>The equations are processed left to right, which breaks traditional operator
precedence. This is regarded as a bug and will be fixed! Anything within
brackets [] is processed first, so this can be used to work around the bad
behaviour.</p>

<pre>
= 1+2^.5
1+2^.5 = 1.73205 (incorrect)

= 1+[2^.5]
1+[2^.5] = 2.41421 (correct)
</pre>

<h2>To Do</h2>

<ul>
<li>Fix precedence.</li>
<li>Add support for e.g. 3e-3 style numbers.</li>
<li>Function support sin[2]</li>
</ul>


<h2>Licence</h2>

<p>This code is licensed under the GPL licence. See the COPYING file in the download for more
information.</p>


<h2>Contact</h2>

<p>ralcalc was written by Roger Light to fulfill his own needs. He knows that
there are other languages that are better suited to this kind of thing, but he
isn't really bothered about that.</p>

<p>Questions/suggestions should be directed to roger_atchoo_org. Replace the _ with @ and . respectively.</p>

</div>
</body></html> 
