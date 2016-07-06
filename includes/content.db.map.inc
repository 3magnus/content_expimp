<?php
global $latest_full_backup;
$latest_full_backup = 'bckp_tecnonet--usersdata-2016-07-04T20-06-06.sql';
// content_tables[] = pathauto_state (pathauto), workflow_node_history (workflow)
global $chosen_tables;
$chosen_tables = array(
  'user' => array(
    'all' => array('field_data_field_birthday', 'field_data_field_fbappscopedid', 'field_data_field_fullname', 'field_data_field_gender', 'field_data_field_nickname', 'field_revision_field_birthday', 'field_revision_field_fbappscopedid', 'field_revision_field_fullname', 'field_revision_field_gender', 'field_revision_field_nickname', 'history', 'users', 'users_roles'),
  ),
  'node' => array(
    'all' => array('node', 'node_access', 'node_counter', 'node_revision', 'pathauto_state', 'workflow_node_history'),
    'address' => array('field_data_field_postal_address', 'field_revision_field_postal_address'),
    'event' => array('field_data_field_event_body', 'field_data_field_event_session', 'field_data_field_event_situation', 'field_data_field_event_type', 'field_data_field_external_link', 'field_data_field_flyer', 'field_data_field_event_linkto', 'field_revision_field_event_body', 'field_revision_field_event_session', 'field_revision_field_event_situation', 'field_revision_field_event_type', 'field_revision_field_external_link', 'field_revision_field_flyer', 'field_revision_field_event_linkto'),
    'ncard' => array('field_data_field_card_categs', 'field_data_field_card_diffs', 'field_data_field_card_image', 'field_data_field_card_text', 'field_data_field_contact_list', 'field_revision_field_card_categs', 'field_revision_field_card_diffs', 'field_revision_field_card_image', 'field_revision_field_card_text', 'field_revision_field_contact_list'),
    'ncardxt' => array('field_data_body', 'field_data_field_banner', 'field_data_field_external_link', 'field_revision_body', 'field_revision_field_banner', 'field_revision_field_external_link'),
    'place' => array('field_data_field_logo', 'field_data_field_place_body', 'field_data_field_place_categs', 'field_data_field_place_diffs', 'field_data_field_place_name', 'field_revision_field_logo', 'field_revision_field_place_body', 'field_revision_field_place_categs', 'field_revision_field_place_diffs', 'field_revision_field_place_name'),
  ),
  'shared' => array(
    'all' => array('field_data_field_datetime', 'field_data_field_geodata', 'field_data_field_location_taxonomize_terms', 'field_data_field_refaddress', 'field_data_field_refplace', 'field_revision_field_datetime', 'field_revision_field_geodata', 'field_revision_field_location_taxonomize_terms', 'field_revision_field_refaddress', 'field_revision_field_refplace', 'file_managed', 'file_usage', 'rdf_mapping', 'url_alias')
  ),
  'taxonomy_term' => array(
    'all' => array('taxonomy_index', 'taxonomy_term_data', 'taxonomy_term_hierarchy'),
    'location_taxonomize' => array('field_data_field_subregions_br', 'field_data_location_taxonomize_longname', 'field_revision_field_subregions_br', 'field_revision_location_taxonomize_longname', ),
    'subregions_br' => array(),
    'place_categories' => array(),
    'place_diffs' => array(),
    'pro_categories' => array(),
    'pro_diffs' => array(),
    'event_types' => array(),
  ),
  'block' => array(
    'all' => array('block', 'block_current_search', 'block_custom', 'block_node_type', 'block_role'),
  ),
  'locale' => array(
    'all' => array('locales_source', 'locales_target'),
  ),
);