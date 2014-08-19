<?php

  /**
   * @file
   * Theming specific to blocks
   *
   */

  /**
   * Islandora Solr advanced search block form
   * @author griffinj
   *
   * @see islandora_solr_advanced_search_form($form, &$form_state)
   *
   * @global type $_islandora_solr_queryclass
   *   the IslandoraSolrQueryProcessor object which includes the current query
   *   settings and the raw Solr results.
   * @param type $form
   * @param array $form_state
   * @return string
   */

function bootstrap_dss_lebanesetown_islandora_solr_advanced_search_form($form, &$form_state) {
  drupal_add_library('system', 'ui.tabs');

  global $_islandora_solr_queryclass;

  // include common.inc
  module_load_include('inc', 'islandora_solr', 'includes/common');

  // check form states
  // 1. form update using ajax
  // 2. populate with current query on search results page
  // 3. anywhere else: empty form

  // 1. form update using ajax
  if (isset($form_state['triggering_element'])) {

    // unset terms from input array
    // http://drupal.stackexchange.com/questions/14855/how-do-i-dynamically-fill-a-textfield-with-ajax/16576#16576
    unset($form_state['input']['terms']);

    /**
     * @author griffinj@lafayette.edu
     *
     */
    // check for add
    if ($form_state['triggering_element']['#value'] == 'Add a field') {
      $form_state['values']['terms'][] = array();
    }
    // check for remove
    elseif ($form_state['triggering_element']['#value'] == 'Remove this field') {
      $field = $form_state['triggering_element']['#field'];
      array_splice($form_state['values']['terms'], $field, 1);
    }
    // set values
    $values = $form_state['values'];
  } elseif (islandora_solr_results_page($_islandora_solr_queryclass) == TRUE && !isset($_islandora_solr_queryclass->internalSolrParams['type'])) {

    // 2. populate with current query on search results page

    // get current query
    $query = $_islandora_solr_queryclass->solrQuery;
    // set value variable
    $values['terms'] = array();

    // explode on space
    $query_explode = explode(' ', $query);

    // break up the solr query to populate the advanced search form
    $i = 0;
    foreach ($query_explode as $key => $value) {
      $term = array();

      // check for first colon to split the string
      if (strpos($value, ':') != FALSE) {
        // split the filter into field and value
        $value_split = explode(':', $value, 2);
        // set field
        $values['terms'][$i]['field'] = $value_split[0];

        // second part of the split is the query value (or first part of it)
        $value_split[1] = str_replace(array('(', ')'), '', $value_split[1]);
        // add search string
        $values['terms'][$i]['search'] = $value_split[1];

      }
      // if the string does not include a colon or AND/OR/NOT, then it is a
      // part of the query value
      elseif (!preg_match('/(AND|OR|NOT)/', $value, $matches)) {
        // trim brackets
        $value = str_replace(array('(', ')'), '', $value);

        if (isset($values['terms'][$i]['search'])) {
          // append to search string
          $values['terms'][$i]['search'] .= ' ' . $value;
        }
        else {
          // search field is not set, so create new search value
          $values['terms'][$i]['search'] = $value;
        }
      }
      // if it matches AND/OR/NOT, then we have the boolean operator
      else {
        // set boolean operator
        $values['terms'][$i]['boolean'] = $value;

        // increment to next field
        $i++;
      }
    }
  }
  // 3. anywhere else: empty form
  else {
    //Need at least one term to draw the search box.
    $values = array(
      'terms' => array(''),
    );
  }

  $terms = array(
    '#type' => 'markup',
    '#prefix' => '<div id="islandora-solr-advanced-terms">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );

  // loop over each term
  foreach ($values['terms'] as $i => $value) {

    $term = array('#tree' => TRUE,
		  '#prefix' => '<div>',
		  '#suffix' => '</div>');

    $term['field'] = array(
      '#title' => t('Field'),
      '#type' => 'select',
      '#default_value' => isset($value['field']) ? $value['field'] : 'dc.title',
      '#options' => islandora_solr_get_fields('search_fields'),
      '#attributes' => array('class' => array('islandora-solr-search-field')));

    /*
    <div class="input-group">
      <div class="input-group-btn">
        <button type="button" class="btn btn-default dropdown-toggle" data-toggle="dropdown">Action <span class="caret"></span></button>
        <ul class="dropdown-menu">
          <li><a href="#">Action</a></li>
          <li><a href="#">Another action</a></li>
          <li><a href="#">Something else here</a></li>
          <li class="divider"></li>
          <li><a href="#">Separated link</a></li>
        </ul>
      </div><!-- /btn-group -->
      <input type="text" class="form-control">
    </div><!-- /input-group -->
     */

    $term['search'] = array(
			    //'#title' => t('Search Terms'),
			    '#type' => 'textfield',
			    '#size' => 20,
			    '#default_value' => isset($value['search']) ? $value['search'] : 'Search...',
			    /*
			      '#prefix' => '<div class="input-group"><div class="input-group-btn"><button type="button" class="btn btn-default dropdown-toggle" data-toggle="dropdown">Action <span class="caret"></span></button>
			      <ul class="dropdown-menu">
			      <li><a href="#">Action</a></li>
			      <li><a href="#">Another action</a></li>
			      <li><a href="#">Something else here</a></li>
			      <li class="divider"></li>
			      <li><a href="#">Separated link</a></li>
			      </ul></div><!-- /btn-group -->',
			      '#suffix' => '</div><!-- /input-group -->'
			    */
			    );

    $term['hidden_submit'] = array(// Used for when the user presses enter on the search field.
      '#type' => 'submit',
      '#value' => t('Search'),
      '#attributes' => array('style' => 'visibility:hidden;position:fixed;top:-1000px;right:-1000px;')
    );

    /**
     * @author griffinj@lafayette.edu
     * Refactor into a theme
     */

    // 'class' => array('islandora-solr-search-add'))
    /*
>       //'#attributes' => array('title' => t('Add field')),
>       //'#attributes' => array('title' => t('+')),
>       '#attributes' => array('title' => t('Search by'),
>                            'class' => array('islandora-solr-search-add')),

     */

    if(count($values['terms']) < 5) {

      $term['add'] = array(
			   '#type' => 'button',
			   '#value' => 'Add a field',
			   '#attributes' => array('title' => t('Add field'),
						  'class' => array('islandora-solr-search-add')),
			   '#name' => 'add-field-' . $i,
			   '#ajax' => array(
					    'callback' => '_islandora_solr_advanced_search_terms',
					    'wrapper' => 'islandora-solr-advanced-terms',
					    'method' => 'replace',
					    'effect' => 'fade',
					    'progress' => array('type' => 'none'),
					    ),
			   );
    }
    if (count($values['terms']) > 1) {
      $term['remove'] = array(
        '#type' => 'button',
        '#field' => $i,
        '#value' => 'Remove this field',
        '#attributes' => array('title' => t('Remove field'),
			       'class' => array('islandora-solr-search-remove')),
        '#name' => 'remove-field-' . $i,
        '#ajax' => array(
          'callback' => '_islandora_solr_advanced_search_terms',
          'wrapper' => 'islandora-solr-advanced-terms',
          'method' => 'replace',
          'effect' => 'fade',
          'progress' => array('type' => 'none'),
        )
      );

      if ((variable_get('islandora_solr_search_boolean', 'user') == 'user') && ((count($values['terms']) -1) != $i)) {
        $term['boolean'] = array(
          '#type' => 'select',
          '#prefix' => '<div class="islandora-solr-advanced-boolean">',
          '#suffix' => '</div>',
          '#default_value' => isset($value['boolean']) ? $value['boolean'] : 'AND',
          '#options' => array(
            'AND' => 'AND',
            'OR' => 'OR',
            'NOT' => 'NOT'),
	  '#attributes' => array('class' => array('islandora-solr-search-boolean'))
        );
      }
    }
    $terms[] = $term;
  }

  /**
   * @author griffinj
   *
   */
  /*
  $terms['terms']['field-controls']['add'] = $term['add'];
  hide($term['add']);

  if(array_key_exists('remove', $terms)) {

    $terms['terms']['field-controls']['remove'] = $term['remove'];
    hide($term['remove']);
  }
  */

  // set form
  $form = array();
  // add terms
  $form['terms'] = $terms;

  // add controls
  $form['controls'] = array('#type' => 'markup',
			    '#prefix' => '<div class="islandora-solr-advanced-controls">',
			    '#suffix' => '</div>');

  $form['controls']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search')
  );
  return $form;
}

