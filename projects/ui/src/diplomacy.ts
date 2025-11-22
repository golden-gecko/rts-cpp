/// <reference path='./panel.ts' />
/// <reference path='./utils.ts' />

class Diplomacy extends Panel {
    public set(data: object) {
        // TODO: Fix and test.
        /*
        set_rows(this.panel_body.find('table'), data);

        let table_header_row = this.panel_body.find('thead tr');
        let table_body = this.panel_body.find('tbody');

        table_header_row.empty();
        jQuery('<th>').appendTo(table_header_row);

        let index = 1;

        jQuery.each(data, function (k, v) {
            jQuery('<th>')
                .html(k)
                .appendTo(table_header_row);

            jQuery('<td>')
                .html(k)
                .addClass('name')
                .prependTo(table_body.find('tr:nth-child(' + index + ')'));

            index += 1;
        });
        */
    }
}
