declare function engine_application_quit(): void;
declare function engine_application_save_options(options: string): void;

declare function engine_game_load(map_name: string): void;
declare function engine_game_new(map_name: string): void;
declare function engine_game_save(): void;
declare function engine_game_quit(): void;

declare function engine_map_set_data_layer(layer_name: string, data_layer_name: string): void;

declare function engine_minimap_click(x: number, y: number): void;
declare function engine_minimap_move(x: number, y: number): void;
declare function engine_minimap_zoom_in(): void;
declare function engine_minimap_zoom_out(): void;

declare function engine_preview_hide(): void;
declare function engine_preview_set_position(x: number, y: number, id: number): void;
declare function engine_preview_show(): void;

declare function engine_technology_research(technology_name: string): void;

declare function engine_ui_look_at_object(data: number): void;
declare function engine_ui_select_object(data: number): void;
declare function engine_ui_set_configuration(configuration_name: string): void;
declare function engine_ui_set_order(order_name: string): void;
declare function engine_ui_set_skill(skill_name: string): void;

class Engine {
    public static application_save_options(options: string) {
        Engine.call(engine_application_save_options, options);
    }

    public static application_quit() {
        Engine.call(engine_application_quit);
    }

    public static game_load(map_name: string) {
        Engine.call(engine_game_load, map_name);
    }

    public static game_new(map_name: string) {
        Engine.call(engine_game_new, map_name);
    }

    public static game_save() {
        Engine.call(engine_game_save);
    }

    public static game_quit() {
        Engine.call(engine_game_quit);
    }

    public static map_set_data_layer(layer_name: string, data_layer_name: string) {
        Engine.call(engine_map_set_data_layer, layer_name, data_layer_name);
    }

    public static minimap_click(x: number, y: number) {
        Engine.call(engine_minimap_click, x, y);
    }

    public static minimap_move(x: number, y: number) {
        Engine.call(engine_minimap_move, x, y);
    }

    public static minimap_zoom_in() {
        Engine.call(engine_minimap_zoom_in);
    }

    public static minimap_zoom_out() {
        Engine.call(engine_minimap_zoom_out);
    }

    public static preview_hide() {
        Engine.call(engine_preview_hide);
    }

    public static preview_set_position(x: number, y: number, id: number) {
        Engine.call(engine_preview_set_position, x, y, id);
    }

    public static preview_show() {
        Engine.call(engine_preview_show);
    }

    public static technology_research(technology_name: string) {
        Engine.call(engine_technology_research, technology_name);
    }

    public static ui_look_at_object(id: number) {
        Engine.call(engine_ui_look_at_object, id);
    }

    public static ui_select_object(id: number) {
        Engine.call(engine_ui_select_object, id);
    }

    public static ui_set_configuration(configuration_name: string) {
        Engine.call(engine_ui_set_configuration, configuration_name);
    }

    public static ui_set_order(order_name: string) {
        Engine.call(engine_ui_set_order, order_name);
    }

    public static ui_set_skill(skill_name: string) {
        Engine.call(engine_ui_set_skill, skill_name);
    }

    private static call(function_name: any, ...args: any[]) {
        function_name(...args);

        // TODO: Check if function exists.
        /*
        if (typeof function_name === 'function') {
            function_name(args);
        } else {
            console.warn('Function ' + function_name + ' not found.');
        }
        */
    }
}
