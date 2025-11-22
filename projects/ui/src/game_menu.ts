/// <reference path='./panel.ts' />

class GameMenu extends Panel {
    public constructor(app: App, panel: JQuery) {
        super(app, panel);

        this.panel_body.find('.button-diplomacy').on('click', this.on_diplomacy.bind(this));
        this.panel_body.find('.button-layers').on('click', this.on_layers.bind(this));
        this.panel_body.find('.button-main-menu').on('click', this.on_main_menu.bind(this));
        this.panel_body.find('.button-objects').on('click', this.on_objects.bind(this));
        this.panel_body.find('.button-orders').on('click', this.on_orders.bind(this));
        this.panel_body.find('.button-statistics').on('click', this.on_statistics.bind(this));
        this.panel_body.find('.button-technologies').on('click', this.on_technologies.bind(this));
    }

    private hide_all(exclude: object) {
        let panels = [
            this.app.diplomacy,
            this.app.layers,
            this.app.main_menu,
            this.app.objects_admin,
            this.app.orders_admin,
            this.app.statistics,
            this.app.technologies
        ]

        for (let i = 0; i < panels.length; ++i) {
            if (panels[i] !== exclude) {
                panels[i].hide();
            }
        }
    }

    private on_diplomacy(e: JQuery.ClickEvent) {
        e.preventDefault();

        this.hide_all(this.app.diplomacy);
        this.app.diplomacy.toggle();

        if (this.app.diplomacy.is_visible()) {
            this.app.background.show();
        } else {
            this.app.background.hide();
        }
    }

    private on_layers(e: JQuery.ClickEvent) {
        e.preventDefault();

        this.hide_all(this.app.layers);
        this.app.layers.toggle();

        if (this.app.layers.is_visible()) {
            this.app.background.show();
        } else {
            this.app.background.hide();
        }
    }

    private on_main_menu(e: JQuery.ClickEvent) {
        e.preventDefault();

        this.hide_all(this.app.main_menu);
        this.app.main_menu.toggle();

        if (this.app.main_menu.is_visible()) {
            this.app.background.show();
        } else {
            this.app.background.hide();
        }
    }

    private on_objects(e: JQuery.ClickEvent) {
        e.preventDefault();

        this.hide_all(this.app.objects_admin);
        this.app.objects_admin.toggle();

        if (this.app.objects_admin.is_visible()) {
            this.app.background.show();
        } else {
            this.app.background.hide();
        }
    }

    private on_orders(e: JQuery.ClickEvent) {
        e.preventDefault();

        this.hide_all(this.app.orders);
        this.app.orders.toggle();

        if (this.app.orders.is_visible()) {
            this.app.background.show();
        } else {
            this.app.background.hide();
        }
    }

    private on_statistics(e: JQuery.ClickEvent) {
        e.preventDefault();

        this.hide_all(this.app.statistics);
        this.app.statistics.toggle();

        if (this.app.statistics.is_visible()) {
            this.app.background.show();
        } else {
            this.app.background.hide();
        }
    }

    private on_technologies(e: JQuery.ClickEvent) {
        e.preventDefault();

        this.hide_all(this.app.technologies);
        this.app.technologies.toggle();

        if (this.app.technologies.is_visible()) {
            this.app.background.show();
        } else {
            this.app.background.hide();
        }
    }
}
