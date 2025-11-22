/// <reference path='./engine.ts' />

interface Bar {
    name: string;
    value: number;
}

interface FloatingDescription {
    id: number;
    name: string;
    left: number;
    top: number;
    bars: Bar[];
}

class UI {
    public constructor(app: App) {
        this.app = app;
    }

    public is_mouse_inside(x: number, y: number): boolean {
        let is_mouse_inside: boolean = false;

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

    public set_floating_descriptions(data: Array<FloatingDescription>) {
        jQuery('.floating-description, .floating-description').remove();

        data.forEach((value: FloatingDescription) => {
            let root = jQuery('<div class="centered floating-description">')
                .attr('id', 'object_' + value.id)
                .css({ left: value.left, top: value.top })
                .appendTo(jQuery('body'));

            jQuery('<div class="text name">').text(value.name).appendTo(root);

            value.bars.forEach((bar: Bar) => {
                let bar_element = jQuery('<div class="bar ' + bar.name + '">').appendTo(root);

                jQuery('<div class="background">').appendTo(bar_element);
                jQuery('<div class="current">').css({ width: bar.value + '%' }).appendTo(bar_element);
            });
        });
    }

    private app: App;
}
