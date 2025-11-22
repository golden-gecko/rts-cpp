/// <reference path='./panel.ts' />
/// <reference path='./utils.ts' />

class Players extends Panel {
    public set(data: Array<object>) {
        Utils.set_rows(this.panel_body.find('tbody'), data);
    }
}
