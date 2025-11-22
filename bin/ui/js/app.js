"use strict";
class Panel {
    constructor(app, panel) {
        this.app = app;
        this.panel = panel;
        this.panel_header = panel.find('.header');
        this.panel_body = panel.find('.body');
        this.panel_placeholder = panel.find('.placeholder');
        this.panel_body.find('.button-close').on('click', this.on_close.bind(this));
    }
    hide() {
        this.panel.hide();
    }
    is_visible() {
        return this.panel.is(':visible');
    }
    set_header(name) {
        this.panel_header.text(name);
    }
    show() {
        this.panel.show();
    }
    toggle() {
        this.panel.toggle();
    }
    on_close(e) {
        e.preventDefault();
        this.hide();
        this.app.background.hide();
    }
}
class Background extends Panel {
}
class Engine {
    static application_save_options(options) {
        Engine.call(engine_application_save_options, options);
    }
    static application_quit() {
        Engine.call(engine_application_quit);
    }
    static game_load(map_name) {
        Engine.call(engine_game_load, map_name);
    }
    static game_new(map_name) {
        Engine.call(engine_game_new, map_name);
    }
    static game_save() {
        Engine.call(engine_game_save);
    }
    static game_quit() {
        Engine.call(engine_game_quit);
    }
    static map_set_data_layer(layer_name, data_layer_name) {
        Engine.call(engine_map_set_data_layer, layer_name, data_layer_name);
    }
    static minimap_click(x, y) {
        Engine.call(engine_minimap_click, x, y);
    }
    static minimap_move(x, y) {
        Engine.call(engine_minimap_move, x, y);
    }
    static minimap_zoom_in() {
        Engine.call(engine_minimap_zoom_in);
    }
    static minimap_zoom_out() {
        Engine.call(engine_minimap_zoom_out);
    }
    static preview_hide() {
        Engine.call(engine_preview_hide);
    }
    static preview_set_position(x, y, id) {
        Engine.call(engine_preview_set_position, x, y, id);
    }
    static preview_show() {
        Engine.call(engine_preview_show);
    }
    static technology_research(technology_name) {
        Engine.call(engine_technology_research, technology_name);
    }
    static ui_look_at_object(id) {
        Engine.call(engine_ui_look_at_object, id);
    }
    static ui_select_object(id) {
        Engine.call(engine_ui_select_object, id);
    }
    static ui_set_configuration(configuration_name) {
        Engine.call(engine_ui_set_configuration, configuration_name);
    }
    static ui_set_order(order_name) {
        Engine.call(engine_ui_set_order, order_name);
    }
    static ui_set_skill(skill_name) {
        Engine.call(engine_ui_set_skill, skill_name);
    }
    static call(function_name, ...args) {
        function_name(...args);
    }
}
class Utils {
    static add_data_id_events(app, element) {
        element.on('click', function (e) {
            e.preventDefault();
            Engine.ui_select_object(parseInt(jQuery(this).attr('data-id')));
        }).on('dblclick', function (e) {
            e.preventDefault();
            Engine.ui_look_at_object(parseInt(jQuery(this).attr('data-id')));
        }).on('mouseenter', function (e) {
            e.preventDefault();
            app.preview.show();
        }).on('mouseleave', function (e) {
            e.preventDefault();
            app.preview.hide();
        });
    }
    static capitalize(value) {
        return value.charAt(0).toUpperCase() + value.slice(1);
    }
    static format_title(title) {
        let words = title.split('_');
        words.forEach((word, index) => {
            words[index] = Utils.capitalize(word);
        });
        return words.join(' ');
    }
    static is_float(value) {
        return Number(value) === value && value % 1 !== 0;
    }
    static set_buttons(placeholder, items) {
        placeholder.empty();
        items.forEach((value) => {
            jQuery('<button type="button" class="btn btn-light">')
                .attr('data-name', value)
                .html(Utils.format_title(value))
                .appendTo(placeholder);
            placeholder.append(' ');
        });
    }
    static set_rows(placeholder, items) {
        placeholder.empty();
        items.forEach((value) => {
            let tr = jQuery('<tr>').appendTo(placeholder);
            jQuery.each(value, function (key, value) {
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
class Configurations extends Panel {
    set(data) {
        Utils.set_buttons(this.panel_placeholder, data);
        this.panel_body.find('button').on('click', this.on_click.bind(this));
    }
    set_header(name) {
        this.panel_header.text('Configurations - ' + Utils.format_title(name));
    }
    on_click(e) {
        e.preventDefault();
        Engine.ui_set_configuration(jQuery(e.target).attr('data-name'));
    }
}
class Diplomacy extends Panel {
    set(data) {
    }
}
class GameMenu extends Panel {
    constructor(app, panel) {
        super(app, panel);
        this.panel_body.find('.button-diplomacy').on('click', this.on_diplomacy.bind(this));
        this.panel_body.find('.button-layers').on('click', this.on_layers.bind(this));
        this.panel_body.find('.button-main-menu').on('click', this.on_main_menu.bind(this));
        this.panel_body.find('.button-objects').on('click', this.on_objects.bind(this));
        this.panel_body.find('.button-orders').on('click', this.on_orders.bind(this));
        this.panel_body.find('.button-statistics').on('click', this.on_statistics.bind(this));
        this.panel_body.find('.button-technologies').on('click', this.on_technologies.bind(this));
    }
    hide_all(exclude) {
        let panels = [
            this.app.diplomacy,
            this.app.layers,
            this.app.main_menu,
            this.app.objects_admin,
            this.app.orders_admin,
            this.app.statistics,
            this.app.technologies
        ];
        for (let i = 0; i < panels.length; ++i) {
            if (panels[i] !== exclude) {
                panels[i].hide();
            }
        }
    }
    on_diplomacy(e) {
        e.preventDefault();
        this.hide_all(this.app.diplomacy);
        this.app.diplomacy.toggle();
        if (this.app.diplomacy.is_visible()) {
            this.app.background.show();
        }
        else {
            this.app.background.hide();
        }
    }
    on_layers(e) {
        e.preventDefault();
        this.hide_all(this.app.layers);
        this.app.layers.toggle();
        if (this.app.layers.is_visible()) {
            this.app.background.show();
        }
        else {
            this.app.background.hide();
        }
    }
    on_main_menu(e) {
        e.preventDefault();
        this.hide_all(this.app.main_menu);
        this.app.main_menu.toggle();
        if (this.app.main_menu.is_visible()) {
            this.app.background.show();
        }
        else {
            this.app.background.hide();
        }
    }
    on_objects(e) {
        e.preventDefault();
        this.hide_all(this.app.objects_admin);
        this.app.objects_admin.toggle();
        if (this.app.objects_admin.is_visible()) {
            this.app.background.show();
        }
        else {
            this.app.background.hide();
        }
    }
    on_orders(e) {
        e.preventDefault();
        this.hide_all(this.app.orders);
        this.app.orders.toggle();
        if (this.app.orders.is_visible()) {
            this.app.background.show();
        }
        else {
            this.app.background.hide();
        }
    }
    on_statistics(e) {
        e.preventDefault();
        this.hide_all(this.app.statistics);
        this.app.statistics.toggle();
        if (this.app.statistics.is_visible()) {
            this.app.background.show();
        }
        else {
            this.app.background.hide();
        }
    }
    on_technologies(e) {
        e.preventDefault();
        this.hide_all(this.app.technologies);
        this.app.technologies.toggle();
        if (this.app.technologies.is_visible()) {
            this.app.background.show();
        }
        else {
            this.app.background.hide();
        }
    }
}
class Info extends Panel {
    set(data) {
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
    }
}
class Layers extends Panel {
    set(data) {
        this.panel_placeholder.empty();
        let row = jQuery('<div class="row">').appendTo(this.panel_placeholder);
        jQuery.each(data, (layer_name, data_layers) => {
            let layer = jQuery('<div class="col-4">').text(layer_name);
            jQuery.each(data_layers, (data_layer_name, data_layer) => {
                this.add_data_layer(layer_name, data_layer_name, data_layer.selected, layer);
            });
            layer.appendTo(row);
        });
        this.panel_placeholder.find('input').on('change', this.on_change.bind(this));
    }
    add_data_layer(layer_name, data_layer_name, selected, parent) {
        let checkbox = jQuery('<div class="form-check">');
        let input_id = layer_name + '_' + data_layer_name;
        jQuery('<input class="form-check-input" type="radio">')
            .attr('data-layer-name', layer_name)
            .attr('data-data-layer-name', data_layer_name)
            .attr('id', input_id)
            .attr('name', layer_name)
            .prop('checked', selected)
            .appendTo(checkbox);
        jQuery('<label class="form-check-label">')
            .attr('for', input_id)
            .text(Utils.capitalize(data_layer_name))
            .appendTo(checkbox);
        checkbox.appendTo(parent);
    }
    on_change(e) {
        e.preventDefault();
        let element = jQuery(e.target);
        let layer_name = element.attr('data-layer-name');
        let data_layer_name = element.attr('data-data-layer-name');
        Engine.map_set_data_layer(layer_name, data_layer_name);
        this.hide();
        this.app.background.hide();
    }
}
class LoadMenu extends Panel {
    constructor(app, panel) {
        super(app, panel);
        this.panel.find('.button-continue').on('click', this.on_continue.bind(this));
        this.panel.find('.button-quit-game').on('click', this.on_quit_game.bind(this));
        this.panel.find('.button-quit-application').on('click', this.on_quit_application.bind(this));
    }
    set_saves(saves) {
        this.panel_placeholder.empty();
        saves.forEach((value) => {
            jQuery('<button class="btn btn-light">')
                .attr('data-name', value)
                .text(value)
                .appendTo(this.panel_placeholder)
                .on('click', this.on_save.bind(this));
        });
    }
    on_continue(e) {
        e.preventDefault();
        this.app.load_menu.hide();
        this.app.background.hide();
    }
    on_save(e) {
        e.preventDefault();
        Engine.game_load(jQuery(this).attr('data-name'));
        this.app.load_menu.hide();
        this.app.background.hide();
    }
    on_quit_game(e) {
        e.preventDefault();
        Engine.game_quit();
    }
    on_quit_application(e) {
        e.preventDefault();
        Engine.application_quit();
    }
}
class Log extends Panel {
    constructor(app, panel) {
        super(app, panel);
        this.panel.on('mouseenter', function (e) {
            e.preventDefault();
            jQuery(this).css({
                'bottom': '5px',
                'height': 'auto',
                'overflow-y': 'auto'
            });
        }).on('mouseleave', function (e) {
            e.preventDefault();
            jQuery(this).css({
                'bottom': 'auto',
                'height': '30px',
                'overflow-y': 'hidden'
            });
        });
    }
    error(text, id) {
        return this.write(text, 'error', id);
    }
    info(text, id) {
        return this.write(text, 'info', id);
    }
    write(text, type, id) {
        let today = new Date();
        let time = today.getHours().toString().padStart(2, '0')
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
class MainMenu extends Panel {
    constructor(app, panel) {
        super(app, panel);
        this.panel.find('.button-continue').on('click', this.on_continue.bind(this));
        this.panel.find('.button-new-game').on('click', this.on_new_game.bind(this));
        this.panel.find('.button-save').on('click', this.on_save.bind(this));
        this.panel.find('.button-load').on('click', this.on_load.bind(this));
        this.panel.find('.button-options').on('click', this.on_options.bind(this));
        this.panel.find('.button-quit-game').on('click', this.on_quit_game.bind(this));
        this.panel.find('.button-quit-application').on('click', this.on_quit_application.bind(this));
    }
    on_continue(e) {
        e.preventDefault();
        this.app.main_menu.hide();
        this.app.background.hide();
    }
    on_new_game(e) {
        e.preventDefault();
        this.app.map_menu.show();
        this.app.main_menu.hide();
    }
    on_save(e) {
        e.preventDefault();
        Engine.game_save();
        this.app.main_menu.hide();
        this.app.background.hide();
    }
    on_load(e) {
        e.preventDefault();
        this.app.load_menu.show();
        this.app.main_menu.hide();
    }
    on_options(e) {
        e.preventDefault();
        this.app.main_menu.hide();
        this.app.options.show();
    }
    on_quit_game(e) {
        e.preventDefault();
        Engine.game_quit();
    }
    on_quit_application(e) {
        e.preventDefault();
        Engine.application_quit();
    }
}
class MapMenu extends Panel {
    constructor(app, panel) {
        super(app, panel);
        this.panel.find('.button-continue').on('click', this.on_continue.bind(this));
        this.panel.find('.button-quit-game').on('click', this.on_quit_game.bind(this));
        this.panel.find('.button-quit-application').on('click', this.on_quit_application.bind(this));
    }
    set_maps(maps) {
        this.panel_placeholder.empty();
        maps.forEach((value) => {
            jQuery('<button class="btn btn-light">')
                .attr('data-name', value)
                .text(value)
                .appendTo(this.panel_placeholder)
                .on('click', this.on_map.bind(this));
        });
    }
    on_continue(e) {
        e.preventDefault();
        this.app.map_menu.hide();
        this.app.background.hide();
    }
    on_map(e) {
        e.preventDefault();
        Engine.game_new(jQuery(this).attr('data-name'));
        this.app.map_menu.hide();
        this.app.background.hide();
    }
    on_quit_game(e) {
        e.preventDefault();
        Engine.game_quit();
    }
    on_quit_application(e) {
        e.preventDefault();
        Engine.application_quit();
    }
}
class Minimap extends Panel {
    constructor(app, panel) {
        super(app, panel);
        this.down = false;
        this.down_x = 0;
        this.down_y = 0;
        this.previous_x = 0;
        this.previous_y = 0;
        this.panel_body.on('mousedown', this.on_mouse_down.bind(this));
        this.panel_body.on('mousemove', this.on_mouse_move.bind(this));
        this.panel_body.on('mouseup', this.on_mouse_up.bind(this));
    }
    on_mouse_down(e) {
        e.preventDefault();
        this.down = true;
        this.down_x = e.originalEvent.offsetX;
        this.down_y = e.originalEvent.offsetY;
        this.previous_x = this.down_x;
        this.previous_y = this.down_y;
    }
    on_mouse_move(e) {
        e.preventDefault();
        if (this.down) {
            let current_x = e.originalEvent.offsetX;
            let current_y = e.originalEvent.offsetY;
            Engine.minimap_move(current_x - this.previous_x, current_y - this.previous_y);
            this.previous_x = current_x;
            this.previous_y = current_y;
        }
    }
    on_mouse_up(e) {
        e.preventDefault();
        this.down = false;
        if (this.down_x === e.originalEvent.offsetX && this.down_y === e.originalEvent.offsetY) {
            Engine.minimap_click(this.down_x, this.down_y);
        }
    }
    on_mouse_wheel(e) {
        e.preventDefault();
    }
}
class ObjectsAdmin extends Panel {
    set(data) {
    }
}
class Options extends Panel {
    constructor(app, panel) {
        super(app, panel);
        this.panel_body.find('.button-save').on('click', this.on_save.bind(this));
    }
    on_save(e) {
        e.preventDefault();
        Engine.application_save_options(this.panel_body.find('form').serialize());
        this.app.options.hide();
        this.app.main_menu.show();
    }
}
class Orders extends Panel {
    set(data) {
        Utils.set_buttons(this.panel_body, data);
        this.panel_body.find('button').on('click', this.on_click.bind(this));
    }
    set_header(name) {
        this.panel_header.text('Orders - ' + Utils.format_title(name));
    }
    on_click(e) {
        e.preventDefault();
        Engine.ui_set_order(jQuery(e.target).attr('data-name'));
    }
}
class OrdersAdmin extends Panel {
    set(data) {
    }
}
class Players extends Panel {
    set(data) {
        Utils.set_rows(this.panel_body.find('tbody'), data);
    }
}
class Preview extends Panel {
    set_position(left, top, id) {
        this.panel.css({
            'left': left,
            'top': top
        });
        Engine.preview_set_position(left + 12, top + 38, id);
    }
    show() {
        super.show();
        Engine.preview_show();
    }
    hide() {
        super.hide();
        Engine.preview_hide();
    }
}
class Resources extends Panel {
    set(data) {
        Utils.set_rows(this.panel_body.find('tbody'), data);
        this.panel_body.find('.per_second').each((index, element) => {
            let per_second_element = jQuery(element);
            let per_second = parseFloat(per_second_element.text());
            if (per_second > 0.0) {
                this.panel_body.find('.per_second:eq(' + index + ')').addClass('green');
            }
            else if (per_second < 0.0) {
                this.panel_body.find('.per_second:eq(' + index + ')').addClass('red');
            }
            per_second_element.text(per_second.toFixed(1));
        });
    }
}
class Skills extends Panel {
    set(data) {
        Utils.set_buttons(this.panel_body, data);
        this.panel_body.find('button').on('click', this.on_click.bind(this));
    }
    set_header(name) {
        this.panel_header.text('Skills - ' + Utils.format_title(name));
    }
    on_click(e) {
        e.preventDefault();
        Engine.ui_set_skill(jQuery(e.target).attr('data-name'));
    }
}
class Statistics extends Panel {
    set(data) {
        Utils.set_rows(this.panel_placeholder, data);
    }
}
class Technologies extends Panel {
    set(data) {
        this.panel_placeholder.empty();
        data.forEach((technology) => {
            let button = jQuery('<button class="button-quit-game btn btn-light">')
                .attr('data-name', technology.configuration)
                .text(technology.name)
                .appendTo(this.panel_placeholder);
            if (technology.locked) {
                button.addClass('btn-outline-secondary');
            }
            button.on('click', function (e) {
                e.preventDefault();
                Engine.technology_research(jQuery(this).attr('data-name'));
            }).on('mouseenter', (e) => {
                e.preventDefault();
                this.panel_placeholder
                    .find('button')
                    .removeClass('btn-outline-success')
                    .removeClass('btn-outline-danger');
                this.highlight(jQuery(e.originalEvent.target).attr('data-name'));
            }).on('mouseleave', (e) => {
                e.preventDefault();
                this.panel_placeholder
                    .find('button')
                    .removeClass('btn-outline-success')
                    .removeClass('btn-outline-danger');
            });
        });
        this.technologies = data;
    }
    highlight(technology_name) {
        let button = jQuery(this.panel_placeholder.find('button[data-name="' + technology_name + '"]'));
        for (let technology of this.technologies) {
            if (technology.configuration == technology_name) {
                if (technology.unlocks) {
                    this.highlight_unlocks(technology.unlocks);
                }
            }
            if (technology.unlocks && technology.unlocks.includes(technology_name)) {
                this.highlight_requirements(technology.configuration);
            }
        }
    }
    highlight_requirements(technology_name) {
        jQuery(this.panel_placeholder.find('button[data-name="' + technology_name + '"]'))
            .addClass('btn-outline-danger');
    }
    highlight_unlocks(unlocks) {
        for (let unlock of unlocks) {
            jQuery(this.panel_placeholder.find('button[data-name="' + unlock + '"]'))
                .addClass('btn-outline-success');
        }
    }
}
class UI {
    constructor(app) {
        this.app = app;
    }
    is_mouse_inside(x, y) {
        let is_mouse_inside = false;
        jQuery('.ui:visible').each(function () {
            let element = jQuery(this);
            let position = element.position();
            let x1 = x >= position.left;
            let x2 = x <= position.left + element.width();
            let y1 = y >= position.top;
            let y2 = y <= position.top + element.height();
            if (x1 && x2 && y1 && y2) {
                is_mouse_inside = true;
                return false;
            }
        });
        return is_mouse_inside;
    }
    set_floating_descriptions(data) {
        jQuery('.floating-description, .floating-description').remove();
        data.forEach((value) => {
            let root = jQuery('<div class="centered floating-description">')
                .attr('id', 'object_' + value.id)
                .css({ left: value.left, top: value.top })
                .appendTo(jQuery('body'));
            jQuery('<div class="text name">').text(value.name).appendTo(root);
            value.bars.forEach((bar) => {
                let bar_element = jQuery('<div class="bar ' + bar.name + '">').appendTo(root);
                jQuery('<div class="background">').appendTo(bar_element);
                jQuery('<div class="current">').css({ width: bar.value + '%' }).appendTo(bar_element);
            });
        });
    }
}
class App {
    constructor() {
        this.background = new Background(this, jQuery('#background'));
        this.configurations = new Configurations(this, jQuery('#configurations'));
        this.diplomacy = new Diplomacy(this, jQuery('#diplomacy'));
        this.game_menu = new GameMenu(this, jQuery('#game-menu'));
        this.info = new Info(this, jQuery('#info'));
        this.layers = new Layers(this, jQuery('#layers'));
        this.load_menu = new LoadMenu(this, jQuery('#load-menu'));
        this.log = new Log(this, jQuery('#log'));
        this.main_menu = new MainMenu(this, jQuery('#main-menu'));
        this.map_menu = new MapMenu(this, jQuery('#map-menu'));
        this.minimap = new Minimap(this, jQuery('#minimap'));
        this.objects_admin = new ObjectsAdmin(this, jQuery('#objects-admin'));
        this.options = new Options(this, jQuery('#options'));
        this.orders = new Orders(this, jQuery('#orders'));
        this.orders_admin = new OrdersAdmin(this, jQuery('#orders-admin'));
        this.players = new Players(this, jQuery('#players'));
        this.preview = new Preview(this, jQuery('#preview'));
        this.resources = new Resources(this, jQuery('#resources'));
        this.skills = new Skills(this, jQuery('#skills'));
        this.statistics = new Statistics(this, jQuery('#statistics'));
        this.technologies = new Technologies(this, jQuery('#technologies'));
        this.ui = new UI(this);
    }
}
class Demo {
    constructor(app) {
        app.configurations.set([
            'coal_mine',
            'iron_mine',
            'vehicle_factory'
        ]);
        app.diplomacy.set({
            'CPU 1': {
                'CPU 1': 'Ally',
                'CPU 2': 'Enemy',
                'World': 'Ally'
            },
            'CPU 2': {
                'CPU 1': 'Enemy',
                'CPU 2': 'Ally',
                'World': 'Ally'
            },
            'World': {
                'CPU 1': 'Ally',
                'CPU 2': 'Ally',
                'World': 'Ally'
            }
        });
        app.info.set({
            'name': 'Tank',
            'position': '10.0:20.0:.30.0',
            'id': 1,
            'player_id': 3,
            'fps': 30.0,
            'time': 12.34567,
            'memory': '20.3 MB',
            'health': {
                'current': 50.0,
                'max': 80.0
            },
            'components': [
                {
                    'name': 'Cannon',
                    'damage': 3.4
                },
                {
                    'name': 'Tracked',
                    'health': {
                        'current': 10.0,
                        'max': 20.0
                    }
                }
            ],
            "processes": {
                "Smelting": {
                    "in": {
                        "Coal": {
                            "consumption": 4.0
                        },
                        "Iron Ore": {
                            "consumption": 2.0
                        }
                    },
                    "out": {
                        "Iron": {
                            "production": 1.0
                        },
                        "Pollution": {
                            "production": 0.2
                        }
                    }
                }
            },
            "resources": {
                "items": {
                    "Coal": {
                        "maximal": 10.0
                    },
                    "Iron": {
                        "maximal": 10.0
                    }
                }
            }
        });
        app.layers.set({
            'Air': {
                'None': { 'name': 'None', 'selected': true },
                'Pollution': { 'name': 'Pollution' },
            },
            'Terrain': {
                'None': { 'name': 'None' },
                'Height': { 'name': 'Height' },
                'Navigation': { 'name': 'Navigation' },
                'Type': { 'name': 'Type', 'selected': true }
            },
            'Water': {
                'None': { 'name': 'None' },
                'Height': { 'name': 'Height', 'selected': true },
                'Pollution': { 'name': 'Pollution' },
                'Type': { 'name': 'Type' }
            }
        });
        app.map_menu.set_maps([
            'tutorial #1',
            'tutorial #2',
            'level #1',
            'level #2',
            'level #3'
        ]);
        app.load_menu.set_saves([
            '2017_10_12_12_51_41',
            '2018_10_12_12_51_41',
            '2019_10_12_12_51_41',
            '2020_10_03_12_51_19',
            '2020_11_10_13_21_33'
        ]);
        app.log.info('Game started');
        app.log.info('Tank created', 2);
        app.log.error('Power plant destroyed', 8);
        app.objects_admin.set([
            { 'current_order': 'None', 'id': 10, 'name': 'Coal Mine', 'order_count': 2 },
            { 'current_order': 'None', 'id': 11, 'name': 'Iron Mine', 'order_count': 4 },
            { 'current_order': 'Move', 'id': 12, 'name': 'Solar Power Plant', 'order_count': 1 }
        ]);
        app.orders.set([
            'Attack',
            'Move',
            'Stop'
        ]);
        app.orders_admin.set([
            { 'id': 30, 'sender_id': 3, 'receiver_id': 8, 'type': 'Attack' },
            { 'id': 31, 'sender_id': 4, 'receiver_id': 3, 'type': 'Move' },
            { 'id': 32, 'sender_id': 6, 'receiver_id': 4, 'type': 'Patrol' }
        ]);
        app.players.set([
            { 'id': 1, 'color': 'green', 'name': 'CPU 1' },
            { 'id': 2, 'color': 'yellow', 'name': 'CPU 2' },
            { 'id': 3, 'color': 'red', 'name': 'Human' }
        ]);
        app.resources.set([
            { 'name': 'Coal', 'production': 10.0, 'max': 100.0, 'per_second': 2.0 },
            { 'name': 'Energy', 'production': 2.0, 'max': 200.0, 'per_second': -2.5 },
            { 'name': 'Iron', 'production': 2.0, 'max': 200.0, 'per_second': 3.5 }
        ]);
        app.skills.set([
            'first_aid_kit',
            'rage',
            'repair'
        ]);
        app.statistics.set([
            { 'name': 'Mouse clicks', 'value': 30 },
            { 'name': 'Objects created', 'value': 250 },
            { 'name': 'Orders completed', 'value': 1080 },
            { 'name': 'Distance driven', 'value': 1080.345 }
        ]);
        app.technologies.set([
            {
                'configuration': 'basic_warfare',
                'name': 'Basic Warfare',
                'locked': false,
                'unlocks': ['rifle']
            },
            {
                'configuration': 'optics',
                'name': 'Optics',
                'locked': false,
                'unlocks': ['lasers']
            },
            {
                'configuration': 'lasers',
                'name': 'Lasers',
                'locked': false,
                'unlocks': ['laser_force_field', 'laser_gun']
            },
            {
                'configuration': 'laser_force_field',
                'name': 'Laser Force Field',
                'locked': true,
                'unlocks': ['laser_wall']
            },
            {
                'configuration': 'laser_wall',
                'name': 'Laser Wall',
                'locked': true
            },
            {
                'configuration': 'laser_gun',
                'name': 'Laser Gun',
                'locked': true
            },
            {
                'configuration': 'Advanced Warfare',
                'name': 'Advanced Warfare',
                'locked': true,
                'obsoletes': ['rifle']
            }
        ]);
        app.ui.set_floating_descriptions([
            {
                'id': 1,
                'name': 'Tank',
                'left': 350,
                'top': 400,
                'bars': [
                    { 'name': 'health', 'value': 90 },
                    { 'name': 'fuel', 'value': 20 },
                    { 'name': 'ammunition', 'value': 25 },
                    { 'name': 'reload', 'value': 30 }
                ]
            },
            {
                'id': 2,
                'name': 'Truck',
                'left': 650,
                'top': 300,
                'bars': [
                    { 'name': 'health', 'value': 40 },
                    { 'name': 'fuel', 'value': 50 }
                ]
            },
            {
                'id': 7,
                'name': 'House',
                'left': 490,
                'top': 440,
                'bars': [
                    { 'name': 'health', 'value': 70 }
                ]
            },
            {
                'id': 8,
                'name': 'Solar Power Plant',
                'left': 250,
                'top': 480,
                'bars': [
                    { 'name': 'health', 'value': 30 }
                ]
            }
        ]);
    }
}