/**
 * @author griffinj
 * Implements hook_block_view_MODULE_DELTA_alter(&$data, $block).
 *
 */
function bootstrap_dss_lebanesetown_block_view_islandora_solr_advanced_alter(&$data, $block) {

  foreach($data['content']['terms'] as $key => &$value) {

    if(is_numeric($key)) {

      $value['add']['#value'] = '<i class="icon-large"></i>';
      $value['add']['#ajax']['callback'] = '_bootstrap_dss_lebanesetown_islandora_solr_advanced_search_terms';

      $value['search']['#value'] = '';
    }
  }
}

/**
 * 
 * @author griffinj
 * sharethisGet_button_HTML is the function that creates the ShareThis code
 * It returns the appropriate html based on your settings.
 * @see sharethis_get_button_HTML()
 *
 */
function _bootstrap_dss_lebanesetown_sharethis_get_button_HTML($data_options, $mPath, $mTitle) {

  // Inject the extra services.
  foreach ($data_options['option_extras'] as $service) {

    $data_options['services'] .= ',"' . $service . '"';
  }

  // The share buttons are simply spans of the form class='st_SERVICE_BUTTONTYPE' -- "st" stands for ShareThis.
  $type = substr($data_options['buttons'], 4);
  $type = $type == "_" ? "" : check_plain($type);
  $service_array = explode(",", $data_options['services']);
  $st_spans = "";
  foreach ($service_array as $service_full) {
    // Strip the quotes from the element in the array (They are there for javascript)
    $service = explode(":", $service_full);

    // Service names are expected to be parsed by Name:machine_name. If only one
    // element in the array is given, it's an invalid service.
    if (count($service) < 2) {
      continue;
    }

    // Find the service code name.
    $serviceCodeName = substr($service[1], 0, -1);

    // Switch the title on a per-service basis if required.
    $title = $mTitle;
    switch ($serviceCodeName) {
      case 'twitter':
        $title = empty($data_options['twitter_suffix']) ? $mTitle : check_plain($mTitle) . ' ' . check_plain($data_options['twitter_suffix']);
        break;
    }

    // Sanitize the service code for display.
    $display = check_plain($serviceCodeName);

    // Put together the span attributes.
    $attributes = array(
      'st_url' => $mPath,
      'st_title' => $title,
      'class' => 'st_' . $display . $type,
    );
    if ($serviceCodeName == 'twitter') {
      if (!empty($data_options['twitter_handle'])) {
        $attributes['st_via'] = $data_options['twitter_handle'];
      }
    }
    // Only show the display text if the type is set.
    if (!empty($type)) {
      $attributes['displaytext'] = check_plain($display);
    }
    // Render the span tag.
    $st_spans .= theme('html_tag', array(
      'element' => array(
        '#tag' => 'span',
        '#attributes' => $attributes,
        '#value' => '', // It's an empty span tag.
      ),
					 ));

    // @author griffinj
    // Appending to the span tag

    //$st_spans .= l($display, $mPath, array('attributes' => $attributes));
  }

  // These are the ShareThis scripts:
  $st_js_options['switchTo5x'] = $data_options['widget'] == 'st_multi' ? TRUE : FALSE;
  if ($data_options['late_load']) {
    $st_js_options['__st_loadLate'] = TRUE;
  }
  $st_js = "<script type='text/javascript'>";
  foreach ($st_js_options as $name => $value) {
    $st_js .= 'var ' . $name . ' = ' . drupal_json_encode($value) . ';';
  }
  $st_js .= '</script>';

  // Check if we're using SSL or not.
  if (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on') {
    $st_js .= "<script type='text/javascript' src='https://ws.sharethis.com/button/buttons.js'></script>";
  }
  else {
    $st_js .= "<script type='text/javascript' src='http://w.sharethis.com/button/buttons.js'></script>";
  }

  // This can only be further themed with JavaScript
  drupal_add_js( drupal_get_path('theme', 'bootstrap_dss_lebanesetown') . '/js/sharethis_helper.js');

  // Provide the publisher ID.
  $stlight = drupal_json_encode(array(
    'publisher' => $data_options['publisherID'],
  ));
  $st_js .= "<script type='text/javascript'>stLight.options($stlight);</script>";
  return '<div class="sharethis-wrapper">' . $st_spans . $st_js . '</div>';
}

