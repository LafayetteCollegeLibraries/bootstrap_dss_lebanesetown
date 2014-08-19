<?php

/**
 * @file
 * islandora-basic-collection-wrapper.tpl.php
 *
 * @TODO: needs documentation about file and variables
 */
?>

<div class="islandora-basic-collection-wrapper">
  
  <div class="islandora-basic-collection clearfix">

    <div class="islandora-solr-content content">

      <?php if(!empty($discovery_controls)): ?>

	<?php print $discovery_controls; ?>
      <?php endif; ?>	

      <?php print $collection_content; ?>
      <?php print $collection_pager; ?>

    </div><!-- /.islandora-solr-content -->
  </div><!-- /.islandora-basic-collection -->
</div><!-- /.islandora-basic-collection-wrapper -->
