#include "Animations.h"
#include "Utils.h"

AnimationSets* AnimationSets::instance = NULL;

void  Animation::AddAnimation(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = Sprites::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame = new AnimationFrame(sprite, t);
	frames.push_back(frame);
}

void Animation::Render(float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}
	}
	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}

void Animation::RenderDirect(float x, float y, int direct, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
			//DebugOut(L"now: %d, lastFrameTime: %d, t: %d\n", now, lastFrameTime, t);
		}
	}
	if (direct < 0)
		frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
	else
		frames[currentFrame]->GetSprite()->DrawFlipX(x, y, alpha);
}

Animations* Animations::instance = NULL;

Animations* Animations::GetInstance()
{
	if (instance == NULL) instance = new Animations();
	return instance;
}

void Animations::AddAnimations(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION Animations::Get(int id)
{
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
	return ani;
}

void Animations::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}

AnimationSets::AnimationSets()
{

}

AnimationSets* AnimationSets::GetInstance()
{
	if (instance == NULL) instance = new AnimationSets();
	return instance;
}

LPANIMATION_SET AnimationSets::Get(unsigned int id)
{
	LPANIMATION_SET ani_set = animation_sets[id];
	if (ani_set == NULL)
		DebugOut(L"[ERROR] Failed to find animation set id: %d\n", id);

	return ani_set;
}

void AnimationSets::AddAnimationSet(int id, LPANIMATION_SET ani_set)
{
	animation_sets[id] = ani_set;
}
