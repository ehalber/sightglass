<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */

/*
 * This code adds some additional logic to the view that
 * allows me to place some additional lines on the page.
 * I was forced to do this when I couldn't clone the views
 * pager but this is much better as I no longer how to use 
 * javascript to get these elements on the page.
 *
 * I basically just setup a counter and just checked when it
 * wasn't the last row in the sequence.
 * 
 */

?>
<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<?php
$counter = 0;
?>

<?php foreach ($rows as $id => $row): ?>
  <div <?php if ($classes_array[$id]) { print 'class="' . $classes_array[$id] .'"';  } ?>>
    <?php print $row; ?>
    <?php //echo $counter%3; ?>
    <?php if ($counter%3 != 2) { print '<div class="divider"></div>'; } ?>
    <?php print '<div class="horz-divider"></div>'; ?>
    <?php $counter++ ?>
  </div>
<?php endforeach; ?>
