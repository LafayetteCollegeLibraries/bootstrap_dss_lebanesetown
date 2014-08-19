<?php

  /**
   * @file
   * For theming the authentication region
   *
   */
?>

<?php if ($content): ?>

  <div class="lafayette-dss-modal" id="auth-modal" tabindex="-1" role="dialog" aria-labelledby="Authentication Modal" aria-hidden="true" data-backdrop="static">
    <div class="modal-dialog">
      <div class="modal-content">
        <div class="modal-header">
          <button type="button" class="close auth-modal-close" data-dismiss="modal" aria-hidden="true">Close</button>
	  <h4 class="modal-title">Sign in Via</h4>
        </div>
        <div class="modal-body">

          <?php print render($content); ?>

	  <button id="auth-modal-help" class="btn btn-default" data-content="Logging in with Lafayette College credentials can grant access to restricted content, while logging in through credentials from another site will enable social feature such as liking on Facebook." data-placement="bottom" data-toggle="popover" data-container="body" type="button" data-original-title="" title=""><i class="icon-large" title="Click for more information"></i></button>
        </div>
      </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
  </div><!-- /.modal -->
<?php endif; ?>
