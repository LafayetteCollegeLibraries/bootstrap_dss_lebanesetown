<?php

  /**
   * @file
   * Template for the Islandora advanced search form
   *
   */

  /* All values for this form are passed in the array stored in "$form"
     This is specified within the implementation of hook_theme(), specifically within the array element 'render element'
     Please note that the contents of $form are, in contrast, modified within the implementation of hook_form_alter()
  */
?>

<div id="islandora-advanced-search-inner-wrapper">

    <?php print drupal_render($form['terms']); ?>
    <?php print drupal_render($form['controls']); ?>
    <?php print drupal_render($form['submit']); ?>

    <?php print drupal_render_children($form); ?>
</div>
