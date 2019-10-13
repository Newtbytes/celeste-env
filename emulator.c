#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <stdarg.h>
#include <math.h>
#include "emulator.h"
#include "celeste.h"
#include "lemon.h"

//game data
static ALLEGRO_BITMAP* atlas = NULL;
static ALLEGRO_BITMAP* sprites[256] = {NULL};
static ALLEGRO_COLOR base_palette[16] = {
	#define RGB(r,g,b) (ALLEGRO_COLOR){(float)r/255.0,(float)g/255.0,(float)b/255.0, 255.0}
	RGB(0x00, 0x00, 0x00),
	RGB(0x1d, 0x2b, 0x53),
	RGB(0x7e, 0x25, 0x53),
	RGB(0x00, 0x87, 0x51),
	RGB(0xab, 0x52, 0x36),
	RGB(0x5f, 0x57, 0x4f),
	RGB(0xc2, 0xc3, 0xc7),
	RGB(0xff, 0xf1, 0xe8),
	RGB(0xff, 0x00, 0x4d),
	RGB(0xff, 0xa3, 0x00),
	RGB(0xff, 0xec, 0x27),
	RGB(0x00, 0xe4, 0x36),
	RGB(0x29, 0xad, 0xff),
	RGB(0x83, 0x76, 0x9c),
	RGB(0xff, 0x77, 0xa8),
	RGB(0xff, 0xcc, 0xaa)
	#undef RGB
};
static ALLEGRO_COLOR palette_swap[16];

#define tilemap_length 8192
static unsigned char tilemap[tilemap_length];
static unsigned char tile_flags[] = { //using https://github.com/dansanderson/picotool
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
	04,02,00,00,00,00,00,00,00,00,00,02,00,00,00,00,
	03,03,03,03,03,03,03,03,04,04,04,02,02,00,00,00,
	03,03,03,03,03,03,03,03,04,04,04,02,02,02,02,02,
	00,00,19,19,19,19,02,02,03,02,02,02,02,02,00,02,
	00,00,19,19,19,19,02,02,04,02,02,02,02,02,02,02,
	00,00,19,19,19,19,00,04,04,02,02,02,02,02,02,02,
	00,00,19,19,19,19,00,00,00,02,02,02,02,02,02,02
};

static ALLEGRO_FONT* font = NULL;

