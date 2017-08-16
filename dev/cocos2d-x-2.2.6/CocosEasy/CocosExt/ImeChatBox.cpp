#include "ImeChatBox.h"
#include "RichTextExt.h"
#include "Utility/Utils.h"
USING_NS_CC_EASY;

cocoseasy::CEImeChatBox::CEImeChatBox()
	:m_isDebug(false)
	 ,m_editBoxEditingDidBeginHandler(-1)
	 ,m_editBoxEditingDidEndHandler(-1) 
	 ,m_insertTextHandler(-1)  
	 ,m_editBoxReturnHandler(-1) 
	 ,m_placeholder("|")
	 ,m_fBlinkInteval(0.7f)
	 ,m_maxLength(2)
	 ,m_pEditBox(NULL)
{
	
}

cocoseasy::CEImeChatBox::~CEImeChatBox()
{
	CC_SAFE_RELEASE(m_pLabelInput);
	CC_SAFE_RELEASE(m_pPlaceholder);
	CC_SAFE_RELEASE(m_pEditBox);
}


bool cocoseasy::CEImeChatBox::init(CCSize size,CCScale9Sprite *pSpr)
{
	Widget::init();
	m_fViewWidth = size.width;

	m_pEditBox = CCEditBox::create(size,pSpr);
	addNode(m_pEditBox);
	m_pEditBox->retain();
	//m_pEditBox->setPosition(ccp(0xffff,0xffff));
	m_pEditBox->setTouchEnabled(false);
	m_pEditBox->setVisible(false);
	m_pEditBox->setAnchorPoint(ccp(0,1));
	m_pEditBox->setDelegate(this);
	m_pEditBox->setReturnType(kKeyboardReturnTypeDone);
	m_pEditBox->setMaxLength(m_maxLength*18);

	float width = size.width*100;
	Widget::init();
	m_pLabelInput = RichTextExt::create(width,"arial");
	addChild(m_pLabelInput);
	m_pLabelInput->retain();
	m_pLabelInput->setAnchorPoint(ccp(0,1));
	m_pLabelInput->setTextHorizontalAlignment(kCCTextAlignmentLeft);

	m_pPlaceholder = RichTextExt::create(width,"arial");
	addChild(m_pPlaceholder);
	m_pPlaceholder->retain();
	m_pPlaceholder->setAnchorPoint(ccp(0,1));
	m_pPlaceholder->setTextHorizontalAlignment(kCCTextAlignmentLeft);
	return true;
}

void cocoseasy::CEImeChatBox::show()
{
	setVisible(false);
	m_pEditBox->setText(m_strInput.c_str());
	m_pEditBox->touchDownAction(m_pEditBox,CCControlEventTouchDown);
   
}

void cocoseasy::CEImeChatBox::hide()
{
	reset();
	updateVisible();
	setVisible(false);
}

void cocoseasy::CEImeChatBox::registInputScript( int editBoxEditingDidBeginHandle, int editBoxEditingDidEndHandler , int insertTextHandler , int editBoxReturnHandler)
{
	m_editBoxEditingDidBeginHandler = editBoxEditingDidBeginHandle;
	m_editBoxEditingDidEndHandler	= editBoxEditingDidEndHandler;
	m_insertTextHandler				= insertTextHandler;
	m_editBoxReturnHandler			= editBoxReturnHandler;
}

void cocoseasy::CEImeChatBox::editBoxEditingDidBegin( CCEditBox* editBox )
{
	log("editBoxEditingDidBegin");
	executeScriptSimple(m_editBoxEditingDidBeginHandler);
}

void cocoseasy::CEImeChatBox::editBoxEditingDidEnd( CCEditBox* editBox )
{
	log("editBoxEditingDidEnd");
	executeScriptSimple(m_editBoxEditingDidEndHandler);
    setVisible(true);
	m_strInput = textFilter(m_strInput.c_str());
	updateVisible();

	//send event to script
	if(m_insertTextHandler<=0)return;
	CCArray* pArrayArgs = CCArray::createWithCapacity(1);
	pArrayArgs->addObject(CCString::create(m_strInput.c_str()));
	CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_insertTextHandler,pArrayArgs);
}

