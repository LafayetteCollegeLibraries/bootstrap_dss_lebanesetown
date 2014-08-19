/**
 * @file
 * Helper for the Sharethis module
 *
 * @author griffinj
 *
 */

(function($, Drupal) {

    //
    Drupal.theme.prototype.bootstrapDssLdrSharethis = function() {

	$('.sharethis-wrapper > span').each(function(i, e) {

		serviceName = $(e).attr('displaytext')[0].toUpperCase() + $(e).attr('displaytext').slice(1);

		if( $(e).children('.sharethis-anchor-wrapper').length == 0) {

		    $(e).append( $('<span class="sharethis-anchor-wrapper"><a class="sharethis-anchor">' + serviceName + '</a></span>')
				 .click(function() {
				     
					 $(this).next('.stButton').click();
				     }));
		}
	    });
    }

    //
    Drupal.behaviors.bootstrapDssLdrSharethis = {

	attach: function(context, settings) {

	    Drupal.theme('bootstrapDssLdrSharethis');
	}
    }
})(jQuery, Drupal);