/**
 * HTML contents for sharethis_block_contents
 * @author griffinj
 *
 * @see sharethis_block_contents()
 * @return string
 *
 */
function _bootstrap_dss_lebanesetown_sharethis_block_contents() {

  if (variable_get('sharethis_location', 'content') == 'block') {

    // First get all of the options for the sharethis widget from the database:
    $data_options = sharethis_get_options_array();
    $path = isset($_GET['q']) ? $_GET['q'] : '<front>';
    $mPath = url($_GET['q'], array('absolute' => TRUE));
    $mTitle = drupal_get_title();
    return _bootstrap_dss_lebanesetown_sharethis_get_button_HTML($data_options, $mPath, $mTitle);
  }
}

/**
 * @author griffinj
 * Implements hook_block_view_MODULE_DELTA_alter(&$data, $block).
 *
 */
function bootstrap_dss_lebanesetown_block_view_sharethis_sharethis_block_alter(&$data, $block) {

  $data['content'] = _bootstrap_dss_lebanesetown_sharethis_block_contents();
}

function bootstrap_dss_lebanesetown_block_view_islandora_solr_simple_alter(&$data, $block) {

  // Magic constant of 13 for width
  // Must refactor this
  $data['content']['simple']['#suffix'] = l(t('Advanced Search'), '', array('attributes' => array('data-toggle' => 'lafayette-dss-modal',
												  'data-target' => '#advanced-search-modal',
												  'data-width' => '560px',
												  'data-width-offset' => '-15px',
												  'data-height-offset' => '16px'),
									    'fragment' => ' ',
									    'external' => TRUE));
}
