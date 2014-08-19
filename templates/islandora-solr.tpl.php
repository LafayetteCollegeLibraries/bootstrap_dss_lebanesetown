<?php
/**
 * @file
 * Islandora solr search primary results template file.
 *
 * Variables available:
 * - $results: Primary profile results array
 *
 * @see template_preprocess_islandora_solr()
 */
?>

<?php if (array_key_exists('restricted_results', $variables) and $variables['restricted_results']): ?>

    <div class="islandora islandora-solr-search-alert">Items displayed include some with access restrictions.  Please log in.</div>

<?php endif; ?>

<div class="islandora islandora-solr-search-results">
<!-- For the list layout -->
<?php if($display == 'list'): ?>

    <?php if (empty($results)): ?>

       <div class="no-results"><?php print t('Sorry, but your search returned no results.'); ?></div>
    <?php else: ?>

    <ol class="islandora-solr-search-result-list" start="<?php print $elements['solr_start'] + 1; ?>">
    <?php

      $row_result = 0;
      foreach($results as $key => $result):
    ?>

        <!-- Search result -->
	<li class="islandora-solr-search-result clear-block <?php print $row_result % 2 == 0 ? 'odd' : 'even'; ?>">

	<?php if(array_key_exists('restricted', $result) AND $result['restricted']): ?>

	  <div class="solr-restricted"></div>
	<?php endif; ?>

	  <!-- Thumbnail -->
          <dl class="solr-thumb">
          <dt>
	    <?php if($result['restricted']): ?>

            <?php
		$image = "<img src='/" . drupal_get_path('theme', 'bootstrap_dss_lebanesetown') . "/files/BigLock.jpg' />";
            ?>

	    <?php else: ?>

            <?php
              $image = '<img src="' . url($result['thumbnail_url'], array('query' => $result['thumbnail_url_params'])) . '" />';
            ?>

	    <?php endif; ?>

<?php

              // Construct options array for l() function call.  Only include
              // what is needed.  Can accept standard url parameters and a
              // single anchor tag (fragment) at the end.
              $options = array(
                'html' => TRUE,
              );
              if (isset($result['object_url_params'])):
                $options['query'] = $result['object_url_params'];
              endif;
              if (isset($result['object_url_fragment'])):
                $options['fragment'] = $result['object_url_fragment'];
              endif;
              // Construct the thumbnail link.
              print l($image, $result['object_url'], $options);

?>

          </dt>
          <dd></dd>
        </dl>
        <!-- Metadata -->
        <dl class="solr-fields islandora-inline-metadata">
          <?php
            $row_field = 0;
            $max_rows = count($results[$row_result]) - 1;
            foreach($result['solr_doc'] as $key => $value): ?>
            <dt class="solr-label
              <?php
                print $value['class'];
                print $row_field == 0 ? ' first' : '';
                print $row_field == $max_rows ? ' last' : '';
              ?>">
              <?php print $value['label']; ?>
            </dt>
            <?php
              if ($key == 'PID'):
                // Construct options array for l() function call.  Only include
                // what is needed.  Can accept standard url parameters and a
                // single anchor tag (fragment) at the end.
                $options = array(
                  'html' => TRUE,
                );
                if (isset($result['object_url_params'])):
                  $options['query'] = $result['object_url_params'];
                endif;
                if (isset($result['object_url_fragment'])):
                  $options['fragment'] = $result['object_url_fragment'];
                endif;
                // Construct the PID link.
                $value['value'] = l($value['value'], $result['object_url'], $options);
              endif;
            ?>
            <dd class="solr-value <?php print $value['class']; ?><?php print $row_field == 0 ? ' first' : ''; ?><?php print $row_field == $max_rows ? ' last' : ''; ?>">
		<?php
		if ($key == 'dc.title'):
		  // Construct options array for l() function call.  Only include
		  // what is needed.  Can accept standard url parameters and a
		  // single anchor tag (fragment) at the end.
		  $options = array(
				   'html' => TRUE,
				   );

                  if (isset($result['object_url_params'])):

		    $options['query'] = $result['object_url_params'];
                  endif;

                  if (isset($result['object_url_fragment'])):

		    $options['fragment'] = $result['object_url_fragment'];
                  endif;

                  // Construct the PID link.
                  //dpm($result['object_url']);

//dpm(drupal_get_path_alias('/' . $result['object_url']));
//dpm(drupal_get_normal_path('/' . $result['object_url']));

