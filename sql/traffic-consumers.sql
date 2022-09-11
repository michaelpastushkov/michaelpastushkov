/* traffic consumers */
select ip4, port, max(geo), max(cn), max(source), max(etime),
    format(sum(bin), 0) as sin, format(sum(bout), 0) as sout, sum(bout) bo
    from  sessions group by ip4, port order by bo desc;
    
select source,
    format(sum(bin), 0) as sin, format(sum(bout), 0) as sout
    from  sessions group by source;
