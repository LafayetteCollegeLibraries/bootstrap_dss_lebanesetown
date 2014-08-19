<?php
/**
 * @file
 *
 */
?>
<div class="book-page-wrapper">
<?php
  print l(t('Return to Book View'), "islandora/object/{$book_object_id}");
?>
<?php if (isset($viewer)): ?>
  <div id="book-page-viewer">
    <?php print $viewer; ?>
  </div>
<?php else: ?>
  <div id="book-page-image">
    <?php
      $params = array(
        'path' => url("islandora/object/{$object->id}/datastream/JPG/view"),
        'attributes' => array()
      );
      print theme_image($params);
    ?>
  </div>
<?php endif; ?>

  <div class="islandora-page-metadata-wrapper">
  <fieldset class="islandora-page-metadata">
  <legend><span class="fieldset-legend"><?php print t('Extended details'); ?></span></legend>
    <div class="fieldset-wrapper">
      <dl class="islandora-inline-metadata islandora-page-fields">
        <?php $row_field = 0; ?>

        <?php if (isset($mods_object)): ?>

          <?php foreach ($mods_object as $key => $value): ?>

	    <!--
	      @griffinj
	      Handling duplicate field labels for elements with multiple values
	    -->
	    <?php if($row_field == 0 || $value['label'] != $mods_object[$row_field - 1]['label'] ): ?>

              <dt class="<?php print $value['class']; ?><?php print $row_field == 0 ? ' first' : ''; ?>">
                <?php print $value['label']; ?>
              </dt>
	    <?php endif; ?>
            <dd class="<?php print $value['class']; ?><?php print $row_field == 0 ? ' first' : ''; ?>">

              <?php if( array_key_exists('date_value', $value)): ?>

		<?php print array_key_exists('facet', $value) ? l($value['date_value'], "islandora/search/eastasia." . $value['facet'] . "%3A" . $value['facet_value']) : $value['date_value']; ?>
	      <?php else: ?>

                <?php print array_key_exists('facet', $value) ? l($value['value'], "islandora/search/eastasia." . $value['facet'] . "%3A" . $value['facet_value']) : $value['value']; ?>
	      <?php endif; ?>
            </dd>
          <?php $row_field++; ?>
        <?php endforeach; ?>
        <?php endif; ?>

      </dl>
    </div>
  </fieldset>
  </div><!-- /.islandora-page-metadata-wrapper -->
</div><!-- /.book-page-wrapper -->