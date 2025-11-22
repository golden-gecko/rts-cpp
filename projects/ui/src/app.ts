/// <reference path='./background.ts' />
/// <reference path='./configurations.ts' />
/// <reference path='./diplomacy.ts' />
/// <reference path='./game_menu.ts' />
/// <reference path='./info.ts' />
/// <reference path='./layers.ts' />
/// <reference path='./load_menu.ts' />
/// <reference path='./log.ts' />
/// <reference path='./main_menu.ts' />
/// <reference path='./map_menu.ts' />
/// <reference path='./minimap.ts' />
/// <reference path='./objects_admin.ts' />
/// <reference path='./options.ts' />
/// <reference path='./orders.ts' />
/// <reference path='./orders_admin.ts' />
/// <reference path='./players.ts' />
/// <reference path='./preview.ts' />
/// <reference path='./resources.ts' />
/// <reference path='./skills.ts' />
/// <reference path='./statistics.ts' />
/// <reference path='./technologies.ts' />
/// <reference path='./ui.ts' />

class App {
    public background: Background;
    public configurations: Configurations;
    public diplomacy: Diplomacy;
    public game_menu: GameMenu;
    public info: Info;
    public layers: Layers
    public load_menu: LoadMenu;
    public log: Log;
    public main_menu: MainMenu;
    public map_menu: MapMenu;
    public minimap: Minimap;
    public objects_admin: ObjectsAdmin;
    public options: Options;
    public orders: Orders;
    public orders_admin: OrdersAdmin;
    public players: Players;
    public preview: Preview;
    public resources: Resources;
    public skills: Skills;
    public statistics: Statistics;
    public technologies: Technologies;
    public ui: UI;

    public constructor() {
        this.background = new Background(this, jQuery('#background'));
        this.configurations = new Configurations(this, jQuery('#configurations'));
        this.diplomacy = new Diplomacy(this, jQuery('#diplomacy'));
        this.game_menu = new GameMenu(this, jQuery('#game-menu'));
        this.info = new Info(this, jQuery('#info'));
        this.layers = new Layers(this, jQuery('#layers'));
        this.load_menu = new LoadMenu(this, jQuery('#load-menu'));
        this.log = new Log(this, jQuery('#log'));
        this.main_menu = new MainMenu(this, jQuery('#main-menu'));
        this.map_menu = new MapMenu(this, jQuery('#map-menu'));
        this.minimap = new Minimap(this, jQuery('#minimap'));
        this.objects_admin = new ObjectsAdmin(this, jQuery('#objects-admin'));
        this.options = new Options(this, jQuery('#options'));
        this.orders = new Orders(this, jQuery('#orders'));
        this.orders_admin = new OrdersAdmin(this, jQuery('#orders-admin'));
        this.players = new Players(this, jQuery('#players'));
        this.preview = new Preview(this, jQuery('#preview'));
        this.resources = new Resources(this, jQuery('#resources'));
        this.skills = new Skills(this, jQuery('#skills'));
        this.statistics = new Statistics(this, jQuery('#statistics'));
        this.technologies = new Technologies(this, jQuery('#technologies'));
        this.ui = new UI(this);
    }
}
