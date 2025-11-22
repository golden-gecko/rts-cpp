/// <reference path='./panel.ts' />
/// <reference path='./utils.ts' />

class OrdersAdmin extends Panel {
    public set(data: Array<object>) {
        // TODO: Fix and test.
        /*
        Utils.set_data_table(this.panel_body.find('table'), data);

        let orders = this.panel_body.find('.receiver_id, .sender_id');

        jQuery.each(orders, function k, v) {
            jQuery(this).attr('data-id', jQuery(v).text());
        });

        Utils.add_data_id_events(this.app, orders);
        */
    }
}
