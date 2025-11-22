/// <reference path='./engine.ts' />
/// <reference path='./panel.ts' />

class Preview extends Panel {
    public set_position(left: number, top: number, id: number) {
        this.panel.css({
            'left': left,
            'top': top
        });

        // TODO: Hardcoded.
        Engine.preview_set_position(left + 12, top + 38, id);
    }

    public show () {
        super.show();

        Engine.preview_show();
    }

    public hide() {
        super.hide();

        Engine.preview_hide();
    }
}
