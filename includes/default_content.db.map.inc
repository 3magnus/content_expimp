<?php
$chosen_tables = array(
  'user' => array(
    'all' => array('history', 'users', 'users_roles'),
  ),
  'node' => array(
    'all' => array('node', 'node_access', 'node_counter', 'node_revision'),
  ),
  'term' => array(
    'all' => array('taxonomy_index', 'taxonomy_term_data', 'taxonomy_term_hierarchy'),
  ),
  'locale' => array(
    'all' => array('locales_source', 'locales_target'),
  ),
  'shared' => array(
    'all' => array('file_managed', 'file_usage', 'rdf_mapping', 'url_alias')
  )
);