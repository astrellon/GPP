#include "dialogue.h"

#include "character.h"

namespace am {
namespace game {

	Dialogue::DialogueMap Dialogue::sDialogueMap;

	Dialogue::Dialogue(const char *id, const char *text, const char *title, 
		const char *subject, UnlockFlag unlock, DialogueAction action) :
		mId(id),
		mText(text),
		mUnlockFlag(unlock),
		mAction(action)
	{
		if (title != NULL)
		{
			mTitle = title;
		}
		if (subject != NULL)
		{
			mSubject = subject;
		}
	}
	Dialogue::~Dialogue()
	{
	}

	void Dialogue::setText(const char *text)
	{
		mText = text;
	}
	const char *Dialogue::getText() const
	{
		return mText.c_str();
	}

	void Dialogue::setTitle(const char *title)
	{
		mTitle = title;
	}
	const char *Dialogue::getTitle() const
	{
		if (mTitle.length() > 0)
		{
			return mTitle.c_str();
		}
		return mId.c_str();
	}

	void Dialogue::setId(const char *id)
	{
		mId = id;
	}
	const char *Dialogue::getId() const
	{
		return mId.c_str();
	}

	void Dialogue::setSubject(const char *subject)
	{
		mSubject = subject;
	}
	const char *Dialogue::getSubject() const
	{
		if (mSubject.length() > 0)
		{
			return mSubject.c_str();
		}
		return mId.c_str();
	}

	void Dialogue::setUnlockFlag(Dialogue::UnlockFlag flag)
	{
		mUnlockFlag = flag;
	}
	Dialogue::UnlockFlag Dialogue::getUnlockFlag() const
	{
		return mUnlockFlag;
	}

	void Dialogue::setDialogueAction(DialogueAction action)
	{
		mAction = action;
	}
	Dialogue::DialogueAction Dialogue::getDialogueAction() const
	{
		return mAction;
	}

	bool Dialogue::addDialogue(Dialogue *dialogue)
	{
		if (dialogue)
		{
			string id = dialogue->getId();
			DialogueMap::const_iterator iter = sDialogueMap.find(id);
			if (iter == sDialogueMap.end())
			{
				sDialogueMap[id] = dialogue;
				return true;
			}
		}
		return false;
	}
	bool Dialogue::removeDialogue(const char *id)
	{
		if (id != NULL)
		{
			DialogueMap::const_iterator iter = sDialogueMap.find(string(id));
			if (iter == sDialogueMap.end())
			{
				sDialogueMap.erase(iter);
				return true;
			}
		}
		return false;
	}
	void Dialogue::removeAllDialogue()
	{
		sDialogueMap.clear();
	}
	Dialogue *Dialogue::getDialogue(const char *id)
	{
		if (id != NULL)
		{
			DialogueMap::iterator iter = sDialogueMap.find(string(id));
			if (iter != sDialogueMap.end())
			{
				return iter->second;
			}
		}
		return NULL;
	}

	void Dialogue::getAvailableDialogues(vector<Dialogue *> &result, const GameObject *talker, const GameObject *talkedTo)
	{
		if (talker == NULL || talkedTo == NULL || 
			talker->getDialogueComp() == NULL || talkedTo->getDialogueComp() == NULL)
		{
			return;
		}

		const Character::SubjectMap &unlocked = talker->getDialogueComp()->getUnlockedSubjects();
		const Character::SubjectMap &available = talkedTo->getDialogueComp()->getDialoguesAvailable();
		Character::SubjectMap::const_iterator iter;
		for (iter = available.begin(); iter != available.end(); ++iter)
		{
			Dialogue *dialogue = getDialogue(iter->first.c_str());
			if (dialogue == NULL)
			{
				continue;
			}
			if (dialogue->getUnlockFlag() == UNLOCK_NONE)
			{
				result.push_back(dialogue);
			}
			else if (dialogue->getUnlockFlag() == UNLOCK_LOCKED)
			{
				// If the subject of the dialogue is in the unlocked map and is true.
				Character::SubjectMap::const_iterator iter = unlocked.find(string(dialogue->getSubject()));
				if (iter != unlocked.end() && !iter->second)
				{
					result.push_back(dialogue);
				}
			}
		}
	}

	Dialogue::UnlockFlag Dialogue::toUnlockFlag(const char *flag)
	{
		if (strcmp("none", flag) == 0)
		{
			return UNLOCK_NONE;
		}
		if (strcmp("locked", flag) == 0)
		{
			return UNLOCK_LOCKED;
		}
		return UNLOCK_UNKNOWN;
	}
	Dialogue::UnlockFlag Dialogue::toUnlockFlag(int flagValue)
	{
		if (flagValue < 0 || flagValue >= UNLOCK_UNKNOWN)
		{
			return UNLOCK_UNKNOWN;
		}
		return static_cast<UnlockFlag>(flagValue);
	}
	const char *Dialogue::getUnlockFlagName(Dialogue::UnlockFlag flag)
	{
		if (flag == UNLOCK_NONE)
		{
			return "none";
		}
		if (flag == UNLOCK_LOCKED)
		{
			return "locked";
		}
		return "unknown";
	}

	Dialogue::DialogueAction Dialogue::toDialogueAction(const char *action)
	{
		if (strcmp("none", action) == 0)
		{
			return ACTION_NONE;
		}
		if (strcmp("shop", action) == 0)
		{
			return ACTION_SHOP;
		}
		if (strcmp("close", action) == 0)
		{
			return ACTION_CLOSE;
		}
		return ACTION_UNKNOWN;
	}
	Dialogue::DialogueAction Dialogue::toDialogueAction(int actionValue)
	{
		if (actionValue < 0 || actionValue >= ACTION_UNKNOWN)
		{
			return ACTION_UNKNOWN;
		}
		return static_cast<DialogueAction>(actionValue);
	}
	const char *Dialogue::getDialogueActionName(Dialogue::DialogueAction action)
	{
		if (action == Dialogue::ACTION_NONE)
		{
			return "none";
		}
		if (action == Dialogue::ACTION_CLOSE)
		{
			return "close";
		}
		if (action == Dialogue::ACTION_SHOP)
		{
			return "shop";
		}
		return "unknown";
	}
	
}
}
