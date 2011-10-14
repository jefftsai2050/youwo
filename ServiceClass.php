<?php

class ServiceClass 
{
	public $name;
	public $description;

	static public function loadAll($tid=NULL) {

		$cache = &drupal_static(__FUNCTION__, array());

		// Return a cached youwo tree if available.
		if (!isset($tid)) {
			$tid = 0;
		}

		if (isset($cache[$tid])) {
			return $cache[$tid];
		}

		$vid = variable_get('youwo_nav_vocabulary', 0);

		// Load and validate the parent term.
		if ($tid) {
			$youwo_term = taxonomy_term_load($tid);
			if (!$youwo_term || ($youwo_term->vid != $vid)) {
				return $cache[$tid] = FALSE;
			}
		}
		// If $tid is 0, create an empty object to hold the child terms.
		elseif ($tid === 0) {
			$youwo_term = (object) array(
				'tid' => 0,
			);
		}

		// Load parent terms.
		$youwo_term->parents = taxonomy_get_parents_all($youwo_term->tid);

		// Load the tree below.
		$classes = taxonomy_get_tree($vid, $tid);
		$youwo_term->classes = $classes;
		return $youwo_term;
	}
};
?>
