/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#pragma once

#ifndef CIV5_INFLUENCE_MAP_H
#define CIV5_INFLUENCE_MAP_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvInfluenceMap
//!  \brief		Used to calculate the relative Influence of a given plot for a player
//
//!  Key Attributes:
//!  - Replaces the AI_getPlotInfluence function in CvPlayerAI
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvInfluenceMap
{
public:
	CvInfluenceMap(void);
	~CvInfluenceMap(void);

	void Init(PlayerTypes ePlayer);
	void Uninit();
	void Reset();

	void PlaceInfluence();
	void Propagate(int iPlotX, int iPlotY, int iRange, int iValue);
	void UpdateInfluencePlot(int iPlotX, int iPlotY, int iValue);
	void AddInfluence(int iPlotX, int iPlotY, int iValue);
	int GetInfluence(const CvPlot& pPlot) const;
	void LogInfluenceMessage(CvString& strMsg);
	void DisplayInfluenceStatus();

private:
	PlayerTypes m_ePlayer;
	FFastVector<uint, true, c_eCiv5GameplayDLL, 0> m_InfluencePlots;

};

#endif //CIV5_PROJECT_CLASSES_H