CREATE TABLE IF NOT EXISTS `j_category` (
  `tid` int(11) NOT NULL AUTO_INCREMENT,
  `c_name` text NOT NULL,
  `weight` int(11) NOT NULL DEFAULT '0',
  `status` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`tid`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=0 ;


CREATE TABLE IF NOT EXISTS `j_price` (
  `pid` int(11) NOT NULL AUTO_INCREMENT,
  `name` text NOT NULL,
  `cost` varchar(255) NOT NULL,
  `tid` int(11) NOT NULL,
  `vid` int(11) NOT NULL,
  `weight` int(11) NOT NULL DEFAULT '0',
  `status` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`pid`),
  UNIQUE KEY `pid` (`pid`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=0 ;

CREATE TABLE IF NOT EXISTS `j_val` (
  `vid` int(11) NOT NULL AUTO_INCREMENT,
  `val` varchar(255) NOT NULL,
  PRIMARY KEY (`vid`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=0 ;