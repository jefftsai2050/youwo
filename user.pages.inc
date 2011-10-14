<?php

/**
 * @file
 * User page callbacks for the youwo module.
 */

/**
 * Menu callback; prints a youwo listing.
 */
function my_score_page() {

  $youwo_user = get_youwo_user();

  return theme('my_score', array('youwo_user' => $youwo_user)); 
}

function get_youwo_user() {
  global $user;

  $query = db_select('youwo_user', 'u');
  $query->fields('u', array('score', 'type',));
  $query->condition('u.uid', $user->uid);


  $result = $query->execute()->fetchObject();
  if ($result)
  	return $result;
  else {
    $youwo_user = new stdClass; 
    $youwo_user->score = 10;
    $youwo_user->type = 2;
    return $youwo_user;
  }
}
