<?php

/**
 * Helper for processing microformats.
 *
 * Compares the info array of properties for a microformat, including sub-
 * properties and nested microformats, and removes unused markup.
 */
function _microformats_filter($qp, $info, $data) {
  foreach (element_children($info) as $property) {
    $sub = isset($info[$property]['#format']) ? microformats_info($info[$property]['#format']) : $info[$property];
    $children = element_children($sub);
    if (!empty($children)) {
      _microformats_filter($qp->branch('.'. $property), $sub, $data[$property]);
    }
    if (empty($data[$property])) {
      if ($qp->branch('.'. $property)->size()) {
        $qp->branch()->find('.'. $property)->remove();
      }
    }
  }
}

/**
 * hCard
 *
 * @see http://microformats.org/wiki/hcard
 */
function template_preprocess_hcard(&$vars) {
  if ($adr = $vars['data']['adr']) {
    $vars['data']['adr'] = microformats_multivalue_nested('adr', $adr);
  }
}

/**
 * Figure
 *
 * $image must be HTML string with 'image' class attribute
 *
 * @see http://microformats.org/wiki/figure
 */
function template_preprocess_figure(&$vars) {
}

/**
 * adr
 *
 * @see http://microformats.org/wiki/adr
 */
function template_preprocess_adr(&$vars) {
}

function template_preprocess_hentry(&$vars) {
}

function microformats_multivalue_nested($type, $items) {
  $output = '';
  if (!empty($items)) {
    if (is_string($items)) {
      $output .= $items;
    }
    else if (is_array($items)) {
      // if the array is associative
      if (array_values($items) !== $items) {
        $items = array($items);
      }
      foreach ($items as $item) {
        $output .= theme($type, $item);
      }
    }
  }
  return $output;
}

/**
 * Implementation of hook_microformat_hcard_alter().
 */
function microformats_microformat_hcard_alter($qp, $args) {
  $info = microformats_info('hcard');
  $data = $args[0];
  _microformats_filter($qp, $info, $data);

  if (empty($data['type'])) {
    if (!empty($data['org']) && $data['fn'] === $data['org']) {
      $data['type'] =  'organization';
    }
    else {
      $data['type'] =  'person';
    }
  }

  if (!$qp->branch('.fn')->size()) {
    if ($data['type'] == 'organization') {
      $selector = '.org';
    }
    else {
      $selector = '.n';
    }
    $qp->branch($selector)->addClass('fn');
  }

  // Unwrap nested adr microformats
  if ($qp->branch('.adr')->children('.adr')->size()) {
    $qp->branch('.adr')->replaceWith($qp->branch('.adr')->children());
  }
}

/**
 * Implementation of hook_microformat_figure_alter().
 */
function microformats_microformat_figure_alter($qp, $args) {
  $info = microformats_info('figure');
  $data = $args[0];
  _microformats_filter($qp, $info, $data);

  if ($qp->branch('.credit')->children('.vcard')->size()) {
    $qp->branch('.credit')->replaceWith($qp->branch('.credit')->children()->addClass('credit'));
  }
}

/**
 * Implementation of hook_microformat_hcard_alter().
 */
function microformats_microformat_hentry_alter($qp, $args) {
  $qp->find('.vcard')->addClass('author');
}

/**
 * theme_hdatetime().
 */
function theme_hdatetime($date, $type = 'medium', $format = '', $langcode = NULL, $attributes = array()) {

  $attributes['title'] = date_format_date($date, 'custom', 'c');

  $array = array();
  $array[] = array(
    'key' => 'abbr',
    'attributes' => $attributes,
    'value' => date_format_date($date, $type, $format, $langcode),
  );
  return format_xml_elements($array);
}
