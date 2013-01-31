<?php

/**
 * @file
 * This template is used to print a single field in a view.
 *
 * It is not actually used in default Views, as this is registered as a theme
 * function which has better performance. For single overrides, the template is
 * perfectly okay.
 *
 * Variables available:
 * - $view: The view object
 * - $field: The field handler object that can process the input
 * - $row: The raw SQL result that can be used
 * - $output: The processed output that will normally be used.
 *
 * When fetching output from the $row, this construct should be used:
 * $data = $row->{$field->field_alias}
 *
 * The above will guarantee that you'll always get the correct data,
 * regardless of any changes in the aliasing that might happen if
 * the view is modified.
 */
?>
<?php 

// Create an empty array to hold nids in view
$nids = array();

// Loop through rows and gather all nids into an array
foreach($row as $key => $record) {
	if ($key === 'nid') {
		$nids[] = $record;
	}
}

// Loop through array to find term name and tid
foreach ($nids as $nid) {

	$result = db_query('select ti.tid, tta.name, n.nid
						from {node} n
						join taxonomy_index AS ti ON n.nid = ti.nid
						join taxonomy_term_data AS tta ON ti.tid = tta.tid
						where n.nid = :nid', array(':nid' => $nid));

	$data = $result->fetchAll();
	$num_rows = $result->rowCount();

	// Create a count to track where we are
	$index = 1;
	print 'Categories: ';
	// Create output
	foreach($data as $item) {
		$cleaned_string = str_replace(' ', '-', strtolower($item->name));
		if ($index < $num_rows) {
			print '<a href="/blog_categories/'. $cleaned_string .'">'. $item->name .'</a> <span class="divider">&#8226;</span> ';
		} else {
			print '<a href="/blog_categories/'. $cleaned_string .'">'. $item->name .'</a>';
		}
		$index++;
	}
}