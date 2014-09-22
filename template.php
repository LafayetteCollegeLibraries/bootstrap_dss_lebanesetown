<?php
/**
 * @file template.php
 * @author griffinj@lafayette.edu
 * This file contains the primary theme hooks found within any given Drupal 7.x theme
 * 
 * @todo Implement some Drupal theming hooks
 */

  // Includes functions to create Islandora Solr blocks.
require_once dirname(__FILE__) . '/includes/blocks.inc';
require_once dirname(__FILE__) . '/includes/forms.inc';
require_once dirname(__FILE__) . '/includes/menus.inc';
require_once dirname(__FILE__) . '/includes/dss_mods.inc';
require_once dirname(__FILE__) . '/includes/dss_dc.inc';
require_once dirname(__FILE__) . '/includes/pager.inc';
require_once dirname(__FILE__) . '/includes/apachesolr.inc';
require_once dirname(__FILE__) . '/includes/islandora_solr.inc';
require_once dirname(__FILE__) . '/includes/islandora_basic_collection.inc';

function bootstrap_dss_lebanesetown_preprocess_node(&$vars) {

  if($vars['page']) {

    // Add header meta tag for IE to head
    global $base_url;
    $meta_element_open_graph_type = array('#type' => 'html_tag',
					  '#tag' => 'meta',
					  '#attributes' => array('property' =>  'og:type',
								 'content' => 'article'),
					  );

    $meta_element_open_graph_url = array('#type' => 'html_tag',
					 '#tag' => 'meta',
					 '#attributes' => array('property' =>  'og:url',
								'content' => $base_url . '/' . drupal_get_path_alias()
								),
					 );
    
    $meta_element_open_graph_author = array('#type' => 'html_tag',
					    '#tag' => 'meta',
					    '#attributes' => array('property' =>  'og:author',
								   'content' => 'https://www.facebook.com/LafayetteCollegeLibrary',
								   )
					    );
    
    $meta_element_open_graph_title = array('#type' => 'html_tag',
					   '#tag' => 'meta',
					   '#attributes' => array('property' =>  'og:title',
								  'content' => $vars['title'],
								  )
					   );

    // For all <meta> elements
    $meta_elements = array('meta_element_open_graph_type' => $meta_element_open_graph_type,
			   'meta_element_open_graph_url' => $meta_element_open_graph_url,
			   'meta_element_open_graph_author' => $meta_element_open_graph_author,
			   'meta_element_open_graph_title' => $meta_element_open_graph_title,
			   );
    $meta_elements['meta_element_open_graph_image'] = array('#type' => 'html_tag',
							    '#tag' => 'meta',
							    '#attributes' => array('property' =>  'og:image',
										   'content' => $base_url . '/' . drupal_get_path('theme', 'bootstrap_dss_lebanesetown') . '/files/dss_logo_full.png',
										   ),
							    );
    $meta_elements['meta_element_open_graph_site_name'] = array('#type' => 'html_tag',
								'#tag' => 'meta',
								'#attributes' => array('property' =>  'og:site_name',
										       'content' => 'Digital Scholarship Services',
										       )
								);

    foreach($meta_elements as $key => $meta_element) {

      // Add header meta tag for IE to head
      drupal_add_html_head($meta_element, $key);
    }
  }

  /**
   * Implements redirection for the Repository Migration page
   * @todo Refactor
   * Resolves DSSSM-826
   */
  if($vars['node_url'] == '/redirect') {

    drupal_add_js('jQuery(document).ready(function() { setTimeout(function() { window.location.replace("/"); }, 7000); });',
		  array('type' => 'inline', 'scope' => 'footer', 'weight' => 5)
		  );
  }
}

/**
 * Implements template_preprocess_hybridauth_widget
 * @griffinj
 *
 */
function bootstrap_dss_lebanesetown_preprocess_hybridauth_widget(&$vars) {

  // Refactor
  $i = 0;
  foreach (hybridauth_get_enabled_providers() as $provider_id => $provider_name) {

    //$vars['providers'][$i] = preg_replace('/(<\/span>)/', "</span><span>&nbsp;$provider_name</span>", $vars['providers'][$i]);
    $i++;
  }
}

function _bootstrap_dss_lebanesetown_user_logout($account) {

  if (variable_get('user_pictures', 0)) {
    
    if (!empty($account->picture)) {

      if (is_numeric($account->picture)) {

        $account->picture = file_load($account->picture);
      }
      if (!empty($account->picture->uri)) {

        $filepath = $account->picture->uri;
      }
    } elseif (variable_get('user_picture_default', '')) {

      $filepath = variable_get('user_picture_default', '');
    }

    if (isset($filepath)) {

      $alt = t("@user's picture", array('@user' => format_username($account)));
      // If the image does not have a valid Drupal scheme (for eg. HTTP),
      // don't load image styles.
      if (module_exists('image') && file_valid_uri($filepath) && $style = variable_get('user_picture_style', '')) {

        $user_picture = theme('image_style', array('style_name' => $style, 'path' => $filepath, 'alt' => $alt, 'title' => $alt));
      }
      else {

        $user_picture = theme('image', array('path' => $filepath, 'alt' => $alt, 'title' => $alt));
      }

      /*
       * Generate the CAS logout link
       *
       */
      $attributes = array('https' => TRUE,
			  'attributes' => array('title' => t('Log out.')),
			  'html' => TRUE,
			  );

      // If we're currently authenticated by CAS, this apparently does not function...
      if(cas_user_is_logged_in()) {

	global $base_url;
	$attributes['query'] = array('destination' => current_path());
	return l($user_picture, 'caslogout', $attributes);
      }

      return l($user_picture, "user/logout", $attributes);
    }
  }
}

/**
 * Preprocess variables for page.tpl.php
 *
 * @see page.tpl.php
 */

