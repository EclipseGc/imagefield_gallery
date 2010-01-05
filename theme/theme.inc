<?php

function template_preprocess_imagefield_gallery_lightbox(&$vars) {
  drupal_add_css(drupal_get_path('module', 'imagefield_gallery') .'/css/imagefield_gallery_grid.css');
  $view     = $vars['view'];
  $result   = $view->result;
  $options  = $view->style_plugin->options;
  $handler  = $view->style_plugin;
  $vars['col_width'] = floor(100 / $options['columns']);

  $columns  = $options['columns'];

  $rows = array();

  if ($options['alignment'] == 'horizontal') {
    $row = array();
    $row_count = 0;
    foreach ($vars['rows'] as $count => $item) {
      $row[] = $item;
      $row_count++;
      if (($count + 1) % $columns == 0) {
        $rows[] = $row;
        $row = array();
        $row_count = 0;
      }
    }
    if ($row) {
      // Fill up the last line.
      for ($i = 0; $i < ($columns - $row_count); $i++) {
        $row[] = '';
      }
      $rows[] = $row;
    }
  }
  else {
    $num_rows = floor(count($vars['rows']) / $columns);
    // The remainders are the 'odd' columns that are slightly longer.
    $remainders = count($vars['rows']) % $columns;
    $row = 0;
    $col = 0;
    foreach ($vars['rows'] as $count => $item) {
      $rows[$row][$col] = $item;
      $row++;

      if (!$remainders && $row == $num_rows) {
        $row = 0;
        $col++;
      }
      else if ($remainders && $row == $num_rows + 1) {
        $row = 0;
        $col++;
        $remainders--;
      }
    }
    for ($i = 0; $i < count($rows[0]); $i++) {
      // This should be string so that's okay :)
      if (!isset($rows[count($rows) - 1][$i])) {
        $rows[count($rows) - 1][$i] = '';
      }
    }
  }
  $vars['rows'] = $rows;
}