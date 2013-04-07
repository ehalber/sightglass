(function ($) {
  
    var logoCSS = {
        width: 142,
        height: 142,
        background: 'url(/sites/all/themes/sightglass/images/logo.png) -568px 0',
        margin: '75px auto 0 auto',
        position: 'fixed'

    }

  $(document).ready(function() {

    // This code creates an overlay and appends site logo and 
    // uses fancy box to render the image on the page in a 
    // nicely formatted way.
    // 
    // This is used both in the blog and to show brewing tools
    // and good from the main catalog
    
    $('.page-blog .views-field-field-image a, .node-type-article .views-field-field-image a').each(function() {
        $(this).attr('href', $(this).find('img').attr('src'));
        var string = $(this).attr('href');
        $(this).attr('href', string.replace(/blog_hero/gi, 'lightbox_display'));  
        $(this).attr('title', $(this).find('img').attr('title')); 
        
        $('.fancybox').fancybox({
            transitionIn: 'fade',
            transitionOut: 'fade',
            easeIn: 'easeInOutQuint',
            easeOut: 'easeInOutQuint',
            speedIn: 250,
            speedOut: 400,
            overlayOpacity: 1,
            overlayColor: '#473000',
            scrolling: 'no',
            padding: 3,
            cyclic: true,
            titleFormat: function() {
                return '<div class="image-title">' + $(this).attr('title') + '</div><div class="author"></div>';
            },
            titlePosition: 'outside',
            onStart: function() {
                //$('#fancybox-overlay').append('<div class="logo" />');
                //$('.logo').css(logoCSS);
                //$('.logo').css('left', $(window).width()/2 - 71);
            },
            onComplete: function() {
                //$('body').css('overflow-y', 'hidden');
            },
            onClosed: function() {
                //$('.logo').remove();
               // $('body').css('overflow-y', 'scroll');
            }
        });
    });

    // TODO - this code should be wrapped in a function because
    // of the global variables, but I'm not sure how to do it
    // efficiently.
    
    if ( $('.brewing-tool .view-product-center .image').length ) {
        var $productImage = $('.brewing-tool .view-product-center .image');
        $productImage.find('a').attr('href', $productImage.find('img').attr('src'));
        var productTemp = $productImage.find('a').attr('href');
        $productImage.find('a').attr('href', productTemp.replace(/220x220_square/gi, 'product_zoom'));

        $('.fancybox').fancybox({
            transitionIn: 'fade',
            transitionOut: 'fade',
            easeIn: 'easeInOutQuint',
            easeOut: 'easeInOutQuint',
            speedIn: 250,
            speedOut: 400,
            overlayOpacity: 1,
            overlayColor: '#473000',
            scrolling: 'no',
            padding: 3,
            cyclic: true,
            titleFormat: function() {
                return '<div class="image-title">' + $(this).attr('title') + '</div><div class="author"></div>';
            },
            titlePosition: 'outside',
            onStart: function() {
                //$('#fancybox-overlay').append('<div class="logo" />');
                //$('.logo').css(logoCSS);
                //$('.logo').css('left', $(window).width()/2 - 71);
            },
            onComplete: function() {
                //$('body').css('overflow-y', 'hidden');
            },
            onClosed: function() {
                //$('.logo').remove();
               // $('body').css('overflow-y', 'scroll');
            }
        });
    }

    // Display a zoomed in version of the map when user
    // click on the map on the coffee pages
    
    $('.product-detail.node-type-latin-coffee .view-product-center .image a').each(function() {
        
        /*
          If this coffee has two images, then load the second
          image which will be white over brown. Otherwise just 
          load the regional map image. This only occurs with 
          coffees that have more than one image which seems
          kind of rare up to this point.
         */
        if ($(this).parents('.view-product-center').find('.attachment img').length ) {
            $(this).attr('href', $(this).parents('.view-product-center').find('.attachment img').attr('src'));
             var string = $(this).attr('href');
            $(this).attr('href', string.replace(/220x220_square/gi, 'coffee_region_large'));  
            $(this).attr('title', $(this).find('img').attr('title'));
        } else {
            $(this).attr('href', $(this).find('img').attr('src'));
            var string = $(this).attr('href');
            $(this).attr('href', string.replace(/220x220_square/gi, 'coffee_region_large'));  
            $(this).attr('title', $(this).find('img').attr('title')); 
        }


        $('.fancybox').fancybox({
            transitionIn: 'fade',
            transitionOut: 'fade',
            easeIn: 'easeInOutQuint',
            easeOut: 'easeInOutQuint',
            speedIn: 250,
            speedOut: 400,
            overlayOpacity: 1,
            overlayColor: '#473000',
            scrolling: 'no',
            padding: 0,
            autoScale: true,
            cyclic: true,
        
            titlePosition: 'outside',
            onStart: function() {
                //$('#fancybox-overlay').append('<div class="logo" />');
                //$('.logo').css(logoCSS);
                //$('.logo').css('left', $(window).width()/2 - 71);
            },
            onComplete: function() {
                //$('body').css('overflow-y', 'hidden');
            },
            onClosed: function() {
                //$('.logo').remove();
                //$('body').css('overflow-y', 'scroll');
            }
        });
    });


    // Load fancy box when a slide is clicked on 
    // any of the overview pages
    $('.not-front .views-slideshow-cycle-main-frame-row').each(function() {
        
        var img = $(this).find('img').attr('src');
        $(this).find('.fancybox').attr('href', img.replace(/tab_slideshow/gi, 'coffee_region_large'));
        $(this).find('.fancybox').fancybox({
            transitionIn: 'fade',
            transitionOut: 'fade',
            easeIn: 'easeInOutQuint',
            easeOut: 'easeInOutQuint',
            speedIn: 250,
            speedOut: 400,
            overlayOpacity: 1,
            overlayColor: '#473000',
            scrolling: 'no',
            padding: 3,
            autoScale: true,
            cyclic: true,
        
            titlePosition: 'outside',
            onStart: function() {
                //$('#fancybox-overlay').append('<div class="logo" />');
                //$('.logo').css(logoCSS);
                //$('.logo').css('left', $(window).width()/2 - 71);
            },
            onComplete: function() {
                //$('body').css('overflow-y', 'hidden');
            },
            onClosed: function() {
                //$('.logo').remove();
                //$('body').css('overflow-y', 'scroll');
            }
        });

    });


    // Load fancy box when a slide is clicked on 
    // any of the overview pages
    $('.page-wholesale-featured-partner .view-featured-partner.view-display-id-block_2 .views-row').each(function() {
            var imgEl = $(this).find('img').attr('src');
            console.log(imgEl);
            $(this).find('.fancybox').attr('href', imgEl.replace(/featured_partner_images_336x200/gi, 'coffee_region_large'));
            $(this).find('.fancybox').fancybox({
                transitionIn: 'fade',
                transitionOut: 'fade',
                easeIn: 'easeInOutQuint',
                easeOut: 'easeInOutQuint',
                speedIn: 250,
                speedOut: 400,
                overlayOpacity: 1,
                overlayColor: '#473000',
                scrolling: 'no',
                padding: 3,
                autoScale: true,
                cyclic: true,
            
                titlePosition: 'outside',
                onStart: function() {
                    //$('#fancybox-overlay').append('<div class="logo" />');
                    //$('.logo').css(logoCSS);
                    //$('.logo').css('left', $(window).width()/2 - 71);
                },
                onComplete: function() {
                    //$('body').css('overflow-y', 'hidden');
                },
                onClosed: function() {
                    //$('.logo').remove();
                    //$('body').css('overflow-y', 'scroll');
                }
            });

    });

    



    
  });
  
}(jQuery));