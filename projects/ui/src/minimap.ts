/// <reference path='./engine.ts' />
/// <reference path='./panel.ts' />

class Minimap extends Panel {
    public constructor(app: App, panel: JQuery) {
        super(app, panel);

        this.panel_body.on('mousedown', this.on_mouse_down.bind(this));
        this.panel_body.on('mousemove', this.on_mouse_move.bind(this));
        this.panel_body.on('mouseup', this.on_mouse_up.bind(this));

        // TODO: Fix and test.
        /*
        this.panel_body.on('mousewheel', this.on_mouse_wheel.bind(this));
        */
    }

    private down: boolean = false;

    private down_x: number = 0;
    private down_y: number = 0;

    private previous_x: number = 0;
    private previous_y: number = 0;

    private on_mouse_down(e: JQuery.MouseDownEvent) {
        e.preventDefault();

        this.down = true;

        this.down_x = e.originalEvent.offsetX;
        this.down_y = e.originalEvent.offsetY;

        this.previous_x = this.down_x;
        this.previous_y = this.down_y;
    }

    private on_mouse_move(e: JQuery.MouseMoveEvent) {
        e.preventDefault();

        if (this.down) {
            let current_x = e.originalEvent.offsetX;
            let current_y = e.originalEvent.offsetY;

            Engine.minimap_move(current_x - this.previous_x, current_y - this.previous_y);

            this.previous_x = current_x;
            this.previous_y = current_y;
        }
    }

    private on_mouse_up(e: JQuery.MouseUpEvent) {
        e.preventDefault();

        this.down = false;

        if (this.down_x === e.originalEvent.offsetX && this.down_y === e.originalEvent.offsetY) {
            Engine.minimap_click(this.down_x, this.down_y);
        }
    }

    private on_mouse_wheel(e: JQuery.ScrollEvent) {
        e.preventDefault();

        // TODO: Fix and test.
        /*
        if (e.originalEvent.wheelDelta > 0) {
            Engine.minimap_zoom_in();
        } else {
            Engine.minimap_zoom_out();
        }
        */
    }
}
