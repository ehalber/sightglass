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
    }

    if ( $('.node-type-brewing-guide').length ) {
        $('.related-resources .views-row:nth-child(2n-1)').append('<div class="divider" />');
    }

    if ( $('.front').length ) {
        $('.view-front-page-gallery .views-row:nth-child(2n-1)').append('<div class="divider" />');
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
    }

    if ( $('.brewing-tools').length ) {
        $('.view-brewing-tools .views-row:nth-child(3n-2)').append('<div class="divider" />');
        $('.view-brewing-tools .views-row:nth-child(3n-1)').append('<div class="divider" />');
        $('.view-brewing-tools .views-row:nth-child(1n)').append('<div class="horz-divider" />');
    }

    // -- On production detail pages, add grid 960 special classes
    if ( $('.product-detail').length ) {

        // -- Add alpha omega class to related content
        //$('.view-related-coffees .views-row:nth-child(3n-2)').addClass('alpha');
        //$('.view-related-coffees .views-row:nth-child(3n+3)').addClass('omega');

        // -- Add dividing lines between products
        $('.view-related-coffees .views-row:nth-child(3n-2)').append('<div class="divider" />');
        $('.view-related-coffees .views-row:nth-child(3n-1)').append('<div class="divider" />');
        $('.view-related-coffees .views-row:nth-child(1n)').append('<div class="horz-divider" />');
        $('.view-related-coffees .views-row').click(function() {
            window.location = $(this).find('.views-field-title a').attr('href');
        });

        $('.coffee-row .views-row-1, .coffee-row .views-row-2, .coffee-row .views-row-3').append('<div class="top-divider" />');

        $('.pane-product-detail-description-panel-pane-1').before('<div class="above-description" />');

        $('.description-button .more-button').click(function() {
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



  });
  
}(jQuery));