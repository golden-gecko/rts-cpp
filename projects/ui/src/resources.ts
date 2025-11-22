/// <reference path='./panel.ts' />
/// <reference path='./utils.ts' />

class Resources extends Panel {
    public set(data: Array<object>) {
        Utils.set_rows(this.panel_body.find('tbody'), data);

        this.panel_body.find('.per_second').each((index: number, element: HTMLElement) => {
            let per_second_element = jQuery(element);
            let per_second = parseFloat(per_second_element.text());

            if (per_second > 0.0) {
                this.panel_body.find('.per_second:eq(' + index + ')').addClass('green');
            } else if (per_second < 0.0) {
                this.panel_body.find('.per_second:eq(' + index + ')').addClass('red');
            }

            per_second_element.text(per_second.toFixed(1));
        });
    }
}
