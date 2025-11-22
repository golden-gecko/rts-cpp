/// <reference path='./engine.ts' />
/// <reference path='./panel.ts' />

class MainMenu extends Panel {
    public constructor(app: App, panel: JQuery) {
        super(app, panel);

        this.panel.find('.button-continue').on('click', this.on_continue.bind(this));
        this.panel.find('.button-new-game').on('click', this.on_new_game.bind(this));
        this.panel.find('.button-save').on('click', this.on_save.bind(this));
        this.panel.find('.button-load').on('click', this.on_load.bind(this));
        this.panel.find('.button-options').on('click', this.on_options.bind(this));
        this.panel.find('.button-quit-game').on('click', this.on_quit_game.bind(this));
        this.panel.find('.button-quit-application').on('click', this.on_quit_application.bind(this));
    }

    private on_continue(e: JQuery.ClickEvent) {
        e.preventDefault();

        this.app.main_menu.hide();
        this.app.background.hide();
    }

    private on_new_game(e: JQuery.ClickEvent) {
        e.preventDefault();

        this.app.map_menu.show();
        this.app.main_menu.hide();
    }

    private on_save(e: JQuery.ClickEvent) {
        e.preventDefault();

        Engine.game_save();

        this.app.main_menu.hide();
        this.app.background.hide();
    }

    private on_load(e: JQuery.ClickEvent) {
        e.preventDefault();

        this.app.load_menu.show();
        this.app.main_menu.hide();
    }

    private on_options(e: JQuery.ClickEvent) {
        e.preventDefault();

        this.app.main_menu.hide();
        this.app.options.show();
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