void InitEmu() {
	Celeste_P8_set_call_func(HandleP8Call);

	//using https://github.com/dansanderson/picotool
	char* mapdata = "2331252548252532323232323300002425262425252631323232252628282824252525252525323328382828312525253232323233000000313232323232323232330000002432323233313232322525252525482525252525252526282824252548252525262828282824254825252526282828283132323225482525252525252331323232332900002829000000242526313232332828002824262a102824254825252526002a2828292810244825282828290000000028282900000000002810000000372829000000002a2831482525252525482525323232332828242525254825323338282a283132252548252628382828282a2a2831323232322525252523201028380000002a0000003d24252523201028292900282426003a382425252548253300002900002a0031252528382900003a676838280000000000003828393e003a2800000000000028002425253232323232332122222328282425252532332828282900002a283132252526282828282900002a282828382824483232332828282900000000003f2020244825262828290000002a243300002a2425322525260000000000000000003125290000000021222328280000000000002a2828343536290000000000002839242526212223202123313232332828242548262b000000000000001c00003b2425262828280000000000282828282824252340283828293a2839000000343522252548262900000000000030000000002433003125333d3f00000000000000003100001c3a3a31252620283900000000000010282828290000000011113a2828313233242526103133202828282838242525262b000000000000000000003b2425262a2828670016002a28283828282425263a282828102829000000000000312525323300000000110000370000003e2400000037212223000000000000000000395868282828242628290000000000002a2828290000000000002123283828292828313233282829002a002a2828242525332b0c00000011110000000c3b314826112810000000006828282828282425252235353628280000000000003a282426003d003a3900270000000000002125001a000024252611111111000000002c28382828283831332800000017170000002a000000001111000024261028290028281b1b1b282800000000002a2125482628390000003b34362b000000002824252328283a67003a28282829002a313225333828282900000000000000283824252320201029003039000000005824480000003a31323235353536675800003c282828281028212329000000000000000000000000003436003a2426282800003828390000002a29000000000031323226101000000000282839000000002a24253328282838002828283900000017002600002a28000000003a283a2828282425252223283900372858390068283132000000282828282820202828283921222829002a28282426000000000000000000000000000020382828312523000000282828290000000000163a67682828003338280b00000010382800000b00003133282828282868282828280000001700330000002867580000281028283422252525482628286720282828382828212200003a283828102900002a28382824252a0000002838242600000017170000000000000000002728282a283133390000282900000000000000002a28282829002a2839000000002a2829000000000000282828382828282828282900000000000000003a2828383e3a2828283828242548252526002a282729002a28283432250000002a282828000000002810282425000000002a282426000000000000000000000000000037280000002a28283900280000003928390000000000282800000028290000002a2828000000000000002a2828282810282828286758000000000000002838282821232800002a28242532322526003a2830000000002a28282400000000002a281111111128282824480000003a28283133000000000000171700013f0000002029000000003828000028013a28281028580000003a28290000002a280c0000003a380c00000000000c00002a2828282828292828290000003a00013a2123282a313329001111112425002831263a3829300000000000002a310000000000002834222236292a0024253e013a3828292a00000000000000000035353536000020000000003d2a28671422222328282828283900582838283d00003a290000000028280000000000000000002a28282a29000058100012002a2822222225262900212311112122222525002a3837282900301111110000003a2800013f0000002a282426290000002425222222232900000000000000171700002a282039003a2000003a003435353535252525222222232828282810282821220b10000000000b28100000000b0000002c00002838000000002a2839170000282548252526111124252222252525482500012a2828673f242222230000003828222223000012002a24260000001224252525252600000000171700000000000000382028392827080028676820282828254825252525262a28282122222225253a28013d0000006828390000000000003c0168282800171717003a2800003a2825252525252222252525252525252525222222222222222525482667586828282548260000270000242600000021252525254826171700000000000000000000002a2028102830003a282828202828282525252548252600002a2425252548252821222300000028282800000000000022222223286700000000282839002838253233000000243232323232323225252525262828282824253232323225482525323232323232322526282828244825252525330000000000000000000000522525323232323331323232323328290026282928670000000000282831323232252525323328280031252548252525482525482628382831323232323232254826282800000030402a282828282824252548262838282831333828290031322526280000163a28283133282838242525482526000000000000000000000000522526000016000000002a10282838390026281a3820393d000000002a3828282825252628282829003b24253232323232323232332828282828281028282031253328390000003700002a3828002a2425252526282828282028292a0000002a313328111111282828000028002a312525252526000000000000000000000000522526000000001111000000292a28290026283a2820102011111121222328281025252628382800003b24262b002a2a38282828282829002a280028283828283128281029000000000000282839002448252526282900282067000000000000003810212223283829003a1029002a242532323367000000000000000000004200252639000000212300000000002122222522222321222321222324482628282832323328282800003b31332b00000028102829000000000029002a28282829002828280016000000162a2828280024252525262700002a2029000000000000002834252533292a0000002a00111124252223282800002c46472c00000042535325262800003a242600001600002425252525482631323331323324252620283822222328292867000028290000000000283800111100001200000028292a1600283828000000000000003a28290024254825263700000029000000000000003a293b2426283900000000003b212225252526382867003c56573c4243435363633233283900282426111111111124252525482526201b1b1b1b1b24252628282825252600002a28143a2900000000000028293b212300001700001128670000002828286758000000586828380000313232323320000000000000000000272828003b2426290000000000003b312548252533282828392122222352535364000029002a28382831323535353522254825252525252300000000003132332810284825261111113435361111111100000000003b3133111111111127282900003b2828282810290000002a28286700002835353536111100000000000011302838003b3133000000000000002a28313225262a282810282425252662636400000000160028282829000000000031322525252525252667580000002000002a28282525323535352222222222353639000000003b34353535353536303800000017282900002a0000000000382a29003a282828283436200000000000002030282800002a29000011110000000028282831260029002a282448252523000000000039003a282900000000000000002831322525482526382900000017000058682832331028293b2448252526282828000000003b201b1b1b1b1b1b302800000017283a0000000000000000280000002828283810292a000000000000002a3710281111111111112136000000002a28380b2600000000212525252526001c0000002828281000000000001100002a382829252525252628000000001700002a212228282908003b242525482628282912000000001b00000000000030290000003b3829000000000000003a102900002838282828000000000000000000002a2828223535353535330000000000002828393300000000313225252533000000000028382829000000003b202b00682828003232323233290000000000000000312528280000003b3132322526382800170000000000000000110000370000000000290000000000000000002a000000282928292a0000000000000000000000282a332838282829000000000000001028280000000042434424252628390000000028002a0000110000001b002a2010292c1b1b1b1b0000000000000000000010312829160000001b1b1b313328106700000000001100003a2700001b000000000000000100000011111100000000002a3a2a0000000000000000000000002a2800282829002a000000000000000028282800000000525354244826282800000000290000003b202b39000000002900003c000000000000000000000000000028282800000000000000001b1b2a28290000010000273900383000000000000000001111201111112122230000001212002a00010000000000000000000000002900290000000000000000002a6768282900003f01005253542425262810673a3900013f0000002a3829001100000000002101000000000000003a67000000002a38286758680000010000000068280000002123003728292830000000000000000022222222222324482611111120201111002739000017170000001717000000000001000000001717000000282838393a0021222352535424253328282838290022232b00000828393b27000000001424230000001200000028290000000000282828102867001717171717282839000031333927101228370000000000000000254825252526242526212222222222223a303800000000000000000000000000001717000000000000003a28282828280024252652535424262828282828283925262b00003a28103b30000000212225260000002700003a28000000000000282838282828390000005868283828000022233830281728270000000000000000000000000000000028242525252548253329000031323232253232323232253200000000000000003b200000313232252526002a283824252532323232323232000000000000002a10282900313232256363636452535353545500000055525325482525262b0000000000002425252526282828242548252525482525252525000000000000586828244825252525251b0016001b1b1b1b301b1b1b1b1b301b000000000000001111200000002a282425330000002a313233000029000038290000000000001100002a0000002a28310000000062636363645500000055525325252548262b3a000000000024254825262838283132323232323232252548250000000000002a102831322525482525000000000000000037000000000037000000000000003b343536001400001031262b0000000000000000000000002a0000000000003b202b0012000000002a280000000000000000006500000055626325252532332b28290011112024252525261029002829000000002a283125252500000000000000002a2828242525254800000000000000001b00000000001b0000000000000000003b3435363900001b267273737373737373737374110000160000001100001b003b202b00000016280000000000000000000000000065000025253328282828103a21222225252525262800002a001111110000283824252500000000000000393a282824252525250000160000110000000000110000000011000000000000000000002010280011252222222222222222222222232b000000003b202b000000001b00000000002a000000000000000039000000000000004826282828382828283132322548252526290000001142434400002a28242548000000000000002a2838282448252525000000003b202b3916003b202b00160027393a00000000000000001b2a283921254825252525253232323232262b00000000001b00000011000000000000000000000000390000682800003a0000000025262828102a002a282829003132323226111111114253535400003b21252525000000000000000028282831323232322800003a001b3a283900001b000000003728383911000000000000113a282831323232322548262829002a10372b00160000000000003b202b0000160000000000003a582828682828282828390000002526382829000000002a00000000000025222222236263636400003b24252525000000111111113a2828101b1b1b1b1b283928283900282800000000000000001b002a282711000000003b272828381b22222223313233160000682900000000000100000000001b0000000000000000683928282828102900002a10283a67682526282839000000000000000000000025254825252328382900003b24482525000068212222232838282839000000002828282828382829000000000000160011003a283727000000003b372a2829112525482638282a0000002a0000000000002011111111111111111111111100162828282a38282800000000002828282825262829000000000000000000000000252525252526102a0000003b24252525000010313225252235353536000000002838002a282828103939000011000000272828291b3039000000001b002a282125322526290000000000003a0000000000343535353535353535353535362b0028282900282829006061003a2838282a482600000000000000000000000000002548252525262900001111112425252500002a28283132261b1b1b1b0000000029000000002a28282828393b272b0000372810001130283a00000011002828312601313316000000000000283900000000202838282828202828282828272b00382800002a283d0070713f28282900002526000000000000390000000000000025252525482600003b212223312548250000002838293b30000000000012000000000000000000282828283b302b00001b2a283827302810390000273a28291b26170000000000003900003828000000001b2a282810281b2a28382a28372b002828390000282122232122232800003a3233000000000000282900000000000032323232323300003b242525233125250000002a28003b37000000003a27000000010000001111111128383b372b003a000028283730290028380037283800112639000000000000280000282829000000000029162a28000028160028280000102900000020313233244826284243430000000000000000280000586800000038282829000000003b2425482523313200000100280000280000002a2830000022222311112135353610282829000038000029001b3000002a28000028280021262829000000003a2829002828000000005868000028283a2828390028291600280001000021222222252525235253530000003f0100003a2800002a1000000028290000000039003b2425252525222240002122232b003839120068383039002548252222260c002a28282800003a28010000003a3708000028390028290031262800000000002838003a2810000000002a282828292a2828382828280000004343434344242548252525252652535300000021360000382839000028001c0028013d3e003a28003b2425252548252521232425262b68282827282810302800252525254826000000282829000028282300000038283900002a28102800001b2638390000000028280028282839000000000038280000002a28102800000000535353535424252525252548265253530000003042440028282800002839000022222223102838393b242525252525252525252525262b1b1b1b3132322526004825323232323232252532323233282825252525252525252525482532332b2832323232322526282800003b24252525254825252525254848252525263828242548252532332828292425323232252500000000000000000000000000000000000000000000000000000000000000002525254825262b0000001b1b1b242600323328670000002824332b2a281028382525254825253232323225261b1b00382901003a282426382900003b2432323232323232323232323232322526102831252532331b1b1028143133382828244800000000000000000000000000000000000000000000000000000000000000002525252525262b00000000002a242667282838290000112a372b003a282729002525252525331b1b1b1b31330000002835353536282426281400003b30282a2a1a2829002a2838282810282426002a3825261b1b00003821232b0000081024250000000000003a000000000000000000000000000000000000000000000000002548253232332b00000011002824262928102a00003b2700290000003830000032322525262b000000003b2700003a282828382828242522232b003b372908000010001100292a2829112a3133003a2825262b0000002a31332b00006828242500000000000010000039000000000000000000000000000000000000000000002525262122232b003a67272b2a24263828290000003b30000000003a2830000022233132332b000000003b302839002a3828291029244825262b000000000000002a3b202b3a16283b202b001100002825262b000000001b1b00002a382a2425000000003a00280000380000000000000000000000494a4b4c4d4e4f000000002525262448262b002a38302b1224262938000000003b3000000000002a303e1425252222232b000000003b30290000002829000000242525262b0000000000000000001b002a2828001b003b202b112a25262b000000000000000000293b2448000000002867280000100039000000000000000000595a5b5c5d5e5f000000003232333132262b120028302b1731330028395868393b3011111111111124222225254825262b000011003b302b0000002811111111242548262b0000000000000000000000001110675811001b3b2016482611111111000000160000003b3125000000002838287600286728000000000000000000696a6b6c6d6e6f00000000280000002a301127002a302b001010392828382834353235353535353525254825252525262b003b27003b302b0000002834353535323225262b00001100000000000000003b20282838272b16001b002525232122232b000000000000001b24000000002a28282123283829000000000000000000797a7b7c7d7e7f0000000029001100003135262b0031353535353529002a0000102828282829003b24252532323232262b163b30003b3000000000291b1b1b1b1b3b24262b003b272b00000011000000001b2a2828372b000000003232333132332b000011110000003b240000006838282125252328393a00000000000000000000000000000000000000003b272b002a38302b0000002a28393b0000000000002a2838282867212525251b1b1b1b372b003b30393b30160000000000000000003b24262b173b302b00003b202b110000001100291b000000003a1b1b1b1b1b1b00111121231100003b240000002a28212548252523283868000000000000000000000000000000000000083b302b00002830111111110028383b111111111100002829002928242548250000003a2800003b30283b37000000000000000000003b2426111111302b0000001b3b202b003b272b000000000000282b01000000003b21222532362b003b315858682829242525252526102828680000000000000000000000000000000000003b372b00002a31353535360028293b22222222231111202b00002a31322525000000103829003b3028282800000000111111113900112425222222332b00000100001b00003b302b000000005868282b17000000003b2425331b1b0000001b281028380031322525482629002a28000000000000000000000000000000000039001b000000001b1b1b1b1b002a003b323232323235361b000000001b1b3125000000002800003b30283829000000002222222328383432323232332b0000002339000000003b372b000000002a10281111111100003b31331b003a00160000002a28393f2123242532332000002800000000000000000000000000000000002829000001001400000000000000003b39013d00002a28000000000000002824000000682867003b30002a286700000025252526002828003a10282a0016003a262800000000001b000000393a283828222222232b00001b1b0000380000006800002122222526313321222328392867000000000000000000000000000000001067583a21222311111111110012003b222223390010283900000000083a103101003a382829003b370000382900000025482526002a28282838290000000028263839120000000000003a2828282810254825262b160000390000283a003a2801003125254825222225252523103828000000000000000000000000000000002838282824252522222222230017003b252526283a28382800000000003828282300102828000000380000281000000025252526171728382800000000003a282628102739000000002a282838282828252548262b00003a38003a28102838282122232425252525252548252522222300000000000000000000000000000000";
	for (int i = 0, c = *mapdata; c; c = mapdata[i+=2]) {
		char s[] = {'0', 'x', mapdata[i], mapdata[i+1], 0};
		sscanf(s, "%hhx", &tilemap[i/2]);
	}

	memcpy(palette_swap, base_palette, sizeof(palette_swap));

	ALLEGRO_BITMAP* font_bmp = al_load_bitmap("assets/font.png");
	//load font from bitmap:  https://liballeg.org/a5docs/trunk/font.html#al_grab_font_from_bitmap
	int ranges[] = {
		'a', 'z', '0', '9',
		'~','~', '!','!', '@','@', '#','#', '$','$', '%','%', '^', '^', '&','&', '*','*', '(',')',
		'_','_', '+','+', '-','-', '=','=', '?','?', ':',':', '.','.', ' ',' '
	};
	font = al_grab_font_from_bitmap(font_bmp, (sizeof(ranges)/sizeof(*ranges))/2, ranges);
	al_destroy_bitmap(font_bmp);

	atlas = al_load_bitmap("assets/atlas.png"); //the graphics atlas
	strong_assert(atlas != NULL);
	for (int tx = 0; tx < al_get_bitmap_width(atlas)/8; tx++) {
		for (int ty = 0; ty < al_get_bitmap_height(atlas)/8; ty++) {
			int i = tx + ty*al_get_bitmap_width(atlas)/8;
			sprites[i] = al_create_sub_bitmap(atlas, tx*8, ty*8, 8, 8);
		}
	}
}

