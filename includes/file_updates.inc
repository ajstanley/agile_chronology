<?php

function update_chronology_files_form($form, $form_state, $starting_year = 1866) {
  module_load_include('inc', 'agile_chronology', 'includes/utilities');
  $selected_year = isset($form_state['values']['year_choice']) ? $form_state['values']['year_choice'] : $starting_year;
  $content_prompt = t('Enter html for @year', array('@year' => $selected_year));
  $content = agile_chronology_get_content_by_year($selected_year);
  $all_years = buid_year_selector();
  return array(
    'year_choice' => array(
      '#title' => t('Select Component'),
      '#type' => 'select',
      '#options' => $all_years,
      '#default_value' => $selected_year,
      '#ajax' => array(
        'callback' => 'update_chronology_files_form_ajax_callback',
        'wrapper' => 'content-wrapper',
        'effect' => 'fade',
        'event' => 'change',
        'method' => 'replace',
      ),
    ),
    'recordBody' => array(
      '#title' => $content_prompt,
      '#prefix' => '<div id="content-wrapper">',
      '#suffix' => '</div>',
      '#type' => 'text_format',
      '#base_type' => 'textarea',
      '#rows' => 30,
      '#value' => $content,
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Create / Update'),
    ),
  );
}

function update_chronology_files_form_ajax_callback(array $form, array $form_state) {
  return $form['recordBody'];
}

function update_chronology_files_form_submit($form, $form_state) {
  module_load_include('inc', 'agile_chronology', 'includes/utilities');
  $recordDate = $form_state['values']['year_choice'];
  $recordBody = $form_state['input']['recordBody']['value'];
  agile_chronology_upsert($recordDate, $recordBody);
  drupal_goto("/admin/config/content/chronology/$recordDate");
}
