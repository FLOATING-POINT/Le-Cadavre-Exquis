/*
SQLyog Community v8.3 
MySQL - 5.1.36-community-log : Database - le-cadavre-exquis
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`le-cadavre-exquis` /*!40100 DEFAULT CHARACTER SET latin1 */;

/*Table structure for table `uploads` */

DROP TABLE IF EXISTS `uploads`;

CREATE TABLE `uploads` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` mediumtext NOT NULL,
  `status` varchar(255) NOT NULL,
  `uploadTime` varchar(255) NOT NULL,
  `vimeoid` varchar(255) NOT NULL,
  `uploadDuration` float NOT NULL,
  `vimeoErrCode` varchar(255) NOT NULL,
  `vimeoErrMsg` mediumtext NOT NULL,
  `localErrMsg` mediumtext NOT NULL,
  `active` enum('0','1') DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=235 DEFAULT CHARSET=latin1;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
