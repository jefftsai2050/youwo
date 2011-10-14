<?php

/**
 * @file
 * Administrative page callbacks for the youwo module.
 */
function youwo_form_main($type, $edit = array()) {
  $edit = (array) $edit;
  if ((isset($_POST['op']) && $_POST['op'] == t('Delete')) || !empty($_POST['confirm'])) {
    return drupal_get_form('youwo_confirm_delete', $edit['tid']);
  }
  switch ($type) {
    case 'youwo':
      return drupal_get_form('youwo_form_youwo', $edit);
      break;
    case 'container':
      return drupal_get_form('youwo_form_container', $edit);
      break;
  }
}

/**
 * Returns a form for adding a youwo to the youwo vocabulary
 *
 * @param $edit Associative array containing a youwo term to be added or edited.
 * @ingroup forms
 * @see youwo_form_submit()
 */
function youwo_form_youwo($form, &$form_state, $edit = array()) {
  $edit += array(
    'name' => '',
    'description' => '',
    'tid' => NULL,
    'weight' => 0,
  );
  $form['name'] = array('#type' => 'textfield',
    '#title' => t('Youwo name'),
    '#default_value' => $edit['name'],
    '#maxlength' => 255,
    '#description' => t('Short but meaningful name for this collection of threaded discussions.'),
    '#required' => TRUE,
  );
  $form['description'] = array('#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $edit['description'],
    '#description' => t('Description and guidelines for discussions within this youwo.'),
  );
  $form['parent']['#tree'] = TRUE;
  $form['parent'][0] = _youwo_parent_select($edit['tid'], t('Parent'), 'youwo');
  $form['weight'] = array('#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $edit['weight'],
    '#description' => t('Youwos are displayed in ascending order by weight (youwos with equal weights are displayed alphabetically).'),
  );

  $form['vid'] = array('#type' => 'hidden', '#value' => variable_get('youwo_nav_vocabulary', ''));
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit' ] = array('#type' => 'submit', '#value' => t('Save'));
  if ($edit['tid']) {
    $form['actions']['delete'] = array('#type' => 'submit', '#value' => t('Delete'));
    $form['tid'] = array('#type' => 'hidden', '#value' => $edit['tid']);
  }
  $form['#submit'][] = 'youwo_form_submit';
  $form['#theme'] = 'youwo_form';

  return $form;
}

/**
 * Process youwo form and container form submissions.
 */
function youwo_form_submit($form, &$form_state) {
  if ($form['form_id']['#value'] == 'youwo_form_container') {
    $container = TRUE;
    $type = t('youwo container');
  }
  else {
    $container = FALSE;
    $type = t('youwo');
  }

  $term = (object) $form_state['values'];
  $status = taxonomy_term_save($term);
  switch ($status) {
    case SAVED_NEW:
      if ($container) {
        $containers = variable_get('youwo_containers', array());
        $containers[] = $term->tid;
        variable_set('youwo_containers', $containers);
      }
      $form_state['values']['tid'] = $term->tid;
      drupal_set_message(t('Created new @type %term.', array('%term' => $form_state['values']['name'], '@type' => $type)));
      break;
    case SAVED_UPDATED:
      drupal_set_message(t('The @type %term has been updated.', array('%term' => $form_state['values']['name'], '@type' => $type)));
      // Clear the page and block caches to avoid stale data.
      cache_clear_all();
      break;
  }
  $form_state['redirect'] = 'admin/structure/youwo';
  return;
}

/**
 * Returns HTML for a youwo form.
 *
 * By default this does not alter the appearance of a form at all,
 * but is provided as a convenience for themers.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_youwo_form($variables) {
  return drupal_render_children($variables['form']);
}

/**
 * Returns a form for adding a container to the youwo vocabulary
 *
 * @param $edit Associative array containing a container term to be added or edited.
 * @ingroup forms
 * @see youwo_form_submit()
 */
function youwo_form_container($form, &$form_state, $edit = array()) {
  $edit += array(
    'name' => '',
    'description' => '',
    'tid' => NULL,
    'weight' => 0,
  );
  // Handle a delete operation.
  $form['name'] = array(
    '#title' => t('Container name'),
    '#type' => 'textfield',
    '#default_value' => $edit['name'],
    '#maxlength' => 255,
    '#description' => t('Short but meaningful name for this collection of related youwos.'),
    '#required' => TRUE
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $edit['description'],
    '#description' => t('Description and guidelines for youwos within this container.')
  );
  $form['parent']['#tree'] = TRUE;
  $form['parent'][0] = _youwo_parent_select($edit['tid'], t('Parent'), 'container');
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $edit['weight'],
    '#description' => t('Containers are displayed in ascending order by weight (containers with equal weights are displayed alphabetically).')
  );

  $form['vid'] = array(
    '#type' => 'hidden',
    '#value' => variable_get('youwo_nav_vocabulary', ''),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );
  if ($edit['tid']) {
    $form['actions']['delete'] = array('#type' => 'submit', '#value' => t('Delete'));
    $form['tid'] = array('#type' => 'value', '#value' => $edit['tid']);
  }
  $form['#submit'][] = 'youwo_form_submit';
  $form['#theme'] = 'youwo_form';

  return $form;
}

/**
 * Returns a confirmation page for deleting a youwo taxonomy term.
 *
 * @param $tid ID of the term to be deleted
 */
