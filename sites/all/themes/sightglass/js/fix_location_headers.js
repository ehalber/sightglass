(function ($) {
  
  $(document).ready(function() {

    // -- On location page, move headers below maps
    
    if ( $('.page-our-company').length ) {
        $('.location-box').each(function(i) {
            $(this).find('.pane-content').append($(this).find('.pane-title'));
        });
    }
    
    // -- On featured partner detail page, close up tabs
    
    if ( $('.node-type-featured-partner').length ) {
        $('.quicktabs-wrapper li.active').removeClass('active');
        $('.quicktabs-tabpage').addClass('quicktabs-hide');
    }

    // -- On inquiry form, move the description underneath the label
    
    if ( $('.page-wholesale').length ) {
        $('.webform-component-textarea').each(function(i) {
            $(this).find('label').append($(this).find('.description'));
        });

        $('#webform-component-currently-owned-equipment .description').append('<div class="description">Please indicate the make / model.</div>');
    }

    // -- On resources page, added vertical divider
    if ( $('.page-resources').length ) {
        $('.view-brewing-guides-home .views-row:nth-child(2n-1)').append('<div class="divider" />');
        $('.view-brewing-guides-home .views-row:nth-child(1n)').append('<div class="horz-divider" />');
    }

    if ( $('.node-type-brewing-guide').length ) {
        //$('.related-resources .views-row:nth-child(2n-1)').append('<div class="divider" />');
    }

    if ( $('.front').length ) {
        $('.view-front-page-gallery .views-row:nth-child(2n-1)').append('<div class="divider" />');
        $('.view-front-page-gallery .views-row:nth-child(1n)').append('<div class="horz-divider" />');

    }

    // -- On product catalog pages, add dividers
    if ( $('.product-catalog').length ) {

        $('.view-product-catalog .views-row:nth-child(3n-2)').append('<div class="divider" />');
        $('.view-product-catalog .views-row:nth-child(3n-1)').append('<div class="divider" />');
        $('.view-product-catalog .views-row:nth-child(1n)').append('<div class="horz-divider" />');
        $('.view-product-catalog .views-row').click(function() {
            window.location = $(this).find('.views-field-title a').attr('href');
        });
        $('.coffee-row .views-row-1, .coffee-row .views-row-2, .coffee-row .views-row-3').append('<div class="top-divider" />');
        $('.view-related-coffees .views-row:nth-child(3n-2)').append('<div class="divider" />');
        $('.view-related-coffees .views-row:nth-child(3n-1)').append('<div class="divider" />');
        $('.view-related-coffees .views-row:nth-child(1n)').append('<div class="horz-divider" />');

        // Add dividers to blog articles in tab
        $('.view-blog-for-coffee .views-row:nth-child(3n-2)').append('<div class="divider" />');
        $('.view-blog-for-coffee .views-row:nth-child(3n-1)').append('<div class="divider" />');
        $('.view-blog-for-coffee .view-footer').prepend('<div class="horz-divider" />');

        // Add dividers for tools
        $('.view-brewing-tools .views-row:nth-child(3n-2)').append('<div class="divider" />');
        $('.view-brewing-tools .views-row:nth-child(3n-1)').append('<div class="divider" />');
        $('.view-brewing-tools .views-row:nth-child(1n)').append('<div class="horz-divider" />');
    



        // Add click through behavior on brewing tools catalog page
        
        $('.view-brewing-tools .views-row').each(function() {
            var url = $(this).find('.views-field-title a').attr('href');
            $(this).click(function() {
                location.href = url;
            })
        });
    }

    // -- On brewing guides taxonomy page
    if ( $('.context-brewing-tools-goods').length ) {
        $('.filtered-tools .views-row:nth-child(3n-2)').append('<div class="divider" />');
        $('.filtered-tools .views-row:nth-child(3n-1)').append('<div class="divider" />');
        $('.filtered-tools .views-row:nth-child(1n)').append('<div class="horz-divider" />');
    }


    // -- On blog page
    if ( $('.node-type-article').length ) {

        // Add dividers to blog articles in tab
        $('.view-blog-references .views-row:nth-child(3n-2)').append('<div class="divider" />');
        $('.view-blog-references .views-row:nth-child(3n-1)').append('<div class="divider" />');
        $('.view-blog-references .view-footer').prepend('<div class="horz-divider" />');
    }

    /*
    DELETE THIS:: I'm removing this code because I have replaced its
    functionality with a views template

    if ( $('.brewing-tools').length ) {
        $('.view-brewing-tools .views-row:nth-child(3n-2)').append('<div class="divider" />');
        $('.view-brewing-tools .views-row:nth-child(3n-1)').append('<div class="divider" />');
        $('.view-brewing-tools .views-row:nth-child(1n)').append('<div class="horz-divider" />');
    }
    */

    // -- On production detail pages, add grid 960 special classes
    if ( $('.product-detail').length ) {

        // -- Add alpha omega class to related content
        //$('.view-related-coffees .views-row:nth-child(3n-2)').addClass('alpha');
        //$('.view-related-coffees .views-row:nth-child(3n+3)').addClass('omega');

        
        // -- Grab the region's parent and place on view to control
        // rollover color

        $('.view-related-coffees .views-row').each(function() {
            $(this).addClass($(this).find('.views-field-field-region .field-content div').attr('class'));
            $(this).append('<div class="top-divider" />');

        });

        // -- Add dividing lines between products
        
        $('.view-related-coffees .views-row:nth-child(3n-2)').append('<div class="divider" />');
        $('.view-related-coffees .views-row:nth-child(3n-1)').append('<div class="divider" />');
        $('.view-related-coffees .views-row:nth-child(1n)').append('<div class="horz-divider" />');
        $('.view-related-coffees .views-row').click(function() {
            window.location = $(this).find('.views-field-title a').attr('href');
        });


        $('.coffee-row .views-row-1, .coffee-row .views-row-2, .coffee-row .views-row-3').append('<div class="top-divider" />');

        $('.pane-product-detail-description-panel-pane-1').before('<div class="above-description" />');

        $('.view-product-center .more-button').click(function() {
            $('.views-field-body').slideToggle();
        });

        // -- Replace file name with Download text
        $('.views-field-field-spec-sheet a, .views-field-field-spec-sheet-1 a').text('Download');

        // -- Replace availablity field value with appropriate message (brewing tools)
        if ( $('.views-field-commerce-stock .field-content').text() != '0' ) {
            $('.views-field-commerce-stock .field-content').text('In stock');
        } else {
            $('.views-field-commerce-stock .field-content').text('Out of Stock');
        }

        // -- Hide/show brewing tool overview when during mouseeenter/mouseleave
        // Got some help for this one at this url:
        // http://stackoverflow.com/questions/5936642/how-to-stop-the-fadein-repeating-over-multiple-hovers
        // This was going to be cooler looking abut a hard cut and slow feed look weird
        /*
        $('.brewing-row .view-related-coffees .views-row').hover(function() {
            $(this).find('.views-field-field-overview').addClass('active');
        }, function() {
            $(this).find('.views-field-field-overview').removeClass('active');
        });
        */
           

    }

    if ( $('.page-subscriptions').length ) {
        $('.view-subscriptions.varietal .views-row').append('<div class="divider" />');
    }

    if ( $('.node-type-brewing-guide').length ) {

          // Add dividers for guides
        $('.view-display-id-panel_pane_5 .views-row:nth-child(3n-2)').append('<div class="divider" />');
        $('.view-display-id-panel_pane_5 .views-row:nth-child(3n-1)').append('<div class="divider" />');
    }

    // On checkout page, update the page title
    if ( $('.page-checkout').length ) {
        $('h1').replaceWith('<h1 class="title" id="page-title"><span>Checkout</span></h1>');
    }



  });
  
}(jQuery));