<?php

/**
 * @file
 * Modified theming for the search results
 * @author griffinj@lafayette.edu
 * See search-result.tpl.php within the core search Module
 *
 * @see template_preprocess()
 * @see template_preprocess_search_result()
 * @see template_process()
 *
 * @ingroup themeable
 */
?>
<li class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
  <h3 class="title"<?php print $title_attributes; ?>>
    <a href="<?php print $url; ?>"><?php print $title; ?></a>
  </h3>
  <?php print render($title_suffix); ?>
  <div class="search-snippet-info">
    <?php if ($snippet): ?>
      <p class="search-snippet"<?php print $content_attributes; ?>><?php print $snippet; ?></p>
    <?php endif; ?>
  </div>
</li>
