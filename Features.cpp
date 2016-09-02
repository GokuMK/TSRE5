/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Features.h"

std::unordered_map<std::string, int> Features::LIST {
    {"AERIALWAY_CABLE_CAR",0},
    {"AERIALWAY_CHAIR_LIFT",1},
    {"AERIALWAY_DRAG_LIFT",2},
    {"AERIALWAY_GONDOLA",3},
    {"AERIALWAY_GOODS",4},
    {"AERIALWAY_MIXED_LIFT",5},
    {"AERIALWAY_PYLON",6},
    {"AERIALWAY_STATION",7},
    {"AEROWAY_AERODROME",8},
    {"AEROWAY_APRON",9},
    {"AEROWAY_GATE",10},
    {"AEROWAY_HELIPAD",11},
    {"AEROWAY_RUNWAY",12},
    {"AEROWAY_TAXIWAY",13},
    {"AEROWAY_TERMINAL",14},
    {"AEROWAY_WINDSOCK",15},
    {"AMENITY_ARTS_CENTRE",16},
    {"AMENITY_ATM",17},
    {"AMENITY_BABY_HATCH",18},
    {"AMENITY_BANK",19},
    {"AMENITY_BAR",20},
    {"AMENITY_BBQ",21},
    {"AMENITY_BENCH",22},
    {"AMENITY_BICYCLE_PARKING",23},
    {"AMENITY_BICYCLE_RENTAL",24},
    {"AMENITY_BIERGARTEN",25},
    {"AMENITY_BROTHEL",26},
    {"AMENITY_BUREAU_DE_CHANGE",27},
    {"AMENITY_BUS_STATION",28},
    {"AMENITY_CAFE",29},
    {"AMENITY_CAR_RENTAL",30},
    {"AMENITY_CAR_SHARING",31},
    {"AMENITY_CAR_WASH",32},
    {"AMENITY_CINEMA",33},
    {"AMENITY_CLINIC",34},
    {"AMENITY_CLOCK",35},
    {"AMENITY_COLLEGE",36},
    {"AMENITY_COMMUNITY_CENTRE",37},
    {"AMENITY_COURTHOUSE",38},
    {"AMENITY_CREMATORIUM",39},
    {"AMENITY_DENTIST",40},
    {"AMENITY_DOCTORS",41},
    {"AMENITY_DRINKING_WATER",42},
    {"AMENITY_EMBASSY",43},
    {"AMENITY_EV_CHARGING",44},
    {"AMENITY_FAST_FOOD",45},
    {"AMENITY_FERRY_TERMINAL",46},
    {"AMENITY_FIRE_STATION",47},
    {"AMENITY_FOOD_COURT",48},
    {"AMENITY_FOUNTAIN",49},
    {"AMENITY_FUEL",50},
    {"AMENITY_GRAVE_YARD",51},
    {"AMENITY_GRIT_BIN",52},
    {"AMENITY_HOSPITAL",53},
    {"AMENITY_HUNTING_STAND",54},
    {"AMENITY_ICE_CREAM",55},
    {"AMENITY_KINDERGARTEN",56},
    {"AMENITY_LIBRARY",57},
    {"AMENITY_MARKETPLACE",58},
    {"AMENITY_NIGHTCLUB",59},
    {"AMENITY_NURSING_HOME",60},
    {"AMENITY_PARKING",61},
    {"AMENITY_PARKING_ENTRANCE",62},
    {"AMENITY_PARKING_SPACE",63},
    {"AMENITY_PHARMACY",64},
    {"AMENITY_PLACE_OF_WORSHIP",65},
    {"AMENITY_POLICE",66},
    {"AMENITY_POST_BOX",67},
    {"AMENITY_POST_OFFICE",68},
    {"AMENITY_PRISON",69},
    {"AMENITY_PUB",70},
    {"AMENITY_PUBLIC_BUILDING",71},
    {"AMENITY_RECYCLING",72},
    {"AMENITY_RESTAURANT",73},
    {"AMENITY_SAUNA",74},
    {"AMENITY_SCHOOL",75},
    {"AMENITY_SHELTER",76},
    {"AMENITY_SHOWER",77},
    {"AMENITY_SOCIAL_CENTRE",78},
    {"AMENITY_SOCIAL_FACILITY",79},
    {"AMENITY_STRIPCLUB",80},
    {"AMENITY_STUDIO",81},
    {"AMENITY_TAXI",82},
    {"AMENITY_TELEPHONE",83},
    {"AMENITY_THEATRE",84},
    {"AMENITY_TOILETS",85},
    {"AMENITY_TOWNHALL",86},
    {"AMENITY_UNIVERSITY",87},
    {"AMENITY_VENDING_MACHINE",88},
    {"AMENITY_VETERINARY",89},
    {"AMENITY_WASTE_BASKET",90},
    {"AMENITY_WASTE_DISPOSAL",91},
    {"AMENITY_WATERING_PLACE",92},
    {"BARRIER_BLOCK",93},
    {"BARRIER_BOLLARD",94},
    {"BARRIER_BORDER_CONTROL",95},
    {"BARRIER_CATTLE_GRID",96},
    {"BARRIER_CHAIN",97},
    {"BARRIER_CYCLE_BARRIER",98},
    {"BARRIER_DEBRIS",99},
    {"BARRIER_DITCH",100},
    {"BARRIER_ENTRANCE",101},
    {"BARRIER_FENCE",102},
    {"BARRIER_FULL-HEIGHT_TURNSTILE",103},
    {"BARRIER_GATE",104},
    {"BARRIER_GUARD_RAIL",105},
    {"BARRIER_HEDGE",106},
    {"BARRIER_HORSE_STILE",107},
    {"BARRIER_JERSEY_BARRIER",108},
    {"BARRIER_KENT_CARRIAGE_GAP",109},
    {"BARRIER_KERB",110},
    {"BARRIER_KISSING_GATE",111},
    {"BARRIER_LIFT_GATE",112},
    {"BARRIER_LOG",113},
    {"BARRIER_MOTORCYCLE_BARRIER",114},
    {"BARRIER_RETAINING_WALL",115},
    {"BARRIER_ROPE",116},
    {"BARRIER_SALLY_PORT",117},
    {"BARRIER_SPIKES",118},
    {"BARRIER_STILE",119},
    {"BARRIER_SWING_GATE",120},
    {"BARRIER_TOLL_BOOTH",121},
    {"BARRIER_TURNSTILE",122},
    {"BARRIER_WALL",123},
    {"BOUNDARY_ADMINISTRATIVE",124},
    {"BOUNDARY_CIVIL",125},
    {"BOUNDARY_MARITIME",126},
    {"BOUNDARY_NATIONAL_PARK",127},
    {"BOUNDARY_POLITICAL",128},
    {"BOUNDARY_PROTECTED_AREA",129},
    {"BUILDING_APARTMENTS",130},
    {"BUILDING_BUNKER",131},
    {"BUILDING_CABIN",132},
    {"BUILDING_CHURCH",133},
    {"BUILDING_CIVIC",134},
    {"BUILDING_COMMERCIAL",135},
    {"BUILDING_DORMITORY",136},
    {"BUILDING_EDUCATION",137},
    {"BUILDING_ENTRANCE",138},
    {"BUILDING_FARM_AUXILIARY",139},
    {"BUILDING_GARAGE",140},
    {"BUILDING_GARAGES",141},
    {"BUILDING_GREENHOUSE",142},
    {"BUILDING_HANGAR",143},
    {"BUILDING_HOSPITAL",144},
    {"BUILDING_HOTEL",145},
    {"BUILDING_HOUSE",146},
    {"BUILDING_HUT",147},
    {"BUILDING_INDUSTRIAL",148},
    {"BUILDING_OFFICE",149},
    {"BUILDING_OFFICES",150},
    {"BUILDING_RESIDENTIAL",151},
    {"BUILDING_RETAIL",152},
    {"BUILDING_ROOF",153},
    {"BUILDING_SCHOOL",154},
    {"BUILDING_SERVICE",155},
    {"BUILDING_SHED",156},
    {"BUILDING_TERRACE",157},
    {"BUILDING_TRAIN_STATION",158},
    {"BUILDING_TRANSPORTATION",159},
    {"BUILDING_UNIVERSITY",160},
    {"BUILDING_WAREHOUSE",161},
    {"BUILDING_YES",162},
    {"CRAFT_AGRICULTURAL_ENGINES",163},
    {"CRAFT_BASKET_MAKER",164},
    {"CRAFT_BEEKEEPER",165},
    {"CRAFT_BLACKSMITH",166},
    {"CRAFT_BOATBUILDER",167},
    {"CRAFT_BONFIRES",168},
    {"CRAFT_BOOKBINDER",169},
    {"CRAFT_BREWERY",170},
    {"CRAFT_CARPENTER",171},
    {"CRAFT_CARPET_LAYER",172},
    {"CRAFT_CATERER",173},
    {"CRAFT_CLOCKMAKER",174},
    {"CRAFT_CONFECTIONERY",175},
    {"CRAFT_DRESSMAKER",176},
    {"CRAFT_ELECTRICIAN",177},
    {"CRAFT_FALLES",178},
    {"CRAFT_GARDENER",179},
    {"CRAFT_GLAZIERY",180},
    {"CRAFT_HANDICRAFT",181},
    {"CRAFT_HVAC",182},
    {"CRAFT_INSULATION",183},
    {"CRAFT_JEWELLER",184},
    {"CRAFT_KEY_CUTTER",185},
    {"CRAFT_LOCKSMITH",186},
    {"CRAFT_METAL_CONSTRUCTION",187},
    {"CRAFT_OPTICIAN",188},
    {"CRAFT_PAINTER",189},
    {"CRAFT_PARQUET_LAYER",190},
    {"CRAFT_PHOTOGRAPHER",191},
    {"CRAFT_PHOTOGRAPHIC_LABORATORY",192},
    {"CRAFT_PLASTERER",193},
    {"CRAFT_PLUMBER",194},
    {"CRAFT_POTTERY",195},
    {"CRAFT_RIGGER",196},
    {"CRAFT_ROOFER",197},
    {"CRAFT_SADDLER",198},
    {"CRAFT_SAILMAKER",199},
    {"CRAFT_SAWMILL",200},
    {"CRAFT_SCAFFOLDER",201},
    {"CRAFT_SCULPTOR",202},
    {"CRAFT_SHOEMAKER",203},
    {"CRAFT_STAND_BUILDER",204},
    {"CRAFT_STONEMASON",205},
    {"CRAFT_SUN_PROTECTION",206},
    {"CRAFT_SWEEP",207},
    {"CRAFT_TAILOR",208},
    {"CRAFT_TILER",209},
    {"CRAFT_TINSMITH",210},
    {"CRAFT_UPHOLSTERER",211},
    {"CRAFT_WATCHMAKER",212},
    {"EMERGENCY_AMBULANCE_STATION",213},
    {"EMERGENCY_FIRE_EXTINGUISHER",214},
    {"EMERGENCY_FIRE_FLAPPER",215},
    {"EMERGENCY_FIRE_HOSE",216},
    {"EMERGENCY_FIRE_HYDRANT",217},
    {"EMERGENCY_PHONE",218},
    {"EMERGENCY_SES_STATION",219},
    {"EMERGENCY_SIREN",220},
    {"GEOLOGICAL_OUTCROP",221},
    {"GEOLOGICAL_PALAEONTOLOGICAL_SITE",222},
    {"HIGHWAY_BRIDLEWAY",223},
    {"HIGHWAY_BUS_GUIDEWAY",224},
    {"HIGHWAY_BUS_STOP",225},
    {"HIGHWAY_BYWAY",226},
    {"HIGHWAY_CONSTRUCTION",227},
    {"HIGHWAY_CROSSING",228},
    {"HIGHWAY_CYCLEWAY",229},
    {"HIGHWAY_EMERGENCY_ACCESS_POINT",230},
    {"HIGHWAY_FOOTWAY",231},
    {"HIGHWAY_FORD",232},
    {"HIGHWAY_GIVE_WAY",233},
    {"HIGHWAY_LIVING_STREET",234},
    {"HIGHWAY_MINI_ROUNDABOUT",235},
    {"HIGHWAY_MOTORWAY",236},
    {"HIGHWAY_MOTORWAY_JUNCTION",237},
    {"HIGHWAY_MOTORWAY_LINK",238},
    {"HIGHWAY_PASSING_PLACE",239},
    {"HIGHWAY_PATH",240},
    {"HIGHWAY_PEDESTRIAN",241},
    {"HIGHWAY_PRIMARY",242},
    {"HIGHWAY_PRIMARY_LINK",243},
    {"HIGHWAY_PROPOSED",244},
    {"HIGHWAY_RACEWAY",245},
    {"HIGHWAY_RESIDENTIAL",246},
    {"HIGHWAY_REST_AREA",247},
    {"HIGHWAY_ROAD",248},
    {"HIGHWAY_SECONDARY",249},
    {"HIGHWAY_SECONDARY_LINK",250},
    {"HIGHWAY_SERVICE",251},
    {"HIGHWAY_SERVICES",252},
    {"HIGHWAY_SPEED_CAMERA",253},
    {"HIGHWAY_STEPS",254},
    {"HIGHWAY_STOP",255},
    {"HIGHWAY_STREET_LAMP",256},
    {"HIGHWAY_TERTIARY",257},
    {"HIGHWAY_TERTIARY_LINK",258},
    {"HIGHWAY_TRACK",259},
    {"HIGHWAY_TRAFFIC_SIGNALS",260},
    {"HIGHWAY_TRUNK",261},
    {"HIGHWAY_TRUNK_LINK",262},
    {"HIGHWAY_TURNING_CIRCLE",263},
    {"HIGHWAY_UNCLASSIFIED",264},
    {"HISTORIC_ARCHAEOLOGICAL_SITE",265},
    {"HISTORIC_BATTLEFIELD",266},
    {"HISTORIC_BOUNDARY_STONE",267},
    {"HISTORIC_CASTLE",268},
    {"HISTORIC_CITY_GATE",269},
    {"HISTORIC_FORT",270},
    {"HISTORIC_MEMORIAL",271},
    {"HISTORIC_MONUMENT",272},
    {"HISTORIC_RUINS",273},
    {"HISTORIC_RUNE_STONE",274},
    {"HISTORIC_WAYSIDE_CROSS",275},
    {"HISTORIC_WAYSIDE_SHRINE",276},
    {"HISTORIC_WRECK",277},
    {"LANDUSE_ALLOTMENTS",278},
    {"LANDUSE_BASIN",279},
    {"LANDUSE_BROWNFIELD",280},
    {"LANDUSE_CEMETERY",281},
    {"LANDUSE_COMMERCIAL",282},
    {"LANDUSE_CONSTRUCTION",283},
    {"LANDUSE_FARM",284},
    {"LANDUSE_FARMLAND",285},
    {"LANDUSE_FARMYARD",286},
    {"LANDUSE_FOREST",287},
    {"LANDUSE_GARAGES",288},
    {"LANDUSE_GRASS",289},
    {"LANDUSE_GREENFIELD",290},
    {"LANDUSE_GREENHOUSE_HORTICULTURE",291},
    {"LANDUSE_INDUSTRIAL",292},
    {"LANDUSE_LANDFILL",293},
    {"LANDUSE_MEADOW",294},
    {"LANDUSE_MILITARY",295},
    {"LANDUSE_ORCHARD",296},
    {"LANDUSE_PLANT_NURSERY",297},
    {"LANDUSE_QUARRY",298},
    {"LANDUSE_RAILWAY",299},
    {"LANDUSE_RECREATION_GROUND",300},
    {"LANDUSE_RESERVOIR",301},
    {"LANDUSE_RESIDENTIAL",302},
    {"LANDUSE_RETAIL",303},
    {"LANDUSE_SALT_POND",304},
    {"LANDUSE_VILLAGE_GREEN",305},
    {"LANDUSE_VINEYARD",306},
    {"LEISURE_COMMON",307},
    {"LEISURE_DANCE",308},
    {"LEISURE_DOG_PARK",309},
    {"LEISURE_FISHING",310},
    {"LEISURE_GARDEN",311},
    {"LEISURE_GOLF_COURSE",312},
    {"LEISURE_ICE_RINK",313},
    {"LEISURE_MARINA",314},
    {"LEISURE_MINIATURE_GOLF",315},
    {"LEISURE_NATURE_RESERVE",316},
    {"LEISURE_PARK",317},
    {"LEISURE_PITCH",318},
    {"LEISURE_PLAYGROUND",319},
    {"LEISURE_SLIPWAY",320},
    {"LEISURE_SPORTS_CENTRE",321},
    {"LEISURE_STADIUM",322},
    {"LEISURE_SWIMMING_POOL",323},
    {"LEISURE_TRACK",324},
    {"LEISURE_WATER_PARK",325},
    {"MAN_MADE_ADIT",326},
    {"MAN_MADE_BEACON",327},
    {"MAN_MADE_BREAKWATER",328},
    {"MAN_MADE_CHIMNEY",329},
    {"MAN_MADE_CRANE",330},
    {"MAN_MADE_CUTLINE",331},
    {"MAN_MADE_FLAGPOLE",332},
    {"MAN_MADE_GROYNE",333},
    {"MAN_MADE_LIGHTHOUSE",334},
    {"MAN_MADE_MINESHAFT",335},
    {"MAN_MADE_MONITORING_STATION",336},
    {"MAN_MADE_PETROLEUM_WELL",337},
    {"MAN_MADE_PIER",338},
    {"MAN_MADE_PIPELINE",339},
    {"MAN_MADE_RESERVOIR_COVERED",340},
    {"MAN_MADE_STORAGE_TANK",341},
    {"MAN_MADE_SURVEILLANCE",342},
    {"MAN_MADE_SURVEY_POINT",343},
    {"MAN_MADE_TOWER",344},
    {"MAN_MADE_WASTEWATER_PLANT",345},
    {"MAN_MADE_WATER_TOWER",346},
    {"MAN_MADE_WATER_WELL",347},
    {"MAN_MADE_WATER_WORKS",348},
    {"MAN_MADE_WATERMILL",349},
    {"MAN_MADE_WINDMILL",350},
    {"MAN_MADE_WORKS",351},
    {"MILITARY_AIRFIELD",352},
    {"MILITARY_BARRACKS",353},
    {"MILITARY_BUNKER",354},
    {"MILITARY_CHECKPOINT",355},
    {"MILITARY_DANGER_AREA",356},
    {"MILITARY_NAVAL_BASE",357},
    {"MILITARY_RANGE",358},
    {"NATURAL_BAY",359},
    {"NATURAL_BEACH",360},
    {"NATURAL_CAVE_ENTRANCE",361},
    {"NATURAL_CLIFF",362},
    {"NATURAL_COASTLINE",363},
    {"NATURAL_FELL",364},
    {"NATURAL_GLACIER",365},
    {"NATURAL_GRASSLAND",366},
    {"NATURAL_HEATH",367},
    {"NATURAL_LAND",368},
    {"NATURAL_MARSH",369},
    {"NATURAL_MUD",370},
    {"NATURAL_PEAK",371},
    {"NATURAL_SAND",372},
    {"NATURAL_SCREE",373},
    {"NATURAL_SCRUB",374},
    {"NATURAL_SPRING",375},
    {"NATURAL_STONE",376},
    {"NATURAL_TREE",377},
    {"NATURAL_VOLCANO",378},
    {"NATURAL_WATER",379},
    {"NATURAL_WETLAND",380},
    {"NATURAL_WOOD",381},
    {"OFFICE_ACCOUNTANT",382},
    {"OFFICE_ADMINISTRATIVE",383},
    {"OFFICE_ARCHITECT",384},
    {"OFFICE_CAMPING",385},
    {"OFFICE_COMPANY",386},
    {"OFFICE_EDUCATIONAL_INSTITUTION",387},
    {"OFFICE_EMPLOYMENT_AGENCY",388},
    {"OFFICE_ESTATE_AGENT",389},
    {"OFFICE_GOVERNMENT",390},
    {"OFFICE_INSURANCE",391},
    {"OFFICE_IT",392},
    {"OFFICE_LAWYER",393},
    {"OFFICE_NEWSPAPER",394},
    {"OFFICE_NGO",395},
    {"OFFICE_POLITICAL_PARTY",396},
    {"OFFICE_QUANGO",397},
    {"OFFICE_RESEARCH",398},
    {"OFFICE_TELECOMMUNICATION",399},
    {"OFFICE_TRAVEL_AGENT",400},
    {"PLACE_CITY",401},
    {"PLACE_CONTINENT",402},
    {"PLACE_COUNTRY",403},
    {"PLACE_COUNTY",404},
    {"PLACE_FARM",405},
    {"PLACE_HAMLET",406},
    {"PLACE_ISLAND",407},
    {"PLACE_ISOLATED_DWELLING",408},
    {"PLACE_LOCALITY",409},
    {"PLACE_NEIGHBOURHOOD",410},
    {"PLACE_REGION",411},
    {"PLACE_STATE",412},
    {"PLACE_SUBURB",413},
    {"PLACE_TOWN",414},
    {"PLACE_VILLAGE",415},
    {"POWER_CABLE_DISTRIBUTION_CABINET",416},
    {"POWER_GENERATOR",417},
    {"POWER_LINE",418},
    {"POWER_MINOR_LINE",419},
    {"POWER_POLE",420},
    {"POWER_STATION",421},
    {"POWER_SUB_STATION",422},
    {"POWER_TOWER",423},
    {"PUBLIC_TRANSPORT_PLATFORM",424},
    {"PUBLIC_TRANSPORT_STATION",425},
    {"PUBLIC_TRANSPORT_STOP_POSITION",426},
    {"RAILWAY_ABANDONED",427},
    {"RAILWAY_BUFFER_STOP",428},
    {"RAILWAY_CONSTRUCTION",429},
    {"RAILWAY_CROSSING",430},
    {"RAILWAY_DISUSED",431},
    {"RAILWAY_FUNICULAR",432},
    {"RAILWAY_HALT",433},
    {"RAILWAY_LEVEL_CROSSING",434},
    {"RAILWAY_LIGHT_RAIL",435},
    {"RAILWAY_MINIATURE",436},
    {"RAILWAY_MONORAIL",437},
    {"RAILWAY_NARROW_GAUGE",438},
    {"RAILWAY_PRESERVED",439},
    {"RAILWAY_RAIL",440},
    {"RAILWAY_ROUNDHOUSE",441},
    {"RAILWAY_STATION",442},
    {"RAILWAY_SUBWAY",443},
    {"RAILWAY_SUBWAY_ENTRANCE",444},
    {"RAILWAY_TRAM",445},
    {"RAILWAY_TRAM_STOP",446},
    {"RAILWAY_TURNTABLE",447},
    {"ROUTE_BICYCLE",448},
    {"ROUTE_BUS",449},
    {"ROUTE_DETOUR",450},
    {"ROUTE_FERRY",451},
    {"ROUTE_HIKING",452},
    {"ROUTE_MTB",453},
    {"ROUTE_PISTE",454},
    {"ROUTE_RAILWAY",455},
    {"ROUTE_ROAD",456},
    {"ROUTE_SKI",457},
    {"ROUTE_TRAIN",458},
    {"ROUTE_TRAM",459},
    {"SHOP_ALCOHOL",460},
    {"SHOP_ANIME",461},
    {"SHOP_APPLIANCE",462},
    {"SHOP_ART",463},
    {"SHOP_BABY_GOODS",464},
    {"SHOP_BAKERY",465},
    {"SHOP_BATHROOM_FURNISHING",466},
    {"SHOP_BEAUTY",467},
    {"SHOP_BED",468},
    {"SHOP_BEVERAGES",469},
    {"SHOP_BICYCLE",470},
    {"SHOP_BOOKS",471},
    {"SHOP_BOUTIQUE",472},
    {"SHOP_BUTCHER",473},
    {"SHOP_CAR",474},
    {"SHOP_CAR_PARTS",475},
    {"SHOP_CAR_REPAIR",476},
    {"SHOP_CARPET",477},
    {"SHOP_CHARITY",478},
    {"SHOP_CHEMIST",479},
    {"SHOP_CLOTHES",480},
    {"SHOP_COMPUTER",481},
    {"SHOP_CONFECTIONERY",482},
    {"SHOP_CONVENIENCE",483},
    {"SHOP_COPYSHOP",484},
    {"SHOP_CURTAIN",485},
    {"SHOP_DELI",486},
    {"SHOP_DEPARTMENT_STORE",487},
    {"SHOP_DIVE",488},
    {"SHOP_DOITYOURSELF",489},
    {"SHOP_DRUGSTORE",490},
    {"SHOP_DRY_CLEANING",491},
    {"SHOP_ELECTRONICS",492},
    {"SHOP_EROTIC",493},
    {"SHOP_FABRIC",494},
    {"SHOP_FARM",495},
    {"SHOP_FLORIST",496},
    {"SHOP_FRAME",497},
    {"SHOP_FUNERAL_DIRECTORS",498},
    {"SHOP_FURNACE",499},
    {"SHOP_FURNITURE",500},
    {"SHOP_GARDEN_CENTRE",501},
    {"SHOP_GAS",502},
    {"SHOP_GENERAL",503},
    {"SHOP_GIFT",504},
    {"SHOP_GLAZIERY",505},
    {"SHOP_GREENGROCER",506},
    {"SHOP_HAIRDRESSER",507},
    {"SHOP_HARDWARE",508},
    {"SHOP_HEARING_AIDS",509},
    {"SHOP_HERBALIST",510},
    {"SHOP_HIFI",511},
    {"SHOP_HUNTING",512},
    {"SHOP_INTERIOR_DECORATION",513},
    {"SHOP_JEWELRY",514},
    {"SHOP_KIOSK",515},
    {"SHOP_KITCHEN",516},
    {"SHOP_LAUNDRY",517},
    {"SHOP_MALL",518},
    {"SHOP_MASSAGE",519},
    {"SHOP_MOBILE_PHONE",520},
    {"SHOP_MONEY_LENDER",521},
    {"SHOP_MOTORCYCLE",522},
    {"SHOP_MUSICAL_INSTRUMENT",523},
    {"SHOP_NEWSAGENT",524},
    {"SHOP_OPTICIAN",525},
    {"SHOP_ORGANIC",526},
    {"SHOP_OUTDOOR",527},
    {"SHOP_PAINT",528},
    {"SHOP_PAWNBROKER",529},
    {"SHOP_PET",530},
    {"SHOP_RADIOTECHNICS",531},
    {"SHOP_SEAFOODORFISH",532},
    {"SHOP_SECOND_HAND",533},
    {"SHOP_SHOES",534},
    {"SHOP_SPORTS",535},
    {"SHOP_STATIONERY",536},
    {"SHOP_SUPERMARKET",537},
    {"SHOP_TATTOO",538},
    {"SHOP_TICKET",539},
    {"SHOP_TOBACCO",540},
    {"SHOP_TOYS",541},
    {"SHOP_TRADE",542},
    {"SHOP_VACANT",543},
    {"SHOP_VACUUM_CLEANER",544},
    {"SHOP_VARIETY_STORE",545},
    {"SHOP_VIDEO",546},
    {"SHOP_WINDOW_BLIND",547},
    {"SPORT_10PIN",548},
    {"SPORT_9PIN",549},
    {"SPORT_AMERICAN_FOOTBALL",550},
    {"SPORT_ARCHERY",551},
    {"SPORT_ATHLETICS",552},
    {"SPORT_AUSTRALIAN_FOOTBALL",553},
    {"SPORT_BADMINTON",554},
    {"SPORT_BASE",555},
    {"SPORT_BASEBALL",556},
    {"SPORT_BASKETBALL",557},
    {"SPORT_BEACHVOLLEYBALL",558},
    {"SPORT_BMX",559},
    {"SPORT_BOULES",560},
    {"SPORT_BOWLS",561},
    {"SPORT_CANADIAN_FOOTBALL",562},
    {"SPORT_CANOE",563},
    {"SPORT_CHESS",564},
    {"SPORT_CLIMBING",565},
    {"SPORT_CRICKET",566},
    {"SPORT_CRICKET_NETS",567},
    {"SPORT_CROQUET",568},
    {"SPORT_CYCLING",569},
    {"SPORT_DIVING",570},
    {"SPORT_DOG_RACING",571},
    {"SPORT_EQUESTRIAN",572},
    {"SPORT_FENCING",573},
    {"SPORT_FOOTBALL",574},
    {"SPORT_FREE_FLYING",575},
    {"SPORT_GAELIC_GAMES",576},
    {"SPORT_GOLF",577},
    {"SPORT_GYMNASTICS",578},
    {"SPORT_HOCKEY",579},
    {"SPORT_HORSE_RACING",580},
    {"SPORT_HORSESHOES",581},
    {"SPORT_ICE_STOCK",582},
    {"SPORT_KARTING",583},
    {"SPORT_KORFBALL",584},
    {"SPORT_MOTOR",585},
    {"SPORT_MULTI",586},
    {"SPORT_ORIENTEERING",587},
    {"SPORT_PADDLE_TENNIS",588},
    {"SPORT_PARAGLIDING",589},
    {"SPORT_PELOTA",590},
    {"SPORT_RACQUET",591},
    {"SPORT_ROWING",592},
    {"SPORT_RUGBY_LEAGUE",593},
    {"SPORT_RUGBY_UNION",594},
    {"SPORT_SHOOTING",595},
    {"SPORT_SKATEBOARD",596},
    {"SPORT_SKATING",597},
    {"SPORT_SKIING",598},
    {"SPORT_SOCCER",599},
    {"SPORT_SURFING",600},
    {"SPORT_SWIMMING",601},
    {"SPORT_TABLE_TENNIS",602},
    {"SPORT_TEAM_HANDBALL",603},
    {"SPORT_TENNIS",604},
    {"SPORT_TOBOGGAN",605},
    {"SPORT_VOLLEYBALL",606},
    {"SPORT_WATER_SKI",607},
    {"TOURISM_ALPINE_HUT",608},
    {"TOURISM_ARTWORK",609},
    {"TOURISM_ATTRACTION",610},
    {"TOURISM_CAMP_SITE",611},
    {"TOURISM_CARAVAN_SITE",612},
    {"TOURISM_CHALET",613},
    {"TOURISM_GUEST_HOUSE",614},
    {"TOURISM_HOSTEL",615},
    {"TOURISM_HOTEL",616},
    {"TOURISM_INFORMATION",617},
    {"TOURISM_MOTEL",618},
    {"TOURISM_MUSEUM",619},
    {"TOURISM_PICNIC_SITE",620},
    {"TOURISM_THEME_PARK",621},
    {"TOURISM_VIEWPOINT",622},
    {"TOURISM_YES",623},
    {"TOURISM_ZOO",624},
    {"WATERWAY_BOATYARD",625},
    {"WATERWAY_CANAL",626},
    {"WATERWAY_DAM",627},
    {"WATERWAY_DITCH",628},
    {"WATERWAY_DOCK",629},
    {"WATERWAY_DRAIN",630},
    {"WATERWAY_LOCK_GATE",631},
    {"WATERWAY_RIVER",632},
    {"WATERWAY_RIVERBANK",633},
    {"WATERWAY_STREAM",634},
    {"WATERWAY_TURNING_POINT",635},
    {"WATERWAY_WEIR",636}
};

