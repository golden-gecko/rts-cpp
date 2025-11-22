/// <reference path='./engine.ts' />
/// <reference path='./panel.ts' />

class LoadMenu extends Panel {
    public constructor(app: App, panel: JQuery) {
        super(app, panel);

        this.panel.find('.button-continue').on('click', this.on_continue.bind(this));
        this.panel.find('.button-quit-game').on('click', this.on_quit_game.bind(this));
        this.panel.find('.button-quit-application').on('click', this.on_quit_application.bind(this));
    }

    public set_saves(saves: Array<string>) {
        this.panel_placeholder.empty();

        saves.forEach((value: string) => {
            jQuery('<button class="btn btn-light">')
                .attr('data-name', value)
                .text(value)
                .appendTo(this.panel_placeholder)
                .on('click', this.on_save.bind(this));
        });
    }

    private on_continue(e: JQuery.ClickEvent) {
        e.preventDefault();

        this.app.load_menu.hide();
        this.app.background.hide();
    }

    private on_save(e: JQuery.ClickEvent) {
        e.preventDefault();

        Engine.game_load(jQuery(this).attr('data-name'));

        this.app.load_menu.hide();
        this.app.background.hide();
    }

    private on_quit_game(e: JQuery.ClickEvent) {
        e.preventDefault();

        Engine.game_quit();
    }

    private on_quit_application(e: JQuery.ClickEvent) {
        e.preventDefault();

        Engine.application_quit();
    }
}
