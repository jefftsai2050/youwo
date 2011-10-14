<?php

/**
 * @file
 * User page callbacks for the youwo module.
 */

/**
 * Menu callback; prints a youwo listing.
 */
include 'ServiceClass.php';
function create_request() {

	$youwo_per_page = variable_get('youwo_per_page', 25);
	$sortby = variable_get('youwo_order', 1);

	$youwo_term = ServiceClass::loadAll();

	return theme('request_select_class', 
		array('categories' => $youwo_term->classes,
		'parents' =>$youwo_term->parents,
		'sortby' => $sortby, 
		'youwos_per_page' => $youwo_per_page));
}