//dpm(drupal_get_path_alias('islandora/object/islandora:33654'));
//dpm(drupal_get_normal_path('islandora/object/islandora:33654'));

//dpm('trace: ' . drupal_lookup_path('source', 'collections/eastasia/imperial-postcards/ip0706'));
//dpm('trace2: ' . drupal_lookup_path('alias', 'islandora/object/islandora:33654'));
//dpm('trace3: ' . drupal_lookup_path('alias', 'islandora/object/islandora:22184'));

                  print l($value['value'], drupal_get_path_alias($result['object_url']), $options);

		else:

		  // @griffinj For formatting date time values
		  print preg_match('/(?:Lower|Upper)/', $value['label']) ? date('F, Y', strtotime($value['value'])) : $value['value'];

                endif;
                ?>
            </dd>
            <?php $row_field++; ?>
          <?php endforeach; ?>
        </dl>
      <!-- </div> --><!-- /.islandora-solr-search-result -->
    </li><!-- /.islandora-solr-search-result-item -->

    <?php endforeach; ?>
    </ol><!-- /.islandora-solr-search-result-list -->

  <?php endif; ?>
  <!-- For the grid layout -->
  <?php else: ?>

    <?php if (empty($results)): ?>

       <div class="no-results"><?php print t('Sorry, but your search returned no results.'); ?></div>
    <?php else: ?>

    <div class="islandora islandora-basic-collection">
    <div class="islandora-basic-collection-grid clearfix">
      <?php
        $row_result = 0;
        foreach($results as $key => $result):
      ?>

      <dl class="islandora-basic-collection-object">
        <dt class="islandora-basic-collection-thumb">

	  <?php if($result['restricted']): ?>

	  <?php
	      $image = "<img src='/" . drupal_get_path('theme', 'bootstrap_dss_lebanesetown') . "/files/BigLock.jpg' />";
          ?>

	  <?php else: ?>
<?php

                $image = '<img src="' . url($result['thumbnail_url'], array('query' => $result['thumbnail_url_params'])) . '" />';
?>
          <?php endif; ?>
	  <?php
                // Construct options array for l() function call.  Only include
                // what is needed.  Can accept standard url parameters and a
                // single anchor tag (fragment) at the end.
                $options = array('html' => TRUE,);
              if (isset($result['object_url_params'])):
                $options['query'] = $result['object_url_params'];
              endif;
              if (isset($result['object_url_fragment'])):
                $options['fragment'] = $result['object_url_fragment'];
              endif;

              // Construct the thumbnail link.
              print l($image, $result['object_url'], $options);
          ?>
        </dt>
        <dd class="islandora-basic-collection-caption <?php print $result['class']; ?>">

	      <?php

		  // Construct options array for l() function call.  Only include
		  // what is needed.  Can accept standard url parameters and a
		  // single anchor tag (fragment) at the end.
		  $options = array(
				   'html' => TRUE,
				   );

                  if (isset($result['object_url_params'])):

		    $options['query'] = $result['object_url_params'];
                  endif;

                  if (isset($result['object_url_fragment'])):

		    $options['fragment'] = $result['object_url_fragment'];
                  endif;

                  // Construct the PID link.
                  print l($result['solr_doc']['dc.title']['value'], $result['object_url'], $options);
              ?>
	</dd>

	<!-- Metadata fields for the purposes of sorting -->
        <?php foreach($result['solr_doc'] as $key => $value): ?>
          <dt class="grid-solr-label solr-label
              <?php
                print $value['class'];
                print $row_field == 0 ? ' first' : '';
                print $row_field == $max_rows ? ' last' : '';
              ?>">
              <?php print $value['label']; ?>
          </dt>
          <dd class="grid-solr-value solr-value <?php print $value['class']; ?>">

              <?php print preg_match('/(?:Lower|Upper)/', $value['label']) ? date('F, Y', strtotime($value['value'])) : $value['value']; ?>
          </dd>
        <?php endforeach; ?>
    </dl><!--/.islandora-basic-collection-object -->
  
  <?php

    $row_result++;
    endforeach;
  ?>
  </div><!-- /.islandora-basic-collection-grid -->
</div><!-- /.islandora-basic-collection-grid -->

<?php endif; ?><!-- Results -->
<?php endif; ?><!-- List/Grid view -->
</div><!-- /.islandora islandora-solr-search-results -->
