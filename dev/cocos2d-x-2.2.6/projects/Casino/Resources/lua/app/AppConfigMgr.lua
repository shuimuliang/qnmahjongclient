require "lua/common/AudioEngine"
require "lua/common/CCBReaderLoad"
require "lua/common/extern"
require "lua/common/cpp"
require "lua/common/CocosEasy"
require "lua/common/log"
require "lua/common/CoroutineEasy"

require "lua/Debug"
require "lua/ErrorCode"

require "lua/app/AppUtils"
require "lua/app/XStr"
require "lua/app/AppConstants"
require "lua/app/AppMacroDefine"
require "lua/localization/localword_ch"
require "lua/Resources"

require "lua/app/AppGlobal"
require "lua/app/AppStyle"

-- model
require "lua/model/Analysis"
require "lua/model/UserContext"
require "lua/model/MusicBox"
require "lua/model/Platform"
require "lua/model/EffectObject"
require "lua/model/EventDispatcher"

-- gameData ==========
require "lua/gameData/GameData"
require "lua/gameData/PlayerData"
require "lua/gameData/ShopData"
require "lua/gameData/UserInfoData"
require "lua/gameData/SystemSwitcher"
require "lua/gameData/BroadcastData"
require "lua/gameData/GameChatData"

require "lua/scene/mj/data/MJPlayerData"
require "lua/scene/mj/data/MJCardData"
require "lua/scene/mj/data/MJCardsSet"

-- third =====================
require "lua/third/protobuf"

-- network ====================
require "lua/network/HttpClient"
require "lua/network/NetManager"
require "lua/network/NetConfigure"
require "lua/network/PacketManager"
require "lua/network/SocketBase"
require "lua/network/SocketKeep"
require "lua/network/Proto_t"

-- info
require "lua/info/DataManager"
require "lua/info/ShopInfo"
require "lua/info/GiftInfo"
require "lua/info/TalkingInfo"

-- scene =====================
--sceneUtils
require "lua/scene/sceneUtils/ProgressHUD"
require "lua/scene/sceneUtils/SceneBase"
require "lua/scene/sceneUtils/SceneTransit"
require "lua/scene/sceneUtils/PageBase"
require "lua/scene/sceneUtils/MessageBox"
require "lua/scene/sceneUtils/Transition"

--mj =========================
require ("lua/scene/mj/MJUtil")
require ("lua/scene/mj/MJSendMgr")
