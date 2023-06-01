#include "stdafx.h"
#include "TerrainSlice.h"

TerrainSlice::TerrainSlice(int amountOfObstacles, int maxWidth, UINT textureID)
	: m_pSlice(nullptr)
	, m_NrObstacles(amountOfObstacles)
	, m_MaxWidth(maxWidth)
	, m_TextureID(textureID)
{
}

TerrainSlice::~TerrainSlice()
{

}
