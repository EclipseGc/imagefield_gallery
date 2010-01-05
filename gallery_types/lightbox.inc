<?php

function imagefield_gallery_lightbox_gallery_options(&$form_state) {
  foreach (imagecache_presets() as $preset) {
    $preset_options[$preset['presetid']] = $preset['presetname'];
  }
  $preset_options['-1'] = t('original');
  $form['lightbox'] = array(
    '#type' => 'fieldset',
    '#title' => t('Lightbox 2 Gallery Settings'),
    '#tree' => TRUE,
    '#input' => TRUE,
    '#prefix' => '<div id="edit-lightbox-wrapper"><div>',
    '#suffix' => '</div></div>',
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('radio:gallery_style' => array('lightbox')),
  );
  $form['lightbox']['thumb_preset'] = array(
    '#type' => 'select',
    '#title' => t('Thumbnail Preset'),
    '#options' => $preset_options,
    '#default_value' => $form_state['page']->thumb_preset,
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('radio:gallery_style' => array('lightbox')),
  );
  $form['lightbox']['preview_preset'] = array(
    '#type' => 'select',
    '#title' => t('Preview Preset'),
    '#options' => $preset_options,
    '#default_value' => $form_state['page']->preview_preset,
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('radio:gallery_style' => array('lightbox')),
  );
  $form['lightbox']['col_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of Columns'),
    '#default_value' => $form_state['page']->col_num,
    '#description' => t('By default the lightbox gallery supplies its own version of a grid view.  Please specify the number of columns for which you would like it formatted.'),
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('radio:gallery_style' => array('lightbox')),
  );
  return $form;
}

function imagefield_gallery_lightbox_gallery_options_validate(&$form, &$form_state) {
  if (!is_numeric($form_state['values']['lightbox']['col_num'])) {
    form_set_error('lightbox][col_num', t('Number of columns must be numeric.'));
  }
}

function imagefield_gallery_lightbox_gallery_options_submit(&$form, &$form_state) {
  $form_state['page']->thumb_preset = $form_state['values']['lightbox']['thumb_preset'];
  $form_state['page']->preview_preset = $form_state['values']['lightbox']['preview_preset'];
  $form_state['page']->col_num = $form_state['values']['lightbox']['col_num'];
}

function imagefield_gallery_lightbox_page_load(&$page) {
  $query = db_query("SELECT thumb_preset, preview_preset, view_conf FROM {imagefield_gallery_lightbox} WHERE task_name = '%s'", $page->task_name);
  while ($result = db_fetch_object($query)) {
    $page->thumb_preset = $result->thumb_preset;
    $page->preview_preset = $result->preview_preset;
    $configurations = unserialize($result->view_conf);
    foreach ($configurations as $key => $config) {
      $page->$key = $config;
    }
  }
}

function imagefield_gallery_lightbox_page_delete($page) {
  db_query("DELETE FROM {imagefield_gallery_lightbox} WHERE task_name = '%s'", $page->task_name);
}

function imagefield_gallery_lightbox_gallery_save($page) {
  $lgid = db_result(db_query("SELECT lgid FROM {imagefield_gallery_lightbox} WHERE task_name = '%s'", $page->task_name));
  $item = new stdClass();
  if ($lgid) {
    $item->lgid = $lgid;
  }
  $item->task_name = $page->task_name;
  $item->thumb_preset = $page->thumb_preset;
  $item->preview_preset = $page->preview_preset;
  $item->view_conf = array('col_num' => $page->col_num);
  if (!$lgid) {
    drupal_write_record('imagefield_gallery_lightbox', $item);
  }
  else {
    drupal_write_record('imagefield_gallery_lightbox', $item, 'lgid');
  }
}

function imagefield_gallery_lightbox_view_alterations(&$view, &$handler, $page) {
  foreach ($page->image_fields as $imagefield) {
    list($type, $field) = explode(':', $imagefield);
    break;
  }
  $presets = imagecache_presets();
  if ($page->preview_preset == '-1') {
    $format = 'imagefield__lightbox2__'. $presets[$page->thumb_preset]['presetname'] .'__original';
  }
  elseif ($page->thumb_preset == '-1') {
    $format = 'imagefield__lightbox2__original__'. $presets[$page->preview_preset]['presetname'];
  }
  else {
    $format = 'imagefield__lightbox2__'. $presets[$page->thumb_preset]['presetname'] .'__'. $presets[$page->preview_preset]['presetname'];
  }
  $handler->override_option('style_plugin', 'lightbox_style_style');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'columns' => $page->col_num,
    'alignment' => 'horizontal',
  ));
  $handler->override_option('fields', array(
    $field .'_fid' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => $format,
      'multiple' => array(
        'group' => 0,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => 0,
      ),
      'exclude' => 0,
      'id' => $field. '_fid',
      'table' => 'node_data_'. $field,
      'field' => $field .'_fid',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Override',
      ),
    ),
  ));
}