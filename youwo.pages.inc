<?php

/**
* @file
* User page callbacks for the youwo module.
*/
/**
* Menu callback; prints a youwo listing.
*/
function youwo_page($youwo_term = NULL) {
if (!isset($youwo_term)) {
// On the main page, display all the top-level youwos.
$youwo_term = youwo_youwo_load(0);
}

$youwo_per_page = variable_get('youwo_per_page', 25);
$sortby = variable_get('youwo_order', 1);

if (empty($youwo_term->container)) {
$topics = youwo_get_topics($youwo_term->tid, $sortby, $youwo_per_page);
}
else {
$topics = '';
}

return theme('youwos', array('youwos' => $youwo_term->youwos, 'topics' => $topics, 'parents' => $youwo_term->parents, 'tid' => $youwo_term->tid, 'sortby' => $sortby, 'youwos_per_page' => $youwo_per_page));
}

function what_it_is_page() {
return theme('what_it_is_page', array());
}

function household() {
return theme('household', array());
}

function kindergarten() {
	return theme('kindergarten', array());
}

function youwo_start() {
return theme('youwo_start_page', array());
}

function theme_youwo_start_page($variables)
{
global $user;

if (!$user->uid) {
drupal_goto('youwo/what_it_is');
}
else {
drupal_goto('youwo');
}
}

function youwo_create_request() {

global $user;

$type = 'youwo';
$node = (object) array('uid' => $user->uid, 'name' => (isset($user->name) ? $user->name : ''), 'type' => $type, 'language' => LANGUAGE_NONE);

$output = drupal_get_form('youwo_create_request_form', $node);

return $output;
}

/**
* Perform validation checks on the given node.
*/
function youwo_node_validate2($node, $form, &$form_state) {
$type = node_type_get_type($node);

if (isset($node->nid) && (node_last_changed($node->nid) > $node->changed)) {
form_set_error('changed', t('The content on this page has either been modified by another user, or you have already submitted modifications using this form. As a result, your changes cannot be saved.'));
}

// Validate the "authored by" field.
if (!empty($node->name) && !($account = user_load_by_name($node->name))) {
// The use of empty() is mandatory in the context of usernames
// as the empty string denotes the anonymous user. In case we
// are dealing with an anonymous user we set the user ID to 0.
form_set_error('name', t('The username %name does not exist.', array('%name' => $node->name)));
}

// Validate the "authored on" field.
if (!empty($node->date) && strtotime($node->date) === FALSE) {
form_set_error('date', t('You have to specify a valid date.'));
}

// Invoke hook_validate() for node type specific validation and
// hook_node_validate() for miscellaneous validation needed by modules. Can't
// use node_invoke() or module_invoke_all(), because $form_state must be
// receivable by reference.
$function = node_type_get_base($node) . '_validate';
if (function_exists($function)) {
$function($node, $form, $form_state);
}
foreach (module_implements('node_validate') as $module) {
$function = $module . '_node_validate';
$function($node, $form, $form_state);
}
}

function youwo_node_form_validate($form, &$form_state) {
// $form_state['node'] contains the actual entity being edited, but we must
// not update it with form values that have not yet been validated, so we
// create a pseudo-entity to use during validation.
$node = (object) $form_state['values'];
youwo_node_validate2($node, $form, $form_state);
entity_form_field_validate('node', $form, $form_state);
}


/**
* Updates the form state's node entity by processing this submission's values.
*
* This is the default builder function for the node form. It is called
* during the "Save" and "Preview" submit handlers to retrieve the entity to
* save or preview. This function can also be called by a "Next" button of a
* wizard to update the form state's entity with the current step's values
* before proceeding to the next step.
*
* @see node_form()
*/
function youwo_node_form_submit_build_node($form, &$form_state) {

$node = $form_state['node'];
entity_form_submit_build_entity('node', $node, $form, $form_state);

//node_submit($node);

return $node;
}

function youwo_node_form_submit($form, &$form_state) {
$node = youwo_node_form_submit_build_node($form, $form_state);
$insert = empty($node->nid);
node_save($node);
$node_link = l(t('view'), 'node/' . $node->nid);
$watchdog_args = array('@type' => $node->type, '%title' => $node->title);
$t_args = array('@type' => node_type_get_name($node), '%title' => $node->title);

if ($insert) {
watchdog('content', '@type: added %title.', $watchdog_args, WATCHDOG_NOTICE, $node_link);
drupal_set_message(t('@type %title has been created.', $t_args));
}
else {
watchdog('content', '@type: updated %title.', $watchdog_args, WATCHDOG_NOTICE, $node_link);
drupal_set_message(t('@type %title has been updated.', $t_args));
}
if ($node->nid) {
$form_state['values']['nid'] = $node->nid;
$form_state['nid'] = $node->nid;
$form_state['redirect'] = 'node/' . $node->nid;
}
else {
// In the unlikely case something went wrong on save, the node will be
// rebuilt and node form redisplayed the same way as in preview.
drupal_set_message(t('The post could not be saved.'), 'error');
$form_state['rebuild'] = TRUE;
}
// Clear the page and block caches.
cache_clear_all();
}

