# labelling3

## ���Ա�```
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

## ������ݿ�����
��sqldrivers.rar��ѹ����ļ��ŵ�Qt�İ�װĿ¼\plugins\sqldriversĿ¼�¡�
## ��װmysql���ݿ�
����ο�����
## ��������������Ϣ
```
�����е�½mysql
mysql -u username -p
��������
create database bookdata;
use bookdata
source bookdata.sql;
```
## �����ļ�
��ѹabrasivemarksystem.rar���������ݵ�F��

���ļ�ͨ���ʼ�ת�����㣩
