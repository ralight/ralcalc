<?php

require("/home/oojah/includes/main/logging.php");
dolog();
print("<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n");

$version = "1.2.4";
$builddate = "20081114";

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "DTD/xhtml1-strict.dtd">
<html><head><title>ralcalc</title>
<link rel="stylesheet" href="/tools/style.css" type="text/css" />
</head><body>

<div>
<?php

print("<h1>ralcalc version $version</h1>\n");

?>
<h2>What is ralcalc?</h2>
<p>ralcalc is a simple command line calculator designed to take a single line input and produce a result. I also have it installed as the "=" command for even quicker access.</p>

<p>For example:</p>

<pre>
= 2*9
2*9 = 18
</pre>

<p>Both the input and output make use of SI prefixes (see <a href="http://en.wikipedia.org/wiki/SI_prefix">http://en.wikipedia.org/wiki/SI_prefix</a>) for easier input and reading of results. Only the common engineering prefixes that are powers a multiple of three are used, so G, M, k, m, u, n, etc.</p>
<p>Numbers of the style 1e-3 are also supported for input.</p>

<p>Version information can be shown with:</p>

<pre>
= -h
ralcalc -h
</pre>

<p>See also the <a href="ralcalc.html">man page</a>.</p>

<h2>Downloads</h2>
<ul>
<?php

print("<li><a href=\"files/ralcalc-$version.tar.bz2\">ralcalc-$version.tar.bz2</a> (11kB) (<a href=\"files/ralcalc-$version.tar.bz2.asc\">GPG Signature</a>)</li>\n");

?>
<li><a href="files/">Old versions</a></li>
</ul>

<p>The source is also available as a <a href="http://www.selenic.com/mercurial/">Mercurial</a> repository at <a href="http://hg.atchoo.org/ralcalc">http://hg.atchoo.org/ralcalc</a>. To get a copy use "hg clone http://hg.atchoo.org/ralcalc".</p>

<h2>Operators, Functions and Constants</h2>

<ul>
<li>+&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Addition</li>
<li>-&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Subtraction</li>
<li>* x&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Multiplication (the 'x' is for use when shell expansion may be a problem)</li>
<li>/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Division</li>
<li>^&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Power</li>
<li>[]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Brackets - increase the precedence of anything within the brackets.</li>
<li>_&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The result of the last ralcalc run (stored in $HOME/.ralcalc_result)</li>
<li>log&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The base 10 logarithm.</li>
<li>ln&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The natural logarithm.</li>
<li>sin, cos, tan, asin, acos, atan</li>
<li>sqrt</li>
<li>pi (3.14159)</li>
<li>exp (2.71828)</li>
</ul>


<pre>
= 2^.5
2^.5 = 1.41421

= 1/_
1/_ = 707.107m
</pre>

<h2>Precedence</h2>

<p>Anything within brackets [] or () is processed first. Within brackets, the order or precedence is ^, * /, + -.</p>

<h2>Bugs / Requests</h2>
<p>Either contact me directly (see the bottom of the page), or else head to <a href="https://launchpad.net/ralcalc/">launchpad</a> to submit a new bug report or see existing ones.</p>

<h2>Changes</h2>

<p>1.2.4 (20081114)</p>
<ul>
<li>Fix bug whereby the precedence wasn't reset correctly after processing a pair of brackets. This would lead to problems with calculations where a power directly followed a close bracket: [2+3]^3.</li>
<li>Included Brazilian Portugese translations thanks to Ricardo Ichizo.</li>
<li>Included French translations thanks to Djalil Oulmane.</li>
<li>Included Spanish translations thanks to Daniel.</li>
</ul>

<p>1.2.3 (20080916)</p>
<ul>
<li>By request, explicit support for square root (sqrt) has been added.</li>
</ul>

<p>1.2.2 (20080915)</p>
<ul>
<li>Added support for the "-a" option which prints out all of the available operators, functions and constants.</li>
</ul>

<p>1.2.1 (20080718)</p>
<ul>
<li>Fixed a string buffer that was being allocated one byte too small and would occasionally lead to spurious "invalid operator" errors.</li>
</ul>

<p>1.2.0 (20080609)</p>
<ul>
<li>Support for sin, cos, tan, asin, acos, atan functions.</li>
<li>Support for pi and exp constants.</li>
<li>Inclusion of Hebrew translations thanks to Yaron.</li>
</ul>

<p>1.1.0 (20080601)</p>
<ul>
<li>Added the "-f filename" option. This will process each line of "filename" as though it was typed in at the command line.</li>
<li>Added the "-i" option which does the same as "-f" but for stdin.</li>
<li>Added support for log/ln functions.</li>
</ul>

<p>1.0.1 (20080322)</p>
<ul>
<li>Add missing file to the download.</li>
</ul>

<p>1.0 (20080322)</p>
<ul>
<li>Internationalisation and localisation support (want to help translate? Go to <a href="https://translations.launchpad.net/ralcalc">Launchpad</a>).</li>
<li>Build tools improvements.</li>
<li>Change of licence to GPL.</li>
</ul>

<p>20070712</p>
<ul>
<li>Added the "-r" option to cause the result to use neither SI prefixes nor exponents.</li>
</ul>

<p>20070702</p>
<ul>
<li>Added the "-q" option to produce a quiet output.</li>
<li>Added the "-e" option to cause the result to be of the form "1e-3" rather than use SI prefixes.</li>
<li>Fixed the return code on error.</li>
</ul>

<p></p>
<ul>
<li>Fixed negation precedence. No outstanding problems.</li>
</ul>

<p>20070616</p>
<ul>
<li>Precedence is now reset correctly after processing a pair of brackets. Previously, the power in e.g. "= [1+2]^3" was being ignored.</li>
<li>Brackets are matched better, so "= ]1+2[" is detected as being invalid.</li>
</ul>

<p>20070614</p>
<ul>
<li>Fixed Makefiles on Mac OS X.</li>
<li>Included missing "=.1" file in download.</li>
<li>Fixed uninstall target.</li>
</ul>

<p>20070613</p>
<ul>
<li>Added support for 1e3 style numbers.</li>
</ul>

<p>20070610</p>
<ul>
<li>Changed number output format slightly so that we get 0.1 instead of 100m.</li>
<li>Allow () brackets as well as [].</li>
<li>Allow "," as a decimal point, depending on locale.</li>
<li>Fixed operator precedence, number negation excluded.</li>
<li>Added a man page.</li>
</ul>

<!--
<h2>To Do</h2>

<ul>
<li>Fix negation precedence.</li>
</ul>

-->

<h2>Licence</h2>

<p>This code is licensed under GPL licence. See the COPYING file in the download for more
information.</p>


<h2>Contact</h2>

<p>ralcalc was written in C by Roger Light to fulfill his own needs. He knows that
there are other languages that are better suited to this kind of thing, but he
isn't really bothered about that.</p>

<p>Questions/suggestions should be directed to roger_atchoo_org. Replace the _ with @ and . respectively.</p>

</div>
</body></html> 
