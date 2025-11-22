/// <reference path='./app.ts' />

class Panel {
    public constructor(app: App, panel: JQuery) {
        this.app = app;

        this.panel = panel;
        this.panel_header = panel.find('.header');
        this.panel_body = panel.find('.body');
        this.panel_placeholder = panel.find('.placeholder');

        this.panel_body.find('.button-close').on('click', this.on_close.bind(this));
    }

    public hide() {
        this.panel.hide();
    }

    public is_visible() {
        return this.panel.is(':visible');
    }

    public set_header(name: string) {
        this.panel_header.text(name);
    }

    public show() {
        this.panel.show();
    }

    public toggle() {
        this.panel.toggle();
    }

    protected app: App;

    protected panel: JQuery;
    protected panel_header: JQuery;
    protected panel_body: JQuery;
    protected panel_placeholder: JQuery;

    private on_close(e: JQuery.ClickEvent) {
        e.preventDefault();

        this.hide();
        this.app.background.hide();
    }
}
