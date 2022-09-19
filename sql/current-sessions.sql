/* current sessions */
SELECT * FROM sessions WHERE etime >= DATE_SUB(NOW(), INTERVAL 3 MINUTE) AND bout > 1000 ORDER BY source, etime;
