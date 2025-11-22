/// <reference path='./engine.ts' />
/// <reference path='./panel.ts' />

interface Technology {
    configuration: string;
    name: string;
    locked: boolean;
    unlocks?: string[];
    obsoletes?: string[];
}

class Technologies extends Panel {
    public set(data: Array<Technology>) {
        this.panel_placeholder.empty();

        data.forEach((technology: Technology) => {
            let button = jQuery('<button class="button-quit-game btn btn-light">')
                .attr('data-name', technology.configuration)
                .text(technology.name)
                .appendTo(this.panel_placeholder);

            if (technology.locked) {
                button.addClass('btn-outline-secondary');
            }

            button.on('click', function (e: JQuery.ClickEvent) {
                e.preventDefault();

                Engine.technology_research(jQuery(this).attr('data-name'));
            }).on('mouseenter', (e: JQuery.MouseEnterEvent) => {
                e.preventDefault();

                this.panel_placeholder
                    .find('button')
                    .removeClass('btn-outline-success')
                    .removeClass('btn-outline-danger');

                this.highlight(jQuery(e.originalEvent.target).attr('data-name'));
            }).on('mouseleave', (e: JQuery.MouseLeaveEvent) => {
                e.preventDefault();

                this.panel_placeholder
                    .find('button')
                    .removeClass('btn-outline-success')
                    .removeClass('btn-outline-danger');
            });
        });

        this.technologies = data;
    }

    private technologies: Technology[];

    private highlight(technology_name: string) {
        let button = jQuery(this.panel_placeholder.find('button[data-name="' + technology_name +'"]'));

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

    private highlight_requirements(technology_name: string) {
        jQuery(this.panel_placeholder.find('button[data-name="' + technology_name +'"]'))
            .addClass('btn-outline-danger');
    }

    private highlight_unlocks(unlocks: string[]) {
        for (let unlock of unlocks) {
            jQuery(this.panel_placeholder.find('button[data-name="' + unlock + '"]'))
                .addClass('btn-outline-success');
        }
    }
}
