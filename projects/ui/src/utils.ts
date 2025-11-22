/// <reference path='./app.ts' />
/// <reference path='./engine.ts' />

class Utils {
    public static add_data_id_events(app: App, element: JQuery) {
        element.on('click', function (e: JQuery.ClickEvent) {
            e.preventDefault();

            Engine.ui_select_object(parseInt(jQuery(this).attr('data-id')));
        }).on('dblclick', function (e: JQuery.DoubleClickEvent) {
            e.preventDefault();

            Engine.ui_look_at_object(parseInt(jQuery(this).attr('data-id')));
        }).on('mouseenter', function (e: JQuery.MouseEnterEvent) {
            e.preventDefault();

            app.preview.show();
        }).on('mouseleave', function (e: JQuery.MouseLeaveEvent) {
            e.preventDefault();

            app.preview.hide();
        });
    }

    public static capitalize(value: string) {
        return value.charAt(0).toUpperCase() + value.slice(1);
    }

    public static format_title(title: string) {
        let words = title.split('_');

        words.forEach((word: string, index: number) => {
            words[index] = Utils.capitalize(word);
        });

        return words.join(' ');
    }

    public static is_float(value: number) {
        return Number(value) === value && value % 1 !== 0;
    }

    public static set_buttons(placeholder: JQuery, items: Array<string>) {
        placeholder.empty();

        items.forEach((value: string) => {
            jQuery('<button type="button" class="btn btn-light">')
                .attr('data-name', value)
                .html(Utils.format_title(value))
                .appendTo(placeholder);

            placeholder.append(' ');
        });
    }

    public static set_rows(placeholder: JQuery, items: Array<object>) {
        placeholder.empty();

        items.forEach((value: object) => {
            let tr = jQuery('<tr>').appendTo(placeholder);

            jQuery.each(value, function (key: string, value: any) {
                if (key === 'id') {
                    tr.attr('data-id', value);
                }

                let formatted_value = value;

                if (Utils.is_float(formatted_value)) {
                    formatted_value = formatted_value.toFixed(2);
                }

                let td = jQuery('<td>').addClass(key).html(formatted_value).appendTo(tr);

                if (typeof value === 'number') {
                    td.addClass('number');
                }
            });
        });
    }
}
