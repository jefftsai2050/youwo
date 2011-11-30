<?php

/**
 * @file
 * Default theme implementation to display a list of youwos and containers.
 *
 * Available variables:
 * - $youwos: An array of youwos and containers to display. It is keyed to the
 *   numeric id's of all child youwos and containers.
 * - $youwo_id: Youwo id for the current youwo. Parent to all items within
 *   the $youwos array.
 *
 * Each $youwo in $youwos contains:
 * - $youwo->is_container: Is TRUE if the youwo can contain other youwos. Is
 *   FALSE if the youwo can contain only topics.
 * - $youwo->depth: How deep the youwo is in the current hierarchy.
 * - $youwo->zebra: 'even' or 'odd' string used for row class.
 * - $youwo->name: The name of the youwo.
 * - $youwo->link: The URL to link to this youwo.
 * - $youwo->description: The description of this youwo.
 * - $youwo->new_topics: True if the youwo contains unread posts.
 * - $youwo->new_url: A URL to the youwo's unread posts.
 * - $youwo->new_text: Text for the above URL which tells how many new posts.
 * - $youwo->old_topics: A count of posts that have already been read.
 * - $youwo->num_posts: The total number of posts in the youwo.
 * - $youwo->last_reply: Text representing the last time a youwo was posted or
 *   commented in.
 *
 * @see template_preprocess_youwo_list()
 * @see theme_youwo_list()
 */
?>
<table id="youwo-<?php print $youwo_id; ?>">
  <thead>
    <tr>
      <th><?php print t('公司名称'); ?></th>
      <th><?php print t('Topics');?></th>
      <th><?php print t('Posts'); ?></th>
      <th><?php print t('Last post'); ?></th>
    </tr>
  </thead>
  <tbody>
  <?php foreach ($youwos as $child_id => $youwo): ?>
    <tr id="youwo-list-<?php print $child_id; ?>" class="<?php print $youwo->zebra; ?>">
      <td <?php print $youwo->is_container ? 'colspan="4" class="container"' : 'class="youwo"'; ?>>
        <?php /* Enclose the contents of this cell with X divs, where X is the
               * depth this youwo resides at. This will allow us to use CSS
               * left-margin for indenting.
               */ ?>
        <?php print str_repeat('<div class="indent">', $youwo->depth); ?>
          <div class="name"><a href="<?php print $youwo->link; ?>"><?php print $youwo->name; ?></a></div>
          <?php if ($youwo->description): ?>
            <div class="description"><?php print $youwo->description; ?></div>
          <?php endif; ?>
        <?php print str_repeat('</div>', $youwo->depth); ?>
      </td>
      <?php if (!$youwo->is_container): ?>
        <td class="topics">
          <?php print $youwo->num_topics ?>
          <?php if ($youwo->new_topics): ?>
            <br />
            <a href="<?php print $youwo->new_url; ?>"><?php print $youwo->new_text; ?></a>
          <?php endif; ?>
        </td>
        <td class="posts"><?php print $youwo->num_posts ?></td>
        <td class="last-reply"><?php print $youwo->last_reply ?></td>
      <?php endif; ?>
    </tr>
  <?php endforeach; ?>
  </tbody>
</table>
