//
//  TalkingDataGA-Lua.cpp
//  LuaDemo
//
//  Created by liweiqiang on 13-10-24.
//
//

#include "TalkingDataGA-Lua.h"
#include "tolua++.h"
#include "TalkingData.h"

#define kTalkingDataGA       "TalkingDataGA"
#define kTDGAAccount         "TDGAAccount"
#define kTDGAMission         "TDGAMission"
#define kTDGAVirtualCurrency "TDGAVirtualCurrency"
#define kTDGAItem            "TDGAItem"

static TDCCAccount* account = NULL;

void tolua_reg_tdga_types (lua_State* tolua_S)
{
    tolua_usertype(tolua_S, kTalkingDataGA);
    tolua_usertype(tolua_S, kTDGAAccount);
    tolua_usertype(tolua_S, kTDGAMission);
    tolua_usertype(tolua_S, kTDGAVirtualCurrency);
    tolua_usertype(tolua_S, kTDGAItem);
}

EventParamMap tolua_tomap(lua_State* L, int narg)
{
    EventParamMap map;
    if (lua_istable(L, narg)) {
        lua_pushnil(L);
        while (lua_next(L, narg))
        {
            lua_pushvalue(L, -2);
            const char* key = lua_tostring(L, -1);
            const char* value = lua_tostring(L, -2);
            map.insert(EventParamPair(key, value));
            lua_pop(L, 2);
        }
    }
    
    return map;
}

static int tolua_TalkingDataGA_onStart(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTalkingDataGA, 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
        !(tolua_isnoobj(tolua_S, 3, &tolua_err) ||
        tolua_isstring(tolua_S, 3, 0, &tolua_err)))
        goto tolua_lerror;
    else