function bootstrap_dss_lebanesetown_preprocess_page(&$variables) {

  // Add information about the number of sidebars.
  if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
    $variables['columns'] = 3;

  }
  elseif (!empty($variables['page']['sidebar_first'])) {
    $variables['columns'] = 2;
  }
  elseif (!empty($variables['page']['sidebar_second'])) {
    $variables['columns'] = 2;
  }
  else {
    $variables['columns'] = 1;
  }

  // Primary nav
  $variables['primary_nav'] = FALSE;
  if ($variables['main_menu']) {
    // Build links
    $variables['primary_nav'] = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
    // Provide default theme wrapper function
    $variables['primary_nav']['#theme_wrappers'] = array('menu_tree__primary');
  }

  // Secondary nav
  $variables['secondary_nav'] = FALSE;
  if ($variables['secondary_menu']) {
    // Build links
    $variables['secondary_nav'] = menu_tree(variable_get('menu_secondary_links_source', 'user-menu'));
    // Provide default theme wrapper function
    $variables['secondary_nav']['#theme_wrappers'] = array('menu_tree__secondary');
  }

  /**
   *browscap integration
   * Capture from the User-Agent value the type of device being used to browse the page
   * (Probably should be decoupled and integrated into CSS and JavaScript)
   *
   */
  $browser = browscap_get_browser();
  $is_smartphone_browser = $browser['ismobiledevice'] && preg_match('/iPhone|(?:Android.*?Mobile)|(?:Windows Phone)/', $browser['useragent']);

  /**
   * Ensure that the "Contact Us" link directs users to the Drupal Node only for non-smartphone devices
   * Resolves DSSSM-635
   * @todo Refactor for specifying the path to the "Contact Us" form
   *
   */
  if($is_smartphone_browser) {

    // The "Contact Us" link (to the path "contact")
    $variables['contact_anchor'] = l(t('Contact Us'), 'contact');
  } else {
  
    // The "Contact Us" link
    $variables['contact_anchor'] = l(t('Contact Us'), '', array('attributes' => array('data-toggle' => 'lafayette-dss-modal',
										      'data-target' => '#contact',
										      'data-anchor-align' => 'false'),
								'fragment' => ' ',
								'external' => TRUE));
  }



  // Different images must be passed based upon the browser type

  // Shouldn't be parsing the string itself; refactor
  if($is_smartphone_browser) {
    //if(TRUE) {

    $variables['dss_logo_image'] = theme_image(array('path' => drupal_get_path('theme', 'bootstrap_dss_lebanesetown') . '/files/dss_logo_mobile.png',
						     'alt' => t('digital scholarship services logo'),
						     'attributes' => array()));
  } else {

    // Work-around for the logo image
    $variables['dss_logo_image'] = theme_image(array('path' => drupal_get_path('theme', 'bootstrap_dss_lebanesetown') . '/files/dss_logo.png',
						     'alt' => t('digital scholarship services logo'),
						     'attributes' => array()));
  }

  // The "Log In" link
  //$variables['auth_anchor'] = l(t('Log In'), '', array('attributes' => array('data-toggle' => 'lafayette-dss-modal',
  /*
  $variables['auth_anchor'] = l('<div class="auth-icon"><img src="/sites/all/themes/bootstrap_lafayette_lib_dss/files/UserIcon.png" /><span>Log In</span></div>', '', array('attributes' => array('data-toggle' => 'lafayette-dss-modal',
														    'data-target' => '#auth-modal',
																								  'data-width-offset' => '10px',
														    'data-height-offset' => '28px'),
											      'fragment' => ' ',
											      //'external' => TRUE));
											      'external' => TRUE,
											      'html' => TRUE
											      ));
  */

  /**
   * Disabled for the initial release of the site
   * @todo Re-integrate for cases requiring Facebook and Twitter authentication
   *
   */
  //  $variables['auth_anchor'] = '<a data-toggle="lafayette-dss-modal" data-target="#auth-modal" data-width-offset="0px" data-height-offset="30px"><div class="auth-icon navbar-icon"><img src="/sites/all/themes/bootstrap_dss_lebanesetown/files/UserIcon.png" /><span>Log In</span></div></a>';
  global $base_url;

  /**
   * Work-around for submitting GET parameters within the "destination" parameter for CAS redirection
   * Resolves DSS-192
   *
   */
  $GET_params = $_SERVER['QUERY_STRING'];

  $variables['auth_anchor'] = l('<div class="auth-icon navbar-icon"><img src="/sites/all/themes/bootstrap_dss_lebanesetown/files/UserIcon.png" /><span>Log In</span></div>',
				'cas',
				array('html' => TRUE,
				      'https' => true,
				      'query' => array('destination' => current_path() . '?' . $GET_params )
				      )
				);

  // The "Log Out" link
  // This needs to be refactored for integration with the CAS module
  // If we're currently authenticated by CAS, this apparently does not function...
  if(cas_user_is_logged_in()) {

    global $base_url;
    $variables['logout_anchor'] = l(t('Log Out'), 'caslogout', array('query' => array('destination' => current_path())));
  } else {

    $variables['logout_anchor'] = l(t('Log Out'), 'user/logout');
  }

  // The "Share" link
  //$variables['share_anchor'] = l(t('Share'), '', array('attributes' => array('data-toggle' => 'lafayette-dss-modal',
  /*
  $variables['share_anchor'] = l('<div class="share-icon"><img src="/sites/all/themes/bootstrap_lafayette_lib_dss/files/ShareIcon.png" /><span>Share</span></div>', '', array('attributes' => array('data-toggle' => 'lafayette-dss-modal',
									     'data-target' => '#share-modal',
																								    'data-width-offset' => '10px',
									     'data-height-offset' => '28px'
									     ),
						       'fragment' => ' ',
						       //'external' => TRUE));
						       'external' => TRUE,
						       'html' => TRUE
						       ));
  */

  $variables['share_anchor'] = '<a data-toggle="lafayette-dss-modal" data-target="#share-modal" data-width-offset="10px" data-height-offset="28px"><div class="share-icon navbar-icon"><img src="/sites/all/themes/bootstrap_dss_lebanesetown/files/ShareIcon.png" /><span>Share</span></div></a>';

  // Render thumbnails for authenticated users
  $variables['user_picture'] = '<span class="button-auth-icon"></span>';

  if(user_is_logged_in()) {

    // For the user thumbnail
    global $user;

    //$user_view = user_view($user);
    //$variables['user_picture'] = drupal_render($user_view['user_picture']);
    $variables['user_picture'] = _bootstrap_dss_lebanesetown_user_logout($user);
  }

  /**
   * Variables for the Islandora simple_search Block
   *
   */
  // A search button must be passed if this is being viewed with a mobile browser

  $search_icon = theme_image(array('path' => drupal_get_path('theme', 'bootstrap_dss_lebanesetown') . '/files/SearchIcon.png',
				   'alt' => t('search the site'),
				   'attributes' => array()));

  $simple_search_mobile = '<a data-toggle="lafayette-dss-modal" data-target="#advanced-search-modal" data-width-offset="-286px" data-height-offset="28px">
