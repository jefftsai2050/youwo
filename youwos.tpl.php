<?php

/**
 * @file
 * Default theme implementation to display a youwo which may contain youwo
 * containers as well as youwo topics.
 *
 * Variables available:
 * - $youwos: The youwos to display (as processed by youwo-list.tpl.php)
 * - $topics: The topics to display (as processed by youwo-topic-list.tpl.php)
 * - $youwos_defined: A flag to indicate that the youwos are configured.
 *
 * @see template_preprocess_youwos()
 * @see theme_youwos()
 */
?>
<?php if ($youwos_defined): ?>
<div id="youwo">
  <?php print $youwos; ?>
  <?php print $topics; ?>
</div>
<?php endif; ?>
