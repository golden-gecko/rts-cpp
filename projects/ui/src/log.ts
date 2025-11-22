/// <reference path='./panel.ts' />
/// <reference path='./utils.ts' />

class Log extends Panel {
    public constructor(app: App, panel: JQuery) {
        super(app, panel);

        this.panel.on('mouseenter', function (e: JQuery.MouseEnterEvent) {
            e.preventDefault();

            jQuery(this).css({
                'bottom': '5px',
                'height': 'auto',
                'overflow-y': 'auto'
            });
        }).on('mouseleave', function (e: JQuery.MouseLeaveEvent){
            e.preventDefault();

            jQuery(this).css({
                'bottom': 'auto',
                'height': '30px',
                'overflow-y': 'hidden'
            });
        });
    }

    public error(text: string, id?: number) {
        return this.write(text, 'error', id)
    }

    public info(text: string, id?: number) {
        return this.write(text, 'info', id)
    }

    private write(text: string, type: string, id?: number) {
        let today = new Date();

        let time
            = today.getHours().toString().padStart(2, '0')
            + ':'
            + today.getMinutes().toString().padStart(2, '0')
            + ':'
            + today.getSeconds().toString().padStart(2, '0');

        let p = jQuery('<p>')
            .attr('class', type)
            .attr('data-id', id)
            .text(time + ' ' + text)
            .prependTo(this.panel_placeholder);

        Utils.add_data_id_events(this.app, p);

        let entries = this.panel_placeholder.find('p');
        let max_entries = 30;

        if (entries.length > max_entries) {
            entries.slice(max_entries - entries.length).remove();
        }

        return this;
    }
}
