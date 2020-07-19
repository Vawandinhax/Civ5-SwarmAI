/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#include "CvGameCoreDLLPCH.h"
#include "CvInfluenceMap.h"
#include "CvGameCoreUtils.h"
#include "CvAStar.h"
#include "CvEnumSerialization.h"
#include "CvDiplomacyAI.h"
#include "CvMilitaryAI.h"
#include "CvMinorCivAI.h"
#include "FireWorks/FRemark.h"
#include "CvUnit.h"

// must be included after all other headers
#include "LintFree.h"
#ifdef _MSC_VER
#pragma warning ( disable : 4505 ) // unreferenced local function has been removed.. needed by REMARK below
#endif//_MSC_VER

REMARK_GROUP("CvInfluenceMap");

/// Constructor
CvInfluenceMap::CvInfluenceMap(void)
	: m_ePlayer(NO_PLAYER)
{

}

/// Destructor
CvInfluenceMap::~CvInfluenceMap(void)
{
	Uninit();
}

/// Initialize
void CvInfluenceMap::Init(PlayerTypes ePlayer)
{
	CvString strMsg;
	strMsg.Format("Clearing Cached Maps");
	LogInfluenceMessage(strMsg);	

	//m_InfluencePlots.clear();

	//Uninit();
	//m_ePlayer = ePlayer;

	strMsg.Format("Initializing Influence Map");
	LogInfluenceMessage(strMsg);		

	int iGridSize = GC.getMap().numPlots();

	strMsg.Format("Total Plots: %d", iGridSize);
	LogInfluenceMessage(strMsg);	

	//m_InfluencePlots.resize(iGridSize);

	strMsg.Format("Array size: %d", m_InfluencePlots.size());
	LogInfluenceMessage(strMsg);

	for(int i = 0; i < iGridSize; i++)
	{	
		strMsg.Format("Initializing Influence Map: %d out of %d", i, iGridSize);
		LogInfluenceMessage(strMsg);		
		m_InfluencePlots[i] = 0;
	}

	strMsg.Format("Finished Initializing Influence Map. Current Status:");
	LogInfluenceMessage(strMsg);
	DisplayInfluenceStatus();
}

/// Uninitialize
void CvInfluenceMap::Uninit()
{
	m_ePlayer = NO_PLAYER;
	m_InfluencePlots.clear();
}

/// Update general info on the map
void CvInfluenceMap::PlaceInfluence()
{	
	UnitHandle pLoopUnit;
	int iLoop;
	CvString strMsg;
	CvString playername;
	CvString playerportugal;
	playerportugal.Format("Portugal");

	strMsg.Format("Finding out who Portugal is...");
	LogInfluenceMessage(strMsg);	

	// Cheap trick to surpass the fact "m_ePlayer = ePlayer;" on Init() was crashing
	for(int iPlayer = 0; iPlayer < MAX_PLAYERS; iPlayer++)
	{
		PlayerTypes ePlayer = (PlayerTypes)iPlayer;
		CvPlayer& loopPlayer = GET_PLAYER(ePlayer);
		playername = loopPlayer.getCivilizationShortDescription();
		if(playername.compare(playerportugal) == 0)
		{	
			m_ePlayer = ePlayer;
		}
	}
	
	CvPlayer& m_pPlayer = GET_PLAYER(m_ePlayer);
	strMsg.Format("Initializing Influence Update - Updating for Friendly Units");
	LogInfluenceMessage(strMsg);	

	// Position Allied Units in the Influence Map
	for(pLoopUnit = m_pPlayer.firstUnit(&iLoop); pLoopUnit; pLoopUnit = m_pPlayer.nextUnit(&iLoop))
	{
		UpdateInfluencePlot(pLoopUnit->getX(), pLoopUnit->getY(), SI_INFLUENCE_UNIT);
	}
	
	strMsg.Format("Influence Update - Updating for Allied City");
	LogInfluenceMessage(strMsg);	

	// Position Allied City in the Influence Map
	CvCity* m_pCity = m_pPlayer.getCapitalCity();
	UpdateInfluencePlot(m_pCity->plot()->getX(), m_pCity->plot()->getY(), SI_INFLUENCE_CITY);

	strMsg.Format("Influence Update - Updating for Enemy Units");
	LogInfluenceMessage(strMsg);	

	// loop through enemy players and position Enemy Units in the Influence Map
	for(int iPlayer = 0; iPlayer < MAX_PLAYERS; iPlayer++)
	{	

		// If the player has already been added, skip it
		PlayerTypes ePlayer = (PlayerTypes)iPlayer;
		if(m_ePlayer == ePlayer)
		{	
			continue;
		}

		CvPlayer& loopPlayer = GET_PLAYER(ePlayer);
		for(pLoopUnit = loopPlayer.firstUnit(&iLoop); pLoopUnit; pLoopUnit = loopPlayer.nextUnit(&iLoop))
		{
			UpdateInfluencePlot(pLoopUnit->getX(), pLoopUnit->getY(), SI_INFLUENCE_ENEMY_UNIT);
		}
	}

	strMsg.Format("Finished Updating, Displaying Influence Status");
	LogInfluenceMessage(strMsg);
	DisplayInfluenceStatus();

}

