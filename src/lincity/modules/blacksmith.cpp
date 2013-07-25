/* ---------------------------------------------------------------------- *
 * blacksmith.c
 * This file is part of lincity.
 * Lincity is copyright (c) I J Peters 1995-1997, (c) Greg Sharp 1997-2001.
 * (c) Corey Keasling, 2004
 * ---------------------------------------------------------------------- */

#include "blacksmith.h"

BlacksmithConstructionGroup blacksmithConstructionGroup(
    "Blacksmith",
    FALSE,                     /* need credit? */
    GROUP_BLACKSMITH,
    2,                         /* size */
    GROUP_BLACKSMITH_COLOUR,
    GROUP_BLACKSMITH_COST_MUL,
    GROUP_BLACKSMITH_BUL_COST,
    GROUP_BLACKSMITH_FIREC,
    GROUP_BLACKSMITH_COST,
    GROUP_BLACKSMITH_TECH,
    GROUP_BLACKSMITH_RANGE
);

Construction *BlacksmithConstructionGroup::createConstruction(int x, int y, unsigned short type) {
    return new Blacksmith(x, y, type);
}

void Blacksmith::update()
{      
    //monthly update   
    if (total_time % 100 == 0)
    {
        productivity = workingdays;
        workingdays = 0;
    }    
    if (pauseCounter++ < 0)
        return;
    if ((commodityCount[STUFF_GOODS] < constructionGroup->commodityRuleCount[STUFF_GOODS].maxload - GOODS_MADE_BY_BLACKSMITH) 
        && (commodityCount[STUFF_COAL] >= BLACKSMITH_COAL_USED)
        && (commodityCount[STUFF_STEEL] >= BLACKSMITH_STEEL_USED)
        && (commodityCount[STUFF_JOBS] >= BLACKSMITH_JOBS))
    {
        commodityCount[STUFF_GOODS] += GOODS_MADE_BY_BLACKSMITH;
        commodityCount[STUFF_COAL] -= BLACKSMITH_COAL_USED;
        commodityCount[STUFF_STEEL] -= BLACKSMITH_STEEL_USED;
        commodityCount[STUFF_JOBS] -= BLACKSMITH_JOBS;
        workingdays++;
        if ((goods_made += GOODS_MADE_BY_BLACKSMITH) >= BLACKSMITH_BATCH)
        {        
            animate = true;
            world(x,y)->pollution++;
            goods_made = 0;        
        }        
    }
    else
    {
        type = CST_BLACKSMITH_0;
        pauseCounter = -BLACKSMITH_CLOSE_TIME;
        return;
    }
    //animation
    if (animate && real_time > anim) {
        anim = real_time + BLACKSMITH_ANIM_SPEED;
        switch (type) {
        case (CST_BLACKSMITH_0):
            type = CST_BLACKSMITH_1;
            break;
        case (CST_BLACKSMITH_1):
            type = CST_BLACKSMITH_2;
            break;
        case (CST_BLACKSMITH_2):
            type = CST_BLACKSMITH_3;
            break;
        case (CST_BLACKSMITH_3):
            type = CST_BLACKSMITH_4;
            break;
        case (CST_BLACKSMITH_4):
            type = CST_BLACKSMITH_5;
            break;
        case (CST_BLACKSMITH_5):
            type = CST_BLACKSMITH_6;
            break;
        case (CST_BLACKSMITH_6):
            type = CST_BLACKSMITH_1;
            animate = false;
            break;
        }
    }
    
}

void Blacksmith::report()
{
    int i = 0;

    mps_store_sd(i++, constructionGroup->name,ID);
    i++;
    mps_store_sfp(i++, _("busy"), (float) productivity);
    i++;
    list_commodities(&i);
}

/** @file lincity/modules/blacksmith.cpp */

