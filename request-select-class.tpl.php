<?php

/**
 * @file
 * Default theme implementation to display all classes
 *
 * Available variables:
 * - $classes
 *
 * @see template_preprocess_request_select_class()
 */
?>
<table>
  <tbody>
  <?php foreach ($categories as $class): ?>
    <tr>
      <td class="title">
        <div>
		   <?php print "<a href=youwo/create_request_2>  $class->name  </a>" ?>
        </div>
      </td>
	</tr>
	<tr>
	  <td>
	      <?php print "$class->description"; ?>
	  </td>
	</tr>
  <?php endforeach ?>
  </tbody>
</table>
