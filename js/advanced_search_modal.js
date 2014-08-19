
/**
 * @file
 * A jQuery UI plug-in based off of the Twitter Bootstrap modal widget
 *
 */

/* =========================================================
 * bootstrap-modal.js v2.3.2
 * http://getbootstrap.com/2.3.2/javascript.html#modals
 * =========================================================
 * Copyright 2013 Twitter, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ========================================================= */


!function ($) {

  "use strict"; // jshint ;_;

 /* MODAL CLASS DEFINITION
  * ====================== */

  var Modal = function (element, options) {

    this.options = options
    this.$element = $(element)
      .delegate('[data-dismiss="modal"]', 'click.dismiss.modal', $.proxy(this.hide, this))

    if (this.options) this.options.remote && this.$element.find('.modal-body').load(this.options.remote)
  }

  Modal.prototype = {

      constructor: Modal

    , toggle: function () {
        return this[!this.isShown ? 'show' : 'hide']()
      }

    , show: function () {
        var that = this
          , e = $.Event('show')

        this.$element.trigger(e)

        if (this.isShown || e.isDefaultPrevented()) return

        this.isShown = true

        this.escape()

        this.backdrop(function () {
          var transition = $.support.transition && that.$element.hasClass('fade')

          if (!that.$element.parent().length) {
            that.$element.appendTo(document.body) //don't move modals dom position
          }

          that.$element.show()

          if (transition) {
            that.$element[0].offsetWidth // force reflow
          }

          that.$element
            .addClass('in')
            .attr('aria-hidden', false)

          that.enforceFocus()

          transition ?
            that.$element.one($.support.transition.end, function () { that.$element.focus().trigger('shown') }) :
            that.$element.focus().trigger('shown')

        })
      }

    , hide: function (e) {
        e && e.preventDefault()

        var that = this

        e = $.Event('hide')

        this.$element.trigger(e)

        if (!this.isShown || e.isDefaultPrevented()) return

        this.isShown = false

        this.escape()

        $(document).off('focusin.modal')

        this.$element
          .removeClass('in')
          .attr('aria-hidden', true)

        $.support.transition && this.$element.hasClass('fade') ?
          this.hideWithTransition() :
          this.hideModal()
      }

    , enforceFocus: function () {
        var that = this
        $(document).on('focusin.modal', function (e) {
          if (that.$element[0] !== e.target && !that.$element.has(e.target).length) {
            that.$element.focus()
          }
        })
      }

    , escape: function () {
        var that = this
        if (this.isShown && this.options.keyboard) {
          this.$element.on('keyup.dismiss.modal', function ( e ) {
            e.which == 27 && that.hide()
          })
        } else if (!this.isShown) {
          this.$element.off('keyup.dismiss.modal')
        }
      }

    , hideWithTransition: function () {
        var that = this
          , timeout = setTimeout(function () {
              that.$element.off($.support.transition.end)
              that.hideModal()
            }, 500)

        this.$element.one($.support.transition.end, function () {
          clearTimeout(timeout)
          that.hideModal()
        })
      }

    , hideModal: function () {
        var that = this
        this.$element.hide()
        this.backdrop(function () {
          that.removeBackdrop()
          that.$element.trigger('hidden')
        })
      }

    , removeBackdrop: function () {
        this.$backdrop && this.$backdrop.remove()
        this.$backdrop = null
      }

    , backdrop: function (callback) {
        var that = this
          , animate = this.$element.hasClass('fade') ? 'fade' : ''

        if (this.isShown && this.options.backdrop) {
          var doAnimate = $.support.transition && animate

          this.$backdrop = $('<div class="modal-backdrop ' + animate + '" />')
            .appendTo(document.body)

          this.$backdrop.click(
            this.options.backdrop == 'static' ?
              $.proxy(this.$element[0].focus, this.$element[0])
            : $.proxy(this.hide, this)
          )

          if (doAnimate) this.$backdrop[0].offsetWidth // force reflow

          this.$backdrop.addClass('in')

          if (!callback) return

          doAnimate ?
            this.$backdrop.one($.support.transition.end, callback) :
            callback()

        } else if (!this.isShown && this.$backdrop) {
          this.$backdrop.removeClass('in')

          $.support.transition && this.$element.hasClass('fade')?
            this.$backdrop.one($.support.transition.end, callback) :
            callback()

        } else if (callback) {
          callback()
        }
      }
  }

  /* MODAL PLUGIN DEFINITION
   * ======================= */

  var old = $.fn.modal

  $.fn.modal = function (option) {
    return this.each(function () {
      var $this = $(this)
        , data = $this.data('modal')
        , options = $.extend({}, $.fn.modal.defaults, $this.data(), typeof option == 'object' && option)
      if (!data) $this.data('modal', (data = new Modal(this, options)))
      if (typeof option == 'string') data[option]()
      else if (options.show) data.show()
    })
  }

  $.fn.modal.defaults = {
      backdrop: true
    , keyboard: true
    , show: true
  }

  $.fn.modal.Constructor = Modal

 /* MODAL NO CONFLICT
  * ================= */

  $.fn.modal.noConflict = function () {
    $.fn.modal = old
    return this
  }

 /* MODAL DATA-API
  * ============== */

  $(document).on('click.modal.data-api', '[data-toggle="modal"]', function (e) {
    var $this = $(this)
      , href = $this.attr('href')
      , $target = $($this.attr('data-target') || (href && href.replace(/.*(?=#[^\s]+$)/, ''))) //strip for ie7
      , option = $target.data('modal') ? 'toggle' : $.extend({ remote:!/#/.test(href) && href }, $target.data(), $this.data())

    e.preventDefault()

    $target
      .modal(option)
      .one('hide', function () {
        $this.focus()
      })
  });

  /**
   * The Lafayette College DSS Modal class
   *
   * Object constructor
   *
   */
  var LafayetteDssModal = function(element, options) {

      this.options = $.extend({ width: 262,
			        widthOffset: 0,
			        heightOffset: 0 }, options);

      // Work-around
      // Refactor
      //this.holdsFocus = false;

      // Set the handler 'click.dismiss.modal' for the specified element to LafayetteDssModal.hide()
      // This needs to support more than one hide closure
      this.$element = $(element);

      // Set the handler for the dismiss event
      this.dismissEvent = 'click.dismiss.' + this.$element.attr('id') + '.modal';

      //.delegate('[data-dismiss="modal"]', 'click.dismiss.modal.', $.proxy(this.hide, this));      
      this.$element.delegate('.' + this.$element.attr('id') + '-close[data-dismiss="modal"]', this.dismissEvent, $.proxy(this.hide, this));

      // Work-around
      // Refactor
      $(document).mousedown(function(e) {

	      $(document).data('LafayetteDssModal', {$lastTarget: $(e.target), $element: this.$element});
	  });

      if (this.options) this.options.remote && this.$element.find('.modal-body').load(this.options.remote);

      // Refactor
      this.shownWidth = parseInt(this.options.width);
      this.widthOffset = parseInt(this.options.widthOffset);
      this.heightOffset = parseInt(this.options.heightOffset);
      this.anchorAlign = this.options.anchorAlign;

      // Should not be parsing this string for the user agent
      // Refactor
      this.isSmartPhoneAgent = /Android.*?(?:Mobile)|iPhone|Windows Mobile/i.test(navigator.userAgent) || ($(window).width() == 320 && $(window).height == 480) || ($(window).width() == 480 && $(window).height == 320);
  };

  /**
   * Modal Object
   *
   */
  LafayetteDssModal.prototype = {

      constructor: LafayetteDssModal,
      toggle: Modal.prototype.toggle,
      
      /**
       * Overriding the show method
       * @see Modal.show()
       *
       */
      show: function(target, options) {

	  var that = this;
	  var $target = $(target);

	  // Refactor for default jQuery UI options
	  var options = $.extend(options, { direction: 'up' });

	  // Adjust the z-index in order to avoid overlapping issues
	  that.$element.css('z-index', Math.floor( new Date().getTime() / 10000 % 2 * 100000  ));

	  // Possible recursion
	  var e = $.Event('show');
	  this.$element.trigger(e);
	  if (this.isShown || e.isDefaultPrevented()) return;

	  this.isShown = true;
	  
	  this.escape();

	  var transition = $.support.transition && that.$element.hasClass('fade');

	  if(!that.$element.parent().length) {

	      that.$element.appendTo(document.body); //don't move modals dom position
	  }

	  //that.$element.show();

	  /**
	   * Add the non-Bootstrap transition
	   *
	   */

	  //that.$element.hide();

	  /*
	    that.$element
	    .addClass('in')
	    .attr('aria-hidden', false);
	  */

	  that.$element.attr('aria-hidden', false);

	  /**
	   * Append a handler for AJAX POST requests
	   *
	   */

	  that.$element.find('form').submit(function(e) {

		  // @todo Refactor for a specific DOM class
		  if($(this).attr('id') != 'islandora-dss-solr-advanced-search-form') {

		      e.preventDefault();

		      $(document).data('LafayetteDssModal.lastForm', $(this));
		      /*
			$(document).data('LafayetteDssModal.lastForm.Id', $(this).attr('id'));
		      */

		      if(!$(this).find('.required').filter(function(i, e) {

				  return $(this).val() == '';
			      }).length) {

			  $.post($(this).attr('action'), $(this).serialize(), function(data, textStatus) {
				  
				  that.hide();
			      }).fail(function(data) {
				      
				      console.log('error');
				      that.hide();
				  });
		      } else {
			  
			  /*
			  $('<div class="alert alert-block alert-error"><a href="#" data-dismiss="alert" class="close">×</a><h4 class="element-invisible">Error message</h4><ul><li>Your Name field is required.</li><li>Your E-Mail Address field is required.</li><li>Subject field is required.</li><li>Message field is required.</li></ul></div>').hide().prependTo($(this).prev())
			      .show($.extend('slide', { direction: 'down' }, function() {
					  //.show($.extend('drop', options, function() {
					  
					  setTimeout(function() {
						  
						  //$(document).data('LafayetteDssModal.lastForm').parent().find('.alert').hide('scale');
						  $(document).data('LafayetteDssModal.lastForm').parent().find('.alert').hide('slide', { direction: 'up' });
						  //$(document).data('LafayetteDssModal.lastForm').parent().find('.alert').hide('drop', { direction: 'up' });
					      }, 1500 );
					  //}}));
				      }));
			  */
			  $('<div class="alert alert-block alert-error"><a href="#" data-dismiss="alert" class="close">×</a><h4 class="element-invisible">Error message</h4><ul><li>Your Name field is required.</li><li>Your E-Mail Address field is required.</li><li>Subject field is required.</li><li>Message field is required.</li></ul></div>').hide().prependTo($(this).prev())
			      .show($.extend('slide', { direction: 'down' }, function() {
					  //.show($.extend('drop', options, function() {
					  
					  setTimeout(function() {
						  
						  //$(document).data('LafayetteDssModal.lastForm').parent().find('.alert').hide('scale');
						  $(document).data('LafayetteDssModal.lastForm').parent().find('.alert').hide('slide', { direction: 'up' });
						  //$(document).data('LafayetteDssModal.lastForm').parent().find('.alert').hide('drop', { direction: 'up' });
					      }, 1500 );
					  //}}));
				      }));
			  
		      }	      
		  }
	      });

	  /**
	   * Integrate with jQuery UI
	   *
	   */

	  //that.$element.show('scale', function() {
	  //that.$element.show('slide', function() {

	  that.$element.addClass('shown');
	  var $navbar = $('.navbar-inner');
	  $(document).data('LafayetteDssModal.navbar.offset.top', $navbar.offset().top);

	  if(that.anchorAlign) {

	      /*
	      that.$element.css('top', ($target.offset().top - $target[0].offsetWidth / 4) + that.heightOffset);
	      that.$element.css('left', ($target.offset().left - that.shownWidth + $target.width() + $target[0].offsetWidth / 4) + that.widthOffset);
	      */

	      that.$element.css('top', Math.floor( ($target.offset().top - $target[0].offsetWidth / 4) + that.heightOffset));

	      if(that.isSmartPhoneAgent) {

		  that.$element.css('left', 'auto');
	      } else {

		  that.$element.css('left', Math.floor( ($target.offset().left - that.shownWidth + $target.width() + $target[0].offsetWidth / 4) + that.widthOffset));
	      }
	  } else {

	      // Ensure that the widget is always appended directly underneath the navbar

	      //var $navbar = $('.navbar-inner');
	      that.$element.css('top', $navbar.offset().top + $navbar.height());
	      //that.$element.css('left', 0);
	      //that.$element.css('margin-left', '33.3%');

	  }

	  //transition ?
	  //that.$element.one($.support.transition.end, function () { that.$element.focus().trigger('shown') }) :
	  that.$element.focus().trigger('shown');

	  that.$element.insertAfter($('.navbar-inner'));

	  /**
	   * Functionality for the contact form dialog
	   *
	   */
	  //that.$element.show('drop', {direction: 'up'}, 500, function() {
	  if(that.$element.attr('id') == 'contact') {

	      that.$element.show();

	      // Refactor into CSS (?)
	      // Explicitly modify the classes using an invocation of window.setTimeout()
	      window.setTimeout(function() {

		      jQuery('#contact.lafayette-dss-modal .modal-dialog').addClass('shown-faded');
		  }, 100);

	      var targetElement = that.$element;
	      $(document).data('LafayetteDssModal.' + $(targetElement).attr('id') + '.offset.top', $(targetElement).offset().top);

	      ////$(this).find('input.form-text:first').focus();
	      $(targetElement).find('input.form-text:first').focus();

	      // Hide when losing focus
	      //$(this).focusout(function(e) {

	      /**
	       * Work-around
	       * This can only be set after the "hide" method has been explicitly inherited from the Twitter Modal Object
	       * Attempting to implement this within the constructor raises an error, as hide(e) has not yet been appended
	       * @todo Refactor
	       *
	       */
	      ////$(this)
	      $(targetElement)
		  .focusin(function(e) {

			  $(document).data('LafayetteDssModal.focusedModal', that);
		      })
		  .off('focusout')
		  .focusout(function(e) {
			  
			  $(document).data('LafayetteDssModal.focusedModal', null);

			  // Ensure that the modal is hidden after 3 seconds
			  setTimeout(function() {
				  
				  var focusedModal = $(document).data('LafayetteDssModal.focusedModal');
				  
				  if(focusedModal) {

				      // Ensure that the last element clicked does not lie within a modal...
				      ////if(!$(document).data('LafayetteDssModal').$lastTarget.is($(this)) &&
				      if(
					 typeof($(document).data('LafayetteDssModal')) == 'undefined' || (
										   !$(document).data('LafayetteDssModal').$lastTarget.is($(targetElement)) &&
										   !$(document).data('LafayetteDssModal').$lastTarget.parents('#' + focusedModal.$element.attr('id')).length )) {
					      
					  /**
					   * @todo Enable this before deployment
					   *
					   */
					  //that.hide();
				      }
				  } else {

				      /**
				       * @todo Enable this before deployment
				       *
				       */
				      //that.hide();
				  }
			      }, 3000);
		      });

	      /**
	       * For handling when scrolling while a modal is open
	       *
	       */
	      $(window).scroll(function() {

		      $('.lafayette-dss-modal.shown').each(function(i,e) {

			      //var offsetTop = $(document).data('LafayetteDssModal.offset.top');
			      var offsetTop = $(document).data('LafayetteDssModal.' + $(e).attr('id') + '.offset.top');
			      var navbarOffsetTop = $('.navbar-inner').offset().top;

			      // If the browser is at the top of the page...
			      if(!$(document).data('LafayetteDssModal.navbar.offset.top') || $(window).scrollTop() == 0) {

				  $(document).data('LafayetteDssModal.navbar.offset.top', navbarOffsetTop);

				  if($(window).scrollTop() == 0) {
				      //if( $('.navbar-inner.affix').length == 0 ) {

				      //$(e).css('top', offsetTop );

				      if($(document).height() - $(e).scrollTop() - $(e).height()) {

					  //console.log('trace1');
					  $(e).css('top', offsetTop );
				      }
				  }
			      }

			      // If the navbar has yet to be affixed...
			      if($(window).scrollTop() < navbarOffsetTop) {

				  if(($(document).height() - $(document).scrollTop()) > $(e).height()) {

				      // ...ensure that the dialog is adjusted as the user scrolls.
				      $(e).css('top', offsetTop );
				  }
			      }
			      
			      var $navbar = $('.navbar-inner.affix');

			      // If the navbar has been affixed and the user hasn't reached the bottom of the page...
			      if($navbar.length > 0 && ($(document).height() - $(document).scrollTop()) > $(e).height()) {

				  console.log('case 3');

				  /**
				   * This relates to DSSSM-517
				   *
				   */
				  $(e).addClass('affixed');

				  // Offset from the top + the number of pixels between the scrollbar and the top of the page
				  //$(e).css('top', offsetTop + $(window).scrollTop());
				  
				  console.log( $(window).scrollTop());
				  $(e).css('top', $(e).css('top') + $(window).scrollTop());

				  // More pixels need to be subtracted
				  //$(e).css('top', offsetTop + $(window).scrollTop() - $(document).data('LafayetteDssModal.navbar.offset.top') );
			      } else {

				  $(e).removeClass('affixed');
			      }
			  });
		  });
	  } else {

	      /**
	       * Functionality for the advanced search, share, and authentication dialogs
	       *
	       */
	      that.$element.show({ effect: 'slide', direction: 'up', duration: 500, complete: function() {

			  //that.$element.show().removeClass('shown-faded').addClass('shown-faded');
			  //that.$element.show().children('.modal-dialog').addClass('shown-faded');


			  //that.$element.show({effect: 'fade', duration: 500, complete: function() {

			  //$._data($(this)[0], 'events');

			  // Ensure that the modal is hidden after 3 seconds
			  /*
			    setTimeout(function() {

			    //$( "#effect:visible" ).removeAttr( "style" ).fadeOut();
			    //that.$element.fadeOut();
			    that.hide();
			    }, 3000);
			  */

			  var targetElement = that.$element;

			  //$(document).data('LafayetteDssModal.offset.top', $(this).offset().top);

			  $(document).data('LafayetteDssModal.' + $(this).attr('id') + '.offset.top', $(this).offset().top);
			  ////$(document).data('LafayetteDssModal.' + $(targetElement).attr('id') + '.offset.top', $(targetElement).offset().top);

			  $(this).find('input.form-text:first').focus();
			  ////$(targetElement).find('input.form-text:first').focus();

			  // Hide when losing focus
			  //$(this).focusout(function(e) {

			  /**
			   * Work-around
			   * This can only be set after the "hide" method has been explicitly inherited from the Twitter Modal Object
			   * Attempting to implement this within the constructor raises an error, as hide(e) has not yet been appended
			   * @todo Refactor
			   *
			   */
			  $(this)
			  ////$(targetElement)
			  .focusin(function(e) {

				  $(document).data('LafayetteDssModal.focusedModal', that);
			      })
			  .off('focusout')
			  .focusout(function(e) {

				  $(document).data('LafayetteDssModal.focusedModal', null);

				  // Ensure that the modal is hidden after 3 seconds
				  setTimeout(function() {

					  var focusedModal = $(document).data('LafayetteDssModal.focusedModal');

					  if(focusedModal) {

					      // Ensure that the last element clicked does not lie within a modal...
					      if(!$(document).data('LafayetteDssModal').$lastTarget.is($(this)) &&
						 ////if(!$(document).data('LafayetteDssModal').$lastTarget.is($(targetElement)) &&
						 !$(document).data('LafayetteDssModal').$lastTarget.parents('#' + focusedModal.$element.attr('id')).length ) {
					      
						  //that.hide();
					      }
					  } else {

					      //that.hide();
					  }
				      }, 3000);
			      });

			  /**
			   * For handling when scrolling while a modal is open
			   *
			   */
			  $(window).scroll(function() {

				  $('.lafayette-dss-modal.shown').each(function(i,e) {

					  //var offsetTop = $(document).data('LafayetteDssModal.offset.top');
					  var offsetTop = $(document).data('LafayetteDssModal.' + $(e).attr('id') + '.offset.top');
					  var navbarOffsetTop = $('.navbar-inner').offset().top;

					  if(!$(document).data('LafayetteDssModal.navbar.offset.top') || $(window).scrollTop() == 0) {

					      $(document).data('LafayetteDssModal.navbar.offset.top', navbarOffsetTop);

					      if($(window).scrollTop() == 0) {
						  //if( $('.navbar-inner.affix').length == 0 ) {

						  //$(e).css('top', offsetTop );
						  if($(document).height() - $(e).scrollTop() - $(e).height()) {

						      //console.log('trace1');
						      $(e).css('top', offsetTop );
						  }
					      }
					  }

					  if($(window).scrollTop() < navbarOffsetTop) {

					      //$(e).css('top', offsetTop );
					      //if($(document).height() - $(e).scrollTop() - $(e).height() > 0) {
					      if(($(document).height() - $(document).scrollTop()) > $(e).height()) {

						  console.log('trace2');
						  //console.log( $(document).height() - $(e).scrollTop() - $(e).height() );
						  //console.log(  $(document).height() - $(document).scrollTop() );

						  $(e).css('top', offsetTop );
					      }
					  }

					  var $navbar = $('.navbar-inner.affix');
					  //if($navbar.length > 0) {
					  //if($navbar.length > 0 && ($('body').height() - $('body').scrollTop()) != $('body').height()) {
					  if($navbar.length > 0 && ($(document).height() - $(document).scrollTop()) > $(e).height()) {

					      //$(e).css('top', $(e).offset().top + $(window).scrollTop());
					      $(e).addClass('affixed');

					      $(e).css('top', offsetTop + $(window).scrollTop() - $(document).data('LafayetteDssModal.navbar.offset.top') );
					  } else {

					      $(e).removeClass('affixed');
					  }
				      });

			  /*
			  var activeElement = $(document).data('LafayetteDssModal').$element;

			  if(activeElement) {

			     activeElement.css('top', activeElement.css('top') + $(window).scrollTop());
			  }
			  */
			      });
			  //});

		      }}); // jQuery show() invocation with jQueryUI animation
	  }

	  that.$element.addClass('shown');

	  //jQuery('.navbar-inner.affix-top').offset().top + jQuery('.navbar-inner.affix-top').height()
	  // Ensure that the widget is always appended directly underneath the navbar

	  var $navbar = $('.navbar-inner');
	  //that.$element.css('top', $navbar.offset().top + $navbar.height());
	  //that.$element.css('left', $target.offset().left - 262 + $target.width());

	  //that.$element.css('top', $target.offset().top - $target[0].offsetWidth / 4);
	  //that.$element.css('left', $target.offset().left - 262 + $target.width() + $target[0].offsetWidth / 4);

	  //$(document).data('LafayetteDssModal').$lastTarget.offset().top

	  if(transition) {

	      that.$element[0].offsetWidth; // force reflow
	  }

	  // Introduces problems related to recursion
	  /*
	  that.enforceFocus();
	  */
	  /*
	  transition ?
	  that.$element.one($.support.transition.end, function () { that.$element.focus().trigger('shown') }) :
	  that.$element.focus().trigger('shown');
	  */

      }, // show() method

      /**
       * Overriding the hide method
       * @see Modal.hide()
       */
      hide: function(e) {

	  e && e.preventDefault();

	  var that = this;

	  e = $.Event('hide');

	  this.$element.trigger(e);

	  if (!this.isShown || e.isDefaultPrevented()) return;

	  this.isShown = false;

	  this.escape();

	  $(document).off('focusin.modal');

	  // Work-around for losing focus
	  this.$element.off('focusout');

	  /**
	   * Updating the Document state
	   * @todo Refactor for race conditions
	   *
	   */
	  if($(document).data('LafayetteDssModal.focusedModal') == that) {

	      $(document).data('LafayetteDssModal.focusedModal', null);
	  }

	  this.$element
          .removeClass('in')
          .attr('aria-hidden', true);

	  // Hide any nested popover widgets
	  this.$element.find('[data-toggle="popover"]').each(function(i,e) {

		  $(e).popover('hide');
	      });

	  //$.support.transition && this.$element.hasClass('fade') ? this.hideWithTransition() : this.hideModal();
	  //this.hideWithTransition();
	  this.hideModal();
      },

      enforceFocus: Modal.prototype.enforceFocus,
      escape: Modal.prototype.escape,

      /**
       * Overriding the hideWithTransition method
       * @see Modal.hideWithTransition()
       *
       */
      hideWithTransition: function() {

	  var that = this;
	  var timeout = setTimeout(function () {

		  that.$element.off($.support.transition.end)
		  that.hideModal()
	      }, 500);

	  this.$element.one($.support.transition.end, function() {

		  clearTimeout(timeout);
		  that.hideModal();
	      });
      },

      //hideModal: Modal.prototype.hideModal,

      /**
       * Overriding the hideWithTransition method
       * @see Modal.hideModal()
       *
       */
      hideModal: function(options) {

	  var that = this;

	  // Refactor for passing jQuery UI options as arguments
	  var options = $.extend(options, { direction: 'up' });

	  //this.$element.hide('scale');
	  //this.$element.hide($.extend(options, {effect: 'slide'}));

	  //this.$element.hide('drop', options);

	  //this.$element.hide('slide', options);
	  //that.$element.removeClass('shown').hide();

	  if(that.$element.attr('id') == 'contact') {

	      jQuery('#contact.lafayette-dss-modal .modal-dialog').removeClass('shown-faded');
	      window.setTimeout(function() {

		      that.$element.hide();
		  }, 100);
	  } else {

	      this.$element.hide($.extend(options, { effect: 'slide' }));
	  }

	  this.backdrop(function () {

		  that.removeBackdrop();
		  that.$element.trigger('hidden');
	      });
      },

      removeBackdrop: Modal.prototype.removeBackdrop,
      backdrop: Modal.prototype.backdrop,
  };

  // MODAL PLUGIN DEFINITION
  // =======================

  var old = $.fn.lafayetteDssModal;

  $.fn.lafayetteDssModal = function(option, _relatedTarget) {

      return this.each(function() {

	      var $this   = $(this);
	      var data    = $this.data('bs.modal');
	      var options = $.extend({}, $.fn.lafayetteDssModal.defaults, $this.data(), typeof option == 'object' && option);

	      if (!data) $this.data('bs.modal', (data = new LafayetteDssModal(this, options)));

	      if (typeof option == 'string') {

		  data[option](_relatedTarget);
	      } else if (options.show) {

		  data.show(_relatedTarget);
	      }
	  });
  };

  /**
   * Default values for the plug-in
   *
   */
  $.fn.lafayetteDssModal.defaults = {

      backdrop: true,
      keyboard: true,
      show: true,
      width: 262,
      anchorAlign: true
  };

  $.fn.lafayetteDssModal.Constructor = LafayetteDssModal;

  // MODAL NO CONFLICT
  // =================

  $.fn.lafayetteDssModal.noConflict = function() {

      $.fn.lafayetteDssModal = old;
      return this;
  };

  // MODAL DATA-API
  // ==============

  $(document).on('click.bs.modal.data-api', '[data-toggle="lafayette-dss-modal"]', function(event) {

	  var $this   = $(this);
	  var href    = $this.attr('href');
	  var $target = $($this.attr('data-target') || (href && href.replace(/.*(?=#[^\s]+$)/, ''))); //strip for ie7
	  var option  = $target.data('modal') ? 'toggle' : $.extend({ remote: !/#/.test(href) && href }, $target.data(), $this.data());

	  // Refactor
	  var width = $this.attr('data-width');
	  var widthOffset = $this.attr('data-width-offset');
	  var heightOffset = $this.attr('data-height-offset');

	  var anchorAlign = true;
	  if( $this.attr('data-anchor-align')) {

	      anchorAlign = $this.attr('data-anchor-align').toLowerCase() == 'true';
	  }

	  option = $.extend(option, { 'width': width,
				      'width-offset': widthOffset,
				      'height-offset': heightOffset,
				      'anchorAlign': anchorAlign,
	      });

	  $target.lafayetteDssModal(option, this);
      });

}(window.jQuery);
