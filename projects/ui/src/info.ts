/// <reference path='./panel.ts' />

class Info extends Panel {
    public set(data: object) {
        let data_string = JSON.stringify(data, null, 2);

        data_string = data_string.replace(/"/g, '');
        data_string = data_string.replace(/{/g, '');
        data_string = data_string.replace(/}/g, '');
        data_string = data_string.replace(/ \[/g, '');
        data_string = data_string.replace(/\]/g, '');
        data_string = data_string.replace(/,/g, '');
        data_string = data_string.replace(/ +\n/g, '\n');
        data_string = data_string.replace(/\n+/g, '\n');
        data_string = data_string.replace(/\n  /g, '\n');

        this.panel_placeholder.html(data_string);

        // TODO: Fix and test.
        /*
        let self = this;
        let tbody = jQuery('<tbody>').addClass('group');

        jQuery.each(data, function (k, v) {
            let tr = jQuery('<tr>');

            jQuery('<td>').appendTo(tr);
            jQuery('<td>').html(format_title(k)).appendTo(tr);

            if (is_float(v)) {
                jQuery('<td>').html(v.toFixed(2)).appendTo(tr);
            } else {
                jQuery('<td>').html(v).appendTo(tr);
            }

            tr.appendTo(tbody);
        });

        tbody.appendTo(self.panel_body);
        */
    }
}
