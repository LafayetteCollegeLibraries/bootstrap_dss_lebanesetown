<header id="navbar" role="banner" class="navbar">

  <div class="navbar-header">

   <!-- Refactor into hook_preprocess_page -->
   <h2><?php print l('Skillman Library', 'http://library.lafayette.edu/'); ?> at <?php print l('Lafayette College', 'http://www.lafayette.edu/'); ?></h2>

   <?php if(!empty($title)): ?>
																			<h1><?php print l('Lebanese Town', '', array('absolute' => TRUE)); ?></h1>
   <?php endif; ?>
  </div>

  <div class="navbar-inner">

    <div class="logo-container logo pull-left">

      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">

        <?php print $dss_logo_image; ?>
      </a>
    </div>

    <!-- <div class="container navbar-inner-container"> -->
    <div class="navbar-inner-container">

      <?php if (!empty($site_name)): ?>
        <h1 id="site-name">
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" class="brand"><?php print $site_name; ?></a>
        </h1>
      <?php endif; ?>

      <?php if (!empty($primary_nav) || !empty($secondary_nav) || !empty($page['navigation'])): ?>

        <div class="nav-collapse collapse">
          <nav role="navigation">
            <?php if (!empty($primary_nav)): ?>
              <?php print render($primary_nav); ?>
            <?php endif; ?>

            <?php if (!empty($secondary_nav)): ?>
              <?php print render($secondary_nav); ?>
            <?php endif; ?>

            <?php if (!empty($page['navigation'])): ?>
              <?php print render($page['navigation']); ?>
            <?php endif; ?>
</div><!-- /.nav-collapse -->

          </nav><!-- /.navigation -->
        </div><!-- /.nav-collapse collapse -->
      <?php endif; ?>

   <div class="auth-share-simple-search-container">

     <?php if (!empty($page['simple_search'])): ?>

       <?php print render($page['simple_search']); ?>
     <?php endif; ?>
   </div><!-- /.auth-share-simple-search-container -->

  <div class="menu-toggle-container container">

    <?php if (isset($menu_toggle_container)): ?>

      <?php print $menu_toggle_container; ?>

      <div class="auth-share-container container">

        <?php print $search_container; ?>

        <?php print $share_container; ?>
        <?php print $auth_container; ?>

      </div><!-- /.auth-share-container -->



    <?php endif; ?>
  </div><!-- /.menu-toggle-container -->
</div><!-- /.navbar-inner -->

</header><! --/.navbar -->

<div class="panel-container">

   <?php if(isset($slide_drawers)): ?>
      <div class="drawers snap-drawers">

	 <div class="left-drawer snap-drawer snap-drawer-left">

	 <?php if (!empty($page['slide_panel'])): ?>
	   <div>
	     <?php print render($page['slide_panel']); ?>
           </div>
         <?php endif; ?>

	 </div><!--/.snap-drawer-left -->
      </div><!--/.snap-drawers -->
   <?php endif; ?>

<div id="content" class="snap-content">
<div class="main-container container">

  <header role="banner" id="page-header">
   <p class="lead"><?php print l($title, '<front>'); ?></p>

    <?php print render($page['header']); ?>
  </header> <!-- /#header -->

  <div class="row-fluid">

    <?php if (!empty($page['sidebar_first'])): ?>
      <aside class="span3" role="complementary">
        <?php print render($page['sidebar_first']); ?>
      </aside>  <!-- /#sidebar-first -->
    <?php endif; ?>

    <section class="<?php print _bootstrap_content_span($columns); ?>">
      <?php if (!empty($page['highlighted'])): ?>
        <div class="highlighted hero-unit"><?php print render($page['highlighted']); ?></div>
      <?php endif; ?>

    <div id="breadcrumb-container">
      <?php if (!empty($breadcrumb)): ?>

        <?php print $breadcrumb; ?>
      <?php endif;?>

      <div id="page-site-links-container">
      <!-- Work-around, hard-coding, refactor -->
      <div id="contact-container" class="breadcrumb" ><?php print $contact_anchor; ?></div>
      <div id="copyright-container" class="breadcrumb" ><?php print l('Copyright & Use', 'copyright'); ?></div>
      </div>
    </div>


      <a id="main-content"></a>
      <?php print render($title_prefix); ?>

      <?php print render($title_suffix); ?>
      <?php print $messages; ?>
      <?php if (!empty($tabs)): ?>
        <?php print render($tabs); ?>
      <?php endif; ?>
      <?php if (!empty($page['help'])): ?>
        <div class="well"><?php print render($page['help']); ?></div>
      <?php endif; ?>
      <?php if (!empty($action_links)): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>
      <?php print render($page['content']); ?>
    </section>

    <?php if (!empty($page['sidebar_second'])): ?>
      <aside class="span3" role="complementary">
        <?php print render($page['sidebar_second']); ?>
      </aside>  <!-- /#sidebar-second -->
    <?php endif; ?>

  </div><!-- /.row-fluid -->
</div>
</div>
</div>

<!--

  griffinj@lafayette.edu
  For storing content to be rendered in modal widgets
  -->
<div class="hidden container">
  <?php print render($page['hidden']); ?>
</div>

<div class="search-modal container">
  <?php if (!empty($page['search_modal'])): ?>
    <?php print render($page['search_modal']); ?>
  <?php endif; ?>
</div>

<div class="auth-modal container">
  <?php if (!empty($page['auth'])): ?>
    <?php print render($page['auth']); ?>
  <?php endif; ?>
</div>

<div class="share-modal container">
  <?php if (!empty($page['share'])): ?>
    <?php print render($page['share']); ?>
  <?php endif; ?>
</div>

<div class="contact-modal container">
  <?php if (!empty($page['contact'])): ?>
    <?php print render($page['contact']); ?>
  <?php endif; ?>
</div>

<footer class="footer container">
  <?php print render($page['footer']); ?>
</footer>
