#include "../SDKManager.h"
#include "../../Plugin/CocosEasyPlugin.h"




namespace PhoneDirect3DXamlAppComponent 
{
	public delegate int BuyGoodDelegate(unsigned int id, Platform::String^ price);
	public ref class PayDelegate sealed
	{
	public:
		PayDelegate(void){}
		void SetBuyGoodDelegate(BuyGoodDelegate^ del)
		{
			m_BuyGoodDelegate = del;

		}
		int BuyGood(unsigned int id,Platform::String^ data)
		{            
			if(m_BuyGoodDelegate)
			{
				return m_BuyGoodDelegate->Invoke(id,data);
			}
			return 0;
		}

	private:
		property static BuyGoodDelegate^ m_BuyGoodDelegate;
	};

}
PhoneDirect3DXamlAppComponent::PayDelegate^ manager;



void CSDKManager::createRoleAnayasis(unsigned int serverID){
}

void CSDKManager::enterMainSceneAnayasis(unsigned int serverID){
}

void CSDKManager::initPlatform(int code)
{
	m_ChannelID = SDK_CHANNEL_WP_WFK;
}

void CSDKManager::preLoadGoods(const char* goodsList){
}

void CSDKManager::login()
{
	loginCallBack(true);
}

bool CSDKManager::isLogined()
{
	return true;
}

bool CSDKManager::isInited()
{
	return true;
}

const std::string CSDKManager::getAccountName()
{
	return CCEPlugin::getDeviceID();
}

const std::string CSDKManager::getAccountID()
{
	return CCEPlugin::getDeviceID();
}

const std::string CSDKManager::getSessionID()
{
	return CCEPlugin::getDeviceID();
}
void CSDKManager::logout(bool iscallback)
{
}

const std::string CSDKManager::getMoneyName()
{
	return "";
}
 
void CSDKManager::userCenter()
{
	switch (m_ChannelID)
	{
        case SDK_CHANNEL_NONE:
            break;
        case SDK_CHANNEL_91:
            break;
        case SDK_CHANNEL_360:
            break;
        case SDK_CHANNEL_PP:
            break;
        case SDK_CHANNEL_MU_ZHI_WAN:
            break;
        case SDK_CHANNEL_APP_STORE:
            break;
        case SDK_CHANNEL_GOOGLE_PLAY:
            break;
        case SDK_CHANNEL_WIN:
            break;
        case SDK_CHANNEL_TB:
            break;
        case SDK_CHANNEL_DEBUG:
            break;
		case SDK_CHANNEL_WP_WFK:
			break;
        default:
            break;
	}	
}

