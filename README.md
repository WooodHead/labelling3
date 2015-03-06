# labelling3

## ¿¿¿¿¿¿¿¿¿¿¿
```
CREATE TABLE `propertyinfo` (
  `uid` int(10) NOT NULL DEFAULT '0',
  `propertyname` varchar(30) DEFAULT NULL,
  `eqmfields` varchar(200) DEFAULT NULL,
  `eqmvalues` varchar(200) DEFAULT NULL,
  `mpfields` varchar(200) DEFAULT NULL,
  `mpvalues` varchar(200) DEFAULT NULL,
  `mprfields` varchar(200) DEFAULT NULL,
  `mprvalues` varchar(200) DEFAULT NULL,
  `fegfields` varchar(200) DEFAULT NULL,
  `fegvalues` varchar(200) DEFAULT NULL,
  `fegpfields` varchar(200) DEFAULT NULL,
  `fegpvalues` varchar(200) DEFAULT NULL,
  `oisfields` varchar(200) DEFAULT NULL,
  `oisvalues` varchar(200) DEFAULT NULL,
  `oiafields` varchar(200) DEFAULT NULL,
  `oiavalues` varchar(200) DEFAULT NULL,
  `abmfields` varchar(200) DEFAULT NULL,
  `abmvalues` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
```

## Ìí¼ÓÊı¾İ¿âÇı¶¯
½«sqldrivers.rar½âÑ¹ºóµÄÎÄ¼ş·Åµ½QtµÄ°²×°Ä¿Â¼\plugins\sqldriversÄ¿Â¼ÏÂ¡£
## °²×°mysqlÊı¾İ¿â
²½Öè²Î¿¼ÍøÂç
## µ¼ÈëÏÖÓĞÊı¾İĞÅÏ¢
```
ÃüÁîĞĞµÇÂ½mysql
mysql -u username -p
ÊäÈëÃÜÂë
create database bookdata;
use bookdata
source bookdata.sql;
```
## ¹ØÁªÎÄ¼ş
½âÑ¹abrasivemarksystem.rar£¬¸´ÖÆÄÚÈİµ½FÅÌ

£¨ÎÄ¼şÍ¨¹ıÓÊ¼ş×ª·¢¸øÄã£©
