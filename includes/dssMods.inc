<?php

  //require 'mods.inc';

/**
 * @file
 * Implements a simple class for working with Dublin Core data.
 *
 * Inspiration and design shamelessly stolen from the Dublin Core class within Islandora
 */

/**
 * DssMods Class
 */
  //class DssMods extends Mods {
class DssMods {

  /**
   * Constructor.
   *
   * @param string $mods_xml
   *   The Dublin Core XML.
   */
  public function __construct($mods_xml = NULL) {
    if (!empty($mods_xml)) {
      $this->mods = self::importFromXMLString($mods_xml);
    }
  }

  /**
   * Creates a new instance of the class by parsing mods_xml.
   *
   * @param string $mods_xml
   *   Dublin Core XML.
   *
   * @return DublinCore
   *   The instantiated object.
   */
  public static function importFromXMLString($mods_xml) {

    $mods_doc = new SimpleXMLElement($mods_xml);

    $mods_doc->registerXPathNamespace("xml", "http://www.w3.org/XML/1998/namespace");
    $mods_doc->registerXPathNamespace("mods", "http://www.loc.gov/mods/v3"); //http://www.loc.gov/mods/v3

    if( property_exists($mods_doc, 'mods')) {

      $mods_doc = $mods_doc->mods;
    }

    $mods_array = array();

    /*
    // Generic
    $template_map = array('Title' => "./mods:titleInfo/mods:title",
                          'Genre' => "./mods:genre",
                          'Location' => "./mods:subject/mods:geographic",
                          'Topic' => "./mods:subject/mods:topic",
                          'Note' => "./mods:note",
                          'Publisher' => "./mods:originInfo/mods:publisher",
                          'Date of Publication' => "(./mods:originInfo/mods:dateOther)[1]",
                          'Date of Digitization' => "(./mods:originInfo/mods:dateOther)[2]",
                          'Rights' => "./mods:accessCondition",

			  /*
			    East Asia Image Collections
			   * /
			  'Title.English' => "./mods:titleInfo/mods:title[@xml:lang='en-US']",
                          'Title.Chinese' => "./mods:titleInfo/mods:title[@xml:lang='zh']",
                          'Title.Japanese' => "./mods:titleInfo/mods:title[@xml:lang='Jpan']",
                          'Title.Korean' => "./mods:titleInfo/mods:title[@xml:lang='Kore']",
                          'Subject.OCM' => array('xpath' => "./mods:subject[@authorityURI='http://www.yale.edu/hraf/outline.htm']/mods:topic",
						 'facet' => true),

			  /*
			    MDL
			  * /
			  'Source' => './mods:location/mods:physicalLocation',
			  'Subject.LCSH' => array('xpath' => "./mods:subject[@authority='lcsh']/mods:topic",
						  'facet' => true),

			  'Format.Extent' => "./mods:physicalDescription/mods:extent",
                          'Description' => "./mods:abstract",
			  'Description.Series' => array('xpath' => "./mods:note[@type='series']",
							'facet' => true),

			  'Identifier.ItemNumber' => "./mods:identifier[@type='item-number']",

                          'Description.Citation' => "./mods:note[@type='citation']",
                          'Description.Critical' => "./mods:note[@type='content']",

                          'Description.Text.English' => "./mods:abstract[@xml:lang='en-US']",
                          'Description.Text.Chinese' => "./mods:abstract[@xml:lang='zh']",
                          'Description.Text.Japanese' => "./mods:abstract[@xml:lang='Jpan']",
                          'Description.Text.Korean' => "./mods:abstract[@xml:lang='Kore']",

                          'Contributors.Digital' => array('xpath' => "./mods:name/mods:role/mods:roleTerm[text()='ctb']/../../mods:namePart",
							  'facet' => true),

			  /*
                          'Creator.Company' => array('xpath' => "./mods:originInfo/mods:publisher",
						     'facet' => true),
			  * /

			  // #{maker}</namePart><role><roleTerm authority='marcrelator' type='code'>pht</roleTerm></role></name>"
                          //'Creator.Maker' => "./mods:name/mods:namePart",
                          'Creator.Maker' => array('xpath' => "./mods:name/mods:role/mods:roleTerm[text()='pht']/../../mods:namePart",
						   'facet' => true),

                          'Description.Ethnicity' => array('xpath' => "./mods:note[@type='ethnicity']",
							   'facet' => true),

			  'Description.Indicia' => array('xpath' => "./mods:note[@type='indicia']",
							 'facet' => true),

			  'Description.Indicia.English' => "./mods:note[@xml:lang='en-US']",
			  'Description.Indicia.Chinese' => "./mods:note[@xml:lang='en-US']",
			  'Description.Indicia.Japanese' => "./mods:note[@xml:lang='en-US']",
			  'Description.Indicia.Korean' => "./mods:note[@xml:lang='en-US']",

			  'Coverage.Location.Country' => array('xpath' => "./mods:subject/mods:hierarchicalGeographic/mods:country",
							       'facet' => true),

			  /*
			  'Coverage.Location' => array('xpath' => "./mods:subject/mods:geographic",
						       'facet' => true),
			  * /

			  'Format.Extant' => "./mods:physicalDescription/mods:extent",

			  'Format.Medium' => array('xpath' => "./mods:physicalDescription/mods:form",
						   'facet' => true),

			  'Date.Original' => array('xpath' => "./mods:originInfo/mods:dateOther[@type='original']",
						   'facet' => true,
						   'date' => true),

			  'Date.Artifact.Upper' => array('xpath' => "./mods:originInfo/mods:dateIssued[@point='end']",
							 'facet' => true,
							 'date' => true),

			  'Date.Artifact.Lower' => array('xpath' => "./mods:originInfo/mods:dateIssued[@point='start']",
							 'facet' => true,
							 'date' => true),

			  'Date.Search' => array('xpath' => "./mods:originInfo/mods:dateOther[@type='search']",
						 'facet' => true,
						 'date' => true),

			  'Date.Image.Upper' => array('xpath' => "./mods:originInfo/mods:dateCreated[@point='end']",
						      'facet' => true,
						      'date' => true),

			  'Date.Image.Lower' => array('xpath' => "./mods:originInfo/mods:dateCreated[@point='start']",
						      'facet' => true,
						      'date' => true),

			  'Relation.IsPartOf' => array('xpath' => "./mods:note[@type='admin']",
						       'facet' => false,
						       'link' => true,
						       'href' => ''),

			  'Relation.SeeAlso' => "./mods:relatedItem[@type='references']/mods:note[@type='citation']",

			  'Creator.Digital' => "./mods:note[@type='creation credits']",

			  'Publisher.Digital' => "./mods:note[@type='statement of responsibility']",

			  //'Rights.Digital' => "./mods:accessCondition",

			  'Format.Digital' => "./mods:note[@type='digital format']",

			  /*
			  'Identifier.Download' => array('xpath' => "./mods:location/mods:url[@displayLabel='Download']",
							 'facet' => false,
							 'link' => true,
							 'href' => '%value'),
			  */

			  // DSS-116
			  //'Identifier.Zoom' => './mods:location/mods:url[@displayLabel="Zoom"]',

			  /*
			    war-casualties
			   * /

			  'Description.class' => array('xpath' => './mods:note[@type="graduating class"]',
						       'facet' => true),

			  'Date.birth.display' => './mods:note[@type="recorded date of birth"]',

			  'Coverage.place.birth' => array('xpath' => './mods:note[@type="recorded place of birth"]',
							  'facet' => true),

			  'Description.millitary.branch' => array('xpath' => "./mods:note[@type='military branch']",
								  'facet' => true),

			  'Description.millitary.rank' => array('xpath' => "./mods:note[@type='military rank']",
								'facet' => true),

			  'Description.millitary.unit' => array('xpath' => "./mods:note[@type='military unit']",
								'facet' => true),

			  'Date.death.display' => array('xpath' => "./mods:note[@type='recorded date of death']",
							'facet' => true,
							'date' => true),

			  'Coverage.place.death' => array('xpath' => "./mods:note[@type='recorded place of death']",
							  'facet' => true),

			  'Description.cause.death' => "./mods:note[@type='recorded cause of death']",

			  'Description.honors' => array('xpath' => "./mods:note[@type='military honors']",
							'facet' => true),

			  'Format.analog' => "./mods:note[@type='analog format']",
                          );
			  */

    // Generic
    /*
    $template_map = array('Title' => "./mods:titleInfo/mods:title",
                          'Genre' => "./mods:genre",
                          'Location' => "./mods:subject/mods:geographic",
                          'Topic' => "./mods:subject/mods:topic",
                          'Note' => "./mods:note",
                          'Publisher' => "./mods:originInfo/mods:publisher",
                          'Date of Publication' => "(./mods:originInfo/mods:dateOther)[1]",
                          'Date of Digitization' => "(./mods:originInfo/mods:dateOther)[2]",
                          'Rights' => "./mods:accessCondition",
			  );
    */

    $template_map = array(
			  /*
			    East Asia Image Collections
			   */
			  'Title.English' => "./mods:titleInfo/mods:title[@xml:lang='en-US']",
                          'Title.Chinese' => "./mods:titleInfo/mods:title[@xml:lang='zh']",
                          'Title.Japanese' => "./mods:titleInfo/mods:title[@xml:lang='Jpan']",
                          'Title.Korean' => "./mods:titleInfo/mods:title[@xml:lang='Kore']",
                          'Subject.OCM' => array('xpath' => "./mods:subject[@authorityURI='http://www.yale.edu/hraf/outline.htm']/mods:topic",
						 'facet' => true),

			  /*
			    MDL
			  */
			  'Source' => './mods:location/mods:physicalLocation',
			  'Subject.LCSH' => array('xpath' => "./mods:subject[@authority='lcsh']/mods:topic",
						  'facet' => true),

			  'Format.Extent' => "./mods:physicalDescription/mods:extent",
                          'Description' => "./mods:abstract",
			  'Description.Series' => array('xpath' => "./mods:note[@type='series']",
							'facet' => true),

			  'Identifier.ItemNumber' => "./mods:identifier[@type='item-number']",

                          'Description.Citation' => "./mods:note[@type='citation']",
                          'Description.Critical' => "./mods:note[@type='content']",

                          'Description.Text.English' => "./mods:abstract[@xml:lang='en-US']",
                          'Description.Text.Chinese' => "./mods:abstract[@xml:lang='zh']",
                          'Description.Text.Japanese' => "./mods:abstract[@xml:lang='Jpan']",
                          'Description.Text.Korean' => "./mods:abstract[@xml:lang='Kore']",

                          'Contributors.Digital' => array('xpath' => "./mods:name/mods:role/mods:roleTerm[text()='ctb']/../../mods:namePart",
							  'facet' => true),

			  /*
                          'Creator.Company' => array('xpath' => "./mods:originInfo/mods:publisher",
						     'facet' => true),
			  */

			  // #{maker}</namePart><role><roleTerm authority='marcrelator' type='code'>pht</roleTerm></role></name>"
                          //'Creator.Maker' => "./mods:name/mods:namePart",
                          'Creator.Maker' => array('xpath' => "./mods:name/mods:role/mods:roleTerm[text()='pht']/../../mods:namePart",
						   'facet' => true),

                          'Description.Ethnicity' => array('xpath' => "./mods:note[@type='ethnicity']",
							   'facet' => true),

			  'Description.Indicia' => array('xpath' => "./mods:note[@type='indicia']",
							 'facet' => true),

			  'Description.Indicia.English' => "./mods:note[@xml:lang='en-US']",
			  'Description.Indicia.Chinese' => "./mods:note[@xml:lang='en-US']",
			  'Description.Indicia.Japanese' => "./mods:note[@xml:lang='en-US']",
			  'Description.Indicia.Korean' => "./mods:note[@xml:lang='en-US']",

			  'Coverage.Location.Country' => array('xpath' => "./mods:subject/mods:hierarchicalGeographic/mods:country",
							       'facet' => true),

			  /*
			  'Coverage.Location' => array('xpath' => "./mods:subject/mods:geographic",
						       'facet' => true),
			  */

			  'Format.Extent' => "./mods:physicalDescription/mods:extent",

			  'Format.Medium' => array('xpath' => "./mods:physicalDescription/mods:form",
						   'facet' => true),

			  'Date.Original' => array('xpath' => "./mods:originInfo/mods:dateOther[@type='original']",
						   'facet' => true,
						   'date' => true),

			  'Date.Artifact.Upper' => array('xpath' => "./mods:originInfo/mods:dateIssued[@point='end']",
							 'facet' => true,
							 'date' => true),

			  'Date.Artifact.Lower' => array('xpath' => "./mods:originInfo/mods:dateIssued[@point='start']",
							 'facet' => true,
							 'date' => true),

			  'Date.Search' => array('xpath' => "./mods:originInfo/mods:dateOther[@type='search']",
						 'facet' => true,
						 'date' => true),

			  'Date.Image.Upper' => array('xpath' => "./mods:originInfo/mods:dateCreated[@point='end']",
						      'facet' => true,
						      'date' => true),

			  'Date.Image.Lower' => array('xpath' => "./mods:originInfo/mods:dateCreated[@point='start']",
						      'facet' => true,
						      'date' => true),

			  'Relation.IsPartOf' => array('xpath' => "./mods:note[@type='admin']",
						       'facet' => false,
						       'link' => true,
						       'href' => ''),

			  'Relation.SeeAlso' => "./mods:relatedItem[@type='references']/mods:note[@type='citation']",

			  'Creator.Digital' => "./mods:note[@type='creation credits']",

			  'Publisher.Digital' => "./mods:note[@type='statement of responsibility']",

			  //'Rights.Digital' => "./mods:accessCondition",

			  'Format.Digital' => "./mods:note[@type='digital format']",

			  /*
			  'Identifier.Download' => array('xpath' => "./mods:location/mods:url[@displayLabel='Download']",
							 'facet' => false,
							 'link' => true,
							 'href' => '%value'),
			  */

			  // DSS-116
			  //'Identifier.Zoom' => './mods:location/mods:url[@displayLabel="Zoom"]',

			  /*
			    war-casualties
			   */

			  'Description.class' => array('xpath' => './mods:note[@type="graduating class"]',
						       'facet' => true),

			  'Date.birth.display' => './mods:note[@type="recorded date of birth"]',

			  'Coverage.place.birth' => array('xpath' => './mods:note[@type="recorded place of birth"]',
							  'facet' => true),

			  'Description.military.branch' => array('xpath' => "./mods:note[@type='military branch']",
								  'facet' => true),

			  'Description.military.rank' => array('xpath' => "./mods:note[@type='military rank']",
								'facet' => true),

			  'Description.military.unit' => array('xpath' => "./mods:note[@type='military unit']",
								'facet' => true),

			  'Date.death.display' => array('xpath' => "./mods:note[@type='recorded date of death']",
							'facet' => true,
							'date' => true),

			  'Coverage.place.death' => array('xpath' => "./mods:note[@type='recorded place of death']",
							  'facet' => true),

			  'Description.cause.death' => "./mods:note[@type='recorded cause of death']",

			  'Description.honors' => array('xpath' => "./mods:note[@type='military honors']",
							'facet' => true),

			  'Format.analog' => "./mods:note[@type='analog format']",
                          );

    foreach($template_map as $facet => $value) {

      $is_facet = false;
      $is_date = false;
      $is_link = false;

      if(is_array($value)) {

	$xpath = $value['xpath'];
	$is_facet = $value['facet'];

	if(array_key_exists('date', $value)) {

	  $is_date = $value['date'];
	}

	if(array_key_exists('link', $value)) {

	  $is_link = $value['link'];
	}
      } else {

	$xpath = $value;
      }

      $map = function($element) {

        $lang = 'en-US';

        // Need to scrape
        // Refactor
        $m = preg_match('/xml\:lang\="(.+)"/', $element->asXML(), $matches);

        if($m) {

          $lang = $matches[1];
        }

        return array('label' => $element->getName(),
                     'language' => $lang,
                     'value' => (string) $element,
		     'class' => drupal_strtolower(preg_replace('/[^A-Za-z0-9]/', '-', $element->getName())));
      };

      $mods_elements = array_map($map, $mods_doc->xpath($xpath));

      for($i = 0; $i < sizeof($mods_elements); $i++) {

	$element = &$mods_elements[$i];

	if($i > 0) {

	  $prev_element = $mods_elements[$i - 1];
	  if($element['label'] == $prev_element['label'] &&
	     $element['language'] == $prev_element['language']) {
	    
	    $element['class'] .= ' metadata-value-set';
	  }
	}
      }

      /*
      $mods_elements = array();
      */

      /*
      $elements = $mods_doc->xpath($xpath);
      
      $elements = is_array($elements) ? $elements : array($elements);
      */
      
      //dpm($elements);

      /*
      for($i = 0; $i < sizeof($elements); $i++) {
	
	$element = $elements[$i];

	$lang = 'en-US';

        // Need to scrape
        // Refactor
        $m = preg_match('/xml\:lang\="(.+)"/', $element->asXML(), $matches);

        if($m) {

          $lang = $matches[1];
        }

	$class = drupal_strtolower(preg_replace('/[^A-Za-z0-9]/', '-', $element->getName()));
	if($i > 0) {

	  $prev_element = $elements[$i - 1];
	  if($element->getName() == $prev_element->getName()) {

	    $class .= ' metadata-value-set';
	  }
	}

	/*
        $mods_elements[] = array('label' => $element->getName(),
				 'language' => $lang,
				 'value' => (string) $element,
				 'class' => $class);
	* /
      }
      */

      // Refactor
      foreach($mods_elements as $i => &$element) {

	if($is_facet) {
	  
	  $element['facet'] = $facet;

	  // Refactor
	  /*
	  $element['facet_value'] = preg_replace('/\ /', '\ ', $element['value']);
	  $element['facet_value'] = preg_replace('/\[/', '\[', $element['facet_value']);
	  $element['facet_value'] = preg_replace('/\]/', '\]', $element['facet_value']);
	  $element['facet_value'] = preg_replace('/\//', '%2F', $element['facet_value']);
	  $element['facet_value'] = preg_replace('/\:/', '\:', $element['facet_value']);
	  */
	  $element['facet_value'] = '"' . $element['value'] . '"';
	}

	if($is_date) {
	  
	  // Refactor this for Drupal administration

	  //$element['date_value'] = date('F, Y', strtotime($element['value']));
	  $element['date_value'] = date('Y-m-d', strtotime($element['value']));
	}

	if($is_link) {

	  $element['href'] = $element['value'];
	}

	// Specific to migrated collections
	$element['label'] = $facet;
      }

      $mods_array = array_merge($mods_array, $mods_elements);
    }

    return $mods_array;
  }
  }
