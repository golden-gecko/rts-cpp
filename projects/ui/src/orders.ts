/// <reference path='./engine.ts' />
/// <reference path='./panel.ts' />
/// <reference path='./utils.ts' />

class Orders extends Panel {
    public set(data: Array<string>) {
        Utils.set_buttons(this.panel_body, data);

        this.panel_body.find('button').on('click', this.on_click.bind(this));
    }

    public set_header(name: string) {
        this.panel_header.text('Orders - ' + Utils.format_title(name));
    }

    private on_click(e: JQuery.ClickEvent) {
        e.preventDefault();

        Engine.ui_set_order(jQuery(e.target).attr('data-name'));
    }
}
