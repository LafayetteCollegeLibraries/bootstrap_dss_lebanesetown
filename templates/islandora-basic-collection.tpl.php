<?php

/**
 * @file
 * islandora-basic-collection.tpl.php
 *
 * @TODO: needs documentation about file and variables
 */
?>

<div class="islandora islandora-basic-collection">

    <?php
      $row_result = 0;
      foreach($associated_objects_array as $associated_object): ?>

      <!-- Search result -->
      <div class="islandora-solr-search-result clear-block <?php print $row_result % 2 == 0 ? 'odd' : 'even'; ?>">
        <!-- Thumbnail -->
        <dl class="solr-thumb">
          <dt>
            <?php if (isset($associated_object['thumb_link'])): ?>
              <?php print $associated_object['thumb_link']; ?>
            <?php endif; ?>
          </dt>
          <dd></dd>
        </dl>
        <!-- Metadata -->
        <dl class="solr-fields islandora-inline-metadata">
          <?php
            $row_field = 0;
            //$max_rows = count($associated_objects_array[$row_result]) - 1;
            foreach($associated_object['dc_array'] as $key => $value): ?>

            <dt class="solr-label
              <?php
                print $value['class'];
                print $row_field == 0 ? ' first' : '';
              ?>">
              <?php print $value['label']; ?>
	    </dt>
	    <dd class="solr-value <?php print $value['class']; ?><?php print $row_field == 0 ? ' first' : ''; ?>">

		<!-- @griffinj For formatting date time values -->
		<?php print preg_match('/Date/', $value['label']) ? date('F, Y', strtotime($value['value'])) : $value['value'] ?>
            </dd>

            <?php $row_field++; ?>
          <?php endforeach; ?>
        </dl>
      </div>

    <?php $row_field++; ?>
    <?php endforeach; ?>
</div>
