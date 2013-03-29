<?php


/**
 * [sightglass_alpha_process_page description]
 * @param  [type] $vars [description]
 * @return [type]       [description]
 */
function sightglass_alpha_process_page(&$vars) {
  
  // If user is logged in, add a class to the menu
  // so the menu can be hidden
  global $user;
  if ($user->uid) {
  	$vars['page']['header']['menu']['menu_second']['menu_menu-top-right'][1044]['#attributes']['class'][] = 'hide';
  } else {
  	$vars['page']['header']['menu']['menu_second']['menu_menu-top-right'][1384]['#attributes']['class'][] = 'hide';
  }

  //dpm($vars);

}