function youwo_create_request_form($form, &$form_state, $node)
{
global $user;

// During initial form build, add the node entity to the form state for use
// during form building and processing. During a rebuild, use what is in the
// form state.
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

// Some special stuff when previewing a node.
if (isset($form_state['node_preview'])) {
$form['#prefix'] = $form_state['node_preview'];
$node->in_preview = TRUE;
}
else {
unset($node->in_preview);
}

// Identify this as a node edit form.
// @todo D8: Remove. Modules can implement hook_form_BASE_FORM_ID_alter() now.
$form['#node_edit_form'] = TRUE;

$form['#attributes']['class'][] = 'node-form';
if (!empty($node->type)) {
$form['#attributes']['class'][] = 'node-' . $node->type . '-form';
}

// Basic node information.
// These elements are just values so they are not even sent to the client.
foreach (array('nid', 'vid', 'uid', 'created', 'type', 'language') as $key) {
$form[$key] = array(
	'#type' => 'value',
	'#value' => isset($node->$key) ? $node->$key : NULL,
);
}

// Changed must be sent to the client, for later overwrite error checking.
$form['changed'] = array(
'#type' => 'hidden',
'#default_value' => isset($node->changed) ? $node->changed : NULL,
);
// Invoke hook_form() to get the node-specific bits. Can't use node_invoke(),
// because hook_form() needs to be able to receive $form_state by reference.
// @todo hook_form() implementations are unable to add #validate or #submit
//   handlers to the form buttons below. Remove hook_form() entirely.
$function = node_type_get_base($node) . '_form';
if (function_exists($function) && ($extra = $function($node, $form_state))) {
$form = array_merge_recursive($form, $extra);
}
// If the node type has a title, and the node type form defined no special
// weight for it, we default to a weight of -5 for consistency.
if (isset($form['title']) && !isset($form['title']['#weight'])) {
$form['title']['#weight'] = -5;
}
// @todo D8: Remove. Modules should access the node using $form_state['node'].
$form['#node'] = $node;

$form['additional_settings'] = array(
'#type' => 'vertical_tabs',
'#weight' => 99,
);

// Add the buttons.
$form['actions'] = array('#type' => 'actions');
$form['actions']['submit'] = array(
'#type' => 'submit',
'#access' => variable_get('node_preview_' . $node->type, DRUPAL_OPTIONAL) != DRUPAL_REQUIRED || (!form_get_errors() && isset($form_state['node_preview'])),
'#value' => t('Save'),
'#weight' => 5,
'#submit' => array('youwo_node_form_submit'),
);
$form['actions']['preview'] = array(
'#access' => variable_get('node_preview_' . $node->type, DRUPAL_OPTIONAL) != DRUPAL_DISABLED,
'#type' => 'submit',
'#value' => t('Preview'),
'#weight' => 10,
'#submit' => array('node_form_build_preview'),
);
if (!empty($node->nid) && node_access('delete', $node)) {
$form['actions']['delete'] = array(
'#type' => 'submit',
'#value' => t('Delete'),
'#weight' => 15,
'#submit' => array('node_form_delete_submit'),
);
}
// This form uses a button-level #submit handler for the form's main submit
// action. node_form_submit() manually invokes all form-level #submit handlers
// of the form. Without explicitly setting #submit, Form API would auto-detect
// node_form_submit() as submit handler, but that is the button-level #submit
// handler for the 'Save' action. To maintain backwards compatibility, a
// #submit handler is auto-suggested for custom node type modules.
$form['#validate'][] = 'youwo_node_form_validate';
if (!isset($form['#submit']) && function_exists($node->type . '_node_form_submit')) {
$form['#submit'][] = $node->type . '_node_form_submit';
}
$form += array('#submit' => array());

_youwo_field_attach_form('node', $node, $form, $form_state, $node->language);
var_dump($form);
return $form;
}

function test_pager() {

	//Create a list of headers for your Html table (see Drupal 7 docs for theme_table here: http://api.drupal.org/api/drupal/includes--theme.inc/function/theme_table/7
	$header = array(
		array('data' => 'Title', 'field' => 'title', 'sort' => 'asc'),
		array('data' => 'Node ID', 'field' => 'nid'),
		array('data' => 'Type', 'field' => 'type'),
		array('data' => 'Created', 'field' => 'created'),
		array('data' => 'Published'),
		);

	//Create the Sql query. This uses various parts of Drupal 7's new DBTNG database abstraction layer. See these links for details:
	//Dynamic queries: http://drupal.org/node/310075
	//Extenders: http://drupal.org/node/508796
	//Table sort: http://drupal.org/node/310075#table_sort
	
	$query = db_select('node', 'n')
				->condition('status', 1)	//Only published nodes, change condition as it suits you
				->extend('PagerDefault') 	//Pager Extender
					->limit(10)				//10 results per page
				->extend('TableSort')		//Sorting Extender
					->orderByHeader($header)//Field to sort on is picked from $header
				->fields ('n', array (
					'nid',
					'title',
					'type',
					'created',
					'status',
				));
	
	$results = $query
				->execute();
	
	$rows = array();	
	foreach ($results as $node) {
		$rows[] = array(
					'data' => array(
						  l($node->title, 'node/'. $node->nid .'/edit'),
						  $node->nid,
						  $node->type,
						  format_date($node->created),
						  $node->status
					)
		);
	}
	
	//Theme the html table: http://api.drupal.org/api/drupal/includes--theme.inc/function/theme_table/7
	$html = theme('table', 
				array(
					'header' => $header,
					'rows'=>$rows,
					'caption' => 'Creating Drupal 7 style tables',	//Optional Caption for the table
					'sticky' => TRUE,						//Optional to indicate whether the table headers should be sticky
					'empty' => 'No nodes created...',		//Optional empty text for the table if resultset is empty
				)
			);
	
	//Append pager: http://api.drupal.org/api/drupal/includes--pager.inc/function/theme_pager
	$html .= theme('pager',
				array(
					'tags' => array()
				)
			);
			
	return ($html);
}
