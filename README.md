free
====

Display FreeBSD Memory Information

#### Usage
<pre>
usage: free [-b|-k|-m|-g] [-t] [-v]
  -b,-k,-m,-g show output in bytes, KB, MB, or GB
  -t display logical summary for RAM
  -v display version information and exit
</pre>
Example
<pre>
[root@besatie free]# ./free -m -t
                   total          active            free        inactive            wire          cached
Memory:             1917              59            1053             575             111              41
Summary:            1917             246            1670
</pre>
