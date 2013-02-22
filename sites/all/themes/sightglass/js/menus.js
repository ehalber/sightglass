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

    // -- Replace share menu dropdown as it doesn't work out of the
    // the box
    $('.region-menu-second-inner a').mouseenter(function() {
      if ( $(this).text() === 'Share') {
        //alert('here');
        if (isAppended == false) {
          var menu = $('.zone-footer .sharing-dropdown .pane-content');
          $('body').find('.qtip').attr('qtip','5').find('.qtip-content').html(menu);
          $('.qtip-content .pane-content').show();
          isAppended = true;
        }
      }
    });
    
    // -- END fix share dropdown
    // ----------------------------------------------------------
  	
	

  });
  
}(jQuery));