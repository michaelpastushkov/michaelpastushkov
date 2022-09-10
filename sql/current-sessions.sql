/* current sessions */
select *, now() - etime from sessions where now() - etime < 180 and bout > 1000 order by source, etime