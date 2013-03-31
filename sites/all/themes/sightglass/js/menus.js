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

    var isShareAppended = false;

    $('.zone-footer .sharing-dropdown .pane-content').hide();

    // Replace share menu dropdown as it doesn't work out of the
    // the box. This solution basically appends the correct working
    // code after the js registers the components correctly. It
    // then removes the non-working code.
    
    $('.region-menu-second-inner a').mouseenter(function() {
      if ( $(this).text() === 'Share') {
        if (isShareAppended == false) {

          // Save the code that's going to be inserted into the DOM 
          var menu = $('.zone-footer .sharing-dropdown .pane-content').contents();

          // Append the code to the correct element, and then remove
          // the element from the DOM
          $('.menu-minipanel-644 .pane-content').after(menu);
          $('.sharing-dropdown .pane-content').remove();
          
          $('.qtip-content .pane-content').show();

          // Set switch to true to prevent this code from running again
          isShareAppended = true;
        }
      }
    });

    var isShoppingAppended = false;

    $('.region-menu-second-inner a').mouseenter(function() {
      if ( $(this).text() === 'Bag') {
        if (isShoppingAppended == false) {

          // Save the code that's going to be inserted into the DOM 
          var menu = $('.block-commerce-cart .cart-contents').contents();
          

          // Append the code to the correct element, and then remove
          // the element from the DOM
          $('.menu-minipanel-1382 .pane-content').after(menu);
          $('.pane-commerce-cart-block .pane-content').remove();
          
          $('.qtip-content .pane-content').show();

          // Set switch to true to prevent this code from running again
          isShoppingAppended = true;
        }
      }
    });

    $('.block-menu-top-right .menu .menu-bag').mouseenter(function() {
        $('.qtip').find('.menu-minipanel-1382').parent().addClass('fix-menu-height');
        $('.view-commerce-cart-block').show();
    });


    // -- END fix share dropdown
    // ----------------------------------------------------------
    

    // Unbind the normal behavior from quicktabs. This change is
    // necessary based on decisions by Geoff and Kyle to add
    // more pages
    
    $('#quicktabs-our_company .quicktabs-tabs li, #quicktabs-our_company .quicktabs-tabs a').each(function() {
      $(this).unbind();
    });

    // When the user click on the quicktab link we check to see 
    // if they are already on this page. If they are we
    // exit out of the logic and do nothing

    $('#quicktabs-our_company .quicktabs-tabs a').each(function() {
      $(this).click(function(e) {
        e.preventDefault();

        if ( $(this).parent().hasClass('active') ){
          return false;
        } 

        $(this).parent().removeClass('active');
        
        var linkName = $(this).text();
        var loc = '';

        switch (linkName) {
          case 'About':
            loc = 'about-sightglass';
            break;
          case 'Sourcing':
            loc = 'sourcing';
            break;
          case 'Roasting':
            loc = 'roasting';
            break;
          case 'Locations':
            loc = 'locations';
            break;
        }

        location.href = '/our-company/' + loc;
      })
    });

    // These blocks of code help to reset quicktabs default behavior

    $('.page-our-company').find('.quicktabs-tabs li').removeClass('active');
    $('.page-our-company-about-sightglass').find('#quicktabs-tab-our_company-0').parent().addClass('active');
    $('.page-our-company-sourcing').find('#quicktabs-tab-our_company-1').parent().addClass('active');
    $('.page-our-company-roasting').find('#quicktabs-tab-our_company-2').parent().addClass('active');
    $('.page-our-company-locations').find('#quicktabs-tab-our_company-3').parent().addClass('active');
    
    $('.page-our-company').find('.quicktabs-tabpage').addClass('quicktabs-hide');
    $('.page-our-company-about-sightglass').find('#quicktabs-tabpage-our_company-0').removeClass('quicktabs-hide');
    $('.page-our-company-sourcing').find('#quicktabs-tabpage-our_company-1').removeClass('quicktabs-hide'); 
    $('.page-our-company-roasting').find('#quicktabs-tabpage-our_company-2').removeClass('quicktabs-hide'); 
    $('.page-our-company-locations').find('#quicktabs-tabpage-our_company-3').removeClass('quicktabs-hide');     


    // Repeat the above logic for partner featured pages
    // Unbind the normal behavior from the first two tabs on the whole page
    
  	$('.page-wholesale-featured-partner #quicktabs-tab-wholesale_tabs-0').unbind();
    $('.page-wholesale-featured-partner #quicktabs-tab-wholesale_tabs-1').unbind();

    // Unbind the normal behavior from the third tab on the feature partner page
    $('.page-wholesale #quicktabs-tab-wholesale_tabs-2').unbind();

  
    // Send user directly to featured partners page when 
    // featured partner tab is clicked when in wholesale,
    // but not on the featured partners page. The other tabs should
    // behave normally

    $('.wholesale-primary #quicktabs-wholesale_tabs .quicktabs-tabs li a').each(function() {

          if ( $(this).attr('id') == 'quicktabs-tab-wholesale_tabs-2' ) {
            $(this).click(function(e) {
                e.preventDefault();
                location.href = '/wholesale/featured-partner';
            });
          }

    });

    // On the featured partners page, don't do anything when the 
    // featured partners tab is clicked. When another tab is clicked
    // take the user back to that page and select the correct tab
    $('.page-wholesale-featured-partner #quicktabs-wholesale_tabs .quicktabs-tabs li a').each(function() {

        if ( $(this).attr('id') == 'quicktabs-tab-wholesale_tabs-2' ) {
          $(this).click(function(e) {
              e.preventDefault();
              return false;
          });
        } else {
              $(this).click(function(e) {
              e.preventDefault();
              var clickedLinkLoc = $(this).attr('href');
              var start = clickedLinkLoc.lastIndexOf('?');
              var destination = clickedLinkLoc.slice(start, clickedLinkLoc.length-1);
              console.log(destination);
              location.href = '/wholesale' + destination;
          });
        }
    });   
    
    // Remove all active classes from tabs
    
    $('.page-wholesale-featured-partner').find('.quicktabs-tabs li').removeClass('active');
    $('.page-wholesale-featured-partner').find('#quicktabs-tab-wholesale_tabs-2').parent().addClass('active');
    $('.page-wholesale-featured-partner').find('.quicktabs-tabpage').addClass('quicktabs-hide');

    $('.page-wholesale-featured-partner').find('#quicktabs-tabpage-wholesale_tabs-2').removeClass('quicktabs-hide');



    // Brewing Guides Menu
    
    $('.aeropress').live('hover', function() {
      $('.brewing-guides').css('background-position', '-712px 0');
    });

    $('.french-press').live('hover', function() {
      $('.brewing-guides').css('background-position', '-178px 0');
    });

    $('.clever').live('hover', function() {
      $('.brewing-guides').css('background-position', '-356px 0');
    });

    $('.chemex').live('hover', function() {
      $('.brewing-guides').css('background-position', '-534px 0');
    });

    $('.v60').live('hover', function() {
      $('.brewing-guides').css('background-position', '0 0');
    });

    // Force the user to select from the dropdown when
    // they want to go to their account
	

  });
  
}(jQuery));