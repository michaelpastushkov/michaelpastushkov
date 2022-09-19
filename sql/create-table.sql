CREATE TABLE `sessions` (
  `id` int NOT NULL AUTO_INCREMENT,
  `cn` varchar(128) NOT NULL,
  `bin` bigint unsigned NOT NULL,
  `bout` bigint unsigned NOT NULL,
  `stime` datetime NOT NULL,
  `etime` datetime DEFAULT NULL,
  `ip4` varchar(45) DEFAULT NULL,
  `port` int DEFAULT NULL,
  `country` varchar(45) DEFAULT NULL,
  `city` varchar(45) DEFAULT NULL,
  `source` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `idx_sessions_cn` (`cn`),
  KEY `idx_sessions_ip4_port` (`ip4`,`port`),
  KEY `idx_sessions_stime` (`stime`),
  KEY `idx_sessions_etime` (`etime`)
) ENGINE=InnoDB AUTO_INCREMENT=539179 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
