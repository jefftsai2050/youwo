<?php

include 'modules/node/node.pages.inc';

function youwo_create_request_form($form, &$form_state, $node) {

  // Initialize a description of the steps for the wizard.
  if (empty($form_state['step'])) {
    $form_state['step'] = 1;

    // This array contains the function to be called at each step to get the
    // relevant form elements. It will also store state information for each
    // step.
    $form_state['step_information'] = _form_create_request_steps();
  }
  $step = &$form_state['step'];

  // Call the function named in $form_state['step_information'] to get the
  // form elements to display for this step.
  $form = $form_state['step_information'][$step]['form']($form, $form_state, $node);

  // Show the 'previous' button if appropriate. Note that #submit is set to
  // a special submit handler, and that we use #limit_validation_errors to
  // skip all complaints about validation when using the back button. The
  // values entered will be discarded, but they will not be validated, which
  // would be annoying in a "back" button.
  if ($step > 1) {
    $form['prev'] = array(
      '#type' => 'submit',
      '#value' => t('Previous'),
      '#name' => 'prev',
      '#submit' => array('youwo_create_request_previous_submit'),
      '#limit_validation_errors' => array(),
	  '#weight' => 19,
    );
  }

  // Show the Next button only if there are more steps defined.
  if ($step < count($form_state['step_information'])) {
    // The Next button should be included on every step
    $form['next'] = array(
      '#type' => 'submit',
      '#value' => t('Next'),
      '#name' => 'next',
      '#submit' => array('youwo_create_request_next_submit'),
	  '#weight' => 20,
    );
  }
  else {
    $form['finish'] = array(
      '#type' => 'submit',
      '#value' => t('提交'),
	  '#weight' => 21,
	  '#submit' => array('youwo_request_form_submit'),
    );
  }

  // Include each validation function defined for the different steps.
  if (function_exists($form_state['step_information'][$step]['form'] . '_validate')) {
    $form['next']['#validate'] = array($form_state['step_information'][$step]['form'] . '_validate');
  }

  return $form;
}

function youwo_create_request_previous_submit($form, &$form_state) {
  $current_step = &$form_state['step'];
  $form_state['step_information'][$current_step]['stored_values'] = $form_state['values'];
  if ($current_step > 1) {
    $current_step--;
    $form_state['values'] = $form_state['step_information'][$current_step]['stored_values'];
  }
  $form_state['rebuild'] = TRUE;
}

function youwo_create_request_next_submit($form, &$form_state) {
  $current_step = &$form_state['step'];
  $form_state['step_information'][$current_step]['stored_values'] = $form_state['values'];

  if ($current_step < count($form_state['step_information'])) {
    $current_step++;
    if (!empty($form_state['step_information'][$current_step]['stored_values'])) {
      $form_state['values'] = $form_state['step_information'][$current_step]['stored_values'];
    }
    else {
      $form_state['values'] = array();
    }
    $form_state['rebuild'] = TRUE;  // Force rebuild with next step.
    return;
  }
}

function _form_create_request_steps() {
	return array(
		1 => array(
			'form' => 'youwo_form_create_request_1',
		),
		2 => array(
			'form' => 'youwo_form_create_request_2',
		),
		3 => array(
			'form' => 'youwo_form_create_request_3',
		),
	);
}

function youwo_create_request() {

	global $user;

	$type = 'youwo';
	$node = (object) array('uid' => $user->uid, 'name' => (isset($user->name) ? $user->name : ''), 'type' => $type, 'language' => LANGUAGE_NONE);

	$output = drupal_get_form('youwo_create_request_form', $node);

	return $output;
}

