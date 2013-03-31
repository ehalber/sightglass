<?php

function sightglass_alpha_preprocess_html(&$vars) {

	// add easing library

	// Add all css from http://www.webink.com/ online type foundry
	drupal_add_css('http://fnt.webink.com/wfs/webink.css/?project=9CFCB3DE-7C0A-413C-AD1F-7638D7F9A803&fonts=D30601E7-1DA0-BAC0-54FE-8E8C7D662B18:f=Typewriter-Medium,D8F3B8D2-63C2-8F91-037A-55138D453343:f=Typewriter-Bold,E595B706-42D6-77FB-3BC7-A0E290BAE8DD:f=Tondo-Regular,D1675F23-7746-D5BB-8E42-E2EED0FD7F4D:f=Typewriter,63593AFE-8EF9-393F-E993-F3BE2C200FBB:f=Tondo-Bold,F868C399-D74A-EC3F-0F48-F27258782A32:f=Tondo-Light,203D55FB-19B6-82BB-0FDE-7A0597273B4A:f=Tondo-Signage', 'external');

	//drupal_add_js('sites/all/libraries/colorbox', 'file');

	//drupal_add_css('sites/all/libraries/shadowbox-3.0.3/shadowbox.css', 'file');
	//drupal_add_js('sites/all/libraries/shadowbox-3.0.3/shadowbox.js', 'file');

	drupal_add_css('sites/all/libraries/jquery.fancybox/fancybox/jquery.fancybox-1.3.4.css', 'file');
	drupal_add_library('system', 'effects');
	drupal_add_js('sites/all/libraries/jquery.fancybox/fancybox/jquery.fancybox-1.3.4.pack.js', 'file');

	

	//drupal_add_js('http://s7.addthis.com/js/250/addthis_widget.js#pubid=ra-511d3a0e269f77f9', 'external');
	//
	// If the user is logged in, replace the contents of 
	// the menu item with the logout action
	
	//dsm($vars);

	//dpm($vars['page']['header']['menu']['menu_second']['menu_menu-top-right'][1044]);



	//unset($vars['page']['header']['menu']['menu_second']['menu_menu-top-right']['1044']);
	//
	//dpm($vars);
	//dsm($vars);

}
