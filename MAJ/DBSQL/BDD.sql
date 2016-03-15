
CREATE TABLE IF NOT EXISTS `characters` (
  `guid` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Global Unique Identifier',
  `account` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Account Identifier',
  `name` varchar(12) NOT NULL DEFAULT '',
  `race` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `classe` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `map` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Map Identifier',
  `data` longtext,
  PRIMARY KEY (`guid`),
  UNIQUE KEY `guid` (`guid`),
  KEY `guid_2` (`guid`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=0 ;


CREATE TABLE IF NOT EXISTS `comptes` (
  `id` int(99) NOT NULL,
  `pseudo` varchar(32) NOT NULL,
  `mdp` varchar(40) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS `creature` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Global Unique Identifier',
  `map` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Map Identifier',
  `modelid` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `nom` varchar(100) NOT NULL DEFAULT '0',
  `position_x` int(10) unsigned NOT NULL DEFAULT '0',
  `position_y` int(10) unsigned NOT NULL DEFAULT '0',
  `spawntimesecs` int(10) unsigned NOT NULL DEFAULT '120',
  `hp` int(10) unsigned NOT NULL DEFAULT '1',
  `mindmg` float NOT NULL DEFAULT '0',
  `maxdmg` float NOT NULL DEFAULT '0',
  `DeathState` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `idx_map` (`map`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Creature System' AUTO_INCREMENT=0 ;

CREATE TABLE IF NOT EXISTS `creature_maj` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Global Unique Identifier',
  `position_x` int(10) unsigned NOT NULL DEFAULT '0',
  `position_y` int(10) unsigned NOT NULL DEFAULT '0',
  `hp` int(10) unsigned NOT NULL DEFAULT '1',
  `DeathState` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `compteurMonstreBouge` int(100) NOT NULL DEFAULT '100',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Creature System' AUTO_INCREMENT=0 ;