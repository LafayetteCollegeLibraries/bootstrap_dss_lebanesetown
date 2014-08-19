<?php

/**
 * @file
 * Implements a simple class for working with Dublin Core data.
 *
 * Inspiration and design shamelessly stolen from the Dublin Core class within Islandora
 */

/**
 * Mods Class
 */
class Mods {

  public $mods = array('titleInfo' => array(),
		       'note' => array(),
		       'name' => array(),
		       'subject' => array(),
		       'typeOfResource' => array(),
		       'classification' => array(),
		       'genre' => array(),
		       'relatedItem' => array(),
		       'originInfo' => array(),
		       'identifier' => array(),
		       'language' => array(),
		       'location' => array(),
		       'physicalDescription' => array(),
		       'accessCondition' => array(),
		       'abstract' => array(),
		       'part' => array(),
		       'tableOfContents' => array(),
		       'extension' => array(),
		       'targetAudience' => array(),
		       'recordInfo' => array(),
		       );

  public $owner;

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
   * Add an element.
   *
   * @param string $element_name
   *   The name of the element to add.
   * @param string $value
   *   The value of the element to add.
   */
  public function addElement($label, $value, $lang='en-US', $facet=NULL) {

    if(is_string($value)) {

      $element = array('label' => $label,
		       'value' => $value,
		       'language' => $lang);

      if(isset($facet)) {

	$element['facet'] = $facet;
      }

      $this->mods[] = $element;
    }

    /*
    if (is_string($value) && is_array($this->mods[$element_name])) {
      $this->mods[$element_name][] = $value;
    }
    */
  }

  /**
   * Replace the given DC element with the given values.
   *
   * @param string $element_name
   *   The name of the elements to set.
   * @param mixed $values
   *   The values of the set the elements too.
   */
  public function setElement($element_name, $values) {

    /*
    if (is_array($values)) {
      $this->mods[$element_name] = $values;
    }
    elseif (is_string($values)) {
      $this->mods[$element_name] = array($values);
    }
    */
  }

  /**
   * Serialize this object as an XML string.
   *
   * @return string
   *   The serialized XML.
   */
  public function asXML() {
    $mods_xml = new DomDocument();
    $mods_root = $mods_xml->createElementNS('http://www.loc.gov/mods/v3', 'mods');
    //$mods_root->setAttribute('xmlns:dc', 'http://purl.org/dc/elements/1.1/');
    foreach ($this->mods as $mods_element => $values) {
      if (is_array($values) && !empty($values)) {
        foreach ($values as $value) {
          $new_item = $mods_xml->createElement($mods_element, $value);
          $mods_root->appendChild($new_item);
        }
      }
      else {
        $new_item = $mods_xml->createElement($mods_element);
        $mods_root->appendChild($new_item);
      }
    }
    $mods_xml->appendChild($mods_root);
    return $mods_xml->saveXML();
  }

  /**
   * Serializes this object as an array.
   *
   * @return array
   *   The serialized object.
   */
  public function asArray() {

    return $this->mods;
    /*
    $mods_array = array();
    foreach($this as $element) {

      if(!empty($element)) {

        foreach($element as $field => $values) {

          // Split value if the result value is an array.
          if(is_array($values)) {

            $value = '';
            $i = 0;
            foreach($values as $piece) {

              if(!empty($piece)) {

                if($i++) {

                  $value .= ", ";
                }
                $value .= $piece;
              }
            }
          } else {

            $value = $values;
          }
          $mods_label = explode(':', $field);
          $element_label = drupal_ucfirst($mods_label[1]);
          $mods_array[$field]['label'] = $element_label;
          $mods_array[$field]['value'] = $value;
          $mods_array[$field]['class'] = drupal_strtolower(preg_replace('/[^A-Za-z0-9]/', '-', $field));
        }
      }
    }
    return $mods_array;
    */
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

    $template_map = array(

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

			  'Format.Medium' => array('xpath' => "./mods:physicalDescription/mods:form",
						   'facet' => true),

			  'Relation.IsPartOf' => "./mods:note[@type='admin']",

			  'Relation.SeeAlso' => "./mods:relatedItem[@type='references']/mods:note[@type='citation']",

			  'Publisher.Digital' => "./mods:note[@type='statement of responsibility']",

			  'Rights.Digital' => "./mods:accessCondition",

			  'Format.Digital' => "./mods:note[@type='digital format']",

			  'Identifier.Download' => "./mods:location/mods:url[@displayLabel='Download']",

			  'Identifier.Zoom' => './mods:location/mods:url[@displayLabel="Zoom"]',

			  /*
			    war-casualties
			   */

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

			  'Relation.IsPartOff' => "./mods:note[@type='admin']",

			  'Format.analog' => "./mods:note[@type='analog format']",
                          );

    foreach($template_map as $facet => $value) {

      $is_facet = false;
      $is_date = false;

      if(is_array($value)) {

	$xpath = $value['xpath'];
	$is_facet = $value['facet'];

	if(array_key_exists('date', $value)) {

	  $is_date = $value['date'];
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

      // Refactor
      foreach($mods_elements as $i => &$element) {

	//$element[''] = drupal_strtolower(preg_replace('/[^A-Za-z0-9]/', '-', $field));

	if($is_facet) {
	  
	  $element['facet'] = $facet;
	  $element['facet_value'] = preg_replace('/\ /', '\ ', $element['value']);
	}

	if($is_date) {
	  
	  $element['date_value'] = date('F, Y', strtotime($element['value']));
	}

	// Specific to migrated collections
	$element['label'] = $facet;
      }

      $mods_array = array_merge($mods_array, $mods_elements);
    }

    return $mods_array;

    /*
    $mods_doc = new DomDocument();

    if (!empty($mods_xml) && $mods_doc->loadXML($mods_xml)) {

      $mods_root = $mods_doc->getElementsByTagNameNS('http://purl.org/dc/elements/1.1/', '*');
      $new_dc = new Mods();
      foreach ($mods_root as $child) {
        if (isset($new_dc->mods[$child->nodeName])) {
          array_push($new_dc->mods[$child->nodeName], $child->nodeValue);
        }
      }
      return $new_dc;
    }
    return NULL;
    */
  }

}
