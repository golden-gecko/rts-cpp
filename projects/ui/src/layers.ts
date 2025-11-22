/// <reference path='./engine.ts' />
/// <reference path='./panel.ts' />
/// <reference path='./utils.ts' />

interface DataLayer {
    name: string;
    selected: boolean;
}

class Layers extends Panel {
    public set(data: object) {
        this.panel_placeholder.empty();

        let row = jQuery('<div class="row">').appendTo(this.panel_placeholder);

        jQuery.each(data, (layer_name: string, data_layers: object) => {
            let layer = jQuery('<div class="col-4">').text(layer_name);

            jQuery.each(data_layers, (data_layer_name: string, data_layer: DataLayer) => {
                this.add_data_layer(layer_name, data_layer_name, data_layer.selected, layer);
            });

            layer.appendTo(row);
        });

        this.panel_placeholder.find('input').on('change', this.on_change.bind(this));
    }

    private add_data_layer(layer_name: string, data_layer_name: string, selected: boolean, parent: JQuery) {
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

    private on_change(e: JQuery.ChangeEvent) {
        e.preventDefault();

        let element = jQuery(e.target);
        let layer_name = element.attr('data-layer-name');
        let data_layer_name = element.attr('data-data-layer-name');

        Engine.map_set_data_layer(layer_name, data_layer_name);

        this.hide();
        this.app.background.hide();
    }
}
