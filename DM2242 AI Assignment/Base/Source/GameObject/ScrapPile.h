#ifndef SCRAPPILE_H
#define SCRAPPILE_H

#include "Workstation.h"

class ScrapPile : public Workstation
{
    int m_scrapQuantity;    // how much scrap there is in the pile; if 0, don't render

public:
    ScrapPile();
    virtual ~ScrapPile();

    virtual void Init();
    virtual void Update(double dt);

    int GetScrapQuantity();
    void SetScrapQuantity(int quantity);
};

#endif