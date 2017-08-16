require "cjson"

-- game name
DEF_GAME_NAME = "Casino"

local jsonStr = CCFileUtils:sharedFileUtils():getFileData("config.json", "rb")
local config = cjson.decode(jsonStr)

DEF_GAME_VERSION 	= config.show_version
DEF_INTERNAL_VERSION= config.innner_version
DEF_VER_CHECK_IP 	= config.version_server_ip
DEF_IAPPAY_PATH 	= config.iappay_path .. "/ipay_order"
DEF_STORAGE_TYPE 	= config.storageType

-- network define
HTTP_PREF = "http://"
DEF_LOGIN_SERVER_IP_LIST = {
	 HTTP_PREF.."52.221.251.178:5001",
}

DEF_LOGIN_SERVER_IP =  DEF_LOGIN_SERVER_IP_LIST[1]

DEF_GAME_SERVER_IP_LIST = {}

DEF_FEEDBACK_PATH = "http://tool.kszjh.info/api/game-feedback"

DEF_UPLOAD_PATH = "http://picture.kszjh.info/upload"

DEF_AVATAR_BASE_PATH = "http://7u2l4h.com1.z0.glb.clouddn.com/"

-- localization
DEF_HB_SEND_TIME = 5--10
DEF_HB_RECV_TIME = 20 * 10
DEF_INSTANT_TIMEOUT = 6


DEF_MSG_PROCESS_MAX_TIME = 15

DEF_HB_SEND_MSG = "ping"
DEF_HB_RECV_MSG = "pong"

LANG = "fonts/en_US.mo"

-- Logo
DEF_MAX_LOG_COUNT = 5

-- language
DEF_DESGIN_LANGUAGE = kLanguageChinese

-- font
DEF_SYSTEM_FONT_NAME = "lanting"
DEF_SYSTEM_FONT_FILE = "lanting.TTF"

DEF_UI_DESIGN_SIZE = CCSizeMake(1136,640)

-- path
DEF_PATH = {
	--art
	ui = "art/ui/",

	-- music
	music = "music/",
	backmusic = "music/backmusic/",
	sound = "music/sound/",
	voice = "music/voice/",

	-- docs
	docs = "docs/",

	-- font
	font = "fonts/",

	-- db
	db = "db/",

	uifont = "font/",
}

_ = assert(require("lua/third/Gettext").gettextFromFile(LANG))
