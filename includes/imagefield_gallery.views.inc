<?php

/**
 * Preparing to build custom view styles, this is not yet active, and doesn't do much... use grid style.
 */

function imagefield_gallery_views_plugins() {
  $path = drupal_get_path('module', 'imagefield_gallery');
  $views_path = drupal_get_path('module', 'views');
  require_once "./$path/theme/theme.inc";

  $data = array(
    'module' => 'imagefield_gallery', // This just tells our themes are elsewhere.
    'display' => array(
      // Parents are not really displays, just needed so the files can
      // be included.
      'parent' => array(
        'no ui' => TRUE,
        'handler' => 'views_plugin_display',
        'path' => "$views_path/plugins",
        'parent' => '',
      ),
      'page' => array(
        'no ui' => TRUE,
        'handler' => 'views_plugin_display_page',
        'path' => "$views_path/plugins",
        'parent' => 'parent',
      ),
    ),
    'style' => array(
      'parent' => array(
        // this isn't really a display but is necessary so the file can
        // be included.
        'no ui' => TRUE,
        'handler' => 'views_plugin_style',
        'path' => "$views_path/plugins",
        'parent' => '',
      ),
      'lightbox_style_style' => array(
        'title' => t('Imagefield Gallery: Grid'),
        'help' => t('A fixed width grid of table columns'),
        'path' => "$path/includes",
        'handler' => 'imagefield_gallery_lightbox_plugin_style',
        'theme' => 'imagefield_gallery_lightbox',
        'theme file' => 'theme.inc',
        'theme path' => "$path/theme",
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
  return $data;
}