<div class="simple-search-icon">' . $search_icon . '<span>Search</span></div></a>' . render($variables['page']['simple_search']);
  unset($variables['page']['simple_search']);
  //$variables['simple_share_mobile_container'] = '<div class="modal-container container"><div id="simple-search-control-container" class="modal-control-container container">' . $simple_search_mobile . '</div></div>';
  $variables['search_container'] = '<div class="modal-container container"><div id="simple-search-control-container" class="modal-control-container container">' . $simple_search_mobile . '</div></div>';


  // Refactor
  $auth_container = '
     <div class="auth-container modal-container container">
       <div id="auth-control-container" class="modal-control-container container">';

  /*
    <?php if (!empty($page['auth'])): ?>

    <!-- <div class="auth-icon"><img src="/sites/all/themes/bootstrap_dss_islandora_dev/files/UserIcon.png" /></div> -->
    <?php print $auth_anchor; ?>
    <?php else: ?>
    
    <div class="auth-icon"><?php print $user_picture; ?></div>
    <div class="auth-link"><?php print $logout_anchor; ?></div>
    <?php endif; ?>
   */

  if(!empty($variables['page']['auth'])) {

    $auth_container .= $variables['auth_anchor'];
  } else {
    
    $auth_container .= '
      <div class="auth-icon">' . $variables['user_picture'] . '</div>
      <div class="auth-link">' . $variables['logout_anchor'] . '</div>';
  }

  $auth_container .= '
       </div><!-- /#auth-control-container -->
     </div><!-- /.auth-container -->';

  $variables['auth_container'] = $auth_container;

  $share_container = '
     <div class="share-container modal-container container">
       <div id="share-control-container" class="modal-control-container container">

         ' . $variables['share_anchor'] . '
       </div><!-- /#share-control-container -->
     </div><!-- /.share-container -->';

  $variables['share_container'] = $share_container;

  $menu_toggle_image = theme_image(array('path' => drupal_get_path('theme', 'bootstrap_dss_lebanesetown') . '/files/MenuIcon.png',
					 'alt' => t('mobile menu'),
					 'attributes' => array()));

  $variables['menu_toggle_image'] = $menu_toggle_image;

  $menu_toggle_container = '

       <div id="menu-toggle-control-container" class="modal-control-container container">
<div class="navbar-collapse-toggle">
<!-- .btn-navbar is used as the toggle for collapsed navbar content -->
  <div data-toggle="collapse" data-target=".nav-collapse">
    <div id="menu-toggle-icon" class="navbar-icon btn-navbar">' . $menu_toggle_image . '<span id="btn-navbar-caption" class="">Menu</span></div>
  </div>
</div><!-- /.navbar-collapse-toggle -->
</div>';

  $variables['menu_toggle_container'] = $menu_toggle_container;

  // Adding the tabs for certain nodes
  /*
  $eastasia_tabs = quicktabs_load('east_asia_image_collections');
  $mdl_tabs = quicktabs_load('marquis_de_lafayette_prints_coll');

  $variables['tabs'] = array('eastasia_tabs' => theme('quicktabs', (array) $eastasia_tabs),
			     'mdl_tabs' => theme('quicktabs', (array) $mdl_tabs));
  */



  // Panel
  /*
  $slide_panel_container = '
      <div id="menu" class="menu nav-collapse collapse width">
        <div class="collapse-inner">
          <div class="navbar navbar-inverse">
            <div class="navbar-inner">
              Menu
            </div>
          </div>
        ' . $variables['page']['slide_panel'] . '
        </div>
      </div><!-- /#menu -->
      <div class="view">
        <div class="navbar navbar-inverse">
          <div class="navbar-inner">
            <button type="button" class="btn btn-navbar" data-toggle="collapse" data-target="#menu">
              <span class="icon-bar"></span>
              <span class="icon-bar"></span>
              <span class="icon-bar"></span>
            </button>
          </div>
        </div><!-- /.view -->
      </div>
';
  */
  $slide_panel_container = '';

  $variables['slide_panel_container'] = $slide_panel_container;

  $variables['breadcrumb'] = theme('breadcrumb', menu_get_active_trail());
  //$variables['breadcrumb'] = theme('breadcrumb', menu_get_active_breadcrumb());
  //$variables['breadcrumb'] = theme('breadcrumb', drupal_get_breadcrumb());

  $variables['slide_drawers'] = TRUE;
}

/**
 * Implements template_preprocess_html
 *
 */
function bootstrap_dss_lebanesetown_preprocess_html(&$variables) {

  drupal_add_library('system', 'effects.drop');
  drupal_add_library('system', 'effects.slide');
}

/**
 * Work-around for ensuring that the search form is not forcibly displayed within search results
 * See https://drupal.org/comment/4573218#comment-4573218
 *
 */
function bootstrap_dss_lebanesetown_process_page(&$variables) {

  if(array_key_exists('search_form', $variables['page']['content']['system_main'])) {

    hide($variables['page']['content']['system_main']['search_form']);
  }
}

function bootstrap_dss_lebanesetown_theme_registry_alter(&$registry) {

  $registry['hybridauth_widget']['file'] = 'template';
}