void DeinitEmu() {
	if (atlas) al_destroy_bitmap(atlas);
	if (font) al_destroy_font(font);
	for (int i = 0; i < sizeof(sprites)/sizeof(*sprites); i++)
		if (sprites[i]) al_destroy_bitmap(sprites[i]);
}

//input:
static int key_mappings[] = {
	ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN,
	ALLEGRO_KEY_C, ALLEGRO_KEY_X
};
static unsigned char button_state = 0;
// k_left = 0;
// k_right = 1;
// k_up = 2;
// k_down = 3;
// k_jump = 4;
// k_dash = 5;

void InputEvKeyDown(int k) {
	for (int i = 0; i < sizeof(key_mappings)/sizeof(*key_mappings); i++) {
		if (k == key_mappings[i]) {
			button_state |= 1 << i;
		}
	}
}

void InputEvKeyUp(int k) {
	for (int i = 0; i < sizeof(key_mappings)/sizeof(*key_mappings); i++) {
		if (k == key_mappings[i]) {
			button_state &= ~(1 << i);
		}
	}
}

void EmuUpdate() {
	Celeste_P8_update();
}

void EmuDraw() {
	//draw normally

	Celeste_P8_draw();

	//now do the palette swap
	#if 1
	ALLEGRO_BITMAP* target = al_get_target_bitmap();
	al_lock_bitmap(target, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READWRITE);
	for (int x = 0; x < 128; x++) for (int y = 0; y < 128; y++) {
		ALLEGRO_COLOR pix = al_get_pixel(target, x, y);
		for (int i = 0; i < sizeof(base_palette)/sizeof(*base_palette); i++) {
			ALLEGRO_COLOR col = base_palette[i];
			//float diff = (pix.r-col.r)*(pix.r-col.r) + (pix.g-col.g)*(pix.g-col.g) + (pix.b-col.b)*(pix.b-col.b);
			
//			if ((pix.r+pix.g+pix.b) == (col.r+col.g+col.b)) {
			if (*(long*)&pix == *(long*)&col) { //cool hack but is it portable
				ALLEGRO_COLOR newpix = palette_swap[i];
				//al_put_pixel(x, y, newpix); //this doesnt work..??
				//but this does:
				pix.r = newpix.r, pix.g = newpix.g, pix.b = newpix.b;
				al_put_pixel(x, y, pix);
				goto next;

			}
		}
		next:;
	}
	al_unlock_bitmap(target);
	#endif
}

