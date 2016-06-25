<?php

/**
 * @file
 * Functions to handle the direct to database source.
 */

class content_expimp_source_db_mysql {
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
   * Delete all database tables.
   *
   * @global array $databases
   */
  function get_database_tables() {
    global $databases;
    $tables = db_find_tables($databases['default']['default']['prefix'] . '%');
    foreach ($tables as $table) {
      db_drop_table($table);
    }
  }

  /**
   * Get the file type for to backup this source to.
   */
  function get_file_type_id() {
    return 'mysql';
  }

}

