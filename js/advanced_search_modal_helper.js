 /**
 * @file Advanced Search helper functionality
 * @author griffinj@lafayette.edu
 *
 */

(function($, Drupal) {

    // Following the Drupal.theme implementation
    // Please see https://drupal.org/node/304258
    Drupal.theme.prototype.bootstrapDssAdvancedSearch = function() {

	buttonHandler = function() {

	    if( $('#islandora-dss-solr-advanced-search-form fieldset').length == 1 ) {

		$('#islandora-dss-solr-advanced-search-form #edit-terms-1-add').addClass('raised');
		$('#advanced-search-modal #search-modal-help').addClass('raised');
	    } else {

		$('#islandora-dss-solr-advanced-search-form #edit-terms-1-add').removeClass('raised');
		$('#advanced-search-modal #search-modal-help').removeClass('raised');
	    }
	};

	$('#islandora-dss-solr-advanced-search-form #edit-terms-1-add').click(buttonHandler);
	$('#islandora-dss-solr-advanced-search-form #edit-terms-0-field-wrapper-remove').click(buttonHandler);
	buttonHandler();

	$('#islandora-dss-solr-advanced-search-form').ajaxComplete(function(event, xhr, settings) {

		if($(event.target.id) == 'islandora-dss-solr-advanced-search-form') {

		    buttonHandler();
		}
	    });

	$('#islandora-dss-solr-advanced-search-form #edit-collection.form-select').change(function(e) {

		$('#islandora-dss-solr-advanced-search-form #edit-scoping-coll').focus();
		$('#islandora-dss-solr-advanced-search-form #edit-scoping-coll').prop('checked', true);
	    });
    }

    // Ensure that the execution of all bootstrap functionality lies within a modular, Drupal-compliant context
    Drupal.behaviors.bootstrapDssLdr = {

	attach: function(context, settings) {

	    Drupal.theme('bootstrapDssAdvancedSearch');
	}
    }

    /**
     * Work-around
     * @todo Investigate why this became necessary on 01/17/14
     *
     */
    $(document).ready(function() {

	    Drupal.theme('bootstrapDssAdvancedSearch');
	});

})(jQuery, Drupal);
