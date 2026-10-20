#include "pb_goalfinder.h"
//#include "pb_goals.h"
#include "parabot.h"
#include "zz_trace.h"




PB_GoalFinder::~PB_GoalFinder()
{
	knownGoals.clear();
}


void PB_GoalFinder::init( CParabot *pb )
// initializes all necessary variables
{
	bot = pb;
#ifdef DEBUG
	bot->goalMove[0] = 0;
	bot->goalView[0] = 0;
	bot->goalAct[0] = 0;
#endif
	for (int i=0; i<MAX_GOALS; i++) { 
		bestGoalFunction[i] = 0;
		responsiblePercept[i] = 0;
		bestWeight[i] = 0;
	}
}


void PB_GoalFinder::addGoal( int goalClass, int triggerId, tGoalFunc gf, tWeightFunc wf )
{
	tGoal g;
	g.type = goalClass;
	g.goal = gf;
	g.weight = wf;
	knownGoals.insert( std::make_pair(triggerId, g) );
#ifdef PB_TRACE
	{
		static FILE *fp = 0;
		if (!fp) fp = fopen( "/tmp/pb_goals.log", "a" );
		if (fp) { fprintf( fp, "ADD trigger=%d type=%d size=%d\n", triggerId, goalClass, (int)knownGoals.size() ); fflush( fp ); }
	}
#endif
}


void PB_GoalFinder::analyze( PB_Perception &senses )
{
	float weight;
	int   type;
	tPerceptionList *perception = senses.perceptionList();
	tPerceptionList::iterator pli = perception->begin();

	while ( pli != perception->end() ) {
		/*PB_Percept dmgp = *pli;
		if ( pli->pClass == PI_DAMAGE ) {
			debugMsg( "damage sensed\n" );
		}*/
		if ( worldTime() >= pli->update ) {
#ifdef PB_TRACE
			pbTrace( "t=%.2f ANLZ bot=%d pclass=%d upd=%.2f vis=%d\n",
				worldTime(), bot->slot, (int)pli->pClass, pli->update, pli->isVisible() ? 1 : 0 );
#endif
			// rating only necessary for enemies:
			if (pli->pClass==PI_FOE) {
				assert (pli->entity != 0);
				// check if enemy can see the bot
				UTIL_MakeVectors( pli->entity->v.v_angle );
				Vector dir = (bot->botPos() - pli->entity->v.origin).Normalize();
				pli->orientation = DotProduct( gpGlobals->v_forward, dir );
				// weapon rating ( -5 .. +5 )
				if (pli->hasBeenSpotted()) pli->rating = bot->combat.getRating( (*pli) ); 
				else pli->rating = 0;
				
				//debugMsg( "Rating %s = %f\n", STRING(pli->entity->v.netname), pli->rating );
				//float dst = (bot->botPos() - pli->entity->v.origin).Length();
				//float hp = bot->action.targetAccuracy();
				//debugMsgF("HitProb = %.2f\n", hp );
			}	
			// NB: multimap::find() may return any element with the key, not
			// necessarily the first. Iterating from it with the old
			// find()+do/while walk skipped most goals for the key on libc++
			// (macOS), so bots never considered shooting their enemies there.
			std::pair<tGoalList::iterator, tGoalList::iterator> range = knownGoals.equal_range( pli->pClass );
			for ( tGoalList::iterator gli = range.first; gli != range.second; gli++ ) {
				//debugMsg( "." );
				assert( gli->first == pli->pClass );
				type = gli->second.type;
				tWeightFunc wf = gli->second.weight;
				if (wf) weight = (*wf)( bot, &(*pli) );
				else weight = -100;

				if (weight > bestWeight[type]) {
					bestGoalFunction[type] = gli->second.goal;
					responsiblePercept[type] = &(*pli);
					assert( responsiblePercept[type]->pClass > 0 && responsiblePercept[type]->pClass <= MAX_PERCEPTION );
					bestWeight[type] = weight;
				}
#ifdef PB_TRACE
				pbTrace( "t=%.2f  W bot=%d pclass=%d type=%d w=%.2f best=%.2f\n",
					worldTime(), bot->slot, (int)pli->pClass, type, weight, bestWeight[type] );
#endif
			}
			//debugMsg( "\n" );
		}
		pli++;
		//check();
	}
	//check();
}


void PB_GoalFinder::analyzeUnconditionalGoals()
{
	float weight;
	int   type;
	tGoalList::iterator gli = knownGoals.begin();

	while ( gli != knownGoals.end() ) {
		if (gli->first == GOAL_UNCONDITIONAL) {
			type = gli->second.type;
			tWeightFunc wf = gli->second.weight;
			if (wf) weight = (*wf)( bot, 0 );
			else weight = -100;

			if (weight > bestWeight[type]) {
				bestGoalFunction[type] = gli->second.goal;
				responsiblePercept[type] = 0;
				bestWeight[type] = weight;
			}
		}
		gli++;
	}
}


void PB_GoalFinder::synchronize()
// deletes goals that can't be executed at the same time
{
	if ( bestWeight[G_MOVE|G_VIEW] > (bestWeight[G_MOVE]+bestWeight[G_VIEW]) ) {
		bestGoalFunction[G_MOVE] = 0;
		responsiblePercept[G_MOVE] = 0;
		bestGoalFunction[G_VIEW] = 0;
		responsiblePercept[G_VIEW] = 0;
	}
	else {
		bestGoalFunction[G_MOVE|G_VIEW] = 0;
		responsiblePercept[G_MOVE|G_VIEW] = 0;
	}
}


void PB_GoalFinder::check()
{
	for (int i=0; i<MAX_GOALS; i++) {
		if (responsiblePercept[i] != 0) {
			assert( responsiblePercept[i]->pClass > 0 && responsiblePercept[i]->pClass <= MAX_PERCEPTION );
		}
	}
}