function youwo_form_create_request_1($form, &$form_state, $node) {

	$form += array('#parents' => array());

	$type = node_type_get_type($node);
	$form['title'] = array(
		'#type' => 'textfield',
		'#title' => check_plain($type->title_label),
		'#default_value' => !empty($form_state['values']['title']) ? $form_state['values']['title'] : '',
		'#required' => TRUE, 
		'#weight' => -5
		);

	if (!empty($node->nid)) {
		$youwo_terms = $node->taxonomy_youwos;
		$form['youwo_tid'] = 
			array('#type' => 'value', '#value' => $node->youwo_tid);
	}

	$par_classes = array();

	$vid = variable_get('youwo_nav_vocabulary');
	$terms = taxonomy_get_tree($vid, 0, NULL, TRUE);
	foreach ($terms as $term) 
		if ($term->parents[0] == 0)
			$par_classes[$term->tid] = $term->name;
		
	$form['class_fieldset'] = array(
		'#title' => t('您要提出哪类请求?'),
		'#prefix' => '<div id="class_fieldset_div">',
		'#suffix' => '</div>',
		'#type' => 'fieldset',
	);
	$form['class_fieldset']['parent_class'] = array(
		'#type' => 'select',
		'#title' => '大类',
		'#options' => $par_classes,
		'#ajax' => array(
		    'callback' => 'parent_class_callback',
			'wrapper' => 'class_fieldset_div',
			'effect' => 'fade',
		),
	);

  if (!empty($form_state['values']['parent_class'])) {
  	$parent_id = $form_state['values']['parent_class'];
	$sub_classes = array();
	foreach ($terms as $term) 
		if ($term->parents[0] == $parent_id)
			$sub_classes[$term->tid] = $term->name;

    $form['class_fieldset']['child_class'] = array(
			'#type' => 'select',
			'#title' => '小类',
			'#options' => $sub_classes,
		);
  }


	/*
	$field = field_info_field('taxonomy_youwos');
	$instance = field_info_instance('node', 'taxonomy_youwos', 'youwo');
	$result = field_default_form('node', $node, $field, $instance, 
	     LANGUAGE_NONE, array(), $form, $form_state);
	$form += (array) $result;

	if (!empty($form_state['values']['taxonomy_youwos'])) {
		$form['taxonomy_youwos']['und']['#default_value'] = 
				$form_state['values']['taxonomy_youwos']['und'][0];
	}*/



	$field = field_info_field('body');
	$instance = field_info_instance('node', 'body', 'youwo');
	$result = field_default_form('node', $node, $field, $instance, 
	     LANGUAGE_NONE, array(), $form, $form_state);
	$form += (array) $result;

	if (!empty($form_state['values']['body'])) {
		$form['body']['und'][0]['#default_value'] = $form_state['values']['body']['und'][0]['value'];
		$form['body']['und'][0]['summary']['#default_value'] = $form_state['values']['body']['und'][0]['summary'];
	}

	$field = field_info_field('youwo_public');
	$instance = field_info_instance('node', 'youwo_public', 'youwo');
	$result = field_default_form('node', $node, $field, $instance, 
	     LANGUAGE_NONE, array(), $form, $form_state);
	$form += (array) $result;

	if (!empty($form_state['values']['youwo_public'])) {
		$form['youwo_public']['und']['#default_value'] = 
			$form_state['values']['youwo_public'];
	}

	$field = field_info_field('youwo_file');
	$instance = field_info_instance('node', 'youwo_file', 'youwo');
	$result = field_default_form('node', $node, $field, $instance, 
	     LANGUAGE_NONE, array(), $form, $form_state);
	$form += (array) $result;

	return $form;
}

function parent_class_callback($form, $form_state) {
  return $form['class_fieldset'];
}

function youwo_form_create_request_2($form, &$form_state, $node) {

	$form += array('#parents' => array());
	var_dump ($form_state['step_information'][1]['stored_values']);

	$field = field_info_field('youwo_expected_fee');
	$instance = field_info_instance('node', 'youwo_expected_fee', 'youwo');
	$result = field_default_form('node', $node, $field, $instance, 
	     LANGUAGE_NONE, array(), $form, $form_state);
	$form += (array) $result;

	if (!empty($form_state['values']['youwo_expected_fee'])) {
		$form['youwo_expected_fee']['und'][0]['value']['#default_value'] = 
			$form_state['values']['youwo_expected_fee']['und'][0]['value'];
	}

	$field = field_info_field('youwo_priority');
	$instance = field_info_instance('node', 'youwo_priority', 'youwo');
	$result = field_default_form('node', $node, $field, $instance, 
	     LANGUAGE_NONE, array(), $form, $form_state);
	$form += (array) $result;
	if (!empty($form_state['values']['youwo_priority'])) {
		var_dump($form_state['values']['youwo_priority']);
		$form['youwo_priority']['und']['#default_value'] = 
			$form_state['values']['youwo_priority']['und'][0];
	}

	return $form;
}

function youwo_form_create_request_3($form, &$form_state, $node) {

	if (!isset($form_state['node'])) {
		if (!isset($node->title)) {
			$node->title = NULL;
		}
		node_object_prepare($node);
		$form_state['node'] = $node;
	}
	else {
		$node = $form_state['node'];
	}

	$result = '标题：' . $form_state['step_information'][1]['stored_values']['title'];
	$result .= '<br>' . '期望的费用：' . $form_state['step_information'][2]['stored_values']['youwo_expected_fee']['und'][0]['value'];
	$priority = $form_state['step_information'][2]['stored_values']['youwo_priority']['und'][0]['value'];
	$result .= '<br>' . '紧急程度：';
	switch ($priority) {
		case "0":
			$result .= '一般';
			break;
		case "1":
			$result .= '有点急';
			break;
		case "2":
			$result .= '很急';
			break;
	}
	
	$form['result'] =  array(
		'#type' => 'item',
		'#title' => '请您确认',
		'#markup' => $result,
	);
	return $form;
}

function youwo_request_form_submit($form, &$form_state) {

	$form_state['values'] = array_merge($form_state['step_information'][1]['stored_values'], $form_state['step_information'][2]['stored_values'], $form_state['values']);
	$form_state['values']['taxonomy_youwos']['und'][0]['tid'] = $form_state['step_information'][1]['stored_values']['child_class'];
	node_form_submit($form, $form_state);
}


/*
	// get all terms
	$terms = entity_load('taxonomy_term', FALSE);
	$options = array();
	foreach ( $terms as $id => $term) {
		$options[$term->tid] = $term->name;
	}
*/

?>
