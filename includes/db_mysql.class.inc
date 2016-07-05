<?php

/**
 * @file
 * Adapted from Backup & Migrate file 'sources.db.mysql.inc' version 3.1
 * Functions to handle the direct to database source.
 */

class backup_migrate_dbmysql {
  private $FileDesc;
  var $connection = NULL;
	
  function backup_migrate_dbmysql() {
    $this->FileDesc;
  }
  function type_name() {
    return t("MySQL Database");
  }

  /**
   * Return a list of backup filetypes.
   */
  function file_types() {
    return array(
      "sql" => array(
        "extension" => "sql",
        "filemime" => "text/x-sql",
        "backup" => TRUE,
        "restore" => TRUE,
      ),
      "mysql" => array(
        "extension" => "mysql",
        "filemime" => "text/x-sql",
        "backup" => TRUE,
        "restore" => TRUE,
      ),
    );
  }

  /**
   * Return the scheme for this db type.
   */
  function default_scheme() {
    return 'mysql';
  }

  /**
   * Get a list of tables in the db.
   */
  function _get_tables_info() {
    $out = array();
    // get auto_increment values and names of all tables
    $result = db_query("SHOW TABLE STATUS", array(), array('fetch' => PDO::FETCH_ASSOC));
    foreach ($result as $table) {
      // Lowercase the keys because between Drupal 7.12 and 7.13/14 the default query behavior was changed.
      // See: http://drupal.org/node/1171866
      $table = array_change_key_case($table);
      if (!empty($table['engine'])) {
        $out[$table['name']] = $table;
      }
    }
    return $out;
  }

  /**
   * Get the sql for the structure of the given table.
   */
  function _get_table_structure_sql($tables_info, $table) {
    $out = "";
    $result = db_query("SHOW CREATE TABLE `". $table ."`", array(), array('fetch' => PDO::FETCH_ASSOC));
    foreach ($result as $create) {
      // Lowercase the keys because between Drupal 7.12 and 7.13/14 the default query behavior was changed.
      // See: http://drupal.org/node/1171866
      $create = array_change_key_case($create);
      // Remove newlines and convert " to ` because PDO seems to convert those for some reason.
      $out .= strtr($create['create table'], array("\n" => ' ', '"' => '`'));
      if ($tables_info[$table]['auto_increment']) {
        $out .= " AUTO_INCREMENT=". $tables_info[$table]['auto_increment'];
      }
      $out .= ";\n";
    }
    $out = str_replace("CREATE TABLE `", "CREATE TABLE IF NOT EXISTS `", $out);
    return $out;
  }

  /**
   *  Get the sql to insert the data for a given table
   */
  function _dump_table_data_sql_to_array($table) {
    $tobe_file = array();
    $rows_per_line = 30;
    $bytes_per_line = 2000;

    $lines = 0;
    $data = db_query("SELECT * FROM `". $table ."`", array(), array('fetch' => PDO::FETCH_ASSOC));
    $rows = $bytes = 0;

    // Escape backslashes, PHP code, special chars
    $search = array('\\', "'", "\x00", "\x0a", "\x0d", "\x1a");
    $replace = array('\\\\', "''", '\0', '\n', '\r', '\Z');
  
    $line = array();
    foreach ($data as $row) {
      // DB Escape the values.
      $items = array();
      foreach ($row as $key => $value) {
        $items[] = is_null($value) ? "null" : "'". str_replace($search, $replace, $value) ."'";
      }

      // If there is a row to be added.
      if ($items) {
        // Start a new line if we need to.
        if ($rows == 0) {
          $tobe_line = "INSERT INTO `". $table ."` VALUES ";
          $bytes = $rows = 0;
        }
        // Otherwise add a comma to end the previous entry.
        else {
          $tobe_line .= ",";
        }

        // Write the data itself.
        $sql = implode(',', $items);
        $tobe_line .= '('. $sql .')';
        $bytes += strlen($sql);
        $rows++;

        // Finish the last line if we've added enough items
        if ($rows >= $rows_per_line || $bytes >= $bytes_per_line) {
          $tobe_line .= ";\n";
          $tobe_file[] = $tobe_line;
          $lines++;
          $bytes = $rows = 0;
        }
      }
    }
    // Finish any unfinished insert statements.
    if ($rows > 0) {
      $tobe_line .= ";\n";
      $tobe_file[] = $tobe_line;
      $lines++;
    }

    return $tobe_file;
  }

  /**
   * The header for the top of the sql dump file. These commands set the connection
   *  character encoding to help prevent encoding conversion issues.
   */
  function _get_sql_file_header() {
    $info['version'] = '5.5.49-0ubuntu0.12.04.1'; //$info = $this->_db_info();

    return "-- Content exPimp ". MODULE_VERSION ." MySQL Dump
-- for Drupal " . VERSION . " (http://www.drupal.org)
-- Host: " . url('', array('absolute' => TRUE)) . "
-- Generation Time: " . format_date(time(), 'custom', 'r') . "
-- MySQL Version: " . $info['version'] . "

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE=NO_AUTO_VALUE_ON_ZERO */;

SET SQL_MODE=\"NO_AUTO_VALUE_ON_ZERO\";
SET NAMES utf8;

";
  }
  
  /**
   * The footer of the sql dump file.
   */
  function _get_sql_file_footer() {
    return "
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
";
  }

  /**
   * Get the version info for the given DB.
   */
  function _db_info() {
    $db = $this->_get_db_connection();
    return array(
      'type' => 'mysql',
      'version' => $db ? $db->version() : t('Unknown'),
    );
  }

  /**
   * Get the db connection for the specified db.
   */
  function _get_db_connection() {
    if (!$this->connection) {
      $this->connection = _create_db_connection();
      // Set the sql mode because the default is ANSI,TRADITIONAL which is not aware of collation or storage engine.
      $this->connection->exec("SET sql_mode=''");
    }
    return $this->connection;
  }

  /**
   * Get the db connection for the specified db.
   */
  function _create_db_connection() {
    if (!$this->connection) {
      $target = $key = '';
      $parts = explode(':', $this->get_id());
      // One of the predefined databases (set in settings.php)
      if ($parts[0] == 'db') {
        $key    = empty($parts[1]) ? 'default' : $parts[1];
        $target = empty($parts[2]) ? 'default' : $parts[2];
      }
      // Another db url.
      else {
        // If the url is specified build it into a connection info array.
        if (!empty($this->dest_url)) {
          $info = array(
            'driver'    => empty($this->dest_url['scheme'])   ? NULL : $this->dest_url['scheme'],
            'host'      => empty($this->dest_url['host'])     ? NULL : $this->dest_url['host'],
            'port'      => empty($this->dest_url['port'])     ? NULL : $this->dest_url['port'],
            'username'  => empty($this->dest_url['user'])     ? NULL : $this->dest_url['user'],
            'password'  => empty($this->dest_url['pass'])     ? NULL : $this->dest_url['pass'],
            'database'  => empty($this->dest_url['path'])     ? NULL : $this->dest_url['path'], 
          );
          $key    = uniqid('backup_migrate_tmp_');
          $target = 'default';
          Database::addConnectionInfo($key, $target, $info);
        }
        // No database selected. Assume the default.
        else {
          $key = $target = 'default';
        }
      }
      if ($target && $key) {
        $this->connection = Database::getConnection($target, $key);
      }
    }
    return $this->connection;
  }
  
}
