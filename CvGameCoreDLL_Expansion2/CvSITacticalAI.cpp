#include "CvGameCoreDLLPCH.h"
#include "CvPlayerAI.h"
#include "CvRandom.h"
#include "CvGlobals.h"
#include "CvGameCoreUtils.h"
#include "CvSITacticalAI.h"

// Include this after all other headers.
#include "LintFree.h"

// public functions

CvSITacticalAI::CvSITacticalAI(void)
{

}


CvSITacticalAI::~CvSITacticalAI(void)
{

}

/// Log current status of the operation
void CvSITacticalAI::LogTacticalMessage(CvString& strMsg, bool bSkipLogDominanceZone)
{
	if(GC.getLogging() && GC.getAILogging())
	{
		CvString strOutBuf;
		CvString strBaseString;
		CvString strPlayerName;
		CvString strTemp;
		FILogFile* pLog;

		// Don't want Dominance Zone logging
		bSkipLogDominanceZone = true;

		strPlayerName = m_pPlayer->getCivilizationShortDescription();
		pLog = LOGFILEMGR.GetLog("PlayerSITacticalAILog_Portugal.csv", FILogFile::kDontTimeStamp);

		// Get the leading info for this line
		strBaseString.Format("Turn: %03d, ", GC.getGame().getElapsedGameTurns());
		strBaseString += strPlayerName + ", ";
		strOutBuf = strBaseString + strMsg;

		pLog->Msg(strOutBuf);
	}
}