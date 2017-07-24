#include "MineField.h"
#include<assert.h>
#include<random>
#include"SpriteCodex.h"

bool MineField::Tile::IsFlagged() const
{
	return state == State::flagged;
}

bool MineField::Tile::IsRevealed() const
{
	return (state == State::revealed);
}

bool MineField::Tile::IsHidden() const
{
	return state == State::hidden;
}

void MineField::Tile::Reveal()
{
	assert(IsHidden());
	assert(!IsRevealed());
	state = State::revealed;
}

void MineField::Tile::ToggleFlag()
{
	assert(IsHidden());
	assert(!IsFlagged());
	state = State::flagged;
}

bool MineField::Tile::HasMine() const
{
	return hasMine;
}

void MineField::Tile::SpawnMine()
{
	assert(hasMine == false);
	hasMine = true;
}

void MineField::Tile::Draw(const Vec2i & screenPos, Graphics & gfx) const
{
	switch (state)
	{
	case State::hidden:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		break;
	case State::flagged:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		break;
	case State::revealed:
		if (HasMine() == true)
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		else
			SpriteCodex::DrawTile0(screenPos, gfx);
		break;
	}
}

MineField::Tile & MineField::TileAt(Vec2i gridPos)
{
	return field[gridPos.x + width * gridPos.y];
	// TODO: insert return statement here
}

const MineField::Tile & MineField::TileAt(Vec2i gridPos)const
{
	return field[gridPos.x + width * gridPos.y];
	// TODO: insert return statement here
}

MineField::MineField(int nMines)
{
	assert(nMines > 0 && nMines < width * height);

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int>xDist(0, width - 1);
	std::uniform_int_distribution<int>yDist(0, height - 1);

	for (int nSpawns = 0; nSpawns < nMines; nSpawns++)
	{
		Vec2i spawnPos;
		do
		{
			spawnPos = Vec2i(xDist(rng), yDist(rng));
		} while (TileAt(spawnPos).HasMine() == true);

		TileAt(spawnPos).SpawnMine();
	}
}

RectI MineField::GetRect(Vec2i offset) const
{
	return RectI(offset + Vec2i(0, 0), offset + Vec2i(width * SpriteCodex::tileSize, height * SpriteCodex::tileSize));
}

void MineField::Draw(const Vec2i& offset,Graphics & gfx) const
{
	RectI rect = GetRect(offset);
	gfx.DrawRect(rect, SpriteCodex::baseColor);
	
	Vec2i gridPos =  Vec2i(0, 0);
	for (; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width ; gridPos.x++)
		{
			TileAt(gridPos).Draw(offset + gridPos * SpriteCodex::tileSize,gfx);
		}
	}
}

Vec2i MineField::ScreenToGrid(const Vec2i & offset, Vec2i & screenPos) const
{
	screenPos -= offset;

	return screenPos / SpriteCodex::tileSize;
}

void MineField::RevealTile(const Vec2i & offset, Vec2i & screenPos)
{
	MineField::Tile& tile = TileAt(ScreenToGrid(offset, screenPos));
	if (tile.IsHidden() && !tile.IsRevealed())
		tile.Reveal();
}

void MineField::MarkFlag(const Vec2i & offset, Vec2i & screenPos)
{
	MineField::Tile& tile = TileAt(ScreenToGrid(offset, screenPos));
	if (tile.IsHidden() && !tile.IsFlagged())
		tile.ToggleFlag();
}

void MineField::Test(int testCases)
{
	assert(testCases > 0 && testCases < height * width);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int>xDist(0, width - 1);
	std::uniform_int_distribution<int>yDist(0, height - 1);

	Vec2i spawnPos;
	for (int i = 0; i < testCases; i++)
	{
		spawnPos = Vec2i(xDist(rng), yDist(rng));
		if (!TileAt(spawnPos).IsRevealed())
			TileAt(spawnPos).Reveal();
	}
}
