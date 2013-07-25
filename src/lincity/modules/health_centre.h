#define GROUP_HEALTH_COLOUR   (green(24))
#define GROUP_HEALTH_COST     100000
#define GROUP_HEALTH_COST_MUL 2
#define GROUP_HEALTH_BUL_COST     1000
#define GROUP_HEALTH_TECH     110
#define GROUP_HEALTH_FIREC 2
#define GROUP_HEALTH_RANGE 14

#define HEALTH_CENTRE_JOBS   6
#define HEALTH_CENTRE_GET_JOBS 7
#define MAX_JOBS_AT_HEALTH_CENTRE (20 * HEALTH_CENTRE_JOBS * DAYS_BETWEEN_COVER)
#define HEALTH_CENTRE_GOODS  40
#define HEALTH_CENTRE_GET_GOODS 41
#define MAX_GOODS_AT_HEALTH_CENTRE (20 * HEALTH_CENTRE_GOODS * DAYS_BETWEEN_COVER)
#define MAX_WASTE_AT_HEALTH_CENTRE (20 * HEALTH_CENTRE_GOODS * DAYS_BETWEEN_COVER /3)
#define HEALTH_RUNNING_COST  2
#define HEALTH_RUNNING_COST_MUL 9



#include "modules.h"
#include "../lintypes.h"
#include "../lctypes.h"
//#include "../range.h"

class HealthCentreConstructionGroup: public ConstructionGroup {
public:
    HealthCentreConstructionGroup(
        const char *name,
        unsigned short no_credit,
        unsigned short group,
        unsigned short size, int colour,
        int cost_mul, int bul_cost, int fire_chance,
        int cost, int tech, int range
    ): ConstructionGroup(
        name, no_credit, group, size, colour, cost_mul, bul_cost, fire_chance, cost, tech, range
    ) {
        commodityRuleCount[Construction::STUFF_JOBS].maxload = MAX_JOBS_AT_HEALTH_CENTRE;
        commodityRuleCount[Construction::STUFF_JOBS].take = true;
        commodityRuleCount[Construction::STUFF_JOBS].give = false;
        commodityRuleCount[Construction::STUFF_GOODS].maxload = MAX_GOODS_AT_HEALTH_CENTRE;
        commodityRuleCount[Construction::STUFF_GOODS].take = true;
        commodityRuleCount[Construction::STUFF_GOODS].give = false;
        commodityRuleCount[Construction::STUFF_WASTE].maxload = MAX_WASTE_AT_HEALTH_CENTRE;
        commodityRuleCount[Construction::STUFF_WASTE].take = false;
        commodityRuleCount[Construction::STUFF_WASTE].give = true;           
    }
    // overriding method that creates a HealthCentre
    virtual Construction *createConstruction(int x, int y, unsigned short type);
};

extern HealthCentreConstructionGroup healthCentreConstructionGroup;

class HealthCentre: public CountedConstruction<HealthCentre> { // HealthCentre inherits from its own CountedConstruction
public:
	HealthCentre(int x, int y, unsigned short type): CountedConstruction<HealthCentre>(x, y, type)
    {           
        constructionGroup = &healthCentreConstructionGroup;   
        this->busy = false;
        initialize_commodities();
        
        int tmp;
        int lenm1 = world.len()-1;
        tmp = x - constructionGroup->range;
        this->xs = (tmp < 1) ? 1 : tmp;         
        tmp = y - constructionGroup->range;
        this->ys = (tmp < 1)? 1 : tmp; 
        tmp = x + constructionGroup->range + constructionGroup->size;
        this->xe = (tmp > lenm1) ? lenm1 : tmp;         
        tmp = y + constructionGroup->range + constructionGroup->size;
        this->ye = (tmp > lenm1)? lenm1 : tmp;
	}
	virtual ~HealthCentre() { }
	virtual void update();
	virtual void report();
    void cover();	

	int xs, ys, xe, ye;
    bool busy;
};

/** @file lincity/modules/health_centre.h */