std::vector<int> Features::LAYER = [] {
    std::vector<int> v;
        /*0*/ v.push_back(0);
        /*1*/ v.push_back(0);
        /*2*/ v.push_back(0);
        /*3*/ v.push_back(0);
        /*4*/ v.push_back(0);
        /*5*/ v.push_back(0);
        /*6*/ v.push_back(0);
        /*7*/ v.push_back(0);
        /*8*/ v.push_back(0);
        /*9*/ v.push_back(0);
        /*10*/ v.push_back(0);
        /*11*/ v.push_back(0);
        /*12*/ v.push_back(4);
        /*13*/ v.push_back(4);
        /*14*/ v.push_back(1);
        /*15*/ v.push_back(0);
        /*16*/ v.push_back(0);
        /*17*/ v.push_back(0);
        /*18*/ v.push_back(0);
        /*19*/ v.push_back(0);
        /*20*/ v.push_back(0);
        /*21*/ v.push_back(0);
        /*22*/ v.push_back(0);
        /*23*/ v.push_back(0);
        /*24*/ v.push_back(0);
        /*25*/ v.push_back(0);
        /*26*/ v.push_back(0);
        /*27*/ v.push_back(0);
        /*28*/ v.push_back(0);
        /*29*/ v.push_back(0);
        /*30*/ v.push_back(0);
        /*31*/ v.push_back(0);
        /*32*/ v.push_back(0);
        /*33*/ v.push_back(0);
        /*34*/ v.push_back(0);
        /*35*/ v.push_back(0);
        /*36*/ v.push_back(0);
        /*37*/ v.push_back(0);
        /*38*/ v.push_back(0);
        /*39*/ v.push_back(0);
        /*40*/ v.push_back(0);
        /*41*/ v.push_back(0);
        /*42*/ v.push_back(0);
        /*43*/ v.push_back(0);
        /*44*/ v.push_back(0);
        /*45*/ v.push_back(0);
        /*46*/ v.push_back(0);
        /*47*/ v.push_back(0);
        /*48*/ v.push_back(0);
        /*49*/ v.push_back(0);
        /*50*/ v.push_back(0);
        /*51*/ v.push_back(0);
        /*52*/ v.push_back(0);
        /*53*/ v.push_back(0);
        /*54*/ v.push_back(0);
        /*55*/ v.push_back(0);
        /*56*/ v.push_back(0);
        /*57*/ v.push_back(0);
        /*58*/ v.push_back(0);
        /*59*/ v.push_back(0);
        /*60*/ v.push_back(0);
        /*61*/ v.push_back(5);
        /*62*/ v.push_back(0);
        /*63*/ v.push_back(0);
        /*64*/ v.push_back(0);
        /*65*/ v.push_back(2);
        /*66*/ v.push_back(0);
        /*67*/ v.push_back(0);
        /*68*/ v.push_back(0);
        /*69*/ v.push_back(0);
        /*70*/ v.push_back(0);
        /*71*/ v.push_back(1);
        /*72*/ v.push_back(0);
        /*73*/ v.push_back(0);
        /*74*/ v.push_back(0);
        /*75*/ v.push_back(6);
        /*76*/ v.push_back(0);
        /*77*/ v.push_back(0);
        /*78*/ v.push_back(0);
        /*79*/ v.push_back(0);
        /*80*/ v.push_back(0);
        /*81*/ v.push_back(0);
        /*82*/ v.push_back(0);
        /*83*/ v.push_back(0);
        /*84*/ v.push_back(0);
        /*85*/ v.push_back(0);
        /*86*/ v.push_back(0);
        /*87*/ v.push_back(0);
        /*88*/ v.push_back(0);
        /*89*/ v.push_back(0);
        /*90*/ v.push_back(0);
        /*91*/ v.push_back(0);
        /*92*/ v.push_back(0);
        /*93*/ v.push_back(0);
        /*94*/ v.push_back(0);
        /*95*/ v.push_back(0);
        /*96*/ v.push_back(0);
        /*97*/ v.push_back(0);
        /*98*/ v.push_back(0);
        /*99*/ v.push_back(0);
        /*100*/ v.push_back(0);
        /*101*/ v.push_back(0);
        /*102*/ v.push_back(0);
        /*103*/ v.push_back(0);
        /*104*/ v.push_back(0);
        /*105*/ v.push_back(0);
        /*106*/ v.push_back(0);
        /*107*/ v.push_back(0);
        /*108*/ v.push_back(0);
        /*109*/ v.push_back(0);
        /*110*/ v.push_back(0);
        /*111*/ v.push_back(0);
        /*112*/ v.push_back(0);
        /*113*/ v.push_back(0);
        /*114*/ v.push_back(0);
        /*115*/ v.push_back(0);
        /*116*/ v.push_back(0);
        /*117*/ v.push_back(0);
        /*118*/ v.push_back(0);
        /*119*/ v.push_back(0);
        /*120*/ v.push_back(0);
        /*121*/ v.push_back(0);
        /*122*/ v.push_back(0);
        /*123*/ v.push_back(0);
        /*124*/ v.push_back(0);
        /*125*/ v.push_back(0);
        /*126*/ v.push_back(0);
        /*127*/ v.push_back(0);
        /*128*/ v.push_back(0);
        /*129*/ v.push_back(0);
        /*130*/ v.push_back(1);
        /*131*/ v.push_back(1);
        /*132*/ v.push_back(1);
        /*133*/ v.push_back(1);
        /*134*/ v.push_back(1);
        /*135*/ v.push_back(1);
        /*136*/ v.push_back(1);
        /*137*/ v.push_back(1);
        /*138*/ v.push_back(1);
        /*139*/ v.push_back(1);
        /*140*/ v.push_back(1);
        /*141*/ v.push_back(1);
        /*142*/ v.push_back(1);
        /*143*/ v.push_back(1);
        /*144*/ v.push_back(1);
        /*145*/ v.push_back(1);
        /*146*/ v.push_back(1);
        /*147*/ v.push_back(1);
        /*148*/ v.push_back(1);
        /*149*/ v.push_back(1);
        /*150*/ v.push_back(1);
        /*151*/ v.push_back(1);
        /*152*/ v.push_back(1);
        /*153*/ v.push_back(1);
        /*154*/ v.push_back(1);
        /*155*/ v.push_back(1);
        /*156*/ v.push_back(1);
        /*157*/ v.push_back(1);
        /*158*/ v.push_back(1);
        /*159*/ v.push_back(1);
        /*160*/ v.push_back(1);
        /*161*/ v.push_back(1);
        /*162*/ v.push_back(1);
        /*163*/ v.push_back(0);
        /*164*/ v.push_back(0);
        /*165*/ v.push_back(0);
        /*166*/ v.push_back(0);
        /*167*/ v.push_back(0);
        /*168*/ v.push_back(0);
        /*169*/ v.push_back(0);
        /*170*/ v.push_back(0);
        /*171*/ v.push_back(0);
        /*172*/ v.push_back(0);
        /*173*/ v.push_back(0);
        /*174*/ v.push_back(0);
        /*175*/ v.push_back(0);
        /*176*/ v.push_back(0);
        /*177*/ v.push_back(0);
        /*178*/ v.push_back(0);
        /*179*/ v.push_back(0);
        /*180*/ v.push_back(0);
        /*181*/ v.push_back(0);
        /*182*/ v.push_back(0);
        /*183*/ v.push_back(0);
        /*184*/ v.push_back(0);
        /*185*/ v.push_back(0);
        /*186*/ v.push_back(0);
        /*187*/ v.push_back(0);
        /*188*/ v.push_back(0);
        /*189*/ v.push_back(0);
        /*190*/ v.push_back(0);
        /*191*/ v.push_back(0);
        /*192*/ v.push_back(0);
        /*193*/ v.push_back(0);
        /*194*/ v.push_back(0);
        /*195*/ v.push_back(0);
        /*196*/ v.push_back(0);
        /*197*/ v.push_back(0);
        /*198*/ v.push_back(0);
        /*199*/ v.push_back(0);
        /*200*/ v.push_back(0);
        /*201*/ v.push_back(0);
        /*202*/ v.push_back(0);
        /*203*/ v.push_back(0);
        /*204*/ v.push_back(0);
        /*205*/ v.push_back(0);
        /*206*/ v.push_back(0);
        /*207*/ v.push_back(0);
        /*208*/ v.push_back(0);
        /*209*/ v.push_back(0);
        /*210*/ v.push_back(0);
        /*211*/ v.push_back(0);
        /*212*/ v.push_back(0);
        /*213*/ v.push_back(0);
        /*214*/ v.push_back(0);
        /*215*/ v.push_back(0);
        /*216*/ v.push_back(0);
        /*217*/ v.push_back(0);
        /*218*/ v.push_back(0);
        /*219*/ v.push_back(0);
        /*220*/ v.push_back(0);
        /*221*/ v.push_back(0);
        /*222*/ v.push_back(0);
        /*223*/ v.push_back(5);
        /*224*/ v.push_back(0);
        /*225*/ v.push_back(0);
        /*226*/ v.push_back(4);
        /*227*/ v.push_back(4);
        /*228*/ v.push_back(0);
        /*229*/ v.push_back(4);
        /*230*/ v.push_back(0);
        /*231*/ v.push_back(5);
        /*232*/ v.push_back(0);
        /*233*/ v.push_back(0);
        /*234*/ v.push_back(4);
        /*235*/ v.push_back(0);
        /*236*/ v.push_back(2);
        /*237*/ v.push_back(3);
        /*238*/ v.push_back(3);
        /*239*/ v.push_back(1);
        /*240*/ v.push_back(5);
        /*241*/ v.push_back(4);
        /*242*/ v.push_back(3);
        /*243*/ v.push_back(3);
        /*244*/ v.push_back(0);
        /*245*/ v.push_back(0);
        /*246*/ v.push_back(4);
        /*247*/ v.push_back(0);
        /*248*/ v.push_back(0);
        /*249*/ v.push_back(3);
        /*250*/ v.push_back(4);
        /*251*/ v.push_back(4);
        /*252*/ v.push_back(0);
        /*253*/ v.push_back(0);
        /*254*/ v.push_back(5);
        /*255*/ v.push_back(0);
        /*256*/ v.push_back(0);
        /*257*/ v.push_back(3);
        /*258*/ v.push_back(4);
        /*259*/ v.push_back(4);
        /*260*/ v.push_back(0);
        /*261*/ v.push_back(2);
        /*262*/ v.push_back(3);
        /*263*/ v.push_back(0);
        /*264*/ v.push_back(4);
        /*265*/ v.push_back(0);
        /*266*/ v.push_back(0);
        /*267*/ v.push_back(0);
        /*268*/ v.push_back(2);
        /*269*/ v.push_back(0);
        /*270*/ v.push_back(0);
        /*271*/ v.push_back(0);
        /*272*/ v.push_back(0);
        /*273*/ v.push_back(0);
        /*274*/ v.push_back(0);
        /*275*/ v.push_back(0);
        /*276*/ v.push_back(0);
        /*277*/ v.push_back(0);
        /*278*/ v.push_back(7);
        /*279*/ v.push_back(6);
        /*280*/ v.push_back(7);
        /*281*/ v.push_back(7);
        /*282*/ v.push_back(7);
        /*283*/ v.push_back(7);
        /*284*/ v.push_back(7);
        /*285*/ v.push_back(8);
        /*286*/ v.push_back(6);
        /*287*/ v.push_back(7);
        /*288*/ v.push_back(6);
        /*289*/ v.push_back(7);
        /*290*/ v.push_back(7);
        /*291*/ v.push_back(6);
        /*292*/ v.push_back(7);
        /*293*/ v.push_back(6);
        /*294*/ v.push_back(7);
        /*295*/ v.push_back(7);
        /*296*/ v.push_back(7);
        /*297*/ v.push_back(7);
        /*298*/ v.push_back(7);
        /*299*/ v.push_back(7);
        /*300*/ v.push_back(7);
        /*301*/ v.push_back(5);
        /*302*/ v.push_back(8);
        /*303*/ v.push_back(7);
        /*304*/ v.push_back(7);
        /*305*/ v.push_back(7);
        /*306*/ v.push_back(7);
        /*307*/ v.push_back(5);
        /*308*/ v.push_back(0);
        /*309*/ v.push_back(0);
        /*310*/ v.push_back(0);
        /*311*/ v.push_back(6);
        /*312*/ v.push_back(5);
        /*313*/ v.push_back(0);
        /*314*/ v.push_back(0);
        /*315*/ v.push_back(0);
        /*316*/ v.push_back(0);
        /*317*/ v.push_back(7);
        /*318*/ v.push_back(4);
        /*319*/ v.push_back(4);
        /*320*/ v.push_back(0);
        /*321*/ v.push_back(7);
        /*322*/ v.push_back(5);
        /*323*/ v.push_back(0);
        /*324*/ v.push_back(6);
        /*325*/ v.push_back(0);
        /*326*/ v.push_back(0);
        /*327*/ v.push_back(0);
        /*328*/ v.push_back(0);
        /*329*/ v.push_back(0);
        /*330*/ v.push_back(0);
        /*331*/ v.push_back(0);
        /*332*/ v.push_back(0);
        /*333*/ v.push_back(0);
        /*334*/ v.push_back(0);
        /*335*/ v.push_back(0);
        /*336*/ v.push_back(0);
        /*337*/ v.push_back(0);
        /*338*/ v.push_back(5);
        /*339*/ v.push_back(0);
        /*340*/ v.push_back(0);
        /*341*/ v.push_back(0);
        /*342*/ v.push_back(0);
        /*343*/ v.push_back(0);
        /*344*/ v.push_back(0);
        /*345*/ v.push_back(0);
        /*346*/ v.push_back(0);
        /*347*/ v.push_back(0);
        /*348*/ v.push_back(0);
        /*349*/ v.push_back(0);
        /*350*/ v.push_back(0);
        /*351*/ v.push_back(0);
        /*352*/ v.push_back(0);
        /*353*/ v.push_back(0);
        /*354*/ v.push_back(0);
        /*355*/ v.push_back(0);
        /*356*/ v.push_back(0);
        /*357*/ v.push_back(0);
        /*358*/ v.push_back(0);
        /*359*/ v.push_back(7);
        /*360*/ v.push_back(6);
        /*361*/ v.push_back(7);
        /*362*/ v.push_back(7);
        /*363*/ v.push_back(7);
        /*364*/ v.push_back(7);
        /*365*/ v.push_back(7);
        /*366*/ v.push_back(7);
        /*367*/ v.push_back(7);
        /*368*/ v.push_back(7);
        /*369*/ v.push_back(7);
        /*370*/ v.push_back(7);
        /*371*/ v.push_back(7);
        /*372*/ v.push_back(7);
        /*373*/ v.push_back(7);
        /*374*/ v.push_back(7);
        /*375*/ v.push_back(7);
        /*376*/ v.push_back(7);
        /*377*/ v.push_back(7);
        /*378*/ v.push_back(7);
        /*379*/ v.push_back(6);
        /*380*/ v.push_back(7);
        /*381*/ v.push_back(7);
        /*382*/ v.push_back(0);
        /*383*/ v.push_back(0);
        /*384*/ v.push_back(0);
        /*385*/ v.push_back(0);
        /*386*/ v.push_back(0);
        /*387*/ v.push_back(0);
        /*388*/ v.push_back(0);
        /*389*/ v.push_back(0);
        /*390*/ v.push_back(0);
        /*391*/ v.push_back(0);
        /*392*/ v.push_back(0);
        /*393*/ v.push_back(0);
        /*394*/ v.push_back(0);
        /*395*/ v.push_back(0);
        /*396*/ v.push_back(0);
        /*397*/ v.push_back(0);
        /*398*/ v.push_back(0);
        /*399*/ v.push_back(0);
        /*400*/ v.push_back(0);
        /*401*/ v.push_back(0);
        /*402*/ v.push_back(0);
        /*403*/ v.push_back(0);
        /*404*/ v.push_back(0);
        /*405*/ v.push_back(0);
        /*406*/ v.push_back(0);
        /*407*/ v.push_back(5);
        /*408*/ v.push_back(0);
        /*409*/ v.push_back(0);
        /*410*/ v.push_back(0);
        /*411*/ v.push_back(0);
        /*412*/ v.push_back(0);
        /*413*/ v.push_back(0);
        /*414*/ v.push_back(0);
        /*415*/ v.push_back(0);
        /*416*/ v.push_back(0);
        /*417*/ v.push_back(0);
        /*418*/ v.push_back(0);
        /*419*/ v.push_back(0);
        /*420*/ v.push_back(0);
        /*421*/ v.push_back(0);
        /*422*/ v.push_back(0);
        /*423*/ v.push_back(0);
        /*424*/ v.push_back(0);
        /*425*/ v.push_back(0);
        /*426*/ v.push_back(0);
        /*427*/ v.push_back(0);
        /*428*/ v.push_back(0);
        /*429*/ v.push_back(0);
        /*430*/ v.push_back(0);
        /*431*/ v.push_back(0);
        /*432*/ v.push_back(0);
        /*433*/ v.push_back(0);
        /*434*/ v.push_back(0);
        /*435*/ v.push_back(0);
        /*436*/ v.push_back(0);
        /*437*/ v.push_back(0);
        /*438*/ v.push_back(0);
        /*439*/ v.push_back(0);
        /*440*/ v.push_back(2);
        /*441*/ v.push_back(0);
        /*442*/ v.push_back(5);
        /*443*/ v.push_back(0);
        /*444*/ v.push_back(0);
        /*445*/ v.push_back(2);
        /*446*/ v.push_back(0);
        /*447*/ v.push_back(0);
        /*448*/ v.push_back(0);
        /*449*/ v.push_back(0);
        /*450*/ v.push_back(0);
        /*451*/ v.push_back(0);
        /*452*/ v.push_back(0);
        /*453*/ v.push_back(0);
        /*454*/ v.push_back(0);
        /*455*/ v.push_back(0);
        /*456*/ v.push_back(0);
        /*457*/ v.push_back(0);
        /*458*/ v.push_back(0);
        /*459*/ v.push_back(0);
        /*460*/ v.push_back(1);
        /*461*/ v.push_back(1);
        /*462*/ v.push_back(1);
        /*463*/ v.push_back(1);
        /*464*/ v.push_back(1);
        /*465*/ v.push_back(1);
        /*466*/ v.push_back(1);
        /*467*/ v.push_back(1);
        /*468*/ v.push_back(1);
        /*469*/ v.push_back(1);
        /*470*/ v.push_back(1);
        /*471*/ v.push_back(1);
        /*472*/ v.push_back(1);
        /*473*/ v.push_back(1);
        /*474*/ v.push_back(1);
        /*475*/ v.push_back(1);
        /*476*/ v.push_back(1);
        /*477*/ v.push_back(1);
        /*478*/ v.push_back(1);
        /*479*/ v.push_back(1);
        /*480*/ v.push_back(1);
        /*481*/ v.push_back(1);
        /*482*/ v.push_back(1);
        /*483*/ v.push_back(1);
        /*484*/ v.push_back(1);
        /*485*/ v.push_back(1);
        /*486*/ v.push_back(1);
        /*487*/ v.push_back(1);
        /*488*/ v.push_back(1);
        /*489*/ v.push_back(1);
        /*490*/ v.push_back(1);
        /*491*/ v.push_back(1);
        /*492*/ v.push_back(1);
        /*493*/ v.push_back(1);
        /*494*/ v.push_back(1);
        /*495*/ v.push_back(1);
        /*496*/ v.push_back(1);
        /*497*/ v.push_back(1);
        /*498*/ v.push_back(1);
        /*499*/ v.push_back(1);
        /*500*/ v.push_back(1);
        /*501*/ v.push_back(1);
        /*502*/ v.push_back(1);
        /*503*/ v.push_back(1);
        /*504*/ v.push_back(1);
        /*505*/ v.push_back(1);
        /*506*/ v.push_back(1);
        /*507*/ v.push_back(1);
        /*508*/ v.push_back(1);
        /*509*/ v.push_back(1);
        /*510*/ v.push_back(1);
        /*511*/ v.push_back(1);
        /*512*/ v.push_back(1);
        /*513*/ v.push_back(1);
        /*514*/ v.push_back(1);
        /*515*/ v.push_back(1);
        /*516*/ v.push_back(1);
        /*517*/ v.push_back(1);
        /*518*/ v.push_back(1);
        /*519*/ v.push_back(1);
        /*520*/ v.push_back(1);
        /*521*/ v.push_back(1);
        /*522*/ v.push_back(1);
        /*523*/ v.push_back(1);
        /*524*/ v.push_back(1);
        /*525*/ v.push_back(1);
        /*526*/ v.push_back(1);
        /*527*/ v.push_back(1);
        /*528*/ v.push_back(1);
        /*529*/ v.push_back(1);
        /*530*/ v.push_back(1);
        /*531*/ v.push_back(1);
        /*532*/ v.push_back(1);
        /*533*/ v.push_back(1);
        /*534*/ v.push_back(1);
        /*535*/ v.push_back(1);
        /*536*/ v.push_back(1);
        /*537*/ v.push_back(1);
        /*538*/ v.push_back(1);
        /*539*/ v.push_back(1);
        /*540*/ v.push_back(1);
        /*541*/ v.push_back(1);
        /*542*/ v.push_back(1);
        /*543*/ v.push_back(1);
        /*544*/ v.push_back(1);
        /*545*/ v.push_back(1);
        /*546*/ v.push_back(1);
        /*547*/ v.push_back(1);
        /*548*/ v.push_back(0);
        /*549*/ v.push_back(0);
        /*550*/ v.push_back(0);
        /*551*/ v.push_back(0);
        /*552*/ v.push_back(0);
        /*553*/ v.push_back(0);
        /*554*/ v.push_back(0);
        /*555*/ v.push_back(0);
        /*556*/ v.push_back(0);
        /*557*/ v.push_back(0);
        /*558*/ v.push_back(0);
        /*559*/ v.push_back(0);
        /*560*/ v.push_back(0);
        /*561*/ v.push_back(0);
        /*562*/ v.push_back(0);
        /*563*/ v.push_back(0);
        /*564*/ v.push_back(0);
        /*565*/ v.push_back(0);
        /*566*/ v.push_back(0);
        /*567*/ v.push_back(0);
        /*568*/ v.push_back(0);
        /*569*/ v.push_back(0);
        /*570*/ v.push_back(0);
        /*571*/ v.push_back(0);
        /*572*/ v.push_back(0);
        /*573*/ v.push_back(0);
        /*574*/ v.push_back(0);
        /*575*/ v.push_back(0);
        /*576*/ v.push_back(0);
        /*577*/ v.push_back(0);
        /*578*/ v.push_back(0);
        /*579*/ v.push_back(0);
        /*580*/ v.push_back(0);
        /*581*/ v.push_back(0);
        /*582*/ v.push_back(0);
        /*583*/ v.push_back(0);
        /*584*/ v.push_back(0);
        /*585*/ v.push_back(0);
        /*586*/ v.push_back(0);
        /*587*/ v.push_back(0);
        /*588*/ v.push_back(0);
        /*589*/ v.push_back(0);
        /*590*/ v.push_back(0);
        /*591*/ v.push_back(0);
        /*592*/ v.push_back(0);
        /*593*/ v.push_back(0);
        /*594*/ v.push_back(0);
        /*595*/ v.push_back(0);
        /*596*/ v.push_back(0);
        /*597*/ v.push_back(0);
        /*598*/ v.push_back(0);
        /*599*/ v.push_back(0);
        /*600*/ v.push_back(0);
        /*601*/ v.push_back(0);
        /*602*/ v.push_back(0);
        /*603*/ v.push_back(0);
        /*604*/ v.push_back(0);
        /*605*/ v.push_back(0);
        /*606*/ v.push_back(0);
        /*607*/ v.push_back(0);
        /*608*/ v.push_back(0);
        /*609*/ v.push_back(0);
        /*610*/ v.push_back(0);
        /*611*/ v.push_back(0);
        /*612*/ v.push_back(0);
        /*613*/ v.push_back(0);
        /*614*/ v.push_back(0);
        /*615*/ v.push_back(0);
        /*616*/ v.push_back(0);
        /*617*/ v.push_back(0);
        /*618*/ v.push_back(0);
        /*619*/ v.push_back(0);
        /*620*/ v.push_back(0);
        /*621*/ v.push_back(0);
        /*622*/ v.push_back(0);
        /*623*/ v.push_back(0);
        /*624*/ v.push_back(6);
        /*625*/ v.push_back(6);
        /*626*/ v.push_back(6);
        /*627*/ v.push_back(6);
        /*628*/ v.push_back(6);
        /*629*/ v.push_back(6);
        /*630*/ v.push_back(6);
        /*631*/ v.push_back(6);
        /*632*/ v.push_back(6);
        /*633*/ v.push_back(6);
        /*634*/ v.push_back(6);
        /*635*/ v.push_back(6);
        /*636*/ v.push_back(6);
    
    return v;
}();