/**
 * Please see http://www.php.net/manual/en/function.ip2long.php#82397
 * @todo Integrate with islandora_dss_solr_net_match()
 * @see islandora_dss_solr_net_match().
 *
 * This assumes a subnet of 139.147.0.0/16 for Lafayette College servers
 * This assumes a subnet of 192.168.101.0/24 for the VPN
 */
function bootstrap_dss_lebanesetown_net_match($CIDR, $IP) {

  list($net, $mask) = explode('/', $CIDR);
  return ( ip2long ($IP) & ~((1 << (32 - $mask)) - 1) ) == ip2long ($net);
}

function bootstrap_dss_lebanesetown_preprocess_islandora_large_image(array &$variables) {

  /**
   * Work-around given the issues for hook_menu_alter() and hook_preprocess_HOOK() implementations
   * @todo Refactor either into hook_menu_alter() or hook_preprocess_HOOK() implementations
   *
   */
  $object = $variables['islandora_object'];

  $client_ip = ip_address();
  $headers = apache_request_headers();

  // ...not within the campus network...
  // (for proxy servers...)
  if(array_key_exists('X-Forwarded-For', $headers)) {

    // Not on the VPN...
    $is_anon_non_lafayette_user = !islandora_dss_solr_net_match('192.168.101.0/24', $headers['X-Forwarded-For']);
    $is_anon_non_lafayette_user &= (bool) !islandora_dss_solr_net_match('139.147.0.0/16', $headers['X-Forwarded-For']);
  } else {

    // Not on the VPN...
    $is_anon_non_lafayette_user = !islandora_dss_solr_net_match('192.168.101.0/24', $client_ip);
    $is_anon_non_lafayette_user &= (bool) !islandora_dss_solr_net_match('139.147.0.0/16', $client_ip);
  }
  $is_anon_non_lafayette_user &= !user_is_logged_in(); // ...and not authenticated.

  // This fully resolves DSS-280
  $is_anon_non_lafayette_user = (bool) $is_anon_non_lafayette_user;

  if(in_array('islandora:geologySlidesEsi', $object->getParents()) and $is_anon_non_lafayette_user) {

    /**
     * Functionality for redirecting authentication requests over HTTPS
     * @see securelogin_secure_redirect()
     * @todo Refactor
     *
     */

    global $is_https;

    // POST requests are not redirected, to prevent unintentional redirects which
    // result in lost POST data. HTTPS requests are also not redirected.
    if(!$is_https) {

      $path = $_GET['q'];
      $http_response_code = 301;
      // Do not permit redirecting to an external URL.
      $options = array('query' => drupal_get_query_parameters(), 'https' => TRUE, 'external' => FALSE);
      // We don't use drupal_goto() here because we want to be able to use the
      // page cache, but let's pretend that we are.
      drupal_alter('drupal_goto', $path, $options, $http_response_code);
      // The 'Location' HTTP header must be absolute.
      $options['absolute'] = TRUE;
      $url = url($path, $options);
      $status = "$http_response_code Moved Permanently";
      drupal_add_http_header('Status', $status);
      drupal_add_http_header('Location', $url);
      // Drupal page cache requires a non-empty page body for some reason.
      print $status;
      // Mimic drupal_exit() and drupal_page_footer() and then exit.
      module_invoke_all('exit', $url);
      drupal_session_commit();
      if (variable_get('cache', 0) && ($cache = drupal_page_set_cache())) {
	drupal_serve_page_from_cache($cache);
      } else {
	ob_flush();
      }

      exit;
    } else {

      drupal_goto('cas', array('query' => array('destination' => current_path())));
    }
  }

  // Refactor
  // Retrieve the MODS Metadata
  if(isset($object['MODS'])) {

    try {

      $mods_str = $object['MODS']->content;

      $mods_str = preg_replace('/<\?xml .*?\?>/', '', $mods_str);
      $mods_object = new DssMods($mods_str);
    } catch (Exception $e) {
    
      drupal_set_message(t('Error retrieving object %s %t', array('%s' => $object->id, '%t' => $e->getMessage())), 'error', FALSE);
    }

    $label_map = array_flip(islandora_solr_get_fields('result_fields', FALSE));
    //$facet_pages_fields_data = variable_get('islandora_solr_facet_pages_fields_data', array());
    //$label_map = array();

    //$element['facet'] = $label_map[$facet];
  }

  /**
   * Resolves DSS-261
   *
   */
  $variables['mods_object'] = isset($mods_object) ? $mods_object->toArray($label_map) : array();
  
  $rendered_fields = array();
  foreach($variables['mods_object'] as $key => &$value) {

    if(!in_array($value['label'], $rendered_fields)) {

      //$value['class'] .= ' islandora-inline-metadata-displayed';
      $rendered_fields[] = $value['label'];
    } else {

      $value['label'] = '';
    }
  }

  /**
   * Work-around for appended site-generated resource metadata into the Object
   * Refactor (or, ideally, update the MODS when Drush creates or updates the path alias)
   * Resolves DSS-243
   *
   */

  global $base_url;
  // The proper approach (production)
  //$path_alias = $base_url . '/' . drupal_get_path_alias("islandora/object/{$object->id}");
  // The less proper approach (enforce HTTP while ensuring that other linked metadata field values are possibly tunneled through TLS/SSL)
  //$path_alias = str_replace('https', 'http', $base_url) . '/' . drupal_get_path_alias("islandora/object/{$object->id}");
  // Specific to the production environment
  $path_alias = 'http://digital.lafayette.edu/' . drupal_get_path_alias("islandora/object/{$object->id}");
  $variables['mods_object']['drupal_path'] = array('class' => '',
						   'label' => 'URL',
						   'value' => $path_alias,
						   'href' =>  $path_alias);
}

//module_load_include('inc', 'bootstrap_dss_lebanesetown', 'includes/dssMods');

