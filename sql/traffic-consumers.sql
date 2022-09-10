/* traffic consumers */
select ip4, port, max(cn), max(source), max(etime), sum(bin) sin, sum(bout) as sout 
	from  sessions group by ip4, port order by sout desc