static int fget(int tile, int flag) {
	return tile < sizeof(tile_flags)/sizeof(*tile_flags) && (tile_flags[tile] & (1 << flag)) != 0;
}

static int camera_x = 0, camera_y = 0; //used by pico8's camera()

Celeste_P8_val HandleP8Call(CELESTE_P8_CALLBACK_TYPE t, ...) {
	va_list args;
	va_start(args, t);
	Celeste_P8_val ret = {.i = 0};

	#define NEXT_INT() va_arg(args, int)
	#define NEXT_BOOL() va_arg(args, Celeste_P8_bool_t)
	#define NEXT_FLOAT() va_arg(args, double)
	#define RETI(x) do{ ret = (Celeste_P8_val){.i=x}; goto end; }while(0)
	#define RETF(x) do{ ret = (Celeste_P8_val){.f=x}; goto end; }while(0)
	#define OP(name, ...) case CELESTE_P8_##name: { __VA_ARGS__ } break;

	switch (t) {
		OP(RND,
			float max = NEXT_FLOAT(); 
			float v = max*((float)random() / (float)RAND_MAX);
			RETF(v);
		)
		OP(MUSIC,
			int index = NEXT_INT();
			int fade = NEXT_INT();
			int mask = NEXT_INT();
			//
		)
		OP(SPR,
			int sprite = NEXT_INT();
			float x = NEXT_FLOAT(), y = NEXT_FLOAT();
			int cols = NEXT_INT();
			int rows = NEXT_INT();
			bool flipx = NEXT_BOOL();
			bool flipy = NEXT_BOOL();

			if (sprite >= 0) {
				int flag = (flipx ? ALLEGRO_FLIP_HORIZONTAL : 0) | (flipy ? ALLEGRO_FLIP_VERTICAL : 0);
				//al_draw_bitmap_region(atlas, 8*(sprite%128), 8*(sprite/128), 8*cols, 8*rows, x, y, flag);
				if (sprites[sprite]) al_draw_bitmap(sprites[sprite], x - camera_x, y - camera_y, flag);
			}
		)
		OP(BTN,
			int b = NEXT_INT();

			RETI(button_state & (1<<b));
		)
		OP(SFX,
			int id = NEXT_INT();
		)
		OP(PAL,
			int a = NEXT_INT();
			int b = NEXT_INT();
			palette_swap[a] = base_palette[b];
		)
		OP(PAL_RESET,
			memcpy(palette_swap, base_palette, sizeof(palette_swap));
		)
		OP(CIRCFILL,
			float x = NEXT_FLOAT(), y = NEXT_FLOAT();
			float r = NEXT_FLOAT();
			int c = NEXT_INT();

			al_draw_filled_circle(x - camera_x, y - camera_y, r, base_palette[c]);
		)
		OP(PRINT,
			char* str = va_arg(args, char*);
			float x = NEXT_FLOAT(), y = NEXT_FLOAT();
			int c = NEXT_INT();

			al_draw_text(font, base_palette[c], x - camera_x, y - camera_y, 0, str);
		)
		OP(RECTFILL,
			float x = NEXT_FLOAT(), y = NEXT_FLOAT();
			float x2 = NEXT_FLOAT(), y2 = NEXT_FLOAT();
			int c = NEXT_INT();

			al_draw_filled_rectangle(x - camera_x, y - camera_y, x2+1, y2+1, base_palette[c]);
		)
		OP(LINE,
			float x = NEXT_FLOAT(), y = NEXT_FLOAT();
			float x2 = NEXT_FLOAT(), y2 = NEXT_FLOAT();
			int c = NEXT_INT();

			al_draw_line(x-camera_x,y-camera_y,x2-camera_x,y2-camera_y,base_palette[c],-1);
		)
		OP(MGET,
			int tx = NEXT_INT(), ty = NEXT_INT();

			RETI(tilemap[tx + ty*128]);
		)
		OP(CAMERA,
			int x = NEXT_INT(), y = NEXT_INT();


			camera_x = x, camera_y = y;
		)
		OP(FGET,
			int tile = NEXT_INT(), flag = NEXT_INT();

			RETI(fget(tile, flag));
		)
		OP(MAP,
			int mx = NEXT_INT(), my = NEXT_INT();
			int tx = NEXT_INT(), ty = NEXT_INT();
			int mw = NEXT_INT(), mh = NEXT_INT();
			int mask = NEXT_INT();

			//draw map
			for (int x = 0; x < mw; x++) {
				for (int y = 0; y < mh; y++) {
					int tile = tilemap[x + mx + (y + my)*128];
					//this is a horrible hack... i dont know how to get this to work otherwise..., but it makes the bg work
					if (mask == 0 || (mask == 4 && tile_flags[tile] == 4) || fget(tile, mask != 4 ? mask-1 : mask)) {
						al_draw_bitmap(sprites[tile], tx+x*8 - camera_x, ty+y*8 - camera_y, 0);
					}
				}
			}
		)
	}

	end:
	va_end(args);
	return ret;
}