function bootstrap_dss_lebanesetown_preprocess_islandora_book_book(array &$variables) {

  $object = $variables['object'];

  /**
   * Work-around for displaying metadata
   * Refactor after re-indexing as transformed MODS
   *
   */
  if(in_array('islandora:newspaper', $object->getParents())) {

    $mods_object = new DssDc($object['DC']->content);
  } else {

    // Refactor
    // Retrieve the MODS Metadata
    try {

      $mods_str = $object['MODS']->content;

      $mods_str = preg_replace('/<\?xml .*?\?>/', '', $mods_str);
      //$mods_str = '<modsCollection>' . $mods_str . '</modsCollection>';

      $mods_object = new DssMods($mods_str);
    } catch (Exception $e) {
    
      drupal_set_message(t('Error retrieving object %s %t', array('%s' => $object->id, '%t' => $e->getMessage())), 'error', FALSE);
    }
  }

  $label_map = array_flip(islandora_solr_get_fields('result_fields', FALSE));

  $variables['mods_object'] = isset($mods_object) ? $mods_object->toArray($label_map) : array();
  
  $rendered_fields = array();
  foreach($variables['mods_object'] as $key => &$value) {

    if(!in_array($value['label'], $rendered_fields)) {

      //$value['class'] .= ' islandora-inline-metadata-displayed';
      $rendered_fields[] = $value['label'];
    } else {

      $value['label'] = '';
    }
  }

  /**
   * Work-around for appended site-generated resource metadata into the Object
   * Refactor (or, ideally, update the MODS when Drush creates or updates the path alias)
   * Resolves DSS-243
   *
   */

  global $base_url;
  // The proper approach (production)
  //$path_alias = $base_url . '/' . drupal_get_path_alias("islandora/object/{$object->id}");
  // The less proper approach (enforce HTTP while ensuring that other linked metadata field values are possibly tunneled through TLS/SSL)
  //$path_alias = str_replace('https', 'http', $base_url) . '/' . drupal_get_path_alias("islandora/object/{$object->id}");
  // Specific to the production environment
  $path_alias = 'http://digital.lafayette.edu/' . drupal_get_path_alias("islandora/object/{$object->id}");
  $variables['mods_object']['drupal_path'] = array('class' => '',
						   'label' => 'URL',
						   'value' => $path_alias,
						   'href' =>  $path_alias);
}

