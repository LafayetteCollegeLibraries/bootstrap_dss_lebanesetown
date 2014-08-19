<?php

  /**
   * @file Functionality for islandora_basic_collection
   * @author griffinj@lafayette.edu
   *
   */

function bootstrap_dss_lebanesetown_process_islandora_basic_collection(&$variables) {

  $islandora_object = $variables['islandora_object'];
  $collection_pid = $islandora_object->id;

  foreach($variables['associated_objects_array'] as &$associated_object) {

    $object = $associated_object['object'];
    $pid = $associated_object['pid'];

    $title = $associated_object['title_link'];
    $thumbnail_img = $associated_object['thumbnail'];
    $object = $associated_object['object'];


    // Work-around
    // Refactor

    $pid_relation_is_part_of_map = array(
					 'eastAsia:imperialPostcards' => 'Imperial Postcard Collection',
					 'eastAsia:linPostcards' => 'Lin Chia-Feng Family Postcard Collection',
					 'eastAsia:lewis' => 'Michael Lewis Taiwan Postcard Collection',
					 'eastAsia:pacwarPostcards' => 'Pacific War Postcard Collection',
					 'eastAsia:paKoshitsu' => 'Japanese Imperial House Postcard Album',
					 'eastAsia:paOmitsu01' => 'Sino-Japanese War Postcard Album 01',
					 'eastAsia:paOmitsu02' => 'Sino-Japanese War Postcard Album 02',
					 'eastAsia:paTsubokura' => 'Tsubokura Russo-Japanese War Postcard Album',
					 );

    if(preg_match('/eastAsia:.*/', $pid)) {

      $associated_object['title_link'] = l($title,
					   'islandora/search/cdm.Relation.IsPartOf:"'. $pid_relation_is_part_of_map[$pid] .'"',
					   array('html' => TRUE,
						 'alias' => TRUE,
						 'attributes' => array('title' => $title)));
    
      $associated_object['thumb_link'] = l($thumbnail_img,
					   'islandora/search/cdm.Relation.IsPartOf:"'. $pid_relation_is_part_of_map[$pid] .'"',
					   array('html' => TRUE,
						 'alias' => TRUE,
						 'attributes' => array('title' => $title)));

    } elseif($pid == 'islandora:cap') {

      $associated_object['title_link'] = l($title,
					   'islandora/search/cdm.Relation.IsPartOf:"Historical Photograph Collection"',
					   array('html' => TRUE,
						 'alias' => TRUE,
						 'attributes' => array('title' => $title)));
    
      $associated_object['thumb_link'] = l($thumbnail_img,
					   'islandora/search/cdm.Relation.IsPartOf:"Historical Photograph Collection"',
					   array('html' => TRUE,
						 'alias' => TRUE,
						 'attributes' => array('title' => $title)));

    } elseif($pid == 'islandora:geologySlidesEsi') {

      $associated_object['title_link'] = l($title,
					   'islandora/search/cdm.Relation.IsPartOf:"John S. Shelton Earth Science Image Collection"',
					   array('html' => TRUE,
						 'alias' => TRUE,
						 'attributes' => array('title' => $title)));

      $associated_object['thumb_link'] = l($thumbnail_img,
					   'islandora/search/cdm.Relation.IsPartOf:"John S. Shelton Earth Science Image Collection"',
					   array('html' => TRUE,
						 'alias' => TRUE,
						 'attributes' => array('title' => $title)));

    } elseif($pid == 'islandora:mdlPrints') {

      $associated_object['title_link'] = l($title,
					   'islandora/search/cdm.Relation.IsPartOf:"Marquis de Lafayette Prints Collection"',
					   array('html' => TRUE,
						 'alias' => TRUE,
						 'attributes' => array('title' => $title)));
    
      $associated_object['thumb_link'] = l($thumbnail_img,
					   'islandora/search/cdm.Relation.IsPartOf:"Marquis de Lafayette Prints Collection"',
					   array('html' => TRUE,
						 'alias' => TRUE,
						 'attributes' => array('title' => $title)));
    }
  }

  
  }

