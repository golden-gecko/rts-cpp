/// <reference path='./app.ts' />

class Demo {
    public constructor(app: App) {
        app.configurations.set([
            'coal_mine',
            'iron_mine',
            'vehicle_factory'
        ]);

        app.diplomacy.set({
            'CPU 1': {
                'CPU 1': 'Ally',
                'CPU 2': 'Enemy',
                'World': 'Ally'
            },
            'CPU 2': {
                'CPU 1': 'Enemy',
                'CPU 2': 'Ally',
                'World': 'Ally'
            },
            'World': {
                'CPU 1': 'Ally',
                'CPU 2': 'Ally',
                'World': 'Ally'
            }
        });

        app.info.set({
            'name': 'Tank',
            'position': '10.0:20.0:.30.0',
            'id': 1,
            'player_id': 3,
            'fps': 30.0,
            'time': 12.34567,
            'memory': '20.3 MB',
            'health': {
                'current': 50.0,
                'max': 80.0
            },
            'components': [
                {
                    'name': 'Cannon',
                    'damage': 3.4
                },
                {
                    'name': 'Tracked',
                    'health': {
                        'current': 10.0,
                        'max': 20.0
                    }
                }
            ],
            "processes": {
                "Smelting": {
                    "in": {
                        "Coal": {
                            "consumption": 4.0
                        },
                        "Iron Ore": {
                            "consumption": 2.0
                        }
                    },
                    "out": {
                        "Iron": {
                            "production": 1.0
                        },
                        "Pollution": {
                            "production": 0.2
                        }
                    }
                }
            },
            "resources": {
                "items": {
                    "Coal": {
                        "maximal": 10.0
                    },
                    "Iron": {
                        "maximal": 10.0
                    }
                }
            }
        });

        app.layers.set({
            'Air': {
                'None': { 'name': 'None', 'selected': true },
                'Pollution': { 'name': 'Pollution' },
            },
            'Terrain': {
                'None': { 'name': 'None' },
                'Height': { 'name': 'Height' },
                'Navigation': { 'name': 'Navigation' },
                'Type': {'name': 'Type', 'selected': true }
            },
            'Water': {
                'None': { 'name': 'None' },
                'Height': { 'name': 'Height', 'selected': true },
                'Pollution': { 'name': 'Pollution' },
                'Type': { 'name': 'Type' }
            }
        });

        app.map_menu.set_maps([
            'tutorial #1',
            'tutorial #2',
            'level #1',
            'level #2',
            'level #3'
        ]);

        app.load_menu.set_saves([
            '2017_10_12_12_51_41',
            '2018_10_12_12_51_41',
            '2019_10_12_12_51_41',
            '2020_10_03_12_51_19',
            '2020_11_10_13_21_33'
        ]);

        app.log.info('Game started');
        app.log.info('Tank created', 2);
        app.log.error('Power plant destroyed', 8);

        app.objects_admin.set([
            { 'current_order': 'None', 'id': 10, 'name': 'Coal Mine', 'order_count': 2 },
            { 'current_order': 'None', 'id': 11, 'name': 'Iron Mine', 'order_count': 4 },
            { 'current_order': 'Move', 'id': 12, 'name': 'Solar Power Plant', 'order_count': 1 }
        ]);

        app.orders.set([
            'Attack',
            'Move',
            'Stop'
        ]);

        app.orders_admin.set([
            { 'id': 30, 'sender_id': 3, 'receiver_id': 8, 'type': 'Attack' },
            { 'id': 31, 'sender_id': 4, 'receiver_id': 3, 'type': 'Move' },
            { 'id': 32, 'sender_id': 6, 'receiver_id': 4, 'type': 'Patrol' }
        ]);

        app.players.set([
            { 'id': 1, 'color': 'green', 'name': 'CPU 1' },
            { 'id': 2, 'color': 'yellow', 'name': 'CPU 2' },
            { 'id': 3, 'color': 'red', 'name': 'Human' }
        ]);

        app.resources.set([
            { 'name': 'Coal', 'production': 10.0, 'max': 100.0, 'per_second': 2.0 },
            { 'name': 'Energy', 'production': 2.0, 'max': 200.0, 'per_second': -2.5 },
            { 'name': 'Iron', 'production': 2.0, 'max': 200.0, 'per_second': 3.5 }
        ]);

        app.skills.set([
            'first_aid_kit',
            'rage',
            'repair'
        ]);

        app.statistics.set([
            { 'name': 'Mouse clicks', 'value': 30 },
            { 'name': 'Objects created', 'value': 250 },
            { 'name': 'Orders completed', 'value': 1080 },
            { 'name': 'Distance driven', 'value': 1080.345 }
        ]);

        app.technologies.set([
            {
                'configuration': 'basic_warfare',
                'name': 'Basic Warfare',
                'locked': false,
                'unlocks': ['rifle']
            },
            {
                'configuration': 'optics',
                'name': 'Optics',
                'locked': false,
                'unlocks': ['lasers']
            },
            {
                'configuration': 'lasers',
                'name': 'Lasers',
                'locked': false,
                'unlocks': ['laser_force_field', 'laser_gun']
            },
            {
                'configuration': 'laser_force_field',
                'name': 'Laser Force Field',
                'locked': true,
                'unlocks': ['laser_wall']
            },
            {
                'configuration': 'laser_wall',
                'name': 'Laser Wall',
                'locked': true
            },
            {
                'configuration': 'laser_gun',
                'name': 'Laser Gun',
                'locked': true
            },
            {
                'configuration': 'Advanced Warfare',
                'name': 'Advanced Warfare',
                'locked': true,
                'obsoletes': ['rifle']
            }
        ]);

        app.ui.set_floating_descriptions([
            {
                'id': 1,
                'name': 'Tank',
                'left': 350,
                'top': 400,
                'bars': [
                    { 'name': 'health', 'value': 90 },
                    { 'name': 'fuel', 'value': 20 },
                    { 'name': 'ammunition', 'value': 25 },
                    { 'name': 'reload', 'value': 30 }
                ]
            },
            {
                'id': 2,
                'name': 'Truck',
                'left': 650,
                'top': 300,
                'bars': [
                    { 'name': 'health', 'value': 40 },
                    { 'name': 'fuel', 'value': 50 }
                ]
            },
            {
                'id': 7,
                'name': 'House',
                'left': 490,
                'top': 440,
                'bars': [
                    { 'name': 'health', 'value': 70 }
                ]
            },
            {
                'id': 8,
                'name': 'Solar Power Plant',
                'left': 250,
                'top': 480,
                'bars': [
                    { 'name': 'health', 'value': 30 }
                ]
            }
        ]);
    }
}