void cocoseasy::CEImeChatBox::editBoxTextChanged( CCEditBox* editBox, const std::string& text )
{
	m_strInput = text;
	log(m_strInput.c_str());
}

void cocoseasy::CEImeChatBox::editBoxReturn( CCEditBox* editBox )
{
	log("editBoxReturn");
	executeScriptSimple(m_editBoxReturnHandler);
}

void cocoseasy::CEImeChatBox::log( const string& str )
{
	if(m_isDebug){
		CCLOG("==================CEImeChatBox:%s" , str.c_str());
	}
}

CEImeChatBox * cocoseasy::CEImeChatBox::create(CCSize size , int maxLength ,CCScale9Sprite *pSpr)
{
	CEImeChatBox* result = new CEImeChatBox;
	result->autorelease();
	result->init(size,pSpr);
	result->setFontSize(20);//20 default size
	result->setMaxLength(maxLength);
	return result;
}

string cocoseasy::CEImeChatBox::textFilter( const char *pChar )
{
	
	string buffer = pChar;
	string result;
	int n = 0;
	for(size_t i =0 ;i<buffer.size(); ++i)
	{
		const char &temp = buffer.at(i);
		if(temp!='\n'){
			result += temp;
		}
	}
	return Engine::utils::removeEmoj(result);
}

void cocoseasy::CEImeChatBox::executeScriptSimple( int handler )
{
	if(handler <= 0) return;
	CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(handler , CCArray::create());
}

void cocoseasy::CEImeChatBox::deleteChar()
{
	if(!m_strInput.empty()){
		m_strInput = m_strInput.substr(0,m_strInput.size()-1);
		updateVisible();
	}
}

void cocoseasy::CEImeChatBox::updateVisible()
{
	m_pLabelInput->setText(m_strInput);
	m_pPlaceholder->setText(m_strInput+m_placeholder);

	float textWidth = m_pPlaceholder->getCurViewWidth();
	float posX = textWidth>m_fViewWidth?m_fViewWidth-textWidth:0.0f;
	m_pLabelInput->setPositionX(posX);
	m_pPlaceholder->setPositionX(posX);
	
}

void cocoseasy::CEImeChatBox::setFontSize( float fontSize )
{
	m_pLabelInput->setDefaultFontSize(fontSize);
	m_pPlaceholder->setDefaultFontSize(fontSize);
}

void cocoseasy::CEImeChatBox::onEnter()
{
	Widget::onEnter();
	scheduleUpdate();
}

void cocoseasy::CEImeChatBox::onExit()
{
	Widget::onExit();
	unscheduleUpdate();
}

void cocoseasy::CEImeChatBox::update( float delta )
{
	static float sum = 0.0f;
	sum += delta;
	if(sum<m_fBlinkInteval){
		m_pLabelInput->setVisible(false);
		m_pPlaceholder->setVisible(true);
	}else if(sum<1.0f){
		m_pLabelInput->setVisible(true);
		m_pPlaceholder->setVisible(false);
	}else{
		sum = 0.0f;
	}
}

void cocoseasy::CEImeChatBox::setFontColor( ccColor3B color )
{
	m_pLabelInput->setColor(color);
	m_pPlaceholder->setColor(color);
}

string cocoseasy::CEImeChatBox::getStrInput()
{
	return m_strInput;
}

void cocoseasy::CEImeChatBox::reset()
{
	m_strInput.clear();
	m_pLabelInput->setText("");
	m_pPlaceholder->setText("");
	m_pEditBox->setText("");
}

void cocoseasy::CEImeChatBox::setFontName( const char *name )
{
	m_pLabelInput->setDefaultFontName(name);
	m_pPlaceholder->setDefaultFontName(name);
}

void cocoseasy::CEImeChatBox::setMaxLength( int maxLength )
{
	m_maxLength = maxLength;
}










