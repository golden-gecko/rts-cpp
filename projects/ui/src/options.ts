/// <reference path='./engine.ts' />
/// <reference path='./panel.ts' />

class Options extends Panel {
    public constructor(app: App, panel: JQuery) {
        super(app, panel);

        this.panel_body.find('.button-save').on('click', this.on_save.bind(this));
    }

    private on_save(e: JQuery.ClickEvent) {
        e.preventDefault();

        Engine.application_save_options(this.panel_body.find('form').serialize());

        this.app.options.hide();
        this.app.main_menu.show();
    }
}
