(function ($) {
  
  $(document).ready(function() {



  	// -- Append "All" link to end of blog category menu in 
  	// main blog dropdown

  	$('#block-menu-menu-top-left .menu a').mouseenter(function() {
  		if ($(this).text() != 'Our Blog') {
  			return;
  		}
  		//$('.view-blog-categories-menu ul li:last-child').append('<li><a href="/blog_categories/all">All</a></li>');
  	});

  	// -- Move tooltip into correct position
  	// When getting an element relative to another element,
  	// the position method is the one to use.
  	
  	$('.menu a').mouseenter(function() {
  		var offset = $(this).position();
  		var linkWidth = $(this).width();
  		$('.qtip-tip').css({
  			left: offset.left + linkWidth / 2 
  		});
  	});

    // -- END move tooltip position fix
    // ----------------------------------------------------------

    var isAppended = false;

    $('.zone-footer .sharing-dropdown .pane-content').hide();

    // Replace share menu dropdown as it doesn't work out of the
    // the box. This solution basically appends the correct working
    // code after the js registers the components correctly. It
    // then removes the non-working code.
    
    $('.region-menu-second-inner a').mouseenter(function() {
      if ( $(this).text() === 'Share') {
        if (isAppended == false) {

          // Save the code that's going to be inserted into the DOM 
          var menu = $('.zone-footer .sharing-dropdown .pane-content').contents();

          // Append the code to the correct element, and then remove
          // the element from the DOM
          $('.menu-minipanel-644 .pane-content').after(menu);
          $('.sharing-dropdown .pane-content').remove();
          
          $('.qtip-content .pane-content').show();

          // Set switch to true to prevent this code from running again
          isAppended = true;
        }
      }
    });

    // -- END fix share dropdown
    // ----------------------------------------------------------
  	
	

  });
  
}(jQuery));