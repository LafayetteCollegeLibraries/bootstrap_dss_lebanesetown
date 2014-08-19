<?php

/**
 * @file
 * Theming for menu trees, menu items, and menu links
 */

function bootstrap_dss_lebanesetown_menu_link__main_menu(&$variables) {

  $element = $variables['element'];
  $sub_menu = '';

  if ($element['#below']) {

    // Prevent dropdown functions from being added to management menu as to not affect navbar module.
    if (($element['#original_link']['menu_name'] == 'management') && (module_exists('navbar'))) {
      $sub_menu = drupal_render($element['#below']);
    }

    else {
      // Add our own wrapper
      unset($element['#below']['#theme_wrappers']);
      $sub_menu = '<ul class="dropdown-menu">' . drupal_render($element['#below']) . '</ul>';

      $element['#localized_options']['attributes']['class'][] = 'dropdown-toggle';
      $element['#localized_options']['attributes']['data-toggle'] = 'dropdown';

      // Check if this element is nested within another
      if ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] > 1)) {

        // Generate as dropdown submenu
        $element['#attributes']['class'][] = 'dropdown-submenu';
	unset($element['#localized_options']['attributes']['data-toggle']);
      }
      else {
        // Generate as standard dropdown
        $element['#attributes']['class'][] = 'dropdown';
        $element['#localized_options']['html'] = TRUE;
        $element['#title'] .= ' <span class="caret"></span>';
      }

      // Set dropdown trigger element to # to prevent inadvertant page loading with submenu click
      $element['#localized_options']['attributes']['data-target'] = '#';
    }
  }

  // Issue #1896674 - On primary navigation menu, class 'active' is not set on active menu item.
  // @see http://drupal.org/node/1896674
  if (($element['#href'] == $_GET['q'] || ($element['#href'] == '<front>' && drupal_is_front_page())) && (empty($element['#localized_options']['language']) || $element['#localized_options']['language']->language == $language_url->language)) {

    /**
     * @author griffinj
     * This is not desirable for the theme implementation
     *
     */
    //$element['#attributes']['class'][] = 'active';
  }

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);

  // Ensure that buttons are only created for the top-level lists
  if(empty($element['#original_link']['depth']) ||
     (empty($element['#below']) && $element['#original_link']['depth'] == '1') || $element['#original_link']['plid'] == '0') {
    
    $element['#attributes']['class'][] = 'btn';
  }
 
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
  }

function bootstrap_dss_lebanesetown_menu_tree__menu_footer_menu(&$variables) {

  //dpm($variables);

  return theme_menu_tree($variables);
}

/**
 * Implements theme_menu_link__MENU_NAME
 *
 * @see https://drupal.org/node/345624
 */
function bootstrap_dss_lebanesetown_menu_link__menu_footer_menu(&$variables) {

  $variables['element']['#attributes']['class'][] = 'footer-menu-item';

  // CSS for "piping" effect
  // HTML for semantically encoded content
  /*
  if(!in_array('last', $variables['element']['#attributes']['class'])) {

    $variables['element']['#below']['#markup'] = '<span class="footer-menu-divider">|</span>';
  }
  */

  return theme_menu_link($variables);
}

/**
 * Implements theme_menu_link__MENU_NAME
 *
 * @see https://drupal.org/node/345624
 * For handling CAS menu styling (custom menu added as a Drupal administrator)
 *
 */
function bootstrap_dss_lebanesetown_menu_link__menu_authentication_cas(&$variables) {

  //$element = $variables['element'];

  // $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  /*
  $variables['element']['#children'] = array('#type' => 'markup',
					     '#markup' => '<span>ICON</span><span>Lafayette College</span>');
  */
  $variables['element']['#title'] = '<span title="Lafayette College" class="hybridauth-icon hybridauth-icon-hybridauth-32 hybridauth-icon-hybridauth-32-lafayette"></span><span>&nbsp;Lafayette College</span>';
  $variables['element']['#localized_options']['html'] = TRUE;

  dpm($variables);


  return theme_menu_link($variables);
}

/**
 * Implements template_preprocess_menu_tree()
 *
 */

/*
function bootstrap_dss_lebanesetown_preprocess_menu_link(&$variables) {

  dpm($variables);
}
*/