void CSDKManager::pay( const char* parmas)
{
	string str = "{\"transaction_id\":\"1000000157371906\",\"receipt-data\":\"ewoJInNpZ25hdHVyZSIgPSAiQWoxYWtJeXZnWG5Bd1M3NjNwOEx5cHVxRjBqZVlscGE0Z0h6RExHTnJFRis3cnJGb056aGRacWVLb2thSjR3N2g4S1A0M0hGSmxMenFNUkc2eGFpYms5SUlqcXlsMWpaL3p5VUpZRjBBaTY4OHlGTFNXK1pOaFlkaTFjTGJLMC9MdjRNaGRodjQ1Q2VGRTFPa1lEVHpYQXN3ZGwwK3d6ayt3L3lhdFk2NmRpZ0FBQURWekNDQTFNd2dnSTdvQU1DQVFJQ0NCdXA0K1BBaG0vTE1BMEdDU3FHU0liM0RRRUJCUVVBTUg4eEN6QUpCZ05WQkFZVEFsVlRNUk13RVFZRFZRUUtEQXBCY0hCc1pTQkpibU11TVNZd0pBWURWUVFMREIxQmNIQnNaU0JEWlhKMGFXWnBZMkYwYVc5dUlFRjFkR2h2Y21sMGVURXpNREVHQTFVRUF3d3FRWEJ3YkdVZ2FWUjFibVZ6SUZOMGIzSmxJRU5sY25ScFptbGpZWFJwYjI0Z1FYVjBhRzl5YVhSNU1CNFhEVEUwTURZd056QXdNREl5TVZvWERURTJNRFV4T0RFNE16RXpNRm93WkRFak1DRUdBMVVFQXd3YVVIVnlZMmhoYzJWU1pXTmxhWEIwUTJWeWRHbG1hV05oZEdVeEd6QVpCZ05WQkFzTUVrRndjR3hsSUdsVWRXNWxjeUJUZEc5eVpURVRNQkVHQTFVRUNnd0tRWEJ3YkdVZ1NXNWpMakVMTUFrR0ExVUVCaE1DVlZNd2daOHdEUVlKS29aSWh2Y05BUUVCQlFBRGdZMEFNSUdKQW9HQkFNbVRFdUxnamltTHdSSnh5MW9FZjBlc1VORFZFSWU2d0Rzbm5hbDE0aE5CdDF2MTk1WDZuOTNZTzdnaTNvclBTdXg5RDU1NFNrTXArU2F5Zzg0bFRjMzYyVXRtWUxwV25iMzRucXlHeDlLQlZUeTVPR1Y0bGpFMU93QytvVG5STStRTFJDbWVOeE1iUFpoUzQ3VCtlWnRERWhWQjl1c2szK0pNMkNvZ2Z3bzdBZ01CQUFHamNqQndNQjBHQTFVZERnUVdCQlNKYUVlTnVxOURmNlpmTjY4RmUrSTJ1MjJzc0RBTUJnTlZIUk1CQWY4RUFqQUFNQjhHQTFVZEl3UVlNQmFBRkRZZDZPS2RndElCR0xVeWF3N1hRd3VSV0VNNk1BNEdBMVVkRHdFQi93UUVBd0lIZ0RBUUJnb3Foa2lHOTJOa0JnVUJCQUlGQURBTkJna3Foa2lHOXcwQkFRVUZBQU9DQVFFQWVhSlYyVTUxcnhmY3FBQWU1QzIvZkVXOEtVbDRpTzRsTXV0YTdONlh6UDFwWkl6MU5ra0N0SUl3ZXlOajVVUllISytIalJLU1U5UkxndU5sMG5rZnhxT2JpTWNrd1J1ZEtTcTY5Tkluclp5Q0Q2NlI0Szc3bmI5bE1UQUJTU1lsc0t0OG9OdGxoZ1IvMWtqU1NSUWNIa3RzRGNTaVFHS01ka1NscDRBeVhmN3ZuSFBCZTR5Q3dZVjJQcFNOMDRrYm9pSjNwQmx4c0d3Vi9abEwyNk0ydWVZSEtZQ3VYaGRxRnd4VmdtNTJoM29lSk9PdC92WTRFY1FxN2VxSG02bTAzWjliN1BSellNMktHWEhEbU9Nazd2RHBlTVZsTERQU0dZejErVTNzRHhKemViU3BiYUptVDdpbXpVS2ZnZ0VZN3h4ZjRjemZIMHlqNXdOelNHVE92UT09IjsKCSJwdXJjaGFzZS1pbmZvIiA9ICJld29KSW05eWFXZHBibUZzTFhCMWNtTm9ZWE5sTFdSaGRHVXRjSE4wSWlBOUlDSXlNREUxTFRBMUxUTXhJREl6T2pVMU9qRXdJRUZ0WlhKcFkyRXZURzl6WDBGdVoyVnNaWE1pT3dvSkluVnVhWEYxWlMxcFpHVnVkR2xtYVdWeUlpQTlJQ0kyTldaaE4yWXhZakl5WWpObVkySTFZemszTTJabE9HVTNNVEV6WWpObU9XTTFOakZoTlRGaElqc0tDU0p2Y21sbmFXNWhiQzEwY21GdWMyRmpkR2x2YmkxcFpDSWdQU0FpTVRBd01EQXdNREUxTnpNM01Ua3dOaUk3Q2draVluWnljeUlnUFNBaU1DNDRMaklpT3dvSkluUnlZVzV6WVdOMGFXOXVMV2xrSWlBOUlDSXhNREF3TURBd01UVTNNemN4T1RBMklqc0tDU0p4ZFdGdWRHbDBlU0lnUFNBaU1TSTdDZ2tpYjNKcFoybHVZV3d0Y0hWeVkyaGhjMlV0WkdGMFpTMXRjeUlnUFNBaU1UUXpNekUwTVRjeE1EUTBPQ0k3Q2draWRXNXBjWFZsTFhabGJtUnZjaTFwWkdWdWRHbG1hV1Z5SWlBOUlDSTJOMEkzTlRBNE5pMHpSamRHTFRSRU4wRXRRalZETnkxQ05EUTJOa0V4UmtZeVFqVWlPd29KSW5CeWIyUjFZM1F0YVdRaUlEMGdJbU52YlM1allYTnBibTh1WjJWdE5pSTdDZ2tpYVhSbGJTMXBaQ0lnUFNBaU1UQXdNRFUxTURnMU1TSTdDZ2tpWW1sa0lpQTlJQ0pqYjIwdWFHdG9heTUwWlhOMElqc0tDU0p3ZFhKamFHRnpaUzFrWVhSbExXMXpJaUE5SUNJeE5ETXpNVFF4TnpFd05EUTRJanNLQ1NKd2RYSmphR0Z6WlMxa1lYUmxJaUE5SUNJeU1ERTFMVEEyTFRBeElEQTJPalUxT2pFd0lFVjBZeTlIVFZRaU93b0pJbkIxY21Ob1lYTmxMV1JoZEdVdGNITjBJaUE5SUNJeU1ERTFMVEExTFRNeElESXpPalUxT2pFd0lFRnRaWEpwWTJFdlRHOXpYMEZ1WjJWc1pYTWlPd29KSW05eWFXZHBibUZzTFhCMWNtTm9ZWE5sTFdSaGRHVWlJRDBnSWpJd01UVXRNRFl0TURFZ01EWTZOVFU2TVRBZ1JYUmpMMGROVkNJN0NuMD0iOwoJImVudmlyb25tZW50IiA9ICJTYW5kYm94IjsKCSJwb2QiID0gIjEwMCI7Cgkic2lnbmluZy1zdGF0dXMiID0gIjAiOwp9\"}";
	this->buyCallBack(true,str.c_str());
}

void CSDKManager::showWebPayView(std::string url,
                                 std::string jsonData){
}

void CSDKManager::unInit()
{
	return;
}

void CSDKManager::payWithPayPal(const char* parmas){
}
AppThirdLoginType CSDKManager::getLoginType()
{
    return kAppThirdLoginTypeNone;
}