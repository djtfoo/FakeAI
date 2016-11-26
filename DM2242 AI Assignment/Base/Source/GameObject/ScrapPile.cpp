#include "ScrapPile.h"

ScrapPile::ScrapPile() : GameObject("Scrap Pile"), m_scrapQuantity(0)
{
}

ScrapPile::~ScrapPile()
{

}

void ScrapPile::Init()
{
    m_scrapQuantity = 3;
}

void ScrapPile::Update(double dt)
{
    if (m_scrapQuantity == 0)
        SetInactive();

    else if (m_scrapQuantity > 0)
        SetActive();
}

int ScrapPile::GetScrapQuantity()
{
    return m_scrapQuantity;
}

void ScrapPile::SetScrapQuantity(int quantity)
{
    m_scrapQuantity = quantity;
}