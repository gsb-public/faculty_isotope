<?php

$plugin = array(
  'title' => t('Faculty Listing'),
  'category' => t('Custom Content & Features'),
  'single' => TRUE,
  'all contexts' => TRUE,
);

/**
 * Implements hook_PLUGIN_content_type_render().
 */
function faculty_isotope_faculty_list_content_type_render($subtype, $conf, $panel_args, $context, $incoming) {
  $select = db_select('node','n');
  $select->fields('n', array('title', 'nid'));
  $select->condition('n.status', 1);
  $select->condition('n.type', 'faculty');

  // Last Name
  $select->leftJoin('field_data_field_last_name', 'ln', 'n.nid = ln.entity_id AND entity_id = 0');

  // Academic Area
  $select->leftJoin('field_data_field_academic_area_single', 'aas', 'n.nid = aas.entity_id AND aas.deleted = 0');
  $select->leftJoin('taxonomy_term_data', 'ttd_aas', 'aas.field_academic_area_single_tid = ttd_aas.tid');
  $select->addField('ttd_aas', 'tid', 'academic_area_tid');
  $select->addField('ttd_aas', 'name', 'academic_area');

  if (!empty($_GET['filter_academic_area'])) {
    $condition = db_or();
    foreach($_GET['filter_academic_area'] as $value) {

      $term_parts = explode('--', $value);
      $tid = (int) array_pop($term_parts);
      $condition->condition('ttd_aas.tid', $tid);
    }

    $select->condition($condition);
  }

  // Centers
  $select->leftJoin('field_data_field_centers_research_programs','crp', 'n.nid = crp.entity_id AND crp.deleted = 0');
  $select->leftJoin('taxonomy_term_data', 'ttd_crp', 'crp.field_centers_research_programs_tid = ttd_crp.tid');
  $select->addField('ttd_crp', 'tid', 'center_tid');
  $select->addField('ttd_crp', 'name', 'center');

  if (!empty($_GET['filter_center'])) {
    $condition = db_or();
    foreach($_GET['filter_center'] as $value) {

      $term_parts = explode('--', $value);
      $tid = (int) array_pop($term_parts);
      $condition->condition('ttd_crp.tid', $tid);
    }

    $select->condition($condition);
  }

  // Rank
  $select->leftJoin('field_data_field_rank', 'r', 'n.nid = r.entity_id AND r.deleted = 0');
  $select->addField('r', 'field_rank_value', 'rank');

  // Photo
  $select->leftJoin('field_data_field_image_single_public', 'isp', 'n.nid = isp.entity_id AND isp.deleted = 0');
  $select->leftJoin('file_managed', 'fm', 'fm.fid = isp.field_image_single_public_fid');
  $select->addField('fm', 'uri', 'photo');

  // Url Alias
  $select->leftJoin('url_alias', 'ua', "ua.source = CONCAT('node/', n.nid)");
  $select->addField('ua', 'alias', 'url');

  // Search
  if (!empty($_GET['search'])) {
    $select->leftJoin('field_data_field_search_field', 'sf', 'n.nid = sf.entity_id AND sf.deleted = 0');
    $select->condition('sf.field_search_field_value', '%' . $_GET['search'] . '%', 'LIKE');
  }

  $select->orderBy('ln.field_last_name_value', 'ASC');
  $select->orderBy('title','ASC');

  $query = $select->execute();

  $items = '';
  $centers = array();
  $academic_areas = array();

  while ($result = $query->fetchObject()) {
    $rank_area = $result->rank;
    if (!empty($result->academic_area)) {
      $rank_area .= ', ' . $result->academic_area;
    }

    $classes = array();
    $academic_area_class = drupal_html_class($result->academic_area . '--' . $result->academic_area_tid);
    $center_class = drupal_html_class($result->center . '--' . $result->center_tid);

    if (!empty($result->academic_area)) {
      $academic_areas[$academic_area_class] = $result->academic_area;
    }

    if (!empty($result->center)) {
      $centers[$center_class] = $result->center;
    }

    $classes[] = $academic_area_class;
    $classes[] = $center_class;

    $variables = array(
      'title' => $result->title,
      'academic_area' => $result->academic_area,
      'rank' => $result->rank,
      'rank_area' => $rank_area,
      'link' => $result->url,
      'photo' => $result->photo,
      'nid' => $result->nid,
    );

    $parameters = array(
      'data' => theme('faculty_isotope_item', $variables),
      'classes' => $classes,
      'item-id' => $result->nid,
    );

    $items[] = $parameters;
  }

  // Get all academic areas
  $academic_area_query = db_select('taxonomy_term_data', 'ttd');
  $academic_area_query->fields('ttd', array('tid', 'name'));
  $academic_area_query->leftJoin('taxonomy_vocabulary', 'tv', 'tv.vid = ttd.vid');
  $academic_area_query->condition('tv.machine_name', 'academic_area');
  $academic_area_query->orderBy('ttd.name', 'ASC');
  $academic_area_results = $academic_area_query->execute();

  while ($academic_area = $academic_area_results->fetchAssoc()) {
    $key = drupal_html_class($academic_area['name'] . '--' . $academic_area['tid']);
    $academic_areas[$key] = $academic_area['name'];
  }
  asort($academic_areas);

  // Get all centers
  $center_query = db_select('taxonomy_term_data', 'ttd');
  $center_query->fields('ttd', array('tid', 'name'));
  $center_query->leftJoin('taxonomy_vocabulary', 'tv', 'tv.vid = ttd.vid');
  $center_query->condition('tv.machine_name', 'academic_area');
  $center_query->orderBy('ttd.name', 'ASC');
  $center_results = $center_query->execute();

  while ($center = $center_results->fetchAssoc()) {
    $key = drupal_html_class($center['name'] . '--' . $center['tid']);
    $centers[$key] = $center['name'];
  }
  asort($centers);

  $options = array(
    'filters' => array(
      'search' => array(
        'type' => 'search',
        'placeholder' => t('search by name, research interests, or other keywords'),
      ),
      'submit' => array(
        'type' => 'submit',
        'title' => t('Search'),
      ),
      'academic_area' => array(
        'type' => 'checkboxes',
        'title' => t('Academic Area'),
        'options' => $academic_areas,
      ),
      'centers' => array(
        'type' => 'checkboxes',
        'title' => t('Centers'),
        'options' => $centers,
      ),
    ),
    'settings' => array(
      'callback' => '/faculty-search',
      'lazyLoad' => TRUE,
    ),
  );

  $output = isotopify_prepare($items, $options);
  return (object) array(
    'content' => array(
      '#markup' => $output,
    ),
  );
}

