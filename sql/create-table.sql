CREATE TABLE `sessions` (
  `id` int NOT NULL AUTO_INCREMENT,
  `cn` varchar(128) NOT NULL,
  `bin` int unsigned NOT NULL,
  `bout` int unsigned NOT NULL,
  `stime` datetime NOT NULL,
  `etime` datetime DEFAULT NULL,
  `ip4` varchar(45) DEFAULT NULL,
  `port` int DEFAULT NULL,
  `geo` varchar(45) DEFAULT NULL,
  `source` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2816 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
