#include "TransLabelBMFont.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/TranslationManager.hpp"

/*

#if CC_USE_LA88_LABELS
#define SHADER_PROGRAM kCCShader_PositionTextureColor
#else
#define SHADER_PROGRAM kCCShader_PositionTextureA8Color
#endif

class CachedLabelTTF : public CCLabelTTF
{
    public:
        static inline std::unordered_map<std::string, std::unordered_map<const char*, CCTexture2D*>> textures = {};
        std::string font;

        void setValues(std::string font, float fontSize)
        {
            this->m_tDimensions = CCSizeZero;
            this->m_hAlignment  = kCCTextAlignmentCenter;
            this->m_vAlignment  = kCCVerticalTextAlignmentTop;
            this->m_pFontName   = new std::string(font);
            this->m_fFontSize   = fontSize;
            this->font = font;
        }

        virtual void setString(const char* label)
        {
            if (textures.contains(font) && textures[font].contains(label))
            {
                this->setTexture(textures[font][label]);

                log::info("SET TEXTURE");
            }
            else
            {
                CCLabelTTF::setString(label);

                if (!textures.contains(font))
                    textures[font] = {};

                textures[font][label] = getTexture();

                log::info("ADDED TEXTURE");
            }
        }
};

*/

CCLabelTTF* TransLabelBMFont::createTTF(std::string font, float fontSize)
{
    return CCLabelTTF::create("", font.c_str(), fontSize);

    /*
    auto ttf = new CachedLabelTTF();
    ttf->autorelease();

    // initWithString
    if (ttf->CCSprite::init())
    {
        ttf->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(SHADER_PROGRAM));
        
        ttf->setValues(font, fontSize);
    }

    return ttf;
    */
}

bool TransLabelBMFont::init(std::string text, std::string font)
{
    if (!CCNode::init())
        return false;

    text = TranslationManager::get()->getTranslatedString(text);

    this->text = text;
    this->font = text;

    label = CCLabelBMFont::create(text.c_str(), font.c_str());

    auto l = CCLabelBMFont::create("l", font.c_str());

    ttf = createTTF("Arial.ttf", l->getContentHeight());

    this->setAnchorPoint(ccp(0.5f, 0.5f));

    label->setAnchorPoint(ccp(0, 0));
    ttf->setAnchorPoint(ccp(0, 0));

    updateTTFVisible();

    this->addChild(label);
    this->addChild(ttf);

    return true;
}

void TransLabelBMFont::limitLabelWidth(float width, float defaultScale, float minScale)
{
    this->setScale(clamp<float>(width / getContentWidth(), minScale, defaultScale));
}

void TransLabelBMFont::updateTTFVisible()
{
    useTtf = false;

    for (auto letter : this->text)
    {
        if (!label->getConfiguration()->getCharacterSet()->contains(as<int>(letter)))
        {
            useTtf = true;
            break;
        }
    }

    label->setString(text.c_str());

    if (useTtf)
        ttf->setString(text.c_str());

    label->setVisible(!useTtf);
    ttf->setVisible(useTtf);

    this->setContentSize(useTtf ? ttf->getScaledContentSize() : label->getScaledContentSize());
}

void TransLabelBMFont::setOpacity(int opacity)
{
    label->setOpacity(opacity);
    ttf->setOpacity(opacity);
}

void TransLabelBMFont::setColor(ccColor3B colour)
{
    label->setColor(colour);
    ttf->setColor(colour);
}

std::string TransLabelBMFont::getString()
{
    return text;
}

void TransLabelBMFont::setString(const char* str)
{
    this->text = str;

    updateTTFVisible();
}

TransLabelBMFont* TransLabelBMFont::create(std::string text, std::string font)
{
    auto pRet = new TransLabelBMFont();

    if (pRet && pRet->init(text, font))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}