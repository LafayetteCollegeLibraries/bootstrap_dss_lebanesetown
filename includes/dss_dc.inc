<?php

  /**
   * @file Object-oriented XML to Array transform
   * @author griffinj@lafayette.edu
   
   * Project-specific
   *
   */

class DssDc extends SimpleXMLElement {

  public function toArray($label_map=array()) {

    $this->registerXPathNamespace("xml", "http://www.w3.org/XML/1998/namespace");
    //$this->registerXPathNamespace("mods", "http://www.loc.gov/mods/v3"); //http://www.loc.gov/mods/v3
    $this->registerXPathNamespace("dc", "http://purl.org/dc/elements/1.1/");

    /*
      dc:titleTitle
      dc:datePublication Date
      dc:coverageCoverage
      dc:descriptionDescription
      dc:subjectSubject
      dc:rightsRights
      dc:publisherPublisher
      dc:sourceSource Material
      dc:typeMedium
     */

    $mods_array = array();

    //$relation_is_part_of_value = (string) array_shift($this->xpath("./mods:note[@type='admin']"));

    $template_map = array(
			  'Title' => "./dc:title",
			  'Publication Date' => array('xpath' => "./dc:date[1]",
						      'facet' => true,
						      'field' => 'dc.date',
						      'date' => true),
			  'Coverage' => "./dc:coverage",
			  'Description' => "./dc:description",
			  'Subject' => array('xpath' => "./dc:subject",
					     'facet' => true,
					     'field' => 'dc.subject'),
			  'Rights' => "./dc:rights",
			  'Publisher' => "./dc:publisher",
			  'Source Material' => "./dc:source",
			  'Medium' => array('xpath' => "./dc:type",
					    'facet' => true,
					    'field' => 'dc.type'),
			  );

    //$this->xpath("./mods:note[@type='admin']");

    foreach($template_map as $facet => $value) {

      $is_facet = false;
      $is_date = false;
      $is_link = false;

      if(is_array($value)) {

	$xpath = $value['xpath'];
	$is_facet = $value['facet'];

	/**
	 * Hard-coded value
	 * @todo Abstract and refactor
	 *
	 */
	if($is_facet && array_key_exists('field', $value)) {

	  //dpm($value['field']);

	  //$facet_field = $value['field'];
	  $facet_field = $value['field'];
	}

	if(array_key_exists('date', $value)) {

	  $is_date = $value['date'];
	}

	if(array_key_exists('link', $value)) {

	  $is_link = $value['link'];
	}
      } else {

	$xpath = $value;
      }

      // Transforming the MODS Document into a PHP Array
      //
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
		     //'class' => drupal_strtolower(preg_replace('/[^A-Za-z0-9]/', '-', $element->getName()))
		     'class' => strtolower(preg_replace('/[^A-Za-z0-9]/', '-', $element->getName()))
		     );
      };

      //dpm($facet);
      //dpm($xpath);

      $mods_elements = array_map($map, $this->xpath($xpath));
      //dpm($mods_elements);
      //dpm($label_map);

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

      $collection_facets = 0;

      // Refactor
      foreach($mods_elements as $i => &$element) {

	if($is_facet) {

	  //dpm( $facet);

	  /**
	   * Hard-coded mapping
	   * @todo Refactor
	   *
	   */
	  if(isset($facet_field) && $facet_field) {

	    //$element['field'] = $facet;
	    $element['facet'] = $facet_field;
	  } elseif(array_key_exists($facet, $label_map)) {

	    $element['facet'] = $label_map[$facet];
	  } else {

	    $element['facet'] = $facet;
	  }

	  if($is_date) {

	    $element['facet_value'] = '[' . $element['value'] . ' TO ' . $element['value'] . ']';
	  } else {
	  
	    $element['facet_value'] = '"' . $element['value'] . '"';
	  }

	  //dpm(array('trace' => $element['facet']));
	  //dpm(array('trace' => $element['facet_value']));

	  $facet_params_values = array();
	  $facet_params = array();

	  if($element['facet'] == 'cdm.Relation.IsPartOf') {

	    $collection_elements = $this->xpath("./mods:note[@type='admin']");

	    //dpm($element['value']);
	    

	    for($i=0; $i<$collection_facets; $i++) {

	      if($element['value'] != (string) $collection_elements[$i]) {
		//if(true) {

		$facet_params["f[{$i}]"] = 'cdm.Relation.IsPartOf' . ':"' . (string) $collection_elements[$i] . '"';
	      }
	    }

	    $facet_params["f[{$i}]"] = $element['facet'] . ':"' . $element['value'] . '"';
	    $collection_facets++;
	  } else {

	    /**
	     * @todo Refactor
	     * Resolves DSS-261
	     *
	     */
	    $facet_index = 1;
	    $facet_params["f[0]"] = 'cdm.Relation.IsPartOf:"Lafayette Newspaper Collection"';

	    $facet_params["f[{$facet_index}]"] = $element['facet'] . ':"' . $element['value'] . '"';
	  }

	  $element['facet_href'] = l($element['value'], "islandora/search/*:*", array('query' => $facet_params,
										      ));
	}

	if($is_date) {

	  $element['date_value'] = gmdate('Y-m-d', strtotime($element['value']));

	  $facet_params["f[{$facet_index}]"] = $element['facet'] . ':[' . $element['value'] . ' TO ' . $element['value'] . ']';
	  $element['facet_href'] = l($element['date_value'], "islandora/search/*:*", array('query' => $facet_params,
											   ));
	}

	if($is_link) {

	  $element['href'] = $element['value'];
	}
	$element['label'] = $facet;
      }

      $mods_array = array_merge($mods_array, $mods_elements);
    }

    $collection_elements = array();

    //dpm( $mods_array);

    return $mods_array;
  }
  }