/// Propagate the Influence Plot
void CvInfluenceMap::Propagate(int iPlotX, int iPlotY, int iRange, int iValue)
{
	int iDistance;
	int iPropagatedValue;
	//CvMap& kMap = GC.getMap();

	for(int iX = -iRange; iX <= iRange; iX++)
	{
		for(int iY = -iRange; iY <= iRange; iY++)
		{
			//CvPlot* pPlot = kMap.plot(iPlotX + iX, iPlotY + iY);
			iDistance = plotDistance(iPlotX, iPlotY, iPlotX + iX, iPlotY + iY);
			iPropagatedValue = iValue - ((iDistance * SI_INFLUENCE_DECAY) * iValue);
			AddInfluence(iPlotX + iX, iPlotY + iY, iValue);
		}
	}

}

/// Updates the Influence plot
void CvInfluenceMap::UpdateInfluencePlot(int iPlotX, int iPlotY, int iValue)
{
	const int idx = iPlotX + iPlotY * GC.getMap().getGridWidth();
	m_InfluencePlots[idx] = iValue;
}

/// Add an amount of influence to a given tile
void CvInfluenceMap::AddInfluence(int iPlotX, int iPlotY, int iValue)
{
	const int idx = iPlotX + iPlotY * GC.getMap().getGridWidth();
	m_InfluencePlots[idx] += iValue;
}

/// Return the Influence value of a given plot
int CvInfluenceMap::GetInfluence(const CvPlot& pPlot) const
{
	const int idx = pPlot.getX() + pPlot.getY() * GC.getMap().getGridWidth();
	return m_InfluencePlots[idx];
}


// Write to InfluenceMap.csv the array status in a user-friendly way
void CvInfluenceMap::DisplayInfluenceStatus()
{

	int iX;
	int iY;
	int iInfluenceValue;
	int iNumPlots = GC.getMap().numPlots();
	int iGridWidth = GC.getMap().getGridWidth();
	int iGridHeigth = GC.getMap().getGridHeight();
	CvString strHeader;
	CvString strBody;
	CvString strAux;
	CvString strHelp;

	strHelp.Format("Grid Width: %d, Grid Heigth: %d, Total Plots: %d", iGridWidth, iGridHeigth, iNumPlots);
	LogInfluenceMessage(strHelp);

	for(iX = iGridWidth - 1; iX >= 0; iX --)
	{	
		strBody.Format("");
		for(iY = 0; iY < iGridHeigth; iY ++)
		{
			iInfluenceValue = m_InfluencePlots[iX + iY * iGridWidth];
			strAux.Format("%02d  ,", iInfluenceValue);
			strBody += strAux;

			//strHelp.Format("Plot - X: %d, Y: %d, Influence Value: %d", iX, iY, iInfluenceValue);
			//LogInfluenceMessage(strHelp);
		}

		LogInfluenceMessage(strBody);		
	}
}

// Writes on file InfluenceMap.csv for debugging purposes
void CvInfluenceMap::LogInfluenceMessage(CvString& strMsg)
{
	CvString strOutBuf;
	CvString strBaseString;
	FILogFile* pLog;

	pLog = LOGFILEMGR.GetLog("InfluenceMap.csv", FILogFile::kDontTimeStamp);
	strBaseString.Format("Influence Status - Turn: %03d, ", GC.getGame().getElapsedGameTurns());
	strOutBuf = strBaseString + strMsg;

	pLog->Msg(strOutBuf);
	
}