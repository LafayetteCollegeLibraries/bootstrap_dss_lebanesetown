<?php

  /**
   * @file Hook implementation and functionality for pagination
   * @author griffinj@lafayette.edu
   *
   */

/**
 * Implements theme_pager_first().
 * @todo Refactor into a hook_process_HOOK implementation
 */
function bootstrap_dss_lebanesetown_pager_first($variables) {

  $text = $variables['text'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  global $pager_page_array;
  $output = '';

  // If we are anywhere but the first page
  if ($pager_page_array[$element] > 0) {
    
    $output = theme('pager_link', array('text' => $text, 'page_new' => pager_load_array(0, $element, $pager_page_array), 'element' => $element, 'parameters' => $parameters, 'attributes' => array('title' => 'Go to First Page')));
  }

  return $output;
  }

/**
 * Implements theme_pager_previous().
 *
 */
function bootstrap_dss_lebanesetown_pager_previous($variables) {

  $text = $variables['text'];
  $element = $variables['element'];
  $interval = $variables['interval'];
  $parameters = $variables['parameters'];

  // Implementing for the @title attribute
  if(array_key_exists('attributes', $variables)) {

    $attributes = $variables['attributes'];
  } else {

    $attributes = array();
  }

  /**
   * @author griffinj
   * This permits one to override the title value based upon the interval passed during the function invocation
   *
   */
  switch($interval) {

  case 1:

    $attributes['title'] = "Back 1 Page";
    break;
  case 10:

    $attributes['title'] = "Back $interval Pages";
    break;
  default:

    unset($attributes['title']);
    break;
  }

  global $pager_page_array;

  $output = '';

  // If we are anywhere but the first page
  if (($interval == 10 and $pager_page_array[$element] > 9) or ($interval <= 3 and $pager_page_array[$element] > 0)) {

    $page_new = pager_load_array($pager_page_array[$element] - $interval, $element, $pager_page_array);

    // If the previous page is the first page, mark the link as such.
    if ($page_new[$element] == 0) {

      $output = theme('pager_first', array('text' => $text, 'element' => $element, 'parameters' => $parameters, 'attributes' => $attributes));
    } else { // The previous page is not the first page.

      $output = theme('pager_link', array('text' => $text, 'page_new' => $page_new, 'element' => $element, 'parameters' => $parameters, 'attributes' => $attributes));
    }
  }

  return $output;
}

/**
 * Implements theme_pager_next().
 *
 */
function bootstrap_dss_lebanesetown_pager_next($variables) {

  $text = $variables['text'];
  $element = $variables['element'];
  $interval = $variables['interval'];
  $parameters = $variables['parameters'];

  // Implementing for the @title attribute
  if(array_key_exists('attributes', $variables)) {

    $attributes = $variables['attributes'];
  } else {

    $attributes = array();
  }
  switch($interval) {

  case 1:

    $attributes['title'] = "Forward 1 Page";
    break;
  case 10:

    $attributes['title'] = "Forward $interval Pages";
    break;
  default:

    unset($attributes['title']);
    break;
  }

  global $pager_page_array, $pager_total;

  $output = '';

  // If we are anywhere but the last page (for an interval of 1)
  if (($interval == 10 and $pager_page_array[$element] < ($pager_total[$element] - 10)) or ($interval <= 3 and $pager_page_array[$element] < ($pager_total[$element] - 1))) {

    $page_new = pager_load_array($pager_page_array[$element] + $interval, $element, $pager_page_array);
    // If the next page is the last page, mark the link as such.
    if ($page_new[$element] == ($pager_total[$element] - 1)) {

      $output = theme('pager_last', array('text' => $text, 'element' => $element, 'parameters' => $parameters, 'attributes' => $attributes));
    } else {   // The next page is not the last page.

      $output = theme('pager_link', array('text' => $text, 'page_new' => $page_new, 'element' => $element, 'parameters' => $parameters, 'attributes' => $attributes));
    }
  }

  return $output;
}

/**
 * Implements theme_pager_last().
 * @todo Refactor into a hook_process_HOOK implementation
 */
function bootstrap_dss_lebanesetown_pager_last($variables) {

  $text = $variables['text'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  global $pager_page_array, $pager_total;
  $output = '';

  // If we are anywhere but the last page
  if ($pager_page_array[$element] < ($pager_total[$element] - 1)) {  

    return theme('pager_link', array('text' => $text, 'page_new' => pager_load_array($pager_total[$element] - 1, $element, $pager_page_array), 'element' => $element, 'parameters' => $parameters, 'attributes' => array('title' => 'Go to Last Page')));
  }

  return $output;
}

  /**
   * Returns HTML for a query pager.
   *
   * @see bootstrap_pager()
   *
   * @param $vars
   *   An associative array containing:
   *   - tags: An array of labels for the controls in the pager.
   *   - element: An optional integer to distinguish between multiple pagers on
   *     one page.
   *   - parameters: An associative array of query string parameters to append to
   *     the pager links.
   *   - quantity: The number of pages in the list.
   */
function bootstrap_dss_lebanesetown_pager($variables) {

  $output = "";
  $tags = $variables['tags'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $quantity = $variables['quantity'];

  global $pager_page_array, $pager_total;

  // Calculate various markers within this pager piece:
  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);
  // current is the page we are currently paged to
  $pager_current = $pager_page_array[$element] + 1;
  // first is the first page listed by this pager piece (re quantity)
  $pager_first = $pager_current - $pager_middle + 1;
  // last is the last page listed by this pager piece (re quantity)
  $pager_last = $pager_current + $quantity - $pager_middle;
  // max is the maximum page number
  $pager_max = $pager_total[$element];
  // End of marker calculations.

  // Prepare for generation loop.
  $i = $pager_first;
  if ($pager_last > $pager_max) {
    // Adjust "center" if at end of query.
    $i = $i + ($pager_max - $pager_last);
    $pager_last = $pager_max;
  }
  if ($i <= 0) {
    // Adjust "center" if at start of query.
    $pager_last = $pager_last + (1 - $i);
    $i = 1;
  }


  /**
   * Interval of 1
   *
   */

  // End of generation loop preparation.
  // First
  $li_first = theme('pager_first', array('text' => (isset($tags[0]) ? $tags[0] : t('|<')), 'element' => $element, 'parameters' => $parameters));
  // Interval of 10
  $li_previous_set = theme('pager_previous', array('text' => (isset($tags[1]) ? $tags[1] : t('<<')), 'element' => $element, 'interval' => 10, 'parameters' => $parameters));
  // Interval of 1
  $li_previous = theme('pager_previous', array('text' => (isset($tags[1]) ? $tags[1] : t('<')), 'element' => $element, 'interval' => 1, 'parameters' => $parameters));

  // Interval of 1
  $li_next = theme('pager_next', array('text' => (isset($tags[3]) ? $tags[3] : t('>')), 'element' => $element, 'interval' => 1, 'parameters' => $parameters));
  // Interval of 10
  $li_next_set = theme('pager_next', array('text' => (isset($tags[3]) ? $tags[3] : t('>>')), 'element' => $element, 'interval' => 10, 'parameters' => $parameters));
  $li_last = theme('pager_last', array('text' => (isset($tags[4]) ? $tags[4] : t('>|')), 'element' => $element, 'parameters' => $parameters));

  if ($pager_total[$element] > 1) {

    if ($li_first) {
      $items[] = array(
        'class' => array('pager-first'),
        'data' => $li_first,
      );
    }

    if ($li_previous_set) {
      $items[] = array(
		       'class' => array('prev'),
		       'data' => $li_previous_set,
		       );
    }

    if ($li_previous) {
      $items[] = array(
		       'class' => array('prev'),
		       'data' => $li_previous,
		       );
      /*
    } else {

      $items[] = array('class' => array('disabled'),
		       'data' => '<span>' . (isset($tags[1]) ? $tags[1] : t('<< Prev')) . '</span>'
		       );
    }
      */
    }

    // When there is more than one page, create the pager list.
    if ($i != $pager_max) {

      // Prepend the ellipsis
      if ($i > 1) {
        $items[] = array(
          'class' => array('pager-ellipsis', 'disabled'),
          'data' => '<span>…</span>',
        );
      }
      // Now generate the actual pager piece.
      for (; $i <= $pager_last && $i <= $pager_max; $i++) {

        if ($i < $pager_current) {
          $items[] = array('class' => array('pager-item'),
			   'data' => theme('pager_previous', array('text' => $i, 'element' => $element, 'interval' => ($pager_current - $i), 'parameters' => $parameters)),
			   );
        }

        if ($i == $pager_current) {
          $items[] = array(
			   //'class' => array('active'), // Add the active class
			   /**
			    * For styling the active pagination element
			    * This resolves DSSSM-513
			    *
			    */
			   'class' => array('pager-item', 'pager-current'), // Add the active class
			   'data' => l($i, '#', array('fragment' => '','external' => TRUE)),
			   );
        }

        if ($i > $pager_current) {

          $items[] = array(
            //'class' => array('pager-item'),
            'data' => theme('pager_next', array('text' => $i, 'element' => $element, 'interval' => ($i - $pager_current), 'parameters' => $parameters)),
          );
        }
      }

      if ($i < $pager_max) {
        $items[] = array(
          'class' => array('pager-ellipsis', 'disabled'),
          'data' => '<span>…</span>',
        );
      }
    }
    // End generation.

    if ($li_next) {

      $items[] = array(
        'class' => array('next'),
        'data' => $li_next,
      );
      /*
    } else {

      $items[] = array('class' => array('disabled'),
		       'data' => '<span>' . (isset($tags[1]) ? $tags[1] : t('Next >>')) . '</span>'
		       );
    }
      */
    }

    if ($li_next_set) {

      $items[] = array(
        'class' => array('next'),
        'data' => $li_next_set,
      );
    }

    if ($li_last) {
      $items[] = array(
        'class' => array('pager-last'),
        'data' => $li_last,
      );
    }

    return '<div class="pagination pagination-centered">'. theme('item_list', array(
      'items' => $items,
      //'attributes' => array('class' => array('pager')),
    )) . '</div>';
  }
  
  return $output;
}