function youwo_confirm_delete($form, &$form_state, $tid) {
  $term = taxonomy_term_load($tid);

  $form['tid'] = array('#type' => 'value', '#value' => $tid);
  $form['name'] = array('#type' => 'value', '#value' => $term->name);

  return confirm_form($form, t('Are you sure you want to delete the youwo %name?', array('%name' => $term->name)), 'admin/structure/youwo', t('Deleting a youwo or container will also delete its sub-youwos, if any. To delete posts in this youwo, visit <a href="@content">content administration</a> first. This action cannot be undone.', array('@content' => url('admin/content'))), t('Delete'), t('Cancel'));
}

/**
 * Implement forms api _submit call. Deletes a youwo after confirmation.
 */
function youwo_confirm_delete_submit($form, &$form_state) {
  taxonomy_term_delete($form_state['values']['tid']);
  drupal_set_message(t('The youwo %term and all sub-youwos have been deleted.', array('%term' => $form_state['values']['name'])));
  watchdog('content', 'youwo: deleted %term and all its sub-youwos.', array('%term' => $form_state['values']['name']));

  $form_state['redirect'] = 'admin/structure/youwo';
  return;
}

/**
 * Form builder for the youwo settings page.
 *
 * @see system_settings_form()
 */
function youwo_admin_settings($form) {
  $number = drupal_map_assoc(array(5, 10, 15, 20, 25, 30, 35, 40, 50, 60, 80, 100, 150, 200, 250, 300, 350, 400, 500));
  $form['youwo_hot_topic'] = array('#type' => 'select',
    '#title' => t('Hot topic threshold'),
    '#default_value' => variable_get('youwo_hot_topic', 15),
    '#options' => $number,
    '#description' => t('The number of replies a topic must have to be considered "hot".'),
  );
  $number = drupal_map_assoc(array(10, 25, 50, 75, 100));
  $form['youwo_per_page'] = array('#type' => 'select',
    '#title' => t('Topics per page'),
    '#default_value' => variable_get('youwo_per_page', 25),
    '#options' => $number,
    '#description' => t('Default number of youwo topics displayed per page.'),
  );
  $forder = array(1 => t('Date - newest first'), 2 => t('Date - oldest first'), 3 => t('Posts - most active first'), 4 => t('Posts - least active first'));
  $form['youwo_order'] = array('#type' => 'radios',
    '#title' => t('Default order'),
    '#default_value' => variable_get('youwo_order', 1),
    '#options' => $forder,
    '#description' => t('Default display order for topics.'),
  );
  return system_settings_form($form);
}

/**
 * Returns an overview list of existing youwos and containers
 */
function youwo_overview($form, &$form_state) {
  module_load_include('inc', 'taxonomy', 'taxonomy.admin');

  $vid = variable_get('youwo_nav_vocabulary', '');
  $vocabulary = taxonomy_vocabulary_load($vid);
  $form = taxonomy_overview_terms($form, $form_state, $vocabulary);

  foreach (element_children($form) as $key) {
    if (isset($form[$key]['#term'])) {
      $term = $form[$key]['#term'];
      $form[$key]['view']['#href'] = 'youwo/' . $term['tid'];
      if (in_array($form[$key]['#term']['tid'], variable_get('youwo_containers', array()))) {
        $form[$key]['edit']['#title'] = t('edit container');
        $form[$key]['edit']['#href'] = 'admin/structure/youwo/edit/container/' . $term['tid'];
      }
      else {
        $form[$key]['edit']['#title'] = t('edit youwo');
        $form[$key]['edit']['#href'] = 'admin/structure/youwo/edit/youwo/' . $term['tid'];
      }
    }
  }

  // Remove the alphabetical reset.
  unset($form['actions']['reset_alphabetical']);

  // The form needs to have submit and validate handlers set explicitly.
  $form['#theme'] = 'taxonomy_overview_terms';
  $form['#submit'] = array('taxonomy_overview_terms_submit'); // Use the existing taxonomy overview submit handler.
  $form['#empty_text'] = t('No containers or youwos available. <a href="@container">Add container</a> or <a href="@youwo">Add youwo</a>.', array('@container' => url('admin/structure/youwo/add/container'), '@youwo' => url('admin/structure/youwo/add/youwo')));
  return $form;
}

/**
 * Returns a select box for available parent terms
 *
 * @param $tid ID of the term which is being added or edited
 * @param $title Title to display the select box with
 * @param $child_type Whether the child is youwo or container
 */
function _youwo_parent_select($tid, $title, $child_type) {

  $parents = taxonomy_get_parents($tid);
  if ($parents) {
    $parent = array_shift($parents);
    $parent = $parent->tid;
  }
  else {
    $parent = 0;
  }

  $vid = variable_get('youwo_nav_vocabulary', '');
  $children = taxonomy_get_tree($vid, $tid);

  // A term can't be the child of itself, nor of its children.
  foreach ($children as $child) {
    $exclude[] = $child->tid;
  }
  $exclude[] = $tid;

  $tree = taxonomy_get_tree($vid);
  $options[0] = '<' . t('root') . '>';
  if ($tree) {
    foreach ($tree as $term) {
      if (!in_array($term->tid, $exclude)) {
        $options[$term->tid] = str_repeat(' -- ', $term->depth) . $term->name;
      }
    }
  }
  if ($child_type == 'container') {
    $description = t('Containers are usually placed at the top (root) level, but may also be placed inside another container or youwo.');
  }
  elseif ($child_type == 'youwo') {
    $description = t('Youwos may be placed at the top (root) level, or inside another container or youwo.');
  }

  return array('#type' => 'select', '#title' => $title, '#default_value' => $parent, '#options' => $options, '#description' => $description, '#required' => TRUE);
}
