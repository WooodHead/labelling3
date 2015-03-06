# labelling3

## 属性表```
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

## 添加数据库驱动
将sqldrivers.rar解压后的文件放到Qt的安装目录\plugins\sqldrivers目录下。
## 安装mysql数据库
步骤参考网络
## 导入现有数据信息
```
命令行登陆mysql
mysql -u username -p
输入密码
create database bookdata;
use bookdata
source bookdata.sql;
```
## 关联文件
解压abrasivemarksystem.rar，复制内容到F盘

（文件通过邮件转发给你）