function bootstrap_dss_lebanesetown_preprocess_islandora_book_page(array &$variables) {

  $object = $variables['object'];

  // Refactor
  // Retrieve the MODS Metadata
  try {

    $mods_str = $object['MODS']->content;

    $mods_str = preg_replace('/<\?xml .*?\?>/', '', $mods_str);
    //$mods_str = '<modsCollection>' . $mods_str . '</modsCollection>';

    $mods_object = new DssMods($mods_str);
  } catch (Exception $e) {
    
    drupal_set_message(t('Error retrieving object %s %t', array('%s' => $object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }

  $variables['mods_object'] = isset($mods_object) ? $mods_object->toArray() : array();
}

function bootstrap_dss_lebanesetown_preprocess_islandora_book_pages(array &$variables) {

  // View Links.
  $display = (empty($_GET['display'])) ? 'grid' : $_GET['display'];
  $grid_active = ($display == 'grid') ? 'active' : '';
  $list_active = ($display == 'active') ? 'active' : '';

  $query_params = drupal_get_query_parameters($_GET);

  $variables['view_links'] = array(
				   array(
					 'title' => 'Grid view',
					 'href' => url("islandora/object/{$object->id}/pages", array('absolute' => TRUE)),
					 'attributes' => array(
							       'class' => "islandora-view-grid $grid_active",
							       ),
					 'query' => $query_params + array('display' => 'grid'),
					 ),
				   array(
					 'title' => 'List view',
					 'href' => url("islandora/object/{$object->id}/pages", array('absolute' => TRUE)),
					 'attributes' => array(
							       'class' => "islandora-view-list $list_active",
							       ),
					 'query' => $query_params + array('display' => 'list'),
					 ),
				   );
}

define('BOOTSTRAP_DSS_DIGITAL_BREADCRUMBS_MAX', 52);

// "Home/Japanese Imperial House Postcard Album/Search"
//define('BOOTSTRAP_DSS_DIGITAL_BREADCRUMBS_MAX', 41);

function bootstrap_dss_lebanesetown_breadcrumb($variables) {

  if(array_key_exists(2, $variables)) {

    if(array_key_exists('map', $variables[2])) {

      if(array_key_exists(2, $variables[2]['map'])) {

	$object = $variables[2]['map'][2];
	//$parent_pids = $object->getParents();
      }
    }
  }

  $output = '<ul class="breadcrumb">';

  // Work-around
  if(array_key_exists('breadcrumb', $variables)) {

    unset($variables['breadcrumb']);
  }

  $breadcrumbs = $variables;
  $count = count(array_keys($variables)) - 1;

  // For the truncation of individual breadcrumbs
  $breadcrumbs_length = 0;

  $path = current_path();
  $path_segments = explode('/', $path);

  $_breadcrumbs = $breadcrumbs;

  /*
					      'Marquis de Lafayette Prints Collection' => array(
												'dc.description',
												'dc.format',
												'dc.identifier',
												'dc.rights',
												'dc.subject',
												'dc.type'
												),
					      'John S. Shelton Earth Science Image Collection' => array('dc.contributor',
   */

  $searched_collection;
  $faceted_collection;

  if(array_key_exists('q', $_GET)) {

    $solr_query = $_GET['q'];
    $facets = array();
    foreach($_GET as $param_key => $param_value) {

      if($param_key != 'q' && $param_key == 'f') {

	//$facets[] = array($param_key => $param_value);
	foreach($param_value as $facet) {

	  $facet_split = explode(':', $facet);
	  //$facet_field = $facet_split[0];
	  $facet_field = array_shift($facet_split);
	  $facet_value = implode(':', $facet_split);
	  //$facets[$facet_field] = $facet_value;

	  if(!array_key_exists($facet_field, $facets) and preg_match('/"(.+?)"/', $facet_value, $facet_value_match)) {

	    $facets[$facet_field] = $facet_value_match[1];
	  }
	}
      }
    }

    $eastasia_subcollections = array(
				     'Japanese Imperial House Postcard Album',
				     'T.W. Ingersoll Co. Stereoviews of the Siege of Port Arthur',
				     'Imperial Postcard Collection',
				     'Tsubokura Russo-Japanese War Postcard Album',
				     'Sino-Japanese War Postcard Album 01',
				     'Sino-Japanese War Postcard Album 02',
				     'Lin Chia-Feng Family Postcard Collection',
				     'Japanese History Study Cards',
				     'Pacific War Postcard Collection',
				     'Michael Lewis Taiwan Postcard Collection',
				     'Gerald & Rella Warner Taiwan Postcard Collection',
				     'Gerald & Rella Warner Dutch East Indies Negative Collection',
				     'Japanese Imperial House Postcard Album',
				     'Gerald & Rella Warner Manchuria Negative Collection',
				     'Gerald & Rella Warner Taiwan Negative Collection',
				     'Gerald & Rella Warner Japan Slide Collection',
				     'Gerald & Rella Warner Souvenirs of Beijing and Tokyo',
				     'Woodsworth Taiwan Image Collection',
				     'Scenic Taiwan',
				     'Taiwan Photographic Monthly',
				     );

    /**
     * Work-around for linking Page Nodes to Islandora Collections
     * @todo Refactor
     *
     */

    $collection_node_map = array(
				 'East Asia Image Collections' => 'node/26',
				 'East Asia Image Collection' => 'node/26',
				 'Easton Library Company' => 'node/30',
				 'Experimental Printmaking Institute Collection' => 'node/31',
				 'Geology Department Slide Collection' => 'node/19',
				 'Historical Photograph Collection' => 'node/20',
				 'Lafayette Newspaper Collection' => 'node/21',
				 'Marquis de Lafayette Prints Collection' => 'node/27',
				 'Silk Road Instrument Database' => 'node/32',
				 'Swift Poems Project' => 'node/33',
				 'Visual Resources Collection' => 'node/34',
				 'McKelvy House Photograph Collection' => 'node/42',
				 'Lafayette World War II Casualties' => 'node/43',
				 'Presidents of Lafayette College' => 'node/41',
				 );

    $collection_elements = array();

    if(isset($object) and isset($object['MODS'])) {

      //$this->registerXPathNamespace("xml", "http://www.w3.org/XML/1998/namespace");
      //$this->registerXPathNamespace("mods", "http://www.loc.gov/mods/v3"); //http://www.loc.gov/mods/v3
      //$relation_is_part_of_value = (string) array_shift($this->xpath("./mods:note[@type='admin']"));

      try {

	$mods_doc = new SimpleXMLElement($object['MODS']->content);
	$mods_doc->registerXPathNamespace("xml", "http://www.w3.org/XML/1998/namespace");
	$mods_doc->registerXPathNamespace("mods", "http://www.loc.gov/mods/v3"); //http://www.loc.gov/mods/v3

	/**
	 * Just use the top-level collection element
	 *
	 */
	//$collection_elements = array_merge($collection_elements, array_map($map, $mods_doc->xpath("./mods:note[@type='admin']")));
	$collection_elements = array_merge($collection_elements, array(array('cdm.Relation.IsPartOf' => array_shift($mods_doc->xpath("./mods:note[@type='admin']")))));

	// For MDL
	$map = function($element) {

	  return array('mdl_prints.description.series' => $element);
	};
	$collection_elements = array_merge($collection_elements, array_map($map, $mods_doc->xpath("./mods:note[@type='series']")));

      } catch (Exception $e) {

	drupal_set_message(t('Error parsing the MODS metadata for the object %s %t', array('%s' => $object->id, '%t' => $e->getMessage())), 'error', FALSE);
      }

      unset($_breadcrumbs[count($_breadcrumbs) - 1]);
      $_breadcrumbs[count($breadcrumbs) - 2] = array('title' => 'Collections', 'href' => 'collections');

      $map = function($element) {

	return array('cdm.Relation.IsPartOf' => $element);
      };

      if(!empty($collection_elements)) {

	$top_collection = (string) $collection_elements[0]['cdm.Relation.IsPartOf'];

	if(array_key_exists($top_collection, $collection_node_map)) {

	  $_breadcrumbs[] = array('title' => $top_collection, 'href' => $collection_node_map[$top_collection]);
	}
	$count++;

	//$facet_params = '?';
	//for($i=0; $i<$count($collection_elements); $i++) {
	$facet_params = array();

	$i=0;
	//foreach($collection_elements as $collection_facet => $facet_value) {
	foreach($collection_elements as $collection_facet => $facets) {

	  //$facet_params .= "f[{$i}]=" . $collection_facet . '"' . $facet_value . '"';
	  //'cdm.Relation.IsPartOf' . ':"' . (string) $collection_elements[$i] . '"';

	  foreach($facets as $facet => $facet_value) {

	    /*
	      $facet_params .= "f[{$i}]=" . $facet . ':"' . $facet_value . '"';
	      //if($i < count($facet_params - 1)) {
	      if($i < count($collection_elements) - 1) {

	      $facet_params .= '&';
	      }
	    */

	    $facet_params["f[{$i}]"] = $facet . ':"' . $facet_value . '"';
	    $i++;
	  }
	}

	$_breadcrumbs[] = array('title' => 'Browse', 'href' => 'islandora/search/*:*', 'options' => array('query' => $facet_params));

	//dpm(  url('islandora/search/*:*', array('query' => $facet_params)));
	//$_breadcrumbs[] = array('title' => 'Browse', 'href' => url('islandora/search/*:*', array('query' => $facet_params)));
	$count++;

	/*
	  foreach($collection_elements as $collection_element) {

	  $collection_content = (string) $collection_element;
	  $_breadcrumbs[] = array('title' => 'Browse', 'href' => '/islandora/search/*:*?f[0]=cdm.Relation.IsPartOf:"' . $collection_content . '"');
	  //dpm($collection_content);

	  $count++;
	  }
	*/
      }

      // Accessing via Search This Collection: Home / [collection name] / Search
      //if(preg_match('/cdm\.Relation\.IsPartOf\:"(.+?)"/', $solr_query, $m)) {
    } elseif(preg_match('/cdm\.Relation\.IsPartOf\:"(.+?)"/', $solr_query, $m)) {

      $title = $m[1];

      if(in_array($title, $eastasia_subcollections)) {

	//$_breadcrumbs[count($breadcrumbs) - 1] = array('title' => 'East Asia Image Collection', 'href' => '/islandora/search/cdm.Relation.IsPartOf:"East Asia Image Collection"');
	//$_breadcrumbs[] = array('title' => $title, 'href' => '/islandora/search/cdm.Relation.IsPartOf:"' . $title . '"');
	//$count++;

	//$_breadcrumbs[count($breadcrumbs) - 1] = array('title' => $title, 'href' => '/islandora/search/cdm.Relation.IsPartOf:"' . $title . '"');
	$_breadcrumbs[count($breadcrumbs) - 1] = array('title' => "East Asia Image Collection", 'href' => '/islandora/search/cdm.Relation.IsPartOf:"East Asia Image Collection"');
      } else {

	//$_breadcrumbs[count($breadcrumbs) - 1] = array('title' => $title, 'href' => '/islandora/search/cdm.Relation.IsPartOf:"' . $title . '"');
	$_breadcrumbs[count($breadcrumbs) - 1] = array('title' => "East Asia Image Collection", 'href' => '/islandora/search/cdm.Relation.IsPartOf:"East Asia Image Collection"');
      }
      
      $_breadcrumbs[] = array('title' => 'Search', 'href' => current_path());
      $count++;

    } else if(array_key_exists('mdl_prints.description.series', $facets)) { // Home / Collections / [collection name] / [MDL series name] / Search

      //dpm($_breadcrumbs);
      $_breadcrumbs[count($breadcrumbs) - 1] = array('title' => 'Collections', 'href' => 'collections');
      //$_breadcrumbs[] = array('title' => 'Collections', 'href' => 'collections');
      $_breadcrumbs[] = array('title' => $facets['cdm.Relation.IsPartOf'], 'href' => $collection_node_map[$facets['cdm.Relation.IsPartOf']]);

      //$_breadcrumbs[] = array('title' => $facets['mdl_prints.description.series'], 'href' => $solr_query . '?f[0]=cdm.Relation.IsPartOf:"' . $facets['cdm.Relation.IsPartOf'] . '"' . '?f[1]=mdl_prints.description.series:"' . $facets['mdl_prints.description.series'] . '"');
      //$_breadcrumbs[] = array('title' => 'Browse', 'href' => $solr_query . '?f[0]=cdm.Relation.IsPartOf:"' . $facets['cdm.Relation.IsPartOf'] . '"');
      //$_breadcrumbs[] = array('title' => 'Browse', 'href' => $solr_query . '?f[0]=cdm.Relation.IsPartOf:"' . $facets['cdm.Relation.IsPartOf'] . '"' . '?f[1]=mdl_prints.description.series:"' . $facets['mdl_prints.description.series'] . '"');
      $_breadcrumbs[] = array('title' => 'Browse', 'href' => $solr_query, 'options' => array('query' => array('f[0]' => 'cdm.Relation.IsPartOf:"' . $facets['cdm.Relation.IsPartOf'] . '"',
													      'f[1]' => 'mdl_prints.description.series:"' . $facets['mdl_prints.description.series'] . '"')));

      $count += 2;

    } else if(array_key_exists('cdm.Relation.IsPartOf', $facets)) { // Home / Collections / [collection name] / Browse

      $_breadcrumbs[count($breadcrumbs) - 1] = array('title' => 'Collections', 'href' => 'collections');
      //$_breadcrumbs[] = array('title' => 'Collections', 'href' => '/collections');

      // Hierarchical collections
      if(in_array($facets['cdm.Relation.IsPartOf'], $eastasia_subcollections)) {

	//$_breadcrumbs[count($breadcrumbs) - 1] = array('title' => 'East Asia Image Collection', 'href' => '/islandora/search/' . $solr_query . '?f[0]=cdm.Relation.IsPartOf:"East Asia Image Collection"');
	//$_breadcrumbs[] = array('title' => $facets['cdm.Relation.IsPartOf'], 'href' => '/islandora/search/' . $solr_query . '?f[1]=cdm.Relation.IsPartOf:"' . $facets['cdm.Relation.IsPartOf'] . '"');
	//$_breadcrumbs[] = array('title' => $facets['cdm.Relation.IsPartOf'], 'href' => '/islandora/search/' . $solr_query . '?f[0]=cdm.Relation.IsPartOf:"' . $facets['cdm.Relation.IsPartOf'] . '"');
	//$count++;

	//$_breadcrumbs[] = array('title' => "East Asia Image Collection", 'href' => '/islandora/search/' . $solr_query . '?f[0]=cdm.Relation.IsPartOf:"East Asia Image Collection"');

	$_breadcrumbs[] = array('title' => "East Asia Image Collection", 'href' => '/islandora/search/' . $solr_query, 'options' => array('query' => array('f[0]' => 'cdm.Relation.IsPartOf:"East Asia Image Collection"'
																			   )));
      } else {

	//$_breadcrumbs[] = array('title' => $facets['cdm.Relation.IsPartOf'], 'href' => '/islandora/search/' . $solr_query . '?f[0]=cdm.Relation.IsPartOf:"' . $facets['cdm.Relation.IsPartOf'] . '"');
	//$_breadcrumbs[] = array('title' => $facets['cdm.Relation.IsPartOf'], 'href' => '/islandora/search/' . $solr_query . '?f[0]=cdm.Relation.IsPartOf:"' . $facets['cdm.Relation.IsPartOf'] . '"');

	//$_breadcrumbs[] = array('title' => "East Asia Image Collection", 'href' => '/islandora/search/' . $solr_query . '?f[0]=cdm.Relation.IsPartOf:"East Asia Image Collection"');
	//$_breadcrumbs[] = array('title' => $facets['cdm.Relation.IsPartOf'], 'href' => $solr_query . '?f[0]=cdm.Relation.IsPartOf:"East Asia Image Collection"');
	
	$_breadcrumbs[] = array('title' => $facets['cdm.Relation.IsPartOf'], 'href' => $collection_node_map[$facets['cdm.Relation.IsPartOf']]);
      }

      //$_breadcrumbs[] = array('title' => 'Browse', 'href' => $solr_query . '?f[0]=cdm.Relation.IsPartOf:"' . $facets['cdm.Relation.IsPartOf'] . '"');
      $_breadcrumbs[] = array('title' => 'Browse', 'href' => $solr_query, 'options' => array('query' => array('f[0]' => 'cdm.Relation.IsPartOf:"' . $facets['cdm.Relation.IsPartOf'] . '"'
													      )));
      $count += 2;

    } else { // Home / Search

      switch($solr_query) {

      case 'node/2':

	$_breadcrumbs[count($breadcrumbs) - 1]['title'] = t('Copyright & Use');
	break;

      case 'node/3':

	$_breadcrumbs[count($breadcrumbs) - 1]['title'] = 'Services';
	break;

      case 'node/4':

	$_breadcrumbs[count($breadcrumbs) - 1]['title'] = 'Repositories';
	break;

      case 'node/9':

	$_breadcrumbs[count($breadcrumbs) - 1]['title'] = 'Contact DSS';
	break;

      case 'node/11':

	$_breadcrumbs[count($breadcrumbs) - 1]['title'] = 'People';
	break;

      case 'node/45':

	$_breadcrumbs[count($breadcrumbs) - 1]['title'] = 'Collections';
	break;

      case 'islandora/object/gis:glendale7':

	$_breadcrumbs[count($breadcrumbs) - 1]['title'] = 'Glendale County Districts';
	break;

      default:

	$_breadcrumbs[count($breadcrumbs) - 1]['title'] = 'Search';
	break;
      }
    }
  }

  if(isset($breadcrumbs[count($breadcrumbs) - 1])) {

    if(preg_match('/search\/node/', $breadcrumbs[count($breadcrumbs) - 1]['href'])) {

      /**
       * For apachesolr search queries
       * This resolves DSSSM-651
       *
       */
      $_breadcrumbs = array_slice($breadcrumbs, 0, 2);
      $count = 1;
    } else {

      switch($breadcrumbs[count($breadcrumbs) - 1]['href']) {
	
      case 'islandora/object/islandora:root':
	
	$_breadcrumbs = array($breadcrumbs[0], $breadcrumbs[count($breadcrumbs) - 1]);
	$count--;
	break;
	
      case 'islandora/object/islandora:eastAsia':
      case 'islandora/object/islandora:newspaper':
      case 'islandora/object/islandora:academicPublications':
      case 'islandora/object/islandora:administrativeArchive':
      case 'islandora/object/islandora:cap':
      case 'islandora/object/islandora:mdl':
      case 'islandora/object/islandora:geologySlidesEsi':
      case 'islandora/object/islandora:mckelvyHouse':
      case 'islandora/object/islandora:warCasualties':
      case 'islandora/object/islandora:presidents':

	$_breadcrumbs = array_merge(array_slice($breadcrumbs, 0, -1), array(array('title' => 'Digital Collections',
										'href' => 'islandora/object/islandora:root')), array_slice($breadcrumbs, -1));
      $count++;
      break;
    
      case 'node/1':
    
	$_breadcrumbs = array_merge(array_slice($breadcrumbs, 0, -1));
	$count--;
	break;

      case 'node/26':
      case 'node/30':
      case 'node/31':
      case 'node/19':
      case 'node/20':
      case 'node/21':
      case 'node/27':
      case 'node/32':
      case 'node/33':
      case 'node/34':
      case 'node/42':
      case 'node/43':

	$_breadcrumbs = array_merge(array_slice($breadcrumbs, 0, -1), array(array('title' => 'Collections',
										'href' => 'node/45')), array_slice($breadcrumbs, -1));
      $count++;
      break;

      case 'node/29':

	$_breadcrumbs = array_merge(array_slice($breadcrumbs, 0, -1), array(array('title' => 'Repositories',
										'href' => 'node/4')), array_slice($breadcrumbs, -1));
	$count++;
      
	break;

      }
    }

    $breadcrumbs = $_breadcrumbs;
    
    $i = 1;
    foreach($breadcrumbs as $key => $breadcrumb) {
      
      if(isset($breadcrumb['href'])) {
	
	$breadcrumbs_length += strlen($breadcrumb['title']);
	
	if($breadcrumbs_length > BOOTSTRAP_DSS_DIGITAL_BREADCRUMBS_MAX) {
	  
	  if($key != count($breadcrumbs) - 1) {
	    
	    $breadcrumbs[$i]['title'] = '…';
	    $breadcrumbs_length -= strlen($breadcrumb['title']) - 1;
	    
	    $i++;
	  }
	}
      }
    }

    foreach($breadcrumbs as $key => $breadcrumb) {
      
      if(isset($breadcrumb['href'])) {
	
	if(!isset($breadcrumb['options'])) {
	  
	  $breadcrumb['options'] = array();
	}
	
	if ($count != $key) {

	  $output .= '<li>' . l($breadcrumb['title'], $breadcrumb['href'], $breadcrumb['options']) . '<span class="divider">/</span></li>';
	} else {
	  
	  $output .= '<li>' . l($breadcrumb['title'], $breadcrumb['href'], $breadcrumb['options']) . '</li>';
	}
      }
    }
    
    $output .= '</ul>';
    return $output;
  }
}

/**
 * Theme hooks
 *
 * Implements hook_theme().
 */
function bootstrap_dss_lebanesetown_theme($existing, $type, $theme, $path) {

  /*
  return array(
	       'islandora_discovery_controls' => array(
						       'file' => 'theme/theme.inc',
						       'template' => 'theme/islandora-basic-collection-yohe',
						       'pattern' => 'islandora_basic_collection_yohe__',
						       'variables' => array('islandora_object' => NULL, 'collection_results' => NULL),
						       ),
	       );
  */

  return array();
}
