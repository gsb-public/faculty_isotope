(function ($) {
  Drupal.behaviors.facultyIsotope = {
    attach: function (context, settings) {
      $grid = $('#faculty-isotope').isotope({
        itemSelector: '.views-row',
        layoutMode: 'fitRows'
      });

      $('#all').click(function(e) {
        e.preventDefault();
        $grid.isotope({ filter: '*' });
      });

      $('#filter-economics').click(function(e) {
        e.preventDefault();
        $grid.isotope({ filter: '.economics' })
      });

      $('#filter-search').click(function(e) {
        e.preventDefault();

        $.getJSON("/faculty-search/house", function(data) {
          results = data;
          $grid.isotope({filter: function() {
            var nid = $(this).data('nid');

            if ($.inArray(nid.toString(), results) == -1) {
              return false;
            }
            else {
              return true;
            }
          }});
        });
      });
    }
  };
})(jQuery);