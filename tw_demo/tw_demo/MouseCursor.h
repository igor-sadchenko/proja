#ifndef __MOUSECURSOR_H__
#define __MOUSECURSOR_H__

#include <OgrePanelOverlayElement.h>
#include <Ogre.h>
#include <sstream>
using namespace std;
using namespace Ogre;

class MouseCursor
{
private:
	static int s_num_mice;
	Overlay* mGuiOverlay;
	OverlayContainer* mCursorContainer;
	TexturePtr mTexture;
	MaterialPtr mMaterial;

	Real mWindowWidth;
	Real mWindowHeight;

public:
	MouseCursor(): mGuiOverlay(0), mCursorContainer(0)
	{
		ostringstream sout1;
		sout1<<"MouseCursor/default"<<s_num_mice;
		
		mMaterial = MaterialManager::getSingleton().create(sout1.str(), "General");
		mCursorContainer = (OverlayContainer*) OverlayManager::getSingletonPtr()->createOverlayElement("Panel", sout1.str());
		mCursorContainer->setMaterialName(mMaterial->getName());
		mCursorContainer->setPosition(0, 0);

		mGuiOverlay = OverlayManager::getSingletonPtr()->create(sout1.str());
		mGuiOverlay->setZOrder(649);
		mGuiOverlay->add2D(mCursorContainer);
		mGuiOverlay->show();
		s_num_mice++;
	}

	~MouseCursor(void) {}

	void setImage(const String& filename)
	{
		mTexture = TextureManager::getSingleton().load(filename, "General");

		TextureUnitState *pTexState;
		if(mMaterial->getTechnique(0)->getPass(0)->getNumTextureUnitStates())
		{
			pTexState = mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);
		}
		else
		{
			pTexState = mMaterial->getTechnique(0)->getPass(0)->createTextureUnitState( mTexture->getName() );
		}
		pTexState->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
		mMaterial->getTechnique(0)->getPass(0)->setSceneBlending(SBT_TRANSPARENT_ALPHA);
	}

	void setWindowDimensions(unsigned int width, unsigned int height) {
		mWindowWidth = (width > 0) ? width : 1;
		mWindowHeight = (height > 0) ? height : 1;

		mCursorContainer->setWidth(mTexture->getWidth() / mWindowWidth);
		mCursorContainer->setHeight(mTexture->getHeight() / mWindowHeight);
	}

	void setVisible(bool visible)
	{
		if(visible) {
			mCursorContainer->show();
		} else {
			mCursorContainer->hide();
		}
	}

	void updatePosition(int x, int y)
	{
		Real rx = x / mWindowWidth;
		Real ry = y / mWindowHeight;
		mCursorContainer->setPosition(clamp(rx, 0.0f, 1.0f), clamp(ry, 0.0f, 1.0f));
	}

	Real clamp(Real a, Real min, Real max) {
		if (a < min) {
			return min;
		}
		if (a > max) {
			return max;
		} 
		return a;
	}
};

#endif
