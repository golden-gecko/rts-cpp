/// <reference path='./panel.ts' />
/// <reference path='./utils.ts' />

class Statistics extends Panel {
    public set(data: Array<object>) {
        Utils.set_rows(this.panel_placeholder, data);
    }
}