#endif
    {
        const char* appId = (const char*) tolua_tostring(tolua_S, 2, 0);
        const char* channelId = (const char*) tolua_tostring(tolua_S, 3, 0);
        TDCCTalkingDataGA::onStart(appId, channelId);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'onStart'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TalkingDataGA_onEvent(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTalkingDataGA, 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
        !(tolua_isnoobj(tolua_S, 3, &tolua_err) ||
        tolua_istable(tolua_S, 3, 0, &tolua_err)))
        goto tolua_lerror;
    else
#endif
    {
        const char* eventId = tolua_tostring(tolua_S, 2, 0);
        EventParamMap eventData = tolua_tomap(tolua_S, 3);
        TDCCTalkingDataGA::onEvent(eventId, &eventData);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'onEvent'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TalkingDataGA_setLocation(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTalkingDataGA, 0, &tolua_err) ||
        !tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isnumber(tolua_S, 3, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 4, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        double latitude = (double) tolua_tonumber(tolua_S, 2, 0);
        double longitude = (double) tolua_tonumber(tolua_S, 3, 0);
        TDCCTalkingDataGA::setLatitude(latitude, longitude);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'setLocation'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TalkingDataGA_getDeviceId(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTalkingDataGA, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 2, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        const char* deviceId = TDCCTalkingDataGA::getDeviceId();
        tolua_pushstring(tolua_S, deviceId);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'getDeviceId'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TalkingDataGA_onKill(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTalkingDataGA, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 2, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        TDCCTalkingDataGA::onKill();
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'onKill'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TalkingDataGA_setVerboseLogDisabled(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTalkingDataGA, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 2, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        TDCCTalkingDataGA::setVerboseLogDisabled();
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'setVerboseLogDisabled'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TDGAAccount_setAccount(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTDGAAccount, 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        const char* accountId = tolua_tostring(tolua_S, 2, 0);
        account = TDCCAccount::setAccount(accountId);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'setAccount'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TDGAAccount_setAccountName(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTDGAAccount, 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        if (account)
        {
            const char* accountName = tolua_tostring(tolua_S, 2, 0);
            account->setAccountName(accountName);
        }
#ifndef TOLUA_RELEASE
        else
        {
            tolua_error(tolua_S, "invalid 'account' in function 'setAccountName'.", &tolua_err);
        }
#endif
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'setAccountName'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TDGAAccount_setAccountType(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTDGAAccount, 0, &tolua_err) ||
        !tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        if (account)
        {
            TDCCAccount::TDCCAccountType accountType = (TDCCAccount::TDCCAccountType)tolua_tonumber(tolua_S, 2, 0);
            account->setAccountType(accountType);
        }
#ifndef TOLUA_RELEASE
        else
        {
            tolua_error(tolua_S, "invalid 'account' in function 'setAccountType'.", &tolua_err);
        }
#endif
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'setAccountType", &tolua_err);
    return 0;
#endif
}

static int tolua_TDGAAccount_setLevel(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTDGAAccount, 0, &tolua_err) ||
        !tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        if (account)
        {
            int level = (int) tolua_tonumber(tolua_S, 2, 0);
            account->setLevel(level);
        }
#ifndef TOLUA_RELEASE
        else
        {
            tolua_error(tolua_S, "invalid 'account' in function 'setLevel'.", &tolua_err);
        }
#endif
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'setLevel'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TDGAAccount_setGender(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTDGAAccount, 0, &tolua_err) ||
        !tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        if (account)
        {
            TDCCAccount::TDCCGender gender = (TDCCAccount::TDCCGender)tolua_tonumber(tolua_S, 2, 0);
            account->setGender(gender);
        }
#ifndef TOLUA_RELEASE
        else
        {
            tolua_error(tolua_S, "invalid 'account' in function 'setGender'.", &tolua_err);
        }
#endif
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'setLevel'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TDGAAccount_setAge(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTDGAAccount, 0, &tolua_err) ||
        !tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        if (account)
        {
            int age = (int) tolua_tonumber(tolua_S, 2, 0);
            account->setAge(age);
        }
#ifndef TOLUA_RELEASE
        else
        {
            tolua_error(tolua_S, "invalid 'account' in function 'setAge'.", &tolua_err);
        }
#endif
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'setAge'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TDGAAccount_setGameServer(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTDGAAccount, 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        if (account)
        {
            const char* gameServer = tolua_tostring(tolua_S, 2, 0);
            account->setGameServer(gameServer);
        }
#ifndef TOLUA_RELEASE
        else
        {
            tolua_error(tolua_S, "invalid 'account' in function 'setGameServer'.", &tolua_err);
        }
#endif
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'setGameServer'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TDGAMission_onBegin(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTDGAMission, 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        const char* missionId = tolua_tostring(tolua_S, 2, 0);
        TDCCMission::onBegin(missionId);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'onBegin'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TDGAMission_onCompleted(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTDGAMission, 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        const char* missionId = tolua_tostring(tolua_S, 2, 0);
        TDCCMission::onCompleted(missionId);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'onCompleted'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TDGAMission_onFailed(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTDGAMission, 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
        !(tolua_isnoobj(tolua_S, 3, &tolua_err) ||
        tolua_isstring(tolua_S, 3, 0, &tolua_err)))
        goto tolua_lerror;
    else
#endif
    {
        const char* missionId = tolua_tostring(tolua_S, 2, 0);
        const char* failedCause = tolua_tostring(tolua_S, 3, 0);
        TDCCMission::onFailed(missionId, failedCause);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'onFailed'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TDGAVirtualCurrency_onChargeRequest(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTDGAVirtualCurrency, 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 3, 0, &tolua_err) ||
        !tolua_isnumber(tolua_S, 4, 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 5, 0, &tolua_err) ||
        !tolua_isnumber(tolua_S, 6, 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 7, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 8, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        const char* orderId = tolua_tostring(tolua_S, 2, 0);
        const char* iapId = tolua_tostring(tolua_S, 3, 0);
        double currencyAmount = (double) tolua_tonumber(tolua_S, 4, 0);
        const char* currencyType = tolua_tostring(tolua_S, 5, 0);
        double virtualCurrencyAmount = (double) tolua_tonumber(tolua_S, 6, 0);
        const char* paymentType = tolua_tostring(tolua_S, 7, 0);
        TDCCVirtualCurrency::onChargeRequest(orderId, iapId, currencyAmount, currencyType, virtualCurrencyAmount, paymentType);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'onChargeRequest'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TDGAVirtualCurrency_onChargeSuccess(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTDGAVirtualCurrency, 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        const char* orderId = tolua_tostring(tolua_S, 2, 0);
        TDCCVirtualCurrency::onChargeSuccess(orderId);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'onChargeSuccess'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TDGAVirtualCurrency_onReward(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTDGAVirtualCurrency, 0, &tolua_err) ||
        !tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 3, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 4, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        double currencyAmount = (double) tolua_tonumber(tolua_S, 2, 0);
        const char* reason = tolua_tostring(tolua_S, 3, 0);
        TDCCVirtualCurrency::onReward(currencyAmount, reason);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'onReward'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TDGAItem_onPurchase(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTDGAItem, 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isnumber(tolua_S, 3, 0, &tolua_err) ||
        !tolua_isnumber(tolua_S, 4, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 5, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        const char* item = tolua_tostring(tolua_S, 2, 0);
        int number = (int) tolua_tonumber(tolua_S, 3, 0);
        double price = (double) tolua_tonumber(tolua_S, 4, 0);
        TDCCItem::onPurchase(item, number, price);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'onPurchase'.", &tolua_err);
    return 0;
#endif
}

static int tolua_TDGAItem_onUse(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, kTDGAItem, 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isnumber(tolua_S, 3, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 4, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        const char* item = tolua_tostring(tolua_S, 2, 0);
        int number = tolua_tonumber(tolua_S, 3, 0);
        TDCCItem::onUse(item, number);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'onUse'.", &tolua_err);
    return 0;
#endif
}

void tolua_tdga_open (lua_State* tolua_S)
{
    tolua_cclass(tolua_S, kTalkingDataGA, kTalkingDataGA, "", NULL);
    tolua_beginmodule(tolua_S, kTalkingDataGA);
    tolua_function(tolua_S, "onStart", tolua_TalkingDataGA_onStart);
    tolua_function(tolua_S, "onEvent", tolua_TalkingDataGA_onEvent);
    tolua_function(tolua_S, "setLocation", tolua_TalkingDataGA_setLocation);
    tolua_function(tolua_S, "getDeviceId", tolua_TalkingDataGA_getDeviceId);
    tolua_function(tolua_S, "onKill", tolua_TalkingDataGA_onKill);
    tolua_function(tolua_S, "setVerboseLogDisabled", tolua_TalkingDataGA_setVerboseLogDisabled);
    tolua_endmodule(tolua_S);
    
    tolua_cclass(tolua_S, kTDGAAccount, kTDGAAccount, "", NULL);
    tolua_beginmodule(tolua_S, kTDGAAccount);
    tolua_constant(tolua_S, "kAccountAnonymous", TDCCAccount::kAccountAnonymous);
    tolua_constant(tolua_S, "kAccountRegistered", TDCCAccount::kAccountRegistered);
    tolua_constant(tolua_S, "kAccountSinaWeibo", TDCCAccount::kAccountSianWeibo);
    tolua_constant(tolua_S, "kAccountQQ", TDCCAccount::kAccountQQ);
    tolua_constant(tolua_S, "kAccountTencentWeibo", TDCCAccount::kAccountTencentWeibo);
    tolua_constant(tolua_S, "kAccountND91", TDCCAccount::kAccountND91);
    tolua_constant(tolua_S, "kAccountType1", TDCCAccount::kAccountType1);
    tolua_constant(tolua_S, "kAccountType2", TDCCAccount::kAccountType2);
    tolua_constant(tolua_S, "kAccountType3", TDCCAccount::kAccountType3);
    tolua_constant(tolua_S, "kAccountType4", TDCCAccount::kAccountType4);
    tolua_constant(tolua_S, "kAccountType5", TDCCAccount::kAccountType5);
    tolua_constant(tolua_S, "kAccountType6", TDCCAccount::kAccountType6);
    tolua_constant(tolua_S, "kAccountType7", TDCCAccount::kAccountType7);
    tolua_constant(tolua_S, "kAccountType8", TDCCAccount::kAccountType8);
    tolua_constant(tolua_S, "kAccountType9", TDCCAccount::kAccountType9);
    tolua_constant(tolua_S, "kAccountType10", TDCCAccount::kAccountType10);
    
    tolua_constant(tolua_S, "kGenderUnknown", TDCCAccount::kGenderUnknown);
    tolua_constant(tolua_S, "kGenderMale", TDCCAccount::kGenderMale);
    tolua_constant(tolua_S, "kGenderFemale", TDCCAccount::kGenderFemale);
    
    tolua_function(tolua_S, "setAccount", tolua_TDGAAccount_setAccount);
    tolua_function(tolua_S, "setAccountName", tolua_TDGAAccount_setAccountName);
    tolua_function(tolua_S, "setAccountType", tolua_TDGAAccount_setAccountType);
    tolua_function(tolua_S, "setLevel", tolua_TDGAAccount_setLevel);
    tolua_function(tolua_S, "setGender", tolua_TDGAAccount_setGender);
    tolua_function(tolua_S, "setAge", tolua_TDGAAccount_setAge);
    tolua_function(tolua_S, "setGameServer", tolua_TDGAAccount_setGameServer);
    tolua_endmodule(tolua_S);
    
    tolua_cclass(tolua_S, kTDGAMission, kTDGAMission, "", NULL);
    tolua_beginmodule(tolua_S, kTDGAMission);
    tolua_function(tolua_S, "onBegin", tolua_TDGAMission_onBegin);
    tolua_function(tolua_S, "onCompleted", tolua_TDGAMission_onCompleted);
    tolua_function(tolua_S, "onFailed", tolua_TDGAMission_onFailed);
    tolua_endmodule(tolua_S);
    
    tolua_cclass(tolua_S, kTDGAVirtualCurrency, kTDGAVirtualCurrency, "", NULL);
    tolua_beginmodule(tolua_S, kTDGAVirtualCurrency);
    tolua_function(tolua_S, "onChargeRequest", tolua_TDGAVirtualCurrency_onChargeRequest);
    tolua_function(tolua_S, "onChargeSuccess", tolua_TDGAVirtualCurrency_onChargeSuccess);
    tolua_function(tolua_S, "onReward", tolua_TDGAVirtualCurrency_onReward);
    tolua_endmodule(tolua_S);
    
    tolua_cclass(tolua_S, kTDGAItem, kTDGAItem, "", NULL);
    tolua_beginmodule(tolua_S, kTDGAItem);
    tolua_function(tolua_S, "onPurchase", tolua_TDGAItem_onPurchase);
    tolua_function(tolua_S, "onUse", tolua_TDGAItem_onUse);
    tolua_endmodule(tolua_S);
}