function bootstrap_dss_lebanesetown_preprocess_islandora_basic_collection_wrapper(&$variables) {
  
  $variables['slide_drawers'] = TRUE;

  // For rendering non-grid content
  drupal_add_css(drupal_get_path('module', 'islandora_solr') . '/css/islandora_solr.base.css');
  drupal_add_css(drupal_get_path('module', 'islandora_solr') . '/css/islandora_solr.theme.css');

  /*
  $query_params['display'] = 'list';
  $list_link = array(
    'title' => t('List view'),
    'attributes' => array(
      'href' => url($path, array('query' => $query_params)),
      'class' => array('islandora-view-list'),
    ),
  );

  $query_params['display'] = 'grid';
  $grid_link = array(
    'title' => t('Grid view'),
    'attributes' => array(
      'href' => url($path, array('query' => $query_params)),
      'class' => array('islandora-view-grid'),
    ),
  );
  */
  $islandora_object = $variables['islandora_object'];
  $display = (empty($_GET['display'])) ? variable_get('islandora_basic_collection_default_view', 'grid') : $_GET['display'];
  $link_text = (empty($_GET['display'])) ? 'grid' : $_GET['display'];
  $query_params = drupal_get_query_parameters($_GET);

  global $base_url;

  if ($display == 'grid') {

    $query_params['display'] = 'list';
    
    /*
    $list_link = array(
		       'title' => 'List view',
		       'href' => $base_url . '/islandora/object/' . $islandora_object->id,
		       'attributes' => array(
					     'class' => 'islandora-view-list',
					     ),
		       'query' => $query_params,
		       );
    */

    $list_link = l('List view',
		   $base_url . '/islandora/object/' . $islandora_object->id,
		   array('attributes' => array('class' => array('islandora-view-list')),
			 'query' => $query_params));

    unset($query_params['display']);
    $query_params['display'] = 'grid';

    /*
    $grid_link = array(
      'title' => 'Grid view',
      'href' => $base_url . '/islandora/object/' . $islandora_object->id,
      'attributes' => array('class' => array('islandora-view-grid', 'active')),
      'query' => $query_params,
    );
    */

    $grid_link = l('Grid view',
		   $base_url . '/islandora/object/' . $islandora_object->id,
		   array('attributes' => array('class' => array('islandora-view-grid', 'active')),
			 'query' => $query_params));

  } else {

    $query_params['display'] = 'list';

    /*
    $list_link = array(
      'title' => 'List view',
      'href' => $base_url . '/islandora/object/' . $islandora_object->id,
      'attributes' => array('class' => array('islandora-view-list', 'active')),
      'query' => $query_params,
    );
    */

    $list_link = l('List view',
		   $base_url . '/islandora/object/' . $islandora_object->id,
		   array('attributes' => array('class' => array('islandora-view-list', 'active')),
			 'query' => $query_params));

    unset($query_params['display']);
    $query_params['display'] = 'grid';

    /*
      $grid_link = array(
		       'title' => 'Grid view',
		       'href' => $base_url . '/islandora/object/' . $islandora_object->id,
		       'attributes' => array('class' => 'islandora-view-grid'),
		       'query' => $query_params,
		       );
    */

    $grid_link = l(t('Grid view'),
		   $base_url . '/islandora/object/' . $islandora_object->id,
		   array('attributes' => array('class' => 'islandora-view-grid'),
			 'query' => $query_params));
  }

  $refine_link = l(t('Refine'),
		   $base_url . '/islandora/object/' . $islandora_object->id . '#',
		   array('attributes' => array('class' => array('snap-trigger'))));
  
  $view_links = array('refine' => $refine_link,
		      'list' => $list_link,
		      'grid' => $grid_link);

  $variables['view_links'] = $view_links;

  $variables['display'] = $display;

  /**
   * Terrible implementation
   * @todo Refactor with an additional theme hook
   *
   */
  
  $collection_pager = $variables['collection_pager'];
  $view_link_markup = '';
  
  foreach($view_links as $label => $link) {

    $view_link_markup .= '

	      <li>
		<span id="view-' . $label . '-icon" ></span>
		' . $link . '
	      </li>
';
  }

  $discovery_controls = '
      <div class="islandora-discovery-controls">

	<div class="islandora-discovery-inner-container">

	  <div class="islandora-page-controls">

	    <!-- @todo Resolve this (possibly related to a styling issue? -->
	    <form id="islandora-discovery-form" action="/" >
	  </div><!--/.islandora-page-controls -->

	  <div class="islandora-discovery-control page-number-control">
	    
	    <span>Show:</span>
	    <select>
	      <option>25</option>
	    </select>
	  </div><!-- /.islandora-discovery-control -->

	  <div class="islandora-discovery-control title-sort-control">

	    <span>Sort by:</span>
	    <select>
	      <option>Title</option>
	    </select>
	  </div><!-- /.islandora-discovery-control -->

	    </form><!-- /#islandora-discovery-form -->
	    ' . $collection_pager . '
	</div><!-- /.islandora-discovery-inner-container -->

	<span class="islandora-basic-collection-display-switch">
	  
	  <ul class="links inline">

	    ' . $view_link_markup . '
	  </ul><!-- /.links -->
	</span><!-- /.islandora-basic-collection-display-switch -->
	
      </div><!-- /.islandora-discovery-controls -->
';

  //$variables['discovery_controls_old'] = $discovery_controls;
}
