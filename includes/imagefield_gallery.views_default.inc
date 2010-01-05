<?php

/**
 * Implementation of hook_views_default_views().
 */
function imagefield_gallery_views_default_views() {
  $pages = imagefield_gallery_get_pages();
  foreach ($pages as $page) {
    $views['imagefield_gallery_'. $page->task_name] = imagefield_gallery_default_view_settings($page);
  }
  return $views;
}

function imagefield_gallery_default_view_settings($page) {
  $page->image_fields = unserialize($page->image_fields);
  $page->view_conf = unserialize($page->view_conf);
  $view = new view;
  $view->name = 'imagefield_gallery_'. $page->task_name;
  $view->description = 'An imagefield gallery generated view.';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  foreach ($page->view_conf as $option => $conf) {
    if ($option != 'views_path') {
      $handler->override_option($option, $conf);
    }
  }
  $galleries = imagefield_gallery_ifg_api();
  $gallery = $galleries[$page->gallery_style];
  module_load_include($gallery['file_type'], $gallery['module'], $gallery['file_name']);
  module_invoke($gallery['module'] .'_'. $page->gallery_style, 'view_alterations', $view, $handler, $page);
  list($module, $hook) = explode(':', $page->gallery_type);
  module_invoke($module, $hook, $view, $handler, $page);
  return $view;
}