/**
 * @file A widget for sorting Islandora Objects within an ordered list (<ol>)
 * @author griffinj@lafayette.edu
 *
 */

"use strict";

    /*
    if(ASC) oldASC = ASC;
    if(DESC) oldDESC = DESC;
    */

    // Globals
    /*
    var ASC = true;
    var DESC = false;
    */

    /**
     * Constructor
     *
     */
var LafayetteDssObjectList = function($, element, options) {
    
    this.$ = $;
    this.element = element;
    this.options = $.extend({
	    fieldSelector: '.islandora-inline-metadata dd.solr-value.dc-title',
	    order: 'asc'
	}, options);
    
    this.fieldSelector = this.options.fieldSelector;
    this.order = this.options.order;

    this.$element = $(element);
    this._index = [];
};

/**
 * ObjectList Object
 *
 */
LafayetteDssObjectList.prototype = {

    constructor: LafayetteDssObjectList,

    sort: function(fieldSelector, order) {

	fieldSelector = fieldSelector || this.fieldSelector;
	order = order || this.order;
	$ = this.$;
	var that = this;

	this._index = this.$element.children('.islandora-solr-search-result').sort(function(u, v) {
		
		return $(u).find(fieldSelector).text().localeCompare($(v).find(fieldSelector).text());
	    });

	if(order != this.order) {

	    this._index = $(this._index.get().reverse());
	    this.order = order;
	}

	this.$element.empty().append(this._index);
    }
};

/**
 * Drupal integration
 *
 */
(function($, Drupal, LafayetteDssObjectList) {

    Drupal.theme.prototype.bootstrapDssObjectList = function() {

	/*
	$('.field-sort').click(function(e) {

		e.preventDefault();
		$('.field-sort').toggleClass('active');
	    });
	*/

	//var objectList = new LafayetteDssObjectList($, $('.islandora-solr-search-result-list'), { order: $('#order-sort-select').val() });
	var objectList = new LafayetteDssObjectList($, $('.islandora-solr-search-result-list'), { order: /field\-sort\-(.+)/.exec( $('.field-sort.active').attr('id'))[1] });

	//$('.islandora-discovery-control.title-sort-control select').change(function() {
	$('.field-sort').click(function(e) {

		$('.field-sort.active').removeClass('active');
		e.preventDefault();
		$(this).toggleClass('active');

		//objectList.sort($(this).val(), $('#order-sort-select').val());
		//objectList.sort($(this).val(), preg_match('/field\-sort\-(.+)/', $('.field-sort.active').attr('id'))[1]);

		objectList.sort($(this).val(), /field\-sort\-(.+)/.exec( $(this).attr('id'))[1] );
	    });
    };

    // @todo Refactor
    $(document).ready(function() {
	    
	    Drupal.theme('bootstrapDssObjectList');
	});

})(jQuery, Drupal, LafayetteDssObjectList);
