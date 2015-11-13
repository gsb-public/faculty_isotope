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
  $query = "SELECT DISTINCT ttd.name AS academic_area, ttd2.name AS center, r.field_rank_value AS rank, fm.uri as photo, n.title AS title, ua.alias as url, n.nid ";
  $query .= "FROM {node} as n ";
  $query .= "LEFT JOIN {field_data_field_last_name} as ln on n.nid = ln.entity_id AND ln.deleted = '0'";
  $query .= "LEFT JOIN {field_data_field_academic_area_single} as aas on n.nid = aas.entity_id AND aas.deleted = '0' ";
  $query .= "LEFT JOIN {taxonomy_term_data} as ttd ON aas.field_academic_area_single_tid = ttd.tid ";
  $query .= "LEFT JOIN {field_data_field_centers_research_programs} as crp on n.nid = crp.entity_id AND crp.deleted = '0' ";
  $query .= "LEFT JOIN {taxonomy_term_data} as ttd2 ON crp.field_centers_research_programs_tid = ttd2.tid ";
  $query .= "LEFT JOIN {field_data_field_rank} as r on n.nid = r.entity_id AND r.deleted = '0' ";
  $query .= "LEFT JOIN {field_data_field_image_single_public} as isp on n.nid = isp.entity_id AND isp.deleted = '0' ";
  $query .= "LEFT JOIN {file_managed} as fm ON fm.fid = isp.field_image_single_public_fid ";
  $query .= "LEFT JOIN {url_alias} as ua on ua.source = CONCAT('node/', n.nid) ";
  $query .= "WHERE n.status = '1' AND n.type = 'faculty' ";
  $query .= "ORDER BY ln.field_last_name_value ASC, title ASC";

  $results = db_query($query);

  $items = '';
  $centers = array();
  $academic_areas = array();

  foreach ($results as $result) {

    $rank_area = $result->rank;
    if (!empty($result->academic_area)) {
      $rank_area .= ', ' . $result->academic_area;
    }

    $classes = array();
    $academic_area_class = drupal_html_class($result->academic_area);
    $center_class = drupal_html_class($result->center);

    if (!empty($academic_area_class)) {
      $academic_areas[$academic_area_class] = $result->academic_area;
    }

    if (!empty($center_class)) {
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
      'class' => implode(' ', $classes),
      'nid' => $result->nid,
    );

    $items .= theme('faculty_isotope_item', $variables);
  }

  $output = theme('faculty_isotope_wrapper', array('items' => $items, 'academic_areas' => $academic_areas, 'centers' => $centers));

  drupal_add_js('https://cdnjs.cloudflare.com/ajax/libs/jquery.isotope/2.2.2/isotope.pkgd.js', 'external');
  drupal_add_js('https://cdnjs.cloudflare.com/ajax/libs/jquery.lazyload/1.9.1/jquery.lazyload.min.js', 'external');
  drupal_add_js(drupal_get_path('module', 'faculty_isotope') . '/js/faculty-isotope.js');

  return (object) array(
    'content' => array(
      '#markup' => $output,
    ),
  );
}

