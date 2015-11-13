(function ($) {
  Drupal.behaviors.facultyIsotope = {
    attach: function (context, settings) {
      $grid = $('.faculty-list').isotope({
        itemSelector: '.views-row',
        layoutMode: 'fitRows',
        onLayout: function() {
          $(window).trigger("scroll");
        }
      });

      var $imgs = $(".faculty-list img");

      $imgs.lazyload({
        failure_limit: Math.max($imgs.length - 1, 0)
      });

      Drupal.settings.facultyIsotope = {};
      Drupal.settings.facultyIsotope.searchResults = '';


      $('.views-widget-filter-secondary input').click(function(e) {
        Drupal.facultyIsotope.update();
      });

      $('#edit-submit-faculty-filters').click(function(e) {
        e.preventDefault();

        var text = $('#edit-search').val();

        $.getJSON("/faculty-search/" + text, function(data) {
          Drupal.settings.facultyIsotope.searchResults = data;
          Drupal.facultyIsotope.update();
        });
      });
    }
  };

  Drupal.facultyIsotope = Drupal.facultyIsotope || {};

  Drupal.facultyIsotope.update = function() {
    $grid.isotope({filter: function() {
      $this = $(this);
      academicAreaTest = false;
      if ($('.academic-areas input:checked').length) {
        $('.academic-areas input:checked').each(function() {
          if ($this.hasClass($(this).val())) {
            academicAreaTest = true;
          }
        });
      }
      else {
        academicAreaTest = true;
      }

      centerTest = false;
      if ($('.centers input:checked').length) {
        $('.centers input:checked').each(function() {
          if ($this.hasClass($(this).val())) {
            centerTest = true;
          }
        });
      }
      else {
        centerTest = true;
      }

      var nid = $(this).data('nid');
      searchTest = true;
      if (Drupal.settings.facultyIsotope.searchResults.length && $.inArray(nid.toString(), Drupal.settings.facultyIsotope.searchResults) == -1) {
        searchTest = false;
      }

      console.log(academicAreaTest);
      console.log(centerTest);
      console.log(searchTest);

      return academicAreaTest && centerTest && searchTest;
    }});

  }
})(jQuery);