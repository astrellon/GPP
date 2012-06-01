#include "gfx_engine.h"

#include <math.h>

#include "../gl.h"
#include "IL/il.h"

#include "gfx_renderable.h"
#include "gfx_asset.h"
#include "gfx_texture.h"
#include "gfx_font.h"
#include "gfx_text_field.h"
#include "gfx_sprite.h"
#include "gfx_layer.h"
#include "gfx_text_list.h"
#include "gfx_log_listener.h"

#include "../event.h"
#include "../mouse_event.h"
#include "../keyboard_event.h"

#include "../logger.h"

#include <sstream>

using namespace std;
using namespace am::ui;

namespace am {
namespace gfx {

	GfxEngine::GfxEngine() :
		mCursor(NULL)
	{
		
	}
	GfxEngine::~GfxEngine()
	{

	}

	void GfxEngine::init()
	{
		// Initialize the DevIL framework.
		ilInit();

		glClearColor (0.0f, 0.3f, 0.4f, 0.0f);
		// Enable texture for the text fields.
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
	
		/*glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);*/

		mRootLayer = new Layer(this);
		mRootLayer->setEnableInteractive(true);

		Asset *mCursorAsset = getAsset("cursor");
		if (mCursorAsset == NULL)
		{
			// TODO: ERROR!
		}
		mCursor = new Sprite(this, mCursorAsset);
		mRootLayer->addChild(mCursor);

		TextList *list = new TextList(this);
		mRootLayer->addChild(list);

		list->setWidth(600.0f);
		list->setBaseFont(getFont("basic"));

		GfxLogListener *listener = new GfxLogListener(list);
		am::log::Logger::getMainLogger().addLogListener(listener);
	}
	void GfxEngine::deinit()
	{
		ilShutDown();
	}
	void GfxEngine::setOrthographic()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(0, mScreenWidth, mScreenHeight, 0, 0, 1);
	
		glMatrixMode(GL_MODELVIEW);
	}
	void GfxEngine::setPerspective()
	{
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity();

		GLfloat zNear = 0.1f;
		GLfloat zFar = 1000.0f;
		GLfloat aspect = float(mScreenWidth)/float(mScreenHeight);
		GLfloat fH = tan( float(65.0f / 360.0f * 3.14159f) ) * zNear;
		GLfloat fW = fH * aspect;
		glFrustum( -fW, fW, -fH, fH, zNear, zFar );

		glMatrixMode(GL_MODELVIEW);
	}

	void GfxEngine::display(float dt)
	{
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		setOrthographic();

		vector<Renderable *>::iterator iter;

		mRootLayer->render(dt);
		if (mCursor != NULL)
		{
			mCursor->render(dt);
		}
	}

	void GfxEngine::reshape(int width, int height)
	{
		mScreenWidth = width;
		mScreenHeight = height;
		glViewport (0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	}

	Sprite *GfxEngine::getCursor()
	{
		return mCursor;
	}
	void GfxEngine::setCursor(Sprite *sprite)
	{
		mRootLayer->removeChild(sprite);
		mCursor = sprite;
		mRootLayer->addChild(mCursor);
	}

	Asset *GfxEngine::getAsset(const char *assetName)
	{
		string assetNameStr = assetName;
		AssetManager::iterator iter = mAssetManager.find(assetNameStr);
		if (iter != mAssetManager.end())
		{
			return iter->second;
		}

		stringstream ss;
		ss << "data/assets/" << assetNameStr << ".ssff";

		JsonValue loaded = JsonValue::import_from_file(ss.str().c_str());
		if (loaded.getType() != JV_OBJ)
		{
			stringstream errss;
			errss << "Unable to load asset '" << assetNameStr << "', using the path '";
			errss << ss.str() << '\''; 
			am_log("ASST", errss.str().c_str());
			return NULL;
		}

		Asset *asset = new Asset(this, assetName);
		int loadAsset = asset->loadDef(loaded);
		if (loadAsset != 0)
		{
			stringstream errss;
			errss << "Error loading asset definition '" << assetNameStr << "': " << loadAsset;
			am_log("ASST", errss.str().c_str());
			delete asset;
			return NULL;
		}

		mAssetManager[assetNameStr] = asset;

		return asset;
	}

	Texture *GfxEngine::getTexture(const char *filename)
	{
		string fileStr = filename;
		TextureManager::iterator iter = mTextureManager.find(fileStr);
		if (iter != mTextureManager.end())
		{
			return iter->second;
		}

		Texture *texture = new Texture(this, filename);
		if (texture->getFilename())
		{
			mTextureManager[fileStr] = texture;
			return texture;
		}
		stringstream errss;
		errss << "Unable to load texture '" << filename << "'";
		am_log("GFX", errss.str().c_str());
		
		return NULL;
	}
	Font *GfxEngine::getFont(const char *fontName)
	{
		string fontNameStr = fontName;
		FontManager::iterator iter = mFontManager.find(fontNameStr);
		if (iter != mFontManager.end())
		{
			return iter->second;
		}

		stringstream ss;
		ss << "data/fonts/" << fontNameStr << ".ssff";

		JsonValue loaded = JsonValue::import_from_file(ss.str().c_str());
		if (loaded.getType() != JV_OBJ)
		{
			stringstream errss;
			errss << "Unable to load font '" << fontNameStr << "', using the path '";
			errss << ss.str() << '\''; 
			am_log("FONT", errss.str().c_str());
			return NULL;
		}

		Font *font = new Font(this, fontName);
		int loadFont = font->loadDef(loaded);
		if (loadFont != 0)
		{
			stringstream errss;
			errss << "Error loading font definition '" << fontNameStr << "': " << loadFont;
			am_log("FONT", errss.str().c_str());
			delete font;
			return NULL;
		}

		mFontManager[fontNameStr] = font;

		return font;
	}

	Layer *GfxEngine::getRootLayer()
	{
		return mRootLayer;
	}
	/*
	void GfxEngine::onMouseDown(int mouseButton, int x, int y)
	{
		mCursor->getTransform().setPosition(am::math::Vector4f(x, y, 0));
		checkForMouseEvent(mRootLayer, "mouse_down", mouseButton, x, y, x, y);
	}
	void GfxEngine::onMouseMove(int mouseButton, int x, int y)
	{
		mCursor->getTransform().setPosition(am::math::Vector4f(x, y, 0));
		checkForMouseEvent(mRootLayer, "mouse_move", mouseButton, x, y, x, y);
	}
	void GfxEngine::onMouseUp(int mouseButton, int x, int y)
	{
		mCursor->getTransform().setPosition(am::math::Vector4f(x, y, 0));
		checkForMouseEvent(mRootLayer, "mouse_up", mouseButton, x, y, x, y);
	}
	*/
	void GfxEngine::onKeyDown(const bool *keys, int key)
	{

	}
	void GfxEngine::onKeyUp(const bool *keys, int key)
	{

	}

}
}
