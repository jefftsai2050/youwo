<?php

/**
 * @file
 * User page callbacks for the youwo module.
 */

/**
 * Menu callback; prints a youwo listing.
 */
include_once 'youwo-common.inc';

function my_requests_page($uid=0) {

  $youwo_per_page = variable_get('youwo_per_page', 25);
  $sortby = variable_get('youwo_order', 1);

  $topics = get_user_requests($sortby, $youwo_per_page);

  return theme('my_requests', array('topics' => $topics, 'parents' => NULL, 'sortby' => $sortby, 'youwos_per_page' => $youwo_per_page));
}

function get_user_requests($sortby, $youwo_per_page) {
  global $user, $user_topic_list_header;

  $user_topic_list_header = array(
    NULL,
    array('data' => t('Topic'), 'field' => 'n.title'),
    array('data' => t('Replies'), 'field' => 'ncs.comment_count'),
    array('data' => t('Last reply'), 'field' => 'ncs.last_comment_timestamp'),
	array('data' => '状态', 'field' => 'r.request_status'),
  );

  $order = _youwo_get_topic_order($sortby);
  for ($i = 0; $i < count($user_topic_list_header); $i++) {
    if ($user_topic_list_header[$i]['field'] == $order['field']) {
      $user_topic_list_header[$i]['sort'] = $order['sort'];
    }
  }

  $query = db_select('node', 'n')->extend('TableSort');
  $query->fields('n', array('title', 'nid', 'type', 'sticky', 'created', 'uid'));
  $query->addField('n', 'comment', 'comment_mode');

  $query->join('node_comment_statistics', 'ncs', 'n.nid = ncs.nid');
  $query->fields('ncs', array('cid', 'last_comment_uid', 'last_comment_timestamp', 'comment_count'));

  /*  $query->join('youwo_index', 'f', 'f.nid = ncs.nid');
    $query->addField('f', 'tid', 'youwo_tid');*/

  $query->join('users', 'u', 'n.uid = u.uid');
  $query->addField('u', 'name');

  $query->join('youwo_request', 'r', 'n.nid = r.nid');
  $query->addField('r', 'request_status');

  $query->join('users', 'u2', 'ncs.last_comment_uid = u2.uid');

  $query->addExpression('CASE ncs.last_comment_uid WHEN 0 THEN ncs.last_comment_name ELSE u2.name END', 'last_comment_name');

  $query
      ->orderByHeader($user_topic_list_header)
      ->condition('u.uid', $user->uid);

  $result = $query->execute();

  $topics = array();
  $first_new_found = FALSE;
  foreach ($result as $topic) {
    if ($user->uid) {
      // folder is new if topic is new or there are new comments since last visit
      $history = _youwo_user_last_visit($topic->nid);
      $topic->new_replies = comment_num_new($topic->nid, $history);
      $topic->new = $topic->new_replies || ($topic->last_comment_timestamp > $history);
    }
    else {
      // Do not track "new replies" status for topics if the user is anonymous.
      $topic->new_replies = 0;
      $topic->new = 0;
    }

    // Make sure only one topic is indicated as the first new topic.
    $topic->first_new = FALSE;
    if ($topic->new != 0 && !$first_new_found) {
      $topic->first_new = TRUE;
      $first_new_found = TRUE;
    }

    if ($topic->comment_count > 0) {
      $last_reply = new stdClass();
      $last_reply->created = $topic->last_comment_timestamp;
      $last_reply->name = $topic->last_comment_name;
      $last_reply->uid = $topic->last_comment_uid;
      $topic->last_reply = $last_reply;
    }

	$topic->request_status = get_request_status_str($topic->request_status);

    $topics[] = $topic;
  }

  return $topics